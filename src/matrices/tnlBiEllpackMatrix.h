#ifndef TNLBIELLPACKMATRIX_H_
#define TNLBIELLPACKMATRIX_H_

#include <matrices/tnlSparseMatrix.h>
#include <core/vectors/tnlVector.h>

template< typename Device >
class tnlBiEllpackMatrixDeviceDependentCode;

template< typename Real, typename Device = tnlCuda, typename Index = int, int StripSize = 32 >
class tnlBiEllpackMatrix : public tnlSparseMatrix< Real, Device, Index >
{
public:
	typedef Real RealType;
	typedef Device DeviceType;
	typedef Index IndexType;
	typedef typename tnlSparseMatrix< RealType, DeviceType, IndexType >::RowLengthsVector RowLengthsVector;
	typedef typename tnlSparseMatrix< RealType, DeviceType, IndexType >::ValuesVector ValuesVector;
	typedef typename tnlSparseMatrix< RealType, DeviceType, IndexType >::ColumnIndexesVector ColumnIndexesVector;
	typedef tnlBiEllpackMatrix< Real, Device, Index > thisType;
	typedef tnlBiEllpackMatrix< Real, tnlHost, Index > hostType;
	typedef tnlBiEllpackMatrix< Real, tnlCuda, Index > cudaType;

	tnlBiEllpackMatrix();

	static tnlString getType();

	tnlString getTypeVirtual() const;

	bool setDimensions( const IndexType rows,
						const IndexType columns );

	bool setRowLengths( const RowLengthsVector& rowLengths );

	IndexType getRowLength( const IndexType row ) const;

	template< typename Real2, typename Device2, typename Index2 >
	bool setLike( const tnlBiEllpackMatrix< Real2, Device2, Index2, StripSize >& matrix );

	void getRowLengths( tnlVector< IndexType, DeviceType, IndexType >& rowLengths ) const;

	bool setElement( const IndexType row,
					 const IndexType column,
					 const RealType& value );

	bool addElement( const IndexType row,
					 const IndexType column,
					 const RealType& value,
					 const RealType& thisElementMultiplicator = 1.0 );

	bool setRow( const IndexType row,
				 const IndexType* columns,
				 const RealType* values,
				 const IndexType numberOfElements );

	bool addRow( const IndexType row,
				 const IndexType* columns,
				 const RealType* values,
				 const IndexType numberOfElements,
				 const RealType& thisElementMultiplicator = 1.0 );

	RealType getElement( const IndexType row,
					 	 const IndexType column ) const;

	void getRow( const IndexType row,
			 	 IndexType* columns,
			 	 RealType* values ) const;

	IndexType getGroupLength( const IndexType strip,
							  const IndexType group ) const;

	template< typename InVector,
			  typename OutVector >
	void vectorProduct( const InVector& inVector,
						OutVector& outVector ) const;

	template< typename InVector >
	typename InVector::RealType rowVectorProduct( const IndexType row,
							   	   	   	   	      const InVector& inVector ) const;

	void setVirtualRows(const IndexType rows);

	IndexType getVirtualRows();

	IndexType getWarpSize();

	IndexType getNumberOfGroups( const IndexType row ) const;

	void reset();

	bool save( tnlFile& file ) const;

	bool load( tnlFile& file );

	bool save( const tnlString& fileName ) const;

	bool load( const tnlString& fileName );

	void print( ostream& str ) const;

#ifdef HAVE_CUDA
	template< typename InVector,
			  typename OutVector,
			  int warpSize>
	__device__
	void spmvCuda( const InVector& inVector,
				   OutVector& outVector,
				   const IndexType warpStart,
				   const IndexType inWarpIdx ) const;
#endif

	typedef tnlBiEllpackMatrixDeviceDependentCode< DeviceType > DeviceDependentCode;
	friend class tnlBiEllpackMatrixDeviceDependentCode< DeviceType >;

private:

	IndexType warpSize;

	IndexType logWarpSize;

	IndexType virtualRows;

	tnlVector< Index, Device, Index > rowPermArray;

	tnlVector< Index, Device, Index > groupPointers;

};

#include <implementation/matrices/tnlBiEllpackMatrix_impl.h>

#endif
