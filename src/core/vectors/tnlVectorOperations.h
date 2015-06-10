/***************************************************************************
                          tnlVectorOperations.h  -  description
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

#ifndef TNLVECTOROPERATIONS_H_
#define TNLVECTOROPERATIONS_H_

#include <core/cuda/cuda-reduction.h>
#include <core/cuda/reduction-operations.h>
#include <core/tnlHost.h>
#include <core/tnlCuda.h>

template< typename Device >
class tnlVectorOperations{};

template<>
class tnlVectorOperations< tnlHost >
{
   public:

   template< typename Vector >
   static void addElement( Vector& v,
                           const typename Vector::IndexType i,
                           const typename Vector::RealType& value );

   template< typename Vector >
   static void addElement( Vector& v,
                           const typename Vector::IndexType i,
                           const typename Vector::RealType& value,
                           const typename Vector::RealType& thisElementMultiplicator );

   template< typename Vector >
   static typename Vector::RealType getVectorMax( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorMin( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorAbsMax( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorAbsMin( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorLpNorm( const Vector& v,
                                                     const typename Vector::RealType& p );

   template< typename Vector >
   static typename Vector::RealType getVectorSum( const Vector& v );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceMax( const Vector1& v1,
                                                               const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceMin( const Vector1& v1,
                                                               const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceAbsMax( const Vector1& v1,
                                                                  const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceAbsMin( const Vector1& v1,
                                                                  const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceLpNorm( const Vector1& v1,
                                                           const Vector2& v2,
                                                           const typename Vector1::RealType& p );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceSum( const Vector1& v1,
                                                               const Vector2& v2 );
   template< typename Vector >
   static void vectorScalarMultiplication( Vector& v,
                                           const typename Vector::RealType& alpha );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getScalarProduct( const Vector1& v1,
                                                         const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static void addVector( Vector1& y,
                          const Vector2& v,
                          const typename Vector2::RealType& multiplicator,
                          const typename Vector1::RealType& thisMultiplicator = 1.0 );
   
   template< typename Vector1, typename Vector2, typename Vector3 >
   static void addVectors( Vector1& v,
                           const Vector2& v1,
                           const typename Vector2::RealType& multiplicator1,
                           const Vector3& v2,
                           const typename Vector3::RealType& multiplicator2,
                           const typename Vector1::RealType& thisMultiplicator = 1.0 );


   /*template< typename Vector1, typename Vector2 >
   static void alphaXPlusBetaY( Vector1& y,
                                const Vector2& x,
                                const typename Vector1::RealType& alpha,
                                const typename Vector1::RealType& beta );

   template< typename Vector1, typename Vector2 >
   static void alphaXPlusBetaZ( Vector1& y,
                                const Vector2& x,
                                const typename Vector1::RealType& alpha,
                                const Vector2& z,
                                const typename Vector1::RealType& beta );

   template< typename Vector1, typename Vector2 >
   static void alphaXPlusBetaZPlusY( Vector1& y,
                                     const Vector2& x,
                                     const typename Vector1::RealType& alpha,
                                     const Vector2& z,
                                     const typename Vector1::RealType& beta );*/

   template< typename Vector >
   static void computePrefixSum( Vector& v,
                                 const typename Vector::IndexType begin,
                                 const typename Vector::IndexType end );

   template< typename Vector >
   static void computeExclusivePrefixSum( Vector& v,
                                          const typename Vector::IndexType begin,
                                          const typename Vector::IndexType end );

};

template<>
class tnlVectorOperations< tnlCuda >
{
   public:

   template< typename Vector >
   static void addElement( Vector& v,
                           const typename Vector::IndexType i,
                           const typename Vector::RealType& value );

   template< typename Vector >
   static void addElement( Vector& v,
                           const typename Vector::IndexType i,
                           const typename Vector::RealType& value,
                           const typename Vector::RealType& thisElementMultiplicator );

   template< typename Vector >
   static typename Vector::RealType getVectorMax( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorMin( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorAbsMax( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorAbsMin( const Vector& v );

   template< typename Vector >
   static typename Vector::RealType getVectorLpNorm( const Vector& v,
                                                     const typename Vector::RealType& p );

   template< typename Vector >
   static typename Vector::RealType getVectorSum( const Vector& v );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceMax( const Vector1& v1,
                                                             const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceMin( const Vector1& v1,
                                                               const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceAbsMax( const Vector1& v1,
                                                                  const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceAbsMin( const Vector1& v1,
                                                                  const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceLpNorm( const Vector1& v1,
                                                           const Vector2& v2,
                                                           const typename Vector1::RealType& p );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getVectorDifferenceSum( const Vector1& v1,
                                                               const Vector2& v2 );
   template< typename Vector >
   static void vectorScalarMultiplication( Vector& v,
                                           const typename Vector::RealType& alpha );

   template< typename Vector1, typename Vector2 >
   static typename Vector1::RealType getScalarProduct( const Vector1& v1,
                                                         const Vector2& v2 );

   template< typename Vector1, typename Vector2 >
   static void addVector( Vector1& y,
                          const Vector2& x,
                          const typename Vector2::RealType& alpha,
                          const typename Vector1::RealType& thisMultiplicator = 1.0 );
   
   template< typename Vector1, typename Vector2, typename Vector3 >
   static void addVectors( Vector1& v,
                           const Vector2& v1,
                           const typename Vector2::RealType& multiplicator1,
                           const Vector3& v2,
                           const typename Vector3::RealType& multiplicator2,
                           const typename Vector1::RealType& thisMultiplicator = 1.0 );
   

   template< typename Vector1, typename Vector2 >
   static void alphaXPlusBetaY( Vector1& y,
                                const Vector2& x,
                                const typename Vector1::RealType& alpha,
                                const typename Vector1::RealType& beta );

   template< typename Vector1, typename Vector2 >
   static void alphaXPlusBetaZ( Vector1& y,
                                const Vector2& x,
                                const typename Vector1::RealType& alpha,
                                const Vector2& z,
                                const typename Vector1::RealType& beta );

   template< typename Vector1, typename Vector2 >
   static void alphaXPlusBetaZPlusY( Vector1& y,
                                     const Vector2& x,
                                     const typename Vector1::RealType& alpha,
                                     const Vector2& z,
                                     const typename Vector1::RealType& beta );

   template< typename Vector >
   static void computePrefixSum( Vector& v,
                                 const typename Vector::IndexType begin,
                                 const typename Vector::IndexType end );

   template< typename Vector >
   static void computeExclusivePrefixSum( Vector& v,
                                          const typename Vector::IndexType begin,
                                          const typename Vector::IndexType end );
};

#include <core/vectors/tnlVectorOperationsHost_impl.h>
#include <core/vectors/tnlVectorOperationsCuda_impl.h>

#endif /* TNLVECTOROPERATIONS_H_ */
