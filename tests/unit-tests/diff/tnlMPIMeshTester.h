/***************************************************************************
                          tnlMPIMeshTester.h  -  description
                             -------------------
    begin                : Dec 15, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
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

#ifndef TNLMPIMESHTESTER_H_
#define TNLMPIMESHTESTER_H_

#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <diff/tnlMPIMesh.h>


#ifdef HAVE_CUDA

#endif

template< typename Real >
class tnlMPIMeshTester : public CppUnit :: TestCase
{
   public:
   tnlMPIMeshTester(){};

   virtual
   ~tnlMPIMeshTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlMPIMeshTester" );
      CppUnit :: TestResult result;
      suiteOfTests -> addTest( new CppUnit :: TestCaller< tnlMPIMeshTester< Real > >(
                               "testContructor",
                               & tnlMPIMeshTester< Real > :: testConstructor )
                             );
      return suiteOfTests;
   }

   void testConstructor()
   {
      tnlMPIMesh< 2, Real > mesh2d;
      tnlMPIMesh< 3, Real > mesh3d;
   }

};



#endif /* TNLMPIMESHTESTER_H_ */