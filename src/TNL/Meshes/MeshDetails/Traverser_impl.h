/***************************************************************************
                          Traverser_impl.h  -  description
                             -------------------
    begin                : Dec 25, 2016
    copyright            : (C) 2016 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Meshes/Traverser.h>
#include <TNL/Algorithms/ParallelFor.h>

namespace TNL {
namespace Meshes {

template< typename Mesh,
          typename MeshEntity,
          int EntitiesDimension >
   template< typename EntitiesProcessor,
             typename UserData >
void
Traverser< Mesh, MeshEntity, EntitiesDimension >::
processBoundaryEntities( const MeshPointer& meshPointer,
                         UserData userData ) const
{
   const GlobalIndexType entitiesCount = meshPointer->template getBoundaryEntitiesCount< MeshEntity::getEntityDimension() >();
   auto kernel = [] __cuda_callable__
      ( const GlobalIndexType i,
        const Mesh* mesh,
        UserData userData )
   {
      const GlobalIndexType entityIndex = mesh->template getBoundaryEntityIndex< MeshEntity::getEntityDimension() >( i );
      auto& entity = mesh->template getEntity< MeshEntity::getEntityDimension() >( entityIndex );
      // TODO: if the Mesh::IdType is void, then we should also pass the entityIndex
      EntitiesProcessor::processEntity( *mesh, userData, entity );
   };
   Pointers::synchronizeSmartPointersOnDevice< DeviceType >();
   Algorithms::ParallelFor< DeviceType >::exec(
         (GlobalIndexType) 0, entitiesCount,
         kernel,
         &meshPointer.template getData< DeviceType >(),
         userData );
}

template< typename Mesh,
          typename MeshEntity,
          int EntitiesDimension >
   template< typename EntitiesProcessor,
             typename UserData >
void
Traverser< Mesh, MeshEntity, EntitiesDimension >::
processInteriorEntities( const MeshPointer& meshPointer,
                         UserData userData ) const
{
   const auto entitiesCount = meshPointer->template getInteriorEntitiesCount< MeshEntity::getEntityDimension() >();
   auto kernel = [] __cuda_callable__
      ( const GlobalIndexType i,
        const Mesh* mesh,
        UserData userData )
   {
      const GlobalIndexType entityIndex = mesh->template getInteriorEntityIndex< MeshEntity::getEntityDimension() >( i );
      auto& entity = mesh->template getEntity< MeshEntity::getEntityDimension() >( entityIndex );
      // TODO: if the Mesh::IdType is void, then we should also pass the entityIndex
      EntitiesProcessor::processEntity( *mesh, userData, entity );
   };
   Pointers::synchronizeSmartPointersOnDevice< DeviceType >();
   Algorithms::ParallelFor< DeviceType >::exec(
         (GlobalIndexType) 0, entitiesCount,
         kernel,
         &meshPointer.template getData< DeviceType >(),
         userData );
}

template< typename Mesh,
          typename MeshEntity,
          int EntitiesDimension >
   template< typename EntitiesProcessor,
             typename UserData >
void
Traverser< Mesh, MeshEntity, EntitiesDimension >::
processAllEntities( const MeshPointer& meshPointer,
                    UserData userData ) const
{
   const auto entitiesCount = meshPointer->template getEntitiesCount< MeshEntity::getEntityDimension() >();
   auto kernel = [] __cuda_callable__
      ( const GlobalIndexType entityIndex,
        const Mesh* mesh,
        UserData userData )
   {
      auto& entity = mesh->template getEntity< MeshEntity::getEntityDimension() >( entityIndex );
      // TODO: if the Mesh::IdType is void, then we should also pass the entityIndex
      EntitiesProcessor::processEntity( *mesh, userData, entity );
   };
   Pointers::synchronizeSmartPointersOnDevice< DeviceType >();
   Algorithms::ParallelFor< DeviceType >::exec(
         (GlobalIndexType) 0, entitiesCount,
         kernel,
         &meshPointer.template getData< DeviceType >(),
         userData );
}

} // namespace Meshes
} // namespace TNL
