/***************************************************************************
                          Matrix.h  -  description
                             -------------------
    begin                : Dec 18, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Object.h>
#include <TNL/Devices/Host.h>
#include <TNL/Containers/Vector.h>
#include <TNL/Containers/VectorView.h>

namespace TNL {
/**
 * \brief Namespace for matrix formats.
 */
namespace Matrices {

template< typename Real = double,
          typename Device = Devices::Host,
          typename Index = int >
class Matrix : public Object
{
public:
   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;
   typedef Containers::Vector< IndexType, DeviceType, IndexType > CompressedRowLengthsVector;
   typedef Containers::VectorView< IndexType, DeviceType, IndexType > CompressedRowLengthsVectorView;
   typedef Containers::VectorView< const IndexType, DeviceType, IndexType > ConstCompressedRowLengthsVectorView;
   typedef Containers::Vector< RealType, DeviceType, IndexType > ValuesVector;

   Matrix();

   virtual void setDimensions( const IndexType rows,
                                 const IndexType columns );

   virtual void setCompressedRowLengths( ConstCompressedRowLengthsVectorView rowLengths ) = 0;

   virtual IndexType getRowLength( const IndexType row ) const = 0;

   // TODO: implementation is not parallel
   // TODO: it would be nice if padding zeros could be stripped
   void getCompressedRowLengths( CompressedRowLengthsVector& rowLengths ) const;

   virtual void getCompressedRowLengths( CompressedRowLengthsVectorView rowLengths ) const;

   template< typename Real2, typename Device2, typename Index2 >
   void setLike( const Matrix< Real2, Device2, Index2 >& matrix );

   virtual IndexType getNumberOfMatrixElements() const = 0;

   virtual IndexType getNumberOfNonzeroMatrixElements() const = 0;

   void reset();

   __cuda_callable__
   IndexType getRows() const;

   __cuda_callable__
   IndexType getColumns() const;

   /****
    * TODO: The fast variants of the following methods cannot be virtual.
    * If they were, they could not be used in the CUDA kernels. If CUDA allows it
    * in the future and it does not slow down, declare them as virtual here.
    */

   virtual bool setElement( const IndexType row,
                            const IndexType column,
                            const RealType& value ) = 0;

   virtual bool addElement( const IndexType row,
                            const IndexType column,
                            const RealType& value,
                            const RealType& thisElementMultiplicator = 1.0 ) = 0;

   virtual bool setRow( const IndexType row,
                        const IndexType* columns,
                        const RealType* values,
                        const IndexType numberOfElements ) = 0;

   virtual bool addRow( const IndexType row,
                        const IndexType* columns,
                        const RealType* values,
                        const IndexType numberOfElements,
                        const RealType& thisElementMultiplicator = 1.0 ) = 0;

   virtual Real getElement( const IndexType row,
                            const IndexType column ) const = 0;
   
   const ValuesVector& getValues() const;
   
   ValuesVector& getValues();

   // TODO: parallelize and optimize for sparse matrices
   template< typename Matrix >
   bool operator == ( const Matrix& matrix ) const;

   template< typename Matrix >
   bool operator != ( const Matrix& matrix ) const;

   virtual bool save( File& file ) const;

   virtual bool load( File& file );

   virtual void print( std::ostream& str ) const;


   // TODO: method for symmetric matrices, should not be in general Matrix interface
   __cuda_callable__
   const IndexType& getNumberOfColors() const;

   // TODO: method for symmetric matrices, should not be in general Matrix interface
   void computeColorsVector(Containers::Vector<Index, Device, Index> &colorsVector);

   // TODO: what is this supposed to do?!?  There are redefinitions only in the
   // EllpackSymmetricGraph and SlicedEllpackSymmetricGraph classes...
   bool help( bool verbose = false ) { return true;};

   // TODO: copy should be done in the operator= and it should work the other way too
   void copyFromHostToCuda( Matrices::Matrix< Real, Devices::Host, Index >& matrix );

   // TODO: missing implementation!
   __cuda_callable__
   Index getValuesSize() const;

   protected:

   IndexType rows, columns, numberOfColors;

   ValuesVector values;
};

template< typename Real, typename Device, typename Index >
std::ostream& operator << ( std::ostream& str, const Matrix< Real, Device, Index >& m )
{
   m.print( str );
   return str;
}

template< typename Matrix,
          typename InVector,
          typename OutVector >
void MatrixVectorProductCuda( const Matrix& matrix,
                              const InVector& inVector,
                              OutVector& outVector );

} // namespace Matrices
} // namespace TNL

#include <TNL/Matrices/Matrix_impl.h>
