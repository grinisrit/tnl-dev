/***************************************************************************
                          GridEntityMeasureGetter.h  -  description
                             -------------------
    begin                : Jan 25, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

namespace TNL {
namespace Meshes {

template< typename Grid,
          int EntityDimensions >
class GridEntityMeasureGetter
{
};

/***
 * Common implementation for vertices
 */
template< int Dimensions,
          typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< Dimensions, Real, Device, Index >, 0 >
{
   public:
 
      typedef Meshes::Grid< Dimensions, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         return 0.0;
      }
};

/****
 * 1D grid
 */

template< typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< 1, Real, Device, Index >, 1 >
{
   public:
 
      typedef Meshes::Grid< 1, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         return grid.template getSpaceStepsProducts< 1 >();
      }
};

/****
 * 2D grid
 */
template< typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< 2, Real, Device, Index >, 2 >
{
   public:
 
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         return grid.template getSpaceStepsProducts< 1, 1 >();
      }
};

template< typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< 2, Real, Device, Index >, 1 >
{
   public:
 
      typedef Meshes::Grid< 2, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         if( entity.getOrientation().x() )
            return grid.template getSpaceStepsProducts< 0, 1 >();
         else
            return grid.template getSpaceStepsProducts< 1, 0 >();
      }
};

/****
 * 3D grid
 */
template< typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< 3, Real, Device, Index >, 3 >
{
   public:
 
      typedef Meshes::Grid< 3, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         return grid.template getSpaceStepsProducts< 1, 1, 1 >();
      }
};

template< typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< 3, Real, Device, Index >, 2 >
{
   public:
 
      typedef Meshes::Grid< 3, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         if( entity.getOrientation().x() )
            return grid.template getSpaceStepsProducts< 0, 1, 1 >();
         if( entity.getOrientation().y() )
            return grid.template getSpaceStepsProducts< 1, 0, 1 >();
         else
            return grid.template getSpaceStepsProducts< 1, 1, 0 >();
      }
};

template< typename Real,
          typename Device,
          typename Index >
class GridEntityMeasureGetter< Meshes::Grid< 3, Real, Device, Index >, 1 >
{
   public:
 
      typedef Meshes::Grid< 3, Real, Device, Index > GridType;
 
      template< typename EntityType >
      static const Real& getMeasure( const GridType& grid,
                                     const EntityType& entity )
      {
         if( entity.getBasis().x() )
            return grid.template getSpaceStepsProducts< 1, 0, 0 >();
         if( entity.getBasis().y() )
            return grid.template getSpaceStepsProducts< 0, 1, 0 >();
         else
            return grid.template getSpaceStepsProducts< 0, 0, 1 >();
      }
};

} // namespace Meshes
} // namespace TNL
