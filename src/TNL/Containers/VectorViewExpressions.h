/***************************************************************************
                          VectorViewExpressions.h  -  description
                             -------------------
    begin                : Apr 27, 2019
    copyright            : (C) 2019 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/Algorithms/ArrayOperations.h>
#include <TNL/Containers/Expressions/ExpressionTemplates.h>

#include "VectorView.h"

namespace TNL {
namespace Containers {

////
// Addition
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator+( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real, Device, Index >, ET, Expressions::Addition >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator+( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< ET, VectorView< Real, Device, Index >, Expressions::Addition >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
operator+( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index >, Expressions::Addition >( a, b );
}

////
// Subtraction
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator-( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real, Device, Index >, ET, Expressions::Subtraction >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator-( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< ET, VectorView< Real, Device, Index >, Expressions::Subtraction >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
operator-( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index >, Expressions::Subtraction >( a, b );
}

////
// Multiplication
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator*( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real, Device, Index >, ET, Expressions::Multiplication >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator*( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< ET, VectorView< Real, Device, Index >, Expressions::Multiplication >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
operator*( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index >, Expressions::Multiplication >( a, b );
}

////
// Division
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator/( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real, Device, Index >, ET, Expressions::Division >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator/( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< ET, VectorView< Real, Device, Index >, Expressions::Division >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
operator/( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::BinaryExpressionTemplate< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index >, Expressions::Division >( a, b );
}

////
// Comparison operations - operator ==
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator==( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::Comparison< VectorView< Real, Device, Index >, ET >::EQ( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator==( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::Comparison< ET, VectorView< Real, Device, Index > >::EQ( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index >
bool
operator==( const VectorView< Real1, Device1, Index >& a, const VectorView< Real2, Device2, Index >& b )
{
   if( a.getSize() != b.getSize() )
      return false;
   if( a.getSize() == 0 )
      return true;
   return Algorithms::ArrayOperations< Device1, Device2 >::
            compare( a.getData(),
                     b.getData(),
                     a.getSize() );
}

////
// Comparison operations - operator !=
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator!=( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::Comparison< VectorView< Real, Device, Index >, ET >::NE( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator!=( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::Comparison< ET, VectorView< Real, Device, Index > >::NE( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index >
bool
operator!=( const VectorView< Real1, Device1, Index >& a, const VectorView< Real2, Device2, Index >& b )
{
   return ! (a == b);
}

////
// Comparison operations - operator <
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::Comparison< VectorView< Real, Device, Index >, ET >::LT( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::Comparison< ET, VectorView< Real, Device, Index > >::LT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
bool
operator<( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::Comparison< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index > >::LT( a, b );
}

////
// Comparison operations - operator <=
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<=( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::Comparison< VectorView< Real, Device, Index >, ET >::LE( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<=( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::Comparison< ET, VectorView< Real, Device, Index > >::LE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
bool
operator<=( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::Comparison< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index > >::LE( a, b );
}

////
// Comparison operations - operator >
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::Comparison< VectorView< Real, Device, Index >, ET >::GT( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::Comparison< ET, VectorView< Real, Device, Index > >::GT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
bool
operator>( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::Comparison< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index > >::GT( a, b );
}

////
// Comparison operations - operator >=
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>=( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::Comparison< VectorView< Real, Device, Index >, ET >::GE( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>=( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::Comparison< ET, VectorView< Real, Device, Index > >::GE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
bool
operator>=( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::Comparison< VectorView< Real1, Device, Index >, VectorView< Real2, Device, Index > >::GE( a, b );
}

////
// Minus
template< typename Real, typename Device, typename Index >
auto
operator-( const VectorView< Real, Device, Index >& a )
{
   return Expressions::UnaryExpressionTemplate< VectorView< Real, Device, Index >, Expressions::Minus >( a );
}

////
// Scalar product
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator,( const VectorView< Real, Device, Index >& a, const ET& b )
{
   return Expressions::ExpressionSum( a * b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator,( const ET& a, const VectorView< Real, Device, Index >& b )
{
   return Expressions::ExpressionSum( a * b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
operator,( const VectorView< Real1, Device, Index >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::ExpressionSum( a * b );
}

} // namespace Containers

////
// All functions are supposed to be in namespace TNL

////
// Min
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
min( const Containers::VectorView< Real, Device, Index >& a, const ET& b )
{
   return Containers::Expressions::BinaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, ET, Containers::Expressions::Min >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
min( const ET& a, const Containers::VectorView< Real, Device, Index >& b )
{
   return Containers::Expressions::BinaryExpressionTemplate< ET, Containers::VectorView< Real, Device, Index >, Containers::Expressions::Min >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
min( const Containers::VectorView< Real1, Device, Index >& a, const Containers::VectorView< Real2, Device, Index >& b )
{
   return Containers::Expressions::BinaryExpressionTemplate< Containers::VectorView< Real1, Device, Index >, Containers::VectorView< Real2, Device, Index >, Containers::Expressions::Min >( a, b );
}

////
// Max
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
max( const Containers::VectorView< Real, Device, Index >& a, const ET& b )
{
   return Containers::Expressions::BinaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, ET, Containers::Expressions::Max >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
max( const ET& a, const Containers::VectorView< Real, Device, Index >& b )
{
   return Containers::Expressions::BinaryExpressionTemplate< ET, Containers::VectorView< Real, Device, Index >, Containers::Expressions::Max >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index >
auto
max( const Containers::VectorView< Real1, Device, Index >& a, const Containers::VectorView< Real2, Device, Index >& b )
{
   return Containers::Expressions::BinaryExpressionTemplate< Containers::VectorView< Real1, Device, Index >, Containers::VectorView< Real2, Device, Index >, Containers::Expressions::Max >( a, b );
}

////
// Dot product - the same as scalar product, just for convenience
template< typename Real, typename Device, typename Index, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
dot( const Containers::VectorView< Real, Device, Index >& a, const ET& b )
{
   return (a, b);
}

template< typename ET, typename Real, typename Device, typename Index,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
dot( const ET& a, const Containers::VectorView< Real, Device, Index >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index1, typename Index2 >
auto
dot( const Containers::VectorView< Real1, Device, Index1 >& a, const Containers::VectorView< Real2, Device, Index2 >& b )
{
   return (a, b);
}


////
// Abs
template< typename Real, typename Device, typename Index >
auto
abs( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Abs >( a );
}

////
// Power
template< typename Real, typename Device, typename Index, typename ExpType >
auto
pow( const Containers::VectorView< Real, Device, Index >& a, const ExpType& exp )
{
   return Containers::Expressions::BinaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, ExpType, Containers::Expressions::Pow >( a, exp );
}

////
// Exp
template< typename Real, typename Device, typename Index >
auto
exp( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Exp >( a );
}

////
// Sqrt
template< typename Real, typename Device, typename Index >
auto
sqrt( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Sqrt >( a );
}

////
// Cbrt
template< typename Real, typename Device, typename Index >
auto
cbrt( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Cbrt >( a );
}

////
// Log
template< typename Real, typename Device, typename Index >
auto
log( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Log >( a );
}

////
// Log10
template< typename Real, typename Device, typename Index >
auto
log10( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Log10 >( a );
}

////
// Log2
template< typename Real, typename Device, typename Index >
auto
log2( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Log2 >( a );
}

////
// Sine
template< typename Real, typename Device, typename Index >
auto
sin( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Sin >( a );
}

////
// Cosine
template< typename Real, typename Device, typename Index >
auto
cos( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Cos >( a );
}

////
// Tangent
template< typename Real, typename Device, typename Index >
auto
tan( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Tan >( a );
}

////
// Asin
template< typename Real, typename Device, typename Index >
auto
asin( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Asin >( a );
}

////
// Acos
template< typename Real, typename Device, typename Index >
auto
acos( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Acos >( a );
}

////
// Atan
template< typename Real, typename Device, typename Index >
auto
atan( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Atan >( a );
}

////
// Sinh
template< typename Real, typename Device, typename Index >
auto
sinh( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Sinh >( a );
}

////
// Cosh
template< typename Real, typename Device, typename Index >
auto
cosh( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Cosh >( a );
}

////
// Tanh
template< typename Real, typename Device, typename Index >
auto
tanh( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Tanh >( a );
}

////
// Asinh
template< typename Real, typename Device, typename Index >
auto
asinh( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Asinh >( a );
}

////
// Acosh
template< typename Real, typename Device, typename Index >
auto
acosh( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Acosh >( a );
}

////
// Atanh
template< typename Real, typename Device, typename Index >
auto
atanh( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Atanh >( a );
}

////
// Floor
template< typename Real, typename Device, typename Index >
auto
floor( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Floor >( a );
}

////
// Ceil
template< typename Real, typename Device, typename Index >
auto
ceil( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Ceil >( a );
}

////
// Sign
template< typename Real, typename Device, typename Index >
auto
sign( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< Containers::VectorView< Real, Device, Index >, Containers::Expressions::Sign >( a );
}

////
// Cast
template< typename ResultType, typename Real, typename Device, typename Index,
          // workaround: templated type alias cannot be declared at block level
          template<typename> class Operation = Containers::Expressions::Cast< ResultType >::template Operation >
auto
cast( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< std::decay_t<decltype(a)>, Operation >( a );
}

////
// Vertical operations - min
template< typename Real,
          typename Device,
          typename Index >
Real
min( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionMin( a );
}

template< typename Real,
          typename Device,
          typename Index >
Real
argMin( const Containers::VectorView< Real, Device, Index >& a, Index& arg )
{
   return Containers::Expressions::ExpressionArgMin( a, arg );
}

template< typename Real,
          typename Device,
          typename Index >
Real
max( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionMax( a );
}

template< typename Real,
          typename Device,
          typename Index >
Real
argMax( const Containers::VectorView< Real, Device, Index >& a, Index& arg )
{
   return Containers::Expressions::ExpressionArgMax( a, arg );
}

template< typename Real,
          typename Device,
          typename Index >
auto
sum( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionSum( a );
}

template< typename Real,
          typename Device,
          typename Index >
auto
maxNorm( const Containers::VectorView< Real, Device, Index >& a )
{
   return max( abs( a ) );
}

template< typename Real,
          typename Device,
          typename Index >
auto
l1Norm( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionL1Norm( a );
}

template< typename Real,
          typename Device,
          typename Index >
auto
l2Norm( const Containers::VectorView< Real, Device, Index >& a )
{
   return TNL::sqrt( Containers::Expressions::ExpressionL2Norm( a ) );
}

template< typename Real,
          typename Device,
          typename Index,
          typename Real2 >
auto
lpNorm( const Containers::VectorView< Real, Device, Index >& a, const Real2& p )
// since (1.0 / p) has type double, TNL::pow returns double
-> double
{
   if( p == 1.0 )
      return l1Norm( a );
   if( p == 2.0 )
      return l2Norm( a );
   return TNL::pow( Containers::Expressions::ExpressionLpNorm( a, p ), 1.0 / p );
}

template< typename Real,
          typename Device,
          typename Index >
auto
product( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionProduct( a );
}

template< typename Real,
          typename Device,
          typename Index >
auto
logicalOr( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionLogicalOr( a );
}

template< typename Real,
          typename Device,
          typename Index >
auto
binaryOr( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionBinaryOr( a );
}

template< typename Real,
          typename Device,
          typename Index >
auto
logicalAnd( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionLogicalAnd( a );
}

template< typename Real,
          typename Device,
          typename Index >
auto
binaryAnd( const Containers::VectorView< Real, Device, Index >& a )
{
   return Containers::Expressions::ExpressionBinaryAnd( a );
}

} // namespace TNL
