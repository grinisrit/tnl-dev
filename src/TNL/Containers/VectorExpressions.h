/***************************************************************************
                          VectorExpressions.h  -  description
                             -------------------
    begin                : Jun 27, 2019
    copyright            : (C) 2019 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/Expressions/ExpressionTemplates.h>

#include "Vector.h"

namespace TNL {
namespace Containers {

/**
 * \brief Addition of vector and vector expression.
 * @param a
 * @param b
 * @return 
 */
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator+( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView, ET, Expressions::Addition >( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator+( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ET, ConstView, Expressions::Addition >( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator+( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Addition >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator+( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Addition >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator+( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Addition >( a.getConstView(), b.getConstView() );
}

////
// Subtraction
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator-( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView, ET, Expressions::Subtraction >( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator-( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ET, ConstView, Expressions::Subtraction >( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator-( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Subtraction >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator-( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Subtraction >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator-( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Subtraction >( a.getConstView(), b.getConstView() );
}

////
// Multiplication
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator*( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView, ET, Expressions::Multiplication >( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator*( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ET, ConstView, Expressions::Multiplication >( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator*( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Multiplication >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator*( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Multiplication >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator*( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Multiplication >( a.getConstView(), b.getConstView() );
}

////
// Division
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator/( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView, ET, Expressions::Division >( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
auto
operator/( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ET, ConstView, Expressions::Division >( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator/( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Division >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator/( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Division >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator/( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Expressions::Division >( a.getConstView(), b.getConstView() );
}

////
// Comparison operations - operator ==
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator==( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView, ET >::EQ( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator==( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ET, ConstView >::EQ( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Allocator >
bool
operator==( const Vector< Real1, Device1, Index, Allocator >& a, const Vector< Real2, Device2, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device1, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Vector< Real2, Device2, Index, Allocator >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::EQ( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Allocator >
bool
operator==( const VectorView< Real1, Device1, Index >& a, const Vector< Real2, Device2, Index, Allocator >& b )
{
   using ConstView1 = VectorView< Real1, Device1, Index >;
   using ConstView2 = typename Vector< Real2, Device2, Index, Allocator >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::EQ( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Allocator >
bool
operator==( const Vector< Real1, Device1, Index, Allocator >& a, const VectorView< Real2, Device2, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device1, Index, Allocator >::ConstViewType;
   using ConstView2 = VectorView< Real2, Device2, Index >;
   return Expressions::Comparison< ConstView1, ConstView2 >::EQ( a.getConstView(), b );
}

////
// Comparison operations - operator !=
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator!=( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView, ET >::NE( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator!=( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ET, ConstView >::NE( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Allocator >
bool
operator!=( const Vector< Real1, Device1, Index, Allocator >& a, const Vector< Real2, Device2, Index, Allocator >& b )
{
   return ! (a == b);
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Allocator >
bool
operator!=( const VectorView< Real1, Device1, Index >& a, const Vector< Real2, Device2, Index, Allocator >& b )
{
   return ! (a == b);
}

template< typename Real1, typename Real2, typename Device1, typename Device2, typename Index, typename Allocator >
bool
operator!=( const Vector< Real1, Device1, Index, Allocator >& a, const VectorView< Real2, Device2, Index >& b )
{
   return ! (a == b);
}

////
// Comparison operations - operator <
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView, ET >::LT( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ET, ConstView >::LT( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator<( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::LT( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator<( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::LT( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator<( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::LT( a.getConstView(), b.getConstView() );
}

////
// Comparison operations - operator <=
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<=( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView, ET >::LE( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator<=( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ET, ConstView >::LE( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator<=( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::LE( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator<=( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::LE( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator<=( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::LE( a.getConstView(), b.getConstView() );
}

////
// Comparison operations - operator >
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView, ET >::GT( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ET, ConstView >::GT( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator>( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::GT( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator>( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::GT( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator>( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::GT( a.getConstView(), b.getConstView() );
}

////
// Comparison operations - operator >=
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>=( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView, ET >::GE( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
bool
operator>=( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename VectorView< Real, Device, Index >::ConstViewType;
   return Expressions::Comparison< ET, ConstView >::GE( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator>=( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::GE( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator>=( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::GE( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
bool
operator>=( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename VectorView< Real2, Device, Index >::ConstViewType;
   return Expressions::Comparison< ConstView1, ConstView2 >::GE( a.getConstView(), b.getConstView() );
}

////
// Minus
template< typename Real, typename Device, typename Index, typename Allocator >
auto
operator-( const Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Expressions::UnaryExpressionTemplate< ConstView, Expressions::Minus >( a.getConstView() );
}

////
// Scalar product
#ifdef DOXYGEN_ONLY
/**
 * \brief Computes scalar product of two vectors or vector expressions. Equivalent to \ref TNL::dot.
 *
 * @param a input vector or vector expression
 * @param b input vector or vector expression
 * @return scalar product of \e a and \e b.
 */
template< typename ET1, typename ET2 >
Real
operator,( const ET1& a, const ET2& b ) {}
#else

template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
Real
operator,( const Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   return Expressions::ExpressionSum( a.getConstView() * b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Expressions::IsNumericExpression<ET>::value > >
Real
operator,( const ET& a, const Vector< Real, Device, Index, Allocator >& b )
{
   return Expressions::ExpressionSum( a * b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator,( const Vector< Real1, Device, Index, Allocator >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   return Expressions::ExpressionSum( a.getConstView() * b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator,( const VectorView< Real1, Device, Index >& a, const Vector< Real2, Device, Index, Allocator >& b )
{
   return Expressions::ExpressionSum( a.getConstView() * b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
operator,( const Vector< Real1, Device, Index, Allocator >& a, const VectorView< Real2, Device, Index >& b )
{
   return Expressions::ExpressionSum( a.getConstView() * b.getConstView() );
}
#endif // DOXYGEN_ONLY

} //namespace Containers

////
// All functions are supposed to be in namespace TNL

////
// Min
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
min( const Containers::Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView, ET, Containers::Expressions::Min >( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
min( const ET& a, const Containers::Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ET, ConstView, Containers::Expressions::Min >( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
min( const Containers::Vector< Real1, Device, Index, Allocator >& a, const Containers::Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Containers::Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Containers::Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Containers::Expressions::Min >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
min( const Containers::VectorView< Real1, Device, Index >& a, const Containers::Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Containers::Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Containers::Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Containers::Expressions::Min >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
min( const Containers::Vector< Real1, Device, Index, Allocator >& a, const Containers::VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Containers::Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Containers::Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Containers::Expressions::Min >( a.getConstView(), b.getConstView() );
}

////
// Max
template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
max( const Containers::Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView, ET, Containers::Expressions::Max >( a.getConstView(), b );
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
max( const ET& a, const Containers::Vector< Real, Device, Index, Allocator >& b )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ET, ConstView, Containers::Expressions::Max >( a, b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
max( const Containers::Vector< Real1, Device, Index, Allocator >& a, const Containers::Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Containers::Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Containers::Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Containers::Expressions::Max >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
max( const Containers::VectorView< Real1, Device, Index >& a, const Containers::Vector< Real2, Device, Index, Allocator >& b )
{
   using ConstView1 = typename Containers::Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Containers::Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Containers::Expressions::Max >( a.getConstView(), b.getConstView() );
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
max( const Containers::Vector< Real1, Device, Index, Allocator >& a, const Containers::VectorView< Real2, Device, Index >& b )
{
   using ConstView1 = typename Containers::Vector< Real1, Device, Index, Allocator >::ConstViewType;
   using ConstView2 = typename Containers::Vector< Real2, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView1, ConstView2, Containers::Expressions::Max >( a.getConstView(), b.getConstView() );
}

////
// Dot product - the same as scalar product, just for convenience
#ifdef DOXYGEN_ONLY
/**
 * \brief Computes scalar product of vector and vector expression. Equivalent to \ref TNL::Containers::operator,.
 * 
 * @param a input vector
 * @param b input vector expression
 * @return scalar product of vector and vector expression
 */
template< typename ET1, typename ET2 >
auto
dot( const ET1& a, const ET2& b ) {}
#else

template< typename Real, typename Device, typename Index, typename Allocator, typename ET,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
dot( const Containers::Vector< Real, Device, Index, Allocator >& a, const ET& b )
{
   return (a, b);
}

template< typename ET, typename Real, typename Device, typename Index, typename Allocator,
          typename..., typename = std::enable_if_t< Containers::Expressions::IsNumericExpression<ET>::value > >
auto
dot( const ET& a, const Containers::Vector< Real, Device, Index, Allocator >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
dot( const Containers::Vector< Real1, Device, Index, Allocator >& a, const Containers::Vector< Real2, Device, Index, Allocator >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
dot( const Containers::VectorView< Real1, Device, Index >& a, const Containers::Vector< Real2, Device, Index, Allocator >& b )
{
   return (a, b);
}

template< typename Real1, typename Real2, typename Device, typename Index, typename Allocator >
auto
dot( const Containers::Vector< Real1, Device, Index, Allocator >& a, const Containers::VectorView< Real2, Device, Index >& b )
{
   return (a, b);
}
#endif // DOXYGEN_ONLY


////
// Abs
template< typename Real, typename Device, typename Index, typename Allocator >
auto
abs( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Abs >( a.getConstView() );
}

////
// Power
template< typename Real, typename Device, typename Index, typename Allocator, typename ExpType >
auto
pow( const Containers::Vector< Real, Device, Index, Allocator >& a, const ExpType& exp )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::BinaryExpressionTemplate< ConstView, ExpType, Containers::Expressions::Pow >( a.getConstView(), exp );
}

////
// Exp
template< typename Real, typename Device, typename Index, typename Allocator >
auto
exp( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Exp >( a.getConstView() );
}

////
// Sqrt
template< typename Real, typename Device, typename Index, typename Allocator >
auto
sqrt( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Sqrt >( a.getConstView() );
}

////
// Cbrt
template< typename Real, typename Device, typename Index, typename Allocator >
auto
cbrt( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Cbrt >( a.getConstView() );
}

////
// Log
template< typename Real, typename Device, typename Index, typename Allocator >
auto
log( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Log >( a.getConstView() );
}

////
// Log10
template< typename Real, typename Device, typename Index, typename Allocator >
auto
log10( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Log10 >( a.getConstView() );
}

////
// Log2
template< typename Real, typename Device, typename Index, typename Allocator >
auto
log2( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Log2 >( a.getConstView() );
}

////
// Sine
template< typename Real, typename Device, typename Index, typename Allocator >
auto
sin( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Sin >( a.getConstView() );
}

////
// Cosine
template< typename Real, typename Device, typename Index, typename Allocator >
auto
cos( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Cos >( a.getConstView() );
}

////
// Tangent
template< typename Real, typename Device, typename Index, typename Allocator >
auto
tan( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Tan >( a.getConstView() );
}

////
// Asin
template< typename Real, typename Device, typename Index, typename Allocator >
auto
asin( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Asin >( a.getConstView() );
}

////
// Acos
template< typename Real, typename Device, typename Index, typename Allocator >
auto
acos( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Acos >( a.getConstView() );
}

////
// Atan
template< typename Real, typename Device, typename Index, typename Allocator >
auto
atan( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Atan >( a.getConstView() );
}

////
// Sinh
template< typename Real, typename Device, typename Index, typename Allocator >
auto
sinh( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Sinh >( a.getConstView() );
}

////
// Cosh
template< typename Real, typename Device, typename Index, typename Allocator >
auto
cosh( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Cosh >( a.getConstView() );
}

////
// Tanh
template< typename Real, typename Device, typename Index, typename Allocator >
auto
tanh( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Tanh >( a.getConstView() );
}

////
// Asinh
template< typename Real, typename Device, typename Index, typename Allocator >
auto
asinh( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Asinh >( a.getConstView() );
}

////
// Acosh
template< typename Real, typename Device, typename Index, typename Allocator >
auto
acosh( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Acosh >( a.getConstView() );
}

////
// Atanh
template< typename Real, typename Device, typename Index, typename Allocator >
auto
atanh( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Atanh >( a.getConstView() );
}

////
// Floor
template< typename Real, typename Device, typename Index, typename Allocator >
auto
floor( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Floor >( a.getConstView() );
}

////
// Ceil
template< typename Real, typename Device, typename Index, typename Allocator >
auto
ceil( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Ceil >( a.getConstView() );
}

////
// Sign
template< typename Real, typename Device, typename Index, typename Allocator >
auto
sign( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   using ConstView = typename Containers::Vector< Real, Device, Index, Allocator >::ConstViewType;
   return Containers::Expressions::UnaryExpressionTemplate< ConstView, Containers::Expressions::Sign >( a.getConstView() );
}

////
// Cast
template< typename ResultType, typename Real, typename Device, typename Index, typename Allocator,
          // workaround: templated type alias cannot be declared at block level
          template<typename> class Operation = Containers::Expressions::Cast< ResultType >::template Operation >
auto
cast( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::UnaryExpressionTemplate< decltype(a.getConstView()), Operation >( a.getConstView() );
}

////
// Vertical operations - min
template< typename Real,
          typename Device,
          typename Index, typename Allocator >
Real
min( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionMin( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
std::pair< Index, std::decay_t< Real > >
argMin( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionArgMin( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
Real
max( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionMax( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
std::pair< Index, std::decay_t< Real > >
argMax( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionArgMax( a.getConstView() );
}

/**
 * \brief Computes sum of all vector elements.
 * 
 * @param a input vector
 * @return sum of all vector elements
 */
template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
sum( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionSum( a.getConstView() );
}

/**
 * \brief Computes maximum norm of a vector.
 * \anchor TNL__maxNorm
 * 
 * @param a input vector
 * @return  maximum norm
 */
template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
maxNorm( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return max( abs( a ) );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
l1Norm( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionL1Norm( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
l2Norm( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return TNL::sqrt( Containers::Expressions::ExpressionL2Norm( a.getConstView() ) );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator,
          typename Real2 >
auto
lpNorm( const Containers::Vector< Real, Device, Index, Allocator >& a, const Real2& p )
// since (1.0 / p) has type double, TNL::pow returns double
-> double
{
   if( p == 1.0 )
      return l1Norm( a );
   if( p == 2.0 )
      return l2Norm( a );
   return TNL::pow( Containers::Expressions::ExpressionLpNorm( a.getConstView(), p ), 1.0 / p );
}

/**
 * \brief Computes product of all vector elements.
 * @param a
 * @return 
 */
template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
product( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionProduct( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
logicalOr( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionLogicalOr( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
binaryOr( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionBinaryOr( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
logicalAnd( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionLogicalAnd( a.getConstView() );
}

template< typename Real,
          typename Device,
          typename Index, typename Allocator >
auto
binaryAnd( const Containers::Vector< Real, Device, Index, Allocator >& a )
{
   return Containers::Expressions::ExpressionBinaryAnd( a.getConstView() );
}

} // namespace TNL
