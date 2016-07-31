/***************************************************************************
                          ArrayOperationsTest.cu  -  description
                             -------------------
    begin                : Jul 16, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include "tnlArrayOperationsTester.h"
#include "../../tnlUnitTestStarter.h"
#include <TNL/Devices/Cuda.h>

using namespace TNL;

int main( int argc, char* argv[] )
{
   if( ! tnlUnitTestStarter :: run< ArrayOperationsTester< int, Devices::Cuda > >() )
      return EXIT_FAILURE;
   return EXIT_SUCCESS;
}
