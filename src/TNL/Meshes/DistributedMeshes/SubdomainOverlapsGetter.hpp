/***************************************************************************
                          SubdomainOverlapsGetter.hpp  -  description
                             -------------------
    begin                : Aug 13, 2018
    copyright            : (C) 2018 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Assert.h>
#include <TNL/Meshes/Grid.h>

namespace TNL {
   namespace Meshes {
      namespace DistributedMeshes {

template< int Dimension,
          typename Real,
          typename Device,
          typename Index,
          typename Communicator >
void
SubdomainOverlapsGetter< Grid< Dimension, Real, Device, Index >, Communicator >::
getOverlaps( const MeshType& mesh,
             SubdomainOverlapsType& lower,
             SubdomainOverlapsType& upper,
             IndexType subdomainOverlapSize )
{
   if( ! CommunicatorType::isDistributed() )
      return;
   DistributedMeshType* distributedMesh = mesh.getDistributedMesh();
   TNL_ASSERT_TRUE( distributedMesh != NULL, "" );
   
   CoordinatesType subdomainCoordinates = distributedMesh->getSubdomainCoordinates();
   
   for( int i = 0; i < Dimension; i++ )
   {
      if( subdomainCoordinates[ i ] > 0 )
         lower[ i ] = subdomainOverlapSize;
      else
         lower[ i ] = 0;
      
      if( subdomainCoordinates[ i ] < mesh.getDimensions()[ i ] - 1 )
         upper[ i ] = subdomainOverlapSize;
      else
         upper[ i ] = 0;
   }
}

      } // namespace DistributedMeshes
   } // namespace Meshes
} // namespace TNL
