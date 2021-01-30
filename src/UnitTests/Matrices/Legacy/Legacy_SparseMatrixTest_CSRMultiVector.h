/***************************************************************************
                          SparseMatrixTest_CSRMultiVector.h -  description
                             -------------------
    begin                : Jan 30, 2021
    copyright            : (C) 2021 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <Benchmarks/SpMV/ReferenceFormats/Legacy/CSR.h>

#include "Legacy_SparseMatrixTest.hpp"
#include <iostream>

#ifdef HAVE_GTEST
#include <gtest/gtest.h>

// test fixture for typed tests
template< typename Matrix >
class CSRMatrixTest : public ::testing::Test
{
protected:
   using CSRMatrixType = Matrix;
};

using namespace TNL::Benchmarks::SpMV::ReferenceFormats;

// types for which MatrixTest is instantiated
using CSRMatrixTypes = ::testing::Types
<
#ifdef HAVE_CUDA
   Legacy::CSR< int,    TNL::Devices::Cuda, int,  Legacy::CSRMultiVector >,
   Legacy::CSR< float,  TNL::Devices::Cuda, int,  Legacy::CSRMultiVector >,
   Legacy::CSR< double, TNL::Devices::Cuda, int,  Legacy::CSRMultiVector >,
   Legacy::CSR< int,    TNL::Devices::Cuda, long, Legacy::CSRMultiVector >,
   Legacy::CSR< float,  TNL::Devices::Cuda, long, Legacy::CSRMultiVector >,
   Legacy::CSR< double, TNL::Devices::Cuda, long, Legacy::CSRMultiVector >
#endif
>;

TYPED_TEST_SUITE( CSRMatrixTest, CSRMatrixTypes);

TYPED_TEST( CSRMatrixTest, setDimensionsTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_SetDimensions< CSRMatrixType >();
}

TYPED_TEST( CSRMatrixTest, setLikeTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_SetLike< CSRMatrixType, CSRMatrixType >();
}

TYPED_TEST( CSRMatrixTest, resetTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_Reset< CSRMatrixType >();
}

TYPED_TEST( CSRMatrixTest, setElementTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_SetElement< CSRMatrixType >();
}

TYPED_TEST( CSRMatrixTest, addElementTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_AddElement< CSRMatrixType >();
}

TYPED_TEST( CSRMatrixTest, setRowTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_SetRow< CSRMatrixType >();
}

/* TYPED_TEST( CSRMatrixTest, vectorProductTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_VectorProduct< CSRMatrixType >();
} */

/*TYPED_TEST( CSRMatrixTest, vectorProductLargerTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_VectorProductLarger< CSRMatrixType >();
}*/

TYPED_TEST( CSRMatrixTest, vectorProductCSRApadtiveTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_VectorProductCSRAdaptive< CSRMatrixType >();
}

TYPED_TEST( CSRMatrixTest, saveAndLoadTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_SaveAndLoad< CSRMatrixType >( "test_Legacy_SparseMatrixTest_CSR_MultiVector" );
}

TYPED_TEST( CSRMatrixTest, printTest )
{
    using CSRMatrixType = typename TestFixture::CSRMatrixType;

    test_Print< CSRMatrixType >();
}

#endif

#include "../../main.h"
