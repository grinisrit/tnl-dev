/***************************************************************************
                          MeshEntityTraits.h  -  description
                             -------------------
    begin                : Feb 13, 2014
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

#include <TNL/Containers/Array.h>
#include <TNL/Containers/UnorderedIndexedSet.h>
#include <TNL/Meshes/Topologies/SubentityVertexMap.h>
#include <TNL/Meshes/MeshDetails/traits/MeshTraits.h>
#include <TNL/Meshes/MeshDetails/initializer/EntitySeed.h>

#include <unordered_set>

namespace TNL {
namespace Meshes {

template< typename MeshConfig, typename Device, typename EntityTopology > class MeshEntity;
template< typename MeshConfig, typename EntityTopology > class MeshEntityReferenceOrientation;

template< typename MeshConfig,
          typename EntityDimensionTag,
          typename SuperDimensionTag = DimensionTag< MeshConfig::meshDimension > >
class MeshEntityOrientationNeeded
{
   using SuperentityTopology = typename MeshTraits< MeshConfig >::template EntityTraits< SuperDimensionTag::value >::EntityTopology;

   static constexpr bool previousSuperDimensionValue = MeshEntityOrientationNeeded< MeshConfig, EntityDimensionTag, typename SuperDimensionTag::Decrement >::value;
   static constexpr bool thisSuperDimensionValue = MeshTraits< MeshConfig >::template SubentityTraits< SuperentityTopology, EntityDimensionTag::value >::orientationEnabled;

public:
   static constexpr bool value = ( previousSuperDimensionValue || thisSuperDimensionValue );
};

template< typename MeshConfig, typename DimensionTag >
class MeshEntityOrientationNeeded< MeshConfig, DimensionTag, DimensionTag >
{
public:
   static constexpr bool value = false;
};


template< typename MeshConfig,
          typename DimensionTag >
struct EntityTopologyGetter
{
   static_assert( DimensionTag::value <= MeshConfig::meshDimension, "There are no entities with dimension higher than the mesh dimension." );
   using Topology = typename Topologies::Subtopology< typename MeshConfig::CellTopology, DimensionTag::value >::Topology;
};

template< typename MeshConfig >
struct EntityTopologyGetter< MeshConfig, DimensionTag< MeshConfig::CellTopology::dimension > >
{
   using Topology = typename MeshConfig::CellTopology;
};


template< typename MeshConfig,
          typename Device,
          int Dimension >
class MeshEntityTraits
{
   using GlobalIndexType = typename MeshConfig::GlobalIndexType;

public:
   static_assert( 0 <= Dimension && Dimension <= MeshConfig::meshDimension, "invalid dimension" );

   using EntityTopology                = typename EntityTopologyGetter< MeshConfig, DimensionTag< Dimension > >::Topology;
   using EntityType                    = MeshEntity< MeshConfig, Device, EntityTopology >;
   using SeedType                      = EntitySeed< MeshConfig, EntityTopology >;
   using ReferenceOrientationType      = MeshEntityReferenceOrientation< MeshConfig, EntityTopology >;

   using StorageArrayType              = Containers::Array< EntityType, Device, GlobalIndexType >;
   using SeedIndexedSetType            = Containers::UnorderedIndexedSet< SeedType, GlobalIndexType, typename SeedType::HashType, typename SeedType::KeyEqual >;
   using SeedSetType                   = std::unordered_set< typename SeedIndexedSetType::key_type, typename SeedIndexedSetType::hasher, typename SeedIndexedSetType::key_equal >;
   using ReferenceOrientationArrayType = Containers::Array< ReferenceOrientationType, Device, GlobalIndexType >;

   static constexpr bool storageEnabled = MeshConfig::entityStorage( Dimension );
   static constexpr bool orientationNeeded = MeshEntityOrientationNeeded< MeshConfig, DimensionTag< Dimension > >::value;
};

} // namespace Meshes
} // namespace TNL
