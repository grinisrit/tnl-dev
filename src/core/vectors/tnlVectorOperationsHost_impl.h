/***************************************************************************
                          tnlVectorOperationsHost_impl.h  -  description
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

#ifndef TNLVECTOROPERATIONSHOST_IMPL_H_
#define TNLVECTOROPERATIONSHOST_IMPL_H_

static const int OpenMPVectorOperationsThreshold = 65536; // TODO: check this threshold

template< typename Vector >
void tnlVectorOperations< tnlHost >::addElement( Vector& v,
                                                 const typename Vector::IndexType i,
                                                 const typename Vector::RealType& value )
{
   v[ i ] += value;
}

template< typename Vector >
void tnlVectorOperations< tnlHost >::addElement( Vector& v,
                                                 const typename Vector::IndexType i,
                                                 const typename Vector::RealType& value,
                                                 const typename Vector::RealType& thisElementMultiplicator )
{
   v[ i ] = thisElementMultiplicator * v[ i ] + value;
}

template< typename Vector >
typename Vector::RealType tnlVectorOperations< tnlHost >::getVectorMax( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   Real result = v. getElement( 0 );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Max( result, v. getElement( i ) );
   return result;
}

template< typename Vector >
typename Vector :: RealType tnlVectorOperations< tnlHost > :: getVectorMin( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );
   Real result = v. getElement( 0 );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Min( result, v. getElement( i ) );
   return result;
}

template< typename Vector >
typename Vector :: RealType tnlVectorOperations< tnlHost > :: getVectorAbsMax( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   tnlAssert( v. getSize() > 0,
                 cerr << "Vector name is " << v. getName() );
   Real result = fabs( v. getElement( 0 ) );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Max( result, ( Real ) fabs( v. getElement( i ) ) );
   return result;
}


template< typename Vector >
typename Vector :: RealType tnlVectorOperations< tnlHost > :: getVectorAbsMin( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   tnlAssert( v. getSize() > 0,
                 cerr << "Vector name is " << v. getName() );
   Real result = fabs( v. getElement( 0 ) );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result = Min( result, ( Real ) fabs( v. getElement( i ) ) );
   return result;
}


template< typename Vector >
typename Vector :: RealType tnlVectorOperations< tnlHost > :: getVectorLpNorm( const Vector& v,
                                                                               const typename Vector :: RealType& p )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   tnlAssert( v. getSize() > 0,
                 cerr << "Vector name is " << v. getName() );
   tnlAssert( p > 0.0,
              cerr << " p = " << p );
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
typename Vector :: RealType tnlVectorOperations< tnlHost > :: getVectorSum( const Vector& v )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;
   tnlAssert( v. getSize() > 0,
                 cerr << "Vector name is " << v. getName() );

   Real result = v. getElement( 0 );
   const Index n = v. getSize();
   for( Index i = 1; i < n; i ++ )
      result += v. getElement( i );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getVectorDifferenceMax( const Vector1& v1,
                                                                                       const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;
   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );
   Real result = v1. getElement( 0 ) - v2. getElement( 0 );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Max( result, v1. getElement( i ) - v2. getElement( i ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getVectorDifferenceMin( const Vector1& v1,
                                                                                       const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   Real result = v1. getElement( 0 ) - v2. getElement( 0 );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Min( result, v1. getElement( i ) - v2. getElement( i ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getVectorDifferenceAbsMax( const Vector1& v1,
                                                                                          const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   Real result = fabs( v1. getElement( 0 ) - v2. getElement( 0 ) );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Max( result, ( Real ) fabs( v1. getElement( i ) - v2. getElement( i ) ) );
   return result;
}

template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getVectorDifferenceAbsMin( const Vector1& v1,
                                                                                          const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   Real result = fabs( v1. getElement( 0 ) - v2. getElement( 0 ) );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result =  Min( result, ( Real ) fabs( v1. getElement( i ) - v2. getElement( i ) ) );
   return result;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getVectorDifferenceLpNorm( const Vector1& v1,
                                                                                          const Vector2& v2,
                                                                                          const typename Vector1 :: RealType& p )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( p > 0.0,
              cerr << " p = " << p );
   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

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
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getVectorDifferenceSum( const Vector1& v1,
                                                                                       const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   Real result = v1. getElement( 0 ) - v2. getElement( 0 );
   const Index n = v1. getSize();
   for( Index i = 1; i < n; i ++ )
      result += v1. getElement( i ) - v2. getElement( i );
   return result;
}


template< typename Vector >
void tnlVectorOperations< tnlHost > :: vectorScalarMultiplication( Vector& v,
                                                                   const typename Vector :: RealType& alpha )
{
   typedef typename Vector :: RealType Real;
   typedef typename Vector :: IndexType Index;

   tnlAssert( v. getSize() > 0,
              cerr << "Vector name is " << v. getName() );

   const Index n = v. getSize();
   for( Index i = 0; i < n; i ++ )
      v[ i ] *= alpha;
}


template< typename Vector1, typename Vector2 >
typename Vector1 :: RealType tnlVectorOperations< tnlHost > :: getScalarProduct( const Vector1& v1,
                                                                                 const Vector2& v2 )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( v1. getSize() > 0,
              cerr << "Vector name is " << v1. getName() );
   tnlAssert( v1. getSize() == v2. getSize(),
              cerr << "Vector names are " << v1. getName() << " and " << v2. getName() );

   Real result = 0;
   const Index n = v1. getSize();
#ifdef HAVE_OPENMP
#pragma omp parallel for reduction(+:result) if( n > OpenMPVectorOperationsThreshold ) // TODO: check this threshold
#endif     
   for( Index i = 0; i < n; i ++ )
      result += v1[ i ] * v2[ i ];
   return result;
}

template< typename Vector1, typename Vector2 >
void tnlVectorOperations< tnlHost > :: addVector( Vector1& y,
                                                  const Vector2& x,
                                                  const typename Vector2::RealType& alpha,
                                                  const typename Vector1::RealType& thisMultiplicator )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( x. getSize() > 0,
              cerr << "Vector name is " << x. getName() );
   tnlAssert( x. getSize() == y. getSize(),
              cerr << "Vector names are " << x. getName() << " and " << y. getName() );

   const Index n = y. getSize();
   if( thisMultiplicator == 1.0 )
      for( Index i = 0; i < n; i ++ )
         y[ i ] += alpha * x[ i ];
   else
      for( Index i = 0; i < n; i ++ )
         y[ i ] = thisMultiplicator * y[ i ] + alpha * x[ i ];
}

template< typename Vector1,
          typename Vector2,
          typename Vector3 >
void
tnlVectorOperations< tnlHost >::
addVectors( Vector1& v,
            const Vector2& v1,
            const typename Vector2::RealType& multiplicator1,
            const Vector3& v2,
            const typename Vector3::RealType& multiplicator2,
            const typename Vector1::RealType& thisMultiplicator )
{
   typedef typename Vector1 :: RealType Real;
   typedef typename Vector1 :: IndexType Index;

   tnlAssert( v.getSize() > 0,
              cerr << "Vector name is " << v.getName() );
   tnlAssert( v.getSize() == v1.getSize(),
              cerr << "Vector names are " << v.getName() << " and " << v1.getName() );
   tnlAssert( v.getSize() == v2.getSize(),
              cerr << "Vector names are " << v.getName() << " and " << v2.getName() );

   
   const Index n = v.getSize();
   if( thisMultiplicator == 1.0 )
      for( Index i = 0; i < n; i ++ )
         v[ i ] += multiplicator1 * v1[ i ] + multiplicator2 * v2[ i ];
   else
      for( Index i = 0; i < n; i ++ )
         v[ i ] = thisMultiplicator * v[ i ] * multiplicator1 * v1[ i ] + multiplicator2 * v2[ i ];
}

template< typename Vector >
void tnlVectorOperations< tnlHost >::computePrefixSum( Vector& v,
                                                       typename Vector::IndexType begin,
                                                       typename Vector::IndexType end )
{
   typedef typename Vector::IndexType Index;
   for( Index i = begin + 1; i < end; i++ )
      v[ i ] += v[ i - 1 ];
}

template< typename Vector >
void tnlVectorOperations< tnlHost >::computeExclusivePrefixSum( Vector& v,
                                                                typename Vector::IndexType begin,
                                                                typename Vector::IndexType end )
{
   typedef typename Vector::IndexType Index;
   typedef typename Vector::RealType Real;
   Real aux( v[ begin ] );
   v[ begin ] = 0.0;
   for( Index i = begin + 1; i < end; i++ )
   {
      Real x = v[ i ];
      v[ i ] = aux;
      aux += x;
   }
}

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

#include <core/vectors/tnlVector.h>

/****
 * Max
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< int, tnlHost, int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< long int, tnlHost, int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< float, tnlHost, int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< double, tnlHost, int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< long double, tnlHost, int >& v );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< int, tnlHost, long int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< long int, tnlHost, long int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< float, tnlHost, long int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< double, tnlHost, long int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorMax( const tnlVector< long double, tnlHost, long int >& v );
#endif
#endif

/****
 * Min
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< int, tnlHost, int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< long int, tnlHost, int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< float, tnlHost, int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< double, tnlHost, int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< long double, tnlHost, int >& v );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< int, tnlHost, long int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< long int, tnlHost, long int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< float, tnlHost, long int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< double, tnlHost, long int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorMin( const tnlVector< long double, tnlHost, long int >& v );
#endif
#endif

/****
 * Abs max
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< int, tnlHost, int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< long int, tnlHost, int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< float, tnlHost, int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< double, tnlHost, int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< long double, tnlHost, int >& v );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< int, tnlHost, long int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< long int, tnlHost, long int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< float, tnlHost, long int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< double, tnlHost, long int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorAbsMax( const tnlVector< long double, tnlHost, long int >& v );
#endif
#endif

/****
 * Abs min
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< int, tnlHost, int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< long int, tnlHost, int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< float, tnlHost, int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< double, tnlHost, int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< long double, tnlHost, int >& v );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< int, tnlHost, long int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< long int, tnlHost, long int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< float, tnlHost, long int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< double, tnlHost, long int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorAbsMin( const tnlVector< long double, tnlHost, long int >& v );
#endif
#endif

/****
 * Lp norm
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< int, tnlHost, int >& v, const int& p );
extern template long int    tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< long int, tnlHost, int >& v, const long int& p );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< float, tnlHost, int >& v, const float& p );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< double, tnlHost, int >& v, const double& p );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< long double, tnlHost, int >& v, const long double& p );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< int, tnlHost, long int >& v, const int& p );
extern template long int    tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< long int, tnlHost, long int >& v, const long int& p );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< float, tnlHost, long int >& v, const float& p );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< double, tnlHost, long int >& v, const double& p );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorLpNorm( const tnlVector< long double, tnlHost, long int >& v, const long double& p );
#endif
#endif

/****
 * Sum
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< int, tnlHost, int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< long int, tnlHost, int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< float, tnlHost, int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< double, tnlHost, int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< long double, tnlHost, int >& v );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< int, tnlHost, long int >& v );
extern template long int    tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< long int, tnlHost, long int >& v );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< float, tnlHost, long int >& v );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< double, tnlHost, long int >& v );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorSum( const tnlVector< long double, tnlHost, long int >& v );
#endif
#endif

/****
 * Difference max
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< int, tnlHost, int >& v1, const tnlVector< int, tnlHost, int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< long int, tnlHost, int >& v1, const tnlVector< long int, tnlHost, int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< float, tnlHost, int >& v1,  const tnlVector< float, tnlHost, int >& v2);
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< double, tnlHost, int >& v1, const tnlVector< double, tnlHost, int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< long double, tnlHost, int >& v1, const tnlVector< long double, tnlHost, int >& v2 );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< int, tnlHost, long int >& v1, const tnlVector< int, tnlHost, long int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< long int, tnlHost, long int >& v1, const tnlVector< long int, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< float, tnlHost, long int >& v1, const tnlVector< float, tnlHost, long int >& v2 );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< double, tnlHost, long int >& v1, const tnlVector< double, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceMax( const tnlVector< long double, tnlHost, long int >& v1, const tnlVector< long double, tnlHost, long int >& v2 );
#endif
#endif

/****
 * Difference min
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< int, tnlHost, int >& v1, const tnlVector< int, tnlHost, int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< long int, tnlHost, int >& v1, const tnlVector< long int, tnlHost, int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< float, tnlHost, int >& v1,  const tnlVector< float, tnlHost, int >& v2);
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< double, tnlHost, int >& v1, const tnlVector< double, tnlHost, int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< long double, tnlHost, int >& v1, const tnlVector< long double, tnlHost, int >& v2 );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< int, tnlHost, long int >& v1, const tnlVector< int, tnlHost, long int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< long int, tnlHost, long int >& v1, const tnlVector< long int, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< float, tnlHost, long int >& v1, const tnlVector< float, tnlHost, long int >& v2 );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< double, tnlHost, long int >& v1, const tnlVector< double, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceMin( const tnlVector< long double, tnlHost, long int >& v1, const tnlVector< long double, tnlHost, long int >& v2 );
#endif
#endif

/****
 * Difference abs max
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< int, tnlHost, int >& v1, const tnlVector< int, tnlHost, int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< long int, tnlHost, int >& v1, const tnlVector< long int, tnlHost, int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< float, tnlHost, int >& v1,  const tnlVector< float, tnlHost, int >& v2);
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< double, tnlHost, int >& v1, const tnlVector< double, tnlHost, int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< long double, tnlHost, int >& v1, const tnlVector< long double, tnlHost, int >& v2 );
#endif

#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< int, tnlHost, long int >& v1, const tnlVector< int, tnlHost, long int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< long int, tnlHost, long int >& v1, const tnlVector< long int, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< float, tnlHost, long int >& v1, const tnlVector< float, tnlHost, long int >& v2 );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< double, tnlHost, long int >& v1, const tnlVector< double, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMax( const tnlVector< long double, tnlHost, long int >& v1, const tnlVector< long double, tnlHost, long int >& v2 );
#endif
#endif

/****
 * Difference abs min
 */
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< int, tnlHost, int >& v1, const tnlVector< int, tnlHost, int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< long int, tnlHost, int >& v1, const tnlVector< long int, tnlHost, int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< float, tnlHost, int >& v1,  const tnlVector< float, tnlHost, int >& v2);
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< double, tnlHost, int >& v1, const tnlVector< double, tnlHost, int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< long double, tnlHost, int >& v1, const tnlVector< long double, tnlHost, int >& v2 );
#endif


#ifdef INSTANTIATE_LONG_INT
extern template int         tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< int, tnlHost, long int >& v1, const tnlVector< int, tnlHost, long int >& v2 );
extern template long int    tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< long int, tnlHost, long int >& v1, const tnlVector< long int, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_FLOAT
extern template float       tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< float, tnlHost, long int >& v1, const tnlVector< float, tnlHost, long int >& v2 );
#endif
extern template double      tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< double, tnlHost, long int >& v1, const tnlVector< double, tnlHost, long int >& v2 );
#ifdef INSTANTIATE_LONG_DOUBLE
extern template long double tnlVectorOperations< tnlHost >::getVectorDifferenceAbsMin( const tnlVector< long double, tnlHost, long int >& v1, const tnlVector< long double, tnlHost, long int >& v2 );
#endif
#endif

#endif


#endif /* TNLVECTOROPERATIONSHOST_IMPL_H_ */
