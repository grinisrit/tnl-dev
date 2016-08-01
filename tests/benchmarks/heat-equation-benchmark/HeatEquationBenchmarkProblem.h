#ifndef HeatEquationBenchmarkPROBLEM_H_
#define HeatEquationBenchmarkPROBLEM_H_

#include <TNL/problems/tnlPDEProblem.h>
#include <TNL/Functions/MeshFunction.h>

using namespace TNL;

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
           typename DifferentialOperator >
class HeatEquationBenchmarkProblem:
   public tnlPDEProblem< Mesh,
                         typename DifferentialOperator::RealType,
                         typename Mesh::DeviceType,
                         typename DifferentialOperator::IndexType >
{
   public:

      typedef typename DifferentialOperator::RealType RealType;
      typedef typename Mesh::DeviceType DeviceType;
      typedef typename DifferentialOperator::IndexType IndexType;
      typedef Functions::MeshFunction< Mesh > MeshFunctionType;
      typedef tnlSharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;
      typedef tnlPDEProblem< Mesh, RealType, DeviceType, IndexType > BaseType;
      typedef tnlSharedPointer< DifferentialOperator > DifferentialOperatorPointer;
      typedef tnlSharedPointer< BoundaryCondition > BoundaryConditionPointer;
      typedef tnlSharedPointer< RightHandSide, DeviceType > RightHandSidePointer;
      

      using typename BaseType::MeshType;
      using typename BaseType::MeshPointer;
      using typename BaseType::DofVectorPointer;
      using typename BaseType::MeshDependentDataType;

      HeatEquationBenchmarkProblem();
      
      static String getTypeStatic();

      String getPrologHeader() const;

      void writeProlog( Logger& logger,
                        const Config::ParameterContainer& parameters ) const;

      bool setup( const Config::ParameterContainer& parameters );

      bool setInitialCondition( const Config::ParameterContainer& parameters,
                                const MeshPointer& meshPointer,
                                DofVectorPointer& dofsPointer,
                                MeshDependentDataType& meshDependentData );

      template< typename Matrix >
      bool setupLinearSystem( const MeshType& mesh,
                              Matrix& matrix );

      bool makeSnapshot( const RealType& time,
                         const IndexType& step,
                         const MeshPointer& meshPointer,
                         DofVectorPointer& dofsPointer,
                         MeshDependentDataType& meshDependentData );

      IndexType getDofs( const MeshPointer& meshPointer ) const;

      void bindDofs( const MeshPointer& meshPointer,
                     DofVectorPointer& dofsPointer );

      void getExplicitRHS( const RealType& time,
                           const RealType& tau,
                           const MeshPointer& meshPointer,
                           DofVectorPointer& _uPointer,
                           DofVectorPointer& _fuPointer,
                           MeshDependentDataType& meshDependentData );

      template< typename MatrixPointer >
      void assemblyLinearSystem( const RealType& time,
                                 const RealType& tau,
                                 const MeshPointer& mesh,
                                 DofVectorPointer& dofs,
                                 MatrixPointer& matrix,
                                 DofVectorPointer& rightHandSide,
                                 MeshDependentDataType& meshDependentData );
      
      ~HeatEquationBenchmarkProblem();

   protected:

      DifferentialOperatorPointer differentialOperatorPointer;
      BoundaryConditionPointer boundaryConditionPointer;
      RightHandSidePointer rightHandSidePointer;
      
      MeshFunctionPointer fu, u;
      
      String cudaKernelType;
      
      MeshType* cudaMesh;
      BoundaryCondition* cudaBoundaryConditions;
      RightHandSide* cudaRightHandSide;
      DifferentialOperator* cudaDifferentialOperator;
      
};

#include "HeatEquationBenchmarkProblem_impl.h"

#endif /* HeatEquationBenchmarkPROBLEM_H_ */
