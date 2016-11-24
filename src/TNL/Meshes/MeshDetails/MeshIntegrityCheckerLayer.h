/***************************************************************************
                          MeshIntegrityCheckerLayer.h  -  description
                             -------------------
    begin                : Mar 21, 2014
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

#include <TNL/Meshes/MeshDetails/traits/MeshEntityTraits.h>
#include <TNL/Meshes/DimensionTag.h>

namespace TNL {
namespace Meshes {

template< typename MeshType,
          typename DimensionTag,
          bool EntityStorageTag = MeshEntityTraits< typename MeshType::Config,
                                                    DimensionTag::value >::storageEnabled >
class MeshIntegrityCheckerLayer;

template< typename MeshType,
          typename DimensionTag >
class MeshIntegrityCheckerLayer< MeshType,
                                 DimensionTag,
                                 true >
   : public MeshIntegrityCheckerLayer< MeshType,
                                       typename DimensionTag::Decrement >
{
   public:
      typedef MeshIntegrityCheckerLayer< MeshType,
                                         typename DimensionTag::Decrement >     BaseType;
      enum { dimension = DimensionTag::value };

      static bool checkEntities( const MeshType& mesh )
      {
         typedef typename MeshType::template EntitiesTraits< dimension >::ContainerType ContainerType;
         typedef typename ContainerType::IndexType                                       GlobalIndexType;
        std::cout << "Checking entities with " << dimension << " dimension ..." << std::endl;
         for( GlobalIndexType entityIdx = 0;
              entityIdx < mesh.template getNumberOfEntities< dimension >();
              entityIdx++ )
         {
           std::cout << "Entity no. " << entityIdx << "               \r" << std::flush;
         }
        std::cout << std::endl;
         if( ! BaseType::checkEntities( mesh ) )
            return false;
         return true;
      }
};

template< typename MeshType >
class MeshIntegrityCheckerLayer< MeshType,
                                 DimensionTag< 0 >,
                                 true >
{
   public:
      enum { dimension = 0 };

      static bool checkEntities( const MeshType& mesh )
      {
         typedef typename MeshType::template EntitiesTraits< dimension >::ContainerType ContainerType;
         typedef typename ContainerType::IndexType                                       GlobalIndexType;
        std::cout << "Checking entities with " << dimension << " dimension ..." << std::endl;
         for( GlobalIndexType entityIdx = 0;
              entityIdx < mesh.template getNumberOfEntities< dimension >();
              entityIdx++ )
         {
           std::cout << "Entity no. " << entityIdx << "          \r" << std::flush;
         }
        std::cout << std::endl;
         return true;
      }
};

template< typename MeshType,
          typename DimensionTag >
class MeshIntegrityCheckerLayer< MeshType,
                                 DimensionTag,
                                 false >
   : public MeshIntegrityCheckerLayer< MeshType,
                                       typename DimensionTag::Decrement >
{
};

template< typename MeshType >
class MeshIntegrityCheckerLayer< MeshType,
                                 DimensionTag< 0 >,
                                 false >
{
};

} // namespace Meshes
} // namespace TNL
