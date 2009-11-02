/***************************************************************************
                          mVector.h  -  description
                             -------------------
    begin                : 2006/03/04
    copyright            : (C) 2006 by Tom� Oberhuber
    email                : oberhuber@seznam.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef mVectorH
#define mVectorH

#include <assert.h>
#include <string.h>
#include <sstream>
#include "param-types.h"

template< int SIZE, typename T > class mVector
{
   public:

   mVector()
   {
      bzero( Data(), SIZE * sizeof( T ) );
   };

   mVector( const T v[ SIZE ] )
   {
      memcpy( &data[ 0 ], &v[ 0 ], SIZE * sizeof( T ) ); 
      /*int i;
      for( i = 0; i < SIZE; i ++ )
         data[ i ] = v[ i ];*/
   };

   mVector( const T& v )
   {
      memcpy( &data[ 0 ], &v. data[ 0 ], SIZE * sizeof( T ) ); 
      /*int i;
      for( i = 0; i < SIZE; i ++ )
         data[ i ] = v. data[ i ];*/
   }

   const T* Data() const
   {
      return &data[ 0 ];
   };

   T* Data()
   {
      return &data[ 0 ];
   };

   const T& operator[]( int i ) const
   {
      assert( i >= 0 && i < SIZE );
      return data[ i ];
   };

   T& operator[]( int i )
   {
      assert( i < SIZE );
      return data[ i ];
   };
   
   //! Adding operator
   mVector& operator += ( const mVector& v )
   {
      int i;
      for( i = 0; i < SIZE; i ++ )
         data[ i ] += v. data[ i ];
   };

   //! Subtracting operator
   mVector& operator -= ( const mVector& v )
   {
      int i;
      for( i = 0; i < SIZE; i ++ )
         data[ i ] -= v. data[ i ];
   };

   //! Multiplication with number
   mVector& operator *= ( const T& c )
   {
      int i;
      for( i = 0; i < SIZE; i ++ )
         data[ i ] *= c;
   };

   //! Adding operator
   mVector operator + ( const mVector& u ) const
   {
      // TODO: Leads to sigsegv 
      return mVector( * this ) += u;
   };

   //! Subtracting operator
   mVector operator - ( const mVector& u ) const
   {
      // TODO: Leads to sigsegv 
      return mVector( * this ) -= u; 
   };

   //! Multiplication with number
   mVector operator * ( const T& c ) const
   { 
      return mVector( * this ) *= c; 
   };

   //! 
   mVector& operator = ( const mVector& v )
   {
      memcpy( &data[ 0 ], &v. data[ 0 ], SIZE * sizeof( T ) ); 
      /*int i;
      for( i = 0; i < SIZE; i ++ )
         data[ i ] = v. data[ i ];*/
      return *this;
   };

   //! Scalar product
   T operator * ( const mVector& u ) const
   { 
      int i;
      T res( 0.0 );
      for( i = 0; i < SIZE; i ++ )
         res += data[ i ] * u. data[ i ];
      return res;
   };

   //! Comparison operator
   bool operator == ( const mVector& v ) const
   { 
      int i;
      for( i = 0; i < SIZE; i ++ )
         if( data[ i ] != v. data[ i ] ) 
            return false;
      return true;
   };

   //! 
   bool operator!= ( const mVector& v ) const 
   { 
      int i;
      for( i = 0; i < SIZE; i ++ )
         if( data[ i ] != v. data[ i ] ) 
            return true;
      return false;
   };

   protected:
   T data[ SIZE ];

};

template< int SIZE, typename T > bool Save( ostream& file, const mVector< SIZE, T >& vec )
{
   file. write( ( char* ) vec. Data(), SIZE * sizeof( T ) );
   if( file. bad() ) return false;
   return true;
};
   
template< int SIZE, typename T > bool Load( istream& file, mVector< SIZE, T >& vec ) 
{
   file. read( ( char* ) vec. Data(), SIZE * sizeof( T ) );
   if( file. bad() ) return false;
   return true;
};

template< int SIZE, typename T > ostream& operator << ( ostream& str, mVector< SIZE, T > v )
{
   int i;
   for( i = 0; i < SIZE - 1; i ++ )
      str << v[ i ] << ", ";
   str << v[ SIZE - 1 ];
   return str;
};

template< int SIZE, typename T > mString GetParameterType( const mVector< SIZE, T >& )
{ 
   T t;
   stringstream str;
   str << "mVector< " << SIZE << ", " << GetParameterType( t ) << " >";
   return mString( str. str(). data() ); 
};

#endif
