/***************************************************************************
                          DistributedNDArrayOverlaps_semi1D_test.h  -  description
                             -------------------
    begin                : Dec 9, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

#ifdef HAVE_GTEST
#include <gtest/gtest.h>

#include <TNL/Communicators/MpiCommunicator.h>
#include <TNL/Communicators/NoDistrCommunicator.h>
#include <TNL/Communicators/ScopedInitializer.h>
#include <TNL/Containers/DistributedNDArray.h>
#include <TNL/Containers/DistributedNDArrayView.h>
#include <TNL/Containers/DistributedNDArraySynchronizer.h>
#include <TNL/Containers/ArrayView.h>
#include <TNL/Containers/Partitioner.h>

using namespace TNL;
using namespace TNL::Containers;

/*
 * Light check of DistributedNDArray.
 *
 * - Number of processes is not limited.
 * - Global size is hardcoded as 97 to force non-uniform distribution.
 * - Communication group is hardcoded as AllGroup -- it may be changed as needed.
 */
template< typename DistributedNDArray >
class DistributedNDArrayOverlaps_semi1D_test
: public ::testing::Test
{
protected:
   using ValueType = typename DistributedNDArray::ValueType;
   using DeviceType = typename DistributedNDArray::DeviceType;
   using CommunicatorType = typename DistributedNDArray::CommunicatorType;
   using IndexType = typename DistributedNDArray::IndexType;
   using DistributedNDArrayType = DistributedNDArray;

   // TODO: use ndarray
   using LocalArrayType = Array< ValueType, DeviceType, IndexType >;
   using LocalArrayViewType = ArrayView< ValueType, DeviceType, IndexType >;

   const int globalSize = 97;  // prime number to force non-uniform distribution
   const int overlaps = __ndarray_impl::get< 1 >( typename DistributedNDArray::OverlapsType{} );

   const typename CommunicatorType::CommunicationGroup group = CommunicatorType::AllGroup;

   DistributedNDArrayType distributedNDArray;

   const int rank = CommunicatorType::GetRank(group);
   const int nproc = CommunicatorType::GetSize(group);

   DistributedNDArrayOverlaps_semi1D_test()
   {
      using LocalRangeType = typename DistributedNDArray::LocalRangeType;
      const LocalRangeType localRange = Partitioner< IndexType, CommunicatorType >::splitRange( globalSize, group );
      distributedNDArray.setSizes( 0, globalSize, globalSize / 2 );
      distributedNDArray.template setDistribution< 1 >( localRange.getBegin(), localRange.getEnd(), group );
      distributedNDArray.allocate();

      EXPECT_EQ( distributedNDArray.template getLocalRange< 1 >(), localRange );
      EXPECT_EQ( distributedNDArray.getCommunicationGroup(), group );
   }
};

// types for which DistributedNDArrayOverlaps_semi1D_test is instantiated
using DistributedNDArrayTypes = ::testing::Types<
   DistributedNDArray< NDArray< double,
                                SizesHolder< int, 9, 0, 0 >,  // Q, X, Y
                                std::index_sequence< 0, 1, 2 >,  // permutation - should not matter
                                Devices::Host >,
                       Communicators::MpiCommunicator,
                       std::index_sequence< 0, 2, 0 > >
#ifdef HAVE_CUDA
   ,
   DistributedNDArray< NDArray< double,
                                SizesHolder< int, 9, 0, 0 >,  // Q, X, Y
                                std::index_sequence< 0, 1, 2 >,  // permutation - should not matter
                                Devices::Cuda >,
                       Communicators::MpiCommunicator,
                       std::index_sequence< 0, 2, 0 > >
#endif
>;

TYPED_TEST_SUITE( DistributedNDArrayOverlaps_semi1D_test, DistributedNDArrayTypes );

TYPED_TEST( DistributedNDArrayOverlaps_semi1D_test, checkSumOfLocalSizes )
{
   using CommunicatorType = typename TestFixture::CommunicatorType;

   const auto localRange = this->distributedNDArray.template getLocalRange< 1 >();
   const int localSize = localRange.getEnd() - localRange.getBegin();
   int sumOfLocalSizes = 0;
   CommunicatorType::Allreduce( &localSize, &sumOfLocalSizes, 1, MPI_SUM, this->group );
   EXPECT_EQ( sumOfLocalSizes, this->globalSize );
   EXPECT_EQ( this->distributedNDArray.template getSize< 1 >(), this->globalSize );

   EXPECT_EQ( this->distributedNDArray.getLocalStorageSize(), 9 * (2 * this->overlaps + localSize) * (this->globalSize / 2) );
}

// separate function because nvcc does not allow __cuda_callable__ lambdas inside
// private or protected methods (which are created by TYPED_TEST macro)
template< typename DistributedArray >
void test_helper_forLocalInternal( DistributedArray& a )
{
   using IndexType = typename DistributedArray::IndexType;

   const int overlaps = __ndarray_impl::get< 1 >( typename DistributedArray::OverlapsType{} );
   const auto localRange = a.template getLocalRange< 1 >();
   auto a_view = a.getView();

   auto setter = [=] __cuda_callable__ ( IndexType q, IndexType i, IndexType j ) mutable
   {
      a_view( q, i, j ) += 1;
   };

   a.setValue( 0 );
   a.forLocalInternal( setter );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getBegin() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() + overlaps; gi < localRange.getEnd() - overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd() - overlaps; gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;

   a.setValue( 0 );
   a_view.forLocalInternal( setter );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getBegin() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() + overlaps; gi < localRange.getEnd() - overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd() - overlaps; gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
}

TYPED_TEST( DistributedNDArrayOverlaps_semi1D_test, forLocalInternal )
{
   test_helper_forLocalInternal( this->distributedNDArray );
}

// separate function because nvcc does not allow __cuda_callable__ lambdas inside
// private or protected methods (which are created by TYPED_TEST macro)
template< typename DistributedArray >
void test_helper_forLocalBoundary( DistributedArray& a )
{
   using IndexType = typename DistributedArray::IndexType;

   const int overlaps = __ndarray_impl::get< 1 >( typename DistributedArray::OverlapsType{} );
   const auto localRange = a.template getLocalRange< 1 >();
   auto a_view = a.getView();

   auto setter = [=] __cuda_callable__ ( IndexType q, IndexType i, IndexType j ) mutable
   {
      a_view( q, i, j ) += 1;
   };

   a.setValue( 0 );
   a.forLocalBoundary( setter );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getBegin() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() + overlaps; gi < localRange.getEnd() - overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd() - overlaps; gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;

   a.setValue( 0 );
   a_view.forLocalBoundary( setter );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getBegin() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() + overlaps; gi < localRange.getEnd() - overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd() - overlaps; gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
}

TYPED_TEST( DistributedNDArrayOverlaps_semi1D_test, forLocalBoundary )
{
   test_helper_forLocalBoundary( this->distributedNDArray );
}

// separate function because nvcc does not allow __cuda_callable__ lambdas inside
// private or protected methods (which are created by TYPED_TEST macro)
template< typename DistributedArray >
void test_helper_forOverlaps( DistributedArray& a )
{
   using IndexType = typename DistributedArray::IndexType;

   const int overlaps = __ndarray_impl::get< 1 >( typename DistributedArray::OverlapsType{} );
   const auto localRange = a.template getLocalRange< 1 >();
   auto a_view = a.getView();

   auto setter = [=] __cuda_callable__ ( IndexType q, IndexType i, IndexType j ) mutable
   {
      a_view( q, i, j ) += 1;
   };

   a.setValue( 0 );
   a.forOverlaps( setter );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() - overlaps; gi < localRange.getBegin(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd(); gi < localRange.getEnd() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;

   a.setValue( 0 );
   a_view.forOverlaps( setter );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() - overlaps; gi < localRange.getBegin(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 0 )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd(); gi < localRange.getEnd() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), 1 )
            << "gi = " << gi;
}

TYPED_TEST( DistributedNDArrayOverlaps_semi1D_test, forOverlaps )
{
   test_helper_forOverlaps( this->distributedNDArray );
}

// separate function because nvcc does not allow __cuda_callable__ lambdas inside
// private or protected methods (which are created by TYPED_TEST macro)
template< typename DistributedArray >
void test_helper_synchronize( DistributedArray& a, const int rank, const int nproc )
{
   using IndexType = typename DistributedArray::IndexType;

   const int overlaps = __ndarray_impl::get< 1 >( typename DistributedArray::OverlapsType{} );
   const auto localRange = a.template getLocalRange< 1 >();
   auto a_view = a.getView();

   auto setter = [=] __cuda_callable__ ( IndexType q, IndexType i, IndexType j ) mutable
   {
      a_view( i ) = i;
   };

   a.setValue( -1 );
   a.forAll( setter );
   DistributedNDArraySynchronizer< DistributedArray > s1;
   s1.synchronize( a );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() - overlaps; gi < localRange.getBegin(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), gi + ((rank == 0) ? 97 : 0) )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), gi )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd(); gi < localRange.getEnd() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), gi - ((rank == nproc-1) ? 97 : 0) )
            << "gi = " << gi;

   a.setValue( -1 );
   a_view.forAll( setter );
   DistributedNDArraySynchronizer< decltype(a_view) > s2;
   s2.synchronize( a_view );

   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin() - overlaps; gi < localRange.getBegin(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), gi + ((rank == 0) ? 97 : 0) )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getBegin(); gi < localRange.getEnd(); gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), gi )
            << "gi = " << gi;
   for( int q = 0; q < 9; q++ )
   for( int gi = localRange.getEnd(); gi < localRange.getEnd() + overlaps; gi++ )
   for( int j = 0; j < a.template getSize< 2 >(); j++ )
      EXPECT_EQ( a.getElement( q, gi, j ), gi - ((rank == nproc-1) ? 97 : 0) )
            << "gi = " << gi;
}

//TYPED_TEST( DistributedNDArrayOverlaps_semi1D_test, synchronize )
//{
//   test_helper_synchronize( this->distributedNDArray, this->rank, this->nproc );
//}

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

#include "../../GtestMissingError.h"
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