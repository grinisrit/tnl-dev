#ifndef advectionPROBLEM_H_
#define advectionPROBLEM_H_

#include <TNL/problems/tnlPDEProblem.h>
#include <TNL/Functions/MeshFunction.h>
#include <TNL/tnlSharedPointer.h>

namespace TNL {

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
           typename DifferentialOperator >
class advectionProblem:
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
      typedef tnlPDEProblem< Mesh, RealType, DeviceType, IndexType > BaseType;
      typedef tnlSharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;
      typedef tnlSharedPointer< DifferentialOperator > DifferentialOperatorPointer;
      typedef tnlSharedPointer< BoundaryCondition > BoundaryConditionPointer;
      typedef tnlSharedPointer< RightHandSide, DeviceType > RightHandSidePointer;
      
      using typename BaseType::MeshType;
      using typename BaseType::MeshPointer;
      using typename BaseType::DofVectorType;
      using typename BaseType::DofVectorPointer;
      using typename BaseType::MeshDependentDataType;     
      
      static String getTypeStatic();

      String getPrologHeader() const;

      void writeProlog( Logger& logger,
                        const Config::ParameterContainer& parameters ) const;

      bool setup( const Config::ParameterContainer& parameters );

      bool setInitialCondition( const Config::ParameterContainer& parameters,
                                const MeshPointer& mesh,
                                DofVectorPointer& dofs,
                                MeshDependentDataType& meshDependentData );

      template< typename Matrix >
      bool setupLinearSystem( const MeshPointer& mesh,
                              Matrix& matrix );

      bool makeSnapshot( const RealType& time,
                         const IndexType& step,
                         const MeshPointer& mesh,
                         DofVectorPointer& dofs,
                         MeshDependentDataType& meshDependentData );

      IndexType getDofs( const MeshPointer& mesh ) const;

      void bindDofs( const MeshPointer& mesh,
                     DofVectorPointer& dofs );

      void getExplicitRHS( const RealType& time,
                           const RealType& tau,
                           const MeshPointer& mesh,
                           DofVectorPointer& _u,
                           DofVectorPointer& _fu,
                           MeshDependentDataType& meshDependentData );

      template< typename Matrix >
      void assemblyLinearSystem( const RealType& time,
                                 const RealType& tau,
                                 const MeshPointer& mesh,
                                 DofVectorPointer& dofs,
                                 Matrix& matrix,
                                 DofVectorPointer& rightHandSide,
                                 MeshDependentDataType& meshDependentData );

   protected:

      MeshFunctionPointer uPointer;

      DifferentialOperatorPointer differentialOperatorPointer;

      BoundaryConditionPointer boundaryConditionPointer;

      RightHandSidePointer rightHandSidePointer;      
      
      String velocityType;
};

} // namespace TNL

#include "advectionProblem_impl.h"

#endif /* advectionPROBLEM_H_ */
