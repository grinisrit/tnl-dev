/***************************************************************************
                          compare-objects.h  -  description
                             -------------------
    begin                : 2009/08/14
    copyright            : (C) 2009 by Tomas Oberhuber
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

#ifndef compare_objectsH
#define compare_objectsH

#include <diff/tnlGrid2D.h>
#include <diff/tnlGrid3D.h>
#include <math.h>
#include <core/mfuncs.h>

template< typename REAL >
bool Compare( const tnlGrid2D< REAL >& u1,
              const tnlGrid2D< REAL >& u2,
              REAL& l1_norm,
              REAL& l2_norm,
              REAL& max_norm,
              tnlGrid2D< REAL >& difference,
              int edge_skip )
{
   if( u1. GetXSize() != u2. GetXSize() ||
       u1. GetYSize() != u2. GetYSize() ||
       u1. GetAx()    != u2. GetAx()    ||
       u1. GetAy()    != u2. GetAy()    ||
       u1. GetBx()    != u2. GetBx()    ||
       u1. GetBy()    != u2. GetBy()    )
   {
      cerr << "Both grids describes different numerical domain." << endl;
      cerr << u1. GetXSize() << "x"  << u1. GetYSize( ) << " on domain < "
           << u1. GetAx()    << ", " << u1. GetBx() << " > x < "
           << u1. GetAy()    << ", " << u1. GetBy() << " > VS. "
           << u2. GetXSize() << "x"  << u2. GetYSize( ) << " on domain "
           << u2. GetAx()    << ", " << u2. GetBx() << " > x < "
           << u2. GetAy()    << ", " << u2. GetBy() << " > ." << endl;
      return false;
   }
   const int x_size = u1. GetXSize();
   const int y_size = u1. GetYSize();
   const REAL& hx = u1. GetHx();
   const REAL& hy = u1. GetHy();

   int i, j;
   l1_norm = l2_norm = max_norm = 0.0;
   for( i = edge_skip; i < x_size - edge_skip; i ++ )
      for( j = edge_skip; j < y_size - edge_skip; j ++ )
      {
         REAL diff = u1( i, j ) - u2( i, j );
         REAL err = fabs( diff );
         l1_norm += hx * hy * err;
         l2_norm += hx * hy * err * err;
         max_norm = Max( max_norm, err );
         if( difference ) difference( i, j ) = diff;
      }
   l2_norm = sqrt( l2_norm );
   return true;
}

template< typename REAL >
bool Compare( const tnlGrid3D< REAL >& u1,
              const tnlGrid3D< REAL >& u2,
              REAL& l1_norm,
              REAL& l2_norm,
              REAL& max_norm,
              tnlGrid3D< REAL >& difference,
              int edge_skip )
{
   if( u1. GetXSize() != u2. GetXSize() ||
       u1. GetYSize() != u2. GetYSize() ||
       u1. GetZSize() != u2. GetZSize() ||
       u1. GetAx()    != u2. GetAx()    ||
       u1. GetAy()    != u2. GetAy()    ||
       u1. GetAz()    != u2. GetAz()    ||
       u1. GetBx()    != u2. GetBx()    ||
       u1. GetBy()    != u2. GetBy()    ||
       u1. GetBz()    != u2. GetBz()    )
   {
      cerr << "Both grids describes different numerical domain." << endl;
      return false;
   }
   const int x_size = u1. GetXSize();
   const int y_size = u1. GetYSize();
   const int z_size = u1. GetZSize();
   const REAL& hx = u1. GetHx();
   const REAL& hy = u1. GetHy();
   const REAL& hz = u1. GetHz();

   int i, j, k;
   l1_norm = l2_norm = max_norm = 0.0;
   for( i = edge_skip; i < x_size - edge_skip; i ++ )
      for( j = edge_skip; j < y_size - edge_skip; j ++ )
         for( k = edge_skip; k < z_size - edge_skip; k ++ )
         {
            REAL diff = u1( i, j, k ) - u2( i, j, k );
            REAL err = fabs( diff );
            l1_norm += hx * hy * hz * err;
            l2_norm += hx * hy * hz * err * err;
            max_norm = Max( max_norm, err );
            if( difference ) difference( i, j, k ) = diff;
         }
   l2_norm = sqrt( l2_norm );
   return true;
}



#endif
