/***************************************************************************
                          tnl-unit-tests.h  -  description
                             -------------------
    begin                : Nov 21, 2009
    copyright            : (C) 2009 by Tomas Oberhuber
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

#include <cppunit/ui/text/TestRunner.h>

#include <debug/tnlDebug.h>
#include "core/tnlFileTester.h"
#include "core/tnlStringTester.h"
#include "core/tnlObjectTester.h"
#include "core/tnlRealTester.h"
#include "core/tnlTupleTester.h"
#include "core/tnlVectorHostTester.h"
#include "core/tnlArrayTester.h"
#include "core/tnlArrayManagerTester.h"
#include "core/tnlGridTester.h"
#include "core/tnlSharedMemoryTester.h"
#include "core/tnlCommunicatorTester.h"
#include "matrix/tnlCSRMatrixTester.h"
#include "matrix/tnlRgCSRMatrixTester.h"
#include "matrix/tnlAdaptiveRgCSRMatrixTester.h"
#include "matrix/tnlEllpackMatrixTester.h"
#include "diff/tnlMPIMeshTester.h"

#include <iostream>

using namespace std;

int main( int argc, char* argv[] )
{
   dbgFunctionName( "", "main" );
   dbgInit( "" );

   CppUnit :: TextTestRunner runner;

   runner. addTest( tnlFileTester :: suite() );

   runner. addTest( tnlStringTester :: suite() );

   //runner. addTest( tnlObjectTester :: suite() );

   runner. addTest( tnlRealTester< float > :: suite() );
   runner. addTest( tnlRealTester< double > :: suite() );

   runner. addTest( tnlTupleTester< 1, int > :: suite() );
   runner. addTest( tnlTupleTester< 2, int > :: suite() );
   runner. addTest( tnlTupleTester< 3, int > :: suite() );
   runner. addTest( tnlTupleTester< 4, int > :: suite() );
   runner. addTest( tnlTupleTester< 1, float > :: suite() );
   runner. addTest( tnlTupleTester< 2, float > :: suite() );
   runner. addTest( tnlTupleTester< 3, float > :: suite() );
   runner. addTest( tnlTupleTester< 4, float > :: suite() );
   runner. addTest( tnlTupleTester< 1, double > :: suite() );
   runner. addTest( tnlTupleTester< 2, double > :: suite() );
   runner. addTest( tnlTupleTester< 3, double > :: suite() );
   runner. addTest( tnlTupleTester< 4, double > :: suite() );

   runner. addTest( tnlArrayManagerTester< int, tnlHost, int > :: suite() );

   runner. addTest( tnlVectorHostTester< int > :: suite() );
   runner. addTest( tnlVectorHostTester< float > :: suite() );
   runner. addTest( tnlVectorHostTester< double > :: suite() );

   runner. addTest( tnlArrayTester< int, tnlHost, int > :: suite() );
   runner. addTest( tnlArrayTester< float, tnlHost, int > :: suite() );
   runner. addTest( tnlArrayTester< double, tnlHost, int > :: suite() );

   /*runner. addTest( tnlCSRMatrixTester< float > :: suite() );
   runner. addTest( tnlCSRMatrixTester< double > :: suite() );*/

   runner. addTest( tnlRgCSRMatrixTester< float > :: suite() );
   runner. addTest( tnlRgCSRMatrixTester< double > :: suite() );

   runner. addTest( tnlEllpackMatrixTester< float > :: suite() );
   runner. addTest( tnlEllpackMatrixTester< double > :: suite() );

   //runner. addTest( tnlAdaptiveRgCSRMatrixTester< float > :: suite() );
   runner. addTest( tnlAdaptiveRgCSRMatrixTester< double, tnlHost > :: suite() );

   runner. addTest( tnlMPIMeshTester< float > :: suite() );

   //runner. addTest( tnlSharedMemoryTester< tnlHost > :: suite() );

   //runner. addTest( tnlCommunicatorTester< tnlHost > :: suite() );


#ifdef HAVE_CUDA
   runner. addTest( tnlFileTester :: suite() );

   runner. addTest( tnlArrayManagerTester< int, tnlCuda, int > :: suite() );

   /*runner.addTest( tnlVectorCUDATester< int > :: suite() );
   runner.addTest( tnlVectorCUDATester< float > :: suite() );
   if( CUDA_ARCH == 13 )
      runner.addTest( tnlVectorCUDATester< double > :: suite() );

   runner.addTest( tnlFieldCUDA2DTester< int > :: suite() );
   runner.addTest( tnlFieldCUDA2DTester< float > :: suite() );
   if( CUDA_ARCH == 13 )
      runner.addTest( tnlFieldCUDA2DTester< double > :: suite() );

   runner.addTest( tnlGridCUDA2DTester< int > :: suite() );
   runner.addTest( tnlGridCUDA2DTester< float > :: suite() );
   if( CUDA_ARCH == 13 )
      runner.addTest( tnlGridCUDA2DTester< double > :: suite() );

   runner.addTest( tnlCUDAKernelsTester< int > :: suite() );
   runner.addTest( tnlCUDAKernelsTester< float > :: suite() );
   if( CUDA_ARCH == 13 )
      runner.addTest( tnlCUDAKernelsTester< double > :: suite() );

   runner.addTest( tnlMersonSolverTester< float, tnlCuda, int > :: suite() );
   if( CUDA_ARCH == 13 )
      runner.addTest( tnlMersonSolverTester< double, tnlCuda, int > :: suite() );*/

   runner. addTest( tnlAdaptiveRgCSRMatrixTester< double, tnlCuda > :: suite() );
#endif


   runner.run();
   return 0;
}