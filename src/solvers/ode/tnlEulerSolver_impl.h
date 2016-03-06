/***************************************************************************
                          tnlEulerSolver_impl.h  -  description
                             -------------------
    begin                : Mar 10, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef tnlEulerSolver_implH
#define tnlEulerSolver_implH

#ifdef HAVE_CUDA
template< typename RealType, typename Index >
__global__ void updateUEuler( const Index size,
                              const RealType tau,
                              const RealType* k1,
                              RealType* u,
                              RealType* cudaBlockResidue );
#endif


template< typename Problem >
tnlEulerSolver< Problem > :: tnlEulerSolver()
: cflCondition( 0.0 )
{
   timer.reset();
   updateTimer.reset();
};

template< typename Problem >
tnlString tnlEulerSolver< Problem > :: getType() const
{
   return tnlString( "tnlEulerSolver< " ) +
          Problem :: getTypeStatic() +
          tnlString( " >" );
};

template< typename Problem >
void tnlEulerSolver< Problem > :: configSetup( tnlConfigDescription& config,
                                               const tnlString& prefix )
{
   //tnlExplicitSolver< Problem >::configSetup( config, prefix );
   config.addEntry< double >( prefix + "euler-cfl", "Coefficient C in the Courant–Friedrichs–Lewy condition.", 0.0 );
};

template< typename Problem >
bool tnlEulerSolver< Problem > :: setup( const tnlParameterContainer& parameters,
                                        const tnlString& prefix )
{
   tnlExplicitSolver< Problem >::setup( parameters, prefix );
   if( parameters.checkParameter( prefix + "euler-cfl" ) )
      this->setCFLCondition( parameters.getParameter< double >( prefix + "euler-cfl" ) );
   return true;
}

template< typename Problem >
void tnlEulerSolver< Problem > :: setCFLCondition( const RealType& cfl )
{
   this->cflCondition = cfl;
}

template< typename Problem >
const typename Problem :: RealType& tnlEulerSolver< Problem > :: getCFLCondition() const
{
   return this->cflCondition;
}

template< typename Problem >
bool tnlEulerSolver< Problem > :: solve( DofVectorType& u )
{
   /****
    * First setup the supporting meshes k1...k5 and k_tmp.
    */
   timer.start();
   if( ! k1. setLike( u ) )
   {
      cerr << "I do not have enough memory to allocate a supporting grid for the Euler explicit solver." << endl;
      return false;
   }
   k1. setValue( 0.0 );


   /****
    * Set necessary parameters
    */
   RealType& time = this->time;
   RealType currentTau = Min( this->getTau(), this->getMaxTau() );
   if( time + currentTau > this->getStopTime() ) currentTau = this->getStopTime() - time;
   if( currentTau == 0.0 ) return true;
   this->resetIterations();
   this->setResidue( this->getConvergenceResidue() + 1.0 );

   this->refreshSolverMonitor();

   /****
    * Start the main loop
    */
   while( 1 )
   {
      /****
       * Compute the RHS
       */
      timer.stop();
      this->problem->getExplicitRHS( time, currentTau, u, k1 );
      timer.start();

      RealType lastResidue = this->getResidue();
      RealType maxResidue( 0.0 );
      if( this->cflCondition != 0.0 )
      {
         maxResidue = k1. absMax();
         if( currentTau * maxResidue > this->cflCondition )
         {
            currentTau *= 0.9;
            continue;
         }
      }
      RealType newResidue( 0.0 );
      updateTimer.start();
      computeNewTimeLevel( u, currentTau, newResidue );
      updateTimer.stop();
      this->setResidue( newResidue );

      /****
       * When time is close to stopTime the new residue
       * may be inaccurate significantly.
       */
      if( currentTau + time == this->stopTime ) this->setResidue( lastResidue );
      time += currentTau;

      if( ! this->nextIteration() )
         return false;

      /****
       * Compute the new time step.
       */
      if( time + currentTau > this->getStopTime() )
         currentTau = this->getStopTime() - time; //we don't want to keep such tau
      else this->tau = currentTau;

      this->refreshSolverMonitor();

      /****
       * Check stop conditions.
       */
      if( time >= this->getStopTime() ||
          ( this->getConvergenceResidue() != 0.0 && this->getResidue() < this->getConvergenceResidue() ) )
      {
         this->refreshSolverMonitor();
         std::cerr << std::endl << "RHS Timer = " << timer.getTime() << std::endl;
         std::cerr << std::endl << "Update Timer = " << updateTimer.getTime() << std::endl;
         return true;
      }

      if( this->cflCondition != 0.0 )
      {
         currentTau /= 0.95;
         currentTau = Min( currentTau, this->getMaxTau() );
      }
   }   
};

