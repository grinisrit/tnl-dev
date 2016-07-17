/***************************************************************************
                          tnlContainer_impl.h  -  description
                             -------------------
    begin                : Feb 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

namespace TNL {

template< typename Element, typename Device, typename Index >
tnlContainer< Element, Device, Index >::tnlContainer()
{
}

template< typename Element, typename Device, typename Index >
tnlContainer< Element, Device, Index >::tnlContainer( const IndexType size )
{
   this->setSize( size );
}

template< typename Element, typename Device, typename Index >
tnlString tnlContainer< Element, Device, Index >::getType()
{
   return tnlString( "tnlContainer< " ) +
                     ::getType< Element >() +
                     Device :: getDeviceType() +
                     ::getType< Index >() +
                     " >";
}

template< typename Element, typename Device, typename Index >
bool tnlContainer< Element, Device, Index >::setSize( const IndexType size )
{
   return this->data.setSize( size );
}

template< typename Element, typename Device, typename Index >
Index tnlContainer< Element, Device, Index >::getSize() const
{
   return this->data.getSize();
}

template< typename Element, typename Device, typename Index >
void tnlContainer< Element, Device, Index >::reset()
{
   this->data.reset();
}

template< typename Element, typename Device, typename Index >
Element& tnlContainer< Element, Device, Index >::operator[]( const IndexType id )
{
   return this->data[ id ];
}

template< typename Element, typename Device, typename Index >
const Element& tnlContainer< Element, Device, Index >::operator[]( const IndexType id ) const
{
   return this->data[ id ];
}

template< typename Element, typename Device, typename Index >
Element tnlContainer< Element, Device, Index >::getElement( const IndexType id ) const
{
   return this->data.getElement( id );
}

template< typename Element, typename Device, typename Index >
void tnlContainer< Element, Device, Index >::setElement( const IndexType id,
                                                 const ElementType& data )
{
   this->data.setElement( id, data );
}

template< typename Element, typename Device, typename Index >
bool tnlContainer< Element, Device, Index >::save( tnlFile& file ) const
{
   if( ! tnlObject::save( file ) ||
       ! this->data.save( file ) )
   {
      cerr << "I am no able to save " << this->getType() << "." << endl;
      return false;
   }
   return true;
}

template< typename Element, typename Device, typename Index >
bool tnlContainer< Element, Device, Index >::load( tnlFile& file )
{
   if( ! tnlObject::load( file ) ||
       ! this->data.load( file ) )
   {
      cerr << "I am no able to load " << this->getType() << "." << endl;
      return false;
   }
   return true;
}

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

// TODO: this does not work with CUDA 5.5 - fix it later

/*extern template class tnlContainer< float, tnlHost, int >;
extern template class tnlContainer< double, tnlHost, int >;
extern template class tnlContainer< float, tnlHost, long int >;
extern template class tnlContainer< double, tnlHost, long int >;*/

#ifdef HAVE_CUDA
/*extern template class tnlContainer< float, tnlCuda, int >;
extern template class tnlContainer< double, tnlCuda, int >;
extern template class tnlContainer< float, tnlCuda, long int >;
extern template class tnlContainer< double, tnlCuda, long int >;*/
#endif

#endif

} // namespace TNL
