/***************************************************************************
                          DistributedArrayTest.h  -  description
                             -------------------
    begin                : Sep 6, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

#ifdef HAVE_GTEST
#include <gtest/gtest.h>

#include <TNL/Communicators/MpiCommunicator.h>
#include <TNL/Communicators/NoDistrCommunicator.h>
#include <TNL/Communicators/ScopedInitializer.h>
#include <TNL/Containers/DistributedArray.h>
#include <TNL/Containers/Partitioner.h>

using namespace TNL;
using namespace TNL::Containers;

/*
 * Light check of DistributedArray.
 *
 * - Number of processes is not limited.
 * - Global size is hardcoded as 97 to force non-uniform distribution.
 * - Communication group is hardcoded as AllGroup -- it may be changed as needed.
 */
template< typename DistributedArray >
class DistributedArrayTest
: public ::testing::Test
{
protected:
   using ValueType = typename DistributedArray::ValueType;
   using DeviceType = typename DistributedArray::DeviceType;
   using CommunicatorType = typename DistributedArray::CommunicatorType;
   using IndexType = typename DistributedArray::IndexType;
   using DistributedArrayType = DistributedArray;
   using ArrayViewType = typename DistributedArrayType::LocalArrayViewType;
   using ArrayType = typename DistributedArrayType::LocalArrayType;

   const int globalSize = 97;  // prime number to force non-uniform distribution

   const typename CommunicatorType::CommunicationGroup group = CommunicatorType::AllGroup;

   DistributedArrayType distributedArray;

   const int rank = CommunicatorType::GetRank(group);
   const int nproc = CommunicatorType::GetSize(group);

   DistributedArrayTest()
   {
      using LocalRangeType = typename DistributedArray::LocalRangeType;
      const LocalRangeType localRange = Partitioner< IndexType, CommunicatorType >::splitRange( globalSize, group );
      distributedArray.setDistribution( localRange, globalSize, group );

      EXPECT_EQ( distributedArray.getLocalRange(), localRange );
      EXPECT_EQ( distributedArray.getCommunicationGroup(), group );
   }
};

// types for which DistributedArrayTest is instantiated
using DistributedArrayTypes = ::testing::Types<
   DistributedArray< double, Devices::Host, int, Communicators::MpiCommunicator >,
   DistributedArray< double, Devices::Host, int, Communicators::NoDistrCommunicator >
#ifdef HAVE_CUDA
   ,
   DistributedArray< double, Devices::Cuda, int, Communicators::MpiCommunicator >,
   DistributedArray< double, Devices::Cuda, int, Communicators::NoDistrCommunicator >
#endif
>;

TYPED_TEST_CASE( DistributedArrayTest, DistributedArrayTypes );

TYPED_TEST( DistributedArrayTest, checkSumOfLocalSizes )
{
   using CommunicatorType = typename TestFixture::CommunicatorType;

   const int localSize = this->distributedArray.getLocalArrayView().getSize();
   int sumOfLocalSizes = 0;
   CommunicatorType::Allreduce( &localSize, &sumOfLocalSizes, 1, MPI_SUM, this->group );
   EXPECT_EQ( sumOfLocalSizes, this->globalSize );
   EXPECT_EQ( this->distributedArray.getSize(), this->globalSize );
}

TYPED_TEST( DistributedArrayTest, copyFromGlobal )
{
   using ArrayViewType = typename TestFixture::ArrayViewType;
   using ArrayType = typename TestFixture::ArrayType;

   this->distributedArray.setValue( 0.0 );
   ArrayViewType localArrayView = this->distributedArray.getLocalArrayView();
   ArrayType globalArray( this->globalSize );
   globalArray.setValue( 1.0 );
   this->distributedArray.copyFromGlobal( globalArray );
   EXPECT_EQ( localArrayView, globalArray );
}

TYPED_TEST( DistributedArrayTest, setLike )
{
   using DistributedArrayType = typename TestFixture::DistributedArrayType;

   EXPECT_EQ( this->distributedArray.getSize(), this->globalSize );
   DistributedArrayType copy;
   EXPECT_EQ( copy.getSize(), 0 );
   copy.setLike( this->distributedArray );
   EXPECT_EQ( copy.getSize(), this->globalSize );
}

