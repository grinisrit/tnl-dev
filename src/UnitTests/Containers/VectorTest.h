/***************************************************************************
                          VectorTest.h  -  description
                             -------------------
    begin                : Oct 25, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// NOTE: Vector = Array + VectorOperations, so we test Vector and VectorOperations at the same time

#pragma once

#ifdef HAVE_GTEST
#include <limits>

#include <TNL/Experimental/Arithmetics/Quad.h>
#include <TNL/Containers/Vector.h>
#include <TNL/Containers/VectorView.h>

#include "gtest/gtest.h"

using namespace TNL;
using namespace TNL::Containers;
using namespace TNL::Containers::Algorithms;
using namespace TNL::Arithmetics;

// should be small enough to have fast tests, but larger than minGPUReductionDataSize
// and large enough to require multiple CUDA blocks for reduction
constexpr int VECTOR_TEST_SIZE = 5000;


template< typename Vector >
void setLinearSequence( Vector& deviceVector )
{
   typename Vector::HostType a;
   a.setLike( deviceVector );
   for( int i = 0; i < a.getSize(); i++ )
      a[ i ] = i;
   deviceVector = a;
}

template< typename Vector >
void setConstantSequence( Vector& deviceVector,
                          typename Vector::RealType v )
{
   deviceVector.setValue( v );
}

template< typename Vector >
void setNegativeLinearSequence( Vector& deviceVector )
{
   typename Vector::HostType a;
   a.setLike( deviceVector );
   for( int i = 0; i < a.getSize(); i++ )
      a[ i ] = -i;
   deviceVector = a;
}

template< typename Vector >
void setOscilatingSequence( Vector& deviceVector,
                            typename Vector::RealType v )
{
   typename Vector::HostType a;
   a.setLike( deviceVector );
   a[ 0 ] = v;
   for( int i = 1; i < a.getSize(); i++ )
      a[ i ] = a[ i-1 ] * -1;
   deviceVector = a;
}


// TODO: test everything with OpenMP with different number of threads

// test fixture for typed tests
template< typename Vector >
class VectorTest : public ::testing::Test
{
protected:
   using VectorType = Vector;
   using VectorOperations = Algorithms::VectorOperations< typename VectorType::DeviceType >;
   using ViewType = VectorView< typename Vector::RealType, typename Vector::DeviceType, typename Vector::IndexType >;
};

// types for which VectorTest is instantiated
// TODO: Quad must be fixed
using VectorTypes = ::testing::Types<
#ifndef HAVE_CUDA
   Vector< int,            Devices::Host, short >,
   Vector< long,           Devices::Host, short >,
   Vector< float,          Devices::Host, short >,
   Vector< double,         Devices::Host, short >,
   //Vector< Quad< float >,  Devices::Host, short >,
   //Vector< Quad< double >, Devices::Host, short >,
   Vector< int,            Devices::Host, int >,
   Vector< long,           Devices::Host, int >,
   Vector< float,          Devices::Host, int >,
   Vector< double,         Devices::Host, int >,
   //Vector< Quad< float >,  Devices::Host, int >,
   //Vector< Quad< double >, Devices::Host, int >,
   Vector< int,            Devices::Host, long >,
   Vector< long,           Devices::Host, long >,
   Vector< float,          Devices::Host, long >,
   Vector< double,         Devices::Host, long >
   //Vector< Quad< float >,  Devices::Host, long >,
   //Vector< Quad< double >, Devices::Host, long >
#endif
#ifdef HAVE_CUDA
   Vector< int,            Devices::Cuda, short >,
   Vector< long,           Devices::Cuda, short >,
   Vector< float,          Devices::Cuda, short >,
   Vector< double,         Devices::Cuda, short >,
   //Vector< Quad< float >,  Devices::Cuda, short >,
   //Vector< Quad< double >, Devices::Cuda, short >,
   Vector< int,            Devices::Cuda, int >,
   Vector< long,           Devices::Cuda, int >,
   Vector< float,          Devices::Cuda, int >,
   Vector< double,         Devices::Cuda, int >,
   //Vector< Quad< float >,  Devices::Cuda, int >,
   //Vector< Quad< double >, Devices::Cuda, int >,
   Vector< int,            Devices::Cuda, long >,
   Vector< long,           Devices::Cuda, long >,
   Vector< float,          Devices::Cuda, long >,
   Vector< double,         Devices::Cuda, long >
   //Vector< Quad< float >,  Devices::Cuda, long >,
   //Vector< Quad< double >, Devices::Cuda, long >
#endif
#ifdef HAVE_MIC
   ,
   Vector< int,    Devices::MIC, short >,
   Vector< long,   Devices::MIC, short >,
   Vector< float,  Devices::MIC, short >,
   Vector< double, Devices::MIC, short >,
   Vector< int,    Devices::MIC, int >,
   Vector< long,   Devices::MIC, int >,
   Vector< float,  Devices::MIC, int >,
   Vector< double, Devices::MIC, int >,
   Vector< int,    Devices::MIC, long >,
   Vector< long,   Devices::MIC, long >,
   Vector< float,  Devices::MIC, long >,
   Vector< double, Devices::MIC, long >
#endif
>;

TYPED_TEST_SUITE( VectorTest, VectorTypes );

TYPED_TEST( VectorTest, constructors )
{
   using VectorType = typename TestFixture::VectorType;

   VectorType u;
   EXPECT_EQ( u.getSize(), 0 );

   VectorType v( 10 );
   EXPECT_EQ( v.getSize(), 10 );

   if( std::is_same< typename VectorType::DeviceType, Devices::Host >::value ) {
      typename VectorType::ValueType data[ 10 ];
      VectorType w( data, 10 );
      EXPECT_EQ( w.getData(), data );

      VectorType z1( w );
      //EXPECT_EQ( z1.getData(), data );
      EXPECT_EQ( z1.getSize(), 10 );

      VectorType z2( w, 1 );
      EXPECT_EQ( z2.getData(), data + 1 );
      EXPECT_EQ( z2.getSize(), 9 );

      VectorType z3( w, 2, 3 );
      EXPECT_EQ( z3.getData(), data + 2 );
      EXPECT_EQ( z3.getSize(), 3 );
   }

   VectorType w( v );
   EXPECT_EQ( w.getSize(), v.getSize() );
   for( int i = 0; i < 10; i++ )
      EXPECT_EQ( v.getElement( i ), w.getElement( i ) );
   v.reset();
   EXPECT_EQ( w.getSize(), 10 );

   VectorType a1 { 1, 2, 3 };
   EXPECT_EQ( a1.getElement( 0 ), 1 );
   EXPECT_EQ( a1.getElement( 1 ), 2 );
   EXPECT_EQ( a1.getElement( 2 ), 3 );

   std::list< int > l = { 4, 5, 6 };
   VectorType a2( l );
   EXPECT_EQ( a2.getElement( 0 ), 4 );
   EXPECT_EQ( a2.getElement( 1 ), 5 );
   EXPECT_EQ( a2.getElement( 2 ), 6 );

   std::vector< int > q = { 7, 8, 9 };

   VectorType a3( q );
   EXPECT_EQ( a3.getElement( 0 ), 7 );
   EXPECT_EQ( a3.getElement( 1 ), 8 );
   EXPECT_EQ( a3.getElement( 2 ), 9 );
}

TYPED_TEST( VectorTest, max )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );
   setLinearSequence( v );

   EXPECT_EQ( v.max(), size - 1 );
   EXPECT_EQ( v_view.max(), size - 1 );
   EXPECT_EQ( VectorOperations::getVectorMax( v ), size - 1 );
}

TYPED_TEST( VectorTest, min )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );
   setLinearSequence( v );

   EXPECT_EQ( v.min(), 0 );
   EXPECT_EQ( v_view.min(), 0 );
   EXPECT_EQ( VectorOperations::getVectorMin( v ), 0 );
}

TYPED_TEST( VectorTest, absMax )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );
   setNegativeLinearSequence( v );

   EXPECT_EQ( v.absMax(), size - 1 );
   EXPECT_EQ( v_view.absMax(), size - 1 );
   EXPECT_EQ( VectorOperations::getVectorAbsMax( v ), size - 1 );
}

TYPED_TEST( VectorTest, absMin )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );
   setNegativeLinearSequence( v );

   EXPECT_EQ( v.absMin(), 0 );
   EXPECT_EQ( v_view.absMin(), 0 );
   EXPECT_EQ( VectorOperations::getVectorAbsMin( v ), 0 );
}

TYPED_TEST( VectorTest, lpNorm )
{
   using VectorType = typename TestFixture::VectorType;
   using RealType = typename VectorType::RealType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;
   const RealType epsilon = 64 * std::numeric_limits< RealType >::epsilon();

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );
   setConstantSequence( v, 1 );

   const RealType expectedL1norm = size;
   const RealType expectedL2norm = std::sqrt( size );
   const RealType expectedL3norm = std::cbrt( size );
   EXPECT_EQ( v.lpNorm( 1.0 ), expectedL1norm );
   EXPECT_EQ( v.lpNorm( 2.0 ), expectedL2norm );
   EXPECT_NEAR( v.lpNorm( 3.0 ), expectedL3norm, epsilon );
   EXPECT_EQ( v_view.lpNorm( 1.0 ), expectedL1norm );
   EXPECT_EQ( v_view.lpNorm( 2.0 ), expectedL2norm );
   EXPECT_NEAR( v_view.lpNorm( 3.0 ), expectedL3norm, epsilon );
   EXPECT_EQ( VectorOperations::getVectorLpNorm( v, 1.0 ), expectedL1norm );
   EXPECT_EQ( VectorOperations::getVectorLpNorm( v, 2.0 ), expectedL2norm );
   EXPECT_NEAR( VectorOperations::getVectorLpNorm( v, 3.0 ), expectedL3norm, epsilon );
}

TYPED_TEST( VectorTest, sum )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   // this test expect an even size
   const int size = VECTOR_TEST_SIZE % 2 ? VECTOR_TEST_SIZE - 1 : VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );

   setConstantSequence( v, 1 );
   EXPECT_EQ( v.sum(), size );
   EXPECT_EQ( v_view.sum(), size );
   EXPECT_EQ( VectorOperations::getVectorSum( v ), size );

   setLinearSequence( v );
   EXPECT_EQ( v.sum(), 0.5 * size * ( size - 1 ) );
   EXPECT_EQ( v_view.sum(), 0.5 * size * ( size - 1 ) );
   EXPECT_EQ( VectorOperations::getVectorSum( v ), 0.5 * size * ( size - 1 ) );

   setNegativeLinearSequence( v );
   EXPECT_EQ( v.sum(), - 0.5 * size * ( size - 1 ) );
   EXPECT_EQ( v_view.sum(), - 0.5 * size * ( size - 1 ) );
   EXPECT_EQ( VectorOperations::getVectorSum( v ), - 0.5 * size * ( size - 1 ) );

   setOscilatingSequence( v, 1.0 );
   EXPECT_EQ( v.sum(), 0 );
   EXPECT_EQ( v_view.sum(), 0 );
   EXPECT_EQ( VectorOperations::getVectorSum( v ), 0 );
}

TYPED_TEST( VectorTest, differenceMax )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   setLinearSequence( u );
   setConstantSequence( v, size / 2 );

   EXPECT_EQ( u.differenceMax( v ), size - 1 - size / 2 );
   EXPECT_EQ( u_view.differenceMax( v_view ), size - 1 - size / 2 );
   EXPECT_EQ( VectorOperations::getVectorDifferenceMax( u, v ), size - 1 - size / 2 );
}

TYPED_TEST( VectorTest, differenceMin )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   setLinearSequence( u );
   setConstantSequence( v, size / 2 );

   EXPECT_EQ( u.differenceMin( v ), - size / 2 );
   EXPECT_EQ( u_view.differenceMin( v_view ), - size / 2 );
   EXPECT_EQ( VectorOperations::getVectorDifferenceMin( u, v ), - size / 2 );
   EXPECT_EQ( v.differenceMin( u ), size / 2 - size + 1 );
   EXPECT_EQ( v_view.differenceMin( u_view ), size / 2 - size + 1 );
   EXPECT_EQ( VectorOperations::getVectorDifferenceMin( v, u ), size / 2 - size + 1 );
}

TYPED_TEST( VectorTest, differenceAbsMax )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   // this test expects an odd size
   const int size = VECTOR_TEST_SIZE % 2 ? VECTOR_TEST_SIZE : VECTOR_TEST_SIZE - 1;

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   setNegativeLinearSequence( u );
   setConstantSequence( v, - size / 2 );

   EXPECT_EQ( u.differenceAbsMax( v ), size - 1 - size / 2 );
   EXPECT_EQ( u_view.differenceAbsMax( v_view ), size - 1 - size / 2 );
   EXPECT_EQ( VectorOperations::getVectorDifferenceAbsMax( u, v ), size - 1 - size / 2 );
}

TYPED_TEST( VectorTest, differenceAbsMin )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   setNegativeLinearSequence( u );
   setConstantSequence( v, - size / 2 );

   EXPECT_EQ( u.differenceAbsMin( v ), 0 );
   EXPECT_EQ( u_view.differenceAbsMin( v_view ), 0 );
   EXPECT_EQ( VectorOperations::getVectorDifferenceAbsMin( u, v ), 0 );
   EXPECT_EQ( v.differenceAbsMin( u ), 0 );
   EXPECT_EQ( v_view.differenceAbsMin( u_view ), 0 );
   EXPECT_EQ( VectorOperations::getVectorDifferenceAbsMin( v, u ), 0 );
}

TYPED_TEST( VectorTest, differenceLpNorm )
{
   using VectorType = typename TestFixture::VectorType;
   using RealType = typename VectorType::RealType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;
   const RealType epsilon = 64 * std::numeric_limits< RealType >::epsilon();

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   u.setValue( 3.0 );
   v.setValue( 1.0 );

   const RealType expectedL1norm = 2.0 * size;
   const RealType expectedL2norm = std::sqrt( 4.0 * size );
   const RealType expectedL3norm = std::cbrt( 8.0 * size );
   EXPECT_EQ( u.differenceLpNorm( v, 1.0 ), expectedL1norm );
   EXPECT_EQ( u.differenceLpNorm( v, 2.0 ), expectedL2norm );
   EXPECT_NEAR( u.differenceLpNorm( v, 3.0 ), expectedL3norm, epsilon );
   EXPECT_EQ( u_view.differenceLpNorm( v_view, 1.0 ), expectedL1norm );
   EXPECT_EQ( u_view.differenceLpNorm( v_view, 2.0 ), expectedL2norm );
   EXPECT_NEAR( u_view.differenceLpNorm( v_view, 3.0 ), expectedL3norm, epsilon );
   EXPECT_EQ( VectorOperations::getVectorDifferenceLpNorm( u, v, 1.0 ), expectedL1norm );
   EXPECT_EQ( VectorOperations::getVectorDifferenceLpNorm( u, v, 2.0 ), expectedL2norm );
   EXPECT_NEAR( VectorOperations::getVectorDifferenceLpNorm( u, v, 3.0 ), expectedL3norm, epsilon );
}

TYPED_TEST( VectorTest, differenceSum )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   // this test expect an even size
   const int size = VECTOR_TEST_SIZE % 2 ? VECTOR_TEST_SIZE - 1 : VECTOR_TEST_SIZE;

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   v.setValue( 1.0 );

   setConstantSequence( u, 2 );
   EXPECT_EQ( u.differenceSum( v ), size );
   EXPECT_EQ( u_view.differenceSum( v_view ), size );
   EXPECT_EQ( VectorOperations::getVectorDifferenceSum( u, v ), size );

   setLinearSequence( u );
   EXPECT_EQ( u.differenceSum( v ), 0.5 * size * ( size - 1 ) - size );
   EXPECT_EQ( u_view.differenceSum( v_view ), 0.5 * size * ( size - 1 ) - size );
   EXPECT_EQ( VectorOperations::getVectorDifferenceSum( u, v ), 0.5 * size * ( size - 1 ) - size );

   setNegativeLinearSequence( u );
   EXPECT_EQ( u.differenceSum( v ), - 0.5 * size * ( size - 1 ) - size );
   EXPECT_EQ( u_view.differenceSum( v_view ), - 0.5 * size * ( size - 1 ) - size );
   EXPECT_EQ( VectorOperations::getVectorDifferenceSum( u, v ), - 0.5 * size * ( size - 1 ) - size );

   setOscilatingSequence( u, 1.0 );
   EXPECT_EQ( u.differenceSum( v ), - size );
   EXPECT_EQ( u_view.differenceSum( v_view ), - size );
   EXPECT_EQ( VectorOperations::getVectorDifferenceSum( u, v ), - size );
}

TYPED_TEST( VectorTest, scalarMultiplication )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType u;
   u.setSize( size );
   ViewType u_view( u );

   typename VectorType::HostType expected;
   expected.setSize( size );
   for( int i = 0; i < size; i++ )
      expected[ i ] = 2.0 * i;

   setLinearSequence( u );
   VectorOperations::vectorScalarMultiplication( u, 2.0 );
   EXPECT_EQ( u, expected );

   setLinearSequence( u );
   u.scalarMultiplication( 2.0 );
   EXPECT_EQ( u, expected );

   setLinearSequence( u );
   u_view.scalarMultiplication( 2.0 );
   EXPECT_EQ( u, expected );

   setLinearSequence( u );
   u *= 2.0;
   EXPECT_EQ( u, expected );

   setLinearSequence( u );
   u_view *= 2.0;
   EXPECT_EQ( u, expected );
}

TYPED_TEST( VectorTest, scalarProduct )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   // this test expects an odd size
   const int size = VECTOR_TEST_SIZE % 2 ? VECTOR_TEST_SIZE : VECTOR_TEST_SIZE - 1;

   VectorType u, v;
   u.setSize( size );
   v.setSize( size );
   ViewType u_view( u ), v_view( v );
   setOscilatingSequence( u, 1.0 );
   setConstantSequence( v, 1 );

   EXPECT_EQ( u.scalarProduct( v ), 1.0 );
   EXPECT_EQ( u_view.scalarProduct( v_view ), 1.0 );
   EXPECT_EQ( VectorOperations::getScalarProduct( u, v ), 1.0 );
}

TYPED_TEST( VectorTest, addVector )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType x, y;
   x.setSize( size );
   y.setSize( size );
   ViewType x_view( x ), y_view( y );

   typename VectorType::HostType expected1, expected2;
   expected1.setSize( size );
   expected2.setSize( size );
   for( int i = 0; i < size; i++ ) {
      expected1[ i ] = 2.0 + 3.0 * i;
      expected2[ i ] = 1.0 + 3.0 * i;
   }

   setConstantSequence( x, 1 );
   setLinearSequence( y );
   VectorOperations::addVector( x, y, 3.0, 2.0 );
   EXPECT_EQ( x, expected1 );

   setConstantSequence( x, 1 );
   setLinearSequence( y );
   x.addVector( y, 3.0, 1.0 );
   EXPECT_EQ( x, expected2 );

   setConstantSequence( x, 1 );
   setLinearSequence( y );
   x_view.addVector( y_view, 3.0, 1.0 );
   EXPECT_EQ( x, expected2 );

   // multiplication by floating-point scalars which produces integer values
   setConstantSequence( x, 2 );
   setConstantSequence( y, 4 );
   x.addVector( y, 2.5, -1.5 );
   EXPECT_EQ( x.min(), 7 );
   EXPECT_EQ( x.max(), 7 );
}

TYPED_TEST( VectorTest, addVectors )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType x, y, z;
   x.setSize( size );
   y.setSize( size );
   z.setSize( size );
   ViewType x_view( x ), y_view( y ), z_view( z );

   typename VectorType::HostType expected1, expected2;
   expected1.setSize( size );
   expected2.setSize( size );
   for( int i = 0; i < size; i++ ) {
      expected1[ i ] = 1.0 + 3.0 * i + 2.0;
      expected2[ i ] = 2.0 + 3.0 * i + 2.0;
   }

   setConstantSequence( x, 1 );
   setLinearSequence( y );
   setConstantSequence( z, 2 );
   VectorOperations::addVectors( x, y, 3.0, z, 1.0, 1.0 );
   EXPECT_EQ( x, expected1 );

   setConstantSequence( x, 1 );
   setLinearSequence( y );
   setConstantSequence( z, 2 );
   x.addVectors( y, 3.0, z, 1.0, 2.0 );
   EXPECT_EQ( x, expected2 );

   setConstantSequence( x, 1 );
   setLinearSequence( y );
   setConstantSequence( z, 2 );
   x_view.addVectors( y_view, 3.0, z_view, 1.0, 2.0 );
   EXPECT_EQ( x, expected2 );

   // multiplication by floating-point scalars which produces integer values
   setConstantSequence( x, 2 );
   setConstantSequence( y, 4 );
   setConstantSequence( z, 6 );
   x.addVectors( y, 2.5, z, -1.5, -1.5 );
   EXPECT_EQ( x.min(), -2 );
   EXPECT_EQ( x.max(), -2 );
}

TYPED_TEST( VectorTest, prefixSum )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );

   setConstantSequence( v, 1 );
   v.computePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), i + 1 );

   v.setValue( 0 );
   v.computePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), 0 );

   setLinearSequence( v );
   v.computePrefixSum();
   for( int i = 1; i < size; i++ )
      EXPECT_EQ( v.getElement( i ) - v.getElement( i - 1 ), i );

   setConstantSequence( v, 1 );
   v_view.computePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), i + 1 );

   v.setValue( 0 );
   v_view.computePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), 0 );

   setLinearSequence( v );
   v_view.computePrefixSum();
   for( int i = 1; i < size; i++ )
      EXPECT_EQ( v.getElement( i ) - v.getElement( i - 1 ), i );
}

TYPED_TEST( VectorTest, exclusivePrefixSum )
{
   using VectorType = typename TestFixture::VectorType;
   using VectorOperations = typename TestFixture::VectorOperations;
   using ViewType = typename TestFixture::ViewType;
   const int size = VECTOR_TEST_SIZE;

   VectorType v;
   v.setSize( size );
   ViewType v_view( v );

   setConstantSequence( v, 1 );
   v.computeExclusivePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), i );

   v.setValue( 0 );
   v.computeExclusivePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), 0 );

   setLinearSequence( v );
   v.computeExclusivePrefixSum();
   for( int i = 1; i < size; i++ )
      EXPECT_EQ( v.getElement( i ) - v.getElement( i - 1 ), i - 1 );

   setConstantSequence( v, 1 );
   v_view.computeExclusivePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), i );

   v.setValue( 0 );
   v_view.computeExclusivePrefixSum();
   for( int i = 0; i < size; i++ )
      EXPECT_EQ( v.getElement( i ), 0 );

   setLinearSequence( v );
   v_view.computeExclusivePrefixSum();
   for( int i = 1; i < size; i++ )
      EXPECT_EQ( v.getElement( i ) - v.getElement( i - 1 ), i - 1 );
}

// TODO: test prefix sum with custom begin and end parameters

TEST( VectorSpecialCasesTest, sumOfBoolVector )
{
   using VectorType = Containers::Vector< bool, Devices::Host >;
   using ViewType = VectorView< bool, Devices::Host >;
   const float epsilon = 64 * std::numeric_limits< float >::epsilon();

   VectorType v( 512 ), w( 512 );
   ViewType v_view( v ), w_view( w );
   v.setValue( true );
   w.setValue( false );

   const int sum = v.sum< int >();
   const int l1norm = v.lpNorm< int >( 1.0 );
   const float l2norm = v.lpNorm< float >( 2.0 );
   const float l3norm = v.lpNorm< float >( 3.0 );
   EXPECT_EQ( sum, 512 );
   EXPECT_EQ( l1norm, 512 );
   EXPECT_NEAR( l2norm, std::sqrt( 512 ), epsilon );
   EXPECT_NEAR( l3norm, std::cbrt( 512 ), epsilon );

   const int diff_sum = v.differenceSum< int >( w );
   const int diff_l1norm = v.differenceLpNorm< int >( w, 1.0 );
   const float diff_l2norm = v.differenceLpNorm< float >( w, 2.0 );
   const float diff_l3norm = v.differenceLpNorm< float >( w, 3.0 );
   EXPECT_EQ( diff_sum, 512 );
   EXPECT_EQ( diff_l1norm, 512 );
   EXPECT_NEAR( diff_l2norm, std::sqrt( 512 ), epsilon );
   EXPECT_NEAR( diff_l3norm, std::cbrt( 512 ), epsilon );

   // test views
   const int sum_view = v_view.sum< int >();
   const int l1norm_view = v_view.lpNorm< int >( 1.0 );
   const float l2norm_view = v_view.lpNorm< float >( 2.0 );
   const float l3norm_view = v_view.lpNorm< float >( 3.0 );
   EXPECT_EQ( sum_view, 512 );
   EXPECT_EQ( l1norm_view, 512 );
   EXPECT_NEAR( l2norm_view, std::sqrt( 512 ), epsilon );
   EXPECT_NEAR( l3norm_view, std::cbrt( 512 ), epsilon );

   const int diff_sum_view = v_view.differenceSum< int >( w_view );
   const int diff_l1norm_view = v_view.differenceLpNorm< int >( w_view, 1.0 );
   const float diff_l2norm_view = v_view.differenceLpNorm< float >( w_view, 2.0 );
   const float diff_l3norm_view = v_view.differenceLpNorm< float >( w_view, 3.0 );
   EXPECT_EQ( diff_sum_view, 512 );
   EXPECT_EQ( diff_l1norm_view, 512 );
   EXPECT_NEAR( diff_l2norm_view, std::sqrt( 512 ), epsilon );
   EXPECT_NEAR( diff_l3norm_view, std::cbrt( 512 ), epsilon );
}

TEST( VectorSpecialCasesTest, assignmentThroughView )
{
   using VectorType = Containers::Vector< int, Devices::Host >;
   using ViewType = VectorView< int, Devices::Host >;

   VectorType u( 100 ), v( 100 );
   ViewType u_view( u ), v_view( v );

   u.setValue( 42 );
   v.setValue( 0 );
   v_view = u_view;
   EXPECT_EQ( u_view.getData(), u.getData() );
   EXPECT_EQ( v_view.getData(), v.getData() );
   for( int i = 0; i < 100; i++ )
      EXPECT_EQ( v_view[ i ], 42 );

   u.setValue( 42 );
   v.setValue( 0 );
   v_view = u;
   EXPECT_EQ( u_view.getData(), u.getData() );
   EXPECT_EQ( v_view.getData(), v.getData() );
   for( int i = 0; i < 100; i++ )
      EXPECT_EQ( v_view[ i ], 42 );
}

TEST( VectorSpecialCasesTest, operationsOnConstView )
{
   using VectorType = Containers::Vector< int, Devices::Host >;
   using ViewType = VectorView< const int, Devices::Host >;

   VectorType u( 100 ), v( 100 );
   ViewType u_view( u ), v_view( v );

   u.setValue( 1 );
   v.setValue( 1 );

   EXPECT_EQ( u_view.max(), 1 );
   EXPECT_EQ( u_view.min(), 1 );
   EXPECT_EQ( u_view.absMax(), 1 );
   EXPECT_EQ( u_view.absMin(), 1 );
   EXPECT_EQ( u_view.lpNorm( 1 ), 100 );
   EXPECT_EQ( u_view.differenceMax( v_view ), 0 );
   EXPECT_EQ( u_view.differenceMin( v_view ), 0 );
   EXPECT_EQ( u_view.differenceAbsMax( v_view ), 0 );
   EXPECT_EQ( u_view.differenceAbsMin( v_view ), 0 );
   EXPECT_EQ( u_view.differenceLpNorm( v_view, 1 ), 0 );
   EXPECT_EQ( u_view.differenceSum( v_view ), 0 );
   EXPECT_EQ( u_view.scalarProduct( v_view ), 100 );
}

TEST( VectorSpecialCasesTest, initializationOfVectorViewByArrayView )
{
   using ArrayType = Containers::Array< int, Devices::Host >;
   using VectorViewType = VectorView< const int, Devices::Host >;
   using ArrayViewType = ArrayView< int, Devices::Host >;

   ArrayType a( 100 );
   a.setValue( 0 );
   ArrayViewType a_view( a );

   VectorViewType v_view( a_view );
   EXPECT_EQ( v_view.getData(), a_view.getData() );
   EXPECT_EQ( v_view.sum(), 0 );
}

TEST( VectorSpecialCasesTest, defaultConstructors )
{
   using ArrayType = Containers::Array< int, Devices::Host >;
   using VectorViewType = VectorView< int, Devices::Host >;
   using ArrayViewType = ArrayView< int, Devices::Host >;

   ArrayType a( 100 );
   a.setValue( 0 );

   ArrayViewType a_view;
   a_view.bind( a );

   VectorViewType v_view;
   v_view.bind( a );
   EXPECT_EQ( v_view.getData(), a_view.getData() );
}

#endif // HAVE_GTEST


#include "../main.h"
