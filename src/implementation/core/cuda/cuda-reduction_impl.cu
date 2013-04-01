/***************************************************************************
                          cuda-reduction_impl.cu  -  description
                             -------------------
    begin                : Mar 24, 2013
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
 
#include <core/cuda/reduction-operations.h>
#include <core/cuda/cuda-reduction.h>
 
#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

/****
 * Sum 
 */

template bool reductionOnCudaDevice< tnlParallelReductionSum< char, int > >
                                   ( const tnlParallelReductionSum< char, int >& operation,
                                     const typename tnlParallelReductionSum< char, int > :: IndexType size,
                                     const typename tnlParallelReductionSum< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSum< int, int > >
                                   ( const tnlParallelReductionSum< int, int >& operation,
                                     const typename tnlParallelReductionSum< int, int > :: IndexType size,
                                     const typename tnlParallelReductionSum< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSum< float, int > >
                                   ( const tnlParallelReductionSum< float, int >& operation,
                                     const typename tnlParallelReductionSum< float, int > :: IndexType size,
                                     const typename tnlParallelReductionSum< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSum< double, int > >
                                   ( const tnlParallelReductionSum< double, int>& operation,
                                     const typename tnlParallelReductionSum< double, int > :: IndexType size,
                                     const typename tnlParallelReductionSum< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionSum< long double, int > >
                                   ( const tnlParallelReductionSum< long double, int>& operation,
                                     const typename tnlParallelReductionSum< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionSum< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionSum< char, long int > >
                                   ( const tnlParallelReductionSum< char, long int >& operation,
                                     const typename tnlParallelReductionSum< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionSum< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSum< int, long int > >
                                   ( const tnlParallelReductionSum< int, long int >& operation,
                                     const typename tnlParallelReductionSum< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionSum< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSum< float, long int > >
                                   ( const tnlParallelReductionSum< float, long int >& operation,
                                     const typename tnlParallelReductionSum< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionSum< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSum< double, long int > >
                                   ( const tnlParallelReductionSum< double, long int>& operation,
                                     const typename tnlParallelReductionSum< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionSum< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionSum< long double, long int > >
                                   ( const tnlParallelReductionSum< long double, long int>& operation,
                                     const typename tnlParallelReductionSum< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionSum< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSum< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSum< long double, long int> :: ResultType& result );*/

/****
 * Min
 */

template bool reductionOnCudaDevice< tnlParallelReductionMin< char, int > >
                                   ( const tnlParallelReductionMin< char, int >& operation,
                                     const typename tnlParallelReductionMin< char, int > :: IndexType size,
                                     const typename tnlParallelReductionMin< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMin< int, int > >
                                   ( const tnlParallelReductionMin< int, int >& operation,
                                     const typename tnlParallelReductionMin< int, int > :: IndexType size,
                                     const typename tnlParallelReductionMin< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMin< float, int > >
                                   ( const tnlParallelReductionMin< float, int >& operation,
                                     const typename tnlParallelReductionMin< float, int > :: IndexType size,
                                     const typename tnlParallelReductionMin< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMin< double, int > >
                                   ( const tnlParallelReductionMin< double, int>& operation,
                                     const typename tnlParallelReductionMin< double, int > :: IndexType size,
                                     const typename tnlParallelReductionMin< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionMin< long double, int > >
                                   ( const tnlParallelReductionMin< long double, int>& operation,
                                     const typename tnlParallelReductionMin< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionMin< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionMin< char, long int > >
                                   ( const tnlParallelReductionMin< char, long int >& operation,
                                     const typename tnlParallelReductionMin< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionMin< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMin< int, long int > >
                                   ( const tnlParallelReductionMin< int, long int >& operation,
                                     const typename tnlParallelReductionMin< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionMin< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMin< float, long int > >
                                   ( const tnlParallelReductionMin< float, long int >& operation,
                                     const typename tnlParallelReductionMin< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionMin< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMin< double, long int > >
                                   ( const tnlParallelReductionMin< double, long int>& operation,
                                     const typename tnlParallelReductionMin< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionMin< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionMin< long double, long int > >
                                   ( const tnlParallelReductionMin< long double, long int>& operation,
                                     const typename tnlParallelReductionMin< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionMin< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMin< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMin< long double, long int> :: ResultType& result );*/

/****
 * Max
 */

template bool reductionOnCudaDevice< tnlParallelReductionMax< char, int > >
                                   ( const tnlParallelReductionMax< char, int >& operation,
                                     const typename tnlParallelReductionMax< char, int > :: IndexType size,
                                     const typename tnlParallelReductionMax< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMax< int, int > >
                                   ( const tnlParallelReductionMax< int, int >& operation,
                                     const typename tnlParallelReductionMax< int, int > :: IndexType size,
                                     const typename tnlParallelReductionMax< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMax< float, int > >
                                   ( const tnlParallelReductionMax< float, int >& operation,
                                     const typename tnlParallelReductionMax< float, int > :: IndexType size,
                                     const typename tnlParallelReductionMax< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMax< double, int > >
                                   ( const tnlParallelReductionMax< double, int>& operation,
                                     const typename tnlParallelReductionMax< double, int > :: IndexType size,
                                     const typename tnlParallelReductionMax< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionMax< long double, int > >
                                   ( const tnlParallelReductionMax< long double, int>& operation,
                                     const typename tnlParallelReductionMax< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionMax< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionMax< char, long int > >
                                   ( const tnlParallelReductionMax< char, long int >& operation,
                                     const typename tnlParallelReductionMax< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionMax< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMax< int, long int > >
                                   ( const tnlParallelReductionMax< int, long int >& operation,
                                     const typename tnlParallelReductionMax< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionMax< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMax< float, long int > >
                                   ( const tnlParallelReductionMax< float, long int >& operation,
                                     const typename tnlParallelReductionMax< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionMax< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionMax< double, long int > >
                                   ( const tnlParallelReductionMax< double, long int>& operation,
                                     const typename tnlParallelReductionMax< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionMax< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionMax< long double, long int > >
                                   ( const tnlParallelReductionMax< long double, long int>& operation,
                                     const typename tnlParallelReductionMax< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionMax< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionMax< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionMax< long double, long int> :: ResultType& result );*/

/****
 * Abs sum
 */

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< char, int > >
                                   ( const tnlParallelReductionAbsSum< char, int >& operation,
                                     const typename tnlParallelReductionAbsSum< char, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< int, int > >
                                   ( const tnlParallelReductionAbsSum< int, int >& operation,
                                     const typename tnlParallelReductionAbsSum< int, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< float, int > >
                                   ( const tnlParallelReductionAbsSum< float, int >& operation,
                                     const typename tnlParallelReductionAbsSum< float, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< double, int > >
                                   ( const tnlParallelReductionAbsSum< double, int>& operation,
                                     const typename tnlParallelReductionAbsSum< double, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< long double, int > >
                                   ( const tnlParallelReductionAbsSum< long double, int>& operation,
                                     const typename tnlParallelReductionAbsSum< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< char, long int > >
                                   ( const tnlParallelReductionAbsSum< char, long int >& operation,
                                     const typename tnlParallelReductionAbsSum< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< int, long int > >
                                   ( const tnlParallelReductionAbsSum< int, long int >& operation,
                                     const typename tnlParallelReductionAbsSum< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< float, long int > >
                                   ( const tnlParallelReductionAbsSum< float, long int >& operation,
                                     const typename tnlParallelReductionAbsSum< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< double, long int > >
                                   ( const tnlParallelReductionAbsSum< double, long int>& operation,
                                     const typename tnlParallelReductionAbsSum< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionAbsSum< long double, long int > >
                                   ( const tnlParallelReductionAbsSum< long double, long int>& operation,
                                     const typename tnlParallelReductionAbsSum< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsSum< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsSum< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsSum< long double, long int> :: ResultType& result );*/

/****
 * Abs min
 */

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< char, int > >
                                   ( const tnlParallelReductionAbsMin< char, int >& operation,
                                     const typename tnlParallelReductionAbsMin< char, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< int, int > >
                                   ( const tnlParallelReductionAbsMin< int, int >& operation,
                                     const typename tnlParallelReductionAbsMin< int, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< float, int > >
                                   ( const tnlParallelReductionAbsMin< float, int >& operation,
                                     const typename tnlParallelReductionAbsMin< float, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< double, int > >
                                   ( const tnlParallelReductionAbsMin< double, int>& operation,
                                     const typename tnlParallelReductionAbsMin< double, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< long double, int > >
                                   ( const tnlParallelReductionAbsMin< long double, int>& operation,
                                     const typename tnlParallelReductionAbsMin< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< char, long int > >
                                   ( const tnlParallelReductionAbsMin< char, long int >& operation,
                                     const typename tnlParallelReductionAbsMin< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< int, long int > >
                                   ( const tnlParallelReductionAbsMin< int, long int >& operation,
                                     const typename tnlParallelReductionAbsMin< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< float, long int > >
                                   ( const tnlParallelReductionAbsMin< float, long int >& operation,
                                     const typename tnlParallelReductionAbsMin< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< double, long int > >
                                   ( const tnlParallelReductionAbsMin< double, long int>& operation,
                                     const typename tnlParallelReductionAbsMin< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionAbsMin< long double, long int > >
                                   ( const tnlParallelReductionAbsMin< long double, long int>& operation,
                                     const typename tnlParallelReductionAbsMin< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMin< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMin< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMin< long double, long int> :: ResultType& result );*/
/****
 * Abs max
 */

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< char, int > >
                                   ( const tnlParallelReductionAbsMax< char, int >& operation,
                                     const typename tnlParallelReductionAbsMax< char, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< int, int > >
                                   ( const tnlParallelReductionAbsMax< int, int >& operation,
                                     const typename tnlParallelReductionAbsMax< int, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< float, int > >
                                   ( const tnlParallelReductionAbsMax< float, int >& operation,
                                     const typename tnlParallelReductionAbsMax< float, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< double, int > >
                                   ( const tnlParallelReductionAbsMax< double, int>& operation,
                                     const typename tnlParallelReductionAbsMax< double, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< long double, int > >
                                   ( const tnlParallelReductionAbsMax< long double, int>& operation,
                                     const typename tnlParallelReductionAbsMax< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< char, long int > >
                                   ( const tnlParallelReductionAbsMax< char, long int >& operation,
                                     const typename tnlParallelReductionAbsMax< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< int, long int > >
                                   ( const tnlParallelReductionAbsMax< int, long int >& operation,
                                     const typename tnlParallelReductionAbsMax< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< float, long int > >
                                   ( const tnlParallelReductionAbsMax< float, long int >& operation,
                                     const typename tnlParallelReductionAbsMax< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< double, long int > >
                                   ( const tnlParallelReductionAbsMax< double, long int>& operation,
                                     const typename tnlParallelReductionAbsMax< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionAbsMax< long double, long int > >
                                   ( const tnlParallelReductionAbsMax< long double, long int>& operation,
                                     const typename tnlParallelReductionAbsMax< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionAbsMax< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionAbsMax< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionAbsMax< long double, long int> :: ResultType& result );*/

/****
 * Logical AND
 */
template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< char, int > >
                                   ( const tnlParallelReductionLogicalAnd< char, int >& operation,
                                     const typename tnlParallelReductionLogicalAnd< char, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< int, int > >
                                   ( const tnlParallelReductionLogicalAnd< int, int >& operation,
                                     const typename tnlParallelReductionLogicalAnd< int, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< float, int > >
                                   ( const tnlParallelReductionLogicalAnd< float, int >& operation,
                                     const typename tnlParallelReductionLogicalAnd< float, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< double, int > >
                                   ( const tnlParallelReductionLogicalAnd< double, int>& operation,
                                     const typename tnlParallelReductionLogicalAnd< double, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< long double, int > >
                                   ( const tnlParallelReductionLogicalAnd< long double, int>& operation,
                                     const typename tnlParallelReductionLogicalAnd< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< char, long int > >
                                   ( const tnlParallelReductionLogicalAnd< char, long int >& operation,
                                     const typename tnlParallelReductionLogicalAnd< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< int, long int > >
                                   ( const tnlParallelReductionLogicalAnd< int, long int >& operation,
                                     const typename tnlParallelReductionLogicalAnd< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< float, long int > >
                                   ( const tnlParallelReductionLogicalAnd< float, long int >& operation,
                                     const typename tnlParallelReductionLogicalAnd< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< double, long int > >
                                   ( const tnlParallelReductionLogicalAnd< double, long int>& operation,
                                     const typename tnlParallelReductionLogicalAnd< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionLogicalAnd< long double, long int > >
                                   ( const tnlParallelReductionLogicalAnd< long double, long int>& operation,
                                     const typename tnlParallelReductionLogicalAnd< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalAnd< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalAnd< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalAnd< long double, long int> :: ResultType& result );*/

/****
 * Logical OR
 */
template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< char, int > >
                                   ( const tnlParallelReductionLogicalOr< char, int >& operation,
                                     const typename tnlParallelReductionLogicalOr< char, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< int, int > >
                                   ( const tnlParallelReductionLogicalOr< int, int >& operation,
                                     const typename tnlParallelReductionLogicalOr< int, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< float, int > >
                                   ( const tnlParallelReductionLogicalOr< float, int >& operation,
                                     const typename tnlParallelReductionLogicalOr< float, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< double, int > >
                                   ( const tnlParallelReductionLogicalOr< double, int>& operation,
                                     const typename tnlParallelReductionLogicalOr< double, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< long double, int > >
                                   ( const tnlParallelReductionLogicalOr< long double, int>& operation,
                                     const typename tnlParallelReductionLogicalOr< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< char, long int > >
                                   ( const tnlParallelReductionLogicalOr< char, long int >& operation,
                                     const typename tnlParallelReductionLogicalOr< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< int, long int > >
                                   ( const tnlParallelReductionLogicalOr< int, long int >& operation,
                                     const typename tnlParallelReductionLogicalOr< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< float, long int > >
                                   ( const tnlParallelReductionLogicalOr< float, long int >& operation,
                                     const typename tnlParallelReductionLogicalOr< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< double, long int > >
                                   ( const tnlParallelReductionLogicalOr< double, long int>& operation,
                                     const typename tnlParallelReductionLogicalOr< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionLogicalOr< long double, long int > >
                                   ( const tnlParallelReductionLogicalOr< long double, long int>& operation,
                                     const typename tnlParallelReductionLogicalOr< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionLogicalOr< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLogicalOr< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLogicalOr< long double, long int> :: ResultType& result );*/


/****
 * Lp Norm
 */
template bool reductionOnCudaDevice< tnlParallelReductionLpNorm< float, int > >
                                   ( const tnlParallelReductionLpNorm< float, int >& operation,
                                     const typename tnlParallelReductionLpNorm< float, int > :: IndexType size,
                                     const typename tnlParallelReductionLpNorm< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLpNorm< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLpNorm< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLpNorm< double, int > >
                                   ( const tnlParallelReductionLpNorm< double, int>& operation,
                                     const typename tnlParallelReductionLpNorm< double, int > :: IndexType size,
                                     const typename tnlParallelReductionLpNorm< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLpNorm< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLpNorm< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionLpNorm< long double, int > >
                                   ( const tnlParallelReductionLpNorm< long double, int>& operation,
                                     const typename tnlParallelReductionLpNorm< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionLpNorm< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLpNorm< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLpNorm< long double, int> :: ResultType& result );*/



template bool reductionOnCudaDevice< tnlParallelReductionLpNorm< float, long int > >
                                   ( const tnlParallelReductionLpNorm< float, long int >& operation,
                                     const typename tnlParallelReductionLpNorm< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionLpNorm< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLpNorm< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLpNorm< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionLpNorm< double, long int > >
                                   ( const tnlParallelReductionLpNorm< double, long int>& operation,
                                     const typename tnlParallelReductionLpNorm< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionLpNorm< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLpNorm< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLpNorm< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionLpNorm< long double, long int > >
                                   ( const tnlParallelReductionLpNorm< long double, long int>& operation,
                                     const typename tnlParallelReductionLpNorm< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionLpNorm< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionLpNorm< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionLpNorm< long double, long int> :: ResultType& result );*/


/****
 * Equalities
 */
template bool reductionOnCudaDevice< tnlParallelReductionEqualities< char, int > >
                                   ( const tnlParallelReductionEqualities< char, int >& operation,
                                     const typename tnlParallelReductionEqualities< char, int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< int, int > >
                                   ( const tnlParallelReductionEqualities< int, int >& operation,
                                     const typename tnlParallelReductionEqualities< int, int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< float, int > >
                                   ( const tnlParallelReductionEqualities< float, int >& operation,
                                     const typename tnlParallelReductionEqualities< float, int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< double, int > >
                                   ( const tnlParallelReductionEqualities< double, int>& operation,
                                     const typename tnlParallelReductionEqualities< double, int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionEqualities< long double, int > >
                                   ( const tnlParallelReductionEqualities< long double, int>& operation,
                                     const typename tnlParallelReductionEqualities< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< char, long int > >
                                   ( const tnlParallelReductionEqualities< char, long int >& operation,
                                     const typename tnlParallelReductionEqualities< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< int, long int > >
                                   ( const tnlParallelReductionEqualities< int, long int >& operation,
                                     const typename tnlParallelReductionEqualities< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< float, long int > >
                                   ( const tnlParallelReductionEqualities< float, long int >& operation,
                                     const typename tnlParallelReductionEqualities< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionEqualities< double, long int > >
                                   ( const tnlParallelReductionEqualities< double, long int>& operation,
                                     const typename tnlParallelReductionEqualities< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionEqualities< long double, long int > >
                                   ( const tnlParallelReductionEqualities< long double, long int>& operation,
                                     const typename tnlParallelReductionEqualities< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionEqualities< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionEqualities< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionEqualities< long double, long int> :: ResultType& result );*/


/****
 * Inequalities
 */
template bool reductionOnCudaDevice< tnlParallelReductionInequalities< char, int > >
                                   ( const tnlParallelReductionInequalities< char, int >& operation,
                                     const typename tnlParallelReductionInequalities< char, int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< int, int > >
                                   ( const tnlParallelReductionInequalities< int, int >& operation,
                                     const typename tnlParallelReductionInequalities< int, int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< float, int > >
                                   ( const tnlParallelReductionInequalities< float, int >& operation,
                                     const typename tnlParallelReductionInequalities< float, int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< double, int > >
                                   ( const tnlParallelReductionInequalities< double, int>& operation,
                                     const typename tnlParallelReductionInequalities< double, int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionInequalities< long double, int > >
                                   ( const tnlParallelReductionInequalities< long double, int>& operation,
                                     const typename tnlParallelReductionInequalities< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< char, long int > >
                                   ( const tnlParallelReductionInequalities< char, long int >& operation,
                                     const typename tnlParallelReductionInequalities< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< int, long int > >
                                   ( const tnlParallelReductionInequalities< int, long int >& operation,
                                     const typename tnlParallelReductionInequalities< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< float, long int > >
                                   ( const tnlParallelReductionInequalities< float, long int >& operation,
                                     const typename tnlParallelReductionInequalities< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionInequalities< double, long int > >
                                   ( const tnlParallelReductionInequalities< double, long int>& operation,
                                     const typename tnlParallelReductionInequalities< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionInequalities< long double, long int > >
                                   ( const tnlParallelReductionInequalities< long double, long int>& operation,
                                     const typename tnlParallelReductionInequalities< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionInequalities< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionInequalities< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionInequalities< long double, long int> :: ResultType& result );*/


/****
 * Sdot
 */
template bool reductionOnCudaDevice< tnlParallelReductionSdot< char, int > >
                                   ( const tnlParallelReductionSdot< char, int >& operation,
                                     const typename tnlParallelReductionSdot< char, int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSdot< int, int > >
                                   ( const tnlParallelReductionSdot< int, int >& operation,
                                     const typename tnlParallelReductionSdot< int, int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSdot< float, int > >
                                   ( const tnlParallelReductionSdot< float, int >& operation,
                                     const typename tnlParallelReductionSdot< float, int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSdot< double, int > >
                                   ( const tnlParallelReductionSdot< double, int>& operation,
                                     const typename tnlParallelReductionSdot< double, int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionSdot< long double, int > >
                                   ( const tnlParallelReductionSdot< long double, int>& operation,
                                     const typename tnlParallelReductionSdot< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionSdot< char, long int > >
                                   ( const tnlParallelReductionSdot< char, long int >& operation,
                                     const typename tnlParallelReductionSdot< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSdot< int, long int > >
                                   ( const tnlParallelReductionSdot< int, long int >& operation,
                                     const typename tnlParallelReductionSdot< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSdot< float, long int > >
                                   ( const tnlParallelReductionSdot< float, long int >& operation,
                                     const typename tnlParallelReductionSdot< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionSdot< double, long int > >
                                   ( const tnlParallelReductionSdot< double, long int>& operation,
                                     const typename tnlParallelReductionSdot< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionSdot< long double, long int > >
                                   ( const tnlParallelReductionSdot< long double, long int>& operation,
                                     const typename tnlParallelReductionSdot< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionSdot< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionSdot< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionSdot< long double, long int> :: ResultType& result );*/

/****
 * Diff sum
 */

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< char, int > >
                                   ( const tnlParallelReductionDiffSum< char, int >& operation,
                                     const typename tnlParallelReductionDiffSum< char, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< int, int > >
                                   ( const tnlParallelReductionDiffSum< int, int >& operation,
                                     const typename tnlParallelReductionDiffSum< int, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< float, int > >
                                   ( const tnlParallelReductionDiffSum< float, int >& operation,
                                     const typename tnlParallelReductionDiffSum< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< double, int > >
                                   ( const tnlParallelReductionDiffSum< double, int>& operation,
                                     const typename tnlParallelReductionDiffSum< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< long double, int > >
                                   ( const tnlParallelReductionDiffSum< long double, int>& operation,
                                     const typename tnlParallelReductionDiffSum< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< char, long int > >
                                   ( const tnlParallelReductionDiffSum< char, long int >& operation,
                                     const typename tnlParallelReductionDiffSum< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< int, long int > >
                                   ( const tnlParallelReductionDiffSum< int, long int >& operation,
                                     const typename tnlParallelReductionDiffSum< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< float, long int > >
                                   ( const tnlParallelReductionDiffSum< float, long int >& operation,
                                     const typename tnlParallelReductionDiffSum< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< double, long int > >
                                   ( const tnlParallelReductionDiffSum< double, long int>& operation,
                                     const typename tnlParallelReductionDiffSum< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffSum< long double, long int > >
                                   ( const tnlParallelReductionDiffSum< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffSum< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffSum< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffSum< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffSum< long double, long int> :: ResultType& result );*/

/****
 * Diff min
 */

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< char, int > >
                                   ( const tnlParallelReductionDiffMin< char, int >& operation,
                                     const typename tnlParallelReductionDiffMin< char, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< int, int > >
                                   ( const tnlParallelReductionDiffMin< int, int >& operation,
                                     const typename tnlParallelReductionDiffMin< int, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< float, int > >
                                   ( const tnlParallelReductionDiffMin< float, int >& operation,
                                     const typename tnlParallelReductionDiffMin< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< double, int > >
                                   ( const tnlParallelReductionDiffMin< double, int>& operation,
                                     const typename tnlParallelReductionDiffMin< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< long double, int > >
                                   ( const tnlParallelReductionDiffMin< long double, int>& operation,
                                     const typename tnlParallelReductionDiffMin< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< char, long int > >
                                   ( const tnlParallelReductionDiffMin< char, long int >& operation,
                                     const typename tnlParallelReductionDiffMin< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< int, long int > >
                                   ( const tnlParallelReductionDiffMin< int, long int >& operation,
                                     const typename tnlParallelReductionDiffMin< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< float, long int > >
                                   ( const tnlParallelReductionDiffMin< float, long int >& operation,
                                     const typename tnlParallelReductionDiffMin< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< double, long int > >
                                   ( const tnlParallelReductionDiffMin< double, long int>& operation,
                                     const typename tnlParallelReductionDiffMin< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffMin< long double, long int > >
                                   ( const tnlParallelReductionDiffMin< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffMin< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMin< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMin< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMin< long double, long int> :: ResultType& result );*/

/****
 * Diff max
 */

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< char, int > >
                                   ( const tnlParallelReductionDiffMax< char, int >& operation,
                                     const typename tnlParallelReductionDiffMax< char, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< int, int > >
                                   ( const tnlParallelReductionDiffMax< int, int >& operation,
                                     const typename tnlParallelReductionDiffMax< int, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< float, int > >
                                   ( const tnlParallelReductionDiffMax< float, int >& operation,
                                     const typename tnlParallelReductionDiffMax< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< double, int > >
                                   ( const tnlParallelReductionDiffMax< double, int>& operation,
                                     const typename tnlParallelReductionDiffMax< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< long double, int > >
                                   ( const tnlParallelReductionDiffMax< long double, int>& operation,
                                     const typename tnlParallelReductionDiffMax< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< char, long int > >
                                   ( const tnlParallelReductionDiffMax< char, long int >& operation,
                                     const typename tnlParallelReductionDiffMax< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< int, long int > >
                                   ( const tnlParallelReductionDiffMax< int, long int >& operation,
                                     const typename tnlParallelReductionDiffMax< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< float, long int > >
                                   ( const tnlParallelReductionDiffMax< float, long int >& operation,
                                     const typename tnlParallelReductionDiffMax< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< double, long int > >
                                   ( const tnlParallelReductionDiffMax< double, long int>& operation,
                                     const typename tnlParallelReductionDiffMax< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< long double, long int > >
                                   ( const tnlParallelReductionDiffMax< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffMax< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< long double, long int> :: ResultType& result );*/

/****
 * Diff abs sum
 */

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< char, int > >
                                   ( const tnlParallelReductionDiffAbsSum< char, int >& operation,
                                     const typename tnlParallelReductionDiffAbsSum< char, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< int, int > >
                                   ( const tnlParallelReductionDiffAbsSum< int, int >& operation,
                                     const typename tnlParallelReductionDiffAbsSum< int, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< float, int > >
                                   ( const tnlParallelReductionDiffAbsSum< float, int >& operation,
                                     const typename tnlParallelReductionDiffAbsSum< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< double, int > >
                                   ( const tnlParallelReductionDiffAbsSum< double, int>& operation,
                                     const typename tnlParallelReductionDiffAbsSum< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< long double, int > >
                                   ( const tnlParallelReductionDiffAbsSum< long double, int>& operation,
                                     const typename tnlParallelReductionDiffAbsSum< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< char, long int > >
                                   ( const tnlParallelReductionDiffAbsSum< char, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsSum< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< int, long int > >
                                   ( const tnlParallelReductionDiffAbsSum< int, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsSum< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< float, long int > >
                                   ( const tnlParallelReductionDiffAbsSum< float, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsSum< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< double, long int > >
                                   ( const tnlParallelReductionDiffAbsSum< double, long int>& operation,
                                     const typename tnlParallelReductionDiffAbsSum< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsSum< long double, long int > >
                                   ( const tnlParallelReductionDiffAbsSum< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffAbsSum< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsSum< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsSum< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsSum< long double, long int> :: ResultType& result );*/

/****
 * Diff abs min
 */

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< char, int > >
                                   ( const tnlParallelReductionDiffAbsMin< char, int >& operation,
                                     const typename tnlParallelReductionDiffAbsMin< char, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< int, int > >
                                   ( const tnlParallelReductionDiffAbsMin< int, int >& operation,
                                     const typename tnlParallelReductionDiffAbsMin< int, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< float, int > >
                                   ( const tnlParallelReductionDiffAbsMin< float, int >& operation,
                                     const typename tnlParallelReductionDiffAbsMin< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< double, int > >
                                   ( const tnlParallelReductionDiffAbsMin< double, int>& operation,
                                     const typename tnlParallelReductionDiffAbsMin< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< long double, int > >
                                   ( const tnlParallelReductionDiffAbsMin< long double, int>& operation,
                                     const typename tnlParallelReductionDiffAbsMin< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< char, long int > >
                                   ( const tnlParallelReductionDiffAbsMin< char, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsMin< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< int, long int > >
                                   ( const tnlParallelReductionDiffAbsMin< int, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsMin< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< float, long int > >
                                   ( const tnlParallelReductionDiffAbsMin< float, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsMin< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< double, long int > >
                                   ( const tnlParallelReductionDiffAbsMin< double, long int>& operation,
                                     const typename tnlParallelReductionDiffAbsMin< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMin< long double, long int > >
                                   ( const tnlParallelReductionDiffAbsMin< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffAbsMin< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMin< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMin< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMin< long double, long int> :: ResultType& result );*/
/****
 * Diff abs max
 */

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< char, int > >
                                   ( const tnlParallelReductionDiffAbsMax< char, int >& operation,
                                     const typename tnlParallelReductionDiffAbsMax< char, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< char, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< char, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< char, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< int, int > >
                                   ( const tnlParallelReductionDiffAbsMax< int, int >& operation,
                                     const typename tnlParallelReductionDiffAbsMax< int, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< int, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< int, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< int, int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< float, int > >
                                   ( const tnlParallelReductionDiffAbsMax< float, int >& operation,
                                     const typename tnlParallelReductionDiffAbsMax< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< double, int > >
                                   ( const tnlParallelReductionDiffAbsMax< double, int>& operation,
                                     const typename tnlParallelReductionDiffAbsMax< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< long double, int > >
                                   ( const tnlParallelReductionDiffAbsMax< long double, int>& operation,
                                     const typename tnlParallelReductionDiffAbsMax< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< char, long int > >
                                   ( const tnlParallelReductionDiffAbsMax< char, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsMax< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< int, long int > >
                                   ( const tnlParallelReductionDiffAbsMax< int, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsMax< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< float, long int > >
                                   ( const tnlParallelReductionDiffAbsMax< float, long int >& operation,
                                     const typename tnlParallelReductionDiffAbsMax< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< double, long int > >
                                   ( const tnlParallelReductionDiffAbsMax< double, long int>& operation,
                                     const typename tnlParallelReductionDiffAbsMax< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffAbsMax< long double, long int > >
                                   ( const tnlParallelReductionDiffAbsMax< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffAbsMax< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffAbsMax< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffAbsMax< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffAbsMax< long double, long int> :: ResultType& result );*/



/****
 * Diff Lp Norm
 */
template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< float, int > >
                                   ( const tnlParallelReductionDiffLpNorm< float, int >& operation,
                                     const typename tnlParallelReductionDiffLpNorm< float, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< float, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< float, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< float, int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< double, int > >
                                   ( const tnlParallelReductionDiffLpNorm< double, int>& operation,
                                     const typename tnlParallelReductionDiffLpNorm< double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< double, int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< long double, int > >
                                   ( const tnlParallelReductionDiffLpNorm< long double, int>& operation,
                                     const typename tnlParallelReductionDiffLpNorm< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< long double, int> :: ResultType& result );*/

template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< char, long int > >
                                   ( const tnlParallelReductionDiffLpNorm< char, long int >& operation,
                                     const typename tnlParallelReductionDiffLpNorm< char, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< char, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< char, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< char, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< int, long int > >
                                   ( const tnlParallelReductionDiffLpNorm< int, long int >& operation,
                                     const typename tnlParallelReductionDiffLpNorm< int, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< int, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< int, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< int, long int > :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< float, long int > >
                                   ( const tnlParallelReductionDiffLpNorm< float, long int >& operation,
                                     const typename tnlParallelReductionDiffLpNorm< float, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< float, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< float, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< float, long int> :: ResultType& result );

template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< double, long int > >
                                   ( const tnlParallelReductionDiffLpNorm< double, long int>& operation,
                                     const typename tnlParallelReductionDiffLpNorm< double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< double, long int> :: ResultType& result );

/*template bool reductionOnCudaDevice< tnlParallelReductionDiffLpNorm< long double, long int > >
                                   ( const tnlParallelReductionDiffLpNorm< long double, long int>& operation,
                                     const typename tnlParallelReductionDiffLpNorm< long double, long int > :: IndexType size,
                                     const typename tnlParallelReductionDiffLpNorm< long double, long int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffLpNorm< long double, long int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffLpNorm< long double, long int> :: ResultType& result );*/



#endif
