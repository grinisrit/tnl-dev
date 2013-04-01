/***************************************************************************
                          vector-operations.h  -  description
                             -------------------
    begin                : Nov 8, 2012
    copyright            : (C) 2012 by Tomas Oberhuber
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

#ifndef VECTOROPERATIONS_H_
#define VECTOROPERATIONS_H_

#include <core/cuda/device-check.h>
#include <core/cuda/cuda-reduction.h>
#include <core/cuda/reduction-operations.h>

template< typename Vector >
typename Vector :: RealType getHostVectorMax( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result = v. getElement( 0 );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Max( result, v. getElement( i ) );
   return result;
}

template< typename Vector >
typename Vector :: RealType getCudaVectorMax( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionMax< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v. getSize(),
                          v. getData(),
                          ( Real* ) 0,
                          result );
   return result;
}

template< typename Vector >
typename Vector :: RealType getVectorMax( const Vector& v )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   typedef typename Vector :: DeviceType Device;
   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorMax( v );
      case tnlCudaDevice:
         return getCudaVectorMax( v );
   }
}

template< typename Vector >
typename Vector :: RealType getHostVectorMin( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result = v. getElement( 0 );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Min( result, v. getElement( i ) );
   return result;
}

template< typename Vector >
typename Vector :: RealType getCudaVectorMin( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionMin< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v. getSize(),
                          v. getData(),
                          ( Real* ) 0,
                          result );
   return result;
}

template< typename Vector >
typename Vector :: RealType getVectorMin( const Vector& v )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   typedef typename Vector :: DeviceType Device;
   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorMin( v );
      case tnlCudaDevice:
         return getCudaVectorMin( v );
   }
}

template< typename Vector >
typename Vector :: RealType getHostVectorAbsMax( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result = fabs( v. getElement( 0 ) );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Max( result, ( Real ) fabs( v. getElement( i ) ) );
   return result;
}

template< typename Vector >
typename Vector :: RealType getCudaVectorAbsMax( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionAbsMax< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v. getSize(),
                          v. getData(),
                          ( Real* ) 0,
                          result );
   return result;
}

template< typename Vector >
typename Vector :: RealType getVectorAbsMax( const Vector& v )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   typedef typename Vector :: DeviceType Device;
   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorAbsMax( v );
      case tnlCudaDevice:
         return getCudaVectorAbsMax( v );
   }
}

template< typename Vector >
typename Vector :: RealType getHostVectorAbsMin( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result = fabs( v. getElement( 0 ) );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Min( result, ( Real ) fabs( v. getElement( i ) ) );
   return result;
}

template< typename Vector >
typename Vector :: RealType getCudaVectorAbsMin( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionAbsMin< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v. getSize(),
                          v. getData(),
                          ( Real* ) 0,
                          result );
   return result;
}

template< typename Vector >
typename Vector :: RealType getVectorAbsMin( const Vector& v )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   typedef typename Vector :: DeviceType Device;
   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorAbsMin( v );
      case tnlCudaDevice:
         return getCudaVectorAbsMin( v );
   }
}

template< typename Vector >
typename Vector :: RealType getHostVectorLpNorm( const Vector& v,
                                                 const typename Vector :: RealType& p )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   if( p == 1.0 )
   {
      Real result = fabs( v. getElement( 0 ) );
      const Index n = v. getSize();
      for( Index i = 1; i < n; i ++ )
         result += fabs( v. getElement( i ) );
      return result;
   }
   if( p == 2.0 )
   {
      Real result = v. getElement( 0 );
      result *= result;
      const Index n = v. getSize();
      for( Index i = 1; i < n; i ++ )
      {
         const Real aux = v. getElement( i );
         result += aux * aux;
      }
      return sqrt( result );
   }
   Real result = pow( fabs( v. getElement( 0 ) ), p );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result += pow( fabs( v. getElement( i ) ), p );
   return pow( result, 1.0 / p );
}

template< typename Vector >
typename Vector :: RealType getCudaVectorLpNorm( const Vector& v,
                                                 const typename Vector :: RealType& p )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;

   Real result( 0 );
   tnlParallelReductionLpNorm< Real, Index > operation;
   operation. setPower( p );
   reductionOnCudaDevice( operation,
                          v. getSize(),
                          v. getData(),
                          ( Real* ) 0,
                          result );
   return pow( result, 1.0 / p );
}

template< typename Vector >
typename Vector :: RealType getVectorLpNorm( const Vector& v,
                                             const typename Vector :: RealType& p )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   tnlAssert( p > 0.0,
              cerr << " p = " << p );
   typedef typename Vector :: DeviceType Device;
   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorLpNorm( v, p );
      case tnlCudaDevice:
         return getCudaVectorLpNorm( v, p );
   }
}
template< typename Vector >
typename Vector :: RealType getHostVectorSum( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;

   Real result = v. getElement( 0 );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result += v. getElement( i );
   return result;
}

template< typename Vector >
typename Vector :: RealType getCudaVectorSum( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;

   Real result( 0 );
   tnlParallelReductionSum< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v. getSize(),
                          v. getData(),
                          ( Real* ) 0,
                          result );
   return result;
}

template< typename Vector >
typename Vector :: RealType getVectorSum( const Vector& v )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   typedef typename Vector :: DeviceType Device;
   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorSum( v );
      case tnlCudaDevice:
         return getCudaVectorSum( v );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorDifferenceMax( const Vector1& v1,
                                                         const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result = v1. getElement( 0 ) - v2. getElement( 0 );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Max( result, v1. getElement( i ) - v2. getElement( i ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorDifferenceMax( const Vector1& v1,
                                                         const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionDiffMax< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v1. getSize(),
                          v1. getData(),
                          v2. getData(),
                          result );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorDifferenceMax( const Vector1& v1,
                                                     const Vector2& v2 )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorDifferenceMax( v1, v2 );
      case tnlCudaDevice:
         return getCudaVectorDifferenceMax( v1, v2 );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorDifferenceMin( const Vector1& v1,
                                                         const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result = v1. getElement( 0 ) - v2. getElement( 0 );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Min( result, v1. getElement( i ) - v2. getElement( i ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorDifferenceMin( const Vector1& v1,
                                                         const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionDiffMin< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v1. getSize(),
                          v1. getData(),
                          v2. getData(),
                          result );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorDifferenceMin( const Vector1& v1,
                                                     const Vector2& v2 )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorDifferenceMin( v1, v2 );
      case tnlCudaDevice:
         return getCudaVectorDifferenceMin( v1, v2 );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorDifferenceAbsMax( const Vector1& v1,
                                                            const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result = fabs( v1. getElement( 0 ) - v2. getElement( 0 ) );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Max( result, ( Real ) fabs( v1. getElement( i ) - v2. getElement( i ) ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorDifferenceAbsMax( const Vector1& v1,
                                                            const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionDiffAbsMax< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v1. getSize(),
                          v1. getData(),
                          v2. getData(),
                          result );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorDifferenceAbsMax( const Vector1& v1,
                                                        const Vector2& v2 )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorDifferenceAbsMax( v1, v2 );
      case tnlCudaDevice:
         return getCudaVectorDifferenceAbsMax( v1, v2 );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorDifferenceAbsMin( const Vector1& v1,
                                                            const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result = fabs( v1. getElement( 0 ) - v2. getElement( 0 ) );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Min( result, ( Real ) fabs( v1. getElement( i ) - v2. getElement( i ) ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorDifferenceAbsMin( const Vector1& v1,
                                                            const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionDiffAbsMin< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v1. getSize(),
                          v1. getData(),
                          v2. getData(),
                          result );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorDifferenceAbsMin( const Vector1& v1,
                                                        const Vector2& v2 )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorDifferenceAbsMin( v1, v2 );
      case tnlCudaDevice:
         return getCudaVectorDifferenceAbsMin( v1, v2 );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorDifferenceLpNorm( const Vector1& v1,
                                                            const Vector2& v2,
                                                            const typename Vector1 :: RealType& p )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   if( p == 1.0 )
   {
      Real result = fabs( v1. getElement( 0 ) - v2. getElement( 0 ) );
      const Index n = v1. getSize();
      for( Index i = 1; i < n; i ++ )
         result += fabs( v1. getElement( i ) - v2. getElement( i ) );
      return result;
   }
   if( p == 2.0 )
   {
      Real result = fabs( v1. getElement( 0 ) - v2. getElement( 0 ) );
      result *= result;
      const Index n = v1. getSize();
      for( Index i = 1; i < n; i ++ )
      {
         Real aux = fabs( v1. getElement( i ) - v2. getElement( i ) );
         result += aux * aux;
      }
      return sqrt( result );
   }
   Real result = pow( fabs( v1. getElement( 0 ) - v2. getElement( 0 ) ), p );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result += pow( fabs( v1. getElement( i ) - v2. getElement( i ) ), p );
   return pow( result, 1.0 / p );
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorDifferenceLpNorm( const Vector1& v1,
                                                            const Vector2& v2,
                                                            const typename Vector1 :: RealType& p )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionDiffLpNorm< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v1. getSize(),
                          v1. getData(),
                          v2. getData(),
                          result );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorDifferenceLpNorm( const Vector1& v1,
                                                        const Vector2& v2,
                                                        const typename Vector1 :: RealType& p )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( p > 0.0,
              cerr << " p = " << p );
   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorDifferenceLpNorm( v1, v2, p );
      case tnlCudaDevice:
         return getCudaVectorDifferenceLpNorm( v1, v2, p );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorDifferenceSum( const Vector1& v1,
                                                         const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result = v1. getElement( 0 ) - v2. getElement( 0 );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result += v1. getElement( i ) - v2. getElement( i );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorDifferenceSum( const Vector1& v1,
                                                         const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   Real result( 0 );
   tnlParallelReductionDiffSum< Real, Index > operation;
   reductionOnCudaDevice( operation,
                          v1. getSize(),
                          v1. getData(),
                          v2. getData(),
                          result );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorDifferenceSum( const Vector1& v1,
                                                     const Vector2& v2 )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorDifferenceSum( v1, v2 );
      case tnlCudaDevice:
         return getCudaVectorDifferenceSum( v1, v2 );
   }
}

template< typename Vector >
void hostVectorScalarMultiplication( Vector& v,
                                     const typename Vector :: RealType& alpha )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;

   const Index n = v. getSize();
   for( Index i = 0; i < n; i ++ )
      v[ i ] *= alpha;
}

template< typename Vector >
void cudaVectorScalarMultiplication( Vector& v,
                                     const typename Vector :: RealType& alpha )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;

#ifdef HAVE_CUDA
   dim3 blockSize, gridSize;
   blockSize. x = 512;
   gridSize. x = v. getSize() / 512 + 1;
   // TODO: Fix this - the grid size might be limiting for large vectors.

   /*
   tnlVectorCUDAScalaMultiplicationKernel<<< gridSize, blockSize >>>( v. getSize(),
                                                                      alpha,
                                                                      v. getData() );*/
#else
   cerr << "I am sorry but CUDA support is missing on this system " << __FILE__ << " line " << __LINE__ << "." << endl;
#endif

}

template< typename Vector >
void vectorScalarMultiplication( Vector& v,
                                 const typename Vector :: RealType& alpha )
{
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );

   typedef typename Vector :: DeviceType Device;

   switch( Device :: getDevice() )
   {
      case tnlHostDevice:
         return hostVectorScalarMultiplication( v, alpha );
      case tnlCudaDevice:
         return cudaVectorScalarMultiplication( v, alpha );
   }
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getHostVectorSdot( const Vector1& v1,
                                                const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result = 0;
   const Index n = v1. getSize();
   for( Index i = 0; i < n; i ++ )
      result += v1. getElement( i ) * v2. getElement( i );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getCudaVectorSdot( const Vector1& v1,
                                                const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   Real result( 0 );
   /*reductionOnCudaDevice< Real,
                               Real,
                               Index,
                               tnlParallelReductionSdot >
                             ( v1. getSize(),
                               v1. getData(),
                               v2. getData(),
                               result,
                               ( Real ) 0 );*/
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType getVectorSdot( const Vector1& v1,
                                            const Vector2& v2 )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return getHostVectorSdot( v1, v2 );
      case tnlCudaDevice:
         return getCudaVectorSdot( v1, v2 );
   }
}

template< typename Vector1, typename Vector2 >
void hostVectorSaxpy( Vector1& y,
                      const Vector2& x,
                      const typename Vector1 :: RealType& alpha )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   const Index n = y. getSize();
   for( Index i = 0; i < n; i ++ )
      y[ i ] += alpha * x[ i ];
}

template< typename Vector1, typename Vector2 >
void cudaVectorSaxpy( Vector1& y,
                      const Vector2& x,
                      const typename Vector1 :: RealType& alpha )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

#ifdef HAVE_CUDA
   dim3 blockSize, gridSize;
   blockSize. x = 512;
   gridSize. x = x. getSize() / 512 + 1;

   // TODO: fix this
   /*tnlVectorCUDASaxpyKernel<<< gridSize, blockSize >>>( y. getSize(),
                                                        alpha,
                                                        x. getData(),
                                                        y. getData() );*/
#else
   cerr << "I am sorry but CUDA support is missing on this system " << __FILE__ << " line " << __LINE__ << "." << endl;
#endif
}

template< typename Vector1, typename Vector2 >
void vectorSaxpy( Vector1& y,
                  const Vector2& x,
                  const typename Vector1 :: RealType& alpha )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( y. getSize() > 0,
              cerr << "Vector name is " << y. getName() );
   tnlAssert( y. getSize() == x. getSize(),
              cerr << "Vector names are " << x. getName() << " and " << y. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << y. getName() << " and " << x. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return hostVectorSaxpy( y, x, alpha );
      case tnlCudaDevice:
         return cudaVectorSaxpy( y, x, alpha );
   }
}

template< typename Vector1, typename Vector2 >
void hostVectorSaxmy( Vector1& y,
                      const Vector2& x,
                      const typename Vector1 :: RealType& alpha )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   const Index n = y. getSize();
   for( Index i = 0; i < n; i ++ )
      y[ i ] = alpha * x[ i ] - y[ i ];
}

template< typename Vector1, typename Vector2 >
void cudaVectorSaxmy( Vector1& y,
                      const Vector2& x,
                      const typename Vector1 :: RealType& alpha )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

#ifdef HAVE_CUDA
   dim3 blockSize, gridSize;
   blockSize. x = 512;
   gridSize. x = x. getSize() / 512 + 1;

   // TODO: fix this
   /*tnlVectorCUDASaxmyKernel<<< gridSize, blockSize >>>( y. getSize(),
                                                        alpha,
                                                        x. getData(),
                                                        y. getData() );*/
#else
   cerr << "I am sorry but CUDA support is missing on this system " << __FILE__ << " line " << __LINE__ << "." << endl;
#endif
}

template< typename Vector1, typename Vector2 >
void vectorSaxmy( Vector1& y,
                  const Vector2& x,
                  const typename Vector1 :: RealType& alpha )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( y. getSize() > 0,
              cerr << "Vector name is " << y. getName() );
   tnlAssert( y. getSize() == x. getSize(),
              cerr << "Vector names are " << x. getName() << " and " << y. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << y. getName() << " and " << x. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return hostVectorSaxmy( y, x, alpha );
      case tnlCudaDevice:
         return cudaVectorSaxmy( y, x, alpha );
   }
}

template< typename Vector1, typename Vector2 >
void hostVectorSaxpsby( Vector1& y,
                        const Vector2& x,
                        const typename Vector1 :: RealType& alpha,
                        const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   const Index n = y. getSize();
   for( Index i = 0; i < n; i ++ )
      y[ i ] = alpha * x[ i ] + beta *  y[ i ];
}

template< typename Vector1, typename Vector2 >
void cudaVectorSaxpsby( Vector1& y,
                        const Vector2& x,
                        const typename Vector1 :: RealType& alpha,
                        const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

#ifdef HAVE_CUDA
   dim3 blockSize, gridSize;
   blockSize. x = 512;
   gridSize. x = x. getSize() / 512 + 1;

   // TODO: fix this
   /*tnlVectorCUDASaxpsbzKernel<<< gridSize, blockSize >>>( y. getSize(),
                                                          alpha,
                                                          x. getData(),
                                                          beta );*/
#else
   cerr << "I am sorry but CUDA support is missing on this system " << __FILE__ << " line " << __LINE__ << "." << endl;
#endif
}


template< typename Vector1, typename Vector2 >
void vectorSaxpsby( Vector1& y,
                    const Vector2& x,
                    const typename Vector1 :: RealType& alpha,
                    const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( y. getSize() > 0,
              cerr << "Vector name is " << y. getName() );
   tnlAssert( y. getSize() == x. getSize(),
              cerr << "Vector names are " << x. getName() << " and " << y. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << y. getName() << " and " << x. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return hostVectorSaxpsby( y, x, alpha, beta );
      case tnlCudaDevice:
         return cudaVectorSaxpsby( y, x, alpha, beta );
   }
}

template< typename Vector1, typename Vector2 >
void hostVectorSaxpsbz( Vector1& y,
                        const Vector2& x,
                        const typename Vector1 :: RealType& alpha,
                        const Vector2& z,
                        const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   const Index n = y. getSize();
   for( Index i = 0; i < n; i ++ )
      y[ i ] = alpha * x[ i ] + beta *  z[ i ];
}

template< typename Vector1, typename Vector2 >
void cudaVectorSaxpsbz( Vector1& y,
                        const Vector2& x,
                        const typename Vector1 :: RealType& alpha,
                        const Vector2& z,
                        const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

#ifdef HAVE_CUDA
   dim3 blockSize, gridSize;
   blockSize. x = 512;
   gridSize. x = x. getSize() / 512 + 1;

   // TODO: fix this
   /*tnlVectorCUDASaxpsbzKernel<<< gridSize, blockSize >>>( y. getSize(),
                                                          alpha,
                                                          x. getData(),
                                                          beta,
                                                          z. getData() );*/
#else
   cerr << "I am sorry but CUDA support is missing on this system " << __FILE__ << " line " << __LINE__ << "." << endl;
#endif
}


template< typename Vector1, typename Vector2 >
void vectorSaxpsbz( Vector1& y,
                    const Vector2& x,
                    const typename Vector1 :: RealType& alpha,
                    const Vector2& z,
                    const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( y. getSize() > 0,
              cerr << "Vector name is " << y. getName() );
   tnlAssert( y. getSize() == x. getSize(),
              cerr << "Vector names are " << x. getName() << " and " << y. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << y. getName() << " and " << x. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return hostVectorSaxpsbz( y, x, alpha, z, beta );
      case tnlCudaDevice:
         return cudaVectorSaxpsbz( y, x, alpha, z, beta );
   }
}

template< typename Vector1, typename Vector2 >
void hostVectorSaxpsbzpy( Vector1& y,
                          const Vector2& x,
                          const typename Vector1 :: RealType& alpha,
                          const Vector2& z,
                          const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   const Index n = y. getSize();
   for( Index i = 0; i < n; i ++ )
      y[ i ] += alpha * x[ i ] + beta *  z[ i ];
}

template< typename Vector1, typename Vector2 >
void cudaVectorSaxpsbzpy( Vector1& y,
                          const Vector2& x,
                          const typename Vector1 :: RealType& alpha,
                          const Vector2& z,
                          const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

#ifdef HAVE_CUDA
   dim3 blockSize, gridSize;
   blockSize. x = 512;
   gridSize. x = x. getSize() / 512 + 1;

   // TODO: fix this
   /*tnlVectorCUDASaxpsbzpyKernel<<< gridSize, blockSize >>>( y. getSize(),
                                                          alpha,
                                                          x. getData(),
                                                          beta,
                                                          z. getData() );*/
#else
   cerr << "I am sorry but CUDA support is missing on this system " << __FILE__ << " line " << __LINE__ << "." << endl;
#endif
}


template< typename Vector1, typename Vector2 >
void vectorSaxpsbzpy( Vector1& y,
                      const Vector2& x,
                      const typename Vector1 :: RealType& alpha,
                      const Vector2& z,
                      const typename Vector1 :: RealType& beta )
{
   typedef typename Vector1 :: DeviceType Device1;
   typedef typename Vector2 :: DeviceType Device2;

   tnlAssert( y. getSize() > 0,
              cerr << "Vector name is " << y. getName() );
   tnlAssert( y. getSize() == x. getSize(),
              cerr << "Vector names are " << x. getName() << " and " << y. getName() );
   tnlAssert( Device1 :: getDevice() == Device2 :: getDevice(),
              cerr << "Vector names are " << y. getName() << " and " << x. getName() );

   switch( Device1 :: getDevice() )
   {
      case tnlHostDevice:
         return hostVectorSaxpsbzpy( y, x, alpha, z, beta );
      case tnlCudaDevice:
         return cudaVectorSaxpsbzpy( y, x, alpha, z, beta );
   }
}


#endif /* VECTOROPERATIONS_H_ */
