/***************************************************************************
                          tnlExactLinearDiffusion_impl.h  -  description
                             -------------------
    begin                : Aug 8, 2014
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

#ifndef TNLEXACTNONLINEARDIFFUSION_IMPL_H_
#define TNLEXACTNONLINEARDIFFUSION_IMPL_H_

template< typename OperatorQ >
tnlString
tnlExactNonlinearDiffusion< OperatorQ, 1 >::
getType()
{
   return "tnlExactNonlinearDiffusion< " + OperatorQ::getType() + ", 1 >";
}

template< typename OperatorQ >
template< typename Function, typename Vertex, typename Real >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Real
tnlExactNonlinearDiffusion< OperatorQ, 1 >::
getValue( const Function& function,
          const Vertex& v,
          const Real& time )
{
   return function.template getValue< 2, 0, 0, Vertex >( v, time ) - function.template getValue< 1, 0, 0, Vertex >( v, time ) 
          * OperatorQ::template getValue<1, 0, 0>(function, v, time ) / OperatorQ::template getValue<0, 0, 0>(function, v, time );
}

template< typename OperatorQ >
tnlString
tnlExactNonlinearDiffusion< OperatorQ, 2 >::
getType()
{
   return "tnlExactNonlinearDiffusion< " + OperatorQ::getType() + ", 2 >";
}

template< typename OperatorQ >
template< typename Function, typename Vertex, typename Real >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Real
tnlExactNonlinearDiffusion< OperatorQ, 2 >::
getValue( const Function& function,
          const Vertex& v,
          const Real& time )
{
   return  function.template getValue< 2, 0, 0, Vertex >( v, time ) +  function.template getValue< 0, 2, 0, Vertex >( v, time )
           -( OperatorQ::template getValue<1, 0, 0> (function, v, time) * function.template getValue< 1, 0, 0, Vertex >( v, time ) 
           + OperatorQ::template getValue<0, 1, 0> (function, v, time) * function.template getValue< 0, 1, 0, Vertex >( v, time ) ) 
           / OperatorQ::template getValue<0, 0, 0> (function, v, time);
}

template< typename OperatorQ >
tnlString
tnlExactNonlinearDiffusion< OperatorQ, 3 >::
getType()
{
   return "tnlExactNonlinearDiffusion< " + OperatorQ::getType() + ", 3 >";
}

template< typename OperatorQ >
template< typename Function, typename Vertex, typename Real >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Real
tnlExactNonlinearDiffusion< OperatorQ, 3 >::
getValue( const Function& function,
          const Vertex& v,
          const Real& time )
{
   return  function.template getValue< 2, 0, 0, Vertex >( v, time ) +  function.template getValue< 0, 2, 0, Vertex >( v, time )
           +  function.template getValue< 0, 0, 2, Vertex >( v, time )
           -( OperatorQ::template getValue<1, 0, 0> (function, v, time) * function.template getValue< 1, 0, 0, Vertex >( v, time ) 
           + OperatorQ::template getValue<0, 1, 0> (function, v, time) * function.template getValue< 0, 1, 0, Vertex >( v, time )
           + OperatorQ::template getValue<0, 0, 1> (function, v, time) * function.template getValue< 0, 0, 1, Vertex >( v, time ) )
           / OperatorQ::template getValue<0, 0, 0> (function, v, time);
}

#endif /* TNLEXACTNONLINEARDIFFUSION_IMPL_H_ */
