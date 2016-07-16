/***************************************************************************
                          tnlRgCSRMatrixTester.h  -  description
                             -------------------
    begin                : Jul 20, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLRgCSRMATRIXTESTER_H_
#define TNLRgCSRMATRIXTESTER_H_

#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <matrices/tnlCSRMatrix.h>
#include "tnlMatrixTester.h"
#include <legacy/matrices/tnlRgCSRMatrix.h>

template< class T > class tnlRgCSRMatrixTester : public CppUnit :: TestCase,
                                                 public tnlMatrixTester< T >
{
   public:
   tnlRgCSRMatrixTester(){};

   virtual
   ~tnlRgCSRMatrixTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlRgCSRMatrixTester" );
      CppUnit :: TestResult result;
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifEmptyMatrixIsStoredProperly",
                               & tnlRgCSRMatrixTester< T > :: ifEmptyMatrixIsStoredProperly )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifDiagonalMatrixIsStoredProperly",
                               & tnlRgCSRMatrixTester< T > :: ifDiagonalMatrixIsStoredProperly )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifTriDiagonalMatrixIsStoredProperly",
                               & tnlRgCSRMatrixTester< T > :: ifTriDiagonalMatrixIsStoredProperly )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifSpmvWithEmptyMatrixWorks",
                               & tnlRgCSRMatrixTester< T > :: ifSpmvWithEmptyMatrixWorks )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifUpperTriangularMatrixIsStoredProperly",
                               & tnlRgCSRMatrixTester< T > :: ifUpperTriangularMatrixIsStoredProperly )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifFullMatrixIsStoredProperly",
                               & tnlRgCSRMatrixTester< T > :: ifFullMatrixIsStoredProperly )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifBcsstk20MatrixIsStoredProperly",
                               & tnlRgCSRMatrixTester< T > :: ifBcsstk20MatrixIsStoredProperly )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifSpmvWithDiagonalMatrixWorks",
                               & tnlRgCSRMatrixTester< T > :: ifSpmvWithDiagonalMatrixWorks )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifSpmvWithTriDiagonalMatrixWorks",
                               & tnlRgCSRMatrixTester< T > :: ifSpmvWithTriDiagonalMatrixWorks )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifSpmvWithUpperTriangularMatrixWorks",
                               & tnlRgCSRMatrixTester< T > :: ifSpmvWithUpperTriangularMatrixWorks  )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifSpmvWithFullMatrixWorks",
                               & tnlRgCSRMatrixTester< T > :: ifSpmvWithFullMatrixWorks  )
                             );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlRgCSRMatrixTester< T > >(
                               "ifSpmvWithBcsstk20MatrixWorks",
                               & tnlRgCSRMatrixTester< T > :: ifSpmvWithBcsstk20MatrixWorks )
                             );
      return suiteOfTests;
   }

   void ifEmptyMatrixIsStoredProperly()
   {
      const int size = 12;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setEmptyMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      bool error( false );
      for( int i = 0; i < size; i ++ )
         for( int j = 0; j < size; j ++ )
            if( argcsrMatrix. getElement( i, j ) != 0 )
               error = true;
      CPPUNIT_ASSERT( ! error );
   };

   void ifDiagonalMatrixIsStoredProperly()
   {
      const int size = 12;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setDiagonalMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      bool error( false );
      for( int i = 0; i < size; i ++ )
         for( int j = 0; j < size; j ++ )
            if( argcsrMatrix. getElement( i, j ) != csrMatrix. getElement( i, j ) )
               error = true;
      CPPUNIT_ASSERT( ! error );
   };

   void ifTriDiagonalMatrixIsStoredProperly()
   {
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      int size = 12;
      this->setTridiagonalMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      bool error( false );
      for( int i = 0; i < size; i ++ )
         for( int j = 0; j < size; j ++ )
            if( csrMatrix. getElement( i, j ) != argcsrMatrix. getElement( i, j ) )
               error = true;
      CPPUNIT_ASSERT( ! error );

   }

   void ifUpperTriangularMatrixIsStoredProperly()
   {
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      const int size = 12;
      this->setUpperTriangularMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      bool error( false );
      for( int i = 0; i < size; i ++ )
         for( int j = 0; j < size; j ++ )
            if( csrMatrix. getElement( i, j ) != argcsrMatrix. getElement( i, j ) )
               error = true;

      CPPUNIT_ASSERT( ! error );
   }

   void ifFullMatrixIsStoredProperly()
   {
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      const int size = 12;
      this->setUpperTriangularMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      bool error( false );
      for( int i = 0; i < size; i ++ )
         for( int j = 0; j < size; j ++ )
            if( csrMatrix. getElement( i, j ) != argcsrMatrix. getElement( i, j ) )
               error = true;

      CPPUNIT_ASSERT( ! error );
   }

   void ifBcsstk20MatrixIsStoredProperly()
   {
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      const int size = 12;
      this->setBcsstk20Matrix( csrMatrix );
      argcsrMatrix. copyFrom( csrMatrix );

      bool error( false );
      for( int i = 0; i < size; i ++ )
         for( int j = 0; j < size; j ++ )
            if( csrMatrix. getElement( i, j ) != argcsrMatrix. getElement( i, j ) )
               error = true;

      CPPUNIT_ASSERT( ! error );
   }

   void ifSpmvWithEmptyMatrixWorks()
   {
      const int size = 35;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setEmptyMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      tnlVector< T > x, b1, b2;
      x. setSize( size );
      b1. setSize( size );
      b2. setSize( size );
      x. setValue( 1.0 );
      csrMatrix. vectorProduct( x, b1 );
      argcsrMatrix. vectorProduct( x, b2 );

      CPPUNIT_ASSERT( b1 == b2 );
   }

   void ifSpmvWithDiagonalMatrixWorks()
   {
      const int size = 35;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setDiagonalMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      tnlVector< T > x, b1, b2;
      x. setSize( size );
      b1. setSize( size );
      b2. setSize( size );
      x. setValue( 1.0 );
      csrMatrix. vectorProduct( x, b1 );
      argcsrMatrix. vectorProduct( x, b2 );

      CPPUNIT_ASSERT( b1 == b2 );
   }

   void ifSpmvWithTriDiagonalMatrixWorks()
   {
      const int size = 12;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setTridiagonalMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      tnlVector< T > x, b1, b2;
      x. setSize( size );
      b1. setSize( size );
      b2. setSize( size );
      x. setValue( 1.0 );
      csrMatrix. vectorProduct( x, b1 );
      argcsrMatrix. vectorProduct( x, b2 );

      CPPUNIT_ASSERT( b1 == b2 );
   }

   void ifSpmvWithUpperTriangularMatrixWorks()
   {
      const int size = 12;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setUpperTriangularMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      tnlVector< T > x, b1, b2;
      x. setSize( size );
      b1. setSize( size );
      b2. setSize( size );
      x. setValue( 1.0 );
      csrMatrix. vectorProduct( x, b1 );
      argcsrMatrix. vectorProduct( x, b2 );

      CPPUNIT_ASSERT( b1 == b2 );
   }

   void ifSpmvWithFullMatrixWorks()
   {
      const int size = 12;
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setFullMatrix( csrMatrix, size );
      argcsrMatrix. copyFrom( csrMatrix );

      tnlVector< T > x, b1, b2;
      x. setSize( size );
      b1. setSize( size );
      b2. setSize( size );
      x. setValue( 1.0 );
      csrMatrix. vectorProduct( x, b1 );
      argcsrMatrix. vectorProduct( x, b2 );

      CPPUNIT_ASSERT( b1 == b2 );
   }

   void ifSpmvWithBcsstk20MatrixWorks()
   {
      tnlCSRMatrix< T > csrMatrix;
      tnlRgCSRMatrix< T > argcsrMatrix;
      this->setBcsstk20Matrix( csrMatrix );
      argcsrMatrix. copyFrom( csrMatrix );
      const int size = csrMatrix. getRows();

      tnlVector< T > x( "x" ), b1( "b1" ), b2( "b2" );
      x. setSize( size );
      b1. setSize( size );
      b2. setSize( size );
      x. setValue( 1.0 );
      csrMatrix. vectorProduct( x, b1 );
      argcsrMatrix. vectorProduct( x, b2 );



      CPPUNIT_ASSERT( b1 == b2 );
   }

};

#endif /* TNLRgCSRMATRIXTESTER_H_ */
