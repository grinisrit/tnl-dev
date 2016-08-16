/***************************************************************************
                          StaticArray_impl.h  -  description
                             -------------------
    begin                : Feb 10, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/core/param-types.h>
#include <TNL/core/mfuncs.h>

namespace TNL {
namespace Containers {   

template< int Size, typename Element >
__cuda_callable__
inline StaticArray< Size, Element >::StaticArray()
{
};

template< int Size, typename Element >
__cuda_callable__
inline StaticArray< Size, Element >::StaticArray( const Element v[ Size ] )
{
   for( int i = 0; i < Size; i++ )
      data[ i ] = v[ i ];
}

template< int Size, typename Element >
__cuda_callable__
inline StaticArray< Size, Element >::StaticArray( const Element& v )
{
   for( int i = 0; i < Size; i++ )
      data[ i ] = v;
}

template< int Size, typename Element >
__cuda_callable__
inline StaticArray< Size, Element >::StaticArray( const StaticArray< Size, Element >& v )
{
   for( int i = 0; i < Size; i++ )
      data[ i ] = v[ i ];
}

template< int Size, typename Element >
String StaticArray< Size, Element >::getType()
{
   return String( "Containers::StaticArray< " ) +
          String( Size ) +
          String( ", " ) +
          TNL::getType< Element >() +
          String( " >" );
}

template< int Size, typename Element >
__cuda_callable__
inline int StaticArray< Size, Element >::getSize() const
{
   return size;
}

template< int Size, typename Element >
__cuda_callable__
inline Element* StaticArray< Size, Element >::getData()
{
   return data;
}

template< int Size, typename Element >
__cuda_callable__
inline const Element* StaticArray< Size, Element >::getData() const
{
   return data;
}

template< int Size, typename Element >
__cuda_callable__
inline const Element& StaticArray< Size, Element >::operator[]( int i ) const
{
   Assert( i >= 0 && i < size,
            std::cerr << "i = " << i << " size = " << size << std::endl; );
   return data[ i ];
}

template< int Size, typename Element >
__cuda_callable__
inline Element& StaticArray< Size, Element >::operator[]( int i )
{
   Assert( i >= 0 && i < size,
            std::cerr << "i = " << i << " size = " << size << std::endl; );
   return data[ i ];
}

template< int Size, typename Element >
__cuda_callable__
inline StaticArray< Size, Element >& StaticArray< Size, Element >::operator = ( const StaticArray< Size, Element >& array )
{
   for( int i = 0; i < size; i++ )
      data[ i ] = array[ i ];
   return *this;
}

template< int Size, typename Element >
   template< typename Array >
__cuda_callable__
inline StaticArray< Size, Element >& StaticArray< Size, Element >::operator = ( const Array& array )
{
   for( int i = 0; i < size; i++ )
      data[ i ] = array[ i ];
   return *this;
}

template< int Size, typename Element >
   template< typename Array >
__cuda_callable__
inline bool StaticArray< Size, Element >::operator == ( const Array& array ) const
{
   if( ( int ) size != ( int ) Array::size )
      return false;
   for( int i = 0; i < size; i++ )
      if( data[ i ] != array[ i ] )
         return false;
   return true;
}

template< int Size, typename Element >
   template< typename Array >
__cuda_callable__
inline bool StaticArray< Size, Element >::operator != ( const Array& array ) const
{
   return ! this->operator == ( array );
}

template< int Size, typename Element >
   template< typename OtherElement >
__cuda_callable__
StaticArray< Size, Element >::
operator StaticArray< Size, OtherElement >() const
{
   StaticArray< Size, OtherElement > aux;
   for( int i = 0; i < Size; i++ )
      aux[ i ] = data[ i ];
   return aux;
}

template< int Size, typename Element >
__cuda_callable__
inline void StaticArray< Size, Element >::setValue( const ElementType& val )
{
   for( int i = 0; i < Size; i++ )
      data[ i ] = val;
}

template< int Size, typename Element >
bool StaticArray< Size, Element >::save( File& file ) const
{
   if( ! file. write< Element, Devices::Host, int >( data, size ) )
   {
      std::cerr << "Unable to write " << getType() << "." << std::endl;
      return false;
   }
   return true;
}

template< int Size, typename Element >
bool StaticArray< Size, Element >::load( File& file)
{
   if( ! file.read< Element, Devices::Host, int >( data, size ) )
   {
      std::cerr << "Unable to read " << getType() << "." << std::endl;
      return false;
   }
   return true;
}

template< int Size, typename Element >
void StaticArray< Size, Element >::sort()
{
   /****
    * We assume that the array data is small and so
    * may sort it with the bubble sort.
    */
   for( int k = Size - 1; k > 0; k--)
      for( int i = 0; i < k; i++ )
         if( data[ i ] > data[ i+1 ] )
            swap( data[ i ], data[ i+1 ] );
}

template< int Size, typename Element >
std::ostream& StaticArray< Size, Element >::write( std::ostream& str, const char* separator ) const
{
   for( int i = 0; i < Size - 1; i++ )
      str << data[ i ] << separator;
   str << data[ Size - 1 ];
   return str;
}


template< int Size, typename Element >
std::ostream& operator << ( std::ostream& str, const StaticArray< Size, Element >& a )
{
   a.write( str, "," );
   /*for( int i = 0; i < Size - 1; i ++ )
   {
      str << a[ i ] << ", ";
   }
   str << a[ Size - 1 ];*/
   return str;
};

#ifdef UNDEF //TEMPLATE_EXPLICIT_INSTANTIATION

// TODO: it does not work with CUDA

#ifndef HAVE_CUDA
extern template class StaticArray< 4, char >;
extern template class StaticArray< 4, int >;
#ifdef INSTANTIATE_LONG_INT
extern template class StaticArray< 4, long int >;
#endif
#ifdef INSTANTIATE_FLOAT
extern template class StaticArray< 4, float >;
#endif
extern template class StaticArray< 4, double >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class StaticArray< 4, long double >;
#endif
#endif

#endif

} // namespace Containers
} // namespace TNL