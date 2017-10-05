/***************************************************************************
                          ExactOperatorComposition.h  -  description
                             -------------------
    begin                : Feb 17, 2016
    copyright            : (C) 2016 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

namespace TNL {
namespace Operators {

template< typename OuterOperator,
          typename InnerOperator >
class ExactOperatorComposition
{
   public:
 
      template< typename Function >
      __cuda_callable__ inline
      typename Function::RealType operator()( const Function& function,
                                              const typename Function::PointType& v,
                                              const typename Function::RealType& time = 0.0 ) const
      {
         return OuterOperator( innerOperator( function, v, time), v, time );
      }
 
   protected:
 
      InnerOperator innerOperator;
 
      OuterOperator outerOperator;
};

} // namespace Operators
} // namespace TNL

