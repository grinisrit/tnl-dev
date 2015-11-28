/***************************************************************************
                          tnlConstantFunction.h  -  description
                             -------------------
    begin                : Aug 2, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
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

#ifndef TNLCONSTANTFUNCTION_H_
#define TNLCONSTANTFUNCTION_H_

#include <iostream>
#include <core/vectors/tnlStaticVector.h>
#include <functors/tnlFunction.h>

template< int dimensions,
          typename Real = double >
class tnlConstantFunction : public tnlFunction< dimensions, tnlAnalyticConstantFunction >
{
   public:
      
      typedef Real RealType;
      typedef tnlStaticVector< dimensions, RealType > VertexType;      
      
      tnlConstantFunction();

      static void configSetup( tnlConfigDescription& config,
                               const tnlString& prefix = "" );

      bool setup( const tnlParameterContainer& parameters,
                 const tnlString& prefix = "" );

      void setConstant( const RealType& constant );

      const RealType& getConstant() const;

   #ifdef HAVE_NOT_CXX11
      template< int XDiffOrder,
                int YDiffOrder,
                int ZDiffOrder,
                typename Vertex >
   #else
      template< int XDiffOrder,
                int YDiffOrder = 0,
                int ZDiffOrder = 0,
                typename Vertex = VertexType >
   #endif
      __cuda_callable__ inline
      RealType getValue( const Vertex& v,
                         const Real& time = 0.0 ) const;

      template< typename Vertex >
      __cuda_callable__ inline
      RealType getValue( const Vertex& v,
                         const Real& time = 0.0 ) const
      {
         return getValue< 0, 0, 0, Vertex >( v, time );
      }
      
       __cuda_callable__ inline
      RealType getValue( const Real& time = 0.0 ) const
      {
          return constant;
      }

   protected:

      RealType constant;
};

template< int dimensions,
          typename Real >
std::ostream& operator << ( std::ostream& str, const tnlConstantFunction< dimensions, Real >& f )
{
   str << "Constant function: constant = " << f.getConstant();
   return str;
}

#include <functors/tnlConstantFunction_impl.h>

#endif /* TNLCONSTANTFUNCTION_H_ */
