/***************************************************************************
                          VectorVerticalOperationsTest.h  -  description
                             -------------------
    begin                : Aug 3, 2019
    copyright            : (C) 2019 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#ifdef HAVE_GTEST

#if defined(DISTRIBUTED_VECTOR)
   #include <TNL/Communicators/MpiCommunicator.h>
   #include <TNL/Communicators/NoDistrCommunicator.h>
   #include <TNL/Containers/DistributedVector.h>
   #include <TNL/Containers/DistributedVectorView.h>
   #include <TNL/Containers/Partitioner.h>
#elif defined(STATIC_VECTOR)
   #include <TNL/Containers/StaticVector.h>
#else
   #include <TNL/Containers/Vector.h>
   #include <TNL/Containers/VectorView.h>
#endif

#include "VectorHelperFunctions.h"

#include "gtest/gtest.h"

using namespace TNL;
using namespace TNL::Containers;
using namespace TNL::Containers::Algorithms;

namespace vertical_tests {

// should be small enough to have fast tests, but larger than minGPUReductionDataSize
// and large enough to require multiple CUDA blocks for reduction
constexpr int VECTOR_TEST_REDUCTION_SIZE = 4999;

// test fixture for typed tests
template< typename T >
class VectorVerticalOperationsTest : public ::testing::Test
{
protected:
   using VectorOrView = T;
#ifdef STATIC_VECTOR
   template< typename Real >
   using Vector = StaticVector< VectorOrView::getSize(), Real >;
#else
   using NonConstReal = std::remove_const_t< typename VectorOrView::RealType >;
   #ifdef DISTRIBUTED_VECTOR
      using CommunicatorType = typename VectorOrView::CommunicatorType;
      using VectorType = DistributedVector< NonConstReal, typename VectorOrView::DeviceType, typename VectorOrView::IndexType, CommunicatorType >;
      template< typename Real >
      using Vector = DistributedVector< Real, typename VectorOrView::DeviceType, typename VectorOrView::IndexType, CommunicatorType >;
   #else
      using VectorType = Containers::Vector< NonConstReal, typename VectorOrView::DeviceType, typename VectorOrView::IndexType >;
      template< typename Real >
      using Vector = Containers::Vector< Real, typename VectorOrView::DeviceType, typename VectorOrView::IndexType >;
   #endif
#endif

   VectorOrView V1;

#ifndef STATIC_VECTOR
   VectorType _V1;
#endif

   void reset( int size )
   {
#ifdef STATIC_VECTOR
      setLinearSequence( V1 );
#else
   #ifdef DISTRIBUTED_VECTOR
      const typename CommunicatorType::CommunicationGroup group = CommunicatorType::AllGroup;
      using LocalRangeType = typename VectorOrView::LocalRangeType;
      const LocalRangeType localRange = Partitioner< typename VectorOrView::IndexType, CommunicatorType >::splitRange( size, group );

      const int rank = CommunicatorType::GetRank(group);
      const int nproc = CommunicatorType::GetSize(group);

      _V1.setDistribution( localRange, size, group );
   #else
      _V1.setSize( size );
   #endif
      setLinearSequence( _V1 );
      bindOrAssign( V1, _V1 );
#endif
   }

   VectorVerticalOperationsTest()
   {
      reset( VECTOR_TEST_REDUCTION_SIZE );
   }
};

#define SETUP_VERTICAL_TEST_ALIASES \
   using VectorOrView = typename TestFixture::VectorOrView; \
   VectorOrView& V1 = this->V1;                             \
   const int size = V1.getSize();                           \

// types for which VectorVerticalOperationsTest is instantiated
#if defined(DISTRIBUTED_VECTOR)
   using VectorTypes = ::testing::Types<
   #ifndef HAVE_CUDA
      DistributedVector<           double, Devices::Host, int, Communicators::MpiCommunicator >,
      DistributedVectorView<       double, Devices::Host, int, Communicators::MpiCommunicator >,
      DistributedVectorView< const double, Devices::Host, int, Communicators::MpiCommunicator >,
      DistributedVector<           double, Devices::Host, int, Communicators::NoDistrCommunicator >,
      DistributedVectorView<       double, Devices::Host, int, Communicators::NoDistrCommunicator >,
      DistributedVectorView< const double, Devices::Host, int, Communicators::NoDistrCommunicator >
   #else
      DistributedVector<           double, Devices::Cuda, int, Communicators::MpiCommunicator >,
      DistributedVectorView<       double, Devices::Cuda, int, Communicators::MpiCommunicator >,
      DistributedVectorView< const double, Devices::Cuda, int, Communicators::MpiCommunicator >,
      DistributedVector<           double, Devices::Cuda, int, Communicators::NoDistrCommunicator >,
      DistributedVectorView<       double, Devices::Cuda, int, Communicators::NoDistrCommunicator >,
      DistributedVectorView< const double, Devices::Cuda, int, Communicators::NoDistrCommunicator >
   #endif
   >;
#elif defined(STATIC_VECTOR)
   using VectorTypes = ::testing::Types<
      StaticVector< 1, int >,
      StaticVector< 1, double >,
      StaticVector< 2, int >,
      StaticVector< 2, double >,
      StaticVector< 3, int >,
      StaticVector< 3, double >,
      StaticVector< 4, int >,
      StaticVector< 4, double >,
      StaticVector< 5, int >,
      StaticVector< 5, double >
   >;
#else
   using VectorTypes = ::testing::Types<
   #ifndef HAVE_CUDA
      Vector<     int,       Devices::Host >,
      VectorView< int,       Devices::Host >,
      VectorView< const int, Devices::Host >,
      Vector<     double,    Devices::Host >,
      VectorView< double,    Devices::Host >
   #endif
   #ifdef HAVE_CUDA
      Vector<     int,       Devices::Cuda >,
      VectorView< int,       Devices::Cuda >,
      VectorView< const int, Devices::Cuda >,
      Vector<     double,    Devices::Cuda >,
      VectorView< double,    Devices::Cuda >
   #endif
   >;
#endif

TYPED_TEST_SUITE( VectorVerticalOperationsTest, VectorTypes );

TYPED_TEST( VectorVerticalOperationsTest, max )
{
   SETUP_VERTICAL_TEST_ALIASES;

   // vector or view
   EXPECT_EQ( max(V1), size - 1 );
   // unary expression
   EXPECT_EQ( max(-V1), 0 );
   // binary expression
   EXPECT_EQ( max(V1 + 2), size - 1 + 2 );
}

TYPED_TEST( VectorVerticalOperationsTest, argMax )
{
   SETUP_VERTICAL_TEST_ALIASES;
   using RealType = typename TestFixture::VectorOrView::RealType;

   // vector or view
   EXPECT_EQ( argMax(V1), std::make_pair(size - 1, (RealType) size - 1) );
   // unary expression
   EXPECT_EQ( argMax(-V1), std::make_pair(0, (RealType) 0) );
   // expression
   EXPECT_EQ( argMax(V1 + 2), std::make_pair(size - 1, (RealType) size - 1 + 2) );
}

TYPED_TEST( VectorVerticalOperationsTest, min )
{
   SETUP_VERTICAL_TEST_ALIASES;

   // vector or view
   EXPECT_EQ( min(V1), 0 );
   // unary expression
   EXPECT_EQ( min(-V1), 1 - size );
   // binary expression
   EXPECT_EQ( min(V1 + 2), 2 );
}

TYPED_TEST( VectorVerticalOperationsTest, argMin )
{
   SETUP_VERTICAL_TEST_ALIASES;
   using RealType = typename TestFixture::VectorOrView::RealType;

   // vector or view
   EXPECT_EQ( argMin(V1), std::make_pair(0, (RealType) 0) );
   // unary expression
   EXPECT_EQ( argMin(-V1), std::make_pair(size - 1, (RealType) 1 - size) );
   // binary expression
   EXPECT_EQ( argMin(V1 + 2), std::make_pair(0, (RealType) 2) );
}

TYPED_TEST( VectorVerticalOperationsTest, sum )
{
   SETUP_VERTICAL_TEST_ALIASES;

   // vector or view
   EXPECT_EQ( sum(V1), 0.5 * size * (size - 1) );
   // unary expression
   EXPECT_EQ( sum(-V1), - 0.5 * size * (size - 1) );
   // binary expression
   EXPECT_EQ( sum(V1 - 1), 0.5 * size * (size - 1) - size );
}

TYPED_TEST( VectorVerticalOperationsTest, maxNorm )
{
   SETUP_VERTICAL_TEST_ALIASES;

   // vector or view
   EXPECT_EQ( maxNorm(V1), size - 1 );
   // unary expression
   EXPECT_EQ( maxNorm(-V1), size - 1 );
   // binary expression
   EXPECT_EQ( maxNorm(V1 - size), size );
}

TYPED_TEST( VectorVerticalOperationsTest, l1Norm )
{
#ifdef STATIC_VECTOR
   setConstantSequence( this->V1, 1 );
   const typename TestFixture::VectorOrView& V1( this->V1 );
#else
   // we have to use _V1 because V1 might be a const view
   setConstantSequence( this->_V1, 1 );
   const typename TestFixture::VectorOrView& V1( this->_V1 );
#endif
   const int size = V1.getSize();

   // vector or vector view
   EXPECT_EQ( l1Norm(V1), size );
   // unary expression
   EXPECT_EQ( l1Norm(-V1), size );
   // binary expression
   EXPECT_EQ( l1Norm(2 * V1 - V1), size );
}

TYPED_TEST( VectorVerticalOperationsTest, l2Norm )
{
   using RealType = typename TestFixture::VectorOrView::RealType;
#ifdef STATIC_VECTOR
   setConstantSequence( this->V1, 1 );
   const typename TestFixture::VectorOrView& V1( this->V1 );
#else
   // we have to use _V1 because V1 might be a const view
   setConstantSequence( this->_V1, 1 );
   const typename TestFixture::VectorOrView& V1( this->_V1 );
#endif
   const int size = V1.getSize();

   const auto expected = std::sqrt( (RealType) size );

   // vector or vector view
   EXPECT_EQ( l2Norm(V1), expected );
   // unary expression
   EXPECT_EQ( l2Norm(-V1), expected );
   // binary expression
   EXPECT_EQ( l2Norm(2 * V1 - V1), expected );
}

TYPED_TEST( VectorVerticalOperationsTest, lpNorm )
{
   using RealType = typename TestFixture::VectorOrView::RealType;
#ifdef STATIC_VECTOR
   setConstantSequence( this->V1, 1 );
   const typename TestFixture::VectorOrView& V1( this->V1 );
#else
   // we have to use _V1 because V1 might be a const view
   setConstantSequence( this->_V1, 1 );
   const typename TestFixture::VectorOrView& V1( this->_V1 );
#endif
   const int size = V1.getSize();

   const auto expectedL1norm = size;
   const auto expectedL2norm = std::sqrt( (RealType) size );
   const auto expectedL3norm = std::cbrt( (RealType) size );

   const auto epsilon = 64 * std::numeric_limits< decltype(expectedL3norm) >::epsilon();

   // vector or vector view
   EXPECT_EQ( lpNorm(V1, 1.0), expectedL1norm );
   EXPECT_EQ( lpNorm(V1, 2.0), expectedL2norm );
   EXPECT_NEAR( lpNorm(V1, 3.0), expectedL3norm, epsilon );
   // unary expression
   EXPECT_EQ( lpNorm(-V1, 1.0), expectedL1norm );
   EXPECT_EQ( lpNorm(-V1, 2.0), expectedL2norm );
   EXPECT_NEAR( lpNorm(-V1, 3.0), expectedL3norm, epsilon );
   // binary expression
   EXPECT_EQ( lpNorm(2 * V1 - V1, 1.0), expectedL1norm );
   EXPECT_EQ( lpNorm(2 * V1 - V1, 2.0), expectedL2norm );
   EXPECT_NEAR( lpNorm(2 * V1 - V1, 3.0), expectedL3norm, epsilon );
}

TYPED_TEST( VectorVerticalOperationsTest, product )
{
   // VERY small size to avoid overflows
   this->reset( 16 );

#ifdef STATIC_VECTOR
   setConstantSequence( this->V1, 2 );
   const typename TestFixture::VectorOrView& V2( this->V1 );
#else
   // we have to use _V1 because V1 might be a const view
   setConstantSequence( this->_V1, 2 );
   const typename TestFixture::VectorOrView& V2( this->_V1 );
#endif
   const int size = V2.getSize();

   // vector or vector view
   EXPECT_EQ( product(V2), std::exp2(size) );
   // unary expression
   EXPECT_EQ( product(-V2), std::exp2(size) * ( (size % 2) ? -1 : 1 ) );
   // binary expression
   EXPECT_EQ( product(2 * V2 - V2), std::exp2(size) );
}

// TODO: tests for logicalOr, binaryOr, logicalAnd, binaryAnd

} // namespace vertical_tests

#endif // HAVE_GTEST

#if !defined(DISTRIBUTED_VECTOR) && !defined(STATIC_VECTOR)
#include "../main.h"
#endif
