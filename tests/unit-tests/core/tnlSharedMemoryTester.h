/***************************************************************************
                          tnlSharedMemoryTester.h  -  description
                             -------------------
    begin                : Feb 22, 2011
    copyright            : (C) 2011 by Tomas Oberhuber
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

#ifndef TNLSHAREDMEMORYTESTER_H_
#define TNLSHAREDMEMORYTESTER_H_

#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/Message.h>
#include <core/tnlSharedMemory.h>
#include <core/tnlFile.h>

template< typename Device > class tnlSharedMemoryTester : public CppUnit :: TestCase
{
   public:
   tnlSharedMemoryTester(){};

   virtual
   ~tnlSharedMemoryTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlSharedMemoryTester" );
      CppUnit :: TestResult result;
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlSharedMemoryTester< Device > >(
                               "testSharedMemory",
                               & tnlSharedMemoryTester< Device > :: testSharedMemory )
                              );



      return suiteOfTests;
   };

   void testSharedMemory()
   {
      tnlSharedMemory< double > sharedMemory( 1234567, 100, true );
      sharedMemory. setReadingCounter( 0 );
      sharedMemory. setWritingCounter( 0 );
      double* data = sharedMemory. getData();
      for( int i = 0; i < 100; i ++ )
         data[ i ] = 3.14567;
      CPPUNIT_ASSERT( sharedMemory. getReadingCounter() == 0 );
      CPPUNIT_ASSERT( sharedMemory. getWritingCounter() == 0 );
      sharedMemory. increaseReadingCounter();
      sharedMemory. increaseWritingCounter();
      CPPUNIT_ASSERT( sharedMemory. getReadingCounter() == 1 );
      CPPUNIT_ASSERT( sharedMemory. getWritingCounter() == 1 );
      for( int i = 0; i < 100; i ++ )
         data[ i ] = 3.14567;
      CPPUNIT_ASSERT( sharedMemory. getReadingCounter() == 1 );
      CPPUNIT_ASSERT( sharedMemory. getWritingCounter() == 1 );
      sharedMemory. setReadingCounter( 2 );
      sharedMemory. setWritingCounter( 2 );
      for( int i = 0; i < 100; i ++ )
         data[ i ] = 3.14567;
      CPPUNIT_ASSERT( sharedMemory. getReadingCounter() == 2 );
      CPPUNIT_ASSERT( sharedMemory. getWritingCounter() == 2 );
   };

};

#endif /* TNLSHAREDMEMORYTESTER_H_ */
