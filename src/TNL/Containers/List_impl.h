/***************************************************************************
                          List_impl.h  -  description
                             -------------------
    begin                : Mar, 5 Apr 2016 12:46 PM
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/List.h>
#include <TNL/Math.h>

namespace TNL {
namespace Containers {

template< typename T >
List< T >::List()
   : first( 0 ),  last( 0 ), size( 0 ), iterator( 0 ), index( 0 )
{
}

template< typename T >
List< T >::List( const List& list )
   : first( 0 ), last( 0 ), size( 0 ), iterator( 0 ), index( 0 )
{
   AppendList( list );
}

template< typename T >
List< T >::~List()
{
   reset();
}

template< typename T >
String List< T >::getType()
{
   return String( "Containers::List< " ) + TNL::getType< T >() +  String( " >" );
}

template< typename T >
bool List< T >::isEmpty() const
{
   return ! size;
}
 
template< typename T >
int List< T >::getSize() const
{
   return size;
}

template< typename T >
T& List< T >::operator[]( const int& ind )
{
   TNL_ASSERT( ind < size, );
   int iter_dist = TNL::abs( index - ind );
   if( ! iterator ||
       iter_dist > ind ||
       iter_dist > size - ind )
   {
      if( ind < size - ind )
      {
         //cout << "Setting curent index to 0." << std::endl;
         index = 0;
         iterator = first;
      }
      else
      {
         //cout << "Setting curent index to size - 1." << std::endl;
         index = size - 1;
         iterator = last;
      }
   }
   while( index != ind )
   {
      //cout << " current index = " << index
      //     << " index = " << ind << std::endl;
      if( ind < index )
      {
         iterator = iterator -> Previous();
         index --;
      }
      else
      {
         iterator = iterator -> Next();
         index ++;
      }
      TNL_ASSERT( iterator, );
   }
   return iterator -> Data();
};
 
template< typename T >
const T& List< T >::operator[]( const int& ind ) const
{
   return const_cast< List< T >* >( this ) -> operator[]( ind );
}

template< typename T >
const List< T >& List< T >::operator = ( const List& lst )
{
   AppendList( lst );
   return( *this );
}

template< typename T >
bool List< T >::operator == ( const List& lst ) const
{
   if( this->getSize() != lst.getSize() )
      return false;
   for( int i = 0; i < this->getSize(); i++ )
      if( (*this)[ i ] != lst[ i ] )
         return false;
   return true;
}

template< typename T >
bool List< T >::operator != ( const List& lst ) const
{
   return ! operator==( lst );
}

template< typename T >
bool List< T >::Append( const T& data )
{
   if( ! first )
   {
      TNL_ASSERT( ! last, );
      first = last = new ListDataElement< T >( data );
   }
   else
   {
      ListDataElement< T >* new_element =  new ListDataElement< T >( data, last, 0 );
      TNL_ASSERT( last, );
      last = last -> Next() = new_element;
   }
   size ++;
   return true;
};

template< typename T >
bool List< T >::Prepend( const T& data )
{
   if( ! first )
   {
      TNL_ASSERT( ! last, );
      first = last = new ListDataElement< T >( data );
   }
   else
   {
      ListDataElement< T >* new_element =  new ListDataElement< T >( data, 0, first );
      first = first -> Previous() = new_element;
   }
   size ++;
   index ++;
   return true;
};

template< typename T >
bool List< T >::Insert( const T& data, const int& ind )
{
   TNL_ASSERT( ind <= size || ! size, );
   if( ind == 0 ) return Prepend( data );
   if( ind == size ) return Append( data );
   operator[]( ind );
   ListDataElement< T >* new_el =
      new ListDataElement< T >( data,
                             iterator -> Previous(),
                             iterator );
   iterator -> Previous() -> Next() = new_el;
   iterator -> Previous() = new_el;
   iterator = new_el;
   size ++;
   return true;
};

template< typename T >
bool List< T >::AppendList( const List< T >& lst )
{
   int i;
   for( i = 0; i < lst. getSize(); i ++ )
   {
      if( ! Append( lst[ i ] ) ) return false;
   }
   return true;
};
 
template< typename T >
bool List< T >::PrependList( const List< T >& lst )

{
   int i;
   for( i = lst. getSize(); i > 0; i -- )
      if( ! Prepend( lst[ i - 1 ] ) ) return false;
   return true;
};

template< typename T >
   template< typename Array >
void List< T >::toArray( Array& array )
{
   array.setSize( this->getSize() );
   for( int i = 0; i < this->getSize(); i++ )
      array[ i ] = ( *this )[ i ];
}
template< typename T >
bool List< T >::containsValue( const T& v ) const
{
   for( int i = 0; i < this->getSize(); i++ )
      if( ( *this )[ i ] == v )
         return true;
   return false;
}

template< typename T >
void List< T >::Erase( const int& ind )
{
   operator[]( ind );
   ListDataElement< T >* tmp_it = iterator;
   if( iterator -> Next() )
      iterator -> Next() -> Previous() = iterator -> Previous();
   if( iterator -> Previous() )
     iterator -> Previous() -> Next() = iterator -> Next();
   if( iterator -> Next() ) iterator = iterator -> Next();
   else
   {
      iterator = iterator -> Previous();
      index --;
   }
   if( first == tmp_it ) first = iterator;
   if( last == tmp_it ) last = iterator;
   delete tmp_it;
   size --;
};

template< typename T >
void List< T >::DeepErase( const int& ind )
{
   operator[]( ind );
   delete iterator -> Data();
   Erase( ind );
};

template< typename T >
void List< T >::reset()
{
   iterator = first;
   ListDataElement< T >* tmp_it;
   while( iterator )
   {
      TNL_ASSERT( iterator, );
      tmp_it = iterator;
      iterator = iterator -> Next();
      delete tmp_it;
   }
   first = last = 0;
   size = 0;
};

template< typename T >
void List< T >::DeepEraseAll()
{
   iterator = first;
   ListDataElement< T >* tmp_it;
   int i( 0 );
   while( iterator )
   {
      tmp_it = iterator;
      iterator = iterator -> Next();
      delete tmp_it -> Data();
      delete tmp_it;
      i++;
   }
   first = last = 0;
   size = 0;
};
 
template< typename T >
bool List< T >::Save( File& file ) const
{
   file.write( &size );
   for( int i = 0; i < size; i ++ )
      if( ! file. write( &operator[]( i ), 1 ) )
         return false;
   return true;
}

template< typename T >
bool List< T >::DeepSave( File& file ) const
{
   file. write( &size );
   for( int i = 0; i < size; i ++ )
      if( ! operator[]( i ). save( file ) ) return false;
   return true;
}

template< typename T >
bool List< T >::Load( File& file )
{
   reset();
   int _size;
   file. read( &_size, 1 );
   if( _size < 0 )
   {
      std::cerr << "The curve size is negative." << std::endl;
      return false;
   }
   T t;
   for( int i = 0; i < _size; i ++ )
   {
      if( ! file. read( &t, 1 ) )
         return false;
      Append( t );
   }
   return true;
};

template< typename T >
bool List< T >::DeepLoad( File& file )
{
   reset();
   int _size;
   file. read( &_size );
   if( _size < 0 )
   {
      std::cerr << "The list size is negative." << std::endl;
      return false;
   }
   for( int i = 0; i < _size; i ++ )
   {
      T t;
      if( ! t. load( file ) ) return false;
      Append( t );
   }
   return true;
};
 
template< typename T >
std::ostream& operator << ( std::ostream& str, const List< T >& list )
{
   int i, size( list. getSize() );
   for( i = 0; i < size; i ++ )
      str << "Item " << i << ":" << list[ i ] << std::endl;
   return str;
};

} // namespace Containers
} // namespace TNL
