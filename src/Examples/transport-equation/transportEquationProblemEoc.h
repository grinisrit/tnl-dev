/***************************************************************************
                          transportEquationProblemEoc.h  -  description
                             -------------------
    begin                : Feb 10, 2017
    copyright            : (C) 2017 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Problems/PDEProblem.h>
#include <TNL/Functions/MeshFunction.h>
#include <TNL/Pointers/SharedPointer.h>
#include "transportEquationProblem.h"

using namespace TNL::Problems;

namespace TNL {

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename Communicator,
          typename DifferentialOperator >
class transportEquationProblemEoc:
public transportEquationProblem< Mesh, BoundaryCondition, RightHandSide, Communicator, DifferentialOperator >
{
   public:

      typedef typename DifferentialOperator::RealType RealType;
      typedef typename Mesh::DeviceType DeviceType;
      typedef typename DifferentialOperator::IndexType IndexType;
      typedef Functions::MeshFunction< Mesh > MeshFunctionType;
      typedef transportEquationProblem< Mesh, BoundaryCondition, RightHandSide, Communicator, DifferentialOperator > BaseType;
      typedef Pointers::SharedPointer<  MeshFunctionType, DeviceType > MeshFunctionPointer;
      typedef Pointers::SharedPointer<  DifferentialOperator > DifferentialOperatorPointer;
      typedef Pointers::SharedPointer<  BoundaryCondition > BoundaryConditionPointer;
      typedef Pointers::SharedPointer<  RightHandSide, DeviceType > RightHandSidePointer;
      typedef typename DifferentialOperator::VelocityFieldType VelocityFieldType;
      typedef Pointers::SharedPointer<  VelocityFieldType, DeviceType > VelocityFieldPointer;
      
      
      typedef Communicator CommunicatorType;
      using typename BaseType::MeshType;
      using typename BaseType::MeshPointer;
      using typename BaseType::DofVectorType;
      using typename BaseType::DofVectorPointer;
      
      //using BaseType::getExplicitUpdate;
      
      static String getType();

      String getPrologHeader() const;

      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" );

      bool setInitialCondition( const Config::ParameterContainer& parameters,
                                DofVectorPointer& dofs );

      /*template< typename Matrix >
      bool setupLinearSystem( Matrix& matrix );

      bool makeSnapshot( const RealType& time,
                         const IndexType& step,
                         DofVectorPointer& dofs );

      IndexType getDofs() const;

      void bindDofs( DofVectorPointer& dofs );

      void getExplicitUpdate( const RealType& time,
                              const RealType& tau,
                              DofVectorPointer& _u,
                              DofVectorPointer& _fu );*/


   protected:
};

} // namespace TNL

#include "transportEquationProblemEoc_impl.h"
