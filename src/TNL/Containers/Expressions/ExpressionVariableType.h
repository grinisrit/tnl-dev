/***************************************************************************
                          ExpressionVariableType.h  -  description
                             -------------------
    begin                : Apr 18, 2019
    copyright            : (C) 2019 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <type_traits>

namespace TNL {
   namespace Containers {

template< int Size, typename Real >
class StaticVector;

template< typename Real, typename Device, typename Index >
class VectorView;

      namespace Expressions {

enum ExpressionVariableType { ArithmeticVariable, VectorVariable, OtherVariable };


/**
 * SFINAE for checking if T has getSize method
 */
template< typename T >
class IsExpressionTemplate
{
private:
    typedef char YesType[1];
    typedef char NoType[2];

    template< typename C > static YesType& test( typename C::IsExpressionTemplate );
    template< typename C > static NoType& test(...);

public:
    static constexpr bool value = ( sizeof( test< typename std::remove_reference< T >::type >(0) ) == sizeof( YesType ) );
};

template< typename T >
struct IsVectorType
{
   static constexpr bool value = false;
};

template< int Size,
          typename Real >
struct IsVectorType< StaticVector< Size, Real > >
{
   static constexpr bool value = true;
};

template< typename Real,
          typename Device,
          typename Index >
struct IsVectorType< VectorView< Real, Device, Index > >
{
   static constexpr bool value = true;
};

template< typename T,
          bool IsArithmetic = std::is_arithmetic< T >::value,
          bool IsVector = IsVectorType< T >::value || IsExpressionTemplate< T >::value >
struct ExpressionVariableTypeGetter
{
   static constexpr ExpressionVariableType value = OtherVariable;
};

template< typename T >
struct  ExpressionVariableTypeGetter< T, true, false >
{
   static constexpr ExpressionVariableType value = ArithmeticVariable;
};

template< typename T >
struct ExpressionVariableTypeGetter< T, false, true >
{
   static constexpr ExpressionVariableType value = VectorVariable;
};

////
// Non-static expression templates might be passed on GPU, for example. In this
// case, we cannot store ET operands using references but we nee to make copies.
template< typename T,
          typename Device >
struct OperandType
{
   using type = typename std::add_const< typename std::remove_reference< T >::type >::type;
};

template< typename T >
struct OperandType< T, Devices::Host >
{
   using type = typename std::add_const< typename std::add_lvalue_reference< T >::type >::type;
};

      } //namespace Expressions
   } //namespace Containers
} //namespace TNL