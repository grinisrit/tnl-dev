/***************************************************************************
                          NeighbourGridEntityGetter2D_impl.h  -  description
                             -------------------
    begin                : Nov 23, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Meshes/GridDetails/NeighbourGridEntityGetter.h>
#include <TNL/Meshes/GridDetails/Grid1D.h>
#include <TNL/Meshes/GridDetails/Grid2D.h>
#include <TNL/Meshes/GridDetails/Grid3D.h>

namespace TNL {
namespace Meshes {

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimension |  Stencil Storage  |
 * +-----------------+---------------------------+-------------------+
 * |       2         |              2            | No specialization |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config,
          typename StencilStorage >
class NeighbourGridEntityGetter<
   GridEntity< Meshes::Grid< 2, Real, Device, Index >, 2, Config >,
   2,
   StencilStorage >
{
   public:
 
      static const int EntityDimension = 2;
      static const int NeighbourEntityDimension = 2;
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
      typedef GridEntity< GridType, EntityDimension, Config > GridEntityType;
      typedef GridEntity< GridType, NeighbourEntityDimension, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef GridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetterType;

      __cuda_callable__ inline
      NeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity() const
      {
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         return NeighbourGridEntityType( this->grid,
                                         CoordinatesType( entity.getCoordinates().x() + stepX,
                                                          entity.getCoordinates().y() + stepY ) );
      }
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      IndexType getEntityIndex() const
      {
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         return this->entity.getIndex() + stepY * entity.getMesh().getDimensions().x() + stepX;
      }
 
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
 
   protected:

      const GridEntityType& entity;
 
      //NeighbourGridEntityGetter(){};
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimension |  Stencil Storage  |
 * +-----------------+---------------------------+-------------------+
 * |       2         |              2            |       Cross       |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config >
class NeighbourGridEntityGetter<
   GridEntity< Meshes::Grid< 2, Real, Device, Index >, 2, Config >,
   2,
   GridEntityStencilStorageTag< GridEntityCrossStencil > >
{
   public:
 
      static const int EntityDimension = 2;
      static const int NeighbourEntityDimension = 2;
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
      typedef GridEntity< GridType, EntityDimension, Config > GridEntityType;
      typedef GridEntity< GridType, NeighbourEntityDimension, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef GridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetterType;
      typedef GridEntityStencilStorageTag< GridEntityCrossStencil > StencilStorage;
      typedef NeighbourGridEntityGetter< GridEntityType, 2, StencilStorage > ThisType;
 
 
      static const int stencilSize = Config::getStencilSize();

      __cuda_callable__ inline
      NeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity() const
      {
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
            return NeighbourGridEntityType( this->entity.getMesh(),
                                            CoordinatesType( entity.getCoordinates().x() + stepX,
                                                             entity.getCoordinates().y() + stepY ) );
      }
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      IndexType getEntityIndex() const
      {
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
#ifndef HAVE_CUDA // TODO: fix this to work with CUDA
         if( ( stepX != 0 && stepY != 0 ) ||
             ( stepX < -stencilSize || stepX > stencilSize ||
               stepY < -stencilSize || stepY > stencilSize ) )
            return this->entity.getIndex() + stepY * entity.getMesh().getDimensions().x() + stepX;
         if( stepY == 0 )
            return stencilX[ stepX + stencilSize ];
         return stencilY[ stepY + stencilSize ];
#else
         return this->entity.getIndex() + stepY * entity.getMesh().getDimensions().x() + stepX;
#endif
 
      }
 
      template< IndexType index >
      class StencilXRefresher
      {
         public:
 
            __cuda_callable__
            static void exec( ThisType& neighbourEntityGetter, const IndexType& entityIndex )
            {
               neighbourEntityGetter.stencilX[ index + stencilSize ] = entityIndex + index;
            }
      };

      template< IndexType index >
      class StencilYRefresher
      {
         public:
 
            __cuda_callable__
            static void exec( ThisType& neighbourEntityGetter, const IndexType& entityIndex )
            {
               neighbourEntityGetter.stencilY[ index + stencilSize ] =
                  entityIndex + index * neighbourEntityGetter.entity.getMesh().getDimensions().x();
            }
      };

 
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex )
      {
#ifndef HAVE_CUDA // TODO: fix this to work with CUDA
         StaticFor< IndexType, -stencilSize, 0, StencilYRefresher >::exec( *this, entityIndex );
         StaticFor< IndexType, 1, stencilSize + 1, StencilYRefresher >::exec( *this, entityIndex );
         StaticFor< IndexType, -stencilSize, stencilSize + 1, StencilXRefresher >::exec( *this, entityIndex );
#endif
      };
 
   protected:

      const GridEntityType& entity;
 
      IndexType stencilX[ 2 * stencilSize + 1 ];
      IndexType stencilY[ 2 * stencilSize + 1 ];
 
      //NeighbourGridEntityGetter(){};
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimension |  Stencil Storage  |
 * +-----------------+---------------------------+-------------------+
 * |       2         |              1            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config,
          typename StencilStorage >
class NeighbourGridEntityGetter<
   GridEntity< Meshes::Grid< 2, Real, Device, Index >, 2, Config >,
   1,
   StencilStorage >
{
   public:
 
      static const int EntityDimension = 2;
      static const int NeighbourEntityDimension = 1;
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
      typedef GridEntity< GridType, EntityDimension, Config > GridEntityType;
      typedef GridEntity< GridType, NeighbourEntityDimension, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef GridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetterType;
      typedef typename GridEntityType::EntityOrientationType EntityOrientationType;
      typedef typename GridEntityType::EntityBasisType EntityBasisType;

      __cuda_callable__ inline
      NeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity() const
      {
         TNL_ASSERT( ! stepX + ! stepY == 1,
                    std::cerr << "Only one of the steps can be non-zero: stepX = " << stepX << " stepY = " << stepY );
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() +
                       CoordinatesType( stepX + ( stepX < 0 ),
                                        stepY + ( stepY < 0 ) ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() +
                       CoordinatesType( stepX + ( stepX < 0 ),
                                        stepY + ( stepY < 0 ) )
                       < entity.getMesh().getDimensions() + CoordinatesType( ( stepX > 0 ), ( stepY > 0 ) ),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         return NeighbourGridEntityType( this->entity.getMesh(),
                                         CoordinatesType( entity.getCoordinates().x() + stepX + ( stepX < 0 ),
                                                          entity.getCoordinates().y() + stepY + ( stepY < 0 ) ),
                                         EntityOrientationType( stepX ? (stepX > 0 ? 1 : -1) : 0,
                                                                stepY ? (stepY > 0 ? 1 : -1) : 0 ),
                                         EntityBasisType( ! stepX, ! stepY ) );
      }
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      IndexType getEntityIndex() const
      {
         return GridEntityGetterType::getEntityIndex( this->entity.getMesh(), this->template getEntity< stepX, stepY >() );
      }
 
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};

   protected:

      const GridEntityType& entity;
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimension |  Stencil Storage  |
 * +-----------------+---------------------------+-------------------+
 * |       2         |            0              |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config,
          typename StencilStorage >
class NeighbourGridEntityGetter<
   GridEntity< Meshes::Grid< 2, Real, Device, Index >, 2, Config >,
   0,
   StencilStorage >
{
   public:
 
      static const int EntityDimension = 2;
      static const int NeighbourEntityDimension = 0;
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
      typedef GridEntity< GridType, EntityDimension, Config > GridEntityType;
      typedef GridEntity< GridType, NeighbourEntityDimension, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef GridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetterType;

      __cuda_callable__ inline
      NeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity() const
      {
         TNL_ASSERT( stepX != 0 && stepY != 0,
                    std::cerr << " stepX = " << stepX << " stepY = " << stepY );
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() +
                       CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() +
                       CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) )
                       < entity.getMesh().getDimensions() + CoordinatesType( ( stepX > 0 ), ( stepY > 0 ) ),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) )
                   << " entity.getMesh().getDimensions() + CoordinatesType( sign( stepX ), sign( stepY ) ) = "
                   << entity.getMesh().getDimensions()  + CoordinatesType( sign( stepX ), sign( stepY ) )
                   << " EntityDimension = " << EntityDimension );
         return NeighbourGridEntityType( this->grid,
                                         CoordinatesType( entity.getCoordinates().x() + stepX + ( stepX < 0 ),
                                                          entity.getCoordinates().y() + stepY + ( stepY < 0 ) ) );
      }
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      IndexType getEntityIndex() const
      {
         return GridEntityGetterType::getEntityIndex( this->entity.getMesh(), this->template getEntity< stepX, stepY >() );
      }
 
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
 
   protected:

      const GridEntityType& entity;
 
      //NeighbourGridEntityGetter(){};
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimension |  Stencil Storage  |
 * +-----------------+---------------------------+-------------------+
 * |       1         |              2            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config,
          typename StencilStorage >
class NeighbourGridEntityGetter<
   GridEntity< Meshes::Grid< 2, Real, Device, Index >, 1, Config >,
   2,
   StencilStorage >
{
   public:
 
      static const int EntityDimension = 1;
      static const int NeighbourEntityDimension = 2;
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
      typedef GridEntity< GridType, EntityDimension, Config > GridEntityType;
      typedef GridEntity< GridType, NeighbourEntityDimension, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef GridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetterType;

      __cuda_callable__ inline
      NeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity() const
      {
         /*TNL_ASSERT( ( ( !! stepX ) == ( !! entity.getOrientation().x() ) ) &&
                    ( ( !! stepY ) == ( !! entity.getOrientation().y() ) ),
                    std::cerr << "( stepX, stepY ) cannot be perpendicular to entity coordinates: stepX = " << stepX << " stepY = " << stepY
                         << " entity.getOrientation() = " << entity.getOrientation() );*/
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() < entity.getMesh().getDimensions() + entity.getOrientation(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() + entity.getOrientation() = " << entity.getMesh().getDimensions() + entity.getOrientation()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() +
                       CoordinatesType( stepX - ( stepX > 0 ) * ( entity.getOrientation().x() != 0.0 ),
                                        stepY - ( stepY > 0 ) * ( entity.getOrientation().y() != 0.0 ) ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() +
                       CoordinatesType( stepX - ( stepX > 0 ) * ( entity.getOrientation().x() != 0.0 ),
                                        stepY - ( stepY > 0 ) * ( entity.getOrientation().y() != 0.0 ) ) < entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 )  * ( entity.getOrientation().x() != 0.0 ), stepY + ( stepY < 0 ) * ( entity.getOrientation().y() != 0.0 ) ) = "
                   << entity.getCoordinates()  + CoordinatesType( stepX + ( stepX < 0 ), stepY + ( stepY < 0 ) )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         return NeighbourGridEntityType( this->entity.getMesh(),
                     CoordinatesType( entity.getCoordinates().x() + stepX - ( stepX > 0 ) * ( entity.getOrientation().x() != 0.0 ),
                                      entity.getCoordinates().y() + stepY - ( stepY > 0 ) * ( entity.getOrientation().y() != 0.0 ) ) );
      }
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      IndexType getEntityIndex() const
      {
         return GridEntityGetterType::getEntityIndex( this->entity.getMesh(), this->template getEntity< stepX, stepY >() );
      }
 
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};
 
   protected:

      const GridEntityType& entity;
};

/****
 * +-----------------+---------------------------+-------------------+
 * | EntityDimenions | NeighbourEntityDimension |  Stencil Storage  |
 * +-----------------+---------------------------+-------------------+
 * |       0         |              0            |       None        |
 * +-----------------+---------------------------+-------------------+
 */
template< typename Real,
          typename Device,
          typename Index,
          typename Config,
          typename StencilStorage >
class NeighbourGridEntityGetter<
   GridEntity< Meshes::Grid< 2, Real, Device, Index >, 0, Config >,
   0,
   StencilStorage >
{
   public:
 
      static const int EntityDimension = 0;
      static const int NeighbourEntityDimension = 0;
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
      typedef GridEntity< GridType, EntityDimension, Config > GridEntityType;
      typedef GridEntity< GridType, NeighbourEntityDimension, Config > NeighbourGridEntityType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef typename GridType::CoordinatesType CoordinatesType;
      typedef GridEntityGetter< GridType, NeighbourGridEntityType > GridEntityGetterType;

      __cuda_callable__ inline
      NeighbourGridEntityGetter( const GridEntityType& entity )
      : entity( entity )
      {}
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      NeighbourGridEntityType getEntity() const
      {
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() <= entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) <= entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         return NeighbourGridEntityType( this->grid,
                                         CoordinatesType( entity.getCoordinates().x() + stepX,
                                                          entity.getCoordinates().y() + stepY ) );
      }
 
      template< int stepX, int stepY >
      __cuda_callable__ inline
      IndexType getEntityIndex() const
      {
         TNL_ASSERT( entity.getCoordinates() >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() <= entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates() = " << entity.getCoordinates()
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         TNL_ASSERT( entity.getCoordinates() + CoordinatesType( stepX, stepY ) >= CoordinatesType( 0, 0 ) &&
                    entity.getCoordinates() + CoordinatesType( stepX, stepY ) <= entity.getMesh().getDimensions(),
              std::cerr << "entity.getCoordinates()  + CoordinatesType( stepX, stepY ) = " << entity.getCoordinates()  + CoordinatesType( stepX, stepY )
                   << " entity.getMesh().getDimensions() = " << entity.getMesh().getDimensions()
                   << " EntityDimension = " << EntityDimension );
         return this->entity.getIndex() + stepY * ( entity.getMesh().getDimensions().x() + 1 ) + stepX;
      }
 
      __cuda_callable__
      void refresh( const GridType& grid, const IndexType& entityIndex ){};

   protected:

      const GridEntityType& entity;
 
      //NeighbourGridEntityGetter(){};
};

} // namespace Meshes
} // namespace TNL

