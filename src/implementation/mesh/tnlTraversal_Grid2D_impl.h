/***************************************************************************
                          tnlTraversal_Grid2D_impl.h  -  description
                             -------------------
    begin                : Jul 29, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
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


#ifndef TNLTRAVERSAL_GRID2D_IMPL_H_
#define TNLTRAVERSAL_GRID2D_IMPL_H_

template< typename Real,
          typename Index >
   template< typename UserData,
             typename BoundaryEntitiesProcessor,
             typename InteriorEntitiesProcessor >
void
tnlTraversal< tnlGrid< 2, Real, tnlHost, Index >, 2 >::
processEntities( const GridType& grid,
                 UserData& userData,
                 BoundaryEntitiesProcessor& boundaryEntitiesProcessor,
                 InteriorEntitiesProcessor& interiorEntitiesProcessor ) const
{
   /****
    * Traversing cells
    */
   CoordinatesType coordinates;
   const IndexType& xSize = grid.getDimensions().x();
   const IndexType& ySize = grid.getDimensions().y();
   for( coordinates.y() = 0; coordinates.y() < ySize; coordinates.y() ++ )
      for( coordinates.x() = 0; coordinates.x() < xSize; coordinates.x() ++ )
      {
         const IndexType index = grid.getCellIndex( coordinates );
         if( grid.isBoundaryCell( coordinates ) )
            boundaryEntitiesProcessor.template processEntity< 2 >( grid, userData, index, coordinates );
         else
            interiorEntitiesProcessor.template processEntity< 2 >( grid, userData, index, coordinates );
      }
}

template< typename Real,
          typename Index >
   template< typename UserData,
             typename BoundaryEntitiesProcessor,
             typename InteriorEntitiesProcessor >
void
tnlTraversal< tnlGrid< 2, Real, tnlHost, Index >, 1 >::
processEntities( const GridType& grid,
                 UserData& userData,
                 BoundaryEntitiesProcessor& boundaryEntitiesProcessor,
                 InteriorEntitiesProcessor& interiorEntitesProcessor ) const
{
   /****
    * Traversing faces
    */
}

template< typename Real,
          typename Index >
   template< typename UserData,
             typename BoundaryEntitiesProcessor,
             typename InteriorEntitiesProcessor >
void
tnlTraversal< tnlGrid< 2, Real, tnlHost, Index >, 0 >::
processEntities( const GridType& grid,
                 UserData& userData,
                 BoundaryEntitiesProcessor& boundaryEntitiesProcessor,
                 InteriorEntitiesProcessor& interiorEntitesProcessor ) const
{
   /****
    * Traversing vertices
    */
}


template< typename Real,
          typename Index >
   template< typename UserData,
             typename BoundaryEntitiesProcessor,
             typename InteriorEntitiesProcessor >
void
tnlTraversal< tnlGrid< 2, Real, tnlCuda, Index >, 2 >::
processEntities( const GridType& grid,
                 UserData& userData,
                 BoundaryEntitiesProcessor& boundaryEntitiesProcessor,
                 InteriorEntitiesProcessor& interiorEntitesProcessor ) const
{
   /****
    * Traversing cells
    */
}

template< typename Real,
          typename Index >
   template< typename UserData,
             typename BoundaryEntitiesProcessor,
             typename InteriorEntitiesProcessor >
void
tnlTraversal< tnlGrid< 2, Real, tnlCuda, Index >, 1 >::
processEntities( const GridType& grid,
                 UserData& userData,
                 BoundaryEntitiesProcessor& boundaryEntitiesProcessor,
                 InteriorEntitiesProcessor& interiorEntitesProcessor ) const
{
   /****
    * Traversing faces
    */
}

template< typename Real,
          typename Index >
   template< typename UserData,
             typename BoundaryEntitiesProcessor,
             typename InteriorEntitiesProcessor >
void
tnlTraversal< tnlGrid< 2, Real, tnlCuda, Index >, 0 >::
processEntities( const GridType& grid,
                 UserData& userData,
                 BoundaryEntitiesProcessor& boundaryEntitiesProcessor,
                 InteriorEntitiesProcessor& interiorEntitesProcessor ) const
{
   /****
    * Traversing edges
    */
}


#endif /* TNLTRAVERSAL_GRID2D_IMPL_H_ */