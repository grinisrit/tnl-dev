/***************************************************************************
                          SubentitySeedsCreator.h  -  description
                             -------------------
    begin                : Aug 20, 2015
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

#include <TNL/StaticFor.h>
#include <TNL/Meshes/MeshDetails/traits/MeshTraits.h>

namespace TNL {
namespace Meshes {

template< typename MeshConfig,
          typename EntityDimensionTag,
          typename SubentityDimensionTag >
class SubentitySeedsCreator
{
   using MeshTraitsType        = MeshTraits< MeshConfig >;
   using GlobalIndexType       = typename MeshTraitsType::GlobalIndexType;
   using LocalIndexType        = typename MeshTraitsType::LocalIndexType;
   using EntityTraitsType      = typename MeshTraitsType::template EntityTraits< EntityDimensionTag::value >;
   using EntityType            = typename EntityTraitsType::EntityType;
   using EntityTopology        = typename EntityTraitsType::EntityTopology;
   using SubvertexAccessorType = typename MeshTraitsType::template SubentityTraits< EntityTopology, 0 >::SubentityAccessorType;
   using SubentityTraits       = typename MeshTraitsType::template SubentityTraits< EntityTopology, SubentityDimensionTag::value >;
   using SubentityType         = typename SubentityTraits::SubentityType;
   using SubentityTopology     = typename SubentityTraits::SubentityTopology;

   static const LocalIndexType SUBENTITIES_COUNT = EntityType::template getSubentitiesCount< SubentityDimensionTag::value >();
   static const LocalIndexType SUBENTITY_VERTICES_COUNT = SubentityType::template getSubentitiesCount< 0 >();

public:
   using SubentitySeedArray = typename SubentityTraits::SeedArrayType;

   static SubentitySeedArray create( const SubvertexAccessorType& subvertices )
   {
      SubentitySeedArray subentitySeeds;
      StaticFor< LocalIndexType, 0, SUBENTITIES_COUNT, CreateSubentitySeeds >::execHost( subentitySeeds, subvertices );

      return subentitySeeds;
   }

private:
   using SubentitySeed = EntitySeed< MeshConfig, SubentityTopology >;

   template< LocalIndexType subentityIndex >
   class CreateSubentitySeeds
   {
      public:
         static void exec( SubentitySeedArray& subentitySeeds, const SubvertexAccessorType& subvertices )
         {
            StaticFor< LocalIndexType, 0, SUBENTITY_VERTICES_COUNT, SetSubentitySeedVertex >::execHost( subentitySeeds[ subentityIndex ], subvertices );
         }

      private:
         template< LocalIndexType subentityVertexIndex >
         class SetSubentitySeedVertex
         {
            public:
               static void exec( SubentitySeed& subentitySeed, const SubvertexAccessorType& subvertices )
               {
                  static const LocalIndexType VERTEX_INDEX = SubentityTraits::template Vertex< subentityIndex, subentityVertexIndex >::index;
                  subentitySeed.setCornerId( subentityVertexIndex, subvertices[ VERTEX_INDEX ] );
               }
         };
   };
};

template< typename MeshConfig,
          typename EntityDimensionTag >
class SubentitySeedsCreator< MeshConfig, EntityDimensionTag, DimensionTag< 0 > >
{
   using MeshTraitsType        = MeshTraits< MeshConfig >;
   using GlobalIndexType       = typename MeshTraitsType::GlobalIndexType;
   using LocalIndexType        = typename MeshTraitsType::LocalIndexType;
   using EntityTraitsType      = typename MeshTraitsType::template EntityTraits< EntityDimensionTag::value >;
   using EntityType            = typename EntityTraitsType::EntityType;
   using EntityTopology        = typename EntityTraitsType::EntityTopology;
   using SubvertexAccessorType = typename MeshTraitsType::template SubentityTraits< EntityTopology, 0 >::SubentityAccessorType;
   using SubentityTraits       = typename MeshTraitsType::template SubentityTraits< EntityTopology, 0 >;

   static const LocalIndexType SUBENTITIES_COUNT = EntityType::template getSubentitiesCount< 0 >();

public:
   using SubentitySeedArray = typename SubentityTraits::SeedArrayType;

   static SubentitySeedArray create( const SubvertexAccessorType& subvertices )
   {
      SubentitySeedArray seeds;
      for( LocalIndexType i = 0; i < seeds.getSize(); i++ )
         seeds[ i ].setCornerId( 0, subvertices[ i ] );
      return seeds;
   }
};

} // namespace Meshes
} // namespace TNL