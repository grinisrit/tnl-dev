/***************************************************************************
                          Comparison.h  -  description
                             -------------------
    begin                : Apr 19, 2019
    copyright            : (C) 2019 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <stdexcept>

#include <TNL/Assert.h>
#include <TNL/Containers/Expressions/ExpressionVariableType.h>
#include <TNL/Algorithms/Reduction.h>
#include <TNL/Algorithms/MultiDeviceMemoryOperations.h>

namespace TNL {
namespace Containers {
namespace Expressions {

////
// Non-static comparison
template< typename T1,
          typename T2,
          ExpressionVariableType T1Type = getExpressionVariableType< T1, T2 >(),
          ExpressionVariableType T2Type = getExpressionVariableType< T2, T1 >() >
struct Comparison;

template< typename T1,
          typename T2,
          bool BothAreNonstaticVectors = IsArrayType< T1 >::value && IsArrayType< T2 >::value &&
                                       ! IsStaticArrayType< T1 >::value && ! IsStaticArrayType< T2 >::value >
struct VectorComparison;

// If both operands are vectors we compare them using array operations.
// It allows to compare vectors on different devices
template< typename T1, typename T2 >
struct VectorComparison< T1, T2, true >
{
   static bool EQ( const T1& a, const T2& b )
   {
      if( a.getSize() != b.getSize() )
         return false;
      if( a.getSize() == 0 )
         return true;
      return Algorithms::MultiDeviceMemoryOperations< typename T1::DeviceType, typename T2::DeviceType >::compare( a.getData(), b.getData(), a.getSize() );
   }
};

// If both operands are not vectors we compare them parallel reduction
template< typename T1, typename T2 >
struct VectorComparison< T1, T2, false >
{
   static bool EQ( const T1& a, const T2& b )
   {
      if( ! std::is_same< typename T1::DeviceType, typename T2::DeviceType >::value )
         throw std::runtime_error( "Cannot compare two expressions with different DeviceType." );

      if( a.getSize() != b.getSize() )
         return false;

      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] == view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }
};

/////
// Comparison of two vector expressions
template< typename T1,
          typename T2 >
struct Comparison< T1, T2, VectorExpressionVariable, VectorExpressionVariable >
{
   static bool EQ( const T1& a, const T2& b )
   {
      return VectorComparison< T1, T2 >::EQ( a, b );
   }

   static bool NE( const T1& a, const T2& b )
   {
      return ! EQ( a, b );
   }

   static bool GT( const T1& a, const T2& b )
   {
      if( ! std::is_same< typename T1::DeviceType, typename T2::DeviceType >::value )
         throw std::runtime_error( "Cannot compare two expressions with different DeviceType." );
      TNL_ASSERT_EQ( a.getSize(), b.getSize(), "Sizes of expressions to be compared do not fit." );

      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] > view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool GE( const T1& a, const T2& b )
   {
      if( ! std::is_same< typename T1::DeviceType, typename T2::DeviceType >::value )
         throw std::runtime_error( "Cannot compare two expressions with different DeviceType." );
      TNL_ASSERT_EQ( a.getSize(), b.getSize(), "Sizes of expressions to be compared do not fit." );

      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] >= view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool LT( const T1& a, const T2& b )
   {
      if( ! std::is_same< typename T1::DeviceType, typename T2::DeviceType >::value )
         throw std::runtime_error( "Cannot compare two expressions with different DeviceType." );
      TNL_ASSERT_EQ( a.getSize(), b.getSize(), "Sizes of expressions to be compared do not fit." );

      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] < view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool LE( const T1& a, const T2& b )
   {
      if( ! std::is_same< typename T1::DeviceType, typename T2::DeviceType >::value )
         throw std::runtime_error( "Cannot compare two expressions with different DeviceType." );
      TNL_ASSERT_EQ( a.getSize(), b.getSize(), "Sizes of expressions to be compared do not fit." );

      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] <= view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }
};

/////
// Comparison of number and vector expression
template< typename T1,
          typename T2 >
struct Comparison< T1, T2, ArithmeticVariable, VectorExpressionVariable >
{
   static bool EQ( const T1& a, const T2& b )
   {
      using DeviceType = typename T2::DeviceType;
      using IndexType = typename T2::IndexType;

      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return a == view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, b.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool NE( const T1& a, const T2& b )
   {
      return ! EQ( a, b );
   }

   static bool GT( const T1& a, const T2& b )
   {
      using DeviceType = typename T2::DeviceType;
      using IndexType = typename T2::IndexType;

      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return a > view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, b.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool GE( const T1& a, const T2& b )
   {
      using DeviceType = typename T2::DeviceType;
      using IndexType = typename T2::IndexType;

      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return a >= view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, b.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool LT( const T1& a, const T2& b )
   {
      using DeviceType = typename T2::DeviceType;
      using IndexType = typename T2::IndexType;

      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return a < view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, b.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool LE( const T1& a, const T2& b )
   {
      using DeviceType = typename T2::DeviceType;
      using IndexType = typename T2::IndexType;

      const auto view_b = b.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return a <= view_b[ i ]; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, b.getSize(), fetch, std::logical_and<>{}, true );
   }
};

/////
// Comparison of vector expressions and number
template< typename T1,
          typename T2 >
struct Comparison< T1, T2, VectorExpressionVariable, ArithmeticVariable >
{
   static bool EQ( const T1& a, const T2& b )
   {
      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] == b; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool NE( const T1& a, const T2& b )
   {
      return ! EQ( a, b );
   }

   static bool GT( const T1& a, const T2& b )
   {
      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] > b; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool GE( const T1& a, const T2& b )
   {
      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] >= b; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool LT( const T1& a, const T2& b )
   {
      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] < b; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }

   static bool LE( const T1& a, const T2& b )
   {
      using DeviceType = typename T1::DeviceType;
      using IndexType = typename T1::IndexType;

      const auto view_a = a.getConstView();
      auto fetch = [=] __cuda_callable__ ( IndexType i ) -> bool { return view_a[ i ] <= b; };
      return Algorithms::Reduction< DeviceType >::reduce( ( IndexType ) 0, a.getSize(), fetch, std::logical_and<>{}, true );
   }
};

} // namespace Expressions
} // namespace Containers
} // namespace TNL
