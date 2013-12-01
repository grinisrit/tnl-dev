/***************************************************************************
                          tnlDenseMatrixTester.h  -  description
                             -------------------
    begin                : Nov 30, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#ifndef TNLDENSEMATRIXTESTER_H_
#define TNLDENSEMATRIXTESTER_H_

#ifdef HAVE_CPPUNIT
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/Message.h>
#include <matrices/tnlDenseMatrix.h>
#include <core/tnlFile.h>
#include <core/vectors/tnlVector.h>

template< typename RealType, typename Device, typename IndexType >
class tnlDenseMatrixTester : public CppUnit :: TestCase
{
   public:
   typedef tnlDenseMatrix< RealType, Device, IndexType > MatrixType;
   typedef tnlVector< RealType, Device, IndexType > VectorType;
   typedef tnlDenseMatrixTester< RealType, Device, IndexType > TesterType;
   typedef typename CppUnit::TestCaller< TesterType > TestCallerType;

   tnlDenseMatrixTester(){};

   virtual
   ~tnlDenseMatrixTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlDenseMatrixTester" );
      CppUnit :: TestResult result;

      suiteOfTests -> addTest( new TestCallerType( "setDimensionsTest", &TesterType::setDimensionsTest ) );
      suiteOfTests -> addTest( new TestCallerType( "setElementTest", &TesterType::setElementTest ) );
      suiteOfTests -> addTest( new TestCallerType( "addToElementTest", &TesterType::addToElementTest ) );
      suiteOfTests -> addTest( new TestCallerType( "vectorProductTest", &TesterType::vectorProductTest ) );
      suiteOfTests -> addTest( new TestCallerType( "matrixTranspositionTest", &TesterType::matrixTranspositionTest ) );
      suiteOfTests -> addTest( new TestCallerType( "addMatrixTest", &TesterType::addMatrixTest ) );
      suiteOfTests -> addTest( new TestCallerType( "matrixProductTest", &TesterType::matrixProductTest ) );

      return suiteOfTests;
   }

   void setDimensionsTest()
   {
      MatrixType m;
      m.setDimensions( 10, 20 );
      CPPUNIT_ASSERT( m.getRows() == 10 );
      CPPUNIT_ASSERT( m.getColumns() == 20 );
   }

   void setElementTest()
   {
      MatrixType m;
      m.setDimensions( 10, 10 );
      for( int i = 0; i < 10; i++ )
         m.setElement( i, i, i );
      for( int i = 0; i < 10; i++ )
         CPPUNIT_ASSERT( m.getElement( i, i ) == i );

      for( int i = 0; i < 10; i++ )
         m( i, i ) = i;
      for( int i = 0; i < 10; i++ )
         CPPUNIT_ASSERT( m( i, i ) == i );
   }

   void addToElementTest()
   {
      MatrixType m;
      m.setDimensions( 10, 10 );
      m.setValue( 0.0 );
      for( int i = 0; i < 10; i++ )
         m.setElement( i, i, i );
      for( int i = 0; i < 10; i++ )
         for( int j = 0; j < 10; j++ )
            m.addToElement( i, j, 1 );

      for( int i = 0; i < 10; i++ )
         for( int j = 0; j < 10; j++ )
            if( i == j )
               CPPUNIT_ASSERT( m.getElement( i, i ) == i + 1 );
            else
               CPPUNIT_ASSERT( m.getElement( i, j ) == 1 );
   }

   void vectorProductTest()
   {
      const int size = 10;
      VectorType v, w;
      v.setSize( size );
      w.setSize( size );
      MatrixType m;
      m.setDimensions( size, size );
      m.setValue( 0.0 );
      for( int i = 0; i < size; i++ )
      {
         v.setElement( i, i );
         m.setElement( i, i, i );
      }
      m.vectorProduct( v, w );

      for( int i = 0; i < size; i++ )
         CPPUNIT_ASSERT( w[ i ] == i*i );
   }

   void addMatrixTest()
   {
      const int size = 10;
      MatrixType m;
      m.setDimensions( 10, 10 );
      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
            m( i, j ) = i*size + j;

      MatrixType m2;
      m2.setLike( m );
      m2.setValue( 3.0 );
      m2.addMatrix( m );

      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
            CPPUNIT_ASSERT( m2( i, j ) == m( i, j ) + 3.0 );

      m2.addMatrix( m, 0.5, 0.0 );

      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
            CPPUNIT_ASSERT( m2( i, j ) == 0.5*m( i, j ) );
   }

   void matrixProductTest()
   {
      const int size = 10;
      MatrixType m1, m2, m3;
      m1.setDimensions( 10, 10 );
      m2.setLike( m1 );
      m3.setLike( m1 );
      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
         {
            m1( i, j ) = i*size + j;
            m2( i, j ) = ( i == j );
         }
      m3.getMatrixProduct( m1, m2 );

      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
            CPPUNIT_ASSERT( m3( i, j ) == m1( i, j) );

   }

   void matrixTranspositionTest()
   {
      const int size = 10;
      MatrixType m;
      m.setDimensions( 10, 10 );
      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
            m( i, j ) = i*size + j;

      MatrixType mTransposed;
      mTransposed.setLike( m );
      mTransposed.getTransposition( m );

      for( int i = 0; i < size; i++ )
         for( int j = 0; j < size; j++ )
            CPPUNIT_ASSERT( m( i, j ) == mTransposed( j, i ) );
   }
};

#endif

#endif /* TNLDENSEMATRIXTESTER_H_ */
