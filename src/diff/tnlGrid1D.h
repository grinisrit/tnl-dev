/***************************************************************************
                          tnlGrid1D.h  -  description
                             -------------------
    begin                : 2007/11/26
    copyright            : (C) 2007 by Tomas Oberhuber
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

#ifndef tnlGrid1DH
#define tnlGrid1DH

#include <core/tnlField1D.h>

template< typename T = double > class tnlGrid1D : public tnlField1D< T >
{
   public:

   tnlGrid1D( const char* name = 0 )
   : tnlField1D< T >( name )
   {
   };

   //! Constructor with the grid and the domain dimensions
   /*! @param x_size and @param y_size define the grid dimensions.
       @param A_x, @param B_x, @param A_y and @param B_y define domain
       Omega = <A_x,B_x>*<A_y,B_y>.
    */
   tnlGrid1D( const char* name,
              int x_size,
              const double& A_x,
              const double& B_x )
   : tnlField1D< T >( name, x_size ),
     Ax( A_x ), Bx( B_x ) 
   {
      assert( Ax < Bx );
      Hx = ( Bx - Ax ) / ( double ) ( x_size - 1 ); 
   };

   tnlGrid1D( const tnlGrid1D& g )
   : tnlField1D< T >( g ),
     Ax( g. Ax ), Bx( g. Bx ),
     Hx( g. Hx )
   { };

   tnlString GetType() const
   {
      T t;
      return tnlString( "tnlGrid1D< " ) + tnlString( GetParameterType( t ) ) + tnlString( " >" );
   };

   void SetNewDomain( const double& A_x,
                      const double& B_x,
                      double hx = 0.0 )
   {
      Ax = A_x;
      Bx = B_x;
      assert( Ax < Bx );
      if( ! hx ) Hx = ( Bx - Ax ) / ( double ) ( tnlField1D< T > :: GetXSize() - 1 ); 
      else Hx = hx;
   }
   ;
   void SetNewDomain( const tnlGrid1D< T >& u )
   {
      SetNewDomain( u. GetAx(),
                    u. GetBx() );
   };

   const double& GetAx() const
   {
      return Ax;
   }

   const double& GetBx() const
   {
      return Bx;
   }

   const double& GetHx() const
   {
      return Hx;
   }

   // Interpolation
   template< typename N > T Value( N x ) const
   {
      x = ( x - Ax ) / Hx;
      int ix = ( int ) ( x );
      N dx = x - ( N ) ix;
      assert( 0 );
   }
   
   //! Forward difference w.r.t x
   T Partial_x_f( const int i ) const
   {
      assert( i >= 0 );
      assert( i < tnlField1D< T > :: x_size - 1 );
      return ( tnlField1D< T > :: operator()( i + 1 ) - 
               tnlField1D< T > ::  operator()( i ) ) / Hx;
   };
   
   //! Backward difference w.r.t x
   T Partial_x_b( const int i ) const
   {
      assert( i > 0 );
      assert( i < tnlField1D< T > :: x_size );
      return ( tnlField1D< T > :: operator()( i ) - 
               tnlField1D< T > :: operator()( i - 1 ) ) / Hx;
   };

   //! Central difference w.r.t. x
   T Partial_x( const int i ) const
   {
      assert( i > 0 );
      assert( i < tnlField1D< T > :: x_size - 1 );
      return ( tnlField1D< T > :: operator()( i + 1 ) - 
               tnlField1D< T > :: operator()( i - 1 ) ) / ( 2.0 * Hx );
   };
   
   //! Second order difference w.r.t. x
   T Partial_xx( const int i ) const
   {
      assert( i > 0 );
      assert( i < tnlField1D< T > :: x_size - 1 );
      return ( tnlField1D< T > :: operator()( i + 1 ) - 
               2.0 * tnlField1D< T > :: operator()( i ) + 
               tnlField1D< T > :: operator()( i - 1 ) ) / ( Hx * Hx );   
   };
   
   //! Method for saving the object to a file as a binary data
   bool Save( ostream& file ) const
   {
      if( ! tnlField1D< T > :: Save( file ) ) return false;
      file. write( ( char* ) &Ax, sizeof( double ) );
      file. write( ( char* ) &Bx, sizeof( double ) );
      file. write( ( char* ) &Hx, sizeof( double ) );
      if( file. bad() ) return false;
      return true;
   };

   //! Method for restoring the object from a file
   bool Load( istream& file )
   {
      if( ! tnlField1D< T > :: Load( file ) ) return false;
      file. read( ( char* ) &Ax, sizeof( double ) );
      file. read( ( char* ) &Bx, sizeof( double ) );
      file. read( ( char* ) &Hx, sizeof( double ) );
      if( file. bad() ) return false;
      return true;
   };   

   protected:

   //! Domain dimensions
   double Ax, Bx;

   //! The grid space steps
   double Hx;
};

// Explicit instatiation
template class tnlGrid1D< double >;

#endif
