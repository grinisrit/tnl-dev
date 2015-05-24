/***************************************************************************
                          tnlExpBumpFunction.h  -  description
                             -------------------
    begin                : Dec 5, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#ifndef TNLPSEUDOSQUAREFUNCTION_H_
#define TNLPSEUDOSQUAREFUNCTION_H_

#include <config/tnlParameterContainer.h>
#include <core/vectors/tnlStaticVector.h>
#include <functions/tnlFunctionType.h>

template< typename Real >
class tnlPseudoSquareFunctionBase
{
   public:

      typedef Real RealType;

      bool setup( const tnlParameterContainer& parameters,
                 const tnlString& prefix = "" );

   protected:

      RealType height;
};

template< int Dimensions,
          typename Real >
class tnlPseudoSquareFunction
{
};

template< typename Real >
class tnlPseudoSquareFunction< 1, Real > : public tnlPseudoSquareFunctionBase< Real >
{
   public:

      enum { Dimensions = 1 };
      typedef Real RealType;
      typedef tnlStaticVector< Dimensions, Real > VertexType;

      static tnlString getType();

      tnlPseudoSquareFunction();

#ifdef HAVE_NOT_CXX11
      template< int XDiffOrder,
                int YDiffOrder,
                int ZDiffOrder,
                typename Vertex >
#else
      template< int XDiffOrder = 0,
                int YDiffOrder = 0,
                int ZDiffOrder = 0,
                typename Vertex = VertexType >
#endif   
#ifdef HAVE_CUDA
      __device__ __host__
#endif
      RealType getValue( const Vertex& v,
                         const Real& time = 0.0 ) const;
};

template< typename Real >
class tnlPseudoSquareFunction< 2, Real > : public tnlPseudoSquareFunctionBase< Real >
{
   public:

      enum { Dimensions = 2 };
      typedef Real RealType;
      typedef tnlStaticVector< Dimensions, Real > VertexType;

      static tnlString getType();

      tnlPseudoSquareFunction();

#ifdef HAVE_NOT_CXX11
      template< int XDiffOrder,
                int YDiffOrder,
                int ZDiffOrder,
                typename Vertex >
#else
      template< int XDiffOrder = 0,
                int YDiffOrder = 0,
                int ZDiffOrder = 0,
                typename Vertex = VertexType >
#endif
#ifdef HAVE_CUDA
      __device__ __host__
#endif
      RealType getValue( const Vertex& v,
                         const Real& time = 0.0 ) const;
};

template< typename Real >
class tnlPseudoSquareFunction< 3, Real > : public tnlPseudoSquareFunctionBase< Real >
{
   public:

      enum { Dimensions = 3 };
      typedef Real RealType;
      typedef tnlStaticVector< Dimensions, Real > VertexType;

      static tnlString getType();

      tnlPseudoSquareFunction();

#ifdef HAVE_NOT_CXX11
      template< int XDiffOrder,
                int YDiffOrder,
                int ZDiffOrder,
                typename Vertex >
#else
      template< int XDiffOrder = 0,
                int YDiffOrder = 0,
                int ZDiffOrder = 0,
                typename Vertex = VertexType >
#endif   
#ifdef HAVE_CUDA
      __device__ __host__
#endif
      RealType getValue( const Vertex& v,
                         const Real& time = 0.0 ) const;
};

template< int Dimensions,
          typename Real >
ostream& operator << ( ostream& str, const tnlPseudoSquareFunction< Dimensions, Real >& f )
{
   str << "Level-set pseudo square function.";
   return str;
}

template< int FunctionDimensions,
          typename Real >
class tnlFunctionType< tnlPseudoSquareFunction< FunctionDimensions, Real > >
{
   public:

      enum { Type = tnlAnalyticFunction };
};


#include <functions/initial_conditions/level_set_functions/tnlPseudoSquareFunction_impl.h>


#endif /* TNLPSEUDOSQUAREFUNCTION_H_ */
