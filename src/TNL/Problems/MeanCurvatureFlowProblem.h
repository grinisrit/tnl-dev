/***************************************************************************
                          HeatEquationProblem.h  -  description
                             -------------------
    begin                : Feb 23, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

/***
 * Authors:
 * Oberhuber Tomas, tomas.oberhuber@fjfi.cvut.cz
 * Szekely Ondrej, ondra.szekely@gmail.com
 */

#pragma once

#include <TNL/Operators/diffusion/OneSidedMeanCurvature.h>
#include <TNL/Problems/PDEProblem.h>
#include <TNL/Operators/operator-Q/tnlOneSideDiffOperatorQ.h>
#include <TNL/Matrices/CSR.h>
#include <TNL/Functions/MeshFunction.h>

namespace TNL {
namespace Problems {

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename DifferentialOperator =
            OneSidedMeanCurvature< Mesh,
                                      typename Mesh::RealType,
                                      typename Mesh::GlobalIndexType,
                                      false > >
class MeanCurvatureFlowProblem : public PDEProblem< Mesh,
                                                     typename DifferentialOperator::RealType,
                                                     typename Mesh::DeviceType,
                                                     typename DifferentialOperator::IndexType >
{
   public:

      typedef typename DifferentialOperator::RealType RealType;
      typedef typename Mesh::DeviceType DeviceType;
      typedef typename DifferentialOperator::IndexType IndexType;
      typedef Functions::MeshFunction< Mesh > MeshFunctionType;
      typedef PDEProblem< Mesh, RealType, DeviceType, IndexType > BaseType;
      typedef CSR< RealType, DeviceType, IndexType> MatrixType;

      using typename BaseType::MeshType;
      using typename BaseType::DofVectorType;
      using typename BaseType::MeshDependentDataType;
      using typename BaseType::MeshDependentDataPointer;

      static String getTypeStatic();

      String getPrologHeader() const;

      void writeProlog( Logger& logger,
                        const Config::ParameterContainer& parameters ) const;

      bool setup( const Config::ParameterContainer& parameters );

      bool setInitialCondition( const Config::ParameterContainer& parameters,
                                const MeshType& mesh,
                                DofVectorType& dofs,
                                MeshDependentDataPointer& meshDependentData );

      template< typename Matrix >
      bool setupLinearSystem( const MeshType& mesh,
                              Matrix& matrix );
 
      bool makeSnapshot( const RealType& time,
                         const IndexType& step,
                         const MeshType& mesh,
                         DofVectorType& dofs,
                         MeshDependentDataPointer& meshDependentData );

      IndexType getDofs( const MeshType& mesh ) const;

      void bindDofs( const MeshType& mesh,
                     DofVectorType& dofs );

      void getExplicitUpdate( const RealType& time,
                           const RealType& tau,
                           const MeshType& mesh,
                           DofVectorType& _u,
                           DofVectorType& _fu,
                           MeshDependentDataPointer& meshDependentData );

      template< typename Matrix >
      void assemblyLinearSystem( const RealType& time,
                                 const RealType& tau,
                                 const MeshType& mesh,
                                 DofVectorType& dofs,
                                 Matrix& matrix,
                                 DofVectorType& rightHandSide,
                                 MeshDependentDataPointer& meshDependentData );

      template< typename Matrix >
      void saveFailedLinearSystem( const Matrix& matrix,
                                   const DofVectorType& dofs,
                                   const DofVectorType& rightHandSide ) const;

      protected:

      SharedVector< RealType, DeviceType, IndexType > solution;

      DifferentialOperator differentialOperator;

      BoundaryCondition boundaryCondition;
 
      RightHandSide rightHandSide;
};

} // namespace Problems
} // namespace TNL

#include "MeanCurvatureFlowProblem_impl.h"
