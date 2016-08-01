/***************************************************************************
                          MultiArrayTester.h -  description
                             -------------------
    begin                : Jul 4, 2012
    copyright            : (C) 2012 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef MultiArrayTESTER_H_
#define MultiArrayTESTER_H_

#ifdef HAVE_CPPUNIT

#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/Message.h>
#include <TNL/Arrays/MultiArray.h>
#include <TNL/File.h>

using namespace TNL;

#ifdef HAVE_CUDA
template< typename ElementType, typename IndexType >
__global__ void testSetGetElementKernel( MultiArray< 1, ElementType, Devices::Cuda, IndexType >* u )
{
   if( threadIdx.x < ( *u ).getDimensions().x() )
      ( *u )( threadIdx.x ) = threadIdx.x;
}

template< typename ElementType, typename IndexType >
__global__ void testSetGetElementKernel( MultiArray< 2, ElementType, Devices::Cuda, IndexType >* u )
{
   if( threadIdx.x < ( *u ).getDimensions().x() &&
       threadIdx.x < ( *u ).getDimensions().y() )
      ( *u )( threadIdx.x, threadIdx.x ) = threadIdx.x;
}

template< typename ElementType, typename IndexType >
__global__ void testSetGetElementKernel( MultiArray< 3, ElementType, Devices::Cuda, IndexType >* u )
{
   if( threadIdx.x < ( *u ).getDimensions().x() &&
       threadIdx.x < ( *u ).getDimensions().y() &&
       threadIdx.x < ( *u ).getDimensions().z() )
      ( *u )( threadIdx.x, threadIdx.x, threadIdx.x ) = threadIdx.x;
}

#endif /* HAVE_CUDA */

template< int Dimensions, typename ElementType, typename Device, typename IndexType >
class MultiArrayTester : public CppUnit :: TestCase
{
   public:

   typedef MultiArrayTester< Dimensions, ElementType, Device, IndexType > MultiArrayTesterType;
   typedef CppUnit :: TestCaller< MultiArrayTesterType > TestCaller;

   MultiArrayTester(){};

   virtual
   ~MultiArrayTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "MultiArrayTester" );
      CppUnit :: TestResult result;
      suiteOfTests -> addTest( new TestCaller( "testConstructorDestructor", &MultiArrayTester::testConstructorDestructor ) );
      suiteOfTests -> addTest( new TestCaller( "testSetSize", &MultiArrayTester::testSetSize ) );
      suiteOfTests -> addTest( new TestCaller( "testSetGetElement", &MultiArrayTester::testSetGetElement ) );
      suiteOfTests -> addTest( new TestCaller( "testComparisonOperator", &MultiArrayTester::testComparisonOperator ) );
      suiteOfTests -> addTest( new TestCaller( "testEquivalenceOperator", &MultiArrayTester::testEquivalenceOperator ) );
      suiteOfTests -> addTest( new TestCaller( "testGetSize", &MultiArrayTester::testGetSize ) );
      suiteOfTests -> addTest( new TestCaller( "testReset", &MultiArrayTester::testReset ) );
      suiteOfTests -> addTest( new TestCaller( "testSetSizeAndDestructor", &MultiArrayTester::testSetSizeAndDestructor ) );
      suiteOfTests -> addTest( new TestCaller( "testSaveAndLoad", &MultiArrayTester::testSaveAndLoad ) );
      return suiteOfTests;
   }

   void testConstructorDestructor()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u;
   }

   void testSetSize()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u, v;
      u. setDimensions( 10 );
      v. setDimensions( 10 );
   }

   void setDiagonalElement( Arrays::MultiArray< 1, ElementType, Device, IndexType >& u,
                            const IndexType& i,
                            const ElementType& v )
   {
      u.setElement( i, v );
   }

   void setDiagonalElement( Arrays::MultiArray< 2, ElementType, Device, IndexType >& u,
                            const IndexType& i,
                            const ElementType& v )
   {
      u.setElement( i, i, v );
   }

   void setDiagonalElement( Arrays::MultiArray< 3, ElementType, Device, IndexType >& u,
                            const IndexType& i,
                            const ElementType& v )
   {
      u.setElement( i, i, i, v );
   }
 
   IndexType getDiagonalElement( Arrays::MultiArray< 1, ElementType, Device, IndexType >& u,
                                 const IndexType& i )
   {
      return u.getElement( i );
   }
 
   IndexType getDiagonalElement( Arrays::MultiArray< 2, ElementType, Device, IndexType >& u,
                                 const IndexType& i )
   {
      return u.getElement( i, i );
   }
 
   IndexType getDiagonalElement( Arrays::MultiArray< 3, ElementType, Device, IndexType >& u,
                                 const IndexType& i )
   {
      return u.getElement( i, i, i );
   }


   void testSetGetElement()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u;
      u. setDimensions( 10 );
      if( std::is_same< Device, Devices::Host >::value )
      {
         for( int i = 0; i < 10; i ++ )
            this->setDiagonalElement( u, i, i  );
      }
      if( std::is_same< Device, Devices::Cuda >::value )
      {
#ifdef HAVE_CUDA
         MultiArray< Dimensions, ElementType, Device, IndexType >* kernel_u =
                  Devices::Cuda::passToDevice( u );
         testSetGetElementKernel<<< 1, 16 >>>( kernel_u );
         Devices::Cuda::freeFromDevice( kernel_u );
         CPPUNIT_ASSERT( checkCudaDevice );
#endif
      }
      for( int i = 0; i < 10; i ++ )
         CPPUNIT_ASSERT( getDiagonalElement( u, i ) == i );
   };

   void testComparisonOperator()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u, v, w;
      u.setDimensions( 10 );
      v.setDimensions( 10 );
      w.setDimensions( 10 );
      u.setValue( 0 );
      v.setValue( 0 );
      w.setValue( 0 );
      for( int i = 0; i < 10; i ++ )
      {
         setDiagonalElement( u, i, i );
         setDiagonalElement( v, i, i );
         setDiagonalElement( w, i, 2*1 );
      }
      CPPUNIT_ASSERT( u == v );
      CPPUNIT_ASSERT( ! ( u != v ) );
      CPPUNIT_ASSERT( u != w );
      CPPUNIT_ASSERT( ! ( u == w ) );
   };

   void testEquivalenceOperator()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u;
      MultiArray< Dimensions, ElementType, Device, IndexType > v;
      u. setDimensions( 10 );
      v. setDimensions( 10 );
      for( int i = 0; i < 10; i ++ )
         setDiagonalElement( u, i, i );
      v = u;
      CPPUNIT_ASSERT( u == v );
      CPPUNIT_ASSERT( ! ( u != v ) );
   };

   void testGetSize()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u;
      const int maxSize = 10;
      for( int i = 1; i < maxSize; i ++ )
         u. setDimensions( i );

      CPPUNIT_ASSERT( u. getDimensions().x() == maxSize - 1 );
   };

   void testReset()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > u;
      u.setDimensions( 100 );
      CPPUNIT_ASSERT( u. getDimensions().x() == 100 );
      u.reset();
      CPPUNIT_ASSERT( u. getDimensions().x() == 0 );
      u.setDimensions( 100 );
      CPPUNIT_ASSERT( u. getDimensions().x() == 100 );
      u.reset();
      CPPUNIT_ASSERT( u. getDimensions().x() == 0 );

   };

   void testSetSizeAndDestructor()
   {
      using namespace TNL::Arrays;
      for( int i = 1; i < 100; i ++ )
      {
         MultiArray< Dimensions, ElementType, Device, IndexType > u;
         u. setDimensions( i );
      }
   }

   void testSaveAndLoad()
   {
      using namespace TNL::Arrays;
      MultiArray< Dimensions, ElementType, Device, IndexType > v;
      const int size( 10 );
      CPPUNIT_ASSERT( v. setDimensions( size ) );
      for( int i = 0; i < size; i ++ )
         setDiagonalElement( v, i, 3.14147 );
      File file;
      file. open( "test-file.tnl", tnlWriteMode );
      CPPUNIT_ASSERT( v. save( file ) );
      file. close();
      MultiArray< Dimensions, ElementType, Device, IndexType > u;
      file. open( "test-file.tnl", tnlReadMode );
      CPPUNIT_ASSERT( u. load( file ) );
      file. close();
      CPPUNIT_ASSERT( u == v );
   }
};

#endif /* HAVE_CPPUNIT */

#endif /* MultiArrayTESTER_H_ */
