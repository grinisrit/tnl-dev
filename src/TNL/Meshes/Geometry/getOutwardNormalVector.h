/***************************************************************************
                          getOutwardNormalVector.h  -  description
                             -------------------
    begin                : Feb 14, 2020
    copyright            : (C) 2020 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Meshes/Geometry/getEntityCenter.h>
#include <TNL/Meshes/Topologies/Edge.h>

namespace TNL {
namespace Meshes {

template< typename Grid, typename Config >
__cuda_callable__
typename Grid::PointType
getOutwardNormalVector( const Grid & grid,
                        const GridEntity< Grid, 0, Config > & face,
                        const typename Grid::PointType cellCenter )
{
   static_assert( Grid::getMeshDimension() == 1, "getOutwardNormalVector can be used only with faces." );
   const typename Grid::PointType faceCenter = getEntityCenter( grid, face );
   if( faceCenter.x() > cellCenter.x() )
      return {1};
   else
      return {-1};
}

template< typename Grid, typename Config >
__cuda_callable__
typename Grid::PointType
getOutwardNormalVector( const Grid & grid,
                        const GridEntity< Grid, 1, Config > & face,
                        const typename Grid::PointType cellCenter )
{
   static_assert( Grid::getMeshDimension() == 2, "getOutwardNormalVector can be used only with faces." );
   const typename Grid::PointType faceCenter = getEntityCenter( grid, face );
   if( face.getOrientation().x() != 0 ) {
      // x-normal face
      if( faceCenter.x() > cellCenter.x() )
         return {1, 0};
      else
         return {-1, 0};
   }
   else {
      // y-normal face
      if( faceCenter.y() > cellCenter.y() )
         return {0, 1};
      else
         return {0, -1};
   }
}

template< typename Grid, typename Config >
__cuda_callable__
typename Grid::PointType
getOutwardNormalVector( const Grid & grid,
                        const GridEntity< Grid, 2, Config > & face,
                        const typename Grid::PointType cellCenter )
{
   static_assert( Grid::getMeshDimension() == 3, "getOutwardNormalVector can be used only with faces." );
   const typename Grid::PointType faceCenter = getEntityCenter( grid, face );
   if( face.getOrientation().x() != 0 ) {
      // x-normal face
      if( faceCenter.x() > cellCenter.x() )
         return {1, 0, 0};
      else
         return {-1, 0, 0};
   }
   else if( face.getOrientation().y() != 0 ) {
      // y-normal face
      if( faceCenter.y() > cellCenter.y() )
         return {0, 1, 0};
      else
         return {0, -1, 0};
   }
   else  {
      // z-normal face
      if( faceCenter.z() > cellCenter.z() )
         return {0, 0, 1};
      else
         return {0, 0, -1};
   }
}

template< typename MeshConfig, typename Device >
__cuda_callable__
typename MeshTraits< MeshConfig >::PointType
getOutwardNormalVector( const Mesh< MeshConfig, Device > & mesh,
                        const MeshEntity< MeshConfig, Device, Topologies::Edge > & face,
                        typename MeshTraits< MeshConfig >::PointType cellCenter )
{
   using MeshType = Mesh< MeshConfig, Device >;
   using FaceType = MeshEntity< MeshConfig, Device, Topologies::Edge >;
   using PointType = typename MeshTraits< MeshConfig >::PointType;
   static_assert( std::is_same< typename MeshType::Face, FaceType >::value, "getOutwardNormalVector called for an entity which is not a face" );
   static_assert( MeshConfig::worldDimension == 2, "TODO: normal vectors for 2D meshes in a 3D space are not implemented yet" );

   const auto& v0 = mesh.getPoint( face.template getSubentityIndex< 0 >( 0 ) );
   const auto& v1 = mesh.getPoint( face.template getSubentityIndex< 0 >( 1 ) );
   const PointType u = v0 - v1;
   const PointType n {u[1], -u[0]};

   // check on which side of the face is the reference cell center
   const PointType faceCenter = getEntityCenter( mesh, face );
   if( dot( n, cellCenter - faceCenter ) < 0 )
      return n / l2Norm( n );
   else
      return - n / l2Norm( n );
}

template< typename MeshConfig, typename Device, typename EntityTopology >
__cuda_callable__
typename MeshTraits< MeshConfig >::PointType
getOutwardNormalVector( const Mesh< MeshConfig, Device > & mesh,
                        const MeshEntity< MeshConfig, Device, EntityTopology > & face,
                        typename MeshTraits< MeshConfig >::PointType cellCenter )
{
   using MeshType = Mesh< MeshConfig, Device >;
   using FaceType = MeshEntity< MeshConfig, Device, EntityTopology >;
   using PointType = typename MeshTraits< MeshConfig >::PointType;
   static_assert( std::is_same< typename MeshType::Face, FaceType >::value, "getOutwardNormalVector called for an entity which is not a face" );
   static_assert( MeshConfig::worldDimension == 3, "general overload intended for 3D was called with the wrong world dimension" );

   const auto& v0 = mesh.getPoint( face.template getSubentityIndex< 0 >( 0 ) );
   const auto& v1 = mesh.getPoint( face.template getSubentityIndex< 0 >( 1 ) );
   const auto& v2 = mesh.getPoint( face.template getSubentityIndex< 0 >( 2 ) );
   const PointType u1 = v0 - v1;
   const PointType u2 = v0 - v2;
   const PointType n {
      u1.y() * u2.z() - u1.z() * u2.y(),   // first component of the cross product
      u1.z() * u2.x() - u1.x() * u2.z(),   // second component of the cross product
      u1.x() * u2.y() - u1.y() * u2.x()    // third component of the cross product
   };

   // check on which side of the face is the reference cell center
   const PointType faceCenter = getEntityCenter( mesh, face );
   if( dot( n, cellCenter - faceCenter ) < 0 )
      return n / l2Norm( n );
   else
      return - n / l2Norm( n );
}

} // namespace Meshes
} // namespace TNL
