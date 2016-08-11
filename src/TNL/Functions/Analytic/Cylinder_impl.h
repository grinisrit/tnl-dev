/***************************************************************************
                          ExpBump_impl.h  -  description
                             -------------------
    begin                : Dec 5, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Functions/Analytic/Cylinder.h>

namespace TNL {
namespace Functions {
namespace Analytic {   

template< typename Real,
          int Dimensions >
bool
CylinderBase< Real, Dimensions >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
   this->diameter = parameters.getParameter< double >( prefix + "diameter" );
   return true;
}

template< typename Real,
          int Dimensions >
void
CylinderBase< Real, Dimensions >::
setDiameter( const Real& sigma )
{
   this->diameter = diameter;
}

template< typename Real,
          int Dimensions >
const Real& CylinderBase< Real, Dimensions >::getDiameter() const
{
   return this->diameter;
}

/***
 * 1D
 */

template< typename Real >
String
Cylinder< 1, Real >::getType()
{
   return "Functions::Analytic::Cylinder< 1, " + TNL::getType< Real >() + String( " >" );
}

template< typename Real >
Cylinder< 1, Real >::Cylinder()
{
}

template< typename Real >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder,
             typename Vertex >
__cuda_callable__
Real
Cylinder< 1, Real >::getPartialDerivative( const Vertex& v,
                                                      const Real& time ) const
{
   const RealType& x = v.x();
   if( YDiffOrder != 0 || ZDiffOrder != 0 )
      return 0.0;
   if( XDiffOrder == 0 )
      return ( ( x*x - this->diameter ) < 0 ) - ( ( x*x - this->diameter ) > 0 ) + 1;
   return 0.0;
}

template< typename Real >
__cuda_callable__
Real
Cylinder< 1, Real >::
operator()( const VertexType& v,
            const Real& time ) const
{
   return this->template getPartialDerivative< 0, 0, 0 >( v, time );
}

/****
 * 2D
 */

template< typename Real >
String
Cylinder< 2, Real >::getType()
{
   return String( "Functions::Analytic::Cylinder< 2, " ) + TNL::getType< Real >() + " >";
}

template< typename Real >
Cylinder< 2, Real >::Cylinder()
{
}

template< typename Real >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder,
             typename Vertex >
__cuda_callable__
Real
Cylinder< 2, Real >::
getPartialDerivative( const Vertex& v,
                      const Real& time ) const
{
   const RealType& x = v.x();
   const RealType& y = v.y();
   if( ZDiffOrder != 0 )
      return 0.0;
   if( XDiffOrder == 0 && YDiffOrder == 0 )
      return ( ( x*x + y*y - this->diameter ) < 0 ) - ( ( x*x + y*y - this->diameter ) > 0 ) + 1;
   return 0.0;
}

template< typename Real >
__cuda_callable__
Real
Cylinder< 2, Real >::
operator()( const VertexType& v,
            const Real& time ) const
{
   return this->template getPartialDerivative< 0, 0, 0 >( v, time );
}


/****
 * 3D
 */

template< typename Real >
String
Cylinder< 3, Real >::getType()
{
   return String( "Functions::Analytic::Cylinder< 3, " ) + TNL::getType< Real >() + " >";
}

template< typename Real >
Cylinder< 3, Real >::Cylinder()
{
}

template< typename Real >
   template< int XDiffOrder,
             int YDiffOrder,
             int ZDiffOrder,
             typename Vertex >
__cuda_callable__
Real
Cylinder< 3, Real >::
getPartialDerivative( const Vertex& v,
                      const Real& time ) const
{
   const RealType& x = v.x();
   const RealType& y = v.y();
   const RealType& z = v.z();
   if( XDiffOrder == 0 && YDiffOrder == 0 && ZDiffOrder == 0 )
      return ( ( x*x + y*y + z*z - this->diameter ) < 0 ) - ( ( x*x + y*y + z*z - this->diameter ) > 0 ) + 1;
   return 0.0;
}

template< typename Real >
__cuda_callable__
Real
Cylinder< 3, Real >::
operator()( const VertexType& v,
            const Real& time ) const
{
   return this->template getPartialDerivative< 0, 0, 0 >( v, time );
}

} // namespace Analytic
} // namespace Functions
} // namespace TNL
