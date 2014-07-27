/***************************************************************************
                          tnlGrid3D_impl.h  -  description
                             -------------------
    begin                : Jan 16, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#ifndef TNLGRID3D_IMPL_H_
#define TNLGRID3D_IMPL_H_

#include <iomanip>
#include <core/tnlAssert.h>

template< typename Real,
          typename Device,
          typename Index >
tnlGrid< 3, Real, Device, Index > :: tnlGrid()
: numberOfCells( 0 ),
  numberOfNxFaces( 0 ),
  numberOfNyFaces( 0 ),
  numberOfNzFaces( 0 ),
  numberOfNxAndNyFaces( 0 ),
  numberOfFaces( 0 ),
  numberOfDxEdges( 0 ),
  numberOfDyEdges( 0 ),
  numberOfDzEdges( 0 ),
  numberOfDxAndDyEdges( 0 ),
  numberOfEdges( 0 ),
  numberOfVertices( 0 )
{
}

template< typename Real,
          typename Device,
          typename Index >
tnlString tnlGrid< 3, Real, Device, Index > :: getType()
{
   return tnlString( "tnlGrid< " ) +
          tnlString( Dimensions ) + ", " +
          tnlString( getParameterType< RealType >() ) + ", " +
          tnlString( Device :: getDeviceType() ) + ", " +
          tnlString( getParameterType< IndexType >() ) + " >";
}

template< typename Real,
          typename Device,
          typename Index >
tnlString tnlGrid< 3, Real, Device, Index > :: getTypeVirtual() const
{
   return this -> getType();
}

template< typename Real,
          typename Device,
          typename Index >
void tnlGrid< 3, Real, Device, Index > :: setDimensions( const Index xSize, const Index ySize, const Index zSize )
{
   tnlAssert( xSize > 0, cerr << "xSize = " << xSize );
   tnlAssert( ySize > 0, cerr << "ySize = " << ySize );
   tnlAssert( zSize > 0, cerr << "zSize = " << zSize );

   this->dimensions.x() = xSize;
   this->dimensions.y() = ySize;
   this->dimensions.z() = zSize;
   this->numberOfCells = xSize * ySize * zSize;
   this->numberOfNxFaces = ( xSize + 1 ) * ySize * zSize;
   this->numberOfNyFaces = xSize * ( ySize + 1 ) * zSize;
   this->numberOfNzFaces = xSize * ySize * ( zSize + 1 );
   this->numberOfNxAndNyFaces = this->numberOfNxFaces + this->numberOfNyFaces;
   this->numberOfFaces = this->numberOfNxFaces +
                         this->numberOfNyFaces +
                         this->numberOfNzFaces;
   this->numberOfDxEdges = xSize * ( ySize + 1 ) * ( zSize + 1 );
   this->numberOfDyEdges = ( xSize + 1 ) * ySize * ( zSize + 1 );
   this->numberOfDzEdges = ( xSize + 1 ) * ( ySize + 1 ) * zSize;
   this->numberOfEdges = this->numberOfDxEdges +
                         this->numberOfDyEdges +
                         this->numberOfDzEdges;
   this->numberOfVertices = ( xSize + 1 ) * ( ySize + 1 ) * ( zSize + 1 );
   
   this->cellProportions.x() = this->proportions  / ( Real ) xSize;
   this->cellProportions.y() = this->proportions  / ( Real ) ySize;
   this->cellProportions.z() = this->proportions  / ( Real ) zSize;
}

template< typename Real,
          typename Device,
          typename Index >
void tnlGrid< 3, Real, Device, Index > :: setDimensions( const CoordinatesType& dimensions )
{
   return this -> setDimensions( dimensions. x(), dimensions. y(), dimensions. z() );
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
const typename tnlGrid< 3, Real, Device, Index > :: CoordinatesType&
   tnlGrid< 3, Real, Device, Index > :: getDimensions() const
{
   return this -> dimensions;
}

template< typename Real,
          typename Device,
          typename Index >
void tnlGrid< 3, Real, Device, Index > :: setDomain( const VertexType& origin,
                                                     const VertexType& proportions )
{
   this->origin = origin;
   this->proportions = proportions;
   if( this->getDimensions().x() > 0 &&
       this->getDimensions().y() > 0 &&
       this->getDimensions().z() > 0 )
   {
      this->cellProportions.x() = proportions.x() / ( Real ) this->getDimensions().x();
      this->cellProportions.y() = proportions.y() / ( Real ) this->getDimensions().y();
      this->cellProportions.z() = proportions.z() / ( Real ) this->getDimensions().z();
   }
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
const typename tnlGrid< 3, Real, Device, Index >::VertexType&
tnlGrid< 3, Real, Device, Index >::getOrigin() const
{
   return this->origin;
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
const typename tnlGrid< 3, Real, Device, Index > :: VertexType&
   tnlGrid< 3, Real, Device, Index > :: getProportions() const
{
	return this->proportions;
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
const typename tnlGrid< 3, Real, Device, Index > :: VertexType&
   tnlGrid< 3, Real, Device, Index > :: getCellProportions() const
{
   return this->cellProportions;
}

template< typename Real,
          typename Device,
          typename Index >
Index tnlGrid< 3, Real, Device, Index > :: getCellIndex( const CoordinatesType& cellCoordinates ) const
{
   tnlAssert( cellCoordinates.x() >= 0 && cellCoordinates.x() < this->getDimensions.x(),
              cerr << "cellCoordinates.x() = " << cellCoordinates.x()
                   << " this->getDimensions().x() = " << this->getDimensions().x()
                   << " this->getName() = " << this->getName(); );
   tnlAssert( cellCoordinates.y() >= 0 && cellCoordinates.y() < this->getDimensions.y(),
              cerr << "cellCoordinates.y() = " << cellCoordinates.y()
                   << " this->getDimensions().y() = " << this->getDimensions().y()
                   << " this->getName() = " << this->getName(); );
   tnlAssert( cellCoordinates.z() >= 0 && cellCoordinates.z() < this->getDimensions.z(),
              cerr << "cellCoordinates.z() = " << cellCoordinates.z()
                   << " this->getDimensions().z() = " << this->getDimensions().z()
                   << " this->getName() = " << this->getName(); );

   return ( cellCoordinates.z() * this->dimensions.y() + cellCoordinates.y() ) * this->dimensions.x() + cellCoordinates.x();
}

template< typename Real,
          typename Device,
          typename Index >
typename tnlGrid< 3, Real, Device, Index >::CoordinatesType
tnlGrid< 3, Real, Device, Index > :: getCellCoordinates( const Index cellIndex ) const
{
   tnlAssert( cellIndex >= 0 && cellIndex < this->getNumberOfCells(),
              cerr << " cellIndex = " << cellIndex
                   << " this->getNumberOfCells() = " << this->getNumberOfCells()
                   << " this->getName() " << this->getName(); );

   return CoordinatesType( cellIndex % this->dimensions.x(),
                           ( cellIndex / this->dimensions.x() ) % this->dimensions.y(),
                           cellIndex / ( this->dimensions.x() * this->dimensions.y() ) );
}

template< typename Real,
          typename Device,
          typename Index >
template< int nx, int ny, int nz >
#ifdef HAVE_CUDA
__device__ __host__
#endif
Index tnlGrid< 3, Real, Device, Index >::getFaceIndex( const CoordinatesType& faceCoordinates ) const
{
   tnlStaticAssert( nx >= 0 && ny >= 0 && && nz >= 0 nx + ny + nz = 1,);
   if( nx )
   {
      tnlAssert( faceCoordinates.x() >= 0 && faceCoordinates.x() < this->getDimensions.x() + 1,
                 cerr << "faceCoordinates.x() = " << faceCoordinates.x()
                      << " this->getDimensions().x() + 1 = " << this->getDimensions().x() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.y() >= 0 && faceCoordinates.y() < this->getDimensions.y(),
                 cerr << "faceCoordinates.y() = " << faceCoordinates.y()
                      << " this->getDimensions().y() = " << this->getDimensions().y()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.z() >= 0 && faceCoordinates.z() < this->getDimensions.z(),
                 cerr << "faceCoordinates.z() = " << faceCoordinates.z()
                      << " this->getDimensions().z() = " << this->getDimensions().z()
                      << " this->getName() = " << this->getName(); );
      return ( faceCoordinates.z() * this->getDimensions.y() + faceCoordinates.y() ) * ( this->getDimensions().x() + 1 ) + faceCoordinates.x();
   }
   if( ny )
   {
      tnlAssert( faceCoordinates.x() >= 0 && faceCoordinates.x() < this->getDimensions.x(),
                 cerr << "faceCoordinates.x() = " << faceCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.y() >= 0 && faceCoordinates.y() < this->getDimensions.y() + 1,
                 cerr << "faceCoordinates.y() = " << faceCoordinates.y()
                      << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                      << " this->getName() = " << this->getName(); )
      tnlAssert( faceCoordinates.z() >= 0 && faceCoordinates.z() < this->getDimensions.z(),
                 cerr << "faceCoordinates.z() = " << faceCoordinates.z()
                      << " this->getDimensions().z() = " << this->getDimensions().z()
                      << " this->getName() = " << this->getName(); );

      return this->numberOfNxFaces + ( faceCoordinates.z() * ( this->getDimensions().y + 1 ) + faceCoordinates.y() ) * this->getDimensions().x() + faceCoordinates.x();
   }
   if( nz )
   {
      tnlAssert( faceCoordinates.x() >= 0 && faceCoordinates.x() < this->getDimensions.x(),
                 cerr << "faceCoordinates.x() = " << faceCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.y() >= 0 && faceCoordinates.y() < this->getDimensions.y(),
                 cerr << "faceCoordinates.y() = " << faceCoordinates.y()
                      << " this->getDimensions().y()= " << this->getDimensions().y()
                      << " this->getName() = " << this->getName(); )
      tnlAssert( faceCoordinates.z() >= 0 && faceCoordinates.z() < this->getDimensions.z() + 1,
                 cerr << "faceCoordinates.z() = " << faceCoordinates.z()
                      << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                      << " this->getName() = " << this->getName(); );

      return this->numberOfNxAndNyFaces + ( faceCoordinates.z() * this->getDimensions().y + faceCoordinates.y() ) * this->getDimensions().x() + faceCoordinates.x();
   }
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
__device__ __host__
#endif
typename tnlGrid< 3, Real, Device, Index >::CoordinatesType
tnlGrid< 3, Real, Device, Index >::getFaceCoordinates( const Index faceIndex, int& nx, int& ny, int& nz ) const
{
   tnlAssert( faceIndex >= 0 && faceIndex < this->getNumberOfFaces(),
              cerr << " faceIndex = " << faceIndex
                   << " this->getNumberOfFaces() = " << this->getNumberOfFaces()
                   << " this->getName() " << this->getName(); );
   if( faceIndex < this->numberOfNxFaces )
   {
      nx = 1;
      ny = 0;
      nz = 0;
      const IndexType aux = this->getDimensions().x() + 1;
      return CoordinatesType( faceIndex % aux,
                             ( faceIndex / aux ) % this->getDimensions().y(),
                             faceIndex / ( aux * this->getDimensions().y() ) );
   }
   if( faceIndex < this->numberOfNxAndNyFaces )
   {
      nx = 0;
      ny = 1;
      nz = 0;
      const IndexType i = faceIndex - this->numberOfNxFaces;
      const IndexType aux = this->getDimensions().y() + 1;
      return CoordinatesType( i % this->getDimensions().x(),
                              ( i / this->getDimensions().x() ) % aux,
                              i / ( aux * this->getDimensions().x ) );
   }
   nx = 0;
   ny = 0;
   nz = 1;
   const IndexType i = faceIndex - this->numberOfNxAndNyFaces;
   return CoordinatesType( i % this->getDimensions().x(),
                           ( i / this->getDimensions().x() ) % this->getDimensions().y(),
                           i / ( this->getDimensions().x() * this->getDimensions().y() ) );
}

template< typename Real,
          typename Device,
          typename Index >
template< int dx, int dy, int dz >
#ifdef HAVE_CUDA
__device__ __host__
#endif
Index tnlGrid< 3, Real, Device, Index > :: getEdgeIndex( const CoordinatesType& edgeCoordinates ) const
{
   tnlStaticAssert( dx >= 0 && dy >= 0 && && dz >= 0 dx + dy + dz = 1,);
   if( dx )
   {
      tnlAssert( edgeCoordinates.x() >= 0 && edgeCoordinates.x() < this->getDimensions.x(),
                 cerr << "edgeCoordinates.x() = " << edgeCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.y() >= 0 && edgeCoordinates.y() < this->getDimensions.y() + 1,
                 cerr << "edgeCoordinates.y() = " << edgeCoordinates.y()
                      << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.z() >= 0 && edgeCoordinates.z() < this->getDimensions.z() + 1,
                 cerr << "edgeCoordinates.z() = " << edgeCoordinates.z()
                      << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                      << " this->getName() = " << this->getName(); );
      return ( edgeCoordinates.z() * ( this->getDimensions.y() + 1 ) + edgeCoordinates.y() ) * this->getDimensions().x() + edgeCoordinates.x();
   }
   if( dy )
   {
      tnlAssert( edgeCoordinates.x() >= 0 && edgeCoordinates.x() < this->getDimensions.x() + 1,
                 cerr << "edgeCoordinates.x() = " << edgeCoordinates.x()
                      << " this->getDimensions().x() + 1 = " << this->getDimensions().x() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.y() >= 0 && edgeCoordinates.y() < this->getDimensions.y(),
                 cerr << "edgeCoordinates.y() = " << edgeCoordinates.y()
                      << " this->getDimensions().y() = " << this->getDimensions().y()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.z() >= 0 && edgeCoordinates.z() < this->getDimensions.z() + 1,
                 cerr << "edgeCoordinates.z() = " << edgeCoordinates.z()
                      << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                      << " this->getName() = " << this->getName(); );

      return this->numberOfDxEdges + ( edgeCoordinates.z() * this->getDimensions().y + edgeCoordinates.y() ) * ( this->getDimensions().x() + 1 ) + edgeCoordinates.x();
   }
   if( dz )
   {
      tnlAssert( edgeCoordinates.x() >= 0 && edgeCoordinates.x() < this->getDimensions.x() + 1,
                 cerr << "edgeCoordinates.x() = " << edgeCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.y() >= 0 && edgeCoordinates.y() < this->getDimensions.y() + 1,
                 cerr << "edgeCoordinates.y() = " << edgeCoordinates.y()
                      << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.z() >= 0 && edgeCoordinates.z() < this->getDimensions.z(),
                 cerr << "edgeCoordinates.z() = " << edgeCoordinates.z()
                      << " this->getDimensions().z() = " << this->getDimensions().z()
                      << " this->getName() = " << this->getName(); );

      return this->numberOfDxAndDyEdges + ( edgeCoordinates.z() * ( this->getDimensions().y + 1 ) + edgeCoordinates.y() ) * ( this->getDimensions().x() + 1 ) + edgeCoordinates.x();
   }
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
__device__ __host__
#endif
typename tnlGrid< 3, Real, Device, Index > :: CoordinatesType
tnlGrid< 3, Real, Device, Index > :: getEdgeCoordinates( const Index i, int& dx, int& dy, int& dz ) const
{
   tnlAssert( edgeIndex >= 0 && edgeIndex < this->getNumberOfEdges(),
              cerr << " edgeIndex = " << edgeIndex
                   << " this->getNumberOfEdges() = " << this->getNumberOfEdges()
                   << " this->getName() " << this->getName(); );
   if( edgeIndex < this->numberOfDxEdges )
   {
      dx = 1;
      dy = 0;
      dz = 0;
      const IndexType aux = this->getDimensions().y() + 1;
      return CoordinatesType( edgeIndex % this->getDimensions().x(),
                             ( edgeIndex / this->getDimensions().x() ) % aux,
                             edgeIndex / ( this->getDimensions().x() * aux ) );
   }
   if( edgeIndex < this->numberOfNxAndNyFaces )
   {
      dx = 0;
      dy = 1;
      dz = 0;
      const IndexType i = edgeIndex - this->numberOfNxEdges;
      const IndexType aux = this->getDimensions().x() + 1;
      return CoordinatesType( i % aux,
                              ( i / aux ) % this->getDimensions().y(),
                              i / ( aux * this->getDimensions().y() ) );
   }
   dx = 0;
   dy = 0;
   dz = 1;
   const IndexType i = edgeIndex - this->numberOfDxAndDyEdges;
   const IndexType aux1 = this->getDimensions().x() + 1;
   const IndexType aux2 = this->getDimensions().y() + 1;
   return CoordinatesType( i % aux1,
                           ( i / aux1 ) % aux2,
                           i / ( aux1 * aux2 ) );

}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Index tnlGrid< 3, Real, Device, Index > :: getVertexIndex( const CoordinatesType& vertexCoordinates ) const
{
   tnlAssert( vertexCoordinates.x() >= 0 && vertexCoordinates.x() < this->getDimensions.x() + 1,
              cerr << "vertexCoordinates.x() = " << vertexCoordinates.x()
                   << " this->getDimensions().x() + 1 = " << this->getDimensions().x() + 1
                   << " this->getName() = " << this->getName(); );
   tnlAssert( vertexCoordinates.y() >= 0 && vertexCoordinates.y() < this->getDimensions.y() + 1,
              cerr << "vertexCoordinates.y() = " << vertexCoordinates.y()
                   << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                   << " this->getName() = " << this->getName(); );
   tnlAssert( vertexCoordinates.z() >= 0 && vertexCoordinates.z() < this->getDimensions.z() + 1,
              cerr << "vertexCoordinates.z() = " << vertexCoordinates.z()
                   << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                   << " this->getName() = " << this->getName(); );

   return ( vertexCoordinates.z() * ( this->getDimensions().y() + 1 ) + vertexCoordinates.y() * ) ( this->getDimensions().x() + 1 ) + vertexCoordinates.x();
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
typename tnlGrid< 3, Real, Device, Index > :: CoordinatesType
tnlGrid< 3, Real, Device, Index > :: getVertexCoordinates( const Index vertexIndex ) const
{
   tnlAssert( vertexIndex >= 0 && vertexIndex < this->getNumberOfVertices(),
              cerr << " vertexIndex = " << vertexIndex
                   << " this->getNumberOfVertices() = " << this->getNumberOfVertices()
                   << " this->getName() " << this->getName(); );
   const IndexType auxX = this->getDimensions().x() + 1;
   const IndexType auxY = this->getDimensions().y() + 1;
   const IndexType auxZ = this->getDimensions().z() + 1;
   return CoordinatesType( vertexIndex % auxX,
                           ( vertexIndex / auxX ) % auxY,
                           vertexIndex / ( auxX * auxY ) );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vertex >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Vertex tnlGrid< 3, Real, Device, Index > :: getCellCenter( const CoordinatesType& cellCoordinates ) const
{
   tnlAssert( cellCoordinates.x() >= 0 && cellCoordinates.x() < this->getDimensions.x(),
              cerr << "cellCoordinates.x() = " << cellCoordinates.x()
                   << " this->getDimensions().x() = " << this->getDimensions().x()
                   << " this->getName() = " << this->getName(); );
   tnlAssert( cellCoordinates.y() >= 0 && cellCoordinates.y() < this->getDimensions.y(),
              cerr << "cellCoordinates.y() = " << cellCoordinates.y()
                   << " this->getDimensions().y() = " << this->getDimensions().y()
                   << " this->getName() = " << this->getName(); );
   tnlAssert( cellCoordinates.z() >= 0 && cellCoordinates.z() < this->getDimensions.z(),
              cerr << "cellCoordinates.z() = " << cellCoordinates.z()
                   << " this->getDimensions().z() = " << this->getDimensions().z()
                   << " this->getName() = " << this->getName(); );


   return Vertex( this->origin.x() + ( cellCoordinates.x() + 0.5 ) * this->cellProportions().x(),
                  this->origin.y() + ( cellCoordinates.y() + 0.5 ) * this->cellProportions().y(),
                  this->origin.z() + ( cellCoordinates.z() + 0.5 ) * this->cellProportions().z() );
}

template< int nx, int ny, int nz >
   template< typename Vertex >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Vertex tnlGrid< 3, Real, Device, Index > :: getFaceCenter( const CoordinatesType& faceCoordinates ) const
{
   tnlStaticAssert( nx >= 0 && ny >= 0 && nz >= 0 && nx + ny + nz = 1,);
   if( nx )
   {
      tnlAssert( faceCoordinates.x() >= 0 && faceCoordinates.x() < this->getDimensions.x() + 1,
                 cerr << "faceCoordinates.x() = " << faceCoordinates.x()
                      << " this->getDimensions().x() + 1 = " << this->getDimensions().x() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.y() >= 0 && faceCoordinates.y() < this->getDimensions.y(),
                 cerr << "faceCoordinates.y() = " << faceCoordinates.y()
                      << " this->getDimensions().y() = " << this->getDimensions().y()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.z() >= 0 && faceCoordinates.z() < this->getDimensions.z(),
                 cerr << "faceCoordinates.z() = " << faceCoordinates.z()
                      << " this->getDimensions().z() = " << this->getDimensions().z()
                      << " this->getName() = " << this->getName(); );
      return Vertex( this->origin.x() + faceCoordinates.x() * this->cellProportions().x(),
                     this->origin.y() + ( faceCoordinates.y() + 0.5 ) * this->cellProportions().y(),
                     this->origin.z() + ( faceCoordinates.y() + 0.5 ) * this->cellProportions().z() );
   }
   if( ny )
   {
      tnlAssert( faceCoordinates.x() >= 0 && faceCoordinates.x() < this->getDimensions.x(),
                 cerr << "faceCoordinates.x() = " << faceCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.y() >= 0 && faceCoordinates.y() < this->getDimensions.y() + 1,
                 cerr << "faceCoordinates.y() = " << faceCoordinates.y()
                      << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.z() >= 0 && faceCoordinates.z() < this->getDimensions.z(),
                 cerr << "faceCoordinates.z() = " << faceCoordinates.z()
                      << " this->getDimensions().z() = " << this->getDimensions().z()
                      << " this->getName() = " << this->getName(); );

      return Vertex( this->origin.x() + ( faceCoordinates.x() + 0.5 ) * this->cellProportions().x(),
                     this->origin.y() + faceCoordinates.y() * this->cellProportions().y(),
                     this->origin.z() + ( faceCoordinates.z() + 0.5 ) * this->cellProportions().z() );
   }
   if( nz )
   {
      tnlAssert( faceCoordinates.x() >= 0 && faceCoordinates.x() < this->getDimensions.x(),
                 cerr << "faceCoordinates.x() = " << faceCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.y() >= 0 && faceCoordinates.y() < this->getDimensions.y(),
                 cerr << "faceCoordinates.y() = " << faceCoordinates.y()
                      << " this->getDimensions().y()= " << this->getDimensions().y()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( faceCoordinates.z() >= 0 && faceCoordinates.z() < this->getDimensions.z() + 1,
                 cerr << "faceCoordinates.z() = " << faceCoordinates.z()
                      << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                      << " this->getName() = " << this->getName(); );
      return Vertex( this->origin.x() + ( faceCoordinates.x() + 0.5 ) * this->cellProportions().x(),
                     this->origin.y() + ( faceCoordinates.y() + 0.5 ) * this->cellProportions().y(),
                     this->origin.z() + faceCoordinates.z() * this->cellProportions().z() );
   }
}

template< int dx, int dy, int dz >
   template< typename Vertex >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Vertex tnlGrid< 3, Real, Device, Index > :: getEdgeCenter( const CoordinatesType& edgeCoordinates ) const
{
   tnlStaticAssert( dx >= 0 && dy >= 0 && && dz >= 0 dx + dy + dz = 1,);
   if( dx )
   {
      tnlAssert( edgeCoordinates.x() >= 0 && edgeCoordinates.x() < this->getDimensions.x(),
                 cerr << "edgeCoordinates.x() = " << edgeCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.y() >= 0 && edgeCoordinates.y() < this->getDimensions.y() + 1,
                 cerr << "edgeCoordinates.y() = " << edgeCoordinates.y()
                      << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.z() >= 0 && edgeCoordinates.z() < this->getDimensions.z() + 1,
                 cerr << "edgeCoordinates.z() = " << edgeCoordinates.z()
                      << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                      << " this->getName() = " << this->getName(); );
      return Vertex( this->origin.x() + ( faceCoordinates.x() + 0.5 ) * this->cellProportions().x(),
                     this->origin.y() + faceCoordinates.y() * this->cellProportions().y(),
                     this->origin.z() + faceCoordinates.z() * this->cellProportions().z() );
   }
   if( dy )
   {
      tnlAssert( edgeCoordinates.x() >= 0 && edgeCoordinates.x() < this->getDimensions.x() + 1,
                 cerr << "edgeCoordinates.x() = " << edgeCoordinates.x()
                      << " this->getDimensions().x() + 1 = " << this->getDimensions().x() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.y() >= 0 && edgeCoordinates.y() < this->getDimensions.y(),
                 cerr << "edgeCoordinates.y() = " << edgeCoordinates.y()
                      << " this->getDimensions().y() = " << this->getDimensions().y()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.z() >= 0 && edgeCoordinates.z() < this->getDimensions.z() + 1,
                 cerr << "edgeCoordinates.z() = " << edgeCoordinates.z()
                      << " this->getDimensions().z() + 1 = " << this->getDimensions().z() + 1
                      << " this->getName() = " << this->getName(); );
      return Vertex( this->origin.x() + faceCoordinates.x() * this->cellProportions().x(),
                     this->origin.y() + ( faceCoordinates.y() + 0.5 ) * this->cellProportions().y(),
                     this->origin.z() + faceCoordinates.z() * this->cellProportions().z() );
   }
   if( dz )
   {
      tnlAssert( edgeCoordinates.x() >= 0 && edgeCoordinates.x() < this->getDimensions.x() + 1,
                 cerr << "edgeCoordinates.x() = " << edgeCoordinates.x()
                      << " this->getDimensions().x() = " << this->getDimensions().x()
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.y() >= 0 && edgeCoordinates.y() < this->getDimensions.y() + 1,
                 cerr << "edgeCoordinates.y() = " << edgeCoordinates.y()
                      << " this->getDimensions().y() + 1 = " << this->getDimensions().y() + 1
                      << " this->getName() = " << this->getName(); );
      tnlAssert( edgeCoordinates.z() >= 0 && edgeCoordinates.z() < this->getDimensions.z(),
                 cerr << "edgeCoordinates.z() = " << edgeCoordinates.z()
                      << " this->getDimensions().z() = " << this->getDimensions().z()
                      << " this->getName() = " << this->getName(); );
      return Vertex( this->origin.x() + faceCoordinates.x() * this->cellProportions().x(),
                     this->origin.y() + faceCoordinates.y() * this->cellProportions().y(),
                     this->origin.z() + ( faceCoordinates.z() + 0.5 ) * this->cellProportions().z() );
   }
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vertex >
#ifdef HAVE_CUDA
__device__ __host__
#endif
Vertex tnlGrid< 3, Real, Device, Index >::getVertex( const CoordinatesType& vertexCoordinates ) const
{
   tnlAssert( vertexCoordinates.x() >= 0 && vertexCoordinates.x() < this->getDimensions.x() + 1,
              cerr << "vertexCoordinates.x() = " << vertexCoordinates.x()
                   << " this->getDimensions().x() = " << this->getDimensions().x()
                   << " this->getName() = " << this->getName(); );
   tnlAssert( vertexCoordinates.y() >= 0 && vertexCoordinates.y() < this->getDimensions.y() + 1,
              cerr << "vertexCoordinates.y() = " << vertexCoordinates.y()
                   << " this->getDimensions().y() = " << this->getDimensions().y()
                   << " this->getName() = " << this->getName(); );
   tnlAssert( vertexCoordinates.z() >= 0 && vertexCoordinates.z() < this->getDimensions.z() + 1,
              cerr << "vertexCoordinates.z() = " << vertexCoordinates.z()
                   << " this->getDimensions().z() = " << this->getDimensions().z()
                   << " this->getName() = " << this->getName(); );

   return Vertex( this->origin.x() + vertexCoordinates.x() * this->cellProportions.x(),
                  this->origin.y() + vertexCoordinates.y() * this->cellProportions.y(),
                  this->origin.z() + vertexCoordinates.z() * this->cellProportions.z() );
}

template< typename Real,
          typename Device,
          typename Index >
Index tnlGrid< 3, Real, Device, Index > :: getNumberOfCells() const
{
   return this->numberOfCells;
};

template< typename Real,
          typename Device,
          typename Index >
Index tnlGrid< 3, Real, Device, Index > :: getNumberOfFaces() const
{
   return this->numberOfFaces;
}

template< typename Real,
          typename Device,
          typename Index >
Index tnlGrid< 3, Real, Device, Index > :: getNumberOfEdges() const
{
   return this->numberOfEdges;
}

template< typename Real,
          typename Device,
          typename Index >
Index tnlGrid< 3, Real, Device, Index > :: getNumberOfVertices() const
{
   return numberOfVertices;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
typename GridFunction::RealType
   tnlGrid< 3, Real, Device, Index >::getAbsMax( const GridFunction& f ) const
{
   return f.absMax();
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
typename GridFunction::RealType
   tnlGrid< 3, Real, Device, Index >::getLpNorm( const GridFunction& f1,
                                                 const typename GridFunction::RealType& p ) const
{
   typename GridFunction::RealType lpNorm( 0.0 );
   for( IndexType k = 0; k < getDimensions().z(); k++ )
      for( IndexType j = 0; j < getDimensions().y(); j++ )
         for( IndexType i = 0; i < getDimensions().x(); i++ )
         {
            IndexType c = this->getElementIndex( i, j, k );
            lpNorm += pow( tnlAbs( f1[ c ] ), p );;
         }
   lpNorm *= this->cellProportions().x() * this->cellProportions().y() * this->cellProportions().z();
   return pow( lpNorm, 1.0/p );
}


template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
      typename GridFunction::RealType
         tnlGrid< 3, Real, Device, Index >::getDifferenceAbsMax( const GridFunction& f1,
                                                                           const GridFunction& f2 ) const
{
   typename GridFunction::RealType maxDiff( -1.0 );
   for( IndexType k = 0; k < getDimensions(). z(); k++ )
      for( IndexType j = 0; j < getDimensions(). y(); j++ )
         for( IndexType i = 0; i < getDimensions(). x(); i++ )
         {
            IndexType c = this -> getElementIndex( i, j, k );
            maxDiff = Max( maxDiff, tnlAbs( f1[ c ] - f2[ c ] ) );
         }
   return maxDiff;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename GridFunction >
      typename GridFunction::RealType
         tnlGrid< 3, Real, Device, Index >::getDifferenceLpNorm( const GridFunction& f1,
                                                                           const GridFunction& f2,
                                                                           const typename GridFunction::RealType& p ) const
{
   typename GridFunction::RealType lpNorm( 0.0 );
   for( IndexType k = 0; k < getDimensions(). z(); k++ )
      for( IndexType j = 0; j < getDimensions(). y(); j++ )
         for( IndexType i = 0; i < getDimensions(). x(); i++ )
         {
            IndexType c = this->getElementIndex( i, j, k );
            lpNorm += pow( tnlAbs( f1[ c ] - f2[ c ] ), p );
         }
   lpNorm *= this->cellProportions().x() * this->cellProportions().y() * this->cellProportions().z();
   return pow( lpNorm, 1.0 / p );
}

template< typename Real,
          typename Device,
          typename Index >
bool tnlGrid< 3, Real, Device, Index > :: save( tnlFile& file ) const
{
   if( ! tnlObject::save( file ) )
      return false;
   if( ! this->origin.save( file ) ||
       ! this->proportions.save( file ) ||
       ! this->dimensions.save( file ) )
   {
      cerr << "I was not able to save the domain description of the tnlGrid "
           << this -> getName() << endl;
      return false;
   }
   return true;
};

template< typename Real,
          typename Device,
          typename Index >
bool tnlGrid< 3, Real, Device, Index > :: load( tnlFile& file )
{
   if( ! tnlObject :: load( file ) )
      return false;
   CoordinatesType dimensions;
   if( ! this->origin.load( file ) ||
       ! this->proportions.load( file ) ||
       ! dimensions.load( file ) )
   {
      cerr << "I was not able to load the domain description of the tnlGrid "
           << this -> getName() << endl;
      return false;
   }
   if( ! this->setDimensions( dimensions ) )
   {
      cerr << "I am not able to allocate the loaded grid." << endl;
      return false;
   }
   return true;
};

template< typename Real,
          typename Device,
          typename Index >
bool tnlGrid< 3, Real, Device, Index > :: save( const tnlString& fileName ) const
{
   return tnlObject :: save( fileName );
};

template< typename Real,
          typename Device,
          typename Index >
bool tnlGrid< 3, Real, Device, Index > :: load( const tnlString& fileName )
{
   return tnlObject :: load( fileName );
};

template< typename Real,
           typename Device,
           typename Index >
bool tnlGrid< 3, Real, Device, Index >::writeMesh( const tnlString& fileName,
                                                   const tnlString& format ) const
{
   tnlAssert( false, cerr << "TODO: FIX THIS"); // TODO: FIX THIS
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename MeshFunction >
bool tnlGrid< 3, Real, Device, Index > :: write( const MeshFunction& function,
                                                 const tnlString& fileName,
                                                 const tnlString& format ) const
{
   if( this -> getDofs() != function. getSize() )
   {
      cerr << "The size ( " << function. getSize() << " ) of the mesh function " << function. getName()
           << " does not agree with the DOFs ( " << this -> getDofs() << " ) of the mesh " << this -> getName() << "." << endl;
      return false;
   }
   fstream file;
   file. open( fileName. getString(), ios :: out );
   if( ! file )
   {
      cerr << "I am not able to open the file " << fileName << "." << endl;
      return false;
   }
   file << setprecision( 12 );
   if( format == "gnuplot" )
   {
      for( IndexType k = 0; k < getDimensions(). z(); k++ )
      {
         for( IndexType j = 0; j < getDimensions(). y(); j++ )
         {
            for( IndexType i = 0; i < getDimensions(). x(); i++ )
            {
               VertexType v;
               this -> getElementCenter( CoordinatesType( i, j, k ), v );
               tnlGnuplotWriter::write( file, v );
               tnlGnuplotWriter::write( file, function[ this -> getElementIndex( i, j, k ) ] );
               file << endl;
            }
         }
         file << endl;
      }
   }

   file. close();
   return true;
}

#endif /* TNLGRID3D_IMPL_H_ */
