/***************************************************************************
                          ILU0_impl.h  -  description
                             -------------------
    begin                : Dec 24, 2016
    copyright            : (C) 2016 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky

#pragma once

#include "ILU0.h"
#include "TriangularSolve.h"

#include <TNL/Exceptions/CudaSupportMissing.h>

namespace TNL {
namespace Solvers {
namespace Linear {
namespace Preconditioners {

template< typename Matrix, typename Real, typename Index >
void
ILU0_impl< Matrix, Real, Devices::Host, Index >::
update( const MatrixPointer& matrixPointer )
{
   TNL_ASSERT_GT( matrixPointer->getRows(), 0, "empty matrix" );
   TNL_ASSERT_EQ( matrixPointer->getRows(), matrixPointer->getColumns(), "matrix must be square" );

   const auto& localMatrix = Traits< Matrix >::getLocalMatrix( *matrixPointer );
   const IndexType N = localMatrix.getRows();
   const IndexType minColumn = getMinColumn( *matrixPointer );

   L.setDimensions( N, N );
   U.setDimensions( N, N );

   // copy row lengths
   typename decltype(L)::CompressedRowLengthsVector L_rowLengths;
   typename decltype(U)::CompressedRowLengthsVector U_rowLengths;
   L_rowLengths.setSize( N );
   U_rowLengths.setSize( N );
   for( IndexType i = 0; i < N; i++ ) {
      const auto row = localMatrix.getRow( i );
      const auto max_length = localMatrix.getRowLength( i );
      IndexType L_entries = 0;
      IndexType U_entries = 0;
      for( IndexType j = 0; j < max_length; j++ ) {
         const auto column = row.getElementColumn( j );
         if( column < minColumn )
            continue;
         if( column < i + minColumn )
            L_entries++;
         else if( column < N + minColumn )
            U_entries++;
         else
            break;
      }
      L_rowLengths[ i ] = L_entries;
      U_rowLengths[ N - 1 - i ] = U_entries;
   }
   L.setCompressedRowLengths( L_rowLengths );
   U.setCompressedRowLengths( U_rowLengths );

   // Incomplete LU factorization
   // The factors L and U are stored separately and the rows of U are reversed.
   for( IndexType i = 0; i < N; i++ ) {
      // copy all non-zero entries from A into L and U
      const auto max_length = localMatrix.getRowLength( i );
      IndexType all_columns[ max_length ];
      RealType all_values[ max_length ];
      localMatrix.getRowFast( i, all_columns, all_values );

      // skip non-local elements
      IndexType* columns = all_columns;
      RealType* values = all_values;
      while( columns[0] < minColumn ) {
         columns++;
         values++;
      }

      // update column column indices
      if( minColumn > 0 )
         for( IndexType c_j = 0; c_j < max_length; c_j++ )
            all_columns[ c_j ] -= minColumn;

      const auto L_entries = L_rowLengths[ i ];
      const auto U_entries = U_rowLengths[ N - 1 - i ];
      L.setRow( i, columns, values, L_entries );
      U.setRow( N - 1 - i, &columns[ L_entries ], &values[ L_entries ], U_entries );

      // this condition is to avoid segfaults on empty L.getRow( i )
      if( L_entries > 0 ) {
         const auto L_i = L.getRow( i );
         const auto U_i = U.getRow( N - 1 - i );

         // loop for k = 0, ..., i - 1; but only over the non-zero entries
         for( IndexType c_k = 0; c_k < L_entries; c_k++ ) {
            const auto k = L_i.getElementColumn( c_k );

            auto L_ik = L.getElementFast( i, k ) / U.getElementFast( N - 1 - k, k );
            L.setElement( i, k, L_ik );

            // loop for j = k+1, ..., N-1; but only over the non-zero entries
            // and split into two loops over L and U separately
            for( IndexType c_j = c_k + 1; c_j < L_entries; c_j++ ) {
               const auto j = L_i.getElementColumn( c_j );
               const auto L_ij = L.getElementFast( i, j ) - L_ik * U.getElementFast( N - 1 - k, j );
               L.setElement( i, j, L_ij );
            }
            for( IndexType c_j = 0; c_j < U_entries; c_j++ ) {
               const auto j = U_i.getElementColumn( c_j );
               const auto U_ij = U.getElementFast( N - 1 - i, j ) - L_ik * U.getElementFast( N - 1 - k, j );
               U.setElement( N - 1 - i, j, U_ij );
            }
         }
      }
   }
}

template< typename Matrix, typename Real, typename Index >
void
ILU0_impl< Matrix, Real, Devices::Host, Index >::
solve( ConstVectorViewType _b, VectorViewType _x ) const
{
   const auto b = Traits< Matrix >::getLocalVectorView( _b );
   auto x = Traits< Matrix >::getLocalVectorView( _x );

   // Step 1: solve y from Ly = b
   triangularSolveLower< true >( L, x, b );

   // Step 2: solve x from Ux = y
   triangularSolveUpper< true, true >( U, x, x );
}


template< typename Matrix >
void
ILU0_impl< Matrix, double, Devices::Cuda, int >::
update( const MatrixPointer& matrixPointer )
{
#ifdef HAVE_CUDA
#ifdef HAVE_CUSPARSE
   // TODO: only numerical factorization has to be done every time, split the rest into separate "setup" method which is called less often
   resetMatrices();

   // Note: the decomposition will be in-place, matrices L and U will have the
   // storage of A
   copyMatrix( *matrixPointer );

   const int m = A.getRows();
   const int nnz = A.getValues().getSize();

   y.setSize( m );

   // create matrix descriptors
   cusparseCreateMatDescr( &descr_A );
   cusparseSetMatIndexBase( descr_A, CUSPARSE_INDEX_BASE_ZERO );
   cusparseSetMatType( descr_A, CUSPARSE_MATRIX_TYPE_GENERAL );

   cusparseCreateMatDescr( &descr_L );
   cusparseSetMatIndexBase( descr_L, CUSPARSE_INDEX_BASE_ZERO );
   cusparseSetMatType( descr_L, CUSPARSE_MATRIX_TYPE_GENERAL );
   cusparseSetMatFillMode( descr_L, CUSPARSE_FILL_MODE_LOWER );
   cusparseSetMatDiagType( descr_L, CUSPARSE_DIAG_TYPE_UNIT );

   cusparseCreateMatDescr( &descr_U);
   cusparseSetMatIndexBase( descr_U, CUSPARSE_INDEX_BASE_ZERO );
   cusparseSetMatType( descr_U, CUSPARSE_MATRIX_TYPE_GENERAL );
   cusparseSetMatFillMode( descr_U, CUSPARSE_FILL_MODE_UPPER );
   cusparseSetMatDiagType( descr_U, CUSPARSE_DIAG_TYPE_NON_UNIT );

   // create info structures
   cusparseCreateCsrilu02Info( &info_A );
   cusparseCreateCsrsv2Info( &info_L );
   cusparseCreateCsrsv2Info( &info_U );

   // query how much memory will be needed in csrilu02 and csrsv2, and allocate the buffer
   int pBufferSize_A, pBufferSize_L, pBufferSize_U;
   cusparseDcsrilu02_bufferSize( handle, m, nnz, descr_A,
                                 A.getValues().getData(),
                                 A.getRowPointers().getData(),
                                 A.getColumnIndexes().getData(),
                                 info_A, &pBufferSize_A );
   cusparseDcsrsv2_bufferSize( handle, trans_L, m, nnz, descr_L,
                               A.getValues().getData(),
                               A.getRowPointers().getData(),
                               A.getColumnIndexes().getData(),
                               info_L, &pBufferSize_L );
   cusparseDcsrsv2_bufferSize( handle, trans_U, m, nnz, descr_U,
                               A.getValues().getData(),
                               A.getRowPointers().getData(),
                               A.getColumnIndexes().getData(),
                               info_U, &pBufferSize_U );
   const int pBufferSize = max( pBufferSize_A, max( pBufferSize_L, pBufferSize_U ) );
   pBuffer.setSize( pBufferSize );

   // Symbolic analysis of the incomplete LU decomposition
   cusparseDcsrilu02_analysis( handle, m, nnz, descr_A,
                               A.getValues().getData(),
                               A.getRowPointers().getData(),
                               A.getColumnIndexes().getData(),
                               info_A, policy_A, pBuffer.getData() );
   int structural_zero;
   cusparseStatus_t
   status = cusparseXcsrilu02_zeroPivot( handle, info_A, &structural_zero );
   if( CUSPARSE_STATUS_ZERO_PIVOT == status ) {
      std::cerr << "A(" << structural_zero << ", " << structural_zero << ") is missing." << std::endl;
      throw 1;
   }

   // Analysis for the triangular solves for L and U
   // Trick: the lower (upper) triangular part of A has the same sparsity
   // pattern as L (U), so we can do the analysis for csrsv2 on the matrix A.
   cusparseDcsrsv2_analysis( handle, trans_L, m, nnz, descr_L,
                             A.getValues().getData(),
                             A.getRowPointers().getData(),
                             A.getColumnIndexes().getData(),
                             info_L, policy_L, pBuffer.getData() );
   cusparseDcsrsv2_analysis( handle, trans_U, m, nnz, descr_U,
                             A.getValues().getData(),
                             A.getRowPointers().getData(),
                             A.getColumnIndexes().getData(),
                             info_U, policy_U, pBuffer.getData() );

   // Numerical incomplete LU decomposition
   cusparseDcsrilu02( handle, m, nnz, descr_A,
                      A.getValues().getData(),
                      A.getRowPointers().getData(),
                      A.getColumnIndexes().getData(),
                      info_A, policy_A, pBuffer.getData() );
   int numerical_zero;
   status = cusparseXcsrilu02_zeroPivot( handle, info_A, &numerical_zero );
   if( CUSPARSE_STATUS_ZERO_PIVOT == status ) {
      std::cerr << "A(" << numerical_zero << ", " << numerical_zero << ") is zero." << std::endl;
      throw 1;
   }
#else
   throw std::runtime_error("The program was not compiled with the CUSPARSE library. Pass -DHAVE_CUSPARSE -lcusparse to the compiler.");
#endif
#else
   throw Exceptions::CudaSupportMissing();
#endif
}

template< typename Matrix >
void
ILU0_impl< Matrix, double, Devices::Cuda, int >::
solve( ConstVectorViewType b, VectorViewType x ) const
{
#ifdef HAVE_CUDA
#ifdef HAVE_CUSPARSE
   const int m = A.getRows();
   const int nnz = A.getValues().getSize();

   // Step 1: solve y from Ly = b
   cusparseDcsrsv2_solve( handle, trans_L, m, nnz, &alpha, descr_L,
                          A.getValues().getData(),
                          A.getRowPointers().getData(),
                          A.getColumnIndexes().getData(),
                          info_L,
                          b.getData(),
                          (RealType*) y.getData(),
                          policy_L, (void*) pBuffer.getData() );

   // Step 2: solve x from Ux = y
   cusparseDcsrsv2_solve( handle, trans_U, m, nnz, &alpha, descr_U,
                          A.getValues().getData(),
                          A.getRowPointers().getData(),
                          A.getColumnIndexes().getData(),
                          info_U,
                          y.getData(),
                          x.getData(),
                          policy_U, (void*) pBuffer.getData() );
#else
   throw std::runtime_error("The program was not compiled with the CUSPARSE library. Pass -DHAVE_CUSPARSE -lcusparse to the compiler.");
#endif
#else
   throw Exceptions::CudaSupportMissing();
#endif
}

} // namespace Preconditioners
} // namespace Linear
} // namespace Solvers
} // namespace TNL