TYPED_TEST( DistributedArrayTest, reset )
{
   EXPECT_EQ( this->distributedArray.getSize(), this->globalSize );
   EXPECT_GT( this->distributedArray.getLocalArrayView().getSize(), 0 );
   this->distributedArray.reset();
   EXPECT_EQ( this->distributedArray.getSize(), 0 );
   EXPECT_EQ( this->distributedArray.getLocalArrayView().getSize(), 0 );
}

// TODO: swap

TYPED_TEST( DistributedArrayTest, setValue )
{
   using ArrayViewType = typename TestFixture::ArrayViewType;
   using ArrayType = typename TestFixture::ArrayType;

   this->distributedArray.setValue( 1.0 );
   ArrayViewType localArrayView = this->distributedArray.getLocalArrayView();
   ArrayType expected( localArrayView.getSize() );
   expected.setValue( 1.0 );
   EXPECT_EQ( localArrayView, expected );
}

TYPED_TEST( DistributedArrayTest, elementwiseAccess )
{
   using ArrayViewType = typename TestFixture::ArrayViewType;
   using IndexType = typename TestFixture::IndexType;

   this->distributedArray.setValue( 0 );
   ArrayViewType localArrayView = this->distributedArray.getLocalArrayView();
   const auto localRange = this->distributedArray.getLocalRange();

   // check initial value
   for( IndexType i = 0; i < localArrayView.getSize(); i++ ) {
      const IndexType gi = localRange.getGlobalIndex( i );
      EXPECT_EQ( localArrayView.getElement( i ), 0 );
      EXPECT_EQ( this->distributedArray.getElement( gi ), 0 );
      if( std::is_same< typename TestFixture::DeviceType, Devices::Host >::value )
         EXPECT_EQ( this->distributedArray[ gi ], 0 );
   }

   // use setValue
   for( IndexType i = 0; i < localArrayView.getSize(); i++ ) {
      const IndexType gi = localRange.getGlobalIndex( i );
      this->distributedArray.setElement( gi, i + 1 );
   }

   // check set value
   for( IndexType i = 0; i < localArrayView.getSize(); i++ ) {
      const IndexType gi = localRange.getGlobalIndex( i );
      EXPECT_EQ( localArrayView.getElement( i ), i + 1 );
      EXPECT_EQ( this->distributedArray.getElement( gi ), i + 1 );
      if( std::is_same< typename TestFixture::DeviceType, Devices::Host >::value )
         EXPECT_EQ( this->distributedArray[ gi ], i + 1 );
   }

   this->distributedArray.setValue( 0 );

   // use operator[]
   if( std::is_same< typename TestFixture::DeviceType, Devices::Host >::value ) {
      for( IndexType i = 0; i < localArrayView.getSize(); i++ ) {
         const IndexType gi = localRange.getGlobalIndex( i );
         this->distributedArray[ gi ] = i + 1;
      }

      // check set value
      for( IndexType i = 0; i < localArrayView.getSize(); i++ ) {
         const IndexType gi = localRange.getGlobalIndex( i );
         EXPECT_EQ( localArrayView.getElement( i ), i + 1 );
         EXPECT_EQ( this->distributedArray.getElement( gi ), i + 1 );
         EXPECT_EQ( this->distributedArray[ gi ], i + 1 );
      }
   }
}

TYPED_TEST( DistributedArrayTest, copyConstructor )
{
   using DistributedArrayType = typename TestFixture::DistributedArrayType;

   this->distributedArray.setValue( 1 );
   DistributedArrayType copy( this->distributedArray );
   // Array has "binding" copy-constructor
   EXPECT_EQ( copy.getLocalArrayView().getData(), this->distributedArray.getLocalArrayView().getData() );
}

TYPED_TEST( DistributedArrayTest, copyAssignment )
{
   using DistributedArrayType = typename TestFixture::DistributedArrayType;

   this->distributedArray.setValue( 1 );
   DistributedArrayType copy;
   copy = this->distributedArray;
   // no binding, but deep copy
   EXPECT_NE( copy.getLocalArrayView().getData(), this->distributedArray.getLocalArrayView().getData() );
   EXPECT_EQ( copy.getLocalArrayView(), this->distributedArray.getLocalArrayView() );
}