template< typename Problem >
void tnlEulerSolver< Problem > :: computeNewTimeLevel( DofVectorType& u,
                                                       RealType tau,
                                                       RealType& currentResidue )
{
   RealType localResidue = RealType( 0.0 );
   IndexType size = k1. getSize();
   RealType* _u = u. getData();
   RealType* _k1 = k1. getData();

   if( std::is_same< DeviceType, tnlHost >::value )
   {
#ifdef HAVE_OPENMP
#pragma omp parallel for reduction(+:localResidue) firstprivate( _u, _k1, tau ) if( tnlOmp::isEnabled() )
#endif
      for( IndexType i = 0; i < size; i ++ )
      {
         const RealType add = tau * _k1[ i ];
         _u[ i ] += add;
         localResidue += fabs( add );
      }
   }
   if( std::is_same< DeviceType, tnlCuda >::value )
   {
#ifdef HAVE_CUDA
      dim3 cudaBlockSize( 512 );
      const IndexType cudaBlocks = tnlCuda::getNumberOfBlocks( size, cudaBlockSize.x );
      const IndexType cudaGrids = tnlCuda::getNumberOfGrids( cudaBlocks );
      this->cudaBlockResidue.setSize( Min( cudaBlocks, tnlCuda::getMaxGridSize() ) );
      const IndexType threadsPerGrid = tnlCuda::getMaxGridSize() * cudaBlockSize.x;

      localResidue = 0.0;
      for( IndexType gridIdx = 0; gridIdx < cudaGrids; gridIdx ++ )
      {
         const IndexType sharedMemory = cudaBlockSize.x * sizeof( RealType );
         const IndexType gridOffset = gridIdx * threadsPerGrid;
         const IndexType currentSize = Min( size - gridOffset, threadsPerGrid );

         updateUEuler<<< cudaBlocks, cudaBlockSize, sharedMemory >>>( currentSize,
                                                                      tau,
                                                                      &_k1[ gridOffset ],
                                                                      &_u[ gridOffset ],
                                                                      this->cudaBlockResidue.getData() );
         localResidue += this->cudaBlockResidue.sum();
         cudaThreadSynchronize();
      }
#endif
   }
   localResidue /= tau * ( RealType ) size;
   :: MPIAllreduce( localResidue, currentResidue, 1, MPI_SUM, this->solver_comm );
}

#ifdef HAVE_CUDA
template< typename RealType, typename Index >
__global__ void updateUEuler( const Index size,
                              const RealType tau,
                              const RealType* k1,
                              RealType* u,
                              RealType* cudaBlockResidue )
{
   extern __shared__ RealType du[];
   const Index blockOffset = blockIdx. x * blockDim. x;
   const Index i = blockOffset  + threadIdx. x;
   if( i < size )
      u[ i ] += du[ threadIdx.x ] = tau * k1[ i ];
   else
      du[ threadIdx.x ] = 0.0;
   du[ threadIdx.x ] = fabs( du[ threadIdx.x ] );
   __syncthreads();

   const Index rest = size - blockOffset;
   Index n =  rest < blockDim.x ? rest : blockDim.x;

   computeBlockResidue( du,
                        cudaBlockResidue,
                        n );   
}
#endif

#endif
