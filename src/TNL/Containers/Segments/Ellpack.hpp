/***************************************************************************
                          Ellpack.hpp -  description
                             -------------------
    begin                : Dec 3, 2019
    copyright            : (C) 2019 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/Vector.h>
#include <TNL/Algorithms/ParallelFor.h>
#include <TNL/Containers/Segments/Ellpack.h>

namespace TNL {
   namespace Containers {
      namespace Segments {


template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
Ellpack()
   : segmentSize( 0 ), size( 0 ), alignedSize( 0 )
{
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
Ellpack( const SegmentsSizes& segmentsSizes )
   : segmentSize( 0 ), size( 0 ), alignedSize( 0 )
{
   this->setSegmentsSizes( segmentsSizes );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
Ellpack( const IndexType segmentsCount, const IndexType segmentSize )
   : segmentSize( 0 ), size( 0 ), alignedSize( 0 )
{
   this->setSegmentsSizes( segmentsCount, segmentSize );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
Ellpack( const Ellpack& ellpack )
   : segmentSize( ellpack.segmentSize ), size( ellpack.size ), alignedSize( ellpack.alignedSize )
{
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
Ellpack( const Ellpack&& ellpack )
   : segmentSize( ellpack.segmentSize ), size( ellpack.size ), alignedSize( ellpack.alignedSize )
{
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
String
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSerializationType()
{
   return "Ellpack< [any_device], " + TNL::getSerializationType< IndexType >() + " >";
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
String
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSegmentsType()
{
   return ViewType::getSegmentsType();
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
auto
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getView() -> ViewType
{
   return ViewType( segmentSize, size, alignedSize );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
auto
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getConstView() const -> const ConstViewType
{
   return ConstViewType( segmentSize, size, alignedSize );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
   template< typename SizesHolder >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
setSegmentsSizes( const SizesHolder& sizes )
{
   this->segmentSize = max( sizes );
   this->size = sizes.getSize();
   if( RowMajorOrder )
      this->alignedSize = this->size;
   else
      this->alignedSize = roundUpDivision( size, this->getAlignment() ) * this->getAlignment();
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
reset()
{
   this->segmentSize = 0;
   this->size = 0;
   this->alignedSize = 0;
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
setSegmentsSizes( const IndexType segmentsCount, const IndexType segmentSize )
{
   this->segmentSize = segmentSize;
   this->size = segmentsCount;
   if( RowMajorOrder )
      this->alignedSize = this->size;
   else
      this->alignedSize = roundUpDivision( size, this->getAlignment() ) * this->getAlignment();
}


template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ auto Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSegmentsCount() const -> IndexType
{
   return this->size;
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ auto Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSegmentSize( const IndexType segmentIdx ) const -> IndexType
{
   return this->segmentSize;
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ auto Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSize() const  -> IndexType
{
   return this->size * this->segmentSize;
}


template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ auto Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getStorageSize() const -> IndexType
{
   return this->alignedSize * this->segmentSize;
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ auto Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getGlobalIndex( const Index segmentIdx, const Index localIdx ) const -> IndexType
{
   if( RowMajorOrder )
      return segmentIdx * this->segmentSize + localIdx;
   else
      return segmentIdx + this->alignedSize * localIdx;
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ 
void Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSegmentAndLocalIndex( const Index globalIdx, Index& segmentIdx, Index& localIdx ) const
{
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
__cuda_callable__ auto Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
getSegmentView( const IndexType segmentIdx ) const -> SegmentViewType
{
   if( RowMajorOrder )
      return SegmentViewType( segmentIdx * this->segmentSize, this->segmentSize, 1 );
   else
      return SegmentViewType( segmentIdx, this->segmentSize, this->alignedSize );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
   template< typename Function, typename... Args >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
forSegments( IndexType first, IndexType last, Function& f, Args... args ) const
{
   this->getConstView().forSegments( first, last, f, args... );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
   template< typename Function, typename... Args >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
forAll( Function& f, Args... args ) const
{
   this->forSegments( 0, this->getSegmentsCount(), f, args... );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
   template< typename Fetch, typename Reduction, typename ResultKeeper, typename Real, typename... Args >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
segmentsReduction( IndexType first, IndexType last, Fetch& fetch, const Reduction& reduction, ResultKeeper& keeper, const Real& zero, Args... args ) const
{
   this->getConstView().segmentsReduction( first, last, fetch, reduction, keeper, zero, args... );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
   template< typename Fetch, typename Reduction, typename ResultKeeper, typename Real, typename... Args >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
allReduction( Fetch& fetch, const Reduction& reduction, ResultKeeper& keeper, const Real& zero, Args... args ) const
{
   this->segmentsReduction( 0, this->getSegmentsCount(), fetch, reduction, keeper, zero, args... );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
   template< typename Device_, typename Index_, typename IndexAllocator_, bool RowMajorOrder_, int Alignment_ >
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >&
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
operator=( const Ellpack< Device_, Index_, IndexAllocator_, RowMajorOrder_, Alignment_ >& source )
{
   this->segmentSize = source.segmentSize;
   this->size = source.size;
   this->alignedSize = roundUpDivision( size, this->getAlignment() ) * this->getAlignment();
   return *this;
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
save( File& file ) const
{
   file.save( &segmentSize );
   file.save( &size );
   file.save( &alignedSize );
}

template< typename Device,
          typename Index,
          typename IndexAllocator,
          bool RowMajorOrder,
          int Alignment >
void
Ellpack< Device, Index, IndexAllocator, RowMajorOrder, Alignment >::
load( File& file )
{
   file.load( &segmentSize );
   file.load( &size );
   file.load( &alignedSize );
}

      } // namespace Segments
   }  // namespace Conatiners
} // namespace TNL
