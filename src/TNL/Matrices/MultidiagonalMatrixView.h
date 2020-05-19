/***************************************************************************
                          MultidiagonalMatrixView.h  -  description
                             -------------------
    begin                : Jan 11, 2020
    copyright            : (C) 2020 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Matrices/MatrixView.h>
#include <TNL/Containers/Vector.h>
#include <TNL/Matrices/MultidiagonalMatrixRowView.h>
#include <TNL/Containers/Segments/Ellpack.h>
#include <TNL/Matrices/details/MultidiagonalMatrixIndexer.h>

namespace TNL {
namespace Matrices {

template< typename Real = double,
          typename Device = Devices::Host,
          typename Index = int,
          ElementsOrganization Organization = Containers::Segments::DefaultElementsOrganization< Device >::getOrganization() >
class MultidiagonalMatrixView : public MatrixView< Real, Device, Index >
{
   public:
      using RealType = Real;
      using DeviceType = Device;
      using IndexType = Index;
      using BaseType = MatrixView< Real, Device, Index >;
      //using DiagonalsShiftsType = Containers::Vector< IndexType, DeviceType, IndexType >;
      using DiagonalsShiftsView = Containers::VectorView< IndexType, DeviceType, IndexType >;
      //using HostDiagonalsShiftsType = Containers::Vector< IndexType, Devices::Host, IndexType >;
      using HostDiagonalsShiftsView = Containers::VectorView< IndexType, Devices::Host, IndexType >;
      using IndexerType = details::MultidiagonalMatrixIndexer< IndexType, Organization >;
      using ValuesViewType = typename BaseType::ValuesView;
      using ViewType = MultidiagonalMatrixView< Real, Device, Index, Organization >;
      using ConstViewType = MultidiagonalMatrixView< typename std::add_const< Real >::type, Device, Index, Organization >;
      using RowView = MultidiagonalMatrixRowView< ValuesViewType, IndexerType, DiagonalsShiftsView >;

      // TODO: remove this - it is here only for compatibility with original matrix implementation
      typedef Containers::Vector< IndexType, DeviceType, IndexType > CompressedRowLengthsVector;
      typedef Containers::VectorView< IndexType, DeviceType, IndexType > CompressedRowLengthsVectorView;
      typedef typename CompressedRowLengthsVectorView::ConstViewType ConstCompressedRowLengthsVectorView;

      template< typename _Real = Real,
                typename _Device = Device,
                typename _Index = Index,
                ElementsOrganization Organization_ = Containers::Segments::DefaultElementsOrganization< Device >::getOrganization() >
      using Self = MultidiagonalMatrixView< _Real, _Device, _Index, Organization_ >;

      MultidiagonalMatrixView();

      MultidiagonalMatrixView( const ValuesViewType& values,
                               const DiagonalsShiftsView& diagonalsShifts,
                               const HostDiagonalsShiftsView& hostDiagonalsShifts,
                               const IndexerType& indexer );

      ViewType getView();

      ConstViewType getConstView() const;

      static String getSerializationType();

      virtual String getSerializationTypeVirtual() const;

      __cuda_callable__
      const IndexType& getDiagonalsCount() const;

      template< typename Vector >
      void getCompressedRowLengths( Vector& rowLengths ) const;

      IndexType getNonemptyRowsCount() const;

      [[deprecated]]
      IndexType getRowLength( const IndexType row ) const;

      IndexType getMaxRowLength() const;

      IndexType getNumberOfNonzeroMatrixElements() const;

      template< typename Real_, typename Device_, typename Index_, ElementsOrganization Organization_ >
      bool operator == ( const MultidiagonalMatrixView< Real_, Device_, Index_, Organization_ >& matrix ) const;

      template< typename Real_, typename Device_, typename Index_, ElementsOrganization Organization_ >
      bool operator != ( const MultidiagonalMatrixView< Real_, Device_, Index_, Organization_ >& matrix ) const;

      __cuda_callable__
      RowView getRow( const IndexType& rowIdx );

      __cuda_callable__
      const RowView getRow( const IndexType& rowIdx ) const;

      void setValue( const RealType& v );

      void setElement( const IndexType row,
                       const IndexType column,
                       const RealType& value );

      void addElement( const IndexType row,
                       const IndexType column,
                       const RealType& value,
                       const RealType& thisElementMultiplicator = 1.0 );

      RealType getElement( const IndexType row,
                           const IndexType column ) const;

      MultidiagonalMatrixView& operator=( const MultidiagonalMatrixView& view );

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

      template< typename InVector,
                typename OutVector >
      void vectorProduct( const InVector& inVector,
                          OutVector& outVector ) const;

      template< typename Real_, typename Device_, typename Index_, ElementsOrganization Organization_ >
      void addMatrix( const MultidiagonalMatrixView< Real_, Device_, Index_, Organization_ >& matrix,
                      const RealType& matrixMultiplicator = 1.0,
                      const RealType& thisMatrixMultiplicator = 1.0 );

      template< typename Real2, typename Index2 >
      void getTransposition( const MultidiagonalMatrixView< Real2, Device, Index2 >& matrix,
                             const RealType& matrixMultiplicator = 1.0 );

      template< typename Vector1, typename Vector2 >
      __cuda_callable__
      void performSORIteration( const Vector1& b,
                                const IndexType row,
                                Vector2& x,
                                const RealType& omega = 1.0 ) const;

      void save( File& file ) const;

      void save( const String& fileName ) const;

      void print( std::ostream& str ) const;

      __cuda_callable__
      const IndexerType& getIndexer() const;

      __cuda_callable__
      IndexerType& getIndexer();

      __cuda_callable__
      IndexType getPaddingIndex() const;

   protected:

      __cuda_callable__
      IndexType getElementIndex( const IndexType row,
                                 const IndexType localIdx ) const;

      DiagonalsShiftsView diagonalsShifts;

      HostDiagonalsShiftsView hostDiagonalsShifts;

      IndexerType indexer;
};

} // namespace Matrices
} // namespace TNL

#include <TNL/Matrices/MultidiagonalMatrixView.hpp>