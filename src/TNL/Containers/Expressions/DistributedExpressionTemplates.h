/***************************************************************************
                          DistributedExpressionTemplates.h  -  description
                             -------------------
    begin                : Jun 28, 2019
    copyright            : (C) 2019 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <iostream>
#include <TNL/Containers/Expressions/ExpressionTemplatesOperations.h>
#include <TNL/Containers/Expressions/ExpressionVariableType.h>
#include <TNL/Containers/Expressions/DistributedComparison.h>
#include <TNL/Containers/Expressions/IsStatic.h>

#include <TNL/Communicators/MPIPrint.h>

#include <typeinfo>
#include <cxxabi.h>


namespace TNL {
   namespace Containers {
      namespace Expressions {

////
// Distributed unary expression template
template< typename T1,
          template< typename > class Operation,
          typename Parameter,
          typename Communicator,
          ExpressionVariableType T1Type = ExpressionVariableTypeGetter< T1 >::value >
struct DistributedUnaryExpressionTemplate
{
};

////
// Distributed binary expression template
template< typename T1,
          typename T2,
          template< typename, typename > class Operation,
          typename Communicator,
          ExpressionVariableType T1Type = ExpressionVariableTypeGetter< T1 >::value,
          ExpressionVariableType T2Type = ExpressionVariableTypeGetter< T2 >::value >
struct DistributedBinaryExpressionTemplate
{
};

template< typename T1,
          typename T2,
          template< typename, typename > class Operation,
          typename Communicator >
struct DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator, VectorExpressionVariable, VectorExpressionVariable >
{
   using RealType = typename std::remove_const< typename T1::RealType >::type;
   using DeviceType = typename T1::DeviceType;
   using IndexType = typename T1::IndexType;
   using IsExpressionTemplate = bool;
   using CommunicatorType = Communicator; //Communicators::MpiCommunicator;
   using CommunicationGroup = typename CommunicatorType::CommunicationGroup;

   static_assert( std::is_same< typename T1::DeviceType, typename T2::DeviceType >::value, "Attempt to mix operands allocated on different device types." );
   static_assert( IsStaticType< T1 >::value == IsStaticType< T2 >::value, "Attempt to mix static and non-static operands in binary expression templates." );
   static constexpr bool isStatic() { return false; }

   DistributedBinaryExpressionTemplate( const T1& a, const T2& b, const CommunicationGroup& group )
      : op1( a ), op2( b ), communicationGroup( group ) {}

   static DistributedBinaryExpressionTemplate evaluate( const T1& a, const T2& b )
   {
      return DistributedBinaryExpressionTemplate( a, b );
   }

   RealType getElement( const IndexType i ) const
   {
       return Operation< typename T1::RealType, typename T2::RealType >::evaluate( op1.getElement( i ), op2.getElement( i ) );
   }

   __cuda_callable__
   RealType operator[]( const IndexType i ) const
   {
       return Operation< typename T1::RealType, typename T2::RealType >::evaluate( op1[ i ], op2[ i ] );
   }

   __cuda_callable__
   int getSize() const
   {
       return op1.getSize();
   }

   CommunicationGroup getCommunicationGroup() const
   {
      return communicationGroup;
   }

   protected:
      const T1 op1;
      const T2 op2;
      CommunicationGroup communicationGroup;
      //typename OperandType< T1, DeviceType >::type op1;
      //typename OperandType< T2, DeviceType >::type op2;
};

template< typename T1,
          typename T2,
          template< typename, typename > class Operation,
          typename Communicator >
struct DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator, VectorExpressionVariable, ArithmeticVariable >
{
   using RealType = typename std::remove_const< typename T1::RealType >::type;
   using DeviceType = typename T1::DeviceType;
   using IndexType = typename T1::IndexType;
   using CommunicatorType = Communicator;
   using CommunicationGroup = typename CommunicatorType::CommunicationGroup;

   using IsExpressionTemplate = bool;
   static constexpr bool isStatic() { return false; }

   DistributedBinaryExpressionTemplate( const T1& a, const T2& b, const CommunicationGroup& group )
      : op1( a ), op2( b ), communicationGroup( group ){}

   DistributedBinaryExpressionTemplate evaluate( const T1& a, const T2& b )
   {
      return DistributedBinaryExpressionTemplate( a, b );
   }

   RealType getElement( const IndexType i ) const
   {
       return Operation< typename T1::RealType, T2 >::evaluate( op1.getElement( i ), op2 );
   }

   __cuda_callable__
   RealType operator[]( const IndexType i ) const
   {
       return Operation< typename T1::RealType, T2 >::evaluate( op1[ i ], op2 );
   }

   __cuda_callable__
   int getSize() const
   {
       return op1.getSize();
   }

   CommunicationGroup getCommunicationGroup() const
   {
      return communicationGroup;
   }

   protected:
      const T1 op1;
      const T2 op2;
      CommunicationGroup communicationGroup;
      //typename OperandType< T1, DeviceType >::type op1;
      //typename OperandType< T2, DeviceType >::type op2;
};

template< typename T1,
          typename T2,
          template< typename, typename > class Operation,
          typename Communicator >
struct DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator, ArithmeticVariable, VectorExpressionVariable >
{
   using RealType = typename std::remove_const< typename T2::RealType >::type;
   using DeviceType = typename T2::DeviceType;
   using IndexType = typename T2::IndexType;
   using CommunicatorType = Communicator;
   using CommunicationGroup = typename CommunicatorType::CommunicationGroup;

   using IsExpressionTemplate = bool;
   static constexpr bool isStatic() { return false; }

   DistributedBinaryExpressionTemplate( const T1& a, const T2& b, const CommunicationGroup& group )
      : op1( a ), op2( b ), communicationGroup( group ){}

   DistributedBinaryExpressionTemplate evaluate( const T1& a, const T2& b )
   {
      return DistributedBinaryExpressionTemplate( a, b );
   }

   RealType getElement( const IndexType i ) const
   {
       return Operation< T1, typename T2::RealType >::evaluate( op1, op2.getElement( i ) );
   }

   __cuda_callable__
   RealType operator[]( const IndexType i ) const
   {
       return Operation< T1, typename T2::RealType >::evaluate( op1, op2[ i ] );
   }

   __cuda_callable__
   int getSize() const
   {
       return op2.getSize();
   }

   CommunicationGroup getCommunicationGroup() const
   {
      return communicationGroup;
   }

   protected:
      const T1 op1;
      const T2 op2;
      CommunicationGroup communicationGroup;
      //typename OperandType< T1, DeviceType >::type op1;
      //typename OperandType< T2, DeviceType >::type op2;
};

////
// Distributed unary expression template
//
// Parameter type serves mainly for pow( base, exp ). Here exp is parameter we need
// to pass to pow.
template< typename T1,
          template< typename > class Operation,
          typename Parameter,
          typename Communicator >
struct DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator, VectorExpressionVariable >
{
   using RealType = typename std::remove_const< typename T1::RealType >::type;
   using DeviceType = typename T1::DeviceType;
   using IndexType = typename T1::IndexType;
   using IsExpressionTemplate = bool;
   using CommunicatorType = Communicator;
   using CommunicationGroup = typename CommunicatorType::CommunicationGroup;
   static constexpr bool isStatic() { return false; }

   DistributedUnaryExpressionTemplate( const T1& a, const Parameter& p, const CommunicationGroup& group )
   : operand( a ), parameter( p ), communicationGroup( group ) {}

   static DistributedUnaryExpressionTemplate evaluate( const T1& a )
   {
      return DistributedUnaryExpressionTemplate( a );
   }

   RealType getElement( const IndexType i ) const
   {
       return Operation< typename T1::RealType >::evaluate( operand.getElement( i ), parameter );
   }

   __cuda_callable__
   RealType operator[]( const IndexType i ) const
   {
       return Operation< typename T1::RealType >::evaluate( operand[ i ], parameter );
   }

   __cuda_callable__
   int getSize() const
   {
       return operand.getSize();
   }

   void set( const Parameter& p ) { parameter = p; }

   const Parameter& get() { return parameter; }

   CommunicationGroup getCommunicationGroup() const
   {
      return communicationGroup;
   }

   protected:
      const T1 operand;
      //typename OperandType< T1, DeviceType >::type operand;
      Parameter parameter;
      CommunicationGroup communicationGroup;
};

////
// Distributed unary expression template with no parameter
template< typename T1,
          template< typename > class Operation,
          typename Communicator >
struct DistributedUnaryExpressionTemplate< T1, Operation, void, Communicator, VectorExpressionVariable >
{
   using RealType = typename std::remove_const< typename T1::RealType >::type;
   using DeviceType = typename T1::DeviceType;
   using IndexType = typename T1::IndexType;
   using IsExpressionTemplate = bool;
   using CommunicatorType = Communicator;
   using CommunicationGroup = typename CommunicatorType::CommunicationGroup;
   static constexpr bool isStatic() { return false; }

   DistributedUnaryExpressionTemplate( const T1& a, const CommunicationGroup& group )
      : operand( a ), communicationGroup( group ){}

   static DistributedUnaryExpressionTemplate evaluate( const T1& a )
   {
      return DistributedUnaryExpressionTemplate( a );
   }

   RealType getElement( const IndexType i ) const
   {
       return Operation< typename T1::RealType >::evaluate( operand.getElement( i ) );
   }

   __cuda_callable__
   RealType operator[]( const IndexType i ) const
   {
       return Operation< typename T1::RealType >::evaluate( operand[ i ] );
   }

   __cuda_callable__
   int getSize() const
   {
       return operand.getSize();
   }

   CommunicationGroup getCommunicationGroup() const
   {
      return communicationGroup;
   }

   protected:
      const T1 operand; // TODO: fix
      //typename std::add_const< typename OperandType< T1, DeviceType >::type >::type operand;
      CommunicationGroup communicationGroup;
};

      } //namespace Expressions
   } //namespace Containers

////
// All operations are supposed to be in namespace TNL

////
// Binary expressions addition
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation,
          typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Addition, Communicator >
operator + ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation,
          typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::Addition, Communicator >
operator + ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   Containers::Expressions::Addition, Communicator >
operator + ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::Addition, Communicator >
operator + ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   Containers::Expressions::Addition, Communicator >
operator + ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Addition, Communicator >
operator + ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
   Containers::Expressions::Addition, Communicator >
operator + ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
      Containers::Expressions::Addition, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Addition, Communicator >
operator + ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1,LOperation, LParameter, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Addition, Communicator >( a, b );
}

////
// Binary expression subtraction
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Subtraction, Communicator >
operator - ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1,LOperation, LParameter, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Subtraction, Communicator >( a, b );
}

////
// Binary expression multiplication
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Multiplication, Communicator >
operator * ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1,LOperation, LParameter, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Multiplication, Communicator >( a, b );
}

////
// Binary expression division
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Division, Communicator >
operator / ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::Division, Communicator >
operator / ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   Containers::Expressions::Division, Communicator >
operator + ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::Division, Communicator >
operator / ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   Containers::Expressions::Division, Communicator >
operator / ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Division, Communicator >
operator / ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
   Containers::Expressions::Division, Communicator >
operator / ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >,
      Containers::Expressions::Division, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Division, Communicator >
operator / ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1,LOperation, LParameter, Communicator >& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Division, Communicator >( a, b );
}


////
// Binary expression min
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Min, Communicator >
min ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
      const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::Min, Communicator >
min( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
     const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   Containers::Expressions::Min, Communicator >
min( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
     const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::Min, Communicator >
min( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
     const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   Containers::Expressions::Min, Communicator >
min( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
     const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Min, Communicator >
min( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
     const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Min, Communicator >
min( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
     const Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Min, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Min, Communicator >
min( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1,LOperation, LParameter, Communicator >& a,
     const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Min, Communicator >( a, b );
}

////
// Binary expression max
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Max, Communicator >
max( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
     const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::Max, Communicator >
max( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
     const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
   Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
   Containers::Expressions::Max, Communicator >
operator + ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType,
      Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::Max, Communicator >
max( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
     const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename T1,
          template< typename > class Operation, typename Parameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
   Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
   Containers::Expressions::Max, Communicator >
max( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
     const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType,
      Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
   Containers::Expressions::Max, Communicator >
max( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
     const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Max, Communicator >
max( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
     const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Max, Communicator >( a, b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
const Containers::Expressions::DistributedBinaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
   Containers::Expressions::Max, Communicator >
max( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1,LOperation, LParameter, Communicator >& a,
     const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >,
      Containers::Expressions::Max, Communicator >( a, b );
}


////
// Comparison operator ==
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator == ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator == ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   using Right = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator == ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
              const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   using Right = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator == ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator == ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator == ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation,
          typename RParameter,
          typename Communicator >
bool
operator == ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template EQ< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

////
// Comparison operator !=
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator != ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator != ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   using Right = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator != ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator != ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
              const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   using Right = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator != ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator != ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator != ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator != ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template NE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

////
// Comparison operator <
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator < ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator < ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   using Right = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator < ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator < ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   using Right = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator < ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator < ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator < ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator < ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

////
// Comparison operator <=
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator <= ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator <= ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   using Right = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator <= ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator <= ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
              const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   using Right = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator <= ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator <= ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator <= ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator <= ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template LE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

////
// Comparison operator >
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator > ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator > ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   using Right = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator > ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   using Right = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator > ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
             const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator > ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
             const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator > ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
             const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator > ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GT< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

////
// Comparison operator >=
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator >= ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator >= ( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   using Right = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator >= ( const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& a,
              const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   using Right = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
bool
operator >= ( const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& a,
              const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename T1,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
bool
operator >= ( const typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType& a,
              const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& b )
{
   using Left = typename Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >::RealType;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
bool
operator >= ( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
              const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >;
   using Right = Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation,
          typename RParameter, typename Communicator >
bool
operator >= ( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
             const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
{
   using Left = Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >;
   using Right = Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, RParameter, Communicator >;
   return Containers::Expressions::DistributedComparison< Left, Right >::template GE< typename Left::CommunicatorType >( a, b, a.getCommunicationGroup() );
}

////
// Unary operations

////
// Minus
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Minus, void, Communicator >
operator -( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Minus, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Minus, void, Communicator >
operator -( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Minus, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Abs
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Abs, void, Communicator >
abs( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Abs, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Abs, void, Communicator >
abs( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Abs, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Sin
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Sin, void, Communicator >
sin( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Sin, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Sin, void, Communicator >
sin( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Sin, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Cos
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Cos, void, Communicator >
cos( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Cos, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Cos, void, Communicator >
cos( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Cos, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Tan
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Tan, void, Communicator >
tan( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Tan, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Tan, void, Communicator >
tan( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Tan, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Sqrt
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Sqrt, void, Communicator >
sqrt( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Sqrt, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Sqrt, void, Communicator >
sqrt( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Sqrt, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Cbrt
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Cbrt, void, Communicator >
cbrt( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Cbrt, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Cbrt, void, Communicator >
cbrt( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Cbrt, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Pow
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename Real, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Pow, Real, Communicator >
pow( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a, const Real& exp )
{
   auto e = Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Pow, Real, Communicator >( a, a.getCommunicationGroup() );
   e.parameter.set( exp );
   return e;
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename Real, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Pow, Real, Communicator >
pow( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a, const Real& exp )
{
   auto e = Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Pow, Real, Communicator >( a, a.getCommunicationGroup() );
   e.parameter.set( exp );
   return e;
}

////
// Floor
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Floor, void, Communicator >
floor( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Floor, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Floor, void, Communicator >
floor( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Floor, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Ceil
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Ceil, void, Communicator >
ceil( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Ceil, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Ceil, void, Communicator >
ceil( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Ceil, void, Communicator>( a );
}

////
// Asin
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Asin, void, Communicator >
asin( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Asin, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Asin, void, Communicator >
asin( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Asin, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Acos
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Acos, void, Communicator >
cos( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Acos, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Acos, void, Communicator >
acos( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Acos, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Atan
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Atan, void, Communicator >
atan( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Atan, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Atan, void, Communicator >
atan( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Atan, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Sinh
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Sinh, void, Communicator >
sinh( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Sinh, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Sinh, void, Communicator >
sinh( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Sinh, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Cosh
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Cosh, void, Communicator >
cosh( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Cosh, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Cosh, void, Communicator >
cosh( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Cosh, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Tanh
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Tanh, void, Communicator >
tanh( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Tanh, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Tanh, void, Communicator >
tanh( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Tanh, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Log
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Log, void, Communicator >
log( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Log, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Log, void, Communicator >
log( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Log, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Log10
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Log10, void, Communicator >
log10( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Log10, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Log10, void, Communicator >
log10( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Log10, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Log2
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Log2, void, Communicator >
log2( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Log2, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
   Containers::Expressions::Log2, void, Communicator >
log2( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >,
      Containers::Expressions::Log2, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Exp
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
   Containers::Expressions::Exp, void, Communicator >
exp( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >,
      Containers::Expressions::Exp, void, Communicator >( a, a.getCommunicationGroup() );
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
const Containers::Expressions::DistributedUnaryExpressionTemplate<
   Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >,
   Containers::Expressions::Exp, void, Communicator >
exp( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate<
      Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >,
      Containers::Expressions::Exp, void, Communicator >( a, a.getCommunicationGroup() );
}

////
// Vertical operations - min
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
min( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   Real result = std::numeric_limits< Real >::max();
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionMin( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_MIN, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter,
          typename Communicator >
double 
min( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using CommunicatorType = Communicator;
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   Real result = std::numeric_limits< Real >::max();
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionMin( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_MIN, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename Communicator,
          typename Index >
auto
argMin( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a, Index& arg ) -> decltype( ExpressionArgMin( a, arg ) )
{
   throw Exceptions::NotImplementedError( "agrMin for distributed vector view is not implemented yet." );
   return ExpressionArgMin( a, arg );
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter,
          typename Communicator,
          typename Index >
auto
argMin( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a, Index& arg ) -> decltype( ExpressionMin( a, arg ) )
{
   throw Exceptions::NotImplementedError( "agrMin for distributed vector view is not implemented yet." );
   return ExpressionArgMin( a );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
max( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = std::numeric_limits< Real >::min();
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionMax( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_MAX, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType
max( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   Real result = std::numeric_limits< Real >::min();
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      Real localResult = Containers::Expressions::ExpressionMax( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_MAX, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename Communicator,
          typename Index >
auto
argMax( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a, Index& arg ) -> decltype( ExpressionArgMax( a, arg ) )
{
   throw Exceptions::NotImplementedError( "agrMax for distributed vector view is not implemented yet." );
   return ExpressionArgMax( a, arg );
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter,
          typename Communicator,
          typename Index >
auto
argMax( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a, Index& arg ) -> decltype( ExpressionMax( a, arg ) )
{
   throw Exceptions::NotImplementedError( "agrMax for distributed vector view is not implemented yet." );
   return ExpressionArgMax( a );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename Communicator >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
sum( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   using CommunicatorType = Communicator; //typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionSum( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_SUM, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType
sum( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   Real result = 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionSum( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_SUM, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename Communicator,
          typename Real >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
lpNorm( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a, const Real& p )
{
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = ( Real ) 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionLpNorm( a, p );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_SUM, a.getCommunicationGroup() );
   }
   return TNL::pow( result, 1.0 / p );
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter,
          typename Communicator,
          typename Real >
typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType
lpNorm( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a, const Real& p )
{
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   Real result = ( Real ) 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = TNL::pow( Containers::Expressions::ExpressionLpNorm( a, p ), p );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_SUM, a.getCommunicationGroup() );
   }
   return TNL::pow( result, 1.0 / p );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
product( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = ( Real ) 1.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionProduct( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_PROD, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType
product( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   Real result = ( Real ) 1.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionProduct( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_PROD, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
bool
logicalOr( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionLogicalOr( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_LOR, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
bool
logicalOr( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   bool result = false;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionLogicalOr( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_LOR, a.getCommunicationGroup() );
   }
   return result;
}


template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
bool
logicalAnd( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   bool result = false;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const bool localResult = Containers::Expressions::ExpressionLogicalAnd( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_LAND, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
bool
logicalAnd( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   bool result = false;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const bool localResult = Containers::Expressions::ExpressionLogicalAnd( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_LAND, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
binaryOr( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = ( Real ) 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionBinaryOr( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_BOR, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType
binaryOr( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   Real result = ( Real ) 0.0;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionBinaryOr( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_BOR, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation, typename Communicator >
typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType
binaryAnd( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >::CommunicatorType;
   Real result = std::numeric_limits< Real >::max;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionBinaryAnd( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_BAND, a.getCommunicationGroup() );
   }
   return result;
}

template< typename L1,
          template< typename > class LOperation,
          typename Parameter, typename Communicator >
typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType
binaryAnd( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >& a )
{
   using Real = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::RealType;
   using CommunicatorType = typename Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, Parameter, Communicator >::CommunicatorType;
   Real result = std::numeric_limits< Real >::max;
   if( a.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const Real localResult = Containers::Expressions::ExpressionBinaryAnd( a );
      CommunicatorType::template Allreduce< Real >( &localResult, &result, 1, MPI_BAND, a.getCommunicationGroup() );
   }
   return result;
}

////
// Scalar product
template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
auto
operator,( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
           const Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
auto
operator,( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
           const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
auto
operator,( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
           const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename Parameter, typename Communicator >
auto
operator,( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
           const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1, ROperation, Parameter, Communicator >& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}

template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
auto
dot( const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& a,
     const typename Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >::RealType& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}

template< typename L1,
          template< typename > class LOperation, typename LParameter,
          typename R1,
          typename R2,
          template< typename, typename > class ROperation, typename Communicator >
auto
dot( const Containers::Expressions::DistributedUnaryExpressionTemplate< L1, LOperation, LParameter, Communicator >& a,
     const typename Containers::Expressions::DistributedBinaryExpressionTemplate< R1, R2, ROperation, Communicator >& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}

template< typename L1,
          typename L2,
          template< typename, typename > class LOperation,
          typename R1,
          template< typename > class ROperation, typename RParameter, typename Communicator >
auto
dot( const Containers::Expressions::DistributedBinaryExpressionTemplate< L1, L2, LOperation, Communicator >& a,
     const typename Containers::Expressions::DistributedUnaryExpressionTemplate< R1,ROperation, RParameter, Communicator >& b )
-> decltype( TNL::sum( a * b ) )
{
   return TNL::sum( a * b );
}


////
// Evaluation with reduction
template< typename Vector,
   typename T1,
   typename T2,
   template< typename, typename > class Operation,
   typename Communicator,
   typename Reduction,
   typename VolatileReduction,
   typename Result >
Result evaluateAndReduce( Vector& lhs,
   const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& expression,
   Reduction& reduction,
   VolatileReduction& volatileReduction,
   const Result& zero )
{
   using RealType = typename Vector::RealType;
   using IndexType = typename Vector::IndexType;
   using DeviceType = typename Vector::DeviceType;

   RealType* lhs_data = lhs.getData();
   auto fetch = [=] __cuda_callable__ ( IndexType i ) -> RealType { return ( lhs_data[ i ] = expression[ i ] ); };
   return Containers::Algorithms::Reduction< DeviceType >::reduce( lhs.getLocalSize(), reduction, volatileReduction, fetch, zero );
}

template< typename Vector,
   typename T1,
   template< typename > class Operation,
   typename Parameter,
   typename Communicator,
   typename Reduction,
   typename VolatileReduction,
   typename Result >
Result evaluateAndReduce( Vector& lhs,
   const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& expression,
   Reduction& reduction,
   VolatileReduction& volatileReduction,
   const Result& zero )
{
   using RealType = typename Vector::RealType;
   using IndexType = typename Vector::IndexType;
   using DeviceType = typename Vector::DeviceType;

   RealType* lhs_data = lhs.getData();
   auto fetch = [=] __cuda_callable__ ( IndexType i ) -> RealType { return ( lhs_data[ i ] = expression[ i ] ); };
   return Containers::Algorithms::Reduction< DeviceType >::reduce( lhs.getLocalSize(), reduction, volatileReduction, fetch, zero );
}

////
// Addition and reduction
template< typename Vector,
   typename T1,
   typename T2,
   template< typename, typename > class Operation,
   typename Communicator,
   typename Reduction,
   typename VolatileReduction,
   typename Result >
Result addAndReduce( Vector& lhs,
   const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& expression,
   Reduction& reduction,
   VolatileReduction& volatileReduction,
   const Result& zero )
{
   using RealType = typename Vector::RealType;
   using IndexType = typename Vector::IndexType;
   using DeviceType = typename Vector::DeviceType;

   RealType* lhs_data = lhs.getData();
   auto fetch = [=] __cuda_callable__ ( IndexType i ) -> RealType {
      const RealType aux = expression[ i ];
      lhs_data[ i ] += aux;
      return aux;
   };
   return Containers::Algorithms::Reduction< DeviceType >::reduce( lhs.getLocalSize(), reduction, volatileReduction, fetch, zero );
}

template< typename Vector,
   typename T1,
   template< typename > class Operation,
   typename Parameter,
   typename Communicator,
   typename Reduction,
   typename VolatileReduction,
   typename Result >
Result addAndReduce( Vector& lhs,
   const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& expression,
   Reduction& reduction,
   VolatileReduction& volatileReduction,
   const Result& zero )
{
   using RealType = typename Vector::RealType;
   using IndexType = typename Vector::IndexType;
   using DeviceType = typename Vector::DeviceType;

   RealType* lhs_data = lhs.getData();
   auto fetch = [=] __cuda_callable__ ( IndexType i ) -> RealType {
      const RealType aux = expression[ i ];
      lhs_data[ i ] += aux;
      return aux;
   };
   return Containers::Algorithms::Reduction< DeviceType >::reduce( lhs.getLocalSize(), reduction, volatileReduction, fetch, zero );
}

////
// Addition and reduction
template< typename Vector,
   typename T1,
   typename T2,
   template< typename, typename > class Operation,
   typename Communicator,
   typename Reduction,
   typename VolatileReduction,
   typename Result >
Result addAndReduceAbs( Vector& lhs,
   const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& expression,
   Reduction& reduction,
   VolatileReduction& volatileReduction,
   const Result& zero )
{
   using RealType = typename Vector::RealType;
   using IndexType = typename Vector::IndexType;
   using DeviceType = typename Vector::DeviceType;

   RealType* lhs_data = lhs.getData();
   auto fetch = [=] __cuda_callable__ ( IndexType i ) -> RealType {
      const RealType aux = expression[ i ];
      lhs_data[ i ] += aux;
      return TNL::abs( aux );
   };
   return Containers::Algorithms::Reduction< DeviceType >::reduce( lhs.getLocalSize(), reduction, volatileReduction, fetch, zero );
}

template< typename Vector,
   typename T1,
   template< typename > class Operation,
   typename Parameter,
   typename Communicator,
   typename Reduction,
   typename VolatileReduction,
   typename Result >
Result addAndReduceAbs( Vector& lhs,
   const Containers::Expressions::DistributedUnaryExpressionTemplate< T1, Operation, Parameter, Communicator >& expression,
   Reduction& reduction,
   VolatileReduction& volatileReduction,
   const Result& zero )
{
   using RealType = typename Vector::RealType;
   using IndexType = typename Vector::IndexType;
   using DeviceType = typename Vector::DeviceType;

   RealType* lhs_data = lhs.getData();
   auto fetch = [=] __cuda_callable__ ( IndexType i ) -> RealType {
      const RealType aux = expression[ i ];
      lhs_data[ i ] += aux;
      return TNL::abs( aux );
   };
   return Containers::Algorithms::Reduction< DeviceType >::reduce( lhs.getLocalSize(), reduction, volatileReduction, fetch, zero );
}

////
// Output stream
template< typename T1,
          typename T2,
          template< typename, typename > class Operation, typename Communicator >
std::ostream& operator << ( std::ostream& str, const Containers::Expressions::DistributedBinaryExpressionTemplate< T1, T2, Operation, Communicator >& expression )
{
   str << "[ ";
   for( int i = 0; i < expression.getLocalSize() - 1; i++ )
      str << expression.getElement( i ) << ", ";
   str << expression.getElement( expression.getLocalSize() - 1 ) << " ]";
   return str;
}

template< typename T,
          template< typename > class Operation,
          typename Parameter, typename Communicator >
std::ostream& operator << ( std::ostream& str, const Containers::Expressions::DistributedUnaryExpressionTemplate< T, Operation, Parameter, Communicator >& expression )
{
   str << "[ ";
   for( int i = 0; i < expression.getLocalSize() - 1; i++ )
      str << expression.getElement( i ) << ", ";
   str << expression.getElement( expression.getLocalSize() - 1 ) << " ]";
   return str;
}
} // namespace TNL
