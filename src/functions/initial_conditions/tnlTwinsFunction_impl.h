/***************************************************************************
                          tnlExpBumpFunction_impl.h  -  description
                             -------------------
    begin                : Dec 5, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <functions/initial_conditions/tnlTwinsFunction.h>

namespace TNL {

template< typename Real,
          int Dimensions >
bool
tnlTwinsFunctionBase< Real, Dimensions >::
setup( const tnlParameterContainer& parameters,
       const tnlString& prefix )
{
   return true;
}


/***
 * 1D
 */

template< typename Real >
tnlString
tnlTwinsFunction< 1, Real >::getType()
{
   return "tnlTwinsFunction< 1, " + TNL::getType< Real >() + tnlString( " >" );
}

template< typename Real >
tnlTwinsFunction< 1, Real >::tnlTwinsFunction()
{
}

template< typename Real >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder,
             typename Vertex >
__cuda_callable__
Real
tnlTwinsFunction< 1, Real >::getPartialDerivative( const Vertex& v,
                                                   const Real& time ) const
{
   const RealType& x = v.x();
   if( YDiffOrder != 0 || ZDiffOrder != 0 )
      return 0.0;
   if( XDiffOrder == 0 )
      return 0.0;
   return 0.0;
}

template< typename Real >
__cuda_callable__
Real
tnlTwinsFunction< 1, Real >::
operator()( const VertexType& v,
            const Real& time ) const
{
   return this->template getPartialDerivative< 0, 0, 0 >( v, time );
}


/****
 * 2D
 */
template< typename Real >
tnlString
tnlTwinsFunction< 2, Real >::getType()
{
   return tnlString( "tnlTwinsFunction< 2, " ) + TNL::getType< Real >() + " >";
}

template< typename Real >
tnlTwinsFunction< 2, Real >::tnlTwinsFunction()
{
}

template< typename Real >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder,
             typename Vertex >
__cuda_callable__
Real
tnlTwinsFunction< 2, Real >::
getPartialDerivative( const Vertex& v,
                      const Real& time ) const
{
   const RealType& x = v.x();
   const RealType& y = v.y();
   if( ZDiffOrder != 0 )
      return 0.0;
   if( XDiffOrder == 0 && YDiffOrder == 0 )
      return -0.5 * ::sin( M_PI * x) * ::sin( M_PI * x) * ( 1 - ( y - 2 ) * ( y - 2 ) ) * ( 1 - ::tanh( 10 * ( ::sqrt( x * x + y * y ) - 0.6 ) ) );
   return 0.0;
}

template< typename Real >
__cuda_callable__
Real
tnlTwinsFunction< 2, Real >::
operator()( const VertexType& v,
            const Real& time ) const
{
   return this->template getPartialDerivative< 0, 0, 0 >( v, time );
}


/****
 * 3D
 */
template< typename Real >
tnlString
tnlTwinsFunction< 3, Real >::getType()
{
   return tnlString( "tnlTwinsFunction< 3, " ) + TNL::getType< Real >() + " >";
}

template< typename Real >
tnlTwinsFunction< 3, Real >::tnlTwinsFunction()
{
}

template< typename Real >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder,
             typename Vertex >
__cuda_callable__
Real
tnlTwinsFunction< 3, Real >::
getPartialDerivative( const Vertex& v,
                      const Real& time ) const
{
   const RealType& x = v.x();
   const RealType& y = v.y();
   const RealType& z = v.z();
   if( XDiffOrder == 0 && YDiffOrder == 0 && ZDiffOrder == 0 )
      return 0.0;
   return 0.0;
}

template< typename Real >
__cuda_callable__
Real
tnlTwinsFunction< 3, Real >::
operator()( const VertexType& v,
            const Real& time ) const
{
   return this->template getPartialDerivative< 0, 0, 0 >( v, time );
}

} // namespace TNL

