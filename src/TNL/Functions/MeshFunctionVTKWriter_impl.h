/***************************************************************************
                          MeshFunctionVTKWriter_impl.h  -  description
                             -------------------
    begin                : Jan 28, 2016
    copyright            : (C) 2016 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

namespace TNL {
namespace Functions {   

template< typename MeshFunction >
bool
MeshFunctionVTKWriter< MeshFunction >::
write( const MeshFunction& function,
                         std::ostream& str )
{
   std::cerr << "VTK writer for mesh functions defined on mesh type " << MeshFunction::MeshType::getType() << " is not (yet) implemented." << std::endl;
   return false;
}

/****
 * 1D grid, cells
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, 1, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, 1, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType origin = mesh.getOrigin().x();
   const RealType spaceStep = mesh.getSpaceSteps().x();
 
   str << "POINTS " << mesh.getDimensions().x() + 1 << " float" << std::endl;
 
   for (int i = 0; i <= mesh.getDimensions().x(); i++)
   {
       str << origin + i * spaceStep << " 0 0" << std::endl;
   }
 
   str << std::endl << "CELLS " << mesh.getDimensions().x() << " " << mesh.getDimensions().x() * 3 << std::endl;
   for (int i = 0; i < mesh.getDimensions().x(); i++)
   {
       str << "2 " << i << " " << i+1 << std::endl;
   }
 
   str << std::endl << "CELL_TYPES " << mesh.getDimensions().x() << std::endl;
   for (int i = 0; i < mesh.getDimensions().x(); i++)
   {
       str << "3 " << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << mesh.getDimensions().x() << std::endl;
   str << "SCALARS cellFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typename MeshType::Cell entity( mesh );
   for( entity.getCoordinates().x() = 0;
        entity.getCoordinates().x() < mesh.getDimensions().x();
        entity.getCoordinates().x() ++ )
   {
      entity.refresh();
      str << function.getData().getElement( entity.getIndex() ) << std::endl;
   }
 
   return true;
}

/****
 * 1D grid, vertices
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, 0, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, 0, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType origin = mesh.getOrigin().x();
   const RealType spaceStep = mesh.getSpaceSteps().x();
 
   str << "POINTS " << mesh.getDimensions().x() + 1 << " float" << std::endl;
 
   for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
   {
       str << origin + i * spaceStep << " 0 0" << std::endl;
   }
 
   str << std::endl << "CELLS " << mesh.getDimensions().x() + 1 << " " << ( mesh.getDimensions().x() + 1 ) * 2 << std::endl;
   for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
   {
       str << "1 " << i << std::endl;
   }
 
   str << std::endl << "CELL_TYPES " << mesh.getDimensions().x() + 1 << std::endl;
   for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
   {
       str << "1 " << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << mesh.getDimensions().x() + 1 << std::endl;
   str << "SCALARS VerticesFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typename MeshType::Vertex entity( mesh );
   for( entity.getCoordinates().x() = 0;
        entity.getCoordinates().x() <= mesh.getDimensions().x();
        entity.getCoordinates().x() ++ )
   {
      entity.refresh();
      str << function.getData().getElement( entity.getIndex() ) << std::endl;
   }
 
   return true;
}

/****
 * 2D grid, cells
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, 2, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, 2, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
 
   str << "POINTS " << (mesh.getDimensions().x() + 1) * (mesh.getDimensions().y() + 1) << " float" << std::endl;
 
   for (int j = 0; j < mesh.getDimensions().y() + 1; j++)
   {
        for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
        {
             str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " 0" << std::endl;
        }
   }
 
   str << std::endl << "CELLS " << mesh.getDimensions().x() * mesh.getDimensions().y() << " " <<
          mesh.getDimensions().x() * mesh.getDimensions().y() * 5 << std::endl;
   for (int j = 0; j < mesh.getDimensions().y(); j++)
   {
        for (int i = 0; i < mesh.getDimensions().x(); i++)
        {
            str << "4 " << j * ( mesh.getDimensions().x() + 1 ) + i << " " << j * ( mesh.getDimensions().x() + 1 )+ i + 1 <<
                   " " << (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " " << (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1 << std::endl;
        }
   }
 
   str << std::endl << "CELL_TYPES " << mesh.getDimensions().x() * mesh.getDimensions().y() << std::endl;
   for (int i = 0; i < mesh.getDimensions().x()*mesh.getDimensions().y(); i++)
   {
       str << "8 " << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << mesh.getDimensions().x() * mesh.getDimensions().y() << std::endl;
   str << "SCALARS cellFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typename MeshType::Cell entity( mesh );
   for( entity.getCoordinates().y() = 0;
        entity.getCoordinates().y() < mesh.getDimensions().y();
        entity.getCoordinates().y() ++ )
   {
      for( entity.getCoordinates().x() = 0;
           entity.getCoordinates().x() < mesh.getDimensions().x();
           entity.getCoordinates().x() ++ )
      {
         entity.refresh();
         str << function.getData().getElement( entity.getIndex() ) << std::endl;
      }
   }
   return true;
}

/****
 * 2D grid, faces
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, 1, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, 1, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   typedef typename MeshType::template MeshEntity< 0 > Vertex;
   typedef typename MeshType::template MeshEntity< 1 > Face;
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
 
   str << "POINTS " << mesh.template getEntitiesCount< Vertex >() << " float" << std::endl;
 
   for (int j = 0; j < ( mesh.getDimensions().y() + 1); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
             str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " 0" << std::endl;
        }
   }
 
   str << std::endl << "CELLS " << mesh.template getEntitiesCount< Face >() << " " <<
          mesh.template getEntitiesCount< Face >() * 3 << std::endl;
   for (int j = 0; j < mesh.getDimensions().y(); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
            str << "2 " << j * ( mesh.getDimensions().x() + 1 ) + i << " " << (j+1) * ( mesh.getDimensions().x() + 1 ) + i << std::endl;
        }
   }
 
   for (int j = 0; j < (mesh.getDimensions().y()+1); j++)
   {
        for (int i = 0; i < mesh.getDimensions().x(); i++)
        {
            str << "2 " << j * ( mesh.getDimensions().x() + 1 ) + i << " " <<j * ( mesh.getDimensions().x() + 1 ) + i + 1<< std::endl;
        }
   }
 
   str << std::endl << "CELL_TYPES " << mesh.template getEntitiesCount< Face >() << std::endl;
   for (int i = 0; i < mesh.template getEntitiesCount< Face >(); i++)
   {
       str << "3" << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << mesh.template getEntitiesCount< Face >() << std::endl;
   str << "SCALARS FaceslFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typedef typename MeshType::Face EntityType;
   typedef typename EntityType::EntityOrientationType EntityOrientation;
   EntityType entity( mesh );
 
   entity.setOrientation( EntityOrientation( 1.0, 0.0 ) );
   for( entity.getCoordinates().y() = 0;
        entity.getCoordinates().y() < mesh.getDimensions().y();
        entity.getCoordinates().y() ++ )
   {
      for( entity.getCoordinates().x() = 0;
           entity.getCoordinates().x() <= mesh.getDimensions().x();
           entity.getCoordinates().x() ++ )
      {
         entity.refresh();
         str << function.getData().getElement( entity.getIndex() ) << std::endl;
      }
   }
 
   entity.setOrientation( EntityOrientation( 0.0, 1.0 ) );
   for( entity.getCoordinates().y() = 0;
        entity.getCoordinates().y() <= mesh.getDimensions().y();
        entity.getCoordinates().y() ++ )

   {
        for( entity.getCoordinates().x() = 0;
             entity.getCoordinates().x() < mesh.getDimensions().x();
             entity.getCoordinates().x() ++ )

      {
         entity.refresh();
         str << function.getData().getElement( entity.getIndex() ) << std::endl;
      }
   }
   return true;
}

/****
 * 2D grid, vertices
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, 0, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, 0, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   typedef typename MeshType::template MeshEntity< 0 > Vertex;
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();

 
   str << "POINTS " << mesh.template getEntitiesCount< Vertex >() << " float" << std::endl;
 
   for (int j = 0; j < ( mesh.getDimensions().y() + 1); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
             str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " 0" << std::endl;
        }
   }
 
   str << std::endl << "CELLS " << mesh.template getEntitiesCount< Vertex >() << " " <<
          mesh.template getEntitiesCount< Vertex >() * 2 << std::endl;
   for (int j = 0; j < ( mesh.getDimensions().y() + 1 ); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
            str << "1 " << j * mesh.getDimensions().x() + i  << std::endl;
        }
   }
 
   str << std::endl << "CELL_TYPES " << mesh.template getEntitiesCount< Vertex >() << std::endl;
   for (int i = 0; i < mesh.template getEntitiesCount< Vertex >(); i++)
   {
       str << "1" << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << mesh.template getEntitiesCount< Vertex >() << std::endl;
   str << "SCALARS VerticesFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typename MeshType::Vertex entity( mesh );
   for( entity.getCoordinates().y() = 0;
        entity.getCoordinates().y() <= mesh.getDimensions().y();
        entity.getCoordinates().y() ++ )
   {
      for( entity.getCoordinates().x() = 0;
           entity.getCoordinates().x() <= mesh.getDimensions().x();
           entity.getCoordinates().x() ++ )
      {
         entity.refresh();
         str << function.getData().getElement( entity.getIndex() ) << std::endl;
      }
   }
 
   return true;
}

/****
 * 3D grid, cells
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 3, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 3, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
   const RealType entitiesCount = mesh.getDimensions().x() * mesh.getDimensions().y() * mesh.getDimensions().z();
 
   str << "POINTS " << (mesh.getDimensions().x()+1) * (mesh.getDimensions().y()+1) * (mesh.getDimensions().z()+1) <<
          " float" << std::endl;
 
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " <<
                        originZ + k * spaceStepZ << std::endl;
            }
       }
   }
 
   str << std::endl << "CELLS " << entitiesCount << " " <<
          entitiesCount * 9 << std::endl;
   for (int k = 0; k < mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j < mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i < mesh.getDimensions().x(); i++)
            {
                str << "8 " <<  k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1 << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1 << std::endl;
            }
        }
   }
 
   str << std::endl << "CELL_TYPES " << entitiesCount << std::endl;
   for (int i = 0; i < entitiesCount; i++)
   {
       str << "11" << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << entitiesCount << std::endl;
   str << "SCALARS cellFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typename MeshType::Cell entity( mesh );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() < mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() < mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() < mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                entity.refresh();
                str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
   return true;
}

/****
 * 3D grid, faces
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 2, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 2, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   typedef typename MeshType::template MeshEntity< 2 > Face;
   typedef typename MeshType::template MeshEntity< 3 > Cell;
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
   const RealType entitiesCount = mesh.template getEntitiesCount< Face >();
   const RealType pointsCount = mesh.template getEntitiesCount< Cell >();
 
   str << "POINTS " << pointsCount <<
          " float" << std::endl;
 
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " <<
                        originZ + k * spaceStepZ << std::endl;
            }
       }
   }
 
   str << std::endl << "CELLS " << entitiesCount << " " <<
          entitiesCount * 5 << std::endl;
   for (int k = 0; k < mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j < mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                str << "4 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << std::endl;
            }
        }
   }
 
   for (int k = 0; k < mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j <= mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i < mesh.getDimensions().x(); i++)
            {
                str << "4 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << std::endl;
            }
        }
   }
 
   for (int k = 0; k <= mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j < mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i < mesh.getDimensions().x(); i++)
            {
                str << "4 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1<< std::endl;
            }
        }
   }
 
   str << std::endl << "CELL_TYPES " << entitiesCount << std::endl;
   for (int i = 0; i < entitiesCount; i++)
   {
       str << "8" << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << entitiesCount << std::endl;
   str << "SCALARS facesFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typedef typename MeshType::Face EntityType;
   typedef typename EntityType::EntityOrientationType EntityOrientation;
   EntityType entity( mesh );
 
   entity.setOrientation( EntityOrientation( 1.0, 0.0 , 0.0) );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() < mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() < mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() <= mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   entity.setOrientation( EntityOrientation( 0.0, 1.0 , 0.0) );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() < mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() <= mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() < mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   entity.setOrientation( EntityOrientation( 0.0, 0.0 , 1.0) );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() <= mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() < mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() < mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   return true;
}

/****
 * 3D grid, edges
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 1, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 1, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   typedef typename MeshType::template MeshEntity< 1 > Edge;
   typedef typename MeshType::template MeshEntity< 3 > Cell;
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
   const RealType entitiesCount = mesh.template getEntitiesCount< Edge >();
   const RealType pointsCount = mesh.template getEntitiesCount< Cell >();
 
   str << "POINTS " << pointsCount <<
          " float" << std::endl;
 
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " <<
                        originZ + k * spaceStepZ << std::endl;
            }
       }
   }
 
   str << std::endl << "CELLS " << entitiesCount << " " <<
          entitiesCount * 3 << std::endl;
   for (int k = 0; k <= mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j <= mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i < mesh.getDimensions().x(); i++)
            {
                str << "3 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << std::endl;
            }
        }
   }
 
   for (int k = 0; k <= mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j < mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                str << "3 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << std::endl;
            }
        }
   }
 
   for (int k = 0; k < mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j <= mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                str << "3 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << std::endl;
            }
        }
   }
 
   str << std::endl << "CELL_TYPES " << entitiesCount << std::endl;
   for (int i = 0; i < entitiesCount; i++)
   {
       str << "3" << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << entitiesCount << std::endl;
   str << "SCALARS edgesFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typedef typename MeshType::Face EntityType;
   typedef typename EntityType::EntityOrientationType EntityOrientation;
   EntityType entity( mesh );
 
   entity.setOrientation( EntityOrientation( 1.0, 0.0 , 0.0) );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() <= mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() <= mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() < mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   entity.setOrientation( EntityOrientation( 0.0, 1.0 , 0.0) );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() <= mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() < mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() <= mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   entity.setOrientation( EntityOrientation( 0.0, 0.0 , 1.0) );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() < mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() <= mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() <= mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   return true;
}

/****
 * 3D grid, vertices
 */

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
void
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 0, Real > >::
writeHeader( const MeshFunctionType& function,
       std::ostream& str )
{
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << std::endl;
    str << "TNL DATA" << std::endl;
    str << "ASCII" << std::endl;
    str << "DATASET UNSTRUCTURED_GRID" << std::endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
MeshFunctionVTKWriter< MeshFunction< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, 0, Real > >::
write( const MeshFunctionType& function,
       std::ostream& str )
{
   typedef typename MeshType::template MeshEntity< 0 > Vertex;
   writeHeader(function, str);
 
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
 
   str << "POINTS " << mesh.template getEntitiesCount< Vertex >() << " float" << std::endl;
 
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " <<
                        originZ + k * spaceStepZ << std::endl;
            }
       }
   }
 
   str << std::endl << "CELLS " << mesh.template getEntitiesCount< Vertex >() << " " <<
          mesh.template getEntitiesCount< Vertex >() * 2 << std::endl;
   for (int k = 0; k < ( mesh.getDimensions().z() + 1 ); k++)
   {
        for (int j = 0; j < ( mesh.getDimensions().y() + 1 ); j++)
        {
            for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
            {
                str << "1 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i  << std::endl;
            }
        }
   }
 
   str << std::endl << "CELL_TYPES " << mesh.template getEntitiesCount< Vertex >() << std::endl;
   for (int i = 0; i < mesh.template getEntitiesCount< Vertex >(); i++)
   {
       str << "1" << std::endl;
   }
 
   str << std::endl << "CELL_DATA " << mesh.template getEntitiesCount< Vertex >() << std::endl;
   str << "SCALARS verticesFunctionValues float 1" << std::endl;
   str << "LOOKUP_TABLE default" << std::endl;

   typename MeshType::Vertex entity( mesh );
   for( entity.getCoordinates().z() = 0;
        entity.getCoordinates().z() <= mesh.getDimensions().z();
        entity.getCoordinates().z() ++ )
   {
        for( entity.getCoordinates().y() = 0;
             entity.getCoordinates().y() <= mesh.getDimensions().y();
             entity.getCoordinates().y() ++ )
        {
            for( entity.getCoordinates().x() = 0;
                 entity.getCoordinates().x() <= mesh.getDimensions().x();
                 entity.getCoordinates().x() ++ )
            {
                 entity.refresh();
                 str << function.getData().getElement( entity.getIndex() ) << std::endl;
            }
        }
   }
 
   return true;
}

} // namespace Functions
} // namespace TNL
