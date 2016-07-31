/***************************************************************************
                          tnl-mesh-convert.h  -  description
                             -------------------
    begin                : Feb 19, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNL_MESH_CONVERT_H_
#define TNL_MESH_CONVERT_H_

#include <TNL/Config/ParameterContainer.h>
#include <TNL/mesh/tnlMeshReaderNetgen.h>
#include <TNL/mesh/tnlMeshWriterVTKLegacy.h>
#include <TNL/mesh/config/tnlMeshConfigBase.h>
#include <TNL/mesh/topologies/tnlMeshTriangleTopology.h>
#include <TNL/mesh/topologies/tnlMeshTetrahedronTopology.h>
#include <TNL/mesh/tnlMesh.h>
#include <TNL/mesh/initializer/tnlMeshInitializer.h>
#include <TNL/mesh/tnlMeshIntegrityChecker.h>
#include <TNL/core/mfilename.h>

using namespace TNL;

template< typename MeshReader,
          typename MeshType >
bool convertMesh( const Config::ParameterContainer& parameters )
{
   const String& inputFileName = parameters.getParameter< String >( "input-file" );
   const String& outputFileName = parameters.getParameter< String >( "output-file" );
   const String outputFileExt = getFileExtension( outputFileName );

   MeshType mesh;
   if( ! MeshReader::readMesh( inputFileName, mesh, true ) )
      return false;
   /*tnlMeshInitializer< typename MeshType::Config > meshInitializer;
   meshInitializer.setVerbose( true );
   if( ! meshInitializer.initMesh( mesh ) )
      return false;
   if( ! tnlMeshIntegrityChecker< MeshType >::checkMesh( mesh ) )
      return false;*/
  std::cout << mesh << std::endl;
  std::cout << "Writing the mesh to a file " << outputFileName << "." << std::endl;
   if( outputFileExt == "tnl" )
   {
      if( ! mesh.save( outputFileName ) )
      {
         std::cerr << "I am not able to write the mesh into the file " << outputFileName << "." << std::endl;
         return false;
      }
   }
   if( outputFileExt == "vtk" )
   {
      if( ! tnlMeshWriterVTKLegacy::write( outputFileName, mesh, true ) )
      {
         std::cerr << "I am not able to write the mesh into the file " << outputFileName << "." << std::endl;
         return false;
      }
      return true;
   }
}

bool readNetgenMesh( const Config::ParameterContainer& parameters )
{
   const String& inputFileName = parameters.getParameter< String >( "input-file" );
 
   tnlMeshReaderNetgen meshReader;
   if( ! meshReader.detectMesh( inputFileName ) )
      return false;

  std::cout << "Reading mesh with " << meshReader.getDimensions() << " dimensions..." << std::endl;
 
   if( meshReader.getDimensions() == 2 )
   {
      if( meshReader.getVerticesInCell() == 3 )
      {
         typedef tnlMesh< tnlMeshConfigBase< tnlMeshTriangleTopology > > MeshType;
        std::cout << "Mesh consisting of triangles was detected ... " << std::endl;
         return convertMesh< tnlMeshReaderNetgen, MeshType >( parameters );
      }
      if( meshReader.getVerticesInCell() == 4 )
      {
         typedef tnlMesh< tnlMeshConfigBase< tnlMeshQuadrilateralTopology > > MeshType;
        std::cout << "Mesh consisting of quadrilaterals was detected ... " << std::endl;
         return convertMesh< tnlMeshReaderNetgen, MeshType >( parameters );
      }
   }
   if( meshReader.getDimensions() == 3 )
   {
      if( meshReader.getVerticesInCell() == 4 )
      {
         typedef tnlMesh< tnlMeshConfigBase< tnlMeshTetrahedronTopology > > MeshType;
        std::cout << "Mesh consisting of tetrahedrons was detected ... " << std::endl;
         return convertMesh< tnlMeshReaderNetgen, MeshType >( parameters );
      }
      if( meshReader.getVerticesInCell() == 8 )
      {
         typedef tnlMesh< tnlMeshConfigBase< tnlMeshHexahedronTopology > > MeshType;
        std::cout << "Mesh consisting of hexahedrons was detected ... " << std::endl;
         return convertMesh< tnlMeshReaderNetgen, MeshType >( parameters );
      }
   }
   std::cerr << "Wrong mesh dimensions were detected ( " << meshReader.getDimensions() << " )." << std::endl;
   return false;
}

bool convertMesh( const Config::ParameterContainer& parameters )
{
   String inputFileName = parameters.getParameter< String >( "input-file" );

   const String fileExt = getFileExtension( inputFileName );
   if( fileExt == "ng" )
      return readNetgenMesh( parameters );
}


#endif /* TNL_MESH_CONVERT_H_ */
