/***************************************************************************
                          tnlMeshFunctionVTKWriter_impl.h  -  description
                             -------------------
    begin                : Jan 28, 2016
    copyright            : (C) 2016 by oberhuber
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

#ifndef TNLMESHFUNCTIONVTKWRITER_IMPL_H
#define	TNLMESHFUNCTIONVTKWRITER_IMPL_H

template< typename MeshFunction >
bool
tnlMeshFunctionVTKWriter< MeshFunction >::
write( const MeshFunction& function,
                         ostream& str )
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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 1, MeshReal, Device, MeshIndex >, 1, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 1, MeshReal, Device, MeshIndex >, 1, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{ 
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType origin = mesh.getOrigin().x();
   const RealType spaceStep = mesh.getSpaceSteps().x();
   
   str << "POINTS " << mesh.getDimensions().x() + 1 << " float" << endl;
   
   for (int i = 0; i <= mesh.getDimensions().x(); i++)
   {
       str << origin + i * spaceStep << " 0 0" << endl;
   }
   
   str << endl << "CELLS " << mesh.getDimensions().x() << " " << mesh.getDimensions().x() * 3 << endl;
   for (int i = 0; i < mesh.getDimensions().x(); i++)
   {
       str << "2 " << i << " " << i+1 << endl;
   }
   
   str << endl << "CELL_TYPES " << mesh.getDimensions().x() << endl;
   for (int i = 0; i < mesh.getDimensions().x(); i++)
   {
       str << "3 " << endl;
   }   
   
   str << endl << "CELL_DATA " << mesh.getDimensions().x() << endl;
   str << "SCALARS cellFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 1, MeshReal, Device, MeshIndex >, 0, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 1, MeshReal, Device, MeshIndex >, 0, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{ 
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType origin = mesh.getOrigin().x();
   const RealType spaceStep = mesh.getSpaceSteps().x();
   
   str << "POINTS " << mesh.getDimensions().x() + 1 << " float" << endl;
   
   for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
   {
       str << origin + i * spaceStep << " 0 0" << endl;
   }
   
   str << endl << "CELLS " << mesh.getDimensions().x() + 1 << " " << ( mesh.getDimensions().x() + 1 ) * 2 << endl;
   for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
   {
       str << "1 " << i << endl;
   }
   
   str << endl << "CELL_TYPES " << mesh.getDimensions().x() + 1 << endl;
   for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
   {
       str << "1 " << endl;
   }   
   
   str << endl << "CELL_DATA " << mesh.getDimensions().x() + 1 << endl;
   str << "SCALARS VerticesFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 2, MeshReal, Device, MeshIndex >, 2, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 2, MeshReal, Device, MeshIndex >, 2, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{  
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   
   str << "POINTS " << (mesh.getDimensions().x() + 1) * (mesh.getDimensions().y() + 1) << " float" << endl;
   
   for (int j = 0; j < mesh.getDimensions().y() + 1; j++)
   {
        for (int i = 0; i < mesh.getDimensions().x() + 1; i++)
        {
             str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " 0" << endl;
        }
   }
   
   str << endl << "CELLS " << mesh.getDimensions().x() * mesh.getDimensions().y() << " " << 
          mesh.getDimensions().x() * mesh.getDimensions().y() * 5 << endl;
   for (int j = 0; j < mesh.getDimensions().y(); j++)
   {
        for (int i = 0; i < mesh.getDimensions().x(); i++)
        {
            str << "4 " << j * ( mesh.getDimensions().x() + 1 ) + i << " " << j * ( mesh.getDimensions().x() + 1 )+ i + 1 << 
                   " " << (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " " << (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1 << endl;
        }
   }
   
   str << endl << "CELL_TYPES " << mesh.getDimensions().x() * mesh.getDimensions().y() << endl;
   for (int i = 0; i < mesh.getDimensions().x()*mesh.getDimensions().y(); i++)
   {
       str << "8 " << endl;
   }   
   
   str << endl << "CELL_DATA " << mesh.getDimensions().x() * mesh.getDimensions().y() << endl;
   str << "SCALARS cellFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
         str << function.getData().getElement( entity.getIndex() ) << endl;      
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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 2, MeshReal, Device, MeshIndex >, 1, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 2, MeshReal, Device, MeshIndex >, 1, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{ 
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   
   str << "POINTS " << mesh.template getEntitiesCount<0>() << " float" << endl;
   
   for (int j = 0; j < ( mesh.getDimensions().y() + 1); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
             str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " 0" << endl;
        }
   }
   
   str << endl << "CELLS " << mesh.template getEntitiesCount<1>() << " " << 
          mesh.template getEntitiesCount<1>() * 3 << endl;
   for (int j = 0; j < mesh.getDimensions().y(); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
            str << "2 " << j * ( mesh.getDimensions().x() + 1 ) + i << " " << (j+1) * ( mesh.getDimensions().x() + 1 ) + i << endl;
        }
   }
   
   for (int j = 0; j < (mesh.getDimensions().y()+1); j++)
   {
        for (int i = 0; i < mesh.getDimensions().x(); i++)
        {
            str << "2 " << j * ( mesh.getDimensions().x() + 1 ) + i << " " <<j * ( mesh.getDimensions().x() + 1 ) + i + 1<< endl;
        }
   }
   
   str << endl << "CELL_TYPES " << mesh.template getEntitiesCount<1>() << endl;
   for (int i = 0; i < mesh.template getEntitiesCount<1>(); i++)
   {
       str << "3" << endl;
   }  
   
   str << endl << "CELL_DATA " << mesh.template getEntitiesCount<1>() << endl;
   str << "SCALARS FaceslFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 2, MeshReal, Device, MeshIndex >, 0, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 2, MeshReal, Device, MeshIndex >, 0, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{  
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   
   str << "POINTS " << mesh.template getEntitiesCount<0>() << " float" << endl;
   
   for (int j = 0; j < ( mesh.getDimensions().y() + 1); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
             str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " 0" << endl;
        }
   }
   
   str << endl << "CELLS " << mesh.template getEntitiesCount<0>() << " " << 
          mesh.template getEntitiesCount<0>() * 2 << endl;
   for (int j = 0; j < ( mesh.getDimensions().y() + 1 ); j++)
   {
        for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
        {
            str << "1 " << j * mesh.getDimensions().x() + i  << endl;
        }
   }
   
   str << endl << "CELL_TYPES " << mesh.template getEntitiesCount<0>() << endl;
   for (int i = 0; i < mesh.template getEntitiesCount<0>(); i++)
   {
       str << "1" << endl;
   }  
   
   str << endl << "CELL_DATA " << mesh.template getEntitiesCount<0>() << endl;
   str << "SCALARS VerticesFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
         str << function.getData().getElement( entity.getIndex() ) << endl;      
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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 3, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 3, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
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
          " float" << endl;
   
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " << 
                        originZ + k * spaceStepZ << endl;
            }
       }
   }
   
   str << endl << "CELLS " << entitiesCount << " " << 
          entitiesCount * 9 << endl;
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
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1 << endl;
            }
        }
   }
   
   str << endl << "CELL_TYPES " << entitiesCount << endl;
   for (int i = 0; i < entitiesCount; i++)
   {
       str << "11" << endl;
   }   
   
   str << endl << "CELL_DATA " << entitiesCount << endl;
   str << "SCALARS cellFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
                str << function.getData().getElement( entity.getIndex() ) << endl;      
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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 2, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 2, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{  
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
   const RealType entitiesCount = mesh.template getEntitiesCount<2>();
   const RealType pointsCount = mesh.template getEntitiesCount<3>();
   
   str << "POINTS " << pointsCount << 
          " float" << endl;
   
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " << 
                        originZ + k * spaceStepZ << endl;
            }
       }
   }
   
   str << endl << "CELLS " << entitiesCount << " " << 
          entitiesCount * 5 << endl;
   for (int k = 0; k < mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j < mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                str << "4 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << endl;
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
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << endl;
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
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i + 1<< endl;
            }
        }
   }
   
   str << endl << "CELL_TYPES " << entitiesCount << endl;
   for (int i = 0; i < entitiesCount; i++)
   {
       str << "8" << endl;
   }   
   
   str << endl << "CELL_DATA " << entitiesCount << endl;
   str << "SCALARS facesFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 1, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 1, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{  
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
   const RealType entitiesCount = mesh.template getEntitiesCount<1>();
   const RealType pointsCount = mesh.template getEntitiesCount<3>();
   
   str << "POINTS " << pointsCount << 
          " float" << endl;
   
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " << 
                        originZ + k * spaceStepZ << endl;
            }
       }
   }
   
   str << endl << "CELLS " << entitiesCount << " " << 
          entitiesCount * 3 << endl;
   for (int k = 0; k <= mesh.getDimensions().z(); k++)
   {
        for (int j = 0; j <= mesh.getDimensions().y(); j++)
        {
            for (int i = 0; i < mesh.getDimensions().x(); i++)
            {
                str << "3 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << " "
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i + 1 << endl;
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
                    << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + (j+1) * ( mesh.getDimensions().x() + 1 ) + i << endl;
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
                    << (k+1) * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i << endl;
            }
        }
   }
   
   str << endl << "CELL_TYPES " << entitiesCount << endl;
   for (int i = 0; i < entitiesCount; i++)
   {
       str << "3" << endl;
   }   
   
   str << endl << "CELL_DATA " << entitiesCount << endl;
   str << "SCALARS edgesFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 0, Real > >::
writeHeader( const MeshFunctionType& function,
       ostream& str )
{ 
    const MeshType& mesh = function.getMesh();
    const typename MeshType::VertexType& origin = mesh.getOrigin();
    const typename MeshType::VertexType& proportions = mesh.getProportions();
    str << "# vtk DataFile Version 2.0" << endl;
    str << "TNL DATA" << endl;
    str << "ASCII" << endl;
    str << "DATASET UNSTRUCTURED_GRID" << endl;
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real >
bool
tnlMeshFunctionVTKWriter< tnlMeshFunction< tnlGrid< 3, MeshReal, Device, MeshIndex >, 0, Real > >::
write( const MeshFunctionType& function,
       ostream& str )
{  
   writeHeader(function, str);
   
   const MeshType& mesh = function.getMesh();
   const RealType originX = mesh.getOrigin().x();
   const RealType spaceStepX = mesh.getSpaceSteps().x();
   const RealType originY = mesh.getOrigin().y();
   const RealType spaceStepY = mesh.getSpaceSteps().y();
   const RealType originZ = mesh.getOrigin().z();
   const RealType spaceStepZ = mesh.getSpaceSteps().z();
   
   str << "POINTS " << mesh.template getEntitiesCount<0>() << " float" << endl;
   
   for (int k = 0; k <= mesh.getDimensions().y(); k++)
   {
       for (int j = 0; j <= mesh.getDimensions().y(); j++)
       {
            for (int i = 0; i <= mesh.getDimensions().x(); i++)
            {
                 str << originX + i * spaceStepX << " " << originY + j * spaceStepY << " " << 
                        originZ + k * spaceStepZ << endl;
            }
       }
   }
   
   str << endl << "CELLS " << mesh.template getEntitiesCount<0>() << " " << 
          mesh.template getEntitiesCount<0>() * 2 << endl;
   for (int k = 0; k < ( mesh.getDimensions().z() + 1 ); k++)
   {
        for (int j = 0; j < ( mesh.getDimensions().y() + 1 ); j++)
        {
            for (int i = 0; i < ( mesh.getDimensions().x() + 1 ); i++)
            {
                str << "1 " << k * ( mesh.getDimensions().y() + 1 ) * ( mesh.getDimensions().x() + 1 ) + j * ( mesh.getDimensions().x() + 1 ) + i  << endl;
            }
        }
   }
   
   str << endl << "CELL_TYPES " << mesh.template getEntitiesCount<0>() << endl;
   for (int i = 0; i < mesh.template getEntitiesCount<0>(); i++)
   {
       str << "1" << endl;
   }  
   
   str << endl << "CELL_DATA " << mesh.template getEntitiesCount<0>() << endl;
   str << "SCALARS verticesFunctionValues float 1" << endl;
   str << "LOOKUP_TABLE default" << endl;

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

#endif	/* TNLMESHFUNCTIONVTKWRITER_IMPL_H */

