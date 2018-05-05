/***************************************************************************
                          Rotation.h  -  description
                             -------------------
    begin                : Feb 6, 2017
    copyright            : (C) 2017 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Functions/Domain.h>
#include <TNL/Devices/Cuda.h>
#include <TNL/Config/ParameterContainer.h>


namespace TNL {
namespace Operators {
namespace Analytic {   
   

// TODO: Implement RotationXY, RotationXZ and RotationYZ in all dimensions. 
//       Where it does not make sense, the operator does nothing.   
   
template< typename Real,
          int Dimensions >
class RotationBase
{
   public:
      typedef Real RealType;
      typedef Containers::StaticVector< Dimenions, RealType > PointType;
      
      RotationBase() : center( 0.0 ) {};
      
      void setCenter( const PointType& center )
      {
         this->center = center;
      }
      
      __cuda_callable__
      const PointType& getCenter() const
      {
         return this->center;
      }
      
   protected:
      
      PointType center;
};
   
template< typename Function,
          int Dimensions = Function::getDomainDimenions() >
class Rotation;

template< typename Function, 1 >
class Rotation: public Functions::Domain< Function::getDomainDimenions(), 
                                          Function::getDomainType() >
{
   public:
      
      typedef typename Function::RealType RealType;
      typedef Containers::StaticVector< Function::getDomainDimenions(), 
                                        RealType > PointType;
      
      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" ){};
      
      
      __cuda_callable__
      RealType operator()( const Function& function,
                           const PointType& vertex,
                           const RealType& time = 0 ) const
      {
         return function( vertex, time );
      }
};

} // namespace Analytic
} // namespace Operators
} // namespace TNL