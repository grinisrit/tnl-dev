/***************************************************************************
                          tnlLogger.cpp  -  description
                             -------------------
    begin                : 2007/08/22
    copyright            : (C) 2007 by Tomas Oberhuber
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

#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>
#include <fstream>
#include <iomanip>
#include <core/tnlLogger.h>
#include <tnlConfig.h>

tnlLogger :: tnlLogger( int _width,
                        ostream& _stream )
: width( _width ),
  stream( _stream )
{
}

void tnlLogger :: writeHeader( const tnlString& title )
{
   int fill = stream. fill(); 
   int titleLength = title. getLength();
   stream << "+" << setfill( '-' ) << setw( width ) << "+" << endl;
   stream << "|" << setfill( ' ' ) << setw( width ) << "|" << endl;
   stream << "|" << setw( width / 2 + titleLength / 2 )
    << title << setw( width / 2 - titleLength / 2  ) << "|" << endl;
   stream << "|" << setfill( ' ' ) << setw( width ) << "|" << endl;
   stream << "+" << setfill( '-' ) << setw( width ) << "+" << endl;
   stream. fill( fill );
}

void tnlLogger :: writeSeparator()
{
   int fill = stream. fill(); 
   stream << "+" << setfill( '-' ) << setw( width ) << "+" << endl;
   stream. fill( fill );
}

bool tnlLogger :: writeSystemInformation()
{
   char host_name[ 256 ];
   struct utsname uts;
   gethostname( host_name, 255 );
   uname( &uts );
   writeParameter< char* >( "Host name:", host_name );
   writeParameter< char* >( "Architecture:", uts. machine );
   fstream file;
   file. open( "/proc/cpuinfo", ios :: in );
   if( file )
   {
      char line[ 1024 ];
      char* cpu_id;
      char* cpu_model_name;
      char* cpu_mhz;
      char* cpu_cache;
      while( ! file. eof() )
      {
         int i;
         file. getline( line, 1024 );
         if( strncmp( line, "processor", strlen( "processor" ) ) == 0 )
         {
            i = strlen( "processor" );
            while( line[ i ] != ':' && line[ i ] ) i ++;
            cpu_id = &line[ i + 1 ];
            writeParameter< char * >( "CPU Id.:", cpu_id );
            continue;
         }
         if( strncmp( line, "model name", strlen( "model name" ) ) == 0 )
         {
            i = strlen( "model name" );
            while( line[ i ] != ':' && line[ i ] ) i ++;
            cpu_model_name = &line[ i + 1 ];
            writeParameter< char * >( "Model name:", cpu_model_name );
            continue;
         }
         if( strncmp( line, "cpu MHz", strlen( "cpu MHz" ) ) == 0 )
         {
            i = strlen( "cpu MHz" );
            while( line[ i ] != ':' && line[ i ] ) i ++;
            cpu_mhz = &line[ i + 1 ];
            writeParameter< char * >( "CPU MHz:", cpu_mhz );
            continue;
         }
         if( strncmp( line, "cache size", strlen( "cache size" ) ) == 0 )
         {
            i = strlen( "cache size" );
            while( line[ i ] != ':' && line[ i ] ) i ++;
            cpu_cache = &line[ i + 1 ];
            writeParameter< char * >( "CPU cache:", cpu_cache );
            continue;
         }
      }
   }
   else
   {
      cerr << "Unable to read information from /proc/cpuinfo." << endl;
      return false;
   }
   file. close();
   writeParameter< char* >( "System:", uts. sysname );
   writeParameter< char* >( "Release:", uts. release );
   writeParameter< char* >( "TNL Compiler:", ( char* ) TNL_CPP_COMPILER_NAME );
   return true;
}

void tnlLogger :: writeCurrentTime( const char* label )
{
   time_t timeval;
   time( &timeval );
   tm *tm_ptr = localtime( &timeval );
   char buf[ 256 ];
   strftime( buf, 256, "%a %b %d %H:%M:%S\0", tm_ptr );
   writeParameter< char* >( label, buf );
}

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION
template void tnlLogger::writeParameter< char* >( const tnlString&,
                                                  const tnlString&,
                                                  const tnlParameterContainer&,
                                                  int );
template void tnlLogger::writeParameter< double >( const tnlString&,
                                                   const tnlString&,
                                                   const tnlParameterContainer&,
                                                   int );
template void tnlLogger::writeParameter< int >( const tnlString&,
                                                const tnlString&,
                                                const tnlParameterContainer&,
                                                int );

// TODO: fix this
//template void tnlLogger :: WriteParameter< char* >( const char*,
//                                                    const char*&,
//                                                    int );
template void tnlLogger::writeParameter< double >( const tnlString&,
                                                   const double&,
                                                   int );
template void tnlLogger::writeParameter< int >( const tnlString&,
                                                const int&,
                                                int );

#endif