/***************************************************************************
                          MeshEntity_impl.h  -  description
                             -------------------
    begin                : Sep 8, 2015
    copyright            : (C) 2015 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

/***
 * Authors:
 * Oberhuber Tomas, tomas.oberhuber@fjfi.cvut.cz
 * Zabka Vitezslav, zabkav@gmail.com
 */

#pragma once

#include <TNL/Meshes/MeshEntity.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
__cuda_callable__
MeshEntity< MeshConfig, Device, EntityTopology >::
MeshEntity( const MeshEntity& entity )
   : SubentityAccessLayerFamily< MeshConfig, Device, EntityTopology >( entity ),
     SuperentityAccessLayerFamily< MeshConfig, Device, EntityTopology >( entity ),
     MeshEntityIndex< typename MeshConfig::IdType >( entity )
{
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
   template< typename Device_ >
MeshEntity< MeshConfig, Device, EntityTopology >::
MeshEntity( const MeshEntity< MeshConfig, Device_, EntityTopology >& entity )
   // no cross-device copy of subentities and superentities here - Mesh constructor has to rebind pointers
   : MeshEntityIndex< typename MeshConfig::IdType >( entity )
{
   static_assert( ! std::is_same< Device, Device_ >::value, "this should never happen" );
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
__cuda_callable__
MeshEntity< MeshConfig, Device, EntityTopology >& 
MeshEntity< MeshConfig, Device, EntityTopology >::
operator=( const MeshEntity& entity )
{
   SubentityAccessLayerFamily< MeshConfig, Device, EntityTopology >::operator=( entity );
   SuperentityAccessLayerFamily< MeshConfig, Device, EntityTopology >::operator=( entity );
   MeshEntityIndex< typename MeshConfig::IdType >::operator=( entity );
   return *this;
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
   template< typename Device_ >
__cuda_callable__
MeshEntity< MeshConfig, Device, EntityTopology >& 
MeshEntity< MeshConfig, Device, EntityTopology >::
operator=( const MeshEntity< MeshConfig, Device_, EntityTopology >& entity )
{
   static_assert( ! std::is_same< Device, Device_ >::value, "this should never happen" );

   // no cross-device copy of subentities and superentities here - Mesh::operator= has to rebind pointers
   MeshEntityIndex< typename MeshConfig::IdType >::operator=( entity );
   return *this;
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
String
MeshEntity< MeshConfig, Device, EntityTopology >::
getSerializationType()
{
   return String( "MeshEntity<" ) +
          TNL::getSerializationType< MeshConfig >() + ", " +
          TNL::getSerializationType< EntityTopology >() + ">";
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
String
MeshEntity< MeshConfig, Device, EntityTopology >::
getSerializationTypeVirtual() const
{
   return this->getSerializationType();
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
void
MeshEntity< MeshConfig, Device, EntityTopology >::
save( File& file ) const
{
   // no I/O for subentities and superentities - not loaded anyway
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
void
MeshEntity< MeshConfig, Device, EntityTopology >::
load( File& file )
{
   // no I/O for subentities and superentities - Mesh::load has to rebind pointers
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
void
MeshEntity< MeshConfig, Device, EntityTopology >::
print( std::ostream& str ) const
{
   str << "\t Mesh entity dimension: " << EntityTopology::dimension << std::endl;
   SubentityAccessLayerFamily< MeshConfig, Device, EntityTopology >::print( str );
   SuperentityAccessLayerFamily< MeshConfig, Device, EntityTopology >::print( str );
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
__cuda_callable__
bool
MeshEntity< MeshConfig, Device, EntityTopology >::
operator==( const MeshEntity& entity ) const
{
   return ( SubentityAccessLayerFamily< MeshConfig, Device, EntityTopology >::operator==( entity ) &&
            SuperentityAccessLayerFamily< MeshConfig, Device, EntityTopology >::operator==( entity ) &&
            MeshEntityIndex< typename MeshConfig::IdType >::operator==( entity ) );
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
__cuda_callable__
bool
MeshEntity< MeshConfig, Device, EntityTopology >::
operator!=( const MeshEntity& entity ) const
{
   return ! ( *this == entity );
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
constexpr int
MeshEntity< MeshConfig, Device, EntityTopology >::
getEntityDimension()
{
   return EntityTopology::dimension;
}

/****
 * Subentities
 */
template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
constexpr typename MeshEntity< MeshConfig, Device, EntityTopology >::LocalIndexType
MeshEntity< MeshConfig, Device, EntityTopology >::
getVerticesCount()
{
   return SubentityTraits< 0 >::count;
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
typename MeshEntity< MeshConfig, Device, EntityTopology >::GlobalIndexType
MeshEntity< MeshConfig, Device, EntityTopology >::
getVertexIndex( const LocalIndexType localIndex ) const
{
   return this->template getSubentityIndex< 0 >( localIndex  );
}


/****
 * Vertex entity specialization
 */
template< typename MeshConfig, typename Device >
__cuda_callable__
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
MeshEntity( const MeshEntity& entity )
   : SuperentityAccessLayerFamily< MeshConfig, Device, Topologies::Vertex >( entity ),
     MeshEntityIndex< typename MeshConfig::IdType >( entity )
{
   setPoint( entity.getPoint() );
}

template< typename MeshConfig, typename Device >
   template< typename Device_ >
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
MeshEntity( const MeshEntity< MeshConfig, Device_, Topologies::Vertex >& entity )
   // no cross-device copy of superentities here - Mesh constructor has to rebind pointers
   : MeshEntityIndex< typename MeshConfig::IdType >( entity )
{
   static_assert( ! std::is_same< Device, Device_ >::value, "this should never happen" );

   setPoint( entity.getPoint() );
}

template< typename MeshConfig, typename Device >
__cuda_callable__
MeshEntity< MeshConfig, Device, Topologies::Vertex >& 
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
operator=( const MeshEntity& entity )
{
   SuperentityAccessLayerFamily< MeshConfig, Device, Topologies::Vertex >::operator=( entity );
   MeshEntityIndex< typename MeshConfig::IdType >::operator=( entity );
   setPoint( entity.getPoint() );
   return *this;
}

template< typename MeshConfig, typename Device >
   template< typename Device_ >
__cuda_callable__
MeshEntity< MeshConfig, Device, Topologies::Vertex >& 
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
operator=( const MeshEntity< MeshConfig, Device_, Topologies::Vertex >& entity )
{
   static_assert( ! std::is_same< Device, Device_ >::value, "this should never happen" );

   // no cross-device copy of superentities here - Mesh::operator= has to rebind pointers
   MeshEntityIndex< typename MeshConfig::IdType >::operator=( entity );
   setPoint( entity.getPoint() );
   return *this;
}

template< typename MeshConfig, typename Device >
String
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
getSerializationType()
{
   return String( "MeshEntity<" ) +
          TNL::getSerializationType< MeshConfig >() + ", " +
          TNL::getSerializationType< Topologies::Vertex >() + ">";
}

template< typename MeshConfig, typename Device >
String
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
getSerializationTypeVirtual() const
{
   return this->getSerializationType();
}

template< typename MeshConfig, typename Device >
void
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
save( File& file ) const
{
   // no I/O for superentities - not loaded anyway
   point.save( file );
}

template< typename MeshConfig, typename Device >
void
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
load( File& file )
{
   // no I/O for superentities - Mesh::load has to rebind pointers
   point.load( file );
}

template< typename MeshConfig, typename Device >
void
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
print( std::ostream& str ) const
{
   str << "\t Mesh entity dimension: " << Topologies::Vertex::dimension << std::endl;
   str << "\t Coordinates = " << point << std::endl;
   SuperentityAccessLayerFamily< MeshConfig, Device, Topologies::Vertex >::print( str );
}

template< typename MeshConfig, typename Device >
__cuda_callable__
bool
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
operator==( const MeshEntity& entity ) const
{
   return ( SuperentityAccessLayerFamily< MeshConfig, Device, Topologies::Vertex >::operator==( entity ) &&
            MeshEntityIndex< typename MeshConfig::IdType >::operator==( entity ) &&
            point == entity.point );
}

template< typename MeshConfig, typename Device >
__cuda_callable__
bool
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
operator!=( const MeshEntity& entity ) const
{
   return ! ( *this == entity );
}

template< typename MeshConfig, typename Device >
constexpr int
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
getEntityDimension()
{
   return EntityTopology::dimension;
}

template< typename MeshConfig, typename Device >
__cuda_callable__
typename MeshEntity< MeshConfig, Device, Topologies::Vertex >::PointType
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
getPoint() const
{
   return this->point;
}

template< typename MeshConfig, typename Device >
__cuda_callable__
void
MeshEntity< MeshConfig, Device, Topologies::Vertex >::
setPoint( const PointType& point )
{
   this->point = point;
}

template< typename MeshConfig,
          typename Device,
          typename EntityTopology >
std::ostream& operator<<( std::ostream& str, const MeshEntity< MeshConfig, Device, EntityTopology >& entity )
{
   entity.print( str );
   return str;
}

} // namespace Meshes
} // namespace TNL
