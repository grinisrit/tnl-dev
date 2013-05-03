/***************************************************************************
                          tnlIdenticalGridGeometry.h  -  description
                             -------------------
    begin                : Apr 28, 2013
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

#ifndef TNLIDENTICALGRIDGEOMETRY_H_
#define TNLIDENTICALGRIDGEOMETRY_H_

#include <core/tnlHost.h>

template< int Dimensions,
          typename Real = double,
          typename Device = tnlHost,
          typename Index = int >
class tnlIdenticalGridGeometry
{
};

template< typename Real,
          typename Device,
          typename Index >
class tnlIdenticalGridGeometry< 1, Real, Device, Index >
{
   public:

   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;

   void setParametricStep( const tnlTuple< 1, Real >& parametricStep );

   const tnlTuple< 1, Real >& getParametricStep() const;

   void getElementCenter( const tnlTuple< 1, Real >& origin,
                          const tnlTuple< 1, Index >& coordinates,
                          tnlTuple< 1, Real >& center ) const;


   Real getElementMeasure( const Index i ) const;

   template< Index dx >
   Real getElementsDistance( const Index i ) const;

   template< Index dx >
   void getEdgeCoordinates( const Index i,
                            const tnlTuple< 1, Real >& origin,
                            tnlTuple< 1, Real >& coordinates ) const;

   template< Index dx >
   Real getEdgeLength( const Index i ) const;

   template< Index dx >
   void getEdgeNormal( const Index i,
                       tnlTuple< 1, Real >& normal ) const;

   void getVertexCoordinates( const Index i,
                              const tnlTuple< 1, Real >& origin,
                              tnlTuple< 1, Real >& coordinates ) const;

   bool save( tnlFile& file ) const;

   bool load( tnlFile& file );

   protected:

   tnlTuple< 1, Real > parametricStep;

   Real elementMeasure;
};

template< typename Real,
          typename Device,
          typename Index >
class tnlIdenticalGridGeometry< 2, Real, Device, Index >
{
   public:

   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;

   void setParametricStep( const tnlTuple< 2, Real >& parametricStep );

   const tnlTuple< 2, Real >& getParametricStep() const;

   void getElementCenter( const tnlTuple< 2, Real >& origin,
                          const tnlTuple< 2, Index >& coordinates,
                          tnlTuple< 2, Real >& center ) const;

   Real getElementMeasure( const tnlTuple< 2, Index >& coordinates ) const;

   template< Index dx, Index dy >
   Real getElementsDistance( const Index i,
                             const Index j ) const;

   template< Index dx, Index dy >
   void getEdgeCoordinates( const Index i,
                            const Index j,
                            const tnlTuple< 2, Real >& origin,
                            tnlTuple< 2, Real >& coordinates ) const;

   template< Index dx, Index dy >
   Real getEdgeLength( const Index i,
                       const Index j ) const;

   template< Index dx, Index dy >
   void getEdgeNormal( const Index i,
                       const Index j,
                       tnlTuple< 2, Real >& normal ) const;

   void getVertexCoordinates( const Index i,
                              const Index j,
                              const tnlTuple< 2, Real >& origin,
                              tnlTuple< 2, Real >& coordinates ) const;

   bool save( tnlFile& file ) const;

   bool load( tnlFile& file );

   protected:

   tnlTuple< 2, Real > parametricStep;

   Real elementMeasure;
};

#include <implementation/mesh/tnlIdenticalGridGeometry_impl.h>

#endif /* TNLIDENTICALGRIDGEOMETRY_H_ */
