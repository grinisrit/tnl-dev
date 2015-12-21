/***************************************************************************
                          tnlNeighbourGridEntityGetter3D_impl.h  -  description
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

#ifndef TNLNEIGHBOURGRIDENTITYGETTER3D_IMPL_H
#define	TNLNEIGHBOURGRIDENTITYGETTER3D_IMPL_H

#include <mesh/grids/tnlNeighbourGridEntityGetter.h>
#include <mesh/grids/tnlGrid1D.h>
#include <mesh/grids/tnlGrid2D.h>
#include <mesh/grids/tnlGrid3D.h>

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       3         |              3            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 3, Config >,
   3,
   tnlGridEntityStencilStorageTag< tnlGridEntityNoStencil > >
{
   public:
      
      static const int EntityDimensions = 3;
      static const int NeighbourEntityDimensions = 3;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntity( CoordinatesType( entity.getCoordinates().x() + stepX,
                                                      entity.getCoordinates().y() + stepY,
                                                      entity.getCoordinates().z() + stepZ ) );
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ ) = " 
                   << entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return this->entity.getIndex() + ( stepZ * entity.getGrid().getDimensions().y() + stepY ) * entity.getGrid().getDimensions().x() + stepX;
      }
      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
      
   protected:

      const GridEntityType& entity;
      
      //tnlNeighbourGridEntityGetter(){};      
      
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       3         |              3            |       Cross       |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 3, Config >,
   3,
   tnlGridEntityStencilStorageTag< tnlGridEntityCrossStencil > >
{
   public:
      
      static const int EntityDimensions = 3;
      static const int NeighbourEntityDimensions = 3;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntity( CoordinatesType( entity.getCoordinates().x() + stepX,
                                                      entity.getCoordinates().y() + stepY,
                                                      entity.getCoordinates().z() + stepZ ) );
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ ) = " 
                   << entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         if( ( stepX != 0 && stepY != 0 && stepZ != 0 ) ||
             ( stepX < -stencilSize || stepX > stencilSize ||
               stepY < -stencilSize || stepY > stencilSize ||
               stepZ < -stencilSize || stepZ > stencilSize ) )                  
            return this->entity.getIndex() + ( stepZ * entity.getGrid().getDimensions().y() + stepY ) * entity.getGrid().getDimensions().x() + stepX;
         if( stepY == 0 && stepZ == 0 )
            return stencilX[ stepX + stencilSize ];
         if( stepZ == 0 )
            return stencilY[ stepY + stencilSize ];
         return stencilZ[ stepZ + stencilSize ];

      }
      
      template< IndexType index > 
      class StencilXRefresher
      {
         public:
            
            __cuda_callable__
            static void exec( ThisType& neighbourEntityGetter, const IndexType& entityIndex )
            {
               neighbourEntityGetter.stencil[ index + stencilSize ] = entityIndex + index;
            }
      };

      template< IndexType index > 
      class StencilYRefresher
      {
         public:
            
            __cuda_callable__
            static void exec( ThisType& neighbourEntityGetter, const IndexType& entityIndex )
            {
               neighbourEntityGetter.stencil[ index + stencilSize ] = 
                  entityIndex + index * neighbourEntityGetter.entity.getGrid().getDimensions().x();
            }
      };

      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex )
      {
         tnlStaticFor< IndexType, -stencilSize, -1, StencilZRefresher >::exec( *this, entityIndex );
         tnlStaticFor< IndexType, 1, stencilSize, StencilZRefresher >::exec( *this, entityIndex );
         tnlStaticFor< IndexType, -stencilSize, -1, StencilYRefresher >::exec( *this, entityIndex );
         tnlStaticFor< IndexType, 1, stencilSize, StencilYRefresher >::exec( *this, entityIndex );         
         tnlStaticFor< IndexType, -stencilSize, stencilSize, StencilXRefresher >::exec( *this, entityIndex );
      };
      
   protected:

      const GridEntityType& entity;
      
      IndexType stencilX[ 2 * stencilSize + 1 ];
      IndexType stencilY[ 2 * stencilSize + 1 ];
      IndexType stencilZ[ 2 * stencilSize + 1 ];
      
      //tnlNeighbourGridEntityGetter(){};            
};



/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       3         |              2            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 3, Config >,
   2,
   tnlGridEntityStencilStorageTag< tnlGridEntityNoStencil > >
{
   public:
      
      static const int EntityDimensions = 3;
      static const int NeighbourEntityDimensions = 2;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;
      typedef typename GridEntityType::EntityOrientationType EntityOrientationType;
      typedef typename GridEntityType::EntityBasisType EntityBasisType;

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( ! stepX + ! stepY + ! stepZ == 2,
                    cerr << "Only one of the steps can be non-zero: stepX = " << stepX 
                         << " stepY = " << stepY
                         << " stepZ = " << stepZ );
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + 
                       CoordinatesType( stepX + ( stepX < 0 ), 
                                        stepY + ( stepY < 0 ),
                                        stepZ + ( stepZ < 0 ) ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + 
                       CoordinatesType( stepX + ( stepX < 0 ),
                                        stepY + ( stepY < 0 ),
                                        stepZ + ( stepZ < 0 ) ) 
                       < entity.getGrid().getDimensions() + 
                        CoordinatesType( Sign( stepX ), Sign( stepY ), Sign(  stepZ ) ),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntityType( this->entity.getGrid(),
                                         CoordinatesType( entity.getCoordinates().x() + stepX + ( stepX < 0 ),
                                                          entity.getCoordinates().y() + stepY + ( stepY < 0 ),
                                                          entity.getCoordinates().z() + stepZ + ( stepZ < 0 ) ),
                                         EntityOrientationType( stepX > 0 ? 1 : -1,
                                                                stepY > 0 ? 1 : -1,
                                                                stepZ > 0 ? 1 : -1 ),
                                         EntityBasisType( ! stepX, !stepY, !stepZ ) );
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         return GridEntityGetter::getEntityIndex( this->entity.getGrid(), getEntity< stepX, stepY, stepZ >() );
      }
      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
      
   protected:

      const GridEntityType& entity;
      
      //tnlNeighbourGridEntityGetter(){};            
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       3         |              1            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 3, Config >,
   1,
   tnlGridEntityStencilStorageTag< tnlGridEntityNoStencil > >
{
   public:
      
      static const int EntityDimensions = 3;
      static const int NeighbourEntityDimensions = 1;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;
      typedef typename GridEntityType::EntityOrientationType EntityOrientationType;
      typedef typename GridEntityType::EntityBasisType EntityBasisType;

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( ! stepX + ! stepY + ! stepZ == 1,
                    cerr << "Exactly two of the steps must be non-zero: stepX = " << stepX 
                         << " stepY = " << stepY
                         << " stepZ = " << stepZ );
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + 
                       CoordinatesType( stepX + ( stepX < 0 ), 
                                        stepY + ( stepY < 0 ),
                                        stepZ + ( stepZ < 0 ) ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + 
                       CoordinatesType( stepX + ( stepX < 0 ),
                                        stepY + ( stepY < 0 ),
                                        stepZ + ( stepZ < 0 ) ) 
                       < entity.getGrid().getDimensions() + 
                        CoordinatesType( Sign( stepX ), Sign( stepY ), Sign(  stepZ ) ),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntity( CoordinatesType( entity.getCoordinates().x() + stepX + ( stepX < 0 ),
                                                      entity.getCoordinates().y() + stepY + ( stepY < 0 ),
                                                      entity.getCoordinates().z() + stepZ + ( stepZ < 0 ) ),
                                     EntityOrientationType( !!stepX, !!stepY, !!stepZ ),
                                     EntityBasisType( !stepX, !stepY, !stepZ ));
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         return GridEntityGetter::getEntityIndex( this->entity.getGrid(), getEntity< stepX, stepY, stepZ >( entity ) );
      }
      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
      
   protected:

      const GridEntityType& entity;
      
      //tnlNeighbourGridEntityGetter(){};            
};


/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       3         |            0              |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 3, Config >,
   0, 
   tnlGridEntityStencilStorageTag< tnlGridEntityNoStencil > >
{
   public:
      
      static const int EntityDimensions = 3;
      static const int NeighbourEntityDimensions = 0;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;
      typedef typename GridEntityType::EntityOrientationType EntityOrientationType;      

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY,int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( stepX != 0 && stepY != 0 && stepZ != 0,
                    cerr << " stepX = " << stepX 
                         << " stepY = " << stepY
                         << " stepZ = " << stepZ );
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + 
                       CoordinatesType( stepX + ( stepX < 0 ),
                                        stepY + ( stepY < 0 ),
                                        stepZ + ( stepZ < 0 ) ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + 
                       CoordinatesType( stepX + ( stepX < 0 ),
                                        stepY + ( stepY < 0 ),
                                        stepZ + ( stepZ < 0 ) ) 
                       < entity.getGrid().getDimensions() + 
                            CoordinatesType( Sign( stepX ), Sign( stepY ), Sign( stepZ ) ),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 )  ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) )
                   << " entity.getGrid().getDimensions() + CoordinatesType( Sign( stepX ), Sign( stepY ), Sign( stepZ ) ) = " 
                   << entity.getGrid().getDimensions()  + CoordinatesType( Sign( stepX ), Sign( stepY ), Sign( stepZ ) )
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntity( CoordinatesType( entity.getCoordinates().x() + stepX + ( stepX < 0 ),
                                                      entity.getCoordinates().y() + stepY + ( stepY < 0 ),
                                                      entity.getCoordinates().z() + stepZ + ( stepZ < 0 ) ) );
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         return GridEntityGetter::getEntityIndex( entity.getGrid(), getEntity< stepX, stepY, stepZ >( entity ) );
      }
      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};

   protected:

      const GridEntityType& entity;
      
      //tnlNeighbourGridEntityGetter(){};            
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       2         |              3            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 2, Config >,
   3, 
   tnlGridEntityStencilStorageTag< tnlGridEntityNoStencil > >
{
   public:
      
      static const int EntityDimensions = 2;
      static const int NeighbourEntityDimensions = 3;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;
      typedef typename GridEntityType::EntityOrientationType EntityOrientationType;

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( ( ( !! stepX ) == ( !! entity.getOrientation().x() ) ) &&
                    ( ( !! stepY ) == ( !! entity.getOrientation().y() ) ) &&
                    ( ( !! stepZ ) == ( !! entity.getOrientation().z() ) ),
                    cerr << "( stepX, stepY, stepZ ) cannot be perpendicular to entity coordinates: stepX = " << stepX
                         << " stepY = " << stepY << " stepZ = " << stepZ
                         << " entity.getOrientation() = " << entity.getOrientation() );
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() < entity.getGrid().getDimensions() + entity.getOrientation(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() + entity.getOrientation() = " << entity.getGrid().getDimensions() + entity.getOrientation()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + 
                       CoordinatesType( stepX - ( stepX > 0 ),
                                        stepY - ( stepY > 0 ),
                                        stepZ - ( stepZ > 0 ) ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + 
                       CoordinatesType( stepX - ( stepX > 0 ),
                                        stepY - ( stepY > 0 ),
                                        stepZ - ( stepZ > 0 ) ) < entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ), stepZ + ( stepZ < 0 ) )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntityType( entity.getGrid(),
                                         CoordinatesType( entity.getCoordinates().x() + stepX - ( stepX > 0 ),
                                                          entity.getCoordinates().y() + stepY - ( stepY > 0 ),
                                                          entity.getCoordinates().z() + stepZ - ( stepZ > 0 ) ) );
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         return GridEntityGetter::getEntityIndex( entity.getGrid(), getEntity< stepX, stepY, stepZ >() );
      }
      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};

   protected:

      const GridEntityType& entity;
      
      //tnlNeighbourGridEntityGetter(){};            
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimensions |  Stored Stencil   |       
 * +-----------------+---------------------------+-------------------+
 * |       0         |              0            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class tnlNeighbourGridEntityGetter< 
   tnlGridEntity< tnlGrid< 3, Real, Device, Index >, 0, Config >,
   0,
   tnlGridEntityStencilStorageTag< tnlGridEntityNoStencil > >
{
   public:
      
      static const int EntityDimensions = 0;
      static const int NeighbourEntityDimensions = 0;
      typedef tnlGrid< 3, Real, Device, Index > GridType;
      typedef tnlGridEntity< GridType, EntityDimensions, Config > GridEntityType;
      typedef tnlGridEntity< GridType, NeighbourEntityDimensions, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef tnlGridEntityGetter< GridType, NeighbourEntityDimensions > GridEntityGetter;

      __cuda_callable__ inline
      tnlNeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity()
      {
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() <= entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) <= entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ ) = " 
                   << entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return NeighbourGridEntity( CoordinatesType( entity.getCoordinates().x() + stepX,
                                                      entity.getCoordinates().y() + stepY,
                                                      entity.getCoordinates().z() + stepZ ) );
      }
      
      template< int stepX, int stepY, int stepZ >
      __cuda_callable__ inline
      IndexType getEntityIndex()
      {
         tnlAssert( entity.getCoordinates() >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() <= entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         tnlAssert( entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) >= CoordinatesType( 0, 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY, stepZ ) <= entity.getGrid().getDimensions(),
              cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX, stepY, stepZ )
                   << " entity.getGrid().getDimensions() = " << entity.getGrid().getDimensions()
                   << " EntityDimensions = " << EntityDimensions );
         return this->entity.getIndex() + stepZ * ( entity.getGrid().getDimensions().y() + 1 + stepY ) * ( entity.getGrid().getDimensions().x() + 1 ) + stepX;
      }
      
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};

   protected:

      const GridEntityType& entity;
      
      //tnlNeighbourGridEntityGetter(){};      
      
};


#endif	/* TNLNEIGHBOURGRIDENTITYGETTER3D_IMPL_H */

