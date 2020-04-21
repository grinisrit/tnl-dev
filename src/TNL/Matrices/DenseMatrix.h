/***************************************************************************
                          DenseMatrix.h  -  description
                             -------------------
    begin                : Nov 29, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Allocators/Default.h>
#include <TNL/Devices/Host.h>
#include <TNL/Matrices/DenseMatrixRowView.h>
#include <TNL/Matrices/Matrix.h>
#include <TNL/Matrices/DenseMatrixView.h>
#include <TNL/Containers/Segments/Ellpack.h>

namespace TNL {
namespace Matrices {

/**
 * \brief Implementation of dense matrix, i.e. matrix storing explicitly all of its elements including zeros.
 * 
 * \tparam Real is a type of matrix elements.
 * \tparam Device is a device where the matrix is allocated.
 * \tparam Index is a type for indexing of the matrix elements.
 * \tparam RowMajorOrder tells the ordering of matrix elements. If it is \e true the matrix elements
 *         are stored in row major order. If it is \e false, the matrix elements are stored in column major order.
 * \tparam RealAllocator is allocator for the matrix elements.
 */
template< typename Real = double,
          typename Device = Devices::Host,
          typename Index = int,
          bool RowMajorOrder = std::is_same< Device, Devices::Host >::value,
          typename RealAllocator = typename Allocators::Default< Device >::template Allocator< Real > >
class DenseMatrix : public Matrix< Real, Device, Index >
{
   protected:
      using BaseType = Matrix< Real, Device, Index, RealAllocator >;
      using ValuesVectorType = typename BaseType::ValuesVectorType;
      using ValuesViewType = typename ValuesVectorType::ViewType;
      using SegmentsType = Containers::Segments::Ellpack< Device, Index, typename Allocators::Default< Device >::template Allocator< Index >, RowMajorOrder, 1 >;
      using SegmentViewType = typename SegmentsType::SegmentViewType;


   public:

      /**
       * \brief The type of matrix elements.
       */
      using RealType = Real;

      /**
       * \brief The device where the matrix is allocated.
       */
      using DeviceType = Device;

      /**
       * \brief The type used for matrix elements indexing.
       */
      using IndexType = Index;

      /**
       * \brief The allocator for matrix elements.
       */
      using RealAllocatorType = RealAllocator;

      /**
       * \brief Type of related matrix view. 
       * 
       * See \ref DenseMatrixView.
       */
      using ViewType = DenseMatrixView< Real, Device, Index, RowMajorOrder >;

      /**
       * \brief Matrix view type for constant instances.
       * 
       * See \ref DenseMatrixView.
       */
      using ConstViewType = DenseMatrixView< typename std::add_const< Real >::type, Device, Index, RowMajorOrder >;

      /**
       * \brief Type for accessing matrix row.
       */
      using RowView = DenseMatrixRowView< SegmentViewType, ValuesViewType >;

      /**
       * \brief Helper type for getting self type or its variations.
       */
      template< typename _Real = Real,
                typename _Device = Device,
                typename _Index = Index,
                bool RowMajorOrder_ = RowMajorOrder,
                typename RealAllocator_ = RealAllocator >
      using Self = DenseMatrix< _Real, _Device, _Index, RowMajorOrder_, RealAllocator_ >;

      // TODO: remove this
      using CompressedRowLengthsVector = typename Matrix< Real, Device, Index >::CompressedRowLengthsVector;
      using ConstCompressedRowLengthsVectorView = typename Matrix< Real, Device, Index >::ConstCompressedRowLengthsVectorView;



      /**
       * \brief Constrictor without parameters.
       */
      DenseMatrix();

      /**
       * \brief Constructor with matrix dimensions.
       * 
       * \param rows is number of matrix rows.
       * \param columns is number of matrix columns.
       */
      DenseMatrix( const IndexType rows, const IndexType columns );

      /**
       * \brief Constructor with 2D initializer list.
       * 
       * The number of matrix rows is set to the outer list size and the number
       * of matrix columns is set to maximum size of inner lists. Missing elements
       * are filled in with zeros.
       * 
       * \param data is a initializer list of initializer lists representing
       * list of matrix rows.
       * 
       * \par Example
       * \include Matrices/DenseMatrixExample_Constructor_init_list.cpp
       * \par Output
       * \include DenseMatrixExample_Constructor_init_list.out
       */
      DenseMatrix( std::initializer_list< std::initializer_list< RealType > > data );

      /**
       * \brief Returns a modifiable view of the dense matrix.
       * 
       * See \ref DenseMatrixView.
       * 
       * \return dense matrix view.
       */
      ViewType getView();

      /**
       * \brief Returns a non-modifiable view of the dense matrix.
       * 
       * See \ref DenseMatrixView.
       * 
       * \return dense matrix view.
       */
      ConstViewType getConstView() const;

      /**
       * \brief Returns string with serialization type.
       * 
       * The string has a form \e `Matrices::DenseMatrix< RealType,  [any_device], IndexType, [any_allocator], true/false >`.
       * 
       * \return \e String with the serialization type.
       */
      static String getSerializationType();

      /**
       * \brief Returns string with serialization type.
       * 
       * See \ref DenseMatrix::getSerializationType.
       * 
       * \return \e String with the serialization type.
       */
      virtual String getSerializationTypeVirtual() const;

      /**
       * \brief Set number of rows and columns of this matrix.
       * 
       * \param rows is the number of matrix rows.
       * \param columns is the number of matrix columns.
       */
      void setDimensions( const IndexType rows,
                          const IndexType columns );

      /**
       * \brief Set the number of matrix rows and columns by the given matrix.
       * 
       * \tparam Matrix is matrix type. This can be any matrix having methods 
       *  \ref getRows and \ref getColumns.
       * 
       * \param matrix in the input matrix dimensions of which are to be adopted.
       */
      template< typename Matrix >
      void setLike( const Matrix& matrix );

      /**
       * \brief This method recreates the dense matrix from 2D initializer list.
       * 
       * The number of matrix rows is set to the outer list size and the number
       * of matrix columns is set to maximum size of inner lists. Missing elements
       * are filled in with zeros.
       * 
       * \param data is a initializer list of initializer lists representing
       * list of matrix rows.
       * 
       * \par Example
       * \include Matrices/DenseMatrixExample_setElements.cpp
       * \par Output
       * \include DenseMatrixExample_setElements.out
       */
      void setElements( std::initializer_list< std::initializer_list< RealType > > data );

      /**
       * \brief This method is only for the compatibility with the sparse matrices.
       * 
       * This method does nothing. In debug mode it contains assertions checking
       * that given rowCapacities are compatible with the current matrix dimensions.
       */
      template< typename RowCapacitiesVector >
      void setRowCapacities( const RowCapacitiesVector& rowCapacities );

      /**
       * \brief Computes number of non-zeros in each row.
       * 
       * \param rowLengths is a vector into which the number of non-zeros in each row
       * will be stored.
       * 
       * \par Example
       * \include Matrices/DenseMatrixExample_getCompressedRowLengths.cpp
       * \par Output
       * \include DenseMatrixExample_getCompressedRowLengths.out
       */
      template< typename RowLengthsVector >
      void getCompressedRowLengths( RowLengthsVector& rowLengths ) const;

      IndexType getMaxRowLength() const;

      IndexType getNumberOfMatrixElements() const;

      IndexType getNumberOfNonzeroMatrixElements() const;

      void reset();

      __cuda_callable__
      const RowView getRow( const IndexType& rowIdx ) const;

      __cuda_callable__
      RowView getRow( const IndexType& rowIdx );


      void setValue( const RealType& v );

      __cuda_callable__
      Real& operator()( const IndexType row,
                        const IndexType column );

      __cuda_callable__
      const Real& operator()( const IndexType row,
                              const IndexType column ) const;

      void setElement( const IndexType row,
                       const IndexType column,
                       const RealType& value );

      void addElement( const IndexType row,
                       const IndexType column,
                       const RealType& value,
                       const RealType& thisElementMultiplicator = 1.0 );

      Real getElement( const IndexType row,
                       const IndexType column ) const;

      template< typename Fetch, typename Reduce, typename Keep, typename FetchReal >
      void rowsReduction( IndexType first, IndexType last, Fetch& fetch, Reduce& reduce, Keep& keep, const FetchReal& zero ) const;

      template< typename Fetch, typename Reduce, typename Keep, typename FetchReal >
      void allRowsReduction( Fetch& fetch, Reduce& reduce, Keep& keep, const FetchReal& zero ) const;

      template< typename Function >
      void forRows( IndexType first, IndexType last, Function& function ) const;

      template< typename Function >
      void forRows( IndexType first, IndexType last, Function& function );

      template< typename Function >
      void forAllRows( Function& function ) const;

      template< typename Function >
      void forAllRows( Function& function );

      template< typename Vector >
      __cuda_callable__
      typename Vector::RealType rowVectorProduct( const IndexType row,
                                                  const Vector& vector ) const;

      template< typename InVector, typename OutVector >
      void vectorProduct( const InVector& inVector,
                          OutVector& outVector ) const;

      template< typename Matrix >
      void addMatrix( const Matrix& matrix,
                      const RealType& matrixMultiplicator = 1.0,
                      const RealType& thisMatrixMultiplicator = 1.0 );

      template< typename Matrix1, typename Matrix2, int tileDim = 32 >
      void getMatrixProduct( const Matrix1& matrix1,
                          const Matrix2& matrix2,
                          const RealType& matrix1Multiplicator = 1.0,
                          const RealType& matrix2Multiplicator = 1.0 );

      template< typename Matrix, int tileDim = 32 >
      void getTransposition( const Matrix& matrix,
                             const RealType& matrixMultiplicator = 1.0 );

      template< typename Vector1, typename Vector2 >
      void performSORIteration( const Vector1& b,
                                const IndexType row,
                                Vector2& x,
                                const RealType& omega = 1.0 ) const;

      /**
       * \brief Assignment operator for exactly the same type of the dense matrix.
       * 
       * @param matrix
       * @return 
       */
      DenseMatrix& operator=( const DenseMatrix& matrix );

      /**
       * \brief Assignment operator for other dense matrices.
       * 
       * @param matrix
       * @return 
       */
      template< typename RHSReal, typename RHSDevice, typename RHSIndex,
                 bool RHSRowMajorOrder, typename RHSRealAllocator >
      DenseMatrix& operator=( const DenseMatrix< RHSReal, RHSDevice, RHSIndex, RHSRowMajorOrder, RHSRealAllocator >& matrix );

      /**
       * \brief Assignment operator for other (sparse) types of matrices.
       * @param matrix
       * @return 
       */
      template< typename RHSMatrix >
      DenseMatrix& operator=( const RHSMatrix& matrix );

      template< typename Real_, typename Device_, typename Index_, typename RealAllocator_ >
      bool operator==( const DenseMatrix< Real_, Device_, Index_, RowMajorOrder >& matrix ) const;

      template< typename Real_, typename Device_, typename Index_, typename RealAllocator_ >
      bool operator!=( const DenseMatrix< Real_, Device_, Index_, RowMajorOrder >& matrix ) const;

      void save( const String& fileName ) const;

      void load( const String& fileName );

      void save( File& file ) const;

      void load( File& file );

      void print( std::ostream& str ) const;

   protected:

      __cuda_callable__
      IndexType getElementIndex( const IndexType row,
                                 const IndexType column ) const;

      SegmentsType segments;

      ViewType view;
};

template< typename Real,
          typename Device,
          typename Index,
          bool RowMajorOrder,
          typename RealAllocator >
std::ostream& operator<< ( std::ostream& str, const DenseMatrix< Real, Device, Index, RowMajorOrder, RealAllocator >& matrix );

} // namespace Matrices
} // namespace TNL

#include <TNL/Matrices/DenseMatrix.hpp>
