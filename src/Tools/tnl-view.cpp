/***************************************************************************
                          tnl-view.cpp  -  description
                             -------------------
    begin                : Jan 21, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include "tnl-view.h"
#include <cstdlib>
#include <TNL/File.h>
#include <TNL/Config/ConfigDescription.h>
#include <TNL/Config/ParameterContainer.h>
#include <TNL/Meshes/DummyMesh.h>
#include <TNL/Meshes/Grid.h>
#include <TNL/Meshes/TypeResolver/TypeResolver.h>

struct TNLViewBuildConfigTag {};

namespace TNL {
namespace Meshes {
namespace BuildConfigTags {

/****
 * Turn off support for float and long double.
 */
template<> struct GridRealTag< TNLViewBuildConfigTag, float > { enum { enabled = false }; };
template<> struct GridRealTag< TNLViewBuildConfigTag, long double > { enum { enabled = false }; };

/****
 * Turn off support for short int and long int indexing.
 */
template<> struct GridIndexTag< TNLViewBuildConfigTag, short int >{ enum { enabled = false }; };
template<> struct GridIndexTag< TNLViewBuildConfigTag, long int >{ enum { enabled = false }; };

/****
 * Unstructured meshes.
 */
template<> struct MeshCellTopologyTag< TNLViewBuildConfigTag, MeshEdgeTopology > { enum { enabled = true }; };

// Meshes are enabled only for the world dimension equal to the cell dimension.
template< typename CellTopology, int WorldDimension >
struct MeshWorldDimensionTag< TNLViewBuildConfigTag, CellTopology, WorldDimension >
{ enum { enabled = ( WorldDimension == CellTopology::dimension ) }; };

// Meshes are enabled only for types explicitly listed below.
template< typename Real > struct MeshRealTag< TNLViewBuildConfigTag, Real > { enum { enabled = false }; };
template< typename GlobalIndex > struct MeshGlobalIndexTag< TNLViewBuildConfigTag, GlobalIndex > { enum { enabled = false }; };
template< typename LocalIndex > struct MeshLocalIndexTag< TNLViewBuildConfigTag, LocalIndex > { enum { enabled = false }; };
template< typename GlobalIndex, typename Id > struct MeshIdTag< TNLViewBuildConfigTag, GlobalIndex, Id > { enum { enabled = false }; };
template<> struct MeshRealTag< TNLViewBuildConfigTag, double > { enum { enabled = true }; };
template<> struct MeshGlobalIndexTag< TNLViewBuildConfigTag, int > { enum { enabled = true }; };
template<> struct MeshLocalIndexTag< TNLViewBuildConfigTag, short int > { enum { enabled = true }; };
template< typename GlobalIndex > struct MeshIdTag< TNLViewBuildConfigTag, GlobalIndex, void > { enum { enabled = false }; };
template< typename GlobalIndex > struct MeshIdTag< TNLViewBuildConfigTag, GlobalIndex, GlobalIndex > { enum { enabled = true }; };

} // namespace BuildConfigTags
} // namespace Meshes
} // namespace TNL

void setupConfig( Config::ConfigDescription& config )
{
   config.addDelimiter                            ( "General settings:" );
   config.addEntry        < String >           ( "mesh", "Mesh file.", "mesh.tnl" );
   config.addRequiredList < String >           ( "input-files", "Input files." );
   config.addList         < String >           ( "output-files", "Output files." );
   config.addEntry        < bool >                ( "check-output-file", "If the output file already exists, do not recreate it.", false );

   config.addDelimiter( "Grid settings:");
   config.addList         < double >              ( "level-lines", "List of level sets which will be drawn." );
   config.addEntry        < int >                 ( "output-x-size", "X size of the output." );
   config.addEntry        < int >                 ( "output-y-size", "Y size of the output." );
   config.addEntry        < int >                 ( "output-z-size", "Z size of the output." );
   config.addEntry        < double >              ( "scale", "Multiply the function by given number.", 1.0 );
   config.addEntry        < String >           ( "output-format", "Output file format.", "gnuplot" );
      config.addEntryEnum  < String >             ( "gnuplot" );
      config.addEntryEnum  < String >             ( "vtk" );
   config.addEntry        < int >                 ( "verbose", "Set the verbosity of the program.", 1 );

   config.addDelimiter( "Matrix settings:" );
   config.addEntry        < String >           ( "matrix-format", "Matrix format to be drawn." );
      config.addEntryEnum  < String >             ( "csr" );
      config.addEntryEnum  < String >             ( "ellpack" );
      config.addEntryEnum  < String >             ( "sliced-ellpack" );
      config.addEntryEnum  < String >             ( "chunked-ellpack" );
   config.addEntry        < int >                 ( "matrix-slice-size", "Sets the slice size of the matrix.", 0 );
   config.addEntry        < int >                 ( "desired-matrix-chunk-size", "Sets desired chunk size for the Chunked Ellpack format.");
   config.addEntry        < int >                 ( "cuda-block-size", "Sets CUDA block size for the Chunked Ellpack format." );
   config.addEntry       < bool >                 ( "sort-matrix", "Sort the matrix rows decreasingly by the number of the non-zero elements.", false );
}

int main( int argc, char* argv[] )
{
   Config::ParameterContainer parameters;
   Config::ConfigDescription conf_desc;
   setupConfig( conf_desc );
   if( ! parseCommandLine( argc, argv, conf_desc, parameters ) )
      return EXIT_FAILURE;

   String meshFile = parameters.getParameter< String >( "mesh" );
   return ! TNL::Meshes::resolveMeshType< TNLViewBuildConfigTag,
                                          Devices::Host,
                                          FilesProcessor >
                                             ( meshFile,
                                                parameters );
}