TYPED_TEST( DistributedArrayTest, comparisonOperators )
{
   using DistributedArrayType = typename TestFixture::DistributedArrayType;
   using IndexType = typename TestFixture::IndexType;

   const auto localRange = this->distributedArray.getLocalRange();
   DistributedArrayType& u = this->distributedArray;
   DistributedArrayType v, w;
   v.setLike( u );
   w.setLike( u );

   for( int i = 0; i < u.getLocalArrayView().getSize(); i ++ ) {
      const IndexType gi = localRange.getGlobalIndex( i );
      u.setElement( gi, i );
      v.setElement( gi, i );
      w.setElement( gi, 2 * i );
   }

   EXPECT_TRUE( u == u );
   EXPECT_TRUE( u == v );
   EXPECT_TRUE( v == u );
   EXPECT_FALSE( u != v );
   EXPECT_FALSE( v != u );
   EXPECT_TRUE( u != w );
   EXPECT_TRUE( w != u );
   EXPECT_FALSE( u == w );
   EXPECT_FALSE( w == u );

   v.reset();
   EXPECT_FALSE( u == v );
   u.reset();
   EXPECT_TRUE( u == v );
}

TYPED_TEST( DistributedArrayTest, containsValue )
{
   using IndexType = typename TestFixture::IndexType;

   const auto localRange = this->distributedArray.getLocalRange();

   for( int i = 0; i < this->distributedArray.getLocalArrayView().getSize(); i++ ) {
      const IndexType gi = localRange.getGlobalIndex( i );
      this->distributedArray.setElement( gi, i % 10 );
   }

   for( int i = 0; i < 10; i++ )
      EXPECT_TRUE( this->distributedArray.containsValue( i ) );

   for( int i = 10; i < 20; i++ )
      EXPECT_FALSE( this->distributedArray.containsValue( i ) );
}

TYPED_TEST( DistributedArrayTest, containsOnlyValue )
{
   using IndexType = typename TestFixture::IndexType;

   const auto localRange = this->distributedArray.getLocalRange();

   for( int i = 0; i < this->distributedArray.getLocalArrayView().getSize(); i++ ) {
      const IndexType gi = localRange.getGlobalIndex( i );
      this->distributedArray.setElement( gi, i % 10 );
   }

   for( int i = 0; i < 20; i++ )
      EXPECT_FALSE( this->distributedArray.containsOnlyValue( i ) );

   this->distributedArray.setValue( 100 );
   EXPECT_TRUE( this->distributedArray.containsOnlyValue( 100 ) );
}

TYPED_TEST( DistributedArrayTest, boolOperator )
{
   EXPECT_GT( this->distributedArray.getSize(), 0 );
   EXPECT_TRUE( this->distributedArray );
   this->distributedArray.reset();
   EXPECT_EQ( this->distributedArray.getSize(), 0 );
   EXPECT_FALSE( this->distributedArray );
}

#endif  // HAVE_GTEST


#if (defined(HAVE_GTEST) && defined(HAVE_MPI))
using CommunicatorType = Communicators::MpiCommunicator;

#include <sstream>

class MinimalistBufferedPrinter
: public ::testing::EmptyTestEventListener
{
private:
   std::stringstream sout;

public:
   // Called before a test starts.
   virtual void OnTestStart(const ::testing::TestInfo& test_info)
   {
      sout << test_info.test_case_name() << "." << test_info.name() << " Start." << std::endl;
   }

   // Called after a failed assertion or a SUCCEED() invocation.
   virtual void OnTestPartResult(const ::testing::TestPartResult& test_part_result)
   {
      sout << (test_part_result.failed() ? "====Failure=== " : "===Success=== ")
           << test_part_result.file_name() << " "
           << test_part_result.line_number() <<std::endl
           << test_part_result.summary() <<std::endl;
   }

   // Called after a test ends.
   virtual void OnTestEnd(const ::testing::TestInfo& test_info)
   {
      const int rank = CommunicatorType::GetRank(CommunicatorType::AllGroup);
      sout << test_info.test_case_name() << "." << test_info.name() << " End." <<std::endl;
      std::cout << rank << ":" << std::endl << sout.str()<< std::endl;
      sout.str( std::string() );
      sout.clear();
   }
};
#endif

#include "../GtestMissingError.h"
int main( int argc, char* argv[] )
{
#ifdef HAVE_GTEST
   ::testing::InitGoogleTest( &argc, argv );

   #ifdef HAVE_MPI
      ::testing::TestEventListeners& listeners =
         ::testing::UnitTest::GetInstance()->listeners();

      delete listeners.Release(listeners.default_result_printer());
      listeners.Append(new MinimalistBufferedPrinter);

      Communicators::ScopedInitializer< CommunicatorType > mpi(argc, argv);
   #endif
   return RUN_ALL_TESTS();
#else
   throw GtestMissingError();
#endif
}