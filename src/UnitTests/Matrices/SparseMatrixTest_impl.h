/***************************************************************************
                          SparseMatrixTest_impl.h -  description
                             -------------------
    begin                : Nov 22, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// TODO
/*
 * getType()                        ::HOW?  How to test this for each format? edit string how?
 *      Found the mistake for Cuda instead of Devices::Cuda. Incorrect String in src/TNL/Devices/Cuda.cpp
 *      MISSING: indexType is missing in CSR_impl.h
 * getTypeVirtual()                 ::TEST? This just calls getType().
 * getSerializationType()           ::TEST? This just calls HostType::getType().
 * getSerializationTypeVirtual()    ::TEST? This just calls getSerializationType().
 * setDimensions()                      ::DONE
 * setCompressedRowLengths()            ::DONE
 * getRowLength()                   ::USED! In test_SetCompressedRowLengths() to verify the test itself.
 * getRowLengthFast()               ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * setLike()                            ::DONE
 * reset()                              ::DONE
 * setElementFast()                 ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * setElement()                         ::DONE
 * addElementFast()                 ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * addElement()                         ::DONE
 * setRowFast()                     ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * setRow()                             ::DONE
 *      MISTAKE!!! In SlicedEllpack: addElement(), line 263, "column <= this->rows" shouldn't it be: "column <= this->columns", otherwise test_SetRow causes the assertion to fail.
 * addRowFast()                     ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * addRow()                         ::NOT IMPLEMENTED! This calls addRowFast() which isn't implemented. Implement? Is it supposed to add an extra row to the matrix or add elements of a row to another row in the matrix?
 * getElementFast()                 ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * getElement()                     ::USED! In test_SetElement(), test_AddElement() and test_setRow() to verify the test itself.
 * getRowFast()                     ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * MatrixRow getRow()               ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * ConstMatrixRow getRow()          ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * rowVectorProduct()               ::TEST? How to test __cuda_callable__? ONLY TEST ON CPU FOR NOW
 * vectorProduct()                      ::DONE
 *      This used to throw illegal memory access, but instead of using ints for vectors, using Types, helped.
 * addMatrix()                      ::NOT IMPLEMENTED!
 * getTransposition()               ::NOT IMPLMENETED!
 * performSORIteration()            ::HOW? Throws segmentation fault CUDA.
 * operator=()                      ::HOW? What is this supposed to enable? Overloading operators?
 * save( File& file)                ::USED! In save( String& fileName )
 * load( File& file )               ::USED! In load( String& fileName )
 * save( String& fileName )             ::DONE
 * load( String& fileName )             ::DONE
 * print()                              ::DONE
 * setCudaKernelType()              ::NOT SUPPOSED TO TEST! via notes from 1.11.2018 supervisor meeting.
 * getCudaKernelType()              ::NOT SUPPOSED TO TEST! via notes from 1.11.2018 supervisor meeting.
 * setCudaWarpSize()                ::NOT SUPPOSED TO TEST! via notes from 1.11.2018 supervisor meeting.
 * getCudaWarpSize()                ::NOT SUPPOSED TO TEST! via notes from 1.11.2018 supervisor meeting.
 * setHybridModeSplit()             ::NOT SUPPOSED TO TEST! via notes from 1.11.2018 supervisor meeting.
 * getHybridModeSplit()             ::NOT SUPPOSED TO TEST! via notes from 1.11.2018 supervisor meeting.
 * spmvCudaVectorized()             ::TEST? How to test __device__?
 * vectorProductCuda()              ::TEST? How to test __device__?
 */

// GENERAL TODO
/*
 * For every function, EXPECT_EQ needs to be done, even for zeros in matrices.
 * Figure out __cuda_callable_. When trying to call __cuda_callable__ functions
 *      a segmentation fault (core dumped) is thrown.
 *  ==>__cuda_callable__ works only for CPU at the moment. (for loops vs thread kernel assignment)
 */

#include <TNL/Containers/Vector.h>
#include <TNL/Containers/VectorView.h>
#include <TNL/Math.h>
#include <iostream>

#ifdef HAVE_GTEST 
#include <gtest/gtest.h>

template< typename MatrixHostFloat, typename MatrixHostInt >
void host_test_GetType()
{
    MatrixHostFloat mtrxHostFloat;
    MatrixHostInt mtrxHostInt;
    
    
    EXPECT_EQ( mtrxHostFloat.getType(), TNL::String( "Matrices::CSR< float, Devices::Host >" ) );
    EXPECT_EQ( mtrxHostInt.getType(), TNL::String( "Matrices::CSR< int, Devices::Host >" ) );
}

template< typename MatrixCudaFloat, typename MatrixCudaInt >
void cuda_test_GetType()
{
    MatrixCudaFloat mtrxCudaFloat;
    MatrixCudaInt mtrxCudaInt;
    

    EXPECT_EQ( mtrxCudaFloat.getType(), TNL::String( "Matrices::CSR< float, Cuda >" ) );    // This is mistakenly labeled in /src/TNL/Devices/Cuda.cpp
    EXPECT_EQ( mtrxCudaInt.getType(), TNL::String( "Matrices::CSR< int, Cuda >" ) );        // Should be Devices::Cuda
}

template< typename Matrix >
void test_SetDimensions()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
    const IndexType rows = 9;
    const IndexType cols = 8;
    
    Matrix m;
    m.setDimensions( rows, cols );
    
    EXPECT_EQ( m.getRows(), 9 );
    EXPECT_EQ( m.getColumns(), 8 );
}

template< typename Matrix >
void test_SetCompressedRowLengths()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
    const IndexType rows = 10;
    const IndexType cols = 11;
    
    Matrix m;
    m.reset();
    m.setDimensions( rows, cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( rows );
    rowLengths.setValue( 3 );
    
    IndexType value = 1;
    for( IndexType i = 2; i < rows; i++ )
        rowLengths.setElement( i, value++ );
    
    m.setCompressedRowLengths( rowLengths );
    
    
    if( m.getType() == TNL::String( TNL::String( "Matrices::CSR< ") +
                       TNL::String( TNL::getType< RealType >() ) +
                       TNL::String( ", " ) +
                       TNL::String( Matrix::DeviceType::getDeviceType() ) +
                       TNL::String( ", " ) +
                       TNL::String( TNL::getType< IndexType >() ) +
                       TNL::String( " >" ) )
      )
    {
        EXPECT_EQ( m.getRowLength( 0 ), 3 );
        EXPECT_EQ( m.getRowLength( 1 ), 3 );
        EXPECT_EQ( m.getRowLength( 2 ), 1 );
        EXPECT_EQ( m.getRowLength( 3 ), 2 );
        EXPECT_EQ( m.getRowLength( 4 ), 3 );
        EXPECT_EQ( m.getRowLength( 5 ), 4 );
        EXPECT_EQ( m.getRowLength( 6 ), 5 );
        EXPECT_EQ( m.getRowLength( 7 ), 6 );
        EXPECT_EQ( m.getRowLength( 8 ), 7 );
        EXPECT_EQ( m.getRowLength( 9 ), 8 );
    }
    else if( m.getType() == TNL::String( TNL::String( "Matrices::Ellpack< ") +
                            TNL::String( TNL::getType< RealType >() ) +
                            TNL::String( ", " ) +
                            TNL::String( Matrix::DeviceType::getDeviceType() ) +
                            TNL::String( ", " ) +
                            TNL::String( TNL::getType< IndexType >() ) +
                            TNL::String( " >" ) ) 
                            || 
             m.getType() == TNL::String( TNL::String( "Matrices::SlicedEllpack< ") +
                            TNL::String( TNL::getType< RealType >() ) +
                            TNL::String( ", " ) +
                            TNL::String( Matrix::DeviceType::getDeviceType() ) +
                            TNL::String( " >" ) )
           )
    {
        EXPECT_EQ( m.getRowLength( 0 ), 8 );
        EXPECT_EQ( m.getRowLength( 1 ), 8 );
        EXPECT_EQ( m.getRowLength( 2 ), 8 );
        EXPECT_EQ( m.getRowLength( 3 ), 8 );
        EXPECT_EQ( m.getRowLength( 4 ), 8 );
        EXPECT_EQ( m.getRowLength( 5 ), 8 );
        EXPECT_EQ( m.getRowLength( 6 ), 8 );
        EXPECT_EQ( m.getRowLength( 7 ), 8 );
        EXPECT_EQ( m.getRowLength( 8 ), 8 );
        EXPECT_EQ( m.getRowLength( 9 ), 8 );
    }
    else
    {
        EXPECT_EQ( m.getRowLength( 0 ), 3 );
        EXPECT_EQ( m.getRowLength( 1 ), 3 );
        EXPECT_EQ( m.getRowLength( 2 ), 1 );
        EXPECT_EQ( m.getRowLength( 3 ), 2 );
        EXPECT_EQ( m.getRowLength( 4 ), 3 );
        EXPECT_EQ( m.getRowLength( 5 ), 4 );
        EXPECT_EQ( m.getRowLength( 6 ), 5 );
        EXPECT_EQ( m.getRowLength( 7 ), 6 );
        EXPECT_EQ( m.getRowLength( 8 ), 7 );
        EXPECT_EQ( m.getRowLength( 9 ), 8 );
    }
}

