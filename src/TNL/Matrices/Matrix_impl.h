/***************************************************************************
                          Matrix_impl.h  -  description
                             -------------------
    begin                : Dec 18, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Matrices/Matrix.h>
#include <TNL/Assert.h>

namespace TNL {
namespace Matrices {

template< typename Real,
          typename Device,
          typename Index >
Matrix< Real, Device, Index >::Matrix()
: rows( 0 ),
  columns( 0 )
{
}

template< typename Real,
          typename Device,
          typename Index >
 bool Matrix< Real, Device, Index >::setDimensions( const IndexType rows,
                                                       const IndexType columns )
{
   TNL_ASSERT( rows > 0 && columns > 0,
            std::cerr << " rows = " << rows << " columns = " << columns );
   this->rows = rows;
   this->columns = columns;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
void Matrix< Real, Device, Index >::getCompressedRowLengths( Containers::Vector< IndexType, DeviceType, IndexType >& rowLengths ) const
{
   rowLengths.setSize( this->getRows() );
   for( IndexType row = 0; row < this->getRows(); row++ )
      rowLengths.setElement( row, this->getRowLength( row ) );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Real2,
             typename Device2,
             typename Index2 >
bool Matrix< Real, Device, Index >::setLike( const Matrix< Real2, Device2, Index2 >& matrix )
{
   return setDimensions( matrix.getRows(), matrix.getColumns() );
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__
Index Matrix< Real, Device, Index >::getRows() const
{
   return this->rows;
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__
Index Matrix< Real, Device, Index >::getColumns() const
{
   return this->columns;
}

template< typename Real,
          typename Device,
          typename Index >
void Matrix< Real, Device, Index >::reset()
{
   this->rows = 0;
   this->columns = 0;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename MatrixT >
bool Matrix< Real, Device, Index >::copyFrom( const MatrixT& matrix,
                                              const CompressedRowLengthsVector& rowLengths )
{
   /*tnlStaticTNL_ASSERT( DeviceType::DeviceType == Devices::HostDevice, );
   tnlStaticTNL_ASSERT( DeviceType::DeviceType == Matrix:DeviceType::DeviceType, );*/

   this->setLike( matrix );
   if( ! this->setCompressedRowLengths( rowLengths ) )
      return false;
   Containers::Vector< RealType, Devices::Host, IndexType > values;
   Containers::Vector< IndexType, Devices::Host, IndexType > columns;
   if( ! values.setSize( this->getColumns() ) ||
       ! columns.setSize( this->getColumns() ) )
      return false;
   for( IndexType row = 0; row < this->getRows(); row++ )
   {
      TNL_ASSERT( false, );
      // TODO: fix this
      //matrix.getRow( row, columns.getData(), values.getData() );
      this->setRow( row, columns.getData(), values.getData(), rowLengths.getElement( row ) );
   }
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
Matrix< Real, Device, Index >& Matrix< Real, Device, Index >::operator = ( const Matrix< RealType, DeviceType, IndexType >& m )
{
   this->setLike( m );

   Containers::Vector< IndexType, DeviceType, IndexType > rowLengths;
   m.getCompressedRowLengths( rowLengths );
   this->setCompressedRowLengths( rowLengths );

   Containers::Vector< RealType, DeviceType, IndexType > rowValues;
   Containers::Vector< IndexType, DeviceType, IndexType > rowColumns;
   const IndexType maxRowLength = rowLengths.max();
   rowValues.setSize( maxRowLength );
   rowColumns.setSize( maxRowLength );
   for( IndexType row = 0; row < this->getRows(); row++ )
   {
      m.getRow( row,
                rowColumns.getData(),
                rowValues.getData() );
      this->setRow( row,
                    rowColumns.getData(),
                    rowValues.getData(),
                    m.getRowLength( row ) );
   }
   return *this;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename MatrixT >
bool Matrix< Real, Device, Index >::operator == ( const MatrixT& matrix ) const
{
   if( this->getRows() != matrix.getRows() ||
       this->getColumns() != matrix.getColumns() )
      return false;
   for( IndexType row = 0; row < this->getRows(); row++ )
      for( IndexType column = 0; column < this->getColumns(); column++ )
         if( this->getElement( row, column ) != matrix.getElement( row, column ) )
            return false;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename MatrixT >
bool Matrix< Real, Device, Index >::operator != ( const MatrixT& matrix ) const
{
   return ! operator == ( matrix );
}

template< typename Real,
          typename Device,
          typename Index >
bool Matrix< Real, Device, Index >::save( File& file ) const
{
#ifdef HAVE_NOT_CXX11
   if( ! Object::save( file ) ||
       ! file.write< IndexType, Devices::Host, Index >( &this->rows, 1 ) ||
       ! file.write< IndexType, Devices::Host, Index >( &this->columns, 1 ) ||
       ! this->values.save( file ) )
      return false;
#else
   if( ! Object::save( file ) ||
       ! file.write( &this->rows ) ||
       ! file.write( &this->columns ) ||
       ! this->values.save( file ) )
      return false;
#endif
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
bool Matrix< Real, Device, Index >::load( File& file )
{
#ifdef HAVE_NOT_CXX11
   if( ! Object::load( file ) ||
       ! file.read< IndexType, Devices::Host, Index >( &this->rows, 1 ) ||
       ! file.read< IndexType, Devices::Host, Index >( &this->columns, 1 ) ||
       ! this->values.load( file ) )
      return false;
#else
   if( ! Object::load( file ) ||
       ! file.read( &this->rows ) ||
       ! file.read( &this->columns ) ||
       ! this->values.load( file ) )
      return false;
#endif
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
void Matrix< Real, Device, Index >::print( std::ostream& str ) const
{
}

#ifdef HAVE_CUDA
template< typename Matrix,
          typename InVector,
          typename OutVector >
__global__ void MatrixVectorProductCudaKernel( const Matrix* matrix,
                                                  const InVector* inVector,
                                                  OutVector* outVector,
                                                  int gridIdx )
{
   static_assert( std::is_same< typename Matrix::DeviceType, Devices::Cuda >::value, "" );
   const typename Matrix::IndexType rowIdx = ( gridIdx * Devices::Cuda::getMaxGridSize() + blockIdx.x ) * blockDim.x + threadIdx.x;
   if( rowIdx < matrix->getRows() )
      ( *outVector )[ rowIdx ] = matrix->rowVectorProduct( rowIdx, *inVector );
}
#endif

template< typename Matrix,
          typename InVector,
          typename OutVector >
void MatrixVectorProductCuda( const Matrix& matrix,
                                 const InVector& inVector,
                                 OutVector& outVector )
{
#ifdef HAVE_CUDA
   typedef typename Matrix::IndexType IndexType;
   Matrix* kernel_this = Devices::Cuda::passToDevice( matrix );
   InVector* kernel_inVector = Devices::Cuda::passToDevice( inVector );
   OutVector* kernel_outVector = Devices::Cuda::passToDevice( outVector );
   dim3 cudaBlockSize( 256 ), cudaGridSize( Devices::Cuda::getMaxGridSize() );
   const IndexType cudaBlocks = roundUpDivision( matrix.getRows(), cudaBlockSize.x );
   const IndexType cudaGrids = roundUpDivision( cudaBlocks, Devices::Cuda::getMaxGridSize() );
   for( IndexType gridIdx = 0; gridIdx < cudaGrids; gridIdx++ )
   {
      if( gridIdx == cudaGrids - 1 )
         cudaGridSize.x = cudaBlocks % Devices::Cuda::getMaxGridSize();
      MatrixVectorProductCudaKernel<<< cudaGridSize, cudaBlockSize >>>
                                     ( kernel_this,
                                       kernel_inVector,
                                       kernel_outVector,
                                       gridIdx );
      checkCudaDevice;
   }
   Devices::Cuda::freeFromDevice( kernel_this );
   Devices::Cuda::freeFromDevice( kernel_inVector );
   Devices::Cuda::freeFromDevice( kernel_outVector );
   checkCudaDevice;
#endif
}

} // namespace Matrices
} // namespace TNL
