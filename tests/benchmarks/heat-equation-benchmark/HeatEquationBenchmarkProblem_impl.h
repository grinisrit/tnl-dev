#ifndef HeatEquationBenchmarkPROBLEM_IMPL_H_
#define HeatEquationBenchmarkPROBLEM_IMPL_H_

#include <core/mfilename.h>
#include <matrices/tnlMatrixSetter.h>
#include <solvers/pde/tnlExplicitUpdater.h>
#include <solvers/pde/tnlLinearSystemAssembler.h>
#include <solvers/pde/tnlBackwardTimeDiscretisation.h>

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
tnlString
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
getTypeStatic()
{
   return tnlString( "HeatEquationBenchmarkProblem< " ) + Mesh :: getTypeStatic() + " >";
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
tnlString
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
getPrologHeader() const
{
   return tnlString( "Heat Equation Benchmark" );
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
void
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
writeProlog( tnlLogger& logger, const tnlParameterContainer& parameters ) const
{
   /****
    * Add data you want to have in the computation report (log) as follows:
    * logger.writeParameter< double >( "Parameter description", parameter );
    */
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
bool
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
setup( const tnlParameterContainer& parameters )
{
   if( ! this->boundaryCondition.setup( parameters, "boundary-conditions-" ) ||
       ! this->rightHandSide.setup( parameters, "right-hand-side-" ) )
      return false;
   return true;
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
typename HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::IndexType
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
getDofs( const MeshType& mesh ) const
{
   /****
    * Return number of  DOFs (degrees of freedom) i.e. number
    * of unknowns to be resolved by the main solver.
    */
   return mesh.template getEntitiesCount< typename MeshType::Cell >();
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
void
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
bindDofs( const MeshType& mesh,
          DofVectorType& dofVector )
{
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
bool
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
setInitialCondition( const tnlParameterContainer& parameters,
                     const MeshType& mesh,
                     DofVectorType& dofs,
                     MeshDependentDataType& meshDependentData )
{
   const tnlString& initialConditionFile = parameters.getParameter< tnlString >( "initial-condition" );
   tnlMeshFunction< Mesh > u( mesh, dofs );
   if( ! u.boundLoad( initialConditionFile ) )
   {
      cerr << "I am not able to load the initial condition from the file " << initialConditionFile << "." << endl;
      return false;
   }
   return true; 
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
   template< typename Matrix >
bool
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
setupLinearSystem( const MeshType& mesh,
                   Matrix& matrix )
{
   const IndexType dofs = this->getDofs( mesh );
   typedef typename Matrix::CompressedRowsLengthsVector CompressedRowsLengthsVectorType;
   CompressedRowsLengthsVectorType rowLengths;
   if( ! rowLengths.setSize( dofs ) )
      return false;
   tnlMatrixSetter< MeshType, DifferentialOperator, BoundaryCondition, CompressedRowsLengthsVectorType > matrixSetter;
   matrixSetter.template getCompressedRowsLengths< typename Mesh::Cell >( mesh,
                                                                          differentialOperator,
                                                                          boundaryCondition,
                                                                          rowLengths );
   matrix.setDimensions( dofs, dofs );
   if( ! matrix.setCompressedRowsLengths( rowLengths ) )
      return false;
   return true;
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
bool
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
makeSnapshot( const RealType& time,
              const IndexType& step,
              const MeshType& mesh,
              DofVectorType& dofs,
              MeshDependentDataType& meshDependentData )
{
   cout << endl << "Writing output at time " << time << " step " << step << "." << endl;
   this->bindDofs( mesh, dofs );
   MeshFunctionType u;
   u.bind( mesh, dofs );
   tnlString fileName;
   FileNameBaseNumberEnding( "u-", step, 5, ".tnl", fileName );
   if( ! u.save( fileName ) )
      return false;
   return true;
}

#ifdef HAVE_CUDA

template< typename Real, typename Index >
__global__ void boundaryConditionsKernel( Real* u,
                                          Real* fu,
                                          const Index gridXSize, const Index gridYSize )
{
   const Index i = ( blockIdx.x ) * blockDim.x + threadIdx.x;
   const Index j = ( blockIdx.y ) * blockDim.y + threadIdx.y;
   if( i == 0 && j < gridYSize )
   {
      fu[ j * gridXSize ] = 0.0;
      u[ j * gridXSize ] = 0.0; //u[ j * gridXSize + 1 ];
   }
   if( i == gridXSize - 1 && j < gridYSize )
   {
      fu[ j * gridXSize + gridYSize - 1 ] = 0.0;
      u[ j * gridXSize + gridYSize - 1 ] = 0.0; //u[ j * gridXSize + gridXSize - 1 ];      
   }
   if( j == 0 && i > 0 && i < gridXSize - 1 )
   {
      fu[ i ] = 0.0; //u[ j * gridXSize + 1 ];
      u[ i ] = 0.0; //u[ j * gridXSize + 1 ];
   }
   if( j == gridYSize -1  && i > 0 && i < gridXSize - 1 )
   {
      fu[ j * gridXSize + i ] = 0.0; //u[ j * gridXSize + gridXSize - 1 ];      
      u[ j * gridXSize + i ] = 0.0; //u[ j * gridXSize + gridXSize - 1 ];      
   }         
}


template< typename Real, typename Index >
__global__ void heatEquationKernel( const Real* u, 
                                    Real* fu,
                                    const Real tau,
                                    const Real hx_inv,
                                    const Real hy_inv,
                                    const Index gridXSize,
                                    const Index gridYSize )
{
   const Index i = blockIdx.x * blockDim.x + threadIdx.x;
   const Index j = blockIdx.y * blockDim.y + threadIdx.y;
   if( i > 0 && i < gridXSize - 1 &&
       j > 0 && j < gridYSize - 1 )
   {
      const Index c = j * gridXSize + i;
      fu[ c ] = ( u[ c - 1 ] - 2.0 * u[ c ] + u[ c + 1 ] ) * hx_inv +
                ( u[ c - gridXSize ] - 2.0 * u[ c ] + u[ c + gridXSize ] ) * hy_inv;
   }
}
#endif

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
void
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
getExplicitRHS( const RealType& time,
                const RealType& tau,
                const MeshType& mesh,
                DofVectorType& u,
                DofVectorType& fu,
                MeshDependentDataType& meshDependentData )
{
   /****
    * If you use an explicit solver like tnlEulerSolver or tnlMersonSolver, you
    * need to implement this method. Compute the right-hand side of
    *
    *   d/dt u(x) = fu( x, u )
    *
    * You may use supporting mesh dependent data if you need.
    */

   if( std::is_same< DeviceType, tnlHost >::value )
   {
      const IndexType gridXSize = mesh.getDimensions().x();
      const IndexType gridYSize = mesh.getDimensions().y();
      const RealType& hx_inv = mesh.template getSpaceStepsProducts< -2,  0 >();
      const RealType& hy_inv = mesh.template getSpaceStepsProducts<  0, -2 >();
      for( IndexType j = 0; j < gridYSize; j++ )
      {
         fu[ j * gridXSize ] = 0.0; //u[ j * gridXSize + 1 ];
         fu[ j * gridXSize + gridXSize - 2 ] = 0.0; //u[ j * gridXSize + gridXSize - 1 ];
      }
      for( IndexType i = 0; i < gridXSize; i++ )
      {
         fu[ i ] = 0.0; //u[ gridXSize + i ];
         fu[ ( gridYSize - 1 ) * gridXSize + i ] = 0.0; //u[ ( gridYSize - 2 ) * gridXSize + i ];
      }

      for( IndexType j = 1; j < gridYSize - 1; j++ )
         for( IndexType i = 1; i < gridXSize - 1; i++ )
         {
            const IndexType c = j * gridXSize + i;
            fu[ c ] = tau * ( ( u[ c - 1 ] - 2.0 * u[ c ] + u[ c + 1 ] ) * hx_inv +
                              ( u[ c - gridXSize ] - 2.0 * u[ c ] + u[ c + gridXSize ] ) * hy_inv );
         }
   }
   if( std::is_same< DeviceType, tnlCuda >::value )
   {
      const IndexType gridXSize = mesh.getDimensions().x();
      const IndexType gridYSize = mesh.getDimensions().y();
      const RealType& hx_inv = mesh.template getSpaceStepsProducts< -2,  0 >();
      const RealType& hy_inv = mesh.template getSpaceStepsProducts<  0, -2 >();
      
      dim3 cudaBlockSize( 16, 16 );
      dim3 cudaGridSize( gridXSize / 16 + ( gridXSize % 16 != 0 ),
                         gridYSize / 16 + ( gridYSize % 16 != 0 ) );
      
      int cudaErr;
      boundaryConditionsKernel<<< cudaGridSize, cudaBlockSize >>>( u.getData(), fu.getData(), gridXSize, gridYSize );
      if( ( cudaErr = cudaGetLastError() ) != cudaSuccess )
      {
         cerr << "Setting of boundary conditions failed. " << cudaErr << endl;
         return;
      }

      /****
       * Laplace operator
       */
      //cout << "Laplace operator ... " << endl;
      heatEquationKernel<<< cudaGridSize, cudaBlockSize >>>
         ( u.getData(), fu.getData(), tau, hx_inv, hy_inv, gridXSize, gridYSize );
      if( cudaGetLastError() != cudaSuccess )
      {
         cerr << "Laplace operator failed." << endl;
         return;
      }
   }
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator >
   template< typename Matrix >
void
HeatEquationBenchmarkProblem< Mesh, BoundaryCondition, RightHandSide, DifferentialOperator >::
assemblyLinearSystem( const RealType& time,
                      const RealType& tau,
                      const MeshType& mesh,
                      DofVectorType& _u,
                      Matrix& matrix,
                      DofVectorType& b,
                      MeshDependentDataType& meshDependentData )
{
   tnlLinearSystemAssembler< Mesh,
                             MeshFunctionType,
                             DifferentialOperator,
                             BoundaryCondition,
                             RightHandSide,
                             tnlBackwardTimeDiscretisation,
                             Matrix,
                             DofVectorType > systemAssembler;

   tnlMeshFunction< Mesh > u( mesh, _u );
   systemAssembler.template assembly< typename Mesh::Cell >( time,
                                                             tau,
                                                             mesh,
                                                             this->differentialOperator,
                                                             this->boundaryCondition,
                                                             this->rightHandSide,
                                                             u,
                                                             matrix,
                                                             b );
}

#endif /* HeatEquationBenchmarkPROBLEM_IMPL_H_ */
