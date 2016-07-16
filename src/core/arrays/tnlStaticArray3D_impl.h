/***************************************************************************
                          tnlStaticArray3D_impl.h  -  description
                             -------------------
    begin                : Feb 10, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLSTATICARRAY3D_IMPL_H_
#define TNLSTATICARRAY3D_IMPL_H_

#include <core/param-types.h>

template< typename Element >
__cuda_callable__
inline tnlStaticArray< 3, Element >::tnlStaticArray()
{
}

template< typename Element >
__cuda_callable__
inline tnlStaticArray< 3, Element >::tnlStaticArray( const Element v[ size ] )
{
   data[ 0 ] = v[ 0 ];
   data[ 1 ] = v[ 1 ];
   data[ 2 ] = v[ 2 ];
}

template< typename Element >
__cuda_callable__
inline tnlStaticArray< 3, Element >::tnlStaticArray( const Element& v )
{
   data[ 0 ] = v;
   data[ 1 ] = v;
   data[ 2 ] = v;
}

template< typename Element >
__cuda_callable__
inline tnlStaticArray< 3, Element >::tnlStaticArray( const Element& v1, const Element& v2, const Element& v3 )
{
   data[ 0 ] = v1;
   data[ 1 ] = v2;
   data[ 2 ] = v3;
}

template< typename Element >
__cuda_callable__
inline tnlStaticArray< 3, Element >::tnlStaticArray( const tnlStaticArray< size, Element >& v )
{
   data[ 0 ] = v[ 0 ];
   data[ 1 ] = v[ 1 ];
   data[ 2 ] = v[ 2 ];
}

template< typename Element >
tnlString tnlStaticArray< 3, Element >::getType()
{
   return tnlString( "tnlStaticArray< " ) +
          tnlString( size ) +
          tnlString( ", " ) +
          ::getType< Element >() +
          tnlString( " >" );
}

template< typename Element >
inline int tnlStaticArray< 3, Element >::getSize() const
{
   return size;
}

template< typename Element >
__cuda_callable__
inline Element* tnlStaticArray< 3, Element >::getData()
{
   return data;
}

template< typename Element >
__cuda_callable__
inline const Element* tnlStaticArray< 3, Element >::getData() const
{
   return data;
}

template< typename Element >
__cuda_callable__
inline const Element& tnlStaticArray< 3, Element >::operator[]( int i ) const
{
   tnlAssert( i >= 0 && i < size,
            cerr << "i = " << i << " size = " << size << endl; );
   return data[ i ];
}

template< typename Element >
__cuda_callable__
inline Element& tnlStaticArray< 3, Element >::operator[]( int i )
{
   tnlAssert( i >= 0 && i < size,
            cerr << "i = " << i << " size = " << size << endl; );
   return data[ i ];
}

template< typename Element >
__cuda_callable__
inline Element& tnlStaticArray< 3, Element >::x()
{
   return data[ 0 ];
}

template< typename Element >
__cuda_callable__
inline const Element& tnlStaticArray< 3, Element >::x() const
{
   return data[ 0 ];
}

template< typename Element >
__cuda_callable__
inline Element& tnlStaticArray< 3, Element >::y()
{
   return data[ 1 ];
}

template< typename Element >
__cuda_callable__
inline const Element& tnlStaticArray< 3, Element >::y() const
{
   return data[ 1 ];
}

template< typename Element >
__cuda_callable__
inline Element& tnlStaticArray< 3, Element >::z()
{
   return data[ 2 ];
}

template< typename Element >
__cuda_callable__
inline const Element& tnlStaticArray< 3, Element >::z() const
{
   return data[ 2 ];
}
template< typename Element >
__cuda_callable__
tnlStaticArray< 3, Element >& tnlStaticArray< 3, Element >::operator = ( const tnlStaticArray< 3, Element >& array )
{
   data[ 0 ] = array[ 0 ];
   data[ 1 ] = array[ 1 ];
   data[ 2 ] = array[ 2 ];
   return *this;
}

template< typename Element >
   template< typename Array >
__cuda_callable__
tnlStaticArray< 3, Element >& tnlStaticArray< 3, Element >::operator = ( const Array& array )
{
   data[ 0 ] = array[ 0 ];
   data[ 1 ] = array[ 1 ];
   data[ 2 ] = array[ 2 ];
   return *this;
}

template< typename Element >
   template< typename Array >
__cuda_callable__
bool tnlStaticArray< 3, Element >::operator == ( const Array& array ) const
{
   return( ( int ) size == ( int ) Array::size &&
           data[ 0 ] == array[ 0 ] &&
           data[ 1 ] == array[ 1 ] &&
           data[ 2 ] == array[ 2 ] );
}

template< typename Element >
   template< typename Array >
__cuda_callable__
bool tnlStaticArray< 3, Element >::operator != ( const Array& array ) const
{
   return ! this->operator == ( array );
}

template< typename Element >
   template< typename OtherElement >
__cuda_callable__
tnlStaticArray< 3, Element >::
operator tnlStaticArray< 3, OtherElement >() const
{
   tnlStaticArray< 3, OtherElement > aux;
   aux[ 0 ] = data[ 0 ];
   aux[ 1 ] = data[ 1 ];
   aux[ 2 ] = data[ 2 ];
   return aux;
}

template< typename Element >
__cuda_callable__
void tnlStaticArray< 3, Element >::setValue( const ElementType& val )
{
   data[ 2 ] = data[ 1 ] = data[ 0 ] = val;
}

template< typename Element >
bool tnlStaticArray< 3, Element >::save( tnlFile& file ) const
{
   if( ! file. write< Element, tnlHost, int >( data, size ) )
   {
      cerr << "Unable to write " << getType() << "." << endl;
      return false;
   }
   return true;
}

template< typename Element >
bool tnlStaticArray< 3, Element >::load( tnlFile& file)
{
   if( ! file.read< Element, tnlHost, int >( data, size ) )
   {
      cerr << "Unable to read " << getType() << "." << endl;
      return false;
   }
   return true;
}

template< typename Element >
void tnlStaticArray< 3, Element >::sort()
{
   /****
    * Bubble sort on three elements
    */
   if( data[ 0 ] > data[ 1 ] )
      Swap( data[ 0 ], data[ 1 ] );
   if( data[ 1 ] > data[ 2 ] )
      Swap( data[ 1 ], data[2  ] );
   if( data[ 0 ] > data[ 1 ] )
      Swap( data[ 0 ], data[ 1 ] );
}

template< typename Element >
ostream& tnlStaticArray< 3, Element >::write( ostream& str, const char* separator ) const
{
   str << data[ 0 ] << separator << data[ 1 ] << separator << data[ 2 ];
   return str;
}


#ifdef UNDEF //TEMPLATE_EXPLICIT_INSTANTIATION

// TODO: it does not work with CUDA

#ifndef HAVE_CUDA
extern template class tnlStaticArray< 3, char >;
extern template class tnlStaticArray< 3, int >;
#ifdef INSTANTIATE_LONG_INT
extern template class tnlStaticArray< 3, long int >;
#endif
#ifdef INSTANTIATE_FLOAT
extern template class tnlStaticArray< 3, float >;
#endif
extern template class tnlStaticArray< 3, double >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class tnlStaticArray< 3, long double >;
#endif
#endif

#endif


#endif /* TNLSTATICARRAY3D_IMPL_H_ */
