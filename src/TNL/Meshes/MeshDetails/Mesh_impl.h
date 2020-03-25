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
#include <TNL/Meshes/MeshDetails/IndexPermutationApplier.h>
#include <TNL/Meshes/MeshDetails/initializer/Initializer.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig, typename Device, typename MeshType >
void
MeshInitializableBase< MeshConfig, Device, MeshType >::
init( typename MeshTraitsType::PointArrayType& points,
      typename MeshTraitsType::CellSeedArrayType& cellSeeds )
{
   Initializer< typename MeshType::Config > initializer;
   initializer.createMesh( points, cellSeeds, *static_cast<MeshType*>(this) );
   // init boundary tags
   static_cast< BoundaryTags::LayerFamily< MeshConfig, Device, MeshType >* >( static_cast< MeshType* >( this ) )->initLayer();
}


template< typename MeshConfig, typename Device >
Mesh< MeshConfig, Device >::
Mesh( const Mesh& mesh )
   : StorageBaseType( mesh ),
     BoundaryTagsLayerFamily( mesh )
{
}

template< typename MeshConfig, typename Device >
   template< typename Device_ >
Mesh< MeshConfig, Device >::
Mesh( const Mesh< MeshConfig, Device_ >& mesh )
   : StorageBaseType( mesh ),
     BoundaryTagsLayerFamily( mesh )
{
}

template< typename MeshConfig, typename Device >
Mesh< MeshConfig, Device >&
Mesh< MeshConfig, Device >::
operator=( const Mesh& mesh )
{
   StorageBaseType::operator=( mesh );
   BoundaryTagsLayerFamily::operator=( mesh );
   return *this;
}

template< typename MeshConfig, typename Device >
   template< typename Device_ >
Mesh< MeshConfig, Device >&
Mesh< MeshConfig, Device >::
operator=( const Mesh< MeshConfig, Device_ >& mesh )
{
   StorageBaseType::operator=( mesh );
   BoundaryTagsLayerFamily::operator=( mesh );
   return *this;
}

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
getSerializationType()
{
   return String( "Meshes::Mesh< ") + TNL::getType< MeshConfig >() + " >";
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
   return EntityTraits< Dimension >::storageEnabled;
}

template< typename MeshConfig, typename Device >
   template< int Dimension >
__cuda_callable__
typename Mesh< MeshConfig, Device >::GlobalIndexType
Mesh< MeshConfig, Device >::
getEntitiesCount() const
{
   static_assert( entitiesAvailable< Dimension >(), "You try to get number of entities which are not configured for storage." );
   return StorageBaseType::getEntitiesCount( DimensionTag< Dimension >() );
}

template< typename MeshConfig, typename Device >
   template< int Dimension >
__cuda_callable__
typename Mesh< MeshConfig, Device >::template EntityType< Dimension >
Mesh< MeshConfig, Device >::
getEntity( const GlobalIndexType entityIndex ) const
{
   static_assert( entitiesAvailable< Dimension >(), "You try to get entity which is not configured for storage." );
   TNL_ASSERT_LT( entityIndex, getEntitiesCount< Dimension >(), "invalid entity index" );
   return EntityType< Dimension >( *this, entityIndex );
}


// duplicated for compatibility with grids
template< typename MeshConfig, typename Device >
   template< typename Entity >
__cuda_callable__
typename Mesh< MeshConfig, Device >::GlobalIndexType
Mesh< MeshConfig, Device >::
getEntitiesCount() const
{
   return getEntitiesCount< Entity::getEntityDimension() >();
}

template< typename MeshConfig, typename Device >
   template< typename Entity >
__cuda_callable__
Entity
Mesh< MeshConfig, Device >::
getEntity( const GlobalIndexType entityIndex ) const
{
   return getEntity< Entity::getEntityDimension() >( entityIndex );
}


template< typename MeshConfig, typename Device >
__cuda_callable__
const typename Mesh< MeshConfig, Device >::PointType&
Mesh< MeshConfig, Device >::
getPoint( const GlobalIndexType vertexIndex ) const
{
   TNL_ASSERT_GE( vertexIndex, 0, "invalid vertex index" );
   TNL_ASSERT_LT( vertexIndex, getEntitiesCount< 0 >(), "invalid vertex index" );
   return this->points[ vertexIndex ];
}

template< typename MeshConfig, typename Device >
__cuda_callable__
typename Mesh< MeshConfig, Device >::PointType&
Mesh< MeshConfig, Device >::
getPoint( const GlobalIndexType vertexIndex )
{
   TNL_ASSERT_GE( vertexIndex, 0, "invalid vertex index" );
   TNL_ASSERT_LT( vertexIndex, getEntitiesCount< 0 >(), "invalid vertex index" );
   return this->points[ vertexIndex ];
}


template< typename MeshConfig, typename Device >
   template< int EntityDimension, int SubentityDimension >
__cuda_callable__
constexpr typename Mesh< MeshConfig, Device >::LocalIndexType
Mesh< MeshConfig, Device >::
getSubentitiesCount( const GlobalIndexType entityIndex ) const
{
   return this->template getSubentityStorageNetwork< EntityDimension, SubentityDimension >().getValuesCount( entityIndex );
}

template< typename MeshConfig, typename Device >
   template< int EntityDimension, int SubentityDimension >
__cuda_callable__
typename Mesh< MeshConfig, Device >::GlobalIndexType
Mesh< MeshConfig, Device >::
getSubentityIndex( const GlobalIndexType entityIndex, const LocalIndexType subentityIndex ) const
{
   return this->template getSubentityStorageNetwork< EntityDimension, SubentityDimension >().getValue( entityIndex, subentityIndex );
}

template< typename MeshConfig, typename Device >
   template< int EntityDimension, int SuperentityDimension >
__cuda_callable__
typename Mesh< MeshConfig, Device >::LocalIndexType
Mesh< MeshConfig, Device >::
getSuperentitiesCount( const GlobalIndexType entityIndex ) const
{
   return this->template getSuperentityStorageNetwork< EntityDimension, SuperentityDimension >().getValuesCount( entityIndex );
}

template< typename MeshConfig, typename Device >
   template< int EntityDimension, int SuperentityDimension >
__cuda_callable__
typename Mesh< MeshConfig, Device >::GlobalIndexType
Mesh< MeshConfig, Device >::
getSuperentityIndex( const GlobalIndexType entityIndex, const LocalIndexType superentityIndex ) const
{
   return this->template getSuperentityStorageNetwork< EntityDimension, SuperentityDimension >().getValue( entityIndex, superentityIndex );
}


template< typename MeshConfig, typename Device >
   template< int Dimension >
void
Mesh< MeshConfig, Device >::
reorderEntities( const GlobalIndexVector& perm,
                 const GlobalIndexVector& iperm )
{
   static_assert( entitiesAvailable< Dimension >(), "Entities which are not stored cannot be reordered." );

   const GlobalIndexType entitiesCount = getEntitiesCount< Dimension >();

   // basic sanity check
   if( perm.getSize() != entitiesCount || iperm.getSize() != entitiesCount ) {
      throw std::logic_error( "Wrong size of permutation vectors: "
                              "perm size = " + std::to_string( perm.getSize() ) + ", "
                              "iperm size = " + std::to_string( iperm.getSize() ) );
   }
   TNL_ASSERT( min( perm ) == 0 && max( perm ) == entitiesCount - 1,
               std::cerr << "Given array is not a permutation: min = " << min( perm )
                         << ", max = " << max( perm )
                         << ", number of entities = " << entitiesCount
                         << ", array = " << perm << std::endl; );
   TNL_ASSERT( min( iperm ) == 0 && max( iperm ) == entitiesCount - 1,
               std::cerr << "Given array is not a permutation: min = " << min( iperm )
                         << ", max = " << max( iperm )
                         << ", number of entities = " << entitiesCount
                         << ", array = " << iperm << std::endl; );

   IndexPermutationApplier< Mesh, Dimension >::exec( *this, perm, iperm );
   // update boundary tags
   static_cast< BoundaryTagsLayerFamily* >( this )->initLayer();
}


template< typename MeshConfig, typename Device >
void
Mesh< MeshConfig, Device >::
save( File& file ) const
{
   Object::save( file );
   StorageBaseType::save( file );
   BoundaryTagsLayerFamily::save( file );
}

template< typename MeshConfig, typename Device >
void
Mesh< MeshConfig, Device >::
load( File& file )
{
   Object::load( file );
   StorageBaseType::load( file );
   BoundaryTagsLayerFamily::load( file );
}

template< typename MeshConfig, typename Device >
void
Mesh< MeshConfig, Device >::
print( std::ostream& str ) const
{
   // FIXME: there is a problem with multimaps and accessors holding pointers into the device memory
   if( std::is_same< Device, Devices::Cuda >::value ) {
      str << "Textual representation of meshes stored on the CUDA device is not supported." << std::endl;
   }
   else {
      StorageBaseType::print( str );
      BoundaryTagsLayerFamily::print( str );
   }
}

template< typename MeshConfig, typename Device >
bool
Mesh< MeshConfig, Device >::
operator==( const Mesh& mesh ) const
{
   return StorageBaseType::operator==( mesh ) &&
          BoundaryTagsLayerFamily::operator==( mesh );
}

template< typename MeshConfig, typename Device >
bool
Mesh< MeshConfig, Device >::
operator!=( const Mesh& mesh ) const
{
   return ! operator==( mesh );
}

template< typename MeshConfig, typename Device >
void
Mesh< MeshConfig, Device >::
writeProlog( Logger& logger ) const
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
