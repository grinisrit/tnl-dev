/***************************************************************************
                          Mesh_impl.h  -  description
                             -------------------
    begin                : Sep 5, 2015
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

#include <TNL/Meshes/Mesh.h>
#include <TNL/Meshes/MeshDetails/initializer/MeshInitializer.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig, typename Device >
constexpr int
Mesh< MeshConfig, Device >::
getMeshDimension()
{
   return MeshTraitsType::meshDimension;
}

template< typename MeshConfig, typename Device >
String
Mesh< MeshConfig, Device >::
getType()
{
   return String( "Meshes::Mesh< ") + MeshConfig::getType() + " >";
}

template< typename MeshConfig, typename Device >
String
Mesh< MeshConfig, Device >::
getTypeVirtual() const
{
   return this->getType();
}

template< typename MeshConfig, typename Device >
String
Mesh< MeshConfig, Device >::
getSerializationType()
{
   return Mesh::getType();
}

template< typename MeshConfig, typename Device >
String
Mesh< MeshConfig, Device >::
getSerializationTypeVirtual() const
{
   return this->getSerializationType();
}

template< typename MeshConfig, typename Device >
   template< int Dimension >
constexpr bool
Mesh< MeshConfig, Device >::
entitiesAvailable()
{
   return MeshTraitsType::template EntityTraits< Dimension >::storageEnabled;
}

template< typename MeshConfig, typename Device >
   template< int Dimension >
typename Mesh< MeshConfig, Device >::GlobalIndexType
Mesh< MeshConfig, Device >::
getEntitiesCount() const
{
   static_assert( EntityTraits< Dimension >::storageEnabled, "You try to get number of entities which are not configured for storage." );
   return StorageBaseType::getEntitiesCount( DimensionTag< Dimension >() );
}

template< typename MeshConfig, typename Device >
   template< int Dimension >
typename Mesh< MeshConfig, Device >::template EntityType< Dimension >&
Mesh< MeshConfig, Device >::
getEntity( const GlobalIndexType& entityIndex )
{
   static_assert( EntityTraits< Dimension >::storageEnabled, "You try to get entity which is not configured for storage." );
   return StorageBaseType::getEntity( DimensionTag< Dimension >(), entityIndex );
}

template< typename MeshConfig, typename Device >
   template< int Dimension >
const typename Mesh< MeshConfig, Device >::template EntityType< Dimension >&
Mesh< MeshConfig, Device >::
getEntity( const GlobalIndexType& entityIndex ) const
{
   static_assert( EntityTraits< Dimension >::storageEnabled, "You try to get entity which is not configured for storage." );
   return StorageBaseType::getEntity( DimensionTag< Dimension >(), entityIndex );
}


// duplicated for compatibility with grids
template< typename MeshConfig, typename Device >
   template< typename Entity >
typename Mesh< MeshConfig, Device >::GlobalIndexType
Mesh< MeshConfig, Device >::
getEntitiesCount() const
{
   return getEntitiesCount< Entity::getEntityDimension() >();
}

template< typename MeshConfig, typename Device >
   template< typename Entity >
Entity&
Mesh< MeshConfig, Device >::
getEntity( const GlobalIndexType& entityIndex )
{
   return getEntity< Entity::getEntityDimension() >( entityIndex );
}

template< typename MeshConfig, typename Device >
   template< typename Entity >
const Entity&
Mesh< MeshConfig, Device >::
getEntity( const GlobalIndexType& entityIndex ) const
{
   return getEntity< Entity::getEntityDimension() >( entityIndex );
}


template< typename MeshConfig, typename Device >
bool
Mesh< MeshConfig, Device >::
save( File& file ) const
{
   if( ! Object::save( file ) ||
       ! StorageBaseType::save( file ) )
   {
      std::cerr << "Mesh saving failed." << std::endl;
      return false;
   }
   return true;
}

template< typename MeshConfig, typename Device >
bool
Mesh< MeshConfig, Device >::
load( File& file )
{
   if( ! Object::load( file ) ||
       ! StorageBaseType::load( file ) )
   {
      std::cerr << "Mesh loading failed." << std::endl;
      return false;
   }
   // TODO: this could be done from the storage layer
   // update pointers from entities into the subentity and superentity storage networks
   MeshEntityStorageRebinder< Mesh< MeshConfig > >::exec( *this );
   return true;
}

template< typename MeshConfig, typename Device >
void
Mesh< MeshConfig, Device >::
print( std::ostream& str ) const
{
   StorageBaseType::print( str );
}

template< typename MeshConfig, typename Device >
bool
Mesh< MeshConfig, Device >::
operator==( const Mesh& mesh ) const
{
   return StorageBaseType::operator==( mesh );
}

template< typename MeshConfig, typename Device >
bool
Mesh< MeshConfig, Device >::
init( typename MeshTraitsType::PointArrayType& points,
      typename MeshTraitsType::CellSeedArrayType& cellSeeds )
{
   MeshInitializer< MeshConfig> meshInitializer;
   if( ! meshInitializer.createMesh( points, cellSeeds, *this ) )
      return false;
   return true;
}

template< typename MeshConfig, typename Device >
void
Mesh< MeshConfig, Device >::
writeProlog( Logger& logger )
{
   logger.writeParameter( "Dimension:", getMeshDimension() );
   logger.writeParameter( "Number of cells:", getEntitiesCount< getMeshDimension() >() );
   if( getMeshDimension() > 1 )
      logger.writeParameter( "Number of faces:", getEntitiesCount< getMeshDimension() - 1 >() );
   logger.writeParameter( "Number of vertices:", getEntitiesCount< 0 >() );
   // TODO: more parameters?
}


template< typename MeshConfig, typename Device >
std::ostream& operator<<( std::ostream& str, const Mesh< MeshConfig, Device >& mesh )
{
   mesh.print( str );
   return str;
}

} // namespace Meshes
} // namespace TNL

