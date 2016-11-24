/***************************************************************************
                          MeshSubentityStorageLayer.h  -  description
                             -------------------
    begin                : Feb 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

/***
 * Authors:
 * Oberhuber Tomas, tomas.oberhuber@fjfi.cvut.cz
 * Zabka Vitezslav, zabkav@gmail.com
 */

#pragma once

#include <TNL/File.h>
#include <TNL/Meshes/DimensionTag.h>
#include <TNL/Meshes/MeshDetails/traits/MeshTraits.h>
#include <TNL/Meshes/MeshDetails/traits/MeshSubentityTraits.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig,
          typename EntityTopology,
          typename SubdimensionTag,
          bool SubentityStorage =
               MeshConfig::subentityStorage( EntityTopology(), SubdimensionTag::value ) >
class MeshSubentityStorageLayer;

template< typename MeshConfig,
          typename EntityTopology >
class MeshSubentityStorageLayers
   : public MeshSubentityStorageLayer< MeshConfig,
                                       EntityTopology,
                                       DimensionTag< 0 > >
{
   using BaseType = MeshSubentityStorageLayer< MeshConfig,
                                               EntityTopology,
                                               DimensionTag< 0 > >;
   using MeshTraitsType = MeshTraits< MeshConfig >;

public:
   template< int Subdimension >
   typename MeshTraitsType::template SubentityTraits< EntityTopology, Subdimension >::StorageNetworkType&
   getSubentityStorageNetwork()
   {
      static_assert( EntityTopology::dimension > Subdimension, "Invalid combination of Dimension and Subdimension." );
      return BaseType::getSubentityStorageNetwork( DimensionTag< Subdimension >() );
   }
};

template< typename MeshConfig,
          typename EntityTopology,
          typename SubdimensionTag >
class MeshSubentityStorageLayer< MeshConfig,
                                 EntityTopology,
                                 SubdimensionTag,
                                 true >
   : public MeshSubentityStorageLayer< MeshConfig,
                                       EntityTopology,
                                       typename SubdimensionTag::Increment >
{
   using BaseType = MeshSubentityStorageLayer< MeshConfig,
                                               EntityTopology,
                                               typename SubdimensionTag::Increment >;

   using MeshTraitsType      = MeshTraits< MeshConfig >;
   using SubentityTraitsType = typename MeshTraitsType::template SubentityTraits< EntityTopology, SubdimensionTag::value >;

protected:
   using GlobalIndexType    = typename SubentityTraitsType::GlobalIndexType;
   using LocalIndexType     = typename SubentityTraitsType::LocalIndexType;
   using StorageNetworkType = typename SubentityTraitsType::StorageNetworkType;

   bool setNumberOfEntities( const GlobalIndexType& entitiesCount )
   {
      if( ! BaseType::setNumberOfEntities( entitiesCount ) )
         return false;
      this->storageNetwork.setKeysRange( entitiesCount );
      return this->storageNetwork.allocate();
   }

   bool save( File& file ) const
   {
      if( ! BaseType::save( file ) ||
          ! this->storageNetwork.save( file ) )
      {
         std::cerr << "Saving of the entity subentities layer with " << SubdimensionTag::value << " dimension failed." << std::endl;
         return false;
      }
      return true;
   }

   bool load( File& file )
   {
      if( ! BaseType::load( file ) ||
          ! this->storageNetwork.load( file ) )
      {
         std::cerr << "Loading of the entity subentities layer with " << SubdimensionTag::value << " dimension failed." << std::endl;
         return false;
      }
      return true;
   }

   void print( std::ostream& str ) const
   {
      BaseType::print( str );
      str << "Storage network for subentities with dimension " << SubdimensionTag::value << " of entities with dimension " << EntityTopology::dimension << " is: " << std::endl;
      str << this->storageNetwork << std::endl;
   }

   bool operator==( const MeshSubentityStorageLayer& layer ) const
   {
      return ( BaseType::operator==( layer ) &&
               storageNetwork == layer.storageNetwork );
   }

   using BaseType::getSubentityStorageNetwork;
   StorageNetworkType& getSubentityStorageNetwork( SubdimensionTag )
   {
      return this->storageNetwork;
   }

private:
   StorageNetworkType storageNetwork;
};

template< typename MeshConfig,
          typename EntityTopology,
          typename SubdimensionTag >
class MeshSubentityStorageLayer< MeshConfig,
                                 EntityTopology,
                                 SubdimensionTag,
                                 false >
   : public MeshSubentityStorageLayer< MeshConfig,
                                       EntityTopology,
                                       typename SubdimensionTag::Increment >
{
};


template< typename MeshConfig,
          typename EntityTopology >
class MeshSubentityStorageLayer< MeshConfig,
                                 EntityTopology,
                                 DimensionTag< EntityTopology::dimension >,
                                 true >
{
   using SubdimensionTag = DimensionTag< EntityTopology::dimension >;

protected:
   /****
    * These methods are due to 'using BaseType::...;' in the derived classes.
    */
   template< typename GlobalIndexType >
   bool setNumberOfEntities( const GlobalIndexType& entitiesCount )
   {
      return true;
   }

   void print( std::ostream& str ) const {}

   bool operator==( const MeshSubentityStorageLayer& layer ) const
   {
      return true;
   }

   bool save( File& file ) const
   {
      return true;
   }

   bool load( File& file )
   {
      return true;
   }
 
   void getSubentityStorageNetwork( SubdimensionTag ) {}
};

template< typename MeshConfig,
          typename EntityTopology >
class MeshSubentityStorageLayer< MeshConfig,
                                 EntityTopology,
                                 DimensionTag< EntityTopology::dimension >,
                                 false >
{
   using SubdimensionTag = DimensionTag< EntityTopology::dimension >;

protected:
   /****
    * These methods are due to 'using BaseType::...;' in the derived classes.
    */
   template< typename GlobalIndexType >
   bool setNumberOfEntities( const GlobalIndexType& entitiesCount )
   {
      return true;
   }

   void print( std::ostream& str ) const {}

   bool operator==( const MeshSubentityStorageLayer& layer ) const
   {
      return true;
   }

   bool save( File& file ) const
   {
      return true;
   }

   bool load( File& file )
   {
      return true;
   }
 
   void getSubentityStorageNetwork( SubdimensionTag ) {}
};

} // namespace Meshes
} // namespace TNL