template< typename Matrix1, typename Matrix2 >
void test_SetLike()
{
    using RealType = typename Matrix1::RealType;
    using DeviceType = typename Matrix1::DeviceType;
    using IndexType = typename Matrix1::IndexType;
        
    const IndexType rows = 8;
    const IndexType cols = 7;
    
    Matrix1 m1;
    m1.reset();
    m1.setDimensions( rows + 1, cols + 2 );
    
    Matrix2 m2;
    m2.reset();
    m2.setDimensions( rows, cols );
    
    m1.setLike( m2 );
    
    
    EXPECT_EQ( m1.getRows(), m2.getRows() );
    EXPECT_EQ( m1.getColumns(), m2.getColumns() );
}

template< typename Matrix >
void test_Reset()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 5x4 sparse matrix:
 *
 *    /  0  0  0  0 \
 *    |  0  0  0  0 |
 *    |  0  0  0  0 |
 *    |  0  0  0  0 |
 *    \  0  0  0  0 /
 */
    
    const IndexType rows = 5;
    const IndexType cols = 4;
    
    Matrix m;
    m.setDimensions( rows, cols );
    
    m.reset();
    
    
    EXPECT_EQ( m.getRows(), 0 );
    EXPECT_EQ( m.getColumns(), 0 );
}

template< typename Matrix >
void test_SetElement()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 5x5 sparse matrix:
 *
 *    /  1  0  0  0  0 \
 *    |  0  2  0  0  0 |
 *    |  0  0  3  0  0 |
 *    |  0  0  0  4  0 |
 *    \  0  0  0  0  5 /
 */
    
    const IndexType rows = 5;
    const IndexType cols = 5;
    
    Matrix m;
    m.reset();
    m.setDimensions( rows, cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( rows );
    rowLengths.setValue( 1 );
    m.setCompressedRowLengths( rowLengths );    
    
    RealType value = 1;
    for( IndexType i = 0; i < rows; i++ )
        m.setElement( i, i, value++ );
    
    
    EXPECT_EQ( m.getElement( 0, 0 ), 1 );
    EXPECT_EQ( m.getElement( 0, 1 ), 0 );
    EXPECT_EQ( m.getElement( 0, 2 ), 0 );
    EXPECT_EQ( m.getElement( 0, 3 ), 0 );
    EXPECT_EQ( m.getElement( 0, 4 ), 0 );
    
    EXPECT_EQ( m.getElement( 1, 0 ), 0 );
    EXPECT_EQ( m.getElement( 1, 1 ), 2 );
    EXPECT_EQ( m.getElement( 1, 2 ), 0 );
    EXPECT_EQ( m.getElement( 1, 3 ), 0 );
    EXPECT_EQ( m.getElement( 1, 4 ), 0 );
    
    EXPECT_EQ( m.getElement( 2, 0 ), 0 );
    EXPECT_EQ( m.getElement( 2, 1 ), 0 );
    EXPECT_EQ( m.getElement( 2, 2 ), 3 );
    EXPECT_EQ( m.getElement( 2, 3 ), 0 );
    EXPECT_EQ( m.getElement( 2, 4 ), 0 );
    
    EXPECT_EQ( m.getElement( 3, 0 ), 0 );
    EXPECT_EQ( m.getElement( 3, 1 ), 0 );
    EXPECT_EQ( m.getElement( 3, 2 ), 0 );
    EXPECT_EQ( m.getElement( 3, 3 ), 4 );
    EXPECT_EQ( m.getElement( 3, 4 ), 0 );
    
    EXPECT_EQ( m.getElement( 4, 0 ), 0 );
    EXPECT_EQ( m.getElement( 4, 1 ), 0 );
    EXPECT_EQ( m.getElement( 4, 2 ), 0 );
    EXPECT_EQ( m.getElement( 4, 3 ), 0 );
    EXPECT_EQ( m.getElement( 4, 4 ), 5 );
}

template< typename Matrix >
void test_AddElement()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 6x5 sparse matrix:
 *
 *    /  1  2  3  0  0 \
 *    |  0  4  5  6  0 |
 *    |  0  0  7  8  9 |
 *    | 10  0  0  0  0 |
 *    |  0 11  0  0  0 |
 *    \  0  0  0 12  0 /
 */
    
    const IndexType rows = 6;
    const IndexType cols = 5;
    
    Matrix m;
    m.reset();
    m.setDimensions( rows, cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( rows );
    rowLengths.setValue( 3 );
    m.setCompressedRowLengths( rowLengths );
    
    RealType value = 1;
    for( IndexType i = 0; i < cols - 2; i++ )     // 0th row
        m.setElement( 0, i, value++ );
    
    for( IndexType i = 1; i < cols - 1; i++ )     // 1st row
        m.setElement( 1, i, value++ );
        
    for( IndexType i = 2; i < cols; i++ )         // 2nd row
        m.setElement( 2, i, value++ );
        
        m.setElement( 3, 0, value++ );      // 3rd row
        
        m.setElement( 4, 1, value++ );      // 4th row
    
        m.setElement( 5, 3, value++ );      // 5th row
    
        
    // Check the set elements
    EXPECT_EQ( m.getElement( 0, 0 ),  1 );
    EXPECT_EQ( m.getElement( 0, 1 ),  2 );
    EXPECT_EQ( m.getElement( 0, 2 ),  3 );
    EXPECT_EQ( m.getElement( 0, 3 ),  0 );
    EXPECT_EQ( m.getElement( 0, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 1, 0 ),  0 );
    EXPECT_EQ( m.getElement( 1, 1 ),  4 );
    EXPECT_EQ( m.getElement( 1, 2 ),  5 );
    EXPECT_EQ( m.getElement( 1, 3 ),  6 );
    EXPECT_EQ( m.getElement( 1, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 2, 0 ),  0 );
    EXPECT_EQ( m.getElement( 2, 1 ),  0 );
    EXPECT_EQ( m.getElement( 2, 2 ),  7 );
    EXPECT_EQ( m.getElement( 2, 3 ),  8 );
    EXPECT_EQ( m.getElement( 2, 4 ),  9 );
    
    EXPECT_EQ( m.getElement( 3, 0 ), 10 );
    EXPECT_EQ( m.getElement( 3, 1 ),  0 );
    EXPECT_EQ( m.getElement( 3, 2 ),  0 );
    EXPECT_EQ( m.getElement( 3, 3 ),  0 );
    EXPECT_EQ( m.getElement( 3, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 4, 0 ),  0 );
    EXPECT_EQ( m.getElement( 4, 1 ), 11 );
    EXPECT_EQ( m.getElement( 4, 2 ),  0 );
    EXPECT_EQ( m.getElement( 4, 3 ),  0 );
    EXPECT_EQ( m.getElement( 4, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 5, 0 ),  0 );
    EXPECT_EQ( m.getElement( 5, 1 ),  0 );
    EXPECT_EQ( m.getElement( 5, 2 ),  0 );
    EXPECT_EQ( m.getElement( 5, 3 ), 12 );
    EXPECT_EQ( m.getElement( 5, 4 ),  0 );
    
    // Add new elements to the old elements with a multiplying factor applied to the old elements.

/*
 * The following setup results in the following 6x5 sparse matrix:
 *
 *    /  3  6  9  0  0 \
 *    |  0 12 15 18  0 |
 *    |  0  0 21 24 27 |
 *    | 30 11 12  0  0 |
 *    |  0 35 14 15  0 |
 *    \  0  0 16 41 18 /
 */
    
    RealType newValue = 1;
    for( IndexType i = 0; i < cols - 2; i++ )         // 0th row
        m.addElement( 0, i, newValue++, 2.0 );
    
    for( IndexType i = 1; i < cols - 1; i++ )         // 1st row
        m.addElement( 1, i, newValue++, 2.0 );
        
    for( IndexType i = 2; i < cols; i++ )             // 2nd row
        m.addElement( 2, i, newValue++, 2.0 );
        
    for( IndexType i = 0; i < cols - 2; i++ )         // 3rd row
        m.addElement( 3, i, newValue++, 2.0 );
    
    for( IndexType i = 1; i < cols - 1; i++ )         // 4th row
        m.addElement( 4, i, newValue++, 2.0 );
    
    for( IndexType i = 2; i < cols; i++ )             // 5th row
        m.addElement( 5, i, newValue++, 2.0 );
    
    
    EXPECT_EQ( m.getElement( 0, 0 ),  3 );
    EXPECT_EQ( m.getElement( 0, 1 ),  6 );
    EXPECT_EQ( m.getElement( 0, 2 ),  9 );
    EXPECT_EQ( m.getElement( 0, 3 ),  0 );
    EXPECT_EQ( m.getElement( 0, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 1, 0 ),  0 );
    EXPECT_EQ( m.getElement( 1, 1 ), 12 );
    EXPECT_EQ( m.getElement( 1, 2 ), 15 );
    EXPECT_EQ( m.getElement( 1, 3 ), 18 );
    EXPECT_EQ( m.getElement( 1, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 2, 0 ),  0 );
    EXPECT_EQ( m.getElement( 2, 1 ),  0 );
    EXPECT_EQ( m.getElement( 2, 2 ), 21 );
    EXPECT_EQ( m.getElement( 2, 3 ), 24 );
    EXPECT_EQ( m.getElement( 2, 4 ), 27 );
    
    EXPECT_EQ( m.getElement( 3, 0 ), 30 );
    EXPECT_EQ( m.getElement( 3, 1 ), 11 );
    EXPECT_EQ( m.getElement( 3, 2 ), 12 );
    EXPECT_EQ( m.getElement( 3, 3 ),  0 );
    EXPECT_EQ( m.getElement( 3, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 4, 0 ),  0 );
    EXPECT_EQ( m.getElement( 4, 1 ), 35 );
    EXPECT_EQ( m.getElement( 4, 2 ), 14 );
    EXPECT_EQ( m.getElement( 4, 3 ), 15 );
    EXPECT_EQ( m.getElement( 4, 4 ),  0 );
    
    EXPECT_EQ( m.getElement( 5, 0 ),  0 );
    EXPECT_EQ( m.getElement( 5, 1 ),  0 );
    EXPECT_EQ( m.getElement( 5, 2 ), 16 );
    EXPECT_EQ( m.getElement( 5, 3 ), 41 );
    EXPECT_EQ( m.getElement( 5, 4 ), 18 );
}

template< typename Matrix >
void test_SetRow()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 3x7 sparse matrix:
 *
 *    /  0  0  0  1  1  1  0 \
 *    |  2  2  2  0  0  0  0 |
 *    \  3  3  3  0  0  0  0 /
 */
    
    const IndexType rows = 3;
    const IndexType cols = 7;
    
    Matrix m;
    m.reset();
    m.setDimensions( rows, cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( rows );
    rowLengths.setValue( 6 );
    rowLengths.setElement( 1, 3 );
    m.setCompressedRowLengths( rowLengths );
    
    RealType value = 1;
    for( IndexType i = 0; i < 3; i++ )
    {
        m.setElement( 0, i + 3, value );
        m.setElement( 1, i, value + 1 );
        m.setElement( 2, i, value + 2 );
    }
    
    RealType row1 [ 3 ] = { 11, 11, 11 }; IndexType colIndexes1 [ 3 ] = { 0, 1, 2 };
    RealType row2 [ 3 ] = { 22, 22, 22 }; IndexType colIndexes2 [ 3 ] = { 0, 1, 2 };
    RealType row3 [ 3 ] = { 33, 33, 33 }; IndexType colIndexes3 [ 3 ] = { 3, 4, 5 };
    
    RealType row = 0;
    IndexType elements = 3;
    
    m.setRow( row++, colIndexes1, row1, elements );
    m.setRow( row++, colIndexes2, row2, elements );
    m.setRow( row++, colIndexes3, row3, elements );
    
    
    EXPECT_EQ( m.getElement( 0, 0 ), 11 );
    EXPECT_EQ( m.getElement( 0, 1 ), 11 );
    EXPECT_EQ( m.getElement( 0, 2 ), 11 );
    EXPECT_EQ( m.getElement( 0, 3 ),  0 );
    EXPECT_EQ( m.getElement( 0, 4 ),  0 );
    EXPECT_EQ( m.getElement( 0, 5 ),  0 );
    EXPECT_EQ( m.getElement( 0, 6 ),  0 );
    
    EXPECT_EQ( m.getElement( 1, 0 ), 22 );
    EXPECT_EQ( m.getElement( 1, 1 ), 22 );
    EXPECT_EQ( m.getElement( 1, 2 ), 22 );
    EXPECT_EQ( m.getElement( 1, 3 ),  0 );
    EXPECT_EQ( m.getElement( 1, 4 ),  0 );
    EXPECT_EQ( m.getElement( 1, 5 ),  0 );
    EXPECT_EQ( m.getElement( 1, 6 ),  0 );
    
    EXPECT_EQ( m.getElement( 2, 0 ),  0 );
    EXPECT_EQ( m.getElement( 2, 1 ),  0 );
    EXPECT_EQ( m.getElement( 2, 2 ),  0 );
    EXPECT_EQ( m.getElement( 2, 3 ), 33 );
    EXPECT_EQ( m.getElement( 2, 4 ), 33 );
    EXPECT_EQ( m.getElement( 2, 5 ), 33 );
    EXPECT_EQ( m.getElement( 2, 6 ),  0 );
}

template< typename Matrix >
void test_VectorProduct()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 5x4 sparse matrix:
 *
 *    /  1  2  3  0 \
 *    |  0  0  0  4 |
 *    |  5  6  7  0 |
 *    |  0  8  9 10 |
 *    \  0  0 11 12 /
 */
    
    const IndexType m_rows = 5;
    const IndexType m_cols = 4;
    
    Matrix m;
    m.reset();
    m.setDimensions( m_rows, m_cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( m_rows );
    rowLengths.setValue( 3 );
    m.setCompressedRowLengths( rowLengths );
    
    RealType value = 1;
    for( IndexType i = 0; i < m_cols - 1; i++ )   // 0th row
        m.setElement( 0, i, value++ );
    
        m.setElement( 1, 3, value++ );      // 1st row
        
    for( IndexType i = 0; i < m_cols - 1; i++ )   // 2nd row
        m.setElement( 2, i, value++ );
        
    for( IndexType i = 1; i < m_cols; i++ )       // 3rd row
        m.setElement( 3, i, value++ );
        
    for( IndexType i = 2; i < m_cols; i++ )       // 4th row
        m.setElement( 4, i, value++ );

    using VectorType = TNL::Containers::Vector< RealType, DeviceType, IndexType >;
    
    VectorType inVector;
    inVector.setSize( 4 );
    for( IndexType i = 0; i < inVector.getSize(); i++ )        
        inVector.setElement( i, 2 );

    VectorType outVector;  
    outVector.setSize( 5 );
    for( IndexType j = 0; j < outVector.getSize(); j++ )
        outVector.setElement( j, 0 );
 
    
    m.vectorProduct( inVector, outVector);
    
   
    EXPECT_EQ( outVector.getElement( 0 ), 12 );
    EXPECT_EQ( outVector.getElement( 1 ),  8 );
    EXPECT_EQ( outVector.getElement( 2 ), 36 );
    EXPECT_EQ( outVector.getElement( 3 ), 54 );
    EXPECT_EQ( outVector.getElement( 4 ), 46 );
}

template< typename Matrix >
void test_PerformSORIteration()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 4x4 sparse matrix:
 *
 *    /  4  1  0  0 \
 *    |  1  4  1  0 |
 *    |  0  1  4  1 |
 *    \  0  0  1  4 /
 */
    
    const IndexType m_rows = 4;
    const IndexType m_cols = 4;
    
    Matrix m;
    m.reset();
    m.setDimensions( m_rows, m_cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( m_rows );
    rowLengths.setValue( 3 );
    m.setCompressedRowLengths( rowLengths );
    
    m.setElement( 0, 0, 4.0 );        // 0th row
    m.setElement( 0, 1, 1.0);
        
    m.setElement( 1, 0, 1.0 );        // 1st row
    m.setElement( 1, 1, 4.0 );
    m.setElement( 1, 2, 1.0 );
        
    m.setElement( 2, 1, 1.0 );        // 2nd row
    m.setElement( 2, 2, 4.0 );
    m.setElement( 2, 3, 1.0 );
        
    m.setElement( 3, 2, 1.0 );        // 3rd row
    m.setElement( 3, 3, 4.0 );
    
    RealType bVector [ 4 ] = { 1, 1, 1, 1 };
    RealType xVector [ 4 ] = { 1, 1, 1, 1 };
    
    IndexType row = 0;
    RealType omega = 1;
    
    
    m.performSORIteration( bVector, row++, xVector, omega);
    
    EXPECT_EQ( xVector[ 0 ], 0.0 );
    EXPECT_EQ( xVector[ 1 ], 1.0 );
    EXPECT_EQ( xVector[ 2 ], 1.0 );
    EXPECT_EQ( xVector[ 3 ], 1.0 );
    
    
    m.performSORIteration( bVector, row++, xVector, omega);
    
    EXPECT_EQ( xVector[ 0 ], 0.0 );
    EXPECT_EQ( xVector[ 1 ], 0.0 );
    EXPECT_EQ( xVector[ 2 ], 1.0 );
    EXPECT_EQ( xVector[ 3 ], 1.0 );
    
    
    m.performSORIteration( bVector, row++, xVector, omega);
    
    EXPECT_EQ( xVector[ 0 ], 0.0 );
    EXPECT_EQ( xVector[ 1 ], 0.0 );
    EXPECT_EQ( xVector[ 2 ], 0.0 );
    EXPECT_EQ( xVector[ 3 ], 1.0 );
    
    
    m.performSORIteration( bVector, row++, xVector, omega);
    
    EXPECT_EQ( xVector[ 0 ], 0.0 );
    EXPECT_EQ( xVector[ 1 ], 0.0 );
    EXPECT_EQ( xVector[ 2 ], 0.0 );
    EXPECT_EQ( xVector[ 3 ], 0.25 );
}

template< typename Matrix >
void test_SaveAndLoad()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 4x4 sparse matrix:
 *
 *    /  1  2  3  0 \
 *    |  0  4  0  5 |
 *    |  6  7  8  0 |
 *    \  0  9 10 11 /
 */
    
    const IndexType m_rows = 4;
    const IndexType m_cols = 4;
    
    Matrix savedMatrix;
    savedMatrix.reset();
    savedMatrix.setDimensions( m_rows, m_cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( m_rows );
    rowLengths.setValue( 3 );
    savedMatrix.setCompressedRowLengths( rowLengths );
    
    RealType value = 1;
    for( IndexType i = 0; i < m_cols - 1; i++ )   // 0th row
        savedMatrix.setElement( 0, i, value++ );
        
        savedMatrix.setElement( 1, 1, value++ );
        savedMatrix.setElement( 1, 3, value++ );      // 1st row
        
    for( IndexType i = 0; i < m_cols - 1; i++ )   // 2nd row
        savedMatrix.setElement( 2, i, value++ );
        
    for( IndexType i = 1; i < m_cols; i++ )       // 3rd row
        savedMatrix.setElement( 3, i, value++ );
        
    savedMatrix.save( "sparseMatrixFile" );
    
    Matrix loadedMatrix;
    loadedMatrix.reset();
    loadedMatrix.setDimensions( m_rows, m_cols );
    typename Matrix::CompressedRowLengthsVector rowLengths2;
    rowLengths2.setSize( m_rows );
    rowLengths2.setValue( 3 );
    loadedMatrix.setCompressedRowLengths( rowLengths2 );
    
    
    loadedMatrix.load( "sparseMatrixFile" );
    
    
    EXPECT_EQ( savedMatrix.getElement( 0, 0 ), loadedMatrix.getElement( 0, 0 ) );
    EXPECT_EQ( savedMatrix.getElement( 0, 1 ), loadedMatrix.getElement( 0, 1 ) );
    EXPECT_EQ( savedMatrix.getElement( 0, 2 ), loadedMatrix.getElement( 0, 2 ) );
    EXPECT_EQ( savedMatrix.getElement( 0, 3 ), loadedMatrix.getElement( 0, 3 ) );
    
    EXPECT_EQ( savedMatrix.getElement( 1, 0 ), loadedMatrix.getElement( 1, 0 ) );
    EXPECT_EQ( savedMatrix.getElement( 1, 1 ), loadedMatrix.getElement( 1, 1 ) );
    EXPECT_EQ( savedMatrix.getElement( 1, 2 ), loadedMatrix.getElement( 1, 2 ) );
    EXPECT_EQ( savedMatrix.getElement( 1, 3 ), loadedMatrix.getElement( 1, 3 ) );
    
    EXPECT_EQ( savedMatrix.getElement( 2, 0 ), loadedMatrix.getElement( 2, 0 ) );
    EXPECT_EQ( savedMatrix.getElement( 2, 1 ), loadedMatrix.getElement( 2, 1 ) );
    EXPECT_EQ( savedMatrix.getElement( 2, 2 ), loadedMatrix.getElement( 2, 2 ) );
    EXPECT_EQ( savedMatrix.getElement( 2, 3 ), loadedMatrix.getElement( 2, 3 ) );
    
    EXPECT_EQ( savedMatrix.getElement( 3, 0 ), loadedMatrix.getElement( 3, 0 ) );
    EXPECT_EQ( savedMatrix.getElement( 3, 1 ), loadedMatrix.getElement( 3, 1 ) );
    EXPECT_EQ( savedMatrix.getElement( 3, 2 ), loadedMatrix.getElement( 3, 2 ) );
    EXPECT_EQ( savedMatrix.getElement( 3, 3 ), loadedMatrix.getElement( 3, 3 ) );
    
    EXPECT_EQ( savedMatrix.getElement( 0, 0 ),  1 );
    EXPECT_EQ( savedMatrix.getElement( 0, 1 ),  2 );
    EXPECT_EQ( savedMatrix.getElement( 0, 2 ),  3 );
    EXPECT_EQ( savedMatrix.getElement( 0, 3 ),  0 );
    
    EXPECT_EQ( savedMatrix.getElement( 1, 0 ),  0 );
    EXPECT_EQ( savedMatrix.getElement( 1, 1 ),  4 );
    EXPECT_EQ( savedMatrix.getElement( 1, 2 ),  0 );
    EXPECT_EQ( savedMatrix.getElement( 1, 3 ),  5 );
    
    EXPECT_EQ( savedMatrix.getElement( 2, 0 ),  6 );
    EXPECT_EQ( savedMatrix.getElement( 2, 1 ),  7 );
    EXPECT_EQ( savedMatrix.getElement( 2, 2 ),  8 );
    EXPECT_EQ( savedMatrix.getElement( 2, 3 ),  0 );
    
    EXPECT_EQ( savedMatrix.getElement( 3, 0 ),  0 );
    EXPECT_EQ( savedMatrix.getElement( 3, 1 ),  9 );
    EXPECT_EQ( savedMatrix.getElement( 3, 2 ), 10 );
    EXPECT_EQ( savedMatrix.getElement( 3, 3 ), 11 );
    
    std::cout << "\nThis will create a file called 'sparseMatrixFile' (of the matrix created in the test function), in .../tnl-dev/Debug/bin/\n\n";
}

template< typename Matrix >
void test_Print()
{
    using RealType = typename Matrix::RealType;
    using DeviceType = typename Matrix::DeviceType;
    using IndexType = typename Matrix::IndexType;
    
/*
 * Sets up the following 5x4 sparse matrix:
 *
 *    /  1  2  3  0 \
 *    |  0  0  0  4 |
 *    |  5  6  7  0 |
 *    |  0  8  9 10 |
 *    \  0  0 11 12 /
 */
    
    const IndexType m_rows = 5;
    const IndexType m_cols = 4;
    
    Matrix m;
    m.reset();
    m.setDimensions( m_rows, m_cols );
    typename Matrix::CompressedRowLengthsVector rowLengths;
    rowLengths.setSize( m_rows );
    rowLengths.setValue( 3 );
    m.setCompressedRowLengths( rowLengths );
    
    RealType value = 1;
    for( IndexType i = 0; i < m_cols - 1; i++ )   // 0th row
        m.setElement( 0, i, value++ );
    
        m.setElement( 1, 3, value++ );      // 1st row
        
    for( IndexType i = 0; i < m_cols - 1; i++ )   // 2nd row
        m.setElement( 2, i, value++ );
        
    for( IndexType i = 1; i < m_cols; i++ )       // 3rd row
        m.setElement( 3, i, value++ );
        
    for( IndexType i = 2; i < m_cols; i++ )       // 4th row
        m.setElement( 4, i, value++ );
    
    // This is from: https://stackoverflow.com/questions/5193173/getting-cout-output-to-a-stdstring
    #include <sstream>
    std::stringstream printed;
    std::stringstream couted;
    
    // This is from: https://stackoverflow.com/questions/19485536/redirect-output-of-an-function-printing-to-console-to-string
    //change the underlying buffer and save the old buffer
    auto old_buf = std::cout.rdbuf(printed.rdbuf()); 

    m.print( std::cout ); //all the std::cout goes to ss

    std::cout.rdbuf(old_buf); //reset
    
    //printed << printed.str() << std::endl;
    couted << "Row: 0 ->  Col:0->1	 Col:1->2	 Col:2->3\t\n"
               "Row: 1 ->  Col:3->4\t\n"
               "Row: 2 ->  Col:0->5	 Col:1->6	 Col:2->7\t\n"
               "Row: 3 ->  Col:1->8	 Col:2->9	 Col:3->10\t\n"
               "Row: 4 ->  Col:2->11	 Col:3->12\t\n";
    
    
    EXPECT_EQ( printed.str(), couted.str() );
}

#endif