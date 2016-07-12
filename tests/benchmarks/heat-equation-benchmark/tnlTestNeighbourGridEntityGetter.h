/***************************************************************************
                          tnlTestNeighbourGridEntityGetter.h  -  description
                             -------------------
    begin                : Nov 23, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
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

#pragma once 

#include <core/tnlAssert.h>


template< typename GridEntity,
          int NeighbourEntityDimensions,
          typename EntityStencilTag = 
            tnlGridEntityStencilStorageTag< GridEntity::ConfigType::template neighbourEntityStorage< GridEntity >( NeighbourEntityDimensions ) > >
class tnlTestNeighbourGridEntityGetter
{
   public:

      // TODO: not all specializations are implemented yet
      
      __cuda_callable__
      tnlTestNeighbourGridEntityGetter( const GridEntity& entity )
      {
         //tnlAssert( false, );
      };
      
      __cuda_callable__
      void refresh( const typename GridEntity::GridType& grid,
                    const typename GridEntity::IndexType& entityIndex )
      {
         //tnlAssert( false, );
      };

};

template< typename Real,
          typename Device,
          typename Index,
          typename Config,
          typename StencilStorage >
class tnlTestNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 2, Real, Device, Index >, 2, Config >,
   2,
   StencilStorage >
{
   public:
      
      static const int EntityDimensions = 2;
      static const int NeighbourEntityDimensions = 2;
      typedef tnlGrid< 2, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetter;

      __cuda_callable__ inline
      tnlTestNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
            
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
      
   protected:

      const GridEntityType& entity;
      
      //tnlTestNeighbourGridEntityGetter(){};      
};


