/***************************************************************************
                          ExactOperatorFunction.h  -  description
                             -------------------
    begin                : Jan 5, 2016
    copyright            : (C) 2016 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Functions/Domain.h>

namespace TNL {
namespace Functions {   

template< typename Operator,
          typename Function >
class ExactOperatorFunction : public Domain< Operator::getDomainDimension(), SpaceDomain >
{
   static_assert( Operator::getDomainDimension() == Function::getDomainDimension(),
      "Operator and function have different number of domain dimensions." );
 
   public:
 
      typedef Operator OperatorType;
      typedef Function FunctionType;
      typedef typename FunctionType::RealType RealType;
      typedef typename FunctionType::PointType PointType;
 
      static constexpr int getDomainDimension() { return Operator::getDomainDimension(); }
 
      ExactOperatorFunction(
         const OperatorType& operator_,
         const FunctionType& function )
      : operator_( operator_ ), function( function ) {}
 
      __cuda_callable__
      RealType operator()(
         const PointType& vertex,
         const RealType& time ) const
      {
         return this->operator_( function, vertex, time );
      }
 
   protected:
 
      const OperatorType& operator_;
 
      const FunctionType& function;
};

} // namespace Functions
} // namespace TNL

