/***************************************************************************
                          MeshSuperentityAccess.h  -  description
                             -------------------
    begin                : Aug 15, 2015
    copyright            : (C) 2015 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Meshes/MeshDetails/traits/MeshTraits.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig,
          typename MeshEntity,
          typename DimensionsTag,
          bool SuperentityStorage =
             MeshTraits< MeshConfig >::template SuperentityTraits< MeshEntity, DimensionsTag::value >::storageEnabled >
class MeshSuperentityAccessLayer;


template< typename MeshConfig,
          typename MeshEntity >
class MeshSuperentityAccess :
   public MeshSuperentityAccessLayer< MeshConfig,
                                         MeshEntity,
                                         MeshDimensionsTag< MeshTraits< MeshConfig >::meshDimensions > >
{
   public:
      typedef MeshSuperentityAccessLayer< MeshConfig,
                                             MeshEntity,
                                             MeshDimensionsTag< MeshTraits< MeshConfig >::meshDimensions > > BaseType;
 
      bool operator == ( const MeshSuperentityAccess< MeshConfig, MeshEntity>& a ) const { return true; } // TODO: fix
 
      void print( std::ostream& str ) const
      {
         BaseType::print( str );
      };

};

template< typename MeshConfig,
          typename MeshEntity,
          typename Dimensions >
class MeshSuperentityAccessLayer< MeshConfig,
                                     MeshEntity,
                                     Dimensions,
                                     true > :
   public MeshSuperentityAccessLayer< MeshConfig, MeshEntity, typename Dimensions::Decrement >
{
	typedef MeshSuperentityAccessLayer< MeshConfig, MeshEntity, typename Dimensions::Decrement > BaseType;

   public:
 
      typedef MeshTraits< MeshConfig >                                                             MeshTraitsType;
      typedef typename MeshTraitsType::template SuperentityTraits< MeshEntity, Dimensions::value > SuperentityTraitsType;
      typedef typename MeshTraitsType::IdArrayAccessorType                                         IdArrayAccessorType;
      typedef typename SuperentityTraitsType::StorageNetworkType                                   StorageNetworkType;
      typedef typename SuperentityTraitsType::SuperentityAccessorType                              SuperentityAccessorType;
      //typedef typename StorageNetworkType::PortsType                            SuperentityAccessorType;

	   using BaseType::superentityIds;
	   IdArrayAccessorType superentityIds( Dimensions ) const { return m_superentityIndices; }

	   using BaseType::superentityIdsArray;
	   IdArrayAccessorType &superentityIdsArray( Dimensions ) { return m_superentityIndices; }
 
      using BaseType::getSuperentityIndices;
      const SuperentityAccessorType& getSuperentityIndices( Dimensions ) const
      {
         std::cerr << "###" << std::endl;
         return this->superentityIndices;
      }
 
      SuperentityAccessorType& getSuperentityIndices( Dimensions )
      {
         std::cerr << "######" << std::endl;
         return this->superentityIndices;
      }
 
      void print( std::ostream& str ) const
      {
         str << "Superentities with " << Dimensions::value << " dimensions are: " <<
            this->superentityIndices << std::endl;
         BaseType::print( str );
      }
 
      //bool operator == ( const MeshSuperentityAccessLayer< MeshConfig, MeshEntity, Dimensions, tnlStorageTraits< true > >& l ) { return true; } // TODO: fix

   private:
	   IdArrayAccessorType m_superentityIndices;
 
      SuperentityAccessorType superentityIndices;
 
};

template< typename MeshConfig,
          typename MeshEntity,
          typename Dimensions >
class MeshSuperentityAccessLayer< MeshConfig,
                                     MeshEntity,
                                     Dimensions,
                                     false > :
   public MeshSuperentityAccessLayer< MeshConfig, MeshEntity, typename Dimensions::Decrement >
{
};

template< typename MeshConfig,
          typename MeshEntity >
class MeshSuperentityAccessLayer< MeshConfig,
                                     MeshEntity,
                                     MeshDimensionsTag< MeshEntity::dimensions >,
                                     false >
{
   protected:
	   /***
       * Necessary because of 'using TBase::...;' in the derived classes
       */
	   void superentityIds()      {}
	   void superentityIdsArray() {}
 
      void getSuperentityIndices() {};
 
      void print( std::ostream& str ) const {};
};

template< typename MeshConfig,
          typename MeshEntity >
class MeshSuperentityAccessLayer< MeshConfig,
                                     MeshEntity,
                                     MeshDimensionsTag< MeshEntity::dimensions >,
                                     true >
{
   protected:
	   /***
       * Necessary because of 'using TBase::...;' in the derived classes
       */
	   void superentityIds()      {}
	   void superentityIdsArray() {}
 
      void getSuperentityIndices() {};
 
      void print( std::ostream& str ) const {};
};

} // namespace Meshes
} // namespace TNL
