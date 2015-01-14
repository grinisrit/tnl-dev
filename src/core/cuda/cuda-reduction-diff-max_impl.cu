/***************************************************************************
                          cuda-reduction-diff-max_impl.cu  -  description
                             -------------------
    begin                : Jan 19, 2014
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
 
#include <core/cuda/reduction-operations.h>
#include <core/cuda/cuda-reduction.h>
 
#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

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

template bool reductionOnCudaDevice< tnlParallelReductionDiffMax< long double, int > >
                                   ( const tnlParallelReductionDiffMax< long double, int>& operation,
                                     const typename tnlParallelReductionDiffMax< long double, int > :: IndexType size,
                                     const typename tnlParallelReductionDiffMax< long double, int > :: RealType* deviceInput1,
                                     const typename tnlParallelReductionDiffMax< long double, int > :: RealType* deviceInput2,
                                     typename tnlParallelReductionDiffMax< long double, int> :: ResultType& result );
                                     
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

#endif                                     