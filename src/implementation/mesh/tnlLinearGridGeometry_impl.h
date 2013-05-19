/***************************************************************************
                          tnlLinearGridGeometry_impl.h  -  description
                             -------------------
    begin                : May 10, 2013
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
#ifndef TNLLINEARGRIDGEOMETRY_IMPL_H_
#define TNLLINEARGRIDGEOMETRY_IMPL_H_

#include <core/tnlFile.h>
#include <core/tnlAssert.h>

/****
 * Linear geometry for 1D
 */

template< typename Real,
          typename Device,
          typename Index >
void tnlLinearGridGeometry< 1, Real, Device, Index > :: setParametricStep( const VertexType& parametricStep )
{
   this -> parametricStep = parametricStep;
   this -> elementMeasure = this -> parametricStep. x();
}

template< typename Real,
          typename Device,
          typename Index >
const typename tnlLinearGridGeometry< 1, Real, Device, Index > :: VertexType&
   tnlLinearGridGeometry< 1, Real, Device, Index > :: getParametricStep() const
{
   return this -> parametricStep;
}

template< typename Real,
          typename Device,
          typename Index >
void tnlLinearGridGeometry< 1, Real, Device, Index > :: getElementCenter( const VertexType& origin,
                                                                             const CoordinatesType& coordinates,
                                                                             VertexType& center ) const
{
   center. x() = ( coordinates. x() + 0.5 ) * parametricStep. x();
}

template< typename Real,
          typename Device,
          typename Index >
Real tnlLinearGridGeometry< 1, Real, Device, Index > :: getElementMeasure( const CoordinatesType& i ) const
{
   return elementMeasure;
}

template< typename Real,
          typename Device,
          typename Index >
   template< Index dx >
Real tnlLinearGridGeometry< 1, Real, Device, Index > :: getElementsDistance( const Index i ) const
{
   return dx * parametricStep. x();
}

template< typename Real,
          typename Device,
          typename Index >
template< Index dx >
void tnlLinearGridGeometry< 1, Real, Device, Index > :: getEdgeCoordinates( const Index i,
                                                                               const VertexType& origin,
                                                                               VertexType& coordinates ) const
{
   coordinates. x() = origin. x() + ( i + 0.5 * ( 1.0 + dx ) ) * parametricStep. x();
}

template< typename Real,
          typename Device,
          typename Index >
template< Index dx >
Real tnlLinearGridGeometry< 1, Real, Device, Index > :: getEdgeLength( const Index i ) const
{
   return 0.0;
}

template< typename Real,
          typename Device,
          typename Index >
template< Index dx >
void tnlLinearGridGeometry< 1, Real, Device, Index > :: getEdgeNormal( const Index i,
                                                                          VertexType& normal ) const
{
   tnlAssert( dx == 1 || dx == -1, cerr << " dx = " << dx << endl );
   normal. x() = dx;
}

template< typename Real,
          typename Device,
          typename Index >
bool tnlLinearGridGeometry< 1, Real, Device, Index > :: save( tnlFile& file ) const
{
   if( ! this -> parametricStep. save( file ) )
      return false;
   return true;
};

template< typename Real,
          typename Device,
          typename Index >
bool tnlLinearGridGeometry< 1, Real, Device, Index > :: load( tnlFile& file )
{
   if( ! this -> parametricStep. load( file ) )
      return false;
   this -> elementMeasure = this -> parametricStep. x();
   return true;
};

/****
 * Linear geometry for 2D
 */

template< typename Real,
          typename Device,
          typename Index >
void tnlLinearGridGeometry< 2, Real, Device, Index > :: setParametricStep( const VertexType& parametricStep )
{
   this -> parametricStep = parametricStep;
   this -> elementMeasure = this -> parametricStep. x() * this -> parametricStep. y();
}

template< typename Real,
          typename Device,
          typename Index >
const typename tnlLinearGridGeometry< 2, Real, Device, Index > :: VertexType&
   tnlLinearGridGeometry< 2, Real, Device, Index > :: getParametricStep() const
{
   return this -> parametricStep;
}

template< typename Real,
          typename Device,
          typename Index >
void tnlLinearGridGeometry< 2, Real, Device, Index > :: getElementCenter( const VertexType& origin,
                                                                             const CoordinatesType& coordinates,
                                                                             VertexType& center ) const
{
   center. x() = ( coordinates. x() + 0.5 ) * parametricStep. x();
   center. y() = ( coordinates. y() + 0.5 ) * parametricStep. y();
}

template< typename Real,
          typename Device,
          typename Index >
Real tnlLinearGridGeometry< 2, Real, Device, Index > :: getElementMeasure( const CoordinatesType& coordinates ) const
{
   VertexType v0, v1, v2, v3;
   const VertexType origin( 0.0 );
   this -> template getVertex< -1, -1 >( coordinates, origin, v0 );
   this -> template getVertex<  1, -1  >( coordinates, origin, v1 );
   this -> template getVertex<  1,  1 >( coordinates, origin, v2 );
   this -> template getVertex< -1,  1 >( coordinates, origin, v3 );

   return tnlTriangleArea( v0, v1, v3 ) + tnlTriangleArea( v2, v3, v1 );

   //return elementMeasure;
}

template< typename Real,
          typename Device,
          typename Index >
   template< int dx, int dy >
Real tnlLinearGridGeometry< 2, Real, Device, Index > :: getDualElementMeasure( const CoordinatesType& coordinates ) const
{
   tnlAssert( ( dx == 0 && ( dy == 1 || dy == -1 ) ) ||
              ( dy == 0 && ( dx == 1 || dx == -1 ) ),
              cerr << " dx = " << dx << " dy = " << dy << endl );
   VertexType v0, v1, v2, v3;
   const VertexType origin( 0.0 );
   this -> getElementCenter( origin, coordinates, v0 );
   if( dy == 0 )
   {
      this -> template getVertex< dx, -1 >( coordinates, origin, v1 );
      this -> template getVertex< dx, 1 >( coordinates, origin, v2 );
      CoordinatesType c2( coordinates );
      c2. x() += dx;
      this -> getElementCenter( origin, c2, v3 );
   }
   if( dx == 0 )
   {
      this -> template getVertex< -1, dy >( coordinates, origin, v1 );
      this -> template getVertex<  1, dy >( coordinates, origin, v2 );
      CoordinatesType c2( coordinates );
      c2. y() += dy;
      this -> getElementCenter( origin, c2, v3 );
   }
   return tnlTriangleArea( v0, v1, v3 ) + tnlTriangleArea( v2, v3, v1 );
}

template< typename Real,
          typename Device,
          typename Index >
template< Index dx, Index dy >
void tnlLinearGridGeometry< 2, Real, Device, Index > :: getEdgeNormal( const CoordinatesType& coordinates,
                                                                       VertexType& normal ) const
{
   tnlAssert( ( dx == 0 && ( dy == 1 || dy == -1 ) ) ||
              ( dy == 0 && ( dx == 1 || dx == -1 ) ),
              cerr << " dx = " << dx << " dy = " << dy << endl );
   VertexType v1, v2, origin( 0.0 );
   if( dy == 0 )
   {
      if( dx == 1 )
      {
         this -> getVertex< 1, 1 >( coordinates, origin, v1 );
         this -> getVertex< 1, -1 >( coordinates, origin, v2 );
      }
      else // dx == -1
      {
         this -> getVertex< -1, -1 >( coordinates, origin, v1 );
         this -> getVertex< -1, 1 >( coordinates, origin, v2 );
      }
   }
   else // dx == 0
   {
      if( dy == 1 )
      {
         this -> getVertex< -1, 1 >( coordinates, origin, v1 );
         this -> getVertex< 1, 1 >( coordinates, origin, v2 );
      }
      else
      {
         this -> getVertex< 1, -1 >( coordinates, origin, v1 );
         this -> getVertex< -1, -1 >( coordinates, origin, v2 );
      }
   }
   normal. x() = v1. y() - v2. y();
   normal. y() = v2. x() - v1. x();
}

template< typename Real,
          typename Device,
          typename Index >
   template< Index dx, Index dy >
void tnlLinearGridGeometry< 2, Real, Device, Index > :: getVertex( const CoordinatesType& coordinates,
                                                                   const VertexType& origin,
                                                                   VertexType& vertex ) const
{
   tnlAssert( ( dx == 0 || dx == 1 || dx == -1 ||
                dy == 0 || dy == 1 || dy == -1 ), cerr << " dx = " << dx << " dy = " << dy << endl );
   vertex. x() = origin. x() + ( coordinates. x() + 0.5 * ( 1 + dx ) ) * parametricStep. x();
   vertex. y() = origin. y() + ( coordinates. y() + 0.5 * ( 1 + dy ) ) * parametricStep. y();
}

template< typename Real,
          typename Device,
          typename Index >
bool tnlLinearGridGeometry< 2, Real, Device, Index > :: save( tnlFile& file ) const
{
   if( ! this -> parametricStep. save( file ) )
      return false;
   return true;
};

template< typename Real,
          typename Device,
          typename Index >
bool tnlLinearGridGeometry< 2, Real, Device, Index > :: load( tnlFile& file )
{
   if( ! this -> parametricStep. load( file ) )
      return false;
   this -> elementMeasure = this -> parametricStep. x() * this -> parametricStep. y();
   return true;
};




#endif /* TNLLINEARGRIDGEOMETRY_IMPL_H_ */
