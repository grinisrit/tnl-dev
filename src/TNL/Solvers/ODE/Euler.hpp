/***************************************************************************
                          Euler_impl.h  -  description
                             -------------------
    begin                : Mar 10, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Devices/MIC.h>
#include <TNL/Communicators/MpiCommunicator.h>
#include <TNL/Communicators/NoDistrCommunicator.h>

namespace TNL {
namespace Solvers {
namespace ODE {

#ifdef HAVE_CUDA
template< typename RealType, typename Index >
__global__ void updateUEuler( const Index size,
                              const RealType tau,
                              const RealType* k1,
                              RealType* u,
                              RealType* cudaBlockResidue );
#endif

template< typename Problem, typename SolverMonitor >
Euler< Problem, SolverMonitor > :: Euler()
: cflCondition( 0.0 )
{
};

template< typename Problem, typename SolverMonitor >
String Euler< Problem, SolverMonitor > :: getType()
{
   return String( "Euler< " ) +
          Problem :: getType() +
          String( " >" );
};

template< typename Problem, typename SolverMonitor >
void Euler< Problem, SolverMonitor > :: configSetup( Config::ConfigDescription& config,
                                               const String& prefix )
{
   //ExplicitSolver< Problem >::configSetup( config, prefix );
   config.addEntry< double >( prefix + "euler-cfl", "Coefficient C in the Courant–Friedrichs–Lewy condition.", 0.0 );
};

template< typename Problem, typename SolverMonitor >
bool Euler< Problem, SolverMonitor > :: setup( const Config::ParameterContainer& parameters,
                                        const String& prefix )
{
   ExplicitSolver< Problem, SolverMonitor >::setup( parameters, prefix );
   if( parameters.checkParameter( prefix + "euler-cfl" ) )
      this->setCFLCondition( parameters.getParameter< double >( prefix + "euler-cfl" ) );
   return true;
}

template< typename Problem, typename SolverMonitor >
void Euler< Problem, SolverMonitor > :: setCFLCondition( const RealType& cfl )
{
   this -> cflCondition = cfl;
}

template< typename Problem, typename SolverMonitor >
const typename Problem :: RealType& Euler< Problem, SolverMonitor > :: getCFLCondition() const
{
   return this -> cflCondition;
}

template< typename Problem, typename SolverMonitor >
bool Euler< Problem, SolverMonitor > :: solve( DofVectorPointer& _u )
{
   /****
    * First setup the supporting meshes k1...k5 and k_tmp.
    */
   //timer.start();
   _k1->setLike( *_u );
   auto k1 = _k1->getView();
   auto u = _u->getView();
   k1 = 0.0;


   /****
    * Set necessary parameters
    */
   RealType& time = this->time;
   RealType currentTau = min( this->getTau(), this->getMaxTau() );
   if( time + currentTau > this->getStopTime() ) currentTau = this->getStopTime() - time;
   if( currentTau == 0.0 ) return true;
   this->resetIterations();
   this->setResidue( this->getConvergenceResidue() + 1.0 );

   this -> refreshSolverMonitor();

   /****
    * Start the main loop
    */
   while( 1 )
   {
      /****
       * Compute the RHS
       */
      //timer.stop();
      this->problem->getExplicitUpdate( time, currentTau, _u, _k1 );
      //timer.start();

      RealType lastResidue = this->getResidue();
      RealType maxResidue( 0.0 );
      if( this -> cflCondition != 0.0 )
      {
         maxResidue = max( abs( k1 ) ); //k1->absMax();
         if( currentTau * maxResidue > this->cflCondition )
         {
            currentTau *= 0.9;
            continue;
         }
      }
      auto reduction = [] __cuda_callable__ ( RealType& a , const RealType& b ) { a += b; };
      auto volatileReduction = [] __cuda_callable__ ( volatile RealType& a , const volatile RealType& b ) { a += b; };
      this->setResidue( addAndReduceAbs( u, currentTau * k1, reduction, volatileReduction, ( RealType ) 0.0 ) / ( currentTau * ( RealType ) u.getSize() ) );

      /****
       * When time is close to stopTime the new residue
       * may be inaccurate significantly.
       */
      if( currentTau + time == this -> stopTime ) this->setResidue( lastResidue );
      time += currentTau;
      this->problem->applyBoundaryConditions( time, _u );

      if( ! this->nextIteration() )
         return this->checkConvergence();

      /****
       * Compute the new time step.
       */
      if( time + currentTau > this -> getStopTime() )
         currentTau = this -> getStopTime() - time; //we don't want to keep such tau
      else this -> tau = currentTau;

      this->refreshSolverMonitor();

      /****
       * Check stop conditions.
       */
      if( time >= this->getStopTime() ||
          ( this -> getConvergenceResidue() != 0.0 && this->getResidue() < this -> getConvergenceResidue() ) )
      {
         this -> refreshSolverMonitor();
         return true;
      }

      if( this -> cflCondition != 0.0 )
      {
         currentTau /= 0.95;
         currentTau = min( currentTau, this->getMaxTau() );
      }
   }
};

} // namespace ODE
} // namespace Solvers
} // namespace TNL