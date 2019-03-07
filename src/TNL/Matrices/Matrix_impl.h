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
void Matrix< Real, Device, Index >::setDimensions( const IndexType rows,
                                                   const IndexType columns )
{
   TNL_ASSERT( rows > 0 && columns > 0,
               std::cerr << " rows = " << rows << " columns = " << columns );
   this->rows = rows;
   this->columns = columns;
}

template< typename Real,
          typename Device,
          typename Index >
void Matrix< Real, Device, Index >::getCompressedRowLengths( CompressedRowLengthsVector& rowLengths ) const
{
   rowLengths.setSize( this->getRows() );
   getCompressedRowLengths( CompressedRowLengthsVectorView( rowLengths ) );
}

template< typename Real,
          typename Device,
          typename Index >
void Matrix< Real, Device, Index >::getCompressedRowLengths( CompressedRowLengthsVectorView rowLengths ) const
{
   TNL_ASSERT_EQ( rowLengths.getSize(), this->getRows(), "invalid size of the rowLengths vector" );
   for( IndexType row = 0; row < this->getRows(); row++ )
      rowLengths.setElement( row, this->getRowLength( row ) );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Real2,
             typename Device2,
             typename Index2 >
void Matrix< Real, Device, Index >::setLike( const Matrix< Real2, Device2, Index2 >& matrix )
{
   setDimensions( matrix.getRows(), matrix.getColumns() );
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
const typename Matrix< Real, Device, Index >::ValuesVector&
Matrix< Real, Device, Index >::
getValues() const
{
   return this->values;
}
   
template< typename Real,
          typename Device,
          typename Index >
typename Matrix< Real, Device, Index >::ValuesVector& 
Matrix< Real, Device, Index >::
getValues()
{
   return this->values;
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
   if( ! Object::save( file ) ||
       ! file.save( &this->rows ) ||
       ! file.save( &this->columns ) ||
       ! this->values.save( file ) )
      return false;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
bool Matrix< Real, Device, Index >::load( File& file )
{
   if( ! Object::load( file ) ||
       ! file.load( &this->rows ) ||
       ! file.load( &this->columns ) ||
       ! this->values.load( file ) )
      return false;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
void Matrix< Real, Device, Index >::print( std::ostream& str ) const
{
}

template< typename Real,
          typename Device,
          typename Index >
__cuda_callable__
const Index&
Matrix< Real, Device, Index >::
getNumberOfColors() const
{
   return this->numberOfColors;
}

template< typename Real,
          typename Device,
          typename Index >
void 
Matrix< Real, Device, Index >::
computeColorsVector(Containers::Vector<Index, Device, Index> &colorsVector)
{
    for( IndexType i = this->getRows() - 1; i >= 0; i-- )
    {
        // init color array
        Containers::Vector< Index, Device, Index > usedColors;
        usedColors.setSize( this->numberOfColors );
        for( IndexType j = 0; j < this->numberOfColors; j++ )
            usedColors.setElement( j, 0 );

        // find all colors used in given row
        for( IndexType j = i + 1; j < this->getColumns(); j++ )
             if( this->getElement( i, j ) != 0.0 )
                 usedColors.setElement( colorsVector.getElement( j ), 1 );

        // find unused color
        bool found = false;
        for( IndexType j = 0; j < this->numberOfColors; j++ )
            if( usedColors.getElement( j ) == 0 )
            {
                colorsVector.setElement( i, j );
                found = true;
                break;
            }
        if( !found )
        {
            colorsVector.setElement( i, this->numberOfColors );
            this->numberOfColors++;
        }
    }
}

template< typename Real,
          typename Device,
          typename Index >
void
Matrix< Real, Device, Index >::
copyFromHostToCuda( Matrix< Real, Devices::Host, Index >& matrix )
{
    this->numberOfColors = matrix.getNumberOfColors();
    this->columns = matrix.getColumns();
    this->rows = matrix.getRows();

    this->values.setSize( matrix.getValuesSize() );
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
      TNL_CHECK_CUDA_DEVICE;
   }
   Devices::Cuda::freeFromDevice( kernel_this );
   Devices::Cuda::freeFromDevice( kernel_inVector );
   Devices::Cuda::freeFromDevice( kernel_outVector );
   TNL_CHECK_CUDA_DEVICE;
#endif
}

} // namespace Matrices
} // namespace TNL
