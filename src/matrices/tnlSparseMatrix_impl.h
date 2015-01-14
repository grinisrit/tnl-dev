/***************************************************************************
                          tnlSparseMatrix_impl.h  -  description
                             -------------------
    begin                : Dec 21, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#ifndef TNLSPARSEMATRIX_IMPL_H_
#define TNLSPARSEMATRIX_IMPL_H_

template< typename Real,
          typename Device,
          typename Index >
tnlSparseMatrix< Real, Device, Index >::tnlSparseMatrix()
: maxRowLength( 0 )
{
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Real2,
             typename Device2,
             typename Index2 >
bool tnlSparseMatrix< Real, Device, Index >::setLike( const tnlSparseMatrix< Real2, Device2, Index2 >& matrix )
{
   if( ! tnlMatrix< Real, Device, Index >::setLike( matrix ) ||
       ! this->allocateMatrixElements( matrix.getNumberOfMatrixElements() ) )
      return false;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
Index tnlSparseMatrix< Real, Device, Index >::getNumberOfMatrixElements() const
{
   return this->values.getSize();
}

template< typename Real,
          typename Device,
          typename Index >
Index tnlSparseMatrix< Real, Device, Index >::getNumberOfNonzeroMatrixElements() const
{
   IndexType nonzeroElements( 0 );
   for( IndexType i = 0; i < this->values.getSize(); i++ )
      if( this->columnIndexes.getElement( i ) != this-> columns &&
          this->values.getElement( i ) != 0.0 )
         nonzeroElements++;
   return nonzeroElements;
}

template< typename Real,
          typename Device,
          typename Index >
Index
tnlSparseMatrix< Real, Device, Index >::
getMaxRowLength() const
{
   return this->maxRowLength;
}

template< typename Real,
          typename Device,
          typename Index >
#ifdef HAVE_CUDA
   __device__ __host__
#endif
Index tnlSparseMatrix< Real, Device, Index >::getPaddingIndex() const
{
   return this->getColumns();
}

template< typename Real,
          typename Device,
          typename Index >
void tnlSparseMatrix< Real, Device, Index >::reset()
{
   tnlMatrix< Real, Device, Index >::reset();
   this->values.reset();
   this->columnIndexes.reset();
}

template< typename Real,
          typename Device,
          typename Index >
bool tnlSparseMatrix< Real, Device, Index >::save( tnlFile& file ) const
{
   if( ! tnlMatrix< Real, Device, Index >::save( file ) ||
       ! this->values.save( file ) ||
       ! this->columnIndexes.save( file ) )
      return false;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
bool tnlSparseMatrix< Real, Device, Index >::load( tnlFile& file )
{
   if( ! tnlMatrix< Real, Device, Index >::load( file ) ||
       ! this->values.load( file ) ||
       ! this->columnIndexes.load( file ) )
      return false;
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
bool tnlSparseMatrix< Real, Device, Index >::allocateMatrixElements( const IndexType& numberOfMatrixElements )
{
   if( ! this->values.setSize( numberOfMatrixElements ) ||
       ! this->columnIndexes.setSize( numberOfMatrixElements ) )
      return false;

   /****
    * Setting a column index to this->columns means that the
    * index is undefined.
    */
   this->columnIndexes.setValue( this->columns );
   return true;
}

template< typename Real,
          typename Device,
          typename Index >
void tnlSparseMatrix< Real, Device, Index >::printStructure( ostream& str ) const
{
}

#endif /* TNLSPARSEMATRIX_IMPL_H_ */