/***************************************************************************
                          DistributedVectorExpressions.h  -  description
                             -------------------
    begin                : Jul 1, 2019
    copyright            : (C) 2019 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/Expressions/DistributedExpressionTemplates.h>

#include "DistributedVector.h"

namespace TNL {
namespace Containers {

////
// Addition
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator+( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ET, Expressions::Addition >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator+( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< ET, std::decay_t<decltype(b)>, Expressions::Addition >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator+( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Addition >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator+( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Addition >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator+( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Addition >( a, b );
}

////
// Subtraction
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator-( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ET, Expressions::Subtraction >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator-( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< ET, std::decay_t<decltype(b)>, Expressions::Subtraction >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator-( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Subtraction >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator-( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Subtraction >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator-( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Subtraction >( a, b );
}

////
// Multiplication
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator*( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ET, Expressions::Multiplication >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator*( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< ET, std::decay_t<decltype(b)>, Expressions::Multiplication >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator*( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Multiplication >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator*( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Multiplication >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator*( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Multiplication >( a, b );
}

////
// Division
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator/( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ET, Expressions::Division >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator/( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< ET, std::decay_t<decltype(b)>, Expressions::Division >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator/( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Division >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator/( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Division >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator/( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Expressions::Division >( a, b );
}

////
// Comparison operations - operator ==
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator==( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, ET >::EQ( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator==( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< ET, std::decay_t<decltype(b)> >::EQ( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Communicator >
bool
operator==( const DistributedVector< Real1, Device1, Index, Communicator >& a, const DistributedVector< Real2, Device2, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::EQ( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Communicator >
bool
operator==( const DistributedVectorView< Real1, Device1, Index, Communicator >& a, const DistributedVector< Real2, Device2, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::EQ( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Communicator >
bool
operator==( const DistributedVector< Real1, Device1, Index, Communicator >& a, const DistributedVectorView< Real2, Device2, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::EQ( a, b );
}

////
// Comparison operations - operator !=
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator!=( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, ET >::NE( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator!=( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< ET, std::decay_t<decltype(b)> >::NE( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Communicator >
bool
operator!=( const DistributedVector< Real1, Device1, Index, Communicator >& a, const DistributedVector< Real2, Device2, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::NE( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Communicator >
bool
operator!=( const DistributedVectorView< Real1, Device1, Index, Communicator >& a, const DistributedVector< Real2, Device2, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::NE( a, b );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Communicator >
bool
operator!=( const DistributedVector< Real1, Device1, Index, Communicator >& a, const DistributedVectorView< Real2, Device2, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::NE( a, b );
}

////
// Comparison operations - operator <
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, ET >::LT( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< ET, std::decay_t<decltype(b)> >::LT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator<( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::LT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator<( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::LT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator<( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::LT( a, b );
}

////
// Comparison operations - operator <=
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<=( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, ET >::LE( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<=( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< ET, std::decay_t<decltype(b)> >::LE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator<=( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::LE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator<=( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::LE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator<=( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::LE( a, b );
}

////
// Comparison operations - operator >
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, ET >::GT( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< ET, std::decay_t<decltype(b)> >::GT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator>( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::GT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator>( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::GT( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator>( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::GT( a, b );
}

////
// Comparison operations - operator >=
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>=( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, ET >::GE( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>=( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< ET, std::decay_t<decltype(b)> >::GE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator>=( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::GE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator>=( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::GE( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
bool
operator>=( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedComparison< std::decay_t<decltype(a)>, std::decay_t<decltype(b)> >::GE( a, b );
}

////
// Minus
template< typename Real, typename Device, typename Index, typename Communicator >
auto
operator-( const DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Expressions::Minus >( a );
}

////
// Scalar product
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
Real
operator,( const DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Expressions::DistributedExpressionSum( a * b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
Real
operator,( const ET& a, const DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Expressions::DistributedExpressionSum( a * b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator,( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedExpressionSum( a * b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator,( const DistributedVectorView< Real1, Device, Index, Communicator >& a, const DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedExpressionSum( a * b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
operator,( const DistributedVector< Real1, Device, Index, Communicator >& a, const DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Expressions::DistributedExpressionSum( a * b );
}

} //namespace Containers

////
// All functions are supposed to be in namespace TNL

////
// Min
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
min( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ET, Containers::Expressions::Min >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
min( const ET& a, const Containers::DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< ET, std::decay_t<decltype(b)>, Containers::Expressions::Min >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
min( const Containers::DistributedVector< Real1, Device, Index, Communicator >& a, const Containers::DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Containers::Expressions::Min >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
min( const Containers::DistributedVectorView< Real1, Device, Index, Communicator >& a, const Containers::DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Containers::Expressions::Min >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
min( const Containers::DistributedVector< Real1, Device, Index, Communicator >& a, const Containers::DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Containers::Expressions::Min >( a, b );
}

////
// Max
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
max( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ET, Containers::Expressions::Max >( a, b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
max( const ET& a, const Containers::DistributedVector< Real, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< ET, std::decay_t<decltype(b)>, Containers::Expressions::Max >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
max( const Containers::DistributedVector< Real1, Device, Index, Communicator >& a, const Containers::DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Containers::Expressions::Max >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
max( const Containers::DistributedVectorView< Real1, Device, Index, Communicator >& a, const Containers::DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Containers::Expressions::Max >( a, b );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
max( const Containers::DistributedVector< Real1, Device, Index, Communicator >& a, const Containers::DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, std::decay_t<decltype(b)>, Containers::Expressions::Max >( a, b );
}

////
// Dot product - the same as scalar product, just for convenience
template< typename Real, typename Device, typename Index, typename Communicator, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
dot( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, const ET& b )
{
   return (a, b);
}

template< typename ET, typename Real, typename Device, typename Index, typename Communicator,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
dot( const ET& a, const Containers::DistributedVector< Real, Device, Index, Communicator >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
dot( const Containers::DistributedVector< Real1, Device, Index, Communicator >& a, const Containers::DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
dot( const Containers::DistributedVectorView< Real1, Device, Index, Communicator >& a, const Containers::DistributedVector< Real2, Device, Index, Communicator >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Communicator >
auto
dot( const Containers::DistributedVector< Real1, Device, Index, Communicator >& a, const Containers::DistributedVectorView< Real2, Device, Index, Communicator >& b )
{
   return (a, b);
}


////
// Abs
template< typename Real, typename Device, typename Index, typename Communicator >
auto
abs( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Abs >( a );
}

////
// Power
template< typename Real, typename Device, typename Index, typename Communicator, typename ExpType >
auto
pow( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, const ExpType& exp )
{
   return Containers::Expressions::DistributedBinaryExpressionTemplate< std::decay_t<decltype(a)>, ExpType, Containers::Expressions::Pow >( a, exp );
}

////
// Exp
template< typename Real, typename Device, typename Index, typename Communicator >
auto
exp( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Exp >( a );
}

////
// Sqrt
template< typename Real, typename Device, typename Index, typename Communicator >
auto
sqrt( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Sqrt >( a );
}

////
// Cbrt
template< typename Real, typename Device, typename Index, typename Communicator >
auto
cbrt( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Cbrt >( a );
}

////
// Log
template< typename Real, typename Device, typename Index, typename Communicator >
auto
log( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Log >( a );
}

////
// Log10
template< typename Real, typename Device, typename Index, typename Communicator >
auto
log10( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Log10 >( a );
}

////
// Log2
template< typename Real, typename Device, typename Index, typename Communicator >
auto
log2( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Log2 >( a );
}

////
// Sine
template< typename Real, typename Device, typename Index, typename Communicator >
auto
sin( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Sin >( a );
}

////
// Cosine
template< typename Real, typename Device, typename Index, typename Communicator >
auto
cos( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Cos >( a );
}

////
// Tangent
template< typename Real, typename Device, typename Index, typename Communicator >
auto
tan( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Tan >( a );
}

////
// Asin
template< typename Real, typename Device, typename Index, typename Communicator >
auto
asin( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Asin >( a );
}

////
// Acos
template< typename Real, typename Device, typename Index, typename Communicator >
auto
acos( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Acos >( a );
}

////
// Atan
template< typename Real, typename Device, typename Index, typename Communicator >
auto
atan( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Atan >( a );
}

////
// Sinh
template< typename Real, typename Device, typename Index, typename Communicator >
auto
sinh( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Sinh >( a );
}

////
// Cosh
template< typename Real, typename Device, typename Index, typename Communicator >
auto
cosh( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Cosh >( a );
}

////
// Tanh
template< typename Real, typename Device, typename Index, typename Communicator >
auto
tanh( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Tanh >( a );
}

////
// Asinh
template< typename Real, typename Device, typename Index, typename Communicator >
auto
asinh( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Asinh >( a );
}

////
// Acosh
template< typename Real, typename Device, typename Index, typename Communicator >
auto
acosh( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Acosh >( a );
}

////
// Atanh
template< typename Real, typename Device, typename Index, typename Communicator >
auto
atanh( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Atanh >( a );
}

////
// Floor
template< typename Real, typename Device, typename Index, typename Communicator >
auto
floor( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Floor >( a );
}

////
// Ceil
template< typename Real, typename Device, typename Index, typename Communicator >
auto
ceil( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Ceil >( a );
}

////
// Sign
template< typename Real, typename Device, typename Index, typename Communicator >
auto
sign( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Containers::Expressions::Sign >( a );
}

////
// Cast
template< typename ResultType, typename Real, typename Device, typename Index, typename Communicator,
          // workaround: templated type alias cannot be declared at block level
          template<typename> class Operation = Containers::Expressions::Cast< ResultType >::template Operation >
auto
cast( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedUnaryExpressionTemplate< std::decay_t<decltype(a)>, Operation >( a );
}

////
// Vertical operations - min
template< typename Real,
          typename Device,
          typename Index, typename Communicator >
Real
min( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionMin( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
Real
argMin( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, Index& arg )
{
   return Containers::Expressions::DistributedExpressionArgMin( a, arg );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
Real
max( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionMax( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
Real
argMax( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, Index& arg )
{
   return Containers::Expressions::DistributedExpressionArgMax( a, arg );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
sum( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionSum( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
maxNorm( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return max( abs( a ) );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
l1Norm( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionL1Norm( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
l2Norm( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return TNL::sqrt( Containers::Expressions::DistributedExpressionL2Norm( a ) );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator,
          typename Real2 >
auto
lpNorm( const Containers::DistributedVector< Real, Device, Index, Communicator >& a, const Real2& p )
// since (1.0 / p) has type double, TNL::pow returns double
-> double
{
   if( p == 1.0 )
      return l1Norm( a );
   if( p == 2.0 )
      return l2Norm( a );
   return TNL::pow( Containers::Expressions::DistributedExpressionLpNorm( a, p ), 1.0 / p );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
product( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionProduct( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
logicalOr( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionLogicalOr( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
binaryOr( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionBinaryOr( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
logicalAnd( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionLogicalAnd( a );
}

template< typename Real,
          typename Device,
          typename Index, typename Communicator >
auto
binaryAnd( const Containers::DistributedVector< Real, Device, Index, Communicator >& a )
{
   return Containers::Expressions::DistributedExpressionBinaryAnd( a );
}

} // namespace TNL
