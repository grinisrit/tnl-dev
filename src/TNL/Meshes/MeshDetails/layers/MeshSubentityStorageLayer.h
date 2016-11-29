/***************************************************************************
                          MeshSubentityStorageLayer.h  -  description
                             -------------------
    begin                : Feb 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/File.h>
#include <TNL/Meshes/MeshDimensionsTag.h>
#include <TNL/Meshes/MeshDetails/traits/MeshSubentityTraits.h>
#include <TNL/Meshes/MeshDetails/MeshEntityOrientation.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag,
          bool SubentityStorage =
            MeshTraits< MeshConfig >::template SubentityTraits< EntityTopology, DimensionsTag::value >::storageEnabled,
          bool SubentityOrientationStorage =
            MeshTraits< MeshConfig >::template SubentityTraits< EntityTopology, DimensionsTag::value >::orientationEnabled >
class MeshSubentityStorageLayer;


template< typename MeshConfig,
          typename EntityTopology >
class MeshSubentityStorageLayers
   : public MeshSubentityStorageLayer< MeshConfig,
                                          EntityTopology,
                                          MeshDimensionsTag< EntityTopology::dimensions - 1 > >
{
};


template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag >
class MeshSubentityStorageLayer< MeshConfig,
                                    EntityTopology,
                                    DimensionsTag,
                                    true,
                                    true >
   : public MeshSubentityStorageLayer< MeshConfig,
                                          EntityTopology,
                                          typename DimensionsTag::Decrement >
{
   typedef MeshSubentityStorageLayer< MeshConfig,
                                         EntityTopology,
                                         typename DimensionsTag::Decrement > BaseType;

   protected:

   static const int Dimensions = DimensionsTag::value;
   typedef MeshTraits< MeshConfig >                                                      MeshTraitsType;
   typedef typename MeshTraitsType::template SubentityTraits< EntityTopology, Dimensions >   SubentityTraitsType;
   typedef typename MeshTraitsType::GlobalIndexType                                          GlobalIndexType;
   typedef typename MeshTraitsType::LocalIndexType                                           LocalIndexType;
   typedef typename SubentityTraitsType::IdArrayType                                         IdArrayType;
   typedef typename SubentityTraitsType::OrientationArrayType                                OrientationArrayType;
   typedef typename MeshTraitsType::IdPermutationArrayAccessorType                           IdPermutationArrayAccessorType;

   MeshSubentityStorageLayer()
   {
      this->subentitiesIndices.setValue( -1 );
   }

   ~MeshSubentityStorageLayer()
   {
      //cout << "      Destroying " << this->sharedSubentitiesIndices.getSize() << " subentities with "<< DimensionsTag::value << " dimensions." << std::endl;
   }

   MeshSubentityStorageLayer& operator = ( const MeshSubentityStorageLayer& layer )
   {
      BaseType::operator=( layer );
      this->subentitiesIndices = layer.subentitiesIndices;
      return *this;
   }

   bool save( File& file ) const
   {
      if( ! BaseType::save( file ) ||
          ! this->subentitiesIndices.save( file ) )
      {
         std::cerr << "Saving of the entity subentities layer with " << DimensionsTag::value << " failed." << std::endl;
         return false;
      }
      return true;
   }

   bool load( File& file )
   {
      if( ! BaseType::load( file ) ||
          ! this->subentitiesIndices.load( file ) )
      {
         std::cerr << "Loading of the entity subentities layer with " << DimensionsTag::value << " failed." << std::endl;
         return false;
      }
      return true;
   }

   void print( std::ostream& str ) const
   {
      BaseType::print( str );
      str << std::endl;
      str << "\t Subentities with " << DimensionsTag::value << " dimensions are: " << subentitiesIndices << ".";
   }

   bool operator==( const MeshSubentityStorageLayer& layer  ) const
   {
      return ( BaseType::operator==( layer ) &&
               subentitiesIndices == layer.subentitiesIndices );
   }

   /****
    * Make visible setters and getters of the lower subentities
    */
   using BaseType::getSubentityIndex;
   using BaseType::setSubentityIndex;

   /****
    * Define setter/getter for the current level of the subentities
    */
   void setSubentityIndex( DimensionsTag,
                           const LocalIndexType localIndex,
                           const GlobalIndexType globalIndex )
   {
      this->subentitiesIndices[ localIndex ] = globalIndex;
   }

   GlobalIndexType getSubentityIndex( DimensionsTag,
                                      const LocalIndexType localIndex ) const
   {
      return this->subentitiesIndices[ localIndex ];
   }

   using BaseType::subentityIdsArray;
   IdArrayType& subentityIdsArray( DimensionsTag ) { return this->subentitiesIndices; }
 
   using BaseType::subentityOrientation;
   IdPermutationArrayAccessorType subentityOrientation( DimensionsTag, LocalIndexType index) const
   {
      TNL_ASSERT( 0 <= index && index < SubentityTraitsType::count, );
 
      return this->subentityOrientations[ index ].getSubvertexPermutation();
   }

   using BaseType::subentityOrientationsArray;
	OrientationArrayType& subentityOrientationsArray( DimensionsTag ) { return this->subentityOrientations; }
 
   private:
      IdArrayType subentitiesIndices;

      OrientationArrayType subentityOrientations;
};


template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag >
class MeshSubentityStorageLayer< MeshConfig,
                                    EntityTopology,
                                    DimensionsTag,
                                    true,
                                    false >
   : public MeshSubentityStorageLayer< MeshConfig,
                                          EntityTopology,
                                          typename DimensionsTag::Decrement >
{
   typedef MeshSubentityStorageLayer< MeshConfig,
                                         EntityTopology,
                                         typename DimensionsTag::Decrement > BaseType;

   protected:
 
   static const int Dimensions = DimensionsTag::value;
   typedef MeshTraits< MeshConfig >                                                      MeshTraitsType;
   typedef typename MeshTraitsType::template SubentityTraits< EntityTopology, Dimensions >   SubentityTraitsType;
   typedef typename MeshTraitsType::GlobalIndexType                                          GlobalIndexType;
   typedef typename MeshTraitsType::LocalIndexType                                           LocalIndexType;
   typedef typename SubentityTraitsType::IdArrayType                                         IdArrayType;
   typedef typename SubentityTraitsType::OrientationArrayType                                OrientationArrayType;
   typedef typename MeshTraitsType::IdPermutationArrayAccessorType                           IdPermutationArrayAccessorType;

   MeshSubentityStorageLayer()
   {
      this->subentitiesIndices.setValue( -1 );
   }

   ~MeshSubentityStorageLayer()
   {
      //cout << "      Destroying " << this->sharedSubentitiesIndices.getSize() << " subentities with "<< DimensionsTag::value << " dimensions." << std::endl;
   }

   MeshSubentityStorageLayer& operator = ( const MeshSubentityStorageLayer& layer )
   {
      BaseType::operator=( layer );
      this->subentitiesIndices = layer.subentitiesIndices;
      return *this;
   }

   bool save( File& file ) const
   {
      if( ! BaseType::save( file ) ||
          ! this->subentitiesIndices.save( file ) )
      {
         std::cerr << "Saving of the entity subentities layer with " << DimensionsTag::value << " failed." << std::endl;
         return false;
      }
      return true;
   }

   bool load( File& file )
   {
      if( ! BaseType::load( file ) ||
          ! this->subentitiesIndices.load( file ) )
      {
         std::cerr << "Loading of the entity subentities layer with " << DimensionsTag::value << " failed." << std::endl;
         return false;
      }
      return true;
   }

   void print( std::ostream& str ) const
   {
      BaseType::print( str );
      str << std::endl;
      str << "\t Subentities with " << DimensionsTag::value << " dimensions are: " << subentitiesIndices << ".";
   }

   bool operator==( const MeshSubentityStorageLayer& layer  ) const
   {
      return ( BaseType::operator==( layer ) &&
               subentitiesIndices == layer.subentitiesIndices );
   }

   /****
    * Make visible setters and getters of the lower subentities
    */
   using BaseType::getSubentityIndex;
   using BaseType::setSubentityIndex;

   /****
    * Define setter/getter for the current level of the subentities
    */
   void setSubentityIndex( DimensionsTag,
                           const LocalIndexType localIndex,
                           const GlobalIndexType globalIndex )
   {
      this->subentitiesIndices[ localIndex ] = globalIndex;
   }

   GlobalIndexType getSubentityIndex( DimensionsTag,
                                      const LocalIndexType localIndex ) const
   {
      return this->subentitiesIndices[ localIndex ];
   }

   using BaseType::subentityIdsArray;
   IdArrayType& subentityIdsArray( DimensionsTag ) { return this->subentitiesIndices; }
 
   using BaseType::subentityOrientationsArray;
   void subentityOrientationsArray() {}
 
   private:
      IdArrayType subentitiesIndices;
};

template< typename MeshConfig,
          typename EntityTopology,
          typename DimensionsTag >
class MeshSubentityStorageLayer< MeshConfig,
                                    EntityTopology,
                                    DimensionsTag,
                                    false,
                                    false >
   : public MeshSubentityStorageLayer< MeshConfig,
                                          EntityTopology,
                                          typename DimensionsTag::Decrement >
{
};


template< typename MeshConfig,
          typename EntityTopology >
class MeshSubentityStorageLayer< MeshConfig,
                                    EntityTopology,
                                    MeshDimensionsTag< 0 >,
                                    true,
                                    false >
{
   typedef MeshDimensionsTag< 0 >                           DimensionsTag;

   protected:
   static const int Dimensions = 0;
   typedef MeshTraits< MeshConfig >                                                          MeshTraitsType;
   typedef typename MeshTraitsType::template SubentityTraits< EntityTopology, Dimensions >   SubentityTraitsType;
   typedef typename MeshTraitsType::GlobalIndexType                                          GlobalIndexType;
   typedef typename MeshTraitsType::LocalIndexType                                           LocalIndexType;
   typedef typename SubentityTraitsType::IdArrayType                                         IdArrayType;

   MeshSubentityStorageLayer()
   {
      this->verticesIndices.setValue( -1 );
   }

   ~MeshSubentityStorageLayer()
   {
      //cout << "      Destroying " << this->sharedVerticesIndices.getSize() << " subentities with "<< DimensionsTag::value << " dimensions." << std::endl;
   }


   MeshSubentityStorageLayer& operator = ( const MeshSubentityStorageLayer& layer )
   {
      this->verticesIndices = layer.verticesIndices;
      return *this;
   }

   bool save( File& file ) const
   {
      if( ! this->verticesIndices.save( file ) )
      {
         std::cerr << "Saving of the entity subentities layer with " << DimensionsTag::value << " failed." << std::endl;
         return false;
      }
      return true;
   }

   bool load( File& file )
   {
      if( ! this->verticesIndices.load( file ) )
      {
         std::cerr << "Loading of the entity subentities layer with " << DimensionsTag::value << " failed." << std::endl;
         return false;
      }
      return true;
   }

   void print( std::ostream& str ) const
   {
      str << "\t Subentities with " << DimensionsTag::value << " dimensions are: " << this->verticesIndices << ".";
   }

   bool operator==( const MeshSubentityStorageLayer& layer  ) const
   {
      return ( verticesIndices == layer.verticesIndices );
   }

   GlobalIndexType getSubentityIndex( DimensionsTag,
                                      const LocalIndexType localIndex ) const
   {
      return this->verticesIndices[ localIndex ];
   }
   void setSubentityIndex( DimensionsTag,
                           const LocalIndexType localIndex,
                           const GlobalIndexType globalIndex )
   {
      this->verticesIndices[ localIndex ] = globalIndex;
   }

   IdArrayType& subentityIdsArray( DimensionsTag ) { return this->verticesIndices; }
 
   protected:
 
      /***
       *  Necessary because of 'using TBase::...;' in the derived classes
       */
	   void subentityOrientation()       {}
	   void subentityOrientationsArray() {}

      IdArrayType verticesIndices;
};

template< typename MeshConfig,
          typename EntityTopology >
class MeshSubentityStorageLayer< MeshConfig,
                                    EntityTopology,
                                    MeshDimensionsTag< 0 >,
                                    false,
                                    false >
{
   public:

   bool save( File& file ) const
   {
      return true;
   }

   bool load( File& file )
   {
      return true;
   }

};

} // namespace Meshes
} // namespace TNL
