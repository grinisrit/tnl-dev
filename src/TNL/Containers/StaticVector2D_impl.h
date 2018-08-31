/***************************************************************************
                          StaticVector2D_impl.h  -  description
                             -------------------
    begin                : Feb 10, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once 

#include <TNL/Containers/StaticVector.h>

namespace TNL {
namespace Containers {   

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >::StaticVector()
{
}

template< typename Real >
   template< typename _unused >
__cuda_callable__
StaticVector< 2, Real >::StaticVector( const Real v[ 2 ] )
: StaticArray< 2, Real >( v )
{
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >::StaticVector( const Real& v )
: StaticArray< 2, Real >( v )
{
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >::StaticVector( const Real& v1, const Real& v2 )
: StaticArray< 2, Real >( v1, v2 )
{
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >::StaticVector( const StaticVector< 2, Real >& v )
: StaticArray< 2, Real >( v )
{
}

template< typename Real >
bool
StaticVector< 2, Real >::setup( const Config::ParameterContainer& parameters,
                                const String& prefix )
{
   this->data[ 0 ] = parameters.getParameter< double >( prefix + "0" );
   this->data[ 1 ] = parameters.getParameter< double >( prefix + "1" );
   return true;
}

template< typename Real >
String StaticVector< 2, Real >::getType()
{
   return String( "Containers::StaticVector< " ) +
          String( 2 ) +
          String( ", " ) +
         TNL::getType< Real >() +
          String( " >" );
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >& StaticVector< 2, Real >::operator += ( const StaticVector& v )
{
   this->data[ 0 ] += v[ 0 ];
   this->data[ 1 ] += v[ 1 ];
   return *this;
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >& StaticVector< 2, Real >::operator -= ( const StaticVector& v )
{
   this->data[ 0 ] -= v[ 0 ];
   this->data[ 1 ] -= v[ 1 ];
   return *this;
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >& StaticVector< 2, Real >::operator *= ( const Real& c )
{
   this->data[ 0 ] *= c;
   this->data[ 1 ] *= c;
   return *this;
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real > StaticVector< 2, Real >::operator + ( const StaticVector& u ) const
{
   StaticVector< 2, Real > res;
   res[ 0 ] = this->data[ 0 ] + u[ 0 ];
   res[ 1 ] = this->data[ 1 ] + u[ 1 ];
   return res;
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real > StaticVector< 2, Real >::operator - ( const StaticVector& u ) const
{
   StaticVector< 2, Real > res;
   res[ 0 ] = this->data[ 0 ] - u[ 0 ];
   res[ 1 ] = this->data[ 1 ] - u[ 1 ];
   return res;
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real > StaticVector< 2, Real >::operator * ( const Real& c ) const
{
   StaticVector< 2, Real > res;
   res[ 0 ] = c * this->data[ 0 ];
   res[ 1 ] = c * this->data[ 1 ];
   return res;
}

template< typename Real >
__cuda_callable__
Real StaticVector< 2, Real >::operator * ( const StaticVector& u ) const
{
   return this->data[ 0 ] * u[ 0 ] +
          this->data[ 1 ] * u[ 1 ];
}

template< typename Real >
__cuda_callable__
bool StaticVector< 2, Real >::operator < ( const StaticVector& v ) const
{
   return ( this->data[ 0 ] < v[ 0 ] &&
            this->data[ 1 ] < v[ 1 ] );
}

template< typename Real >
__cuda_callable__
bool StaticVector< 2, Real >::operator <= ( const StaticVector& v ) const
{
   return ( this->data[ 0 ] <= v[ 0 ] &&
            this->data[ 1 ] <= v[ 1 ] );
}

template< typename Real >
__cuda_callable__
bool StaticVector< 2, Real >::operator > ( const StaticVector& v ) const
{
   return ( this->data[ 0 ] > v[ 0 ] &&
            this->data[ 1 ] > v[ 1 ] );
}

template< typename Real >
__cuda_callable__
bool StaticVector< 2, Real >::operator >= ( const StaticVector& v ) const
{
   return ( this->data[ 0 ] >= v[ 0 ] &&
            this->data[ 1 ] >= v[ 1 ] );
}

template< typename Real >
   template< typename OtherReal >
__cuda_callable__
StaticVector< 2, Real >::
operator StaticVector< 2, OtherReal >() const
{
   StaticVector< 2, OtherReal > aux;
   aux[ 0 ] = this->data[ 0 ];
   aux[ 1 ] = this->data[ 1 ];
   return aux;
}

template< typename Real >
__cuda_callable__
StaticVector< 2, Real >
StaticVector< 2, Real >::abs() const
{
   return StaticVector< 2, Real >( TNL::abs( this->data[ 0 ] ),
                                   TNL::abs( this->data[ 1 ] ) );
}

template< typename Real >
__cuda_callable__
Real
StaticVector< 2, Real >::lpNorm( const Real& p ) const
{
   if( p == 1.0 )
      return TNL::abs( this->data[ 0 ] ) + TNL::abs( this->data[ 1 ] );
   if( p == 2.0 )
      return TNL::sqrt( this->data[ 0 ] * this->data[ 0 ] + 
                        this->data[ 1 ] * this->data[ 1 ] );
   return TNL::pow( TNL::pow( TNL::abs( this->data[ 0 ] ), p ) +
                    TNL::pow( TNL::abs( this->data[ 1 ] ), p ), 1.0 / p ); 
}

#ifdef UNDEF //TEMPLATE_EXPLICIT_INSTANTIATION

#ifndef HAVE_CUDA
// TODO: does not work with CUDA
#ifdef INSTANTIATE_FLOAT
extern template class StaticVector< 2, float >;
#endif
extern template class StaticVector< 2, double >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class StaticVector< 2, long double >;
#endif
#endif

#endif

} // namespace Containers
} // namespace TNL

