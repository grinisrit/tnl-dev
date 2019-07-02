/***************************************************************************
                          SparseMatrixTest_Ellpack.h -  description
                             -------------------
    begin                : Nov 2, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <TNL/Matrices/Ellpack.h>

#include "SparseMatrixTest.hpp"
#include <iostream>

#ifdef HAVE_GTEST 
#include <gtest/gtest.h>

// test fixture for typed tests
template< typename Matrix >
class EllpackMatrixTest : public ::testing::Test
{
protected:
   using EllpackMatrixType = Matrix;
};

// types for which MatrixTest is instantiated
using EllpackMatrixTypes = ::testing::Types
<
    TNL::Matrices::Ellpack< int,    TNL::Devices::Host, short >,
    TNL::Matrices::Ellpack< long,   TNL::Devices::Host, short >,
    TNL::Matrices::Ellpack< float,  TNL::Devices::Host, short >,
    TNL::Matrices::Ellpack< double, TNL::Devices::Host, short >,
    TNL::Matrices::Ellpack< int,    TNL::Devices::Host, int >,
    TNL::Matrices::Ellpack< long,   TNL::Devices::Host, int >,
    TNL::Matrices::Ellpack< float,  TNL::Devices::Host, int >,
    TNL::Matrices::Ellpack< double, TNL::Devices::Host, int >,
    TNL::Matrices::Ellpack< int,    TNL::Devices::Host, long >,
    TNL::Matrices::Ellpack< long,   TNL::Devices::Host, long >,
    TNL::Matrices::Ellpack< float,  TNL::Devices::Host, long >,
    TNL::Matrices::Ellpack< double, TNL::Devices::Host, long >
#ifdef HAVE_CUDA
    ,TNL::Matrices::Ellpack< int,    TNL::Devices::Cuda, short >,
    TNL::Matrices::Ellpack< long,   TNL::Devices::Cuda, short >,
    TNL::Matrices::Ellpack< float,  TNL::Devices::Cuda, short >,
    TNL::Matrices::Ellpack< double, TNL::Devices::Cuda, short >,
    TNL::Matrices::Ellpack< int,    TNL::Devices::Cuda, int >,
    TNL::Matrices::Ellpack< long,   TNL::Devices::Cuda, int >,
    TNL::Matrices::Ellpack< float,  TNL::Devices::Cuda, int >,
    TNL::Matrices::Ellpack< double, TNL::Devices::Cuda, int >,
    TNL::Matrices::Ellpack< int,    TNL::Devices::Cuda, long >,
    TNL::Matrices::Ellpack< long,   TNL::Devices::Cuda, long >,
    TNL::Matrices::Ellpack< float,  TNL::Devices::Cuda, long >,
    TNL::Matrices::Ellpack< double, TNL::Devices::Cuda, long >
#endif
>;

TYPED_TEST_CASE( EllpackMatrixTest, EllpackMatrixTypes );

TYPED_TEST( EllpackMatrixTest, setDimensionsTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_SetDimensions< EllpackMatrixType >();
}

//TYPED_TEST( EllpackMatrixTest, setCompressedRowLengthsTest )
//{
////    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
//    
////    test_SetCompressedRowLengths< EllpackMatrixType >();
//    
//    bool testRan = false;
//    EXPECT_TRUE( testRan );
//    std::cout << "\nTEST DID NOT RUN. NOT WORKING.\n\n";
//    std::cout << "      This test is dependent on the input format. \n";
//    std::cout << "      Almost every format allocates elements per row differently.\n\n";
//    std::cout << "\n    TODO: Finish implementation of getNonZeroRowLength (Only non-zero elements, not the number of allocated elements.)\n\n";
//}

TYPED_TEST( EllpackMatrixTest, setLikeTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_SetLike< EllpackMatrixType, EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, resetTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_Reset< EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, setElementTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_SetElement< EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, addElementTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_AddElement< EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, setRowTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_SetRow< EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, vectorProductTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_VectorProduct< EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, saveAndLoadTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_SaveAndLoad< EllpackMatrixType >();
}

TYPED_TEST( EllpackMatrixTest, printTest )
{
    using EllpackMatrixType = typename TestFixture::EllpackMatrixType;
    
    test_Print< EllpackMatrixType >();
}

#endif

#include "../main.h"