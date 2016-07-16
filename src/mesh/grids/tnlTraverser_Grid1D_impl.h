/***************************************************************************
                          tnlTraverser_Grid1D_impl.h  -  description
                             -------------------
    begin                : Jul 29, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLTRAVERSER_GRID1D_IMPL_H_
#define TNLTRAVERSER_GRID1D_IMPL_H_

#include <mesh/grids/tnlGridTraverser.h>


/****
 * Grid 1D, cells
 */
template< typename Real,
          typename Device,
          typename Index,
          typename GridEntity >
   template< typename UserData,
             typename EntitiesProcessor >
void
tnlTraverser< tnlGrid< 1, Real, Device, Index >, GridEntity, 1 >::
processBoundaryEntities( const GridType& grid,
                         UserData& userData ) const
{
   /****
    * Boundary cells
    */
   static_assert( GridEntity::entityDimensions == 1, "The entity has wrong dimensions." );

   tnlGridTraverser< GridType >::template processEntities< GridEntity, EntitiesProcessor, UserData, true >(
      grid,
      CoordinatesType( 0 ),
      grid.getDimensions() - CoordinatesType( 1 ),
      CoordinatesType(),
      CoordinatesType(),
      userData );
}

template< typename Real,
          typename Device,
          typename Index,
          typename GridEntity >
   template< typename UserData,
             typename EntitiesProcessor >
void
tnlTraverser< tnlGrid< 1, Real, Device, Index >, GridEntity, 1 >::
processInteriorEntities( const GridType& grid,
                         UserData& userData ) const
{
   /****
    * Interior cells
    */
   static_assert( GridEntity::entityDimensions == 1, "The entity has wrong dimensions." );

   tnlGridTraverser< GridType >::template processEntities< GridEntity, EntitiesProcessor, UserData, false >(
      grid,
      CoordinatesType( 1 ),
      grid.getDimensions() - CoordinatesType( 2 ),
      CoordinatesType(),
      CoordinatesType(),
      userData );
}

template< typename Real,
          typename Device,
          typename Index,
          typename GridEntity >
   template< typename UserData,
             typename EntitiesProcessor >
void
tnlTraverser< tnlGrid< 1, Real, Device, Index >, GridEntity, 1 >::
processAllEntities(
   const GridType& grid,
   UserData& userData ) const
{
   /****
    * All cells
    */
   static_assert( GridEntity::entityDimensions == 1, "The entity has wrong dimensions." );

   tnlGridTraverser< GridType >::template processEntities< GridEntity, EntitiesProcessor, UserData, false >(
      grid,
      CoordinatesType( 0 ),
      grid.getDimensions() - CoordinatesType( 1 ),
      CoordinatesType(),
      CoordinatesType(),
      userData );
}

/****
 * Grid 1D, vertices
 */
template< typename Real,
          typename Device,
          typename Index,
          typename GridEntity >
   template< typename UserData,
             typename EntitiesProcessor >
void
tnlTraverser< tnlGrid< 1, Real, Device, Index >, GridEntity, 0 >::
processBoundaryEntities( const GridType& grid,
                         UserData& userData ) const
{
   /****
    * Boundary vertices
    */
   static_assert( GridEntity::entityDimensions == 0, "The entity has wrong dimensions." );

   tnlGridTraverser< GridType >::template processEntities< GridEntity, EntitiesProcessor, UserData, true >(
      grid,
      CoordinatesType( 0 ),
      grid.getDimensions(),
      CoordinatesType(),
      CoordinatesType(),
      userData );
}

template< typename Real,
          typename Device,
          typename Index,
          typename GridEntity >
   template< typename UserData,
             typename EntitiesProcessor >
void
tnlTraverser< tnlGrid< 1, Real, Device, Index >, GridEntity, 0 >::
processInteriorEntities( const GridType& grid,
                         UserData& userData ) const
{
   /****
    * Interior vertices
    */
   static_assert( GridEntity::entityDimensions == 0, "The entity has wrong dimensions." );

   tnlGridTraverser< GridType >::template processEntities< GridEntity, EntitiesProcessor, UserData, false >(
      grid,
      CoordinatesType( 1 ),
      grid.getDimensions() - CoordinatesType( 1 ),
      CoordinatesType(),
      CoordinatesType(),
      userData );
}

template< typename Real,
          typename Device,
          typename Index,
          typename GridEntity >
   template< typename UserData,
             typename EntitiesProcessor >
void
tnlTraverser< tnlGrid< 1, Real, Device, Index >, GridEntity, 0 >::
processAllEntities(
   const GridType& grid,
   UserData& userData ) const
{
   /****
    * All vertices
    */
   static_assert( GridEntity::entityDimensions == 0, "The entity has wrong dimensions." );

   tnlGridTraverser< GridType >::template processEntities< GridEntity, EntitiesProcessor, UserData, false >(
      grid,
      CoordinatesType( 0 ),
      grid.getDimensions(),
      CoordinatesType(),
      CoordinatesType(),
      userData );
}

#endif /* TNLTRAVERSER_GRID1D_IMPL_H_ */
