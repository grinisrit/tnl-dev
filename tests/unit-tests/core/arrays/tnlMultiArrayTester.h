/***************************************************************************
                          tnlMultiArrayTester.h -  description
                             -------------------
    begin                : Jul 4, 2012
    copyright            : (C) 2012 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TNLMULTIARRAYTESTER_H_
#define TNLMULTIARRAYTESTER_H_

#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/Message.h>
#include <core/arrays/tnlMultiArray.h>
#include <core/tnlFile.h>


template< int Dimension, typename ElementType, typename Device, typename IndexType >
class tnlMultiArrayTester : public CppUnit :: TestCase
{
   public:

   typedef tnlMultiArrayTester< Dimensions, ElementType, Device, IndexType > MultiArrayTester;
   typedef CppUnit :: TestCaller< MultiArrayTester > TestCaller;

   tnlMultiArrayTester(){};

   virtual
   ~tnlMultiArrayTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlMultiArrayTester" );
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
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u;
   }

   void testSetSize()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u, v;
      u. setSize( 10 );
      v. setSize( 10 );
   }

   void testSetGetElement()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u( "tnlMultiArrayTester :: u" );
      u. setSize( 10 );
      for( int i = 0; i < 10; i ++ )
         u. setElement( i, i );
      for( int i = 0; i < 10; i ++ )
         CPPUNIT_ASSERT( u. getElement( i ) == i );
   };

   void testComparisonOperator()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u( "tnlMultiArrayTester :: u" );
      tnlMultiArray< Dimension, ElementType, Device, IndexType > v( "tnlMultiArrayTester :: v" );
      tnlMultiArray< Dimension, ElementType, Device, IndexType > w( "tnlMultiArrayTester :: w" );
      u. setSize( 10 );
      v. setSize( 10 );
      w. setSize( 10 );
      for( int i = 0; i < 10; i ++ )
      {
         u. setElement( i, i );
         v. setElement( i, i );
         w. setElement( i, 2*1 );
      }
      CPPUNIT_ASSERT( u == v );
      CPPUNIT_ASSERT( ! ( u != v ) );
      CPPUNIT_ASSERT( u != w );
      CPPUNIT_ASSERT( ! ( u == w ) );
   };

   void testEquivalenceOperator()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u;
      tnlMultiArray< Dimension, ElementType, Device, IndexType > v;
      u. setName( "tnlMultiArrayTester :: testEquivalenceOperator :: u" );
      v. setName( "tnlMultiArrayTester :: testEquivalenceOperator :: v" );
      u. setSize( 10 );
      v. setSize( 10 );
      for( int i = 0; i < 10; i ++ )
         u. setElement( i, i );
      v = u;
      //CPPUNIT_ASSERT( u == v );
      //CPPUNIT_ASSERT( ! ( u != v ) );
   };

   void testGetSize()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u( "tnlMultiArrayTester :: testSetSize - u" );
      const int maxSize = 10;
      for( int i = 0; i < maxSize; i ++ )
         u. setSize( i );

      CPPUNIT_ASSERT( u. getSize() == maxSize - 1 );
   };

   void testReset()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u( "tnlMultiArrayTester :: testReset - u" );
      u. setSize( 100 );
      CPPUNIT_ASSERT( u. getSize() == 100 );
      u. reset();
      CPPUNIT_ASSERT( u. getSize() == 0 );
      u. setSize( 100 );
      CPPUNIT_ASSERT( u. getSize() == 100 );
      u. reset();
      CPPUNIT_ASSERT( u. getSize() == 0 );

   };

   void testSetSizeAndDestructor()
   {
      for( int i = 0; i < 100; i ++ )
      {
         tnlMultiArray< Dimension, ElementType, Device, IndexType > u( "tnlMultiArrayTester :: testSetSizeAndDestructor - u" );
         u. setSize( i );
      }
   }

   void testSaveAndLoad()
   {
      tnlMultiArray< Dimension, ElementType, Device, IndexType > v( "test-array-v" );
      v. setSize( 100 );
      for( int i = 0; i < 100; i ++ )
         v. setElement( i, 3.14147 );
      tnlFile file;
      file. open( "test-file.tnl", tnlWriteMode );
      v. save( file );
      file. close();
      tnlMultiArray< Dimension, ElementType, Device, IndexType > u( "test-array-u" );
      file. open( "test-file.tnl", tnlReadMode );
      u. load( file );
      file. close();
      CPPUNIT_ASSERT( u == v );
   }
};


#endif /* TNLMULTIARRAYTESTER_H_ */
