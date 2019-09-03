/***************************************************************************
                          VectorTest-2.h  -  description
                             -------------------
    begin                : Oct 25, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#ifdef HAVE_GTEST
#include "VectorTestSetup.h"

// should be small enough to have fast tests, but larger than minGPUReductionDataSize
// and large enough to require multiple CUDA blocks for reduction
constexpr int VECTOR_TEST_SIZE = 10000;

TYPED_TEST( VectorTest, prefixSum )
{
   using VectorType = typename TestFixture::VectorType;
   using ViewType = typename TestFixture::ViewType;
   using RealType = typename VectorType::RealType;
   using DeviceType = typename VectorType::DeviceType;
   using IndexType = typename VectorType::IndexType;
   const int size = VECTOR_TEST_SIZE;

   // FIXME: tests should work in all cases
   if( std::is_same< RealType, float >::value )
      return;

   VectorType v( size );
   ViewType v_view( v );
   typename VectorType::HostType v_host( size );

   setConstantSequence( v, 0 );
   v_host = -1;
   v.prefixSum();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

   setConstantSequence( v, 1 );
   v_host = -1;
   v.prefixSum();
   v_host = v_view;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], i + 1 ) << "i = " << i;

   setLinearSequence( v );
   v_host = -1;
   v.prefixSum();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], (i * (i + 1)) / 2 ) << "i = " << i;

   // test views
   setConstantSequence( v, 0 );
   v_host = -1;
   v_view.prefixSum();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

   setConstantSequence( v, 1 );
   v_host = -1;
   v_view.prefixSum();
   v_host = v_view;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], i + 1 ) << "i = " << i;

   setLinearSequence( v );
   v_host = -1;
   v_view.prefixSum();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], (i * (i + 1)) / 2 ) << "i = " << i;

   ////
   // With CUDA, perform tests with multiple CUDA grids.
   if( std::is_same< DeviceType, Devices::Cuda >::value )
   {
#ifdef HAVE_CUDA
      Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::maxGridSize() = 3;

      setConstantSequence( v, 0 );
      v_host = -1;
      v.prefixSum();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::gridsCount() ), 1  );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

      setConstantSequence( v, 1 );
      v_host = -1;
      v.prefixSum();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::gridsCount() ), 1  );
      v_host = v_view;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], i + 1 ) << "i = " << i;

      setLinearSequence( v );
      v_host = -1;
      v.prefixSum();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::gridsCount() ), 1  );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], (i * (i + 1)) / 2 ) << "i = " << i;

      // test views
      setConstantSequence( v, 0 );
      v_host = -1;
      v_view.prefixSum();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::gridsCount() ), 1  );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

      setConstantSequence( v, 1 );
      v_host = -1;
      v_view.prefixSum();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::gridsCount() ), 1  );
      v_host = v_view;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], i + 1 ) << "i = " << i;

      setLinearSequence( v );
      v_host = -1;
      v_view.prefixSum();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::gridsCount() ), 1  );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], (i * (i + 1)) / 2 ) << "i = " << i;

      Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Inclusive, RealType, IndexType >::resetMaxGridSize();
#endif
   }
}

TYPED_TEST( VectorTest, exclusiveScan )
{
   using VectorType = typename TestFixture::VectorType;
   using ViewType = typename TestFixture::ViewType;
   using RealType = typename VectorType::RealType;
   using DeviceType = typename VectorType::DeviceType;
   using IndexType = typename VectorType::IndexType;
   const int size = VECTOR_TEST_SIZE;

   // FIXME: tests should work in all cases
   if( std::is_same< RealType, float >::value )
      return;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );
   typename VectorType::HostType v_host( size );

   setConstantSequence( v, 0 );
   v_host = -1;
   v.template prefixSum< Algorithms::ScanType::Exclusive >();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

   setConstantSequence( v, 1 );
   v_host = -1;
   v.template prefixSum< Algorithms::ScanType::Exclusive >();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], i ) << "i = " << i;

   setLinearSequence( v );
   v_host = -1;
   v.template prefixSum< Algorithms::ScanType::Exclusive >();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], (i * (i - 1)) / 2 ) << "i = " << i;

   // test views
   setConstantSequence( v, 0 );
   v_host = -1;
   v_view.template prefixSum< Algorithms::ScanType::Exclusive >();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

   setConstantSequence( v, 1 );
   v_host = -1;
   v_view.template prefixSum< Algorithms::ScanType::Exclusive >();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], i ) << "i = " << i;

   setLinearSequence( v );
   v_host = -1;
   v_view.template prefixSum< Algorithms::ScanType::Exclusive >();
   v_host = v;
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_host[ i ], (i * (i - 1)) / 2 ) << "i = " << i;

   ////
   // With CUDA, perform tests with multiple CUDA grids.
   if( std::is_same< DeviceType, Devices::Cuda >::value )
   {
#ifdef HAVE_CUDA
      Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::maxGridSize() = 3;

      setConstantSequence( v, 0 );
      v_host = -1;
      v.template prefixSum< Algorithms::ScanType::Exclusive >();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::gridsCount() ), 1 );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

      setConstantSequence( v, 1 );
      v_host = -1;
      v.template prefixSum< Algorithms::ScanType::Exclusive >();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::gridsCount() ), 1 );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], i ) << "i = " << i;

      setLinearSequence( v );
      v_host = -1;
      v.template prefixSum< Algorithms::ScanType::Exclusive >();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::gridsCount() ), 1 );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], (i * (i - 1)) / 2 ) << "i = " << i;

      // test views
      setConstantSequence( v, 0 );
      v_host = -1;
      v_view.template prefixSum< Algorithms::ScanType::Exclusive >();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::gridsCount() ), 1 );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], 0 ) << "i = " << i;

      setConstantSequence( v, 1 );
      v_host = -1;
      v_view.template prefixSum< Algorithms::ScanType::Exclusive >();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::gridsCount() ), 1 );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], i ) << "i = " << i;

      setLinearSequence( v );
      v_host = -1;
      v_view.template prefixSum< Algorithms::ScanType::Exclusive >();
      EXPECT_GT( ( Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::gridsCount() ), 1 );
      v_host = v;
      for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_host[ i ], (i * (i - 1)) / 2 ) << "i = " << i;

      Algorithms::CudaScanKernelLauncher< Algorithms::ScanType::Exclusive, RealType, IndexType >::resetMaxGridSize();
#endif
   }
}

// TODO: test prefix sum with custom begin and end parameters


template< typename FlagsView >
void setupFlags( FlagsView& f )
{
   auto f1 = [] __cuda_callable__ ( typename FlagsView::IndexType i ) { return ( i % 5 ) == 0; };
   f.evaluate( f1 );
}

/*
TYPED_TEST( VectorTest, segmentedScan )
{
   using VectorType = typename TestFixture::VectorType;
   using ViewType = typename TestFixture::ViewType;
   using RealType = typename VectorType::RealType;
   using DeviceType = typename VectorType::DeviceType;
   using IndexType = typename VectorType::IndexType;
   using FlagsArrayType = Array< bool, DeviceType, IndexType >;
   using FlagsViewType = ArrayView< bool, DeviceType, IndexType >;
   const int size = VECTOR_TEST_SIZE;

   VectorType v( size );
   ViewType v_view( v );

   FlagsArrayType flags( size ), flags_copy( size );
   FlagsViewType flags_view( flags );
   //auto f1 = [] __cuda_callable__ ( IndexType i ) { return ( i % 5 ) == 0; };
   //flags_view.evaluate( f1 );
   setupFlags( flags_view );
   flags_copy = flags_view;

   v = 0;
   v.computeSegmentedScan( flags_view );
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), 0 );
   flags_view = flags_copy;

   v = 1;
   v.computeSegmentedScan( flags_view );
   for( int i = 0; i < size; i++ )
         EXPECT_EQ( v.getElement( i ), ( i % 5 ) + 1 );
   flags_view = flags_copy;

   setLinearSequence( v );
   v.computeSegmentedScan( flags_view );
   for( int i = 1; i < size; i++ )
   {
      if( flags.getElement( i ) )
         EXPECT_EQ( v.getElement( i ), i );
      else
         EXPECT_EQ( v.getElement( i ) - v.getElement( i - 1 ), i );
   }
   flags_view = flags_copy;

   v_view = 0;
   v_view.computeSegmentedScan( flags_view );
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v_view.getElement( i ), 0 );
   flags_view = flags_copy;

   v_view = 1;
   v_view.computeSegmentedScan( flags_view );
   for( int i = 0; i < size; i++ )
         EXPECT_EQ( v_view.getElement( i ), ( i % 5 ) + 1 );
   flags_view = flags_copy;

   //v_view.evaluate( [] __cuda_callable__ ( IndexType i ) { return i; } );
   setLinearSequence( v );
   v_view.computeSegmentedScan( flags_view );
   for( int i = 1; i < size; i++ )
   {
      if( flags.getElement( i ) )
         EXPECT_EQ( v_view.getElement( i ), i );
      else
         EXPECT_EQ( v_view.getElement( i ) - v_view.getElement( i - 1 ), i );
   }
}
*/

#endif // HAVE_GTEST

#include "../main.h"