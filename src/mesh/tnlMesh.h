/***************************************************************************
                          tnlMesh.h  -  description
                             -------------------
    begin                : Feb 16, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
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

#ifndef TNLMESH_H_
#define TNLMESH_H_

#include <mesh/tnlMeshEntity.h>
#include <mesh/layers/tnlMeshStorageLayer.h>

template< typename ConfigTag >
class tnlMesh : public tnlMeshStorageLayers< ConfigTag >
{
   //template<typename, typename, typename> friend class InitializerLayer;
   //friend class IOReader<ConfigTag>;

   typedef tnlMeshStorageLayers<ConfigTag>        BaseType;

   public:
   typedef ConfigTag                              Config;
   typedef typename tnlMeshTraits< ConfigTag >::PointType PointType;
   enum { dimensions = tnlMeshTraits< ConfigTag >::meshDimensions };

   template< int Dimensions >
   struct EntitiesTraits
   {
      typedef tnlDimensionsTraits< Dimensions >                       DimensionsTraits;
      typedef tnlMeshEntitiesTraits< ConfigTag, DimensionsTraits >    MeshEntitiesTraits;
      typedef typename MeshEntitiesTraits::Type                       Type;
      typedef typename MeshEntitiesTraits::ContainerType              ContainerType;
      typedef typename ContainerType::IndexType                       GlobalIndexType;
      typedef typename ContainerType::ElementType                     EntityType;
      //typedef typename MeshEntitiesTraits::SharedArrayType          SharedArrayType;
      enum { available = tnlMeshEntityStorage< ConfigTag, Dimensions >::enabled };
   };

   template< int Dimensions >
   bool entitiesAvalable() const
   {
      return EntitiesTraits< Dimensions >::available;
   }

   template< int Dimensions >
   bool setNumberOfEntities( typename EntitiesTraits< Dimensions >::GlobalIndexType size )
   {
      return BaseType::setNumberOfEntities( tnlDimensionsTraits< Dimensions >(), size );
   }

   template< int Dimensions >
   typename EntitiesTraits< Dimensions >::GlobalIndexType getNumberOfEntities() const
   {
      return BaseType::getNumberOfEntities( tnlDimensionsTraits< Dimensions >() );
   }

   template< int Dimensions >
      typename EntitiesTraits< Dimensions >::EntityType&
         getEntity( const typename EntitiesTraits< Dimensions >::GlobalIndexType entityIndex )
   {
      return BaseType::getEntity( tnlDimensionsTraits< Dimensions >(), entityIndex );
   }

   template< int Dimensions >
      const typename EntitiesTraits< Dimensions >::EntityType&
         getEntity( const typename EntitiesTraits< Dimensions >::GlobalIndexType entityIndex ) const
   {
      return BaseType::getEntity( tnlDimensionsTraits< Dimensions >(), entityIndex );
   }

   template< int Dimensions >
      void getEntity( const typename EntitiesTraits< Dimensions >::GlobalIndexType entityIndex,
                      const typename EntitiesTraits< Dimensions >::EntityType& entity )
   {
      BaseType::setEntity( tnlDimensionsTraits< Dimensions >(), entityIndex, entity );
   }

   using BaseType::setNumberOfVertices;
   using BaseType::getNumberOfVertices;
   using BaseType::setVertex;
   using BaseType::getVertex;
   using BaseType::setPoint;
   using BaseType::getPoint;


   void load(const char *filename);
   void write(const char *filename) const;

   //void load(IOReader<ConfigTag> &reader);
   //void write(IOWriter<ConfigTag> &writer) const;

   //using BaseType::entities;
   //template< int Dimensions >
   //typename EntitiesArray<dim>::Type entities() const { return this->entities(DimTag<dim>()); }

private:
   void init();

   tnlStaticAssert( dimensions > 0, "The mesh dimesnions must be greater than 0." );
   tnlStaticAssert( EntitiesTraits< 0 >::available, "Vertices must always be stored" );
   tnlStaticAssert( EntitiesTraits< dimensions >::available, "Cells must always be stored" );
};


#endif /* TNLMESH_H_ */
