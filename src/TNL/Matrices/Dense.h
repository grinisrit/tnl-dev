/***************************************************************************
                          Dense.h  -  description
                             -------------------
    begin                : Nov 29, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Devices/Host.h>
#include <TNL/Matrices/Matrix.h>
#include <TNL/Matrices/DenseRow.h>
#include <TNL/Containers/Array.h>

namespace TNL {
namespace Matrices {   

template< typename Device >
class DenseDeviceDependentCode;

template< typename Real = double,
          typename Device = Devices::Host,
          typename Index = int >
class Dense : public Matrix< Real, Device, Index >
{
   public:

   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;
   typedef typename Matrix< Real, Device, Index >::CompressedRowsLengthsVector CompressedRowsLengthsVector;
   typedef Dense< Real, Device, Index > ThisType;
   typedef Dense< Real, Devices::Host, Index > HostType;
   typedef Dense< Real, Devices::Cuda, Index > CudaType;
   typedef Matrix< Real, Device, Index > BaseType;
   typedef DenseRow< Real, Index > MatrixRow;


   Dense();

   static String getType();

   String getTypeVirtual() const;

   bool setDimensions( const IndexType rows,
                       const IndexType columns );

   template< typename Real2, typename Device2, typename Index2 >
   bool setLike( const Dense< Real2, Device2, Index2 >& matrix );

   /****
    * This method is only for the compatibility with the sparse matrices.
    */
   bool setCompressedRowsLengths( const CompressedRowsLengthsVector& rowLengths );

   /****
    * Returns maximal number of the nonzero matrix elements that can be stored
    * in a given row.
    */
   IndexType getRowLength( const IndexType row ) const;

   IndexType getMaxRowLength() const;

   IndexType getNumberOfMatrixElements() const;

   IndexType getNumberOfNonzeroMatrixElements() const;

   void reset();

   void setValue( const RealType& v );

   __cuda_callable__
   bool setElementFast( const IndexType row,
                        const IndexType column,
                        const RealType& value );

   bool setElement( const IndexType row,
                    const IndexType column,
                    const RealType& value );

   __cuda_callable__
   bool addElementFast( const IndexType row,
                        const IndexType column,
                        const RealType& value,
                        const RealType& thisElementMultiplicator = 1.0 );

   bool addElement( const IndexType row,
                    const IndexType column,
                    const RealType& value,
                    const RealType& thisElementMultiplicator = 1.0 );

   __cuda_callable__
   bool setRowFast( const IndexType row,
                    const IndexType* columns,
                    const RealType* values,
                    const IndexType elements );

   bool setRow( const IndexType row,
                const IndexType* columns,
                const RealType* values,
                const IndexType elements );

   __cuda_callable__
   bool addRowFast( const IndexType row,
                    const IndexType* columns,
                    const RealType* values,
                    const IndexType elements,
                    const RealType& thisRowMultiplicator = 1.0 );

   bool addRow( const IndexType row,
                const IndexType* columns,
                const RealType* values,
                const IndexType elements,
                const RealType& thisRowMultiplicator = 1.0 );

   __cuda_callable__
   const Real& getElementFast( const IndexType row,
                               const IndexType column ) const;

   Real getElement( const IndexType row,
                    const IndexType column ) const;

   __cuda_callable__
   void getRowFast( const IndexType row,
                    IndexType* columns,
                    RealType* values ) const;

   /*void getRow( const IndexType row,
                IndexType* columns,
                RealType* values ) const;*/

   __cuda_callable__
   MatrixRow getRow( const IndexType rowIndex );

   __cuda_callable__
   const MatrixRow getRow( const IndexType rowIndex ) const;

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

#ifdef HAVE_NOT_CXX11
   template< typename Matrix1, typename Matrix2, int tileDim >
   void getMatrixProduct( const Matrix1& matrix1,
                       const Matrix2& matrix2,
                       const RealType& matrix1Multiplicator = 1.0,
                       const RealType& matrix2Multiplicator = 1.0 );
#else
   template< typename Matrix1, typename Matrix2, int tileDim = 32 >
   void getMatrixProduct( const Matrix1& matrix1,
                       const Matrix2& matrix2,
                       const RealType& matrix1Multiplicator = 1.0,
                       const RealType& matrix2Multiplicator = 1.0 );
#endif

#ifdef HAVE_NOT_CXX11
   template< typename Matrix, int tileDim >
   void getTransposition( const Matrix& matrix,
                          const RealType& matrixMultiplicator = 1.0 );
#else
   template< typename Matrix, int tileDim = 32 >
   void getTransposition( const Matrix& matrix,
                          const RealType& matrixMultiplicator = 1.0 );
#endif

   template< typename Vector >
   void performSORIteration( const Vector& b,
                             const IndexType row,
                             Vector& x,
                             const RealType& omega = 1.0 ) const;

   bool save( const String& fileName ) const;

   bool load( const String& fileName );

   bool save( File& file ) const;

   bool load( File& file );

   void print( std::ostream& str ) const;

   protected:

   __cuda_callable__
   IndexType getElementIndex( const IndexType row,
                              const IndexType column ) const;

   typedef DenseDeviceDependentCode< DeviceType > DeviceDependentCode;
   friend class DenseDeviceDependentCode< DeviceType >;

};

} // namespace Matrices
} // namespace TNL

#include <TNL/Matrices/Dense_impl.h>
