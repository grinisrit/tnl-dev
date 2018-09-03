/***************************************************************************
                          eulerProblem.h  -  description
                             -------------------
    begin                : Feb 13, 2017
    copyright            : (C) 2017 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Problems/PDEProblem.h>
#include <TNL/Functions/MeshFunction.h>
#include "CompressibleConservativeVariables.h"


using namespace TNL::Problems;

namespace TNL {

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
class eulerProblem:
   public PDEProblem< Mesh,
                      Communicator,
                      typename InviscidOperators::RealType,
                      typename Mesh::DeviceType,
                      typename InviscidOperators::IndexType >
{
   public:
      
      typedef typename InviscidOperators::RealType RealType;
      typedef typename Mesh::DeviceType DeviceType;
      typedef typename InviscidOperators::IndexType IndexType;
      typedef PDEProblem< Mesh, Communicator, RealType, DeviceType, IndexType > BaseType;
      
      using typename BaseType::MeshType;
      using typename BaseType::MeshPointer;
      using typename BaseType::DofVectorType;
      using typename BaseType::DofVectorPointer;

      static const int Dimensions = Mesh::getMeshDimension();      

      typedef Functions::MeshFunction< Mesh > MeshFunctionType;
      typedef CompressibleConservativeVariables< MeshType > ConservativeVariablesType;
      typedef Functions::VectorField< Dimensions, MeshFunctionType > VelocityFieldType;
      typedef SharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;
      typedef SharedPointer< ConservativeVariablesType > ConservativeVariablesPointer;
      typedef SharedPointer< VelocityFieldType > VelocityFieldPointer;
      typedef SharedPointer< InviscidOperators > InviscidOperatorsPointer;
      typedef SharedPointer< BoundaryCondition > BoundaryConditionPointer;
      typedef SharedPointer< RightHandSide, DeviceType > RightHandSidePointer;
      using CommunicatorType = Communicator;

      static String getTypeStatic();

      String getPrologHeader() const;

      void writeProlog( Logger& logger,
                        const Config::ParameterContainer& parameters ) const;

      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" );

      bool setInitialCondition( const Config::ParameterContainer& parameters,
                                DofVectorPointer& dofs );

      template< typename Matrix >
      bool setupLinearSystem( Matrix& matrix );

      bool makeSnapshot( const RealType& time,
                         const IndexType& step,
                         DofVectorPointer& dofs );

      IndexType getDofs() const;

      void bindDofs( DofVectorPointer& dofs );

      void getExplicitUpdate( const RealType& time,
                              const RealType& tau,
                              DofVectorPointer& _u,
                              DofVectorPointer& _fu );

      template< typename Matrix >
      void assemblyLinearSystem( const RealType& time,
                                 const RealType& tau,
                                 DofVectorPointer& dofs,
                                 Matrix& matrix,
                                 DofVectorPointer& rightHandSide );

      bool postIterate( const RealType& time,
                        const RealType& tau,
                        DofVectorPointer& dofs );

   protected:

      InviscidOperatorsPointer inviscidOperatorsPointer;
         
      BoundaryConditionPointer boundaryConditionPointer;
      RightHandSidePointer rightHandSidePointer;
      
      ConservativeVariablesPointer conservativeVariables,
                                   conservativeVariablesRHS;
      
      VelocityFieldPointer velocity;
      MeshFunctionPointer pressure;
      
      RealType gamma;          
};

} // namespace TNL

#include "eulerProblem_impl.h"

