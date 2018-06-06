/***************************************************************************
                          UnorderedIndexedSet_impl.h  -  description
                             -------------------
    begin                : Feb 15, 2014
    copyright            : (C) 2014 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/UnorderedIndexedSet.h>

namespace TNL {
namespace Containers {

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
void
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::clear()
{
   map.clear();
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
typename UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::size_type
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::size() const
{
   return map.size();
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
Index
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::insert( const Key& key )
{
   auto iter = map.insert( value_type( key, size() ) ).first;
   return iter->second;
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
std::pair< Index, bool >
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::try_insert( const Key& key )
{
   auto pair = map.insert( value_type( key, size() ) );
   return std::pair< Index, bool >{ pair.first->second, pair.second };
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
bool
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::find( const Key& key, Index& index ) const
{
   auto iter = map.find( Key( key ) );
   if( iter == map.end() )
      return false;
   index = iter->second.index;
   return true;
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
typename UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::size_type
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::count( const Key& key ) const
{
   return map.count( key );
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
typename UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::size_type
UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::erase( const Key& key )
{
   return map.erase( key );
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
void UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >::print( std::ostream& str ) const
{
   auto iter = map.begin();
   str << iter->second.data;
   iter++;
   while( iter != map.end() )
   {
      str << ", " << iter->second.data;
      iter++;
   }
}

template< class Key,
          class Index,
          class Hash,
          class KeyEqual,
          class Allocator >
std::ostream& operator<<( std::ostream& str, UnorderedIndexedSet< Key, Index, Hash, KeyEqual, Allocator >& set )
{
   set.print( str );
   return str;
}

} // namespace Containers
} // namespace TNL