/***************************************************************************
                          tnl-init.cpp  -  description
                             -------------------
    begin                : Nov 23, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include "tnl-init.h"

#include <TNL/File.h>
#include <TNL/Config/ConfigDescription.h>
#include <TNL/Config/ParameterContainer.h>
#include <TNL/Functions/TestFunction.h>
#include <TNL/Meshes/DummyMesh.h>
#include <TNL/Meshes/Grid.h>

#include <TNL/Communicators/NoDistrCommunicator.h>
#include <TNL/Communicators/MpiCommunicator.h>


using namespace TNL;

void setupConfig( Config::ConfigDescription& config )
{
   config.addDelimiter                            ( "General settings:" );
   config.addEntry< String >( "mesh", "Mesh file. If none is given, a regular rectangular mesh is assumed.", "mesh.tnl" );
   config.addEntry< String >( "real-type", "Precision of the function evaluation.", "mesh-real-type" );
      config.addEntryEnum< String >( "mesh-real-type" );
      config.addEntryEnum< String >( "float" );
      config.addEntryEnum< String >( "double" );
      config.addEntryEnum< String >( "long-double" );
   config.addEntry< double >( "initial-time", "Initial time for a serie of snapshots of the time-dependent function.", 0.0 );
   config.addEntry< double >( "final-time", "Final time for a serie of snapshots of the time-dependent function.", 0.0 );
   config.addEntry< double >( "snapshot-period", "Period between snapshots in a serie of the time-dependent function.", 0.0 );
   config.addEntry< int >( "x-derivative", "Order of the partial derivative w.r.t x.", 0 );
   config.addEntry< int >( "y-derivative", "Order of the partial derivative w.r.t y.", 0 );
   config.addEntry< int >( "z-derivative", "Order of the partial derivative w.r.t <.", 0 );
   config.addEntry< bool >( "numerical-differentiation", "The partial derivatives will be computed numerically.", false );
   config.addRequiredEntry< String >( "output-file", "Output file name." );
   config.addEntry< bool >( "check-output-file", "If the output file already exists, do not recreate it.", false );
   config.addEntry< String >( "help", "Write help." );
 
   config.addDelimiter                            ( "Functions parameters:" );
   Functions::TestFunction< 1 >::configSetup( config );
}



int main( int argc, char* argv[] )
{

   Config::ParameterContainer parameters;
   Config::ConfigDescription conf_desc;

   setupConfig( conf_desc );

    //iniicialization needs argc and argc-> needs to be close to main
       Communicators::NoDistrCommunicator::Init(argc,argv, true);
#ifdef HAVE_MPI
       Communicators::MpiCommunicator::Init(argc,argv,true);
#endif
 
   if( ! parseCommandLine( argc, argv, conf_desc, parameters ) )
      return EXIT_FAILURE;

   String meshFile = parameters. getParameter< String >( "mesh" );
   String meshType;
   if( ! getObjectType( meshFile, meshType ) )
   {
      std::cerr << "I am not able to detect the mesh type from the file " << meshFile << "." << std::endl;
      return EXIT_FAILURE;
   }
   std::cout << meshType << " detected in " << meshFile << " file." << std::endl;
   Containers::List< String > parsedMeshType;
   if( ! parseObjectType( meshType, parsedMeshType ) )
   {
      std::cerr << "Unable to parse the mesh type " << meshType << "." << std::endl;
      return EXIT_FAILURE;
   }
   if( ! resolveMeshType( parsedMeshType, parameters ) )
      return EXIT_FAILURE;

#ifdef HAVE_MPI
   Communicators::MpiCommunicator::Finalize();
#endif
      
   return EXIT_SUCCESS;
}
