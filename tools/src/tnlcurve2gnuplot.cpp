/***************************************************************************
                          tnlCurve2gnuplot.cpp  -  description
                             -------------------
    begin                : 2007/12/16
    copyright            : (C) 2007 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <config/tnlParameterContainer.h>
#include <core/tnlCurve.h>
#include <core/vectors/tnlStaticVector.h>

#include "tnlConfig.h"
//const char configFile[] = TNL_CONFIG_DIRECTORY "tnlcurve2gnuplot.cfg.desc";

using namespace TNL;

void setupConfig( tnlConfigDescription& config )
{
   config.addDelimiter                            ( "General settings:" );
   config.addRequiredList< tnlString >(  "input-files", "Input files." );
   config.addList< tnlString >( "output-files", "Output files." );
   config.addEntry< int >( "output-step", "Decrease number of the output curve nodes." );
   config.addEntry< tnlString >( "output-file-format", "Output file format. Can be gnuplot.", "gnuplot" );
}

//--------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
   tnlParameterContainer parameters;
   tnlConfigDescription conf_desc;
 
   setupConfig( conf_desc );
   if( ! parseCommandLine( argc, argv, conf_desc, parameters ) )
   {
      conf_desc.printUsage( argv[ 0 ] );
      return 1;
   }

   tnlList< tnlString > input_files = parameters. getParameter< tnlList< tnlString > >( "input-files" );
   tnlList< tnlString > output_files;
   if( ! parameters. getParameter< tnlList< tnlString > >( "output-files", output_files ) )
      std::cout << "No output files were given." << std::endl;
   int output_step( 1 );
   parameters. getParameter< int >( "output-step", output_step );
   tnlString output_file_format = parameters. getParameter< tnlString >( "output-file-format" );

   int size = input_files. getSize();
   /*if( size != output_files. getSize() )
   {
      std::cerr << "Sorry, there is different number of input and output files." << std::endl;
      return 1;
   }*/
   int i;
   tnlCurve< tnlStaticVector< 2, double > > crv( "tnlcurve2gnuplot:curve" );
   for( i = 0; i < size; i ++ )
   {
      const char* input_file = input_files[ i ]. getString();
      std::cout << "Processing file " << input_file << " ... " << std::flush;
 
      tnlFile file;
      if( ! file. open( input_files[ i ], tnlReadMode ) )
      {
         std::cout << " unable to open file " << input_files[ i ] << std::endl;
         continue;
      }
      if( ! crv. load( file ) )
      {
         std::cout << " unable to restore the data " << std::endl;
         continue;
      }
      file. close();

      tnlCurve< tnlStaticVector< 2, double > > out_crv( "tnlcurve2gnuplot:outcurve" );
      const int size = crv. getSize();
      int i;
      for( i = 0; i < size; i += output_step )
      {
         out_crv. Append( crv[ i ]. position, crv[ i ]. separator );
         //tnlStaticVector< 2, double > v = crv[ i ]. position;
         //v[ 0 ] = u( i );
         //v[ 1 ] = u( i + 1 );
         //out_crv. Append( v );
      }

      tnlString output_file_name;
      if( ! output_files. isEmpty() ) output_file_name = output_files[ i ];
      else
      {
         if( output_file_format == "gnuplot" )
            output_file_name += ".gplt";
      }
      std::cout << " writing... " << output_file_name << std::endl;
      if( ! Write( out_crv, output_file_name. getString(), output_file_format. getString() ) )
      {
         std::cerr << " unable to write to " << output_file_name << std::endl;
      }
   }
}
