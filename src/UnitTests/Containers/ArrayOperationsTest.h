/***************************************************************************
                          ArrayOperationsTest.h  -  description
                             -------------------
    begin                : Jul 15, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#ifdef HAVE_GTEST
#include <TNL/Allocators/Host.h>
#include <TNL/Allocators/Cuda.h>
#include <TNL/Containers/Algorithms/ArrayOperations.h>

#include "gtest/gtest.h"

using namespace TNL;
using namespace TNL::Containers;
using namespace TNL::Containers::Algorithms;

constexpr int ARRAY_TEST_SIZE = 5000;

// test fixture for typed tests
template< typename Value >
class ArrayOperationsTest : public ::testing::Test
{
protected:
   using ValueType = Value;
};

// types for which ArrayTest is instantiated
using ValueTypes = ::testing::Types< short int, int, long, float, double >;

TYPED_TEST_SUITE( ArrayOperationsTest, ValueTypes );

TYPED_TEST( ArrayOperationsTest, allocateMemory_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );
   ASSERT_NE( data, nullptr );

   allocator.deallocate( data, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, setMemoryElement_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );
   for( int i = 0; i < ARRAY_TEST_SIZE; i++ ) {
      ArrayOperations< Devices::Host >::setMemoryElement( data + i, (ValueType) i );
      EXPECT_EQ( data[ i ], i );
      EXPECT_EQ( ArrayOperations< Devices::Host >::getMemoryElement( data + i ), i );
   }
   allocator.deallocate( data, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, setMemory_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data, (ValueType) 13, ARRAY_TEST_SIZE );
   for( int i = 0; i < ARRAY_TEST_SIZE; i ++ )
      EXPECT_EQ( data[ i ], 13 );
   allocator.deallocate( data, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, copyMemory_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data1 = allocator.allocate( ARRAY_TEST_SIZE );
   ValueType* data2 = allocator.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data1, (ValueType) 13, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::copyMemory< ValueType, ValueType >( data2, data1, ARRAY_TEST_SIZE );
   for( int i = 0; i < ARRAY_TEST_SIZE; i ++ )
      EXPECT_EQ( data1[ i ], data2[ i ]);
   allocator.deallocate( data1, ARRAY_TEST_SIZE );
   allocator.deallocate( data2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, copyMemoryWithConversion_host )
{
   using Allocator1 = Allocators::Host< int >;
   using Allocator2 = Allocators::Host< float >;

   Allocator1 allocator1;
   Allocator2 allocator2;
   int* data1 = allocator1.allocate( ARRAY_TEST_SIZE );
   float* data2 = allocator2.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data1, 13, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::copyMemory< float, int >( data2, data1, ARRAY_TEST_SIZE );
   for( int i = 0; i < ARRAY_TEST_SIZE; i ++ )
      EXPECT_EQ( data1[ i ], data2[ i ] );
   allocator1.deallocate( data1, ARRAY_TEST_SIZE );
   allocator2.deallocate( data2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, compareMemory_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data1 = allocator.allocate( ARRAY_TEST_SIZE );
   ValueType* data2 = allocator.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data1, (ValueType) 7, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data2, (ValueType) 0, ARRAY_TEST_SIZE );
   EXPECT_FALSE( ( ArrayOperations< Devices::Host >::compareMemory< ValueType, ValueType >( data1, data2, ARRAY_TEST_SIZE ) ) );
   ArrayOperations< Devices::Host >::setMemory( data2, (ValueType) 7, ARRAY_TEST_SIZE );
   EXPECT_TRUE( ( ArrayOperations< Devices::Host >::compareMemory< ValueType, ValueType >( data1, data2, ARRAY_TEST_SIZE ) ) );
   allocator.deallocate( data1, ARRAY_TEST_SIZE );
   allocator.deallocate( data2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, compareMemoryWithConversion_host )
{
   using Allocator1 = Allocators::Host< int >;
   using Allocator2 = Allocators::Host< float >;

   Allocator1 allocator1;
   Allocator2 allocator2;
   int* data1 = allocator1.allocate( ARRAY_TEST_SIZE );
   float* data2 = allocator2.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data1, 7, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( data2, (float) 0.0, ARRAY_TEST_SIZE );
   EXPECT_FALSE( ( ArrayOperations< Devices::Host >::compareMemory< int, float >( data1, data2, ARRAY_TEST_SIZE ) ) );
   ArrayOperations< Devices::Host >::setMemory( data2, (float) 7.0, ARRAY_TEST_SIZE );
   EXPECT_TRUE( ( ArrayOperations< Devices::Host >::compareMemory< int, float >( data1, data2, ARRAY_TEST_SIZE ) ) );
   allocator1.deallocate( data1, ARRAY_TEST_SIZE );
   allocator2.deallocate( data2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, containsValue_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      data[ i ] = i % 10;
   for( int i = 0; i < 10; i++ )
      EXPECT_TRUE( ( ArrayOperations< Devices::Host >::containsValue( data, ARRAY_TEST_SIZE, (ValueType) i ) ) );
   for( int i = 10; i < 20; i++ )
      EXPECT_FALSE( ( ArrayOperations< Devices::Host >::containsValue( data, ARRAY_TEST_SIZE, (ValueType) i ) ) );

   allocator.deallocate( data, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, containsOnlyValue_host )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Host< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      data[ i ] = i % 10;
   for( int i = 0; i < 20; i++ )
      EXPECT_FALSE( ( ArrayOperations< Devices::Host >::containsOnlyValue( data, ARRAY_TEST_SIZE, (ValueType) i ) ) );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      data[ i ] = 10;
   EXPECT_TRUE( ( ArrayOperations< Devices::Host >::containsOnlyValue( data, ARRAY_TEST_SIZE, (ValueType) 10 ) ) );

   allocator.deallocate( data, ARRAY_TEST_SIZE );
}


#ifdef HAVE_CUDA
TYPED_TEST( ArrayOperationsTest, allocateMemory_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Cuda< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );
   ASSERT_NO_THROW( TNL_CHECK_CUDA_DEVICE );
   ASSERT_NE( data, nullptr );

   allocator.deallocate( data, ARRAY_TEST_SIZE );
   ASSERT_NO_THROW( TNL_CHECK_CUDA_DEVICE );
}

TYPED_TEST( ArrayOperationsTest, setMemoryElement_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using Allocator = Allocators::Cuda< ValueType >;

   Allocator allocator;
   ValueType* data = allocator.allocate( ARRAY_TEST_SIZE );
   ASSERT_NO_THROW( TNL_CHECK_CUDA_DEVICE );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      ArrayOperations< Devices::Cuda >::setMemoryElement( &data[ i ], (ValueType) i );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
   {
      ValueType d;
      ASSERT_EQ( cudaMemcpy( &d, &data[ i ], sizeof( ValueType ), cudaMemcpyDeviceToHost ), cudaSuccess );
      EXPECT_EQ( d, i );
      EXPECT_EQ( ArrayOperations< Devices::Cuda >::getMemoryElement( &data[ i ] ), i );
   }

   allocator.deallocate( data, ARRAY_TEST_SIZE );
   ASSERT_NO_THROW( TNL_CHECK_CUDA_DEVICE );
}

TYPED_TEST( ArrayOperationsTest, setMemory_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using HostAllocator = Allocators::Host< ValueType >;
   using CudaAllocator = Allocators::Cuda< ValueType >;

   HostAllocator hostAllocator;
   CudaAllocator cudaAllocator;
   ValueType* hostData = hostAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData = cudaAllocator.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( hostData, (ValueType) 0, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData, (ValueType) 13, ARRAY_TEST_SIZE );
   ASSERT_NO_THROW( TNL_CHECK_CUDA_DEVICE );
   ArrayOperations< Devices::Host, Devices::Cuda >::copyMemory< ValueType, ValueType >( hostData, deviceData, ARRAY_TEST_SIZE );
   ASSERT_NO_THROW( TNL_CHECK_CUDA_DEVICE );
   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      EXPECT_EQ( hostData[ i ], 13 );
   hostAllocator.deallocate( hostData, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, copyMemory_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using HostAllocator = Allocators::Host< ValueType >;
   using CudaAllocator = Allocators::Cuda< ValueType >;

   HostAllocator hostAllocator;
   CudaAllocator cudaAllocator;
   ValueType* hostData = hostAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* hostData2 = hostAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData = cudaAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData2 = cudaAllocator.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( hostData, (ValueType) 13, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< ValueType >( deviceData, hostData, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::copyMemory< ValueType, ValueType >( deviceData2, deviceData, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host, Devices::Cuda >::copyMemory< ValueType, ValueType >( hostData2, deviceData2, ARRAY_TEST_SIZE );
   EXPECT_TRUE( ( ArrayOperations< Devices::Host >::compareMemory< ValueType, ValueType >( hostData, hostData2, ARRAY_TEST_SIZE) ) );
   hostAllocator.deallocate( hostData, ARRAY_TEST_SIZE );
   hostAllocator.deallocate( hostData2, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, copyMemoryWithConversions_cuda )
{
   using HostAllocator1 = Allocators::Host< int >;
   using HostAllocator2 = Allocators::Host< double >;
   using CudaAllocator1 = Allocators::Cuda< long >;
   using CudaAllocator2 = Allocators::Cuda< float >;

   HostAllocator1 hostAllocator1;
   HostAllocator2 hostAllocator2;
   CudaAllocator1 cudaAllocator1;
   CudaAllocator2 cudaAllocator2;
   int* hostData = hostAllocator1.allocate( ARRAY_TEST_SIZE );
   double* hostData2 = hostAllocator2.allocate( ARRAY_TEST_SIZE );
   long* deviceData = cudaAllocator1.allocate( ARRAY_TEST_SIZE );
   float* deviceData2 = cudaAllocator2.allocate( ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host >::setMemory( hostData, 13, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< long, int >( deviceData, hostData, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::copyMemory< float, long >( deviceData2, deviceData, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Host, Devices::Cuda >::copyMemory< double, float >( hostData2, deviceData2, ARRAY_TEST_SIZE );
   for( int i = 0; i < ARRAY_TEST_SIZE; i ++ )
      EXPECT_EQ( hostData[ i ], hostData2[ i ] );
   hostAllocator1.deallocate( hostData, ARRAY_TEST_SIZE );
   hostAllocator2.deallocate( hostData2, ARRAY_TEST_SIZE );
   cudaAllocator1.deallocate( deviceData, ARRAY_TEST_SIZE );
   cudaAllocator2.deallocate( deviceData2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, compareMemory_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using HostAllocator = Allocators::Host< ValueType >;
   using CudaAllocator = Allocators::Cuda< ValueType >;

   HostAllocator hostAllocator;
   CudaAllocator cudaAllocator;
   ValueType* hostData = hostAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData = cudaAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData2 = cudaAllocator.allocate( ARRAY_TEST_SIZE );

   ArrayOperations< Devices::Host >::setMemory( hostData, (ValueType) 7, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData, (ValueType) 8, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData2, (ValueType) 9, ARRAY_TEST_SIZE );
   EXPECT_FALSE(( ArrayOperations< Devices::Host, Devices::Cuda >::compareMemory< ValueType, ValueType >( hostData, deviceData, ARRAY_TEST_SIZE ) ));
   EXPECT_FALSE(( ArrayOperations< Devices::Cuda, Devices::Host >::compareMemory< ValueType, ValueType >( deviceData, hostData, ARRAY_TEST_SIZE ) ));
   EXPECT_FALSE(( ArrayOperations< Devices::Cuda >::compareMemory< ValueType, ValueType >( deviceData, deviceData2, ARRAY_TEST_SIZE ) ));

   ArrayOperations< Devices::Cuda >::setMemory( deviceData, (ValueType) 7, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData2, (ValueType) 7, ARRAY_TEST_SIZE );
   EXPECT_TRUE(( ArrayOperations< Devices::Host, Devices::Cuda >::compareMemory< ValueType, ValueType >( hostData, deviceData, ARRAY_TEST_SIZE ) ));
   EXPECT_TRUE(( ArrayOperations< Devices::Cuda, Devices::Host >::compareMemory< ValueType, ValueType >( deviceData, hostData, ARRAY_TEST_SIZE ) ));
   EXPECT_TRUE(( ArrayOperations< Devices::Cuda >::compareMemory< ValueType, ValueType >( deviceData, deviceData2, ARRAY_TEST_SIZE ) ));

   hostAllocator.deallocate( hostData, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, compareMemoryWithConversions_cuda )
{
   using HostAllocator = Allocators::Host< int >;
   using CudaAllocator1 = Allocators::Cuda< float >;
   using CudaAllocator2 = Allocators::Cuda< double >;

   HostAllocator hostAllocator;
   CudaAllocator1 cudaAllocator1;
   CudaAllocator2 cudaAllocator2;
   int* hostData = hostAllocator.allocate( ARRAY_TEST_SIZE );
   float* deviceData = cudaAllocator1.allocate( ARRAY_TEST_SIZE );
   double* deviceData2 = cudaAllocator2.allocate( ARRAY_TEST_SIZE );

   ArrayOperations< Devices::Host >::setMemory( hostData, 7, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData, (float) 8, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData2, (double) 9, ARRAY_TEST_SIZE );
   EXPECT_FALSE(( ArrayOperations< Devices::Host, Devices::Cuda >::compareMemory< int, float >( hostData, deviceData, ARRAY_TEST_SIZE ) ));
   EXPECT_FALSE(( ArrayOperations< Devices::Cuda, Devices::Host >::compareMemory< float, int >( deviceData, hostData, ARRAY_TEST_SIZE ) ));
   EXPECT_FALSE(( ArrayOperations< Devices::Cuda >::compareMemory< float, double >( deviceData, deviceData2, ARRAY_TEST_SIZE ) ));

   ArrayOperations< Devices::Cuda >::setMemory( deviceData, (float) 7, ARRAY_TEST_SIZE );
   ArrayOperations< Devices::Cuda >::setMemory( deviceData2, (double) 7, ARRAY_TEST_SIZE );
   EXPECT_TRUE(( ArrayOperations< Devices::Host, Devices::Cuda >::compareMemory< int, float >( hostData, deviceData, ARRAY_TEST_SIZE ) ));
   EXPECT_TRUE(( ArrayOperations< Devices::Cuda, Devices::Host >::compareMemory< float, int >( deviceData, hostData, ARRAY_TEST_SIZE ) ));
   EXPECT_TRUE(( ArrayOperations< Devices::Cuda >::compareMemory< float, double >( deviceData, deviceData2, ARRAY_TEST_SIZE ) ));

   hostAllocator.deallocate( hostData, ARRAY_TEST_SIZE );
   cudaAllocator1.deallocate( deviceData, ARRAY_TEST_SIZE );
   cudaAllocator2.deallocate( deviceData2, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, containsValue_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using HostAllocator = Allocators::Host< ValueType >;
   using CudaAllocator = Allocators::Cuda< ValueType >;

   HostAllocator hostAllocator;
   CudaAllocator cudaAllocator;
   ValueType* hostData = hostAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData = cudaAllocator.allocate( ARRAY_TEST_SIZE );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      hostData[ i ] = i % 10;
   ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory( deviceData, hostData, ARRAY_TEST_SIZE );

   for( int i = 0; i < 10; i++ )
      EXPECT_TRUE( ( ArrayOperations< Devices::Cuda >::containsValue( deviceData, ARRAY_TEST_SIZE, (ValueType) i ) ) );
   for( int i = 10; i < 20; i++ )
      EXPECT_FALSE( ( ArrayOperations< Devices::Cuda >::containsValue( deviceData, ARRAY_TEST_SIZE, (ValueType) i ) ) );

   hostAllocator.deallocate( hostData, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData, ARRAY_TEST_SIZE );
}

TYPED_TEST( ArrayOperationsTest, containsOnlyValue_cuda )
{
   using ValueType = typename TestFixture::ValueType;
   using HostAllocator = Allocators::Host< ValueType >;
   using CudaAllocator = Allocators::Cuda< ValueType >;

   HostAllocator hostAllocator;
   CudaAllocator cudaAllocator;
   ValueType* hostData = hostAllocator.allocate( ARRAY_TEST_SIZE );
   ValueType* deviceData = cudaAllocator.allocate( ARRAY_TEST_SIZE );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      hostData[ i ] = i % 10;
   ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory( deviceData, hostData, ARRAY_TEST_SIZE );

   for( int i = 0; i < 20; i++ )
      EXPECT_FALSE( ( ArrayOperations< Devices::Cuda >::containsOnlyValue( deviceData, ARRAY_TEST_SIZE, (ValueType) i ) ) );

   for( int i = 0; i < ARRAY_TEST_SIZE; i++ )
      hostData[ i ] = 10;
   ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory( deviceData, hostData, ARRAY_TEST_SIZE );

   EXPECT_TRUE( ( ArrayOperations< Devices::Cuda >::containsOnlyValue( deviceData, ARRAY_TEST_SIZE, (ValueType) 10 ) ) );

   hostAllocator.deallocate( hostData, ARRAY_TEST_SIZE );
   cudaAllocator.deallocate( deviceData, ARRAY_TEST_SIZE );
}
#endif // HAVE_CUDA
#endif // HAVE_GTEST


#include "../main.h"
