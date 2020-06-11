/***************************************************************************
                          TridiagonalMatrixRowView.h  -  description
                             -------------------
    begin                : Dec 31, 2014
    copyright            : (C) 2014 by oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

namespace TNL {
namespace Matrices {   

/**
 * \brief RowView is a simple structure for accessing rows of tridiagonal matrix.
 * 
 * \tparam ValuesView is a vector view storing the matrix elements values.
 * \tparam Indexer is type of object responsible for indexing and organization of
 *    matrix elements.
 * 
 * See \ref TridiagonalMatrix and \ref TridiagonalMatrixView.
 * 
 * \par Example
 * \include Matrices/TridiagonalMatrix/TridiagonalMatrixExample_getRow.cpp
 * \par Output
 * \include TridiagonalatrixExample_getRow.out
 * 
 * \par Example
 * \include Matrices/TridiagonalMatrix/TridiagonalMatrixViewExample_getRow.cpp
 * \par Output
 * \include TridiagonalMatrixViewExample_getRow.out
 */
template< typename ValuesView,
          typename Indexer >
class TridiagonalMatrixRowView
{
   public:

      /**
       * \brief The type of matrix elements.
       */
      using RealType = typename ValuesView::RealType;

      /**
       * \brief The type used for matrix elements indexing.
       */
      using IndexType = typename ValuesView::IndexType;

      /**
       * \brief Type of container view used for storing the matrix elements values.
       */
      using ValuesViewType = ValuesView;

      /**
       * \brief Type of object responsible for indexing and organization of
       * matrix elements.
       */
      using IndexerType = Indexer;

      /**
       * \brief Constructor with all necessary data.
       * 
       * \param rowIdx is index of the matrix row this RowView refer to.
       * \param values is a vector view holding values of matrix elements.
       * \param indexer is object responsible for indexing and organization of matrix elements
       */
      __cuda_callable__
      TridiagonalMatrixRowView( const IndexType rowIdx,
                                const ValuesViewType& values,
                                const IndexerType& indexer );

      /**
       * \brief Returns number of diagonals of the tridiagonal matrix which is three.
       * 
       * \return number three.
       */
      __cuda_callable__
      IndexType getSize() const;

      /**
       * \brief Computes column index of matrix element on given subdiagonal.
       * 
       * \param localIdx is an index of the subdiagonal.
       * 
       * \return column index of matrix element on given subdiagonal.
       */
      __cuda_callable__
      const IndexType getColumnIndex( const IndexType localIdx ) const;

      /**
       * \brief Returns value of matrix element on given subdiagonal.
       * 
       * \param localIdx is an index of the subdiagonal.
       * 
       * \return constant reference to matrix element value.
       */
      __cuda_callable__
      const RealType& getValue( const IndexType localIdx ) const;

      /**
       * \brief Returns value of matrix element on given subdiagonal.
       * 
       * \param localIdx is an index of the subdiagonal.
       * 
       * \return non-constant reference to matrix element value.
       */
      __cuda_callable__
      RealType& getValue( const IndexType localIdx );

      /**
       * \brief Changes value of matrix element on given subdiagonal.
       * 
       * \param localIdx is an index of the matrix subdiagonal.
       * \param value is the new value of the matrix element.
       */
      __cuda_callable__
      void setElement( const IndexType localIdx,
                       const RealType& value );
   protected:

      IndexType rowIdx;

      ValuesViewType values;

      Indexer indexer;
};

} // namespace Matrices
} // namespace TNL

#include <TNL/Matrices/TridiagonalMatrixRowView.hpp>
