/***************************************************************************
                          tnlDebug.cpp  -  description
                             -------------------
    begin                : 2004/09/05
    copyright            : (C) 2004 by Tomas Oberhuber
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

#include <string>
#include <fstream>
#include "tnlDebugParser.h"
#include "tnlDebugStructure.h"
#include "tnlDebug.h"

using namespace std;

int __tnldbg_mpi_i_proc;

static tnlDebugStructure __tnl_debug_structure;
//--------------------------------------------------------------------------
bool tnlInitDebug( const char* file_name, const char* program_name )
{
   cout << "tnlDebug initiation..." << endl;
   tnlDebugParser debug_parser; 
   fstream in_file;
   in_file. open( file_name, ios :: in );
   if( ! in_file )
   {
      cerr << "Unable to open file " << file_name << endl;
      return false;
   }
   debug_parser. setScanner( &in_file ); 
   int errs = debug_parser. runParsing( &__tnl_debug_structure );
   if( errs != 0 )
   {    
      cerr << errs << " errors occured while parsing " << file_name << endl;
      return false;
   }
   cout << "Parsing of " << file_name << " succesfuly done ..." << endl;
#ifdef DEBUG
   cout << "Parsed data are ... " << endl;
   __tnl_debug_structure. Print();
   cout << "---------- end of listing -----------" << endl;
#endif
   return true;
}
//--------------------------------------------------------------------------
bool _tnldbg_debug_func( const char* group_name,
                         const char* function_name )
{
   bool debug = __tnl_debug_structure. Debug( group_name, function_name );
   //cout << "Debug ( " << group_name << ", " << function_name 
   //     << " ) -> " << debug << endl;
   return debug;
}
//--------------------------------------------------------------------------
bool _tnldbg_interactive_func( const char* group_name,
                               const char* function_name )
{
   return __tnl_debug_structure. Interactive( group_name, function_name );
}
