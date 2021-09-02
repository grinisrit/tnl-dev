/***************************************************************************
                          PVTIWriter.hpp  -  description
                             -------------------
    begin                : Jun 25, 2021
    copyright            : (C) 2021 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovský

#pragma once

#include <experimental/filesystem>

#include <TNL/Meshes/Writers/PVTIWriter.h>

namespace TNL {
namespace Meshes {
namespace Writers {

template< typename Grid >
void
PVTIWriter< Grid >::writeMetadata( int cycle, double time )
{
   if( ! vtkfileOpen )
      writeHeader();
   if( pImageDataOpen )
      throw std::logic_error("The <PImageData> tag is already open, but writeMetadata should be called before writeImageData.");

   if( cycle >= 0 || time >= 0 )
      metadata << "<FieldData>\n";

   if( cycle >= 0 ) {
      metadata << "<DataArray type=\"Int32\" Name=\"CYCLE\" NumberOfTuples=\"1\" format=\"ascii\">"
          << cycle << "</DataArray>\n";
   }
   if( time >= 0 ) {
      metadata.precision( std::numeric_limits< double >::digits10 );
      metadata << "<DataArray type=\"Float64\" Name=\"TIME\" NumberOfTuples=\"1\" format=\"ascii\">"
          << time << "</DataArray>\n";
   }

   if( cycle >= 0 || time >= 0 )
      metadata << "</FieldData>\n";
}

template< typename Grid >
void
PVTIWriter< Grid >::writeImageData( const DistributedMeshes::DistributedMesh< Grid >& distributedGrid )
{
   writeImageData( distributedGrid.getGlobalGrid(), distributedGrid.getGhostLevels() ); // TODO: ..., Grid::Config::dualGraphMinCommonVertices );
}

template< typename Grid >
void
PVTIWriter< Grid >::writeImageData( const Grid& globalGrid,
                                    const unsigned GhostLevel,
                                    const unsigned MinCommonVertices )
{
   if( ! vtkfileOpen )
      writeHeader();
   if( pImageDataOpen )
      throw std::logic_error("The <PImageData> tag is already open.");

   std::stringstream extent, origin, spacing;

   auto dims = globalGrid.getDimensions();
   for( IndexType j = 0; j < dims.getSize(); j++ )
      extent << "0 " << dims[ j ] << " ";
   // VTK knows only 3D grids
   for( IndexType j = dims.getSize(); j < 3; j++ )
      extent << "0 0 ";

   auto o = globalGrid.getOrigin();
   for( IndexType j = 0; j < o.getSize(); j++ )
      origin << std::scientific << o[ j ] << " ";
   // VTK knows only 3D grids
   for( IndexType j = o.getSize(); j < 3; j++ )
      origin << 0 << " ";

   auto h = globalGrid.getSpaceSteps();
   for( IndexType j = 0; j < h.getSize(); j++ )
      spacing << std::scientific << h[ j ] << " ";
   // VTK knows only 3D grids
   for( IndexType j = h.getSize(); j < 3; j++ )
      spacing << 0 << " ";

   str << "<PImageData"
       << " WholeExtent=\"" << extent.str() << "\""
       << " Origin=\"" << origin.str() << "\""
       << " Spacing=\"" << spacing.str() << "\""
       << " GhostLevel=\"" << GhostLevel << "\"";
   if( MinCommonVertices > 0 )
      str << " MinCommonVertices=\"" << MinCommonVertices << "\"";
   str << ">\n";
   pImageDataOpen = true;

   // write metadata if present
   if( ! metadata.str().empty() ) {
      str << metadata.str();
      metadata.str() = "";
   }
}

template< typename Grid >
   template< int EntityDimension >
void
PVTIWriter< Grid >::writeEntities( const DistributedMeshes::DistributedMesh< Grid >& distributedMesh )
{
   writeImageData( distributedMesh );
}

template< typename Grid >
   template< int EntityDimension >
void
PVTIWriter< Grid >::writeEntities( const Grid& grid,
                                   const unsigned GhostLevel,
                                   const unsigned MinCommonVertices )
{
   writeImageData( grid, GhostLevel, MinCommonVertices );
}

template< typename Grid >
   template< typename ValueType >
void
PVTIWriter< Grid >::writePPointData( const String& name,
                                     const int numberOfComponents )
{
   if( ! vtkfileOpen )
      throw std::logic_error("The VTKFile has not been opened yet - call writeEntities first.");
   openPPointData();
   writePDataArray< ValueType >( name, numberOfComponents );
}

template< typename Grid >
   template< typename ValueType >
void
PVTIWriter< Grid >::writePCellData( const String& name,
                                    const int numberOfComponents )
{
   if( ! vtkfileOpen )
      throw std::logic_error("The VTKFile has not been opened yet - call writeEntities first.");
   openPCellData();
   writePDataArray< ValueType >( name, numberOfComponents );
}

template< typename Grid >
   template< typename ValueType >
void
PVTIWriter< Grid >::writePDataArray( const String& name,
                                     const int numberOfComponents )
{
   if( numberOfComponents != 0 && numberOfComponents != 1 && numberOfComponents != 3 )
      throw std::logic_error("Unsupported numberOfComponents parameter: " + std::to_string(numberOfComponents));

   str << "<PDataArray type=\"" << VTK::getTypeName( ValueType{} ) << "\" ";
   str << "Name=\"" << name << "\" ";
   str << "NumberOfComponents=\"" << numberOfComponents << "\"/>\n";
}

template< typename Grid >
std::string
PVTIWriter< Grid >::addPiece( const String& mainFileName,
                              const unsigned subdomainIndex,
                              const typename Grid::CoordinatesType& globalBegin,
                              const typename Grid::CoordinatesType& globalEnd )
{
   if( ! mainFileName.endsWith( ".pvti" ) )
      throw std::logic_error("The mainFileName parameter must be the name of the "
                             ".pvti file (i.e., it must have the .pvti suffix).");

   // close PCellData and PPointData sections
   closePCellData();
   closePPointData();

   // prepare the extent
   std::stringstream extent;
   for( IndexType j = 0; j < Grid::getMeshDimension(); j++ )
      extent << globalBegin[ j ] <<  " " << globalEnd[ j ] << " ";
   // VTK knows only 3D grids
   for( IndexType j = Grid::getMeshDimension(); j < 3; j++ )
      extent << "0 0 ";

   namespace fs = std::experimental::filesystem;

   // get the basename of the main file (filename without extension)
   const fs::path mainPath = mainFileName.getString();
   const fs::path basename = mainPath.stem();

   // create subdirectory for subdomains
   const fs::path subdirectory = mainPath.parent_path() / basename;
   fs::create_directory( subdirectory );

   // write <Piece> tag
   const std::string subfile = "subdomain." + std::to_string(subdomainIndex) + ".vti";
   const std::string source = basename / subfile;
   str << "<Piece Extent=\"" << extent.str() << "\" Source=\"" << source << "\"/>\n";

   // return subfile path
   return subdirectory / subfile;
}

template< typename Grid >
std::string
PVTIWriter< Grid >::addPiece( const String& mainFileName,
                              const DistributedMeshes::DistributedMesh< Grid >& distributedMesh )
{
   const MPI_Comm group = distributedMesh.getCommunicationGroup();
   const typename Grid::CoordinatesType& globalBegin = distributedMesh.getGlobalBegin() - distributedMesh.getLowerOverlap();
   const typename Grid::CoordinatesType& globalEnd = globalBegin + distributedMesh.getLocalSize() + distributedMesh.getUpperOverlap();

   // exchange globalBegin and globalEnd among the ranks
   const int nproc = MPI::GetSize( group );
   typename Grid::CoordinatesType beginsForScatter[ nproc ];
   typename Grid::CoordinatesType endsForScatter[ nproc ];
   for( int i = 0; i < nproc; i++ ) {
      beginsForScatter[ i ] = globalBegin;
      endsForScatter[ i ] = globalEnd;
   }
   typename Grid::CoordinatesType globalBegins[ nproc ];
   typename Grid::CoordinatesType globalEnds[ nproc ];
   // NOTE: exchanging general data types does not work with MPI
   //MPI::Alltoall( beginsForScatter, 1, globalBegins, 1, group );
   //MPI::Alltoall( endsForScatter, 1, globalEnds, 1, group );
   MPI::Alltoall( (char*) beginsForScatter, sizeof(typename Grid::CoordinatesType), (char*) globalBegins, sizeof(typename Grid::CoordinatesType), group );
   MPI::Alltoall( (char*) endsForScatter, sizeof(typename Grid::CoordinatesType), (char*) globalEnds, sizeof(typename Grid::CoordinatesType), group );

   // add pieces for all ranks, return the source for the current rank
   std::string source;
   for( int i = 0; i < MPI::GetSize( group ); i++ ) {
      const std::string s = addPiece( mainFileName, i, globalBegins[ i ], globalEnds[ i ] );
      if( i == MPI::GetRank( group ) )
         source = s;
   }
   return source;
}

template< typename Grid >
void
PVTIWriter< Grid >::writeHeader()
{
   str << "<?xml version=\"1.0\"?>\n";
   str << "<VTKFile type=\"PImageData\" version=\"1.0\"";
   if( isLittleEndian() )
      str << " byte_order=\"LittleEndian\"";
   else
      str << " byte_order=\"BigEndian\"";
   str << " header_type=\"" << VTK::getTypeName( HeaderType{} ) << "\"";
#ifdef HAVE_ZLIB
   if( format == VTK::FileFormat::zlib_compressed )
      str << " compressor=\"vtkZLibDataCompressor\"";
#endif
   str << ">\n";

   vtkfileOpen = true;
}

template< typename Grid >
void
PVTIWriter< Grid >::writeFooter()
{
   closePCellData();
   closePPointData();
   str << "</PImageData>\n";
   str << "</VTKFile>\n";
}

template< typename Grid >
PVTIWriter< Grid >::~PVTIWriter()
{
   if( vtkfileOpen )
      writeFooter();
}

template< typename Grid >
void
PVTIWriter< Grid >::openPCellData()
{
   if( pCellDataClosed )
      throw std::logic_error("The <PCellData> tag has already been closed.");
   closePPointData();
   if( ! pCellDataOpen ) {
      str << "<PCellData>\n";
      pCellDataOpen = true;
   }
}

template< typename Grid >
void
PVTIWriter< Grid >::closePCellData()
{
   if( pCellDataOpen ) {
      str << "</PCellData>\n";
      pCellDataClosed = true;
      pCellDataOpen = false;
   }
}

template< typename Grid >
void
PVTIWriter< Grid >::openPPointData()
{
   if( pPointDataClosed )
      throw std::logic_error("The <PPointData> tag has already been closed.");
   closePCellData();
   if( ! pPointDataOpen ) {
      str << "<PPointData>\n";
      pPointDataOpen = true;
   }
}

template< typename Grid >
void
PVTIWriter< Grid >::closePPointData()
{
   if( pPointDataOpen ) {
      str << "</PPointData>\n";
      pPointDataClosed = true;
      pPointDataOpen = false;
   }
}

} // namespace Writers
} // namespace Meshes
} // namespace TNL