/***************************************************************************
                          SparseMatrixCopyTest.h -  description
                             -------------------
    begin                : Jun 25, 2017
    copyright            : (C) 2017 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <TNL/Matrices/CSR.h>
#include <TNL/Matrices/Ellpack.h>
#include <TNL/Matrices/SlicedEllpack.h>

using CSR_host = TNL::Matrices::CSR< int, TNL::Devices::Host, int >;
using CSR_cuda = TNL::Matrices::CSR< int, TNL::Devices::Cuda, int >;
using E_host = TNL::Matrices::Ellpack< int, TNL::Devices::Host, int >;
using E_cuda = TNL::Matrices::Ellpack< int, TNL::Devices::Cuda, int >;
using SE_host = TNL::Matrices::SlicedEllpack< int, TNL::Devices::Host, int, 2 >;
using SE_cuda = TNL::Matrices::SlicedEllpack< int, TNL::Devices::Cuda, int, 2 >;

#ifdef HAVE_GTEST 
#include <gtest/gtest.h>

/*
 * Sets up the following 7x6 sparse matrix:
 *
 *    / 1  2             \
 *    | 3  4  5          |
 *    |    6  7  8       |
 *    |       9 10 11    |
 *    |         12 13 14 |
 *    |            15 16 |
 *    \               17 /
 */
template< typename Matrix >
void setupMatrix( Matrix& m )
{
   const int rows = 7;
   const int cols = 6;
   m.reset();
   m.setDimensions( rows, cols );
   typename Matrix::CompressedRowLengthsVector rowLengths;
   rowLengths.setSize( rows );
   rowLengths.setValue( 3 );
   rowLengths.setElement( 0 , 4 );
   rowLengths.setElement( 1,  4 );
   m.setCompressedRowLengths( rowLengths );

   int value = 1;
   for( int i = 0; i < rows; i++ )
      for( int j = 0; j < 3; j++ )
         if( i + j - 1 >= 0 && i + j - 1 < cols )
            m.setElement( i, i + j - 1, value++ );
}

template< typename Matrix >
void checkMatrix( Matrix& m )
{
   ASSERT_EQ( m.getRows(), 7 );
   ASSERT_EQ( m.getColumns(), 6 );

   EXPECT_EQ( m.getElement( 0, 0 ),  1 );
   EXPECT_EQ( m.getElement( 0, 1 ),  2 );
   EXPECT_EQ( m.getElement( 0, 2 ),  0 );
   EXPECT_EQ( m.getElement( 0, 3 ),  0 );
   EXPECT_EQ( m.getElement( 0, 4 ),  0 );
   EXPECT_EQ( m.getElement( 0, 5 ),  0 );

   EXPECT_EQ( m.getElement( 1, 0 ),  3 );
   EXPECT_EQ( m.getElement( 1, 1 ),  4 );
   EXPECT_EQ( m.getElement( 1, 2 ),  5 );
   EXPECT_EQ( m.getElement( 1, 3 ),  0 );
   EXPECT_EQ( m.getElement( 1, 4 ),  0 );
   EXPECT_EQ( m.getElement( 1, 5 ),  0 );

   EXPECT_EQ( m.getElement( 2, 0 ),  0 );
   EXPECT_EQ( m.getElement( 2, 1 ),  6 );
   EXPECT_EQ( m.getElement( 2, 2 ),  7 );
   EXPECT_EQ( m.getElement( 2, 3 ),  8 );
   EXPECT_EQ( m.getElement( 2, 4 ),  0 );
   EXPECT_EQ( m.getElement( 2, 5 ),  0 );

   EXPECT_EQ( m.getElement( 3, 0 ),  0 );
   EXPECT_EQ( m.getElement( 3, 1 ),  0 );
   EXPECT_EQ( m.getElement( 3, 2 ),  9 );
   EXPECT_EQ( m.getElement( 3, 3 ), 10 );
   EXPECT_EQ( m.getElement( 3, 4 ), 11 );
   EXPECT_EQ( m.getElement( 3, 5 ),  0 );

   EXPECT_EQ( m.getElement( 4, 0 ),  0 );
   EXPECT_EQ( m.getElement( 4, 1 ),  0 );
   EXPECT_EQ( m.getElement( 4, 2 ),  0 );
   EXPECT_EQ( m.getElement( 4, 3 ), 12 );
   EXPECT_EQ( m.getElement( 4, 4 ), 13 );
   EXPECT_EQ( m.getElement( 4, 5 ), 14 );

   EXPECT_EQ( m.getElement( 5, 0 ),  0 );
   EXPECT_EQ( m.getElement( 5, 1 ),  0 );
   EXPECT_EQ( m.getElement( 5, 2 ),  0 );
   EXPECT_EQ( m.getElement( 5, 3 ),  0 );
   EXPECT_EQ( m.getElement( 5, 4 ), 15 );
   EXPECT_EQ( m.getElement( 5, 5 ), 16 );

   EXPECT_EQ( m.getElement( 6, 0 ),  0 );
   EXPECT_EQ( m.getElement( 6, 1 ),  0 );
   EXPECT_EQ( m.getElement( 6, 2 ),  0 );
   EXPECT_EQ( m.getElement( 6, 3 ),  0 );
   EXPECT_EQ( m.getElement( 6, 4 ),  0 );
   EXPECT_EQ( m.getElement( 6, 5 ), 17 );
}

template< typename Matrix1, typename Matrix2 >
void testMatrixCopy()
{
   Matrix1 m1;
   setupMatrix( m1 );
   checkMatrix( m1 );

   Matrix2 m2;
   m2 = m1;
   checkMatrix( m2 );
}


TEST( SparseMatrixCopyTest, CSR_HostToHost )
{
   testMatrixCopy< CSR_host, CSR_host >();
}

#ifdef HAVE_CUDA
TEST( SparseMatrixCopyTest, CSR_HostToCuda )
{
   testMatrixCopy< CSR_host, CSR_cuda >();
}

TEST( SparseMatrixCopyTest, CSR_CudaToHost )
{
   testMatrixCopy< CSR_cuda, CSR_host >();
}

TEST( SparseMatrixCopyTest, CSR_CudaToCuda )
{
   testMatrixCopy< CSR_cuda, CSR_cuda >();
}
#endif


TEST( SparseMatrixCopyTest, Ellpack_HostToHost )
{
   testMatrixCopy< E_host, E_host >();
}

#ifdef HAVE_CUDA
TEST( SparseMatrixCopyTest, Ellpack_HostToCuda )
{
   testMatrixCopy< E_host, E_cuda >();
}

TEST( SparseMatrixCopyTest, Ellpack_CudaToHost )
{
   testMatrixCopy< E_cuda, E_host >();
}

TEST( SparseMatrixCopyTest, Ellpack_CudaToCuda )
{
   testMatrixCopy< E_cuda, E_cuda >();
}
#endif


TEST( SparseMatrixCopyTest, SlicedEllpack_HostToHost )
{
   testMatrixCopy< SE_host, SE_host >();
}

#ifdef HAVE_CUDA
TEST( SparseMatrixCopyTest, SlicedEllpack_HostToCuda )
{
   testMatrixCopy< SE_host, SE_cuda >();
}

TEST( SparseMatrixCopyTest, SlicedEllpack_CudaToHost )
{
   testMatrixCopy< SE_cuda, SE_host >();
}

TEST( SparseMatrixCopyTest, SlicedEllpack_CudaToCuda )
{
   testMatrixCopy< SE_cuda, SE_cuda >();
}
#endif

#endif

int main( int argc, char* argv[] )
{
#ifdef HAVE_GTEST
   ::testing::InitGoogleTest( &argc, argv );
   return RUN_ALL_TESTS();
#else
   return EXIT_FAILURE;
#endif
}
