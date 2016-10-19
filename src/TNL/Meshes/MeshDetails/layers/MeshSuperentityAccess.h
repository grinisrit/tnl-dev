/***************************************************************************
                          MeshSuperentityAccess.h  -  description
                             -------------------
    begin                : Aug 15, 2015
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

#include <TNL/Meshes/MeshDetails/traits/MeshTraits.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag,
          bool SuperentityStorage =
             MeshTraits< MeshConfig >::template SuperentityTraits< EntityTopology, DimensionsTag::value >::storageEnabled >
class MeshSuperentityAccessLayer;


template< typename MeshConfig,
          typename EntityTopology >
class MeshSuperentityAccess
   : public MeshSuperentityAccessLayer< MeshConfig,
                                        EntityTopology,
                                        MeshDimensionsTag< MeshTraits< MeshConfig >::meshDimensions > >
{
   typedef MeshSuperentityAccessLayer< MeshConfig,
                                       EntityTopology,
                                       MeshDimensionsTag< MeshTraits< MeshConfig >::meshDimensions > > BaseType;

   static const int Dimensions = MeshTraits< MeshConfig >::meshDimensions;
   typedef MeshTraits< MeshConfig >                                                          MeshTraitsType;

   template< int SuperDimensions >
   using SuperentityTraits = typename MeshTraitsType::template SuperentityTraits< EntityTopology, SuperDimensions >;

public:
   bool operator == ( const MeshSuperentityAccess< MeshConfig, EntityTopology >& a ) const { return true; } // TODO: fix

   void print( std::ostream& str ) const
   {
      BaseType::print( str );
   }

   // TODO: probably should be moved to MeshEntity itself - or the related methods could be moved here
   template< int SuperDimensions >
   void bindSuperentitiesStorageNetwork( const typename SuperentityTraits< SuperDimensions >::SuperentityAccessorType& storage )
   {
      static_assert( SuperentityTraits< SuperDimensions >::storageEnabled, "You try to bind superentities which are not configured for storage." );
      BaseType::bindSuperentitiesStorageNetwork( MeshDimensionsTag< SuperDimensions >(),
                                                 storage );
   }

   template< int SuperDimensions >
   bool setNumberOfSuperentities( const typename SuperentityTraits< SuperDimensions >::LocalIndexType size )
   {
      static_assert( SuperentityTraits< SuperDimensions >::storageEnabled, "You try to get number of superentities which are not configured for storage." );
      return BaseType::setNumberOfSuperentities( MeshDimensionsTag< SuperDimensions >(),
                                                 size );
   }
};

template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag >
class MeshSuperentityAccessLayer< MeshConfig,
                                  EntityTopology,
                                  DimensionsTag,
                                  true >
   : public MeshSuperentityAccessLayer< MeshConfig, EntityTopology, typename DimensionsTag::Decrement >
{
	typedef MeshSuperentityAccessLayer< MeshConfig, EntityTopology, typename DimensionsTag::Decrement > BaseType;

   typedef MeshTraits< MeshConfig >                                                             MeshTraitsType;
   typedef typename MeshTraitsType::template SuperentityTraits< EntityTopology, DimensionsTag::value > SuperentityTraitsType;

public:
   typedef typename MeshTraitsType::IdArrayAccessorType                                         IdArrayAccessorType;
   typedef typename SuperentityTraitsType::GlobalIndexType                                      GlobalIndexType;
   typedef typename SuperentityTraitsType::LocalIndexType                                       LocalIndexType;
   typedef typename SuperentityTraitsType::StorageNetworkType                                   StorageNetworkType;
   typedef typename SuperentityTraitsType::SuperentityAccessorType                              SuperentityAccessorType;

   /****
     * Make visible setters and getters of the lower superentities
     */
   using BaseType::bindSuperentitiesStorageNetwork;
   using BaseType::setNumberOfSuperentities;
   using BaseType::getNumberOfSuperentities;
   using BaseType::setSuperentityIndex;
   using BaseType::getSuperentityIndex;

   /****
    * Define setter/getter for the current level of the superentities
    */
   void bindSuperentitiesStorageNetwork( DimensionsTag,
                                         const SuperentityAccessorType& storage )
   {
      this->superentityIndices.bind( storage );
   }

   bool setNumberOfSuperentities( DimensionsTag,
                                  const LocalIndexType size )
   {
      return this->superentityIndices.setSize( size );
   }

   LocalIndexType getNumberOfSuperentities( DimensionsTag ) const
   {
      return this->superentityIndices.getSize();
   }

   void setSuperentityIndex( DimensionsTag,
                             const LocalIndexType& localIndex,
                             const GlobalIndexType& globalIndex )
   {
      this->superentityIndices[ localIndex ] = globalIndex;
   }

   GlobalIndexType getSuperentityIndex( DimensionsTag,
                                        const LocalIndexType localIndex ) const
   {
      return this->superentityIndices[ localIndex ];
   }

   using BaseType::superentityIds;
   IdArrayAccessorType superentityIds( DimensionsTag ) const { return m_superentityIndices; }

   using BaseType::superentityIdsArray;
   IdArrayAccessorType &superentityIdsArray( DimensionsTag ) { return m_superentityIndices; }

   using BaseType::getSuperentityIndices;
   const SuperentityAccessorType& getSuperentityIndices( DimensionsTag ) const
   {
      return this->superentityIndices;
   }

   SuperentityAccessorType& getSuperentityIndices( DimensionsTag )
   {
      return this->superentityIndices;
   }

   void print( std::ostream& str ) const
   {
      str << "Superentities with " << DimensionsTag::value << " dimensions are: " <<
         this->superentityIndices << std::endl;
      BaseType::print( str );
   }

   //bool operator == ( const MeshSuperentityAccessLayer< MeshConfig, EntityTopology, Dimensions, tnlStorageTraits< true > >& l ) { return true; } // TODO: fix

private:
   // TODO: used only in mesh initializer, should be removed
   IdArrayAccessorType m_superentityIndices;

   SuperentityAccessorType superentityIndices;
};

template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag >
class MeshSuperentityAccessLayer< MeshConfig,
                                  EntityTopology,
                                  DimensionsTag,
                                  false >
   : public MeshSuperentityAccessLayer< MeshConfig, EntityTopology, typename DimensionsTag::Decrement >
{
};

template< typename MeshConfig,
          typename EntityTopology >
class MeshSuperentityAccessLayer< MeshConfig,
                                  EntityTopology,
                                  MeshDimensionsTag< EntityTopology::dimensions >,
                                  false >
{
   static const int Dimensions = EntityTopology::dimensions;
   typedef MeshDimensionsTag< EntityTopology::dimensions >     DimensionsTag;
   typedef MeshTraits< MeshConfig >                                                             MeshTraitsType;
   typedef typename MeshTraitsType::template SuperentityTraits< EntityTopology, DimensionsTag::value > SuperentityTraitsType;

protected:
   typedef typename SuperentityTraitsType::GlobalIndexType           GlobalIndexType;
   typedef typename SuperentityTraitsType::LocalIndexType            LocalIndexType;
   typedef typename SuperentityTraitsType::SuperentityAccessorType   SuperentityAccessorType;

   /***
    * Necessary because of 'using BaseType::...;' in the derived classes
    */
   void bindSuperentitiesStorageNetwork( DimensionsTag,
                                         const SuperentityAccessorType& storage ) {}
   void setNumberOfSuperentities( DimensionsTag,
                                  const LocalIndexType size ) {}
   LocalIndexType getNumberOfSuperentities( DimensionsTag ) const {}
   GlobalIndexType getSuperentityIndex( DimensionsTag,
                                        const LocalIndexType localIndex ) {}
   void setSuperentityIndex( DimensionsTag,
                             const LocalIndexType& localIndex,
                             const GlobalIndexType& globalIndex ) {}

   void superentityIds()      {}
   void superentityIdsArray() {}

   void getSuperentityIndices() {}

   void print( std::ostream& str ) const {}
};

template< typename MeshConfig,
          typename EntityTopology >
class MeshSuperentityAccessLayer< MeshConfig,
                                  EntityTopology,
                                  MeshDimensionsTag< EntityTopology::dimensions >,
                                  true >
{
   static const int Dimensions = EntityTopology::dimensions;
   typedef MeshDimensionsTag< EntityTopology::dimensions >     DimensionsTag;
   typedef MeshTraits< MeshConfig >                                                             MeshTraitsType;
   typedef typename MeshTraitsType::template SuperentityTraits< EntityTopology, DimensionsTag::value > SuperentityTraitsType;

protected:
   typedef typename SuperentityTraitsType::GlobalIndexType           GlobalIndexType;
   typedef typename SuperentityTraitsType::LocalIndexType            LocalIndexType;
   typedef typename SuperentityTraitsType::SuperentityAccessorType   SuperentityAccessorType;

   /***
    * Necessary because of 'using BaseType::...;' in the derived classes
    */
   void bindSuperentitiesStorageNetwork( DimensionsTag,
                                         const SuperentityAccessorType& storage ) {}
   void setNumberOfSuperentities( DimensionsTag,
                                  const LocalIndexType size ) {}
   LocalIndexType getNumberOfSuperentities( DimensionsTag ) const {}
   GlobalIndexType getSuperentityIndex( DimensionsTag,
                                        const LocalIndexType localIndex ) {}
   void setSuperentityIndex( DimensionsTag,
                             const LocalIndexType& localIndex,
                             const GlobalIndexType& globalIndex ) {}

   void superentityIds()      {}
   void superentityIdsArray() {}

   void getSuperentityIndices() {}

   void print( std::ostream& str ) const {}
};

} // namespace Meshes
} // namespace TNL

