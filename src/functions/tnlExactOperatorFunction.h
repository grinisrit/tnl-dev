/***************************************************************************
                          tnlExactOperatorFunction.h  -  description
                             -------------------
    begin                : Jan 5, 2016
    copyright            : (C) 2016 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TNLEXACTOPERATORFUNCTION_H
#define	TNLEXACTOPERATORFUNCTION_H

#include <functions/tnlDomain.h>

template< typename Operator,
          typename Function >
class tnlExactOperatorFunction : public tnlDomain< Operator::Dimensions, SpaceDomain >
{   
   public:
      
      typedef Operator OperatorType;
      typedef Function FunctionType;
      typedef typename FunctionType::RealType RealType;
      typedef typename FunctionType::VertexType VertexType;
      
      tnlExactOperatorFunction(
         const OperatorType& operator_,
         const FunctionType& function )
      : operator_( operator_ ), function( function ) {};
      
      template< typename VertexType,
                typename RealType = typename VertexType::RealType >
      __cuda_callable__
      RealType getValue(
         const VertexType& vertex,
         const RealType& time ) const
      {
         return this->operator_.getValue( function, vertex, time );
      }
      
   protected:
      
      const OperatorType& operator_;
      
      const FunctionType& function;               
};

#endif	/* TNLEXACTOPERATORFUNCTION_H */

