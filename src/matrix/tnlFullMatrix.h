/***************************************************************************
                          tnlFullMatrix.h  -  description
                             -------------------
    begin                : 2007/07/23
    copyright            : (C) 2007 by Tomá¹ Oberhuber
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

#ifndef tnlFullMatrixH
#define tnlFullMatrixH

#include <core/tnlField2D.h>
#include <matrix/tnlBaseMatrix.h>


template< typename T > class tnlFullMatrix : public tnlBaseMatrix< T >, public tnlField2D< T >
{

   public:

   //! Basic constructor
   tnlFullMatrix(){};

   //! Constructor with matrix dimension
   tnlFullMatrix( const int size )
   : tnlField2D< T >( size, size ){};

   tnlString GetType() const
   {
      T t;
      return tnlString( "tnlFullMatrix< " ) + tnlString( GetParameterType( t ) ) + tnlString( " >" );
   };

   const tnlString& GetMatrixClass() const
   {
      return tnlMatrixClass :: main;
   };

   int GetSize() const
   {
      return tnlField2D< T > :: GetXSize(); // it is the same as GetYSize()
   };
   
   T GetElement( int i, int j ) const
   {
      return ( *this )( i, j );
   };

   bool SetElement( int i, int j, const T& v )
   {
      ( *this )( i, j ) = v;
      return true;
   };

   bool AddToElement( int i, int j, const T& v )
   {
      ( *this )( i, j ) += v;
      return true;
   };

   //! Row product
   /*! Compute product of given vector with given row
    */
   T RowProduct( const int row, const T* vec ) const
   {
      const int size = GetSize();
      int pos = row * size;
      const T* data = tnlField2D< T > :: Data();
      T res( 0.0 );
      int i;
      for( i = 0; i < size; i ++ )
      {
         res += data[ pos ] * vec[ i ];
         pos ++; 
      }
      return res;
   };

   //! Vector product
   void VectorProduct( const T* vec, T* result ) const
   {
      const int size = GetSize();
      int pos( 0 );
      const T* data = tnlField2D< T > :: Data();
      T res;
      int i, j;
      for( i = 0; i < size; i ++ )
      {
         res = 0.0;
         for( j = 0; j < size; j ++ )
         {
            res += data[ pos ] * vec[ j ];
            pos ++; 
         }
         result[ i ] = res;
      }
   };


   //! Multiply row
   void MultiplyRow( const int row, const T& c )
   {
      const int size = GetSize();
      T* data = tnlField2D< T > :: Data();
      int i;
      int pos = row * size;
      for( i = 0; i < size; i ++ )
      {
         data[ pos + i ] *= c;
      }
   };

   //! Get row L1 norm
   T GetRowL1Norm( const int row ) const
   {
      const int size = GetSize();
      const T* data = tnlField2D< T > :: Data();
      T res( 0.0 );
      int i;
      int pos = row * size;
      for( i = 0; i < size; i ++ )
         res += fabs( data[ pos + i ] );
      return res;
   };

   //! Destructor
   ~tnlFullMatrix(){};
};

//! Matrix product
template< typename T > void MatrixProduct( const tnlFullMatrix< T >& m1,
                                           const tnlFullMatrix< T >& m2,
                                           tnlFullMatrix< T >& result )
{
   assert( m1. GetSize() == m2. GetSize() && m2. GetSize() == result. GetSize() );
   int size = result. GetSize();
   int i, j, k;
   for( i = 0; i < size; i ++ )
      for( j = 0; j < size; j ++ )
      {
         T res( 0.0 );
         for( k = 0; k < size; k ++ )
            res += m1( i, k ) * m2( k, j ); 
         result( i, j ) = res;
      }
};

//! Matrix sum
template< typename T > void MatrixSum( const tnlFullMatrix< T >& m1,
                                       const tnlFullMatrix< T >& m2,
                                       tnlFullMatrix< T >& result )
{
   assert( m1. GetSize() == m2. GetSize() && m2. GetSize() == result. GetSize() );
   int size = result. GetSize();
   int i,j;
   for( i = 0; i < size; i ++ )
      for( j = 0; j < size; j ++ )
         result( i, j ) = m1( i, j ) + m2( i, j );
};

//! Operator <<
template< typename T > ostream& operator << ( ostream& o_str, const tnlFullMatrix< T >& A )
{
   return operator << ( o_str, ( const tnlBaseMatrix< T >& ) A );
};

#endif