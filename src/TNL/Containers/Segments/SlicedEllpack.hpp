/***************************************************************************
                          SlicedEllpack.hpp -  description
                             -------------------
    begin                : Dec 4, 2019
    copyright            : (C) 2019 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/Vector.h>
#include <TNL/Algorithms/ParallelFor.h>
#include <TNL/Containers/Segments/SlicedEllpack.h>
#include <TNL/Containers/Segments/Ellpack.h>

namespace TNL {
   namespace Containers {
      namespace Segments {


template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
SlicedEllpack()
   : size( 0 ), alignedSize( 0 ), segmentsCount( 0 )
{
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
SlicedEllpack( const Vector< IndexType, DeviceType, IndexType >& sizes )
   : size( 0 ), alignedSize( 0 ), segmentsCount( 0 )
{
   this->setSegmentsSizes( sizes );
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
SlicedEllpack( const SlicedEllpack& slicedEllpack )
   : size( slicedEllpack.size ), alignedSize( slicedEllpack.alignedSize ),
     segmentsCount( slicedEllpack.segmentsCount ), sliceOffsets( slicedEllpack.sliceOffsets ),
     sliceSegmentSizes( slicedEllpack.sliceSegmentSizes )
{
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
SlicedEllpack( const SlicedEllpack&& slicedEllpack )
   : size( slicedEllpack.size ), alignedSize( slicedEllpack.alignedSize ),
     segmentsCount( slicedEllpack.segmentsCount ), sliceOffsets( slicedEllpack.sliceOffsets ),
     sliceSegmentSizes( slicedEllpack.sliceSegmentSizes )
{
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
   template< typename SizesHolder >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
setSegmentsSizes( const SizesHolder& sizes )
{
   this->segmentsCount = sizes.getSize();
   const IndexType slicesCount = roundUpDivision( this->segmentsCount, getSliceSize() );
   this->sliceOffsets.setSize( slicesCount + 1 );
   this->sliceOffsets = 0;
   this->sliceSegmentSizes.setSize( slicesCount );
   Ellpack< DeviceType, IndexType, true > ellpack;
   ellpack.setSegmentsSizes( slicesCount, SliceSize );

   const IndexType _size = sizes.getSize();
   const auto sizes_view = sizes.getConstView();
   auto slices_view = this->sliceOffsets.getView();
   auto slice_segment_size_view = this->sliceSegmentSizes.getView();
   auto fetch = [=] __cuda_callable__ ( IndexType segmentIdx, IndexType globalIdx ) -> IndexType {
      if( globalIdx < _size )
         return sizes_view[ globalIdx ];
      return 0;
   };
   auto reduce = [] __cuda_callable__ ( IndexType& aux, const IndexType i ) {
      aux = TNL::max( aux, i );
   };
   auto keep = [=] __cuda_callable__ ( IndexType i, IndexType res ) mutable {
      slices_view[ i ] = res * SliceSize;
      slice_segment_size_view[ i ] = res;
   };
   ellpack.allReduction( fetch, reduce, keep, std::numeric_limits< IndexType >::min() );
   this->sliceOffsets.template scan< Algorithms::ScanType::Exclusive >();
   this->size = sum( sizes );
   this->alignedSize = this->sliceOffsets.getElement( slicesCount );
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
__cuda_callable__
Index
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
getSegmentsCount() const
{
   return this->segmentsCount;
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
__cuda_callable__
Index
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
getSegmentSize( const IndexType segmentIdx ) const
{
   const Index sliceIdx = segmentIdx / SliceSize;
   if( std::is_same< DeviceType, Devices::Host >::value )
      return this->sliceSegmentSizes[ sliceIdx ];
   else
   {
#ifdef __CUDA_ARCH__
   return this->sliceSegmentSizes[ sliceIdx ];
#else
   return this->sliceSegmentSizes.getElement( sliceIdx );
#endif
   }
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
__cuda_callable__
Index
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
getSize() const
{
   return this->size;
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
__cuda_callable__
Index
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
getStorageSize() const
{
   return this->alignedSize;
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
__cuda_callable__
Index
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
getGlobalIndex( const Index segmentIdx, const Index localIdx ) const
{
   const IndexType sliceIdx = segmentIdx / SliceSize;
   const IndexType segmentInSliceIdx = segmentIdx % SliceSize;
   IndexType sliceOffset, segmentSize;
   if( std::is_same< DeviceType, Devices::Host >::value )
   {
      sliceOffset = this->sliceOffsets[ sliceIdx ];
      segmentSize = this->sliceSegmentSizes[ sliceIdx ];
   }
   else
   {
#ifdef __CUDA__ARCH__
      sliceOffset = this->sliceOffsets[ sliceIdx ];
      segmentSize = this->sliceSegmentSizes[ sliceIdx ];
#else
      sliceOffset = this->sliceOffsets.getElement( sliceIdx );
      segmentSize = this->sliceSegmentSizes.getElement( sliceIdx );
#endif
   }
   if( RowMajorOrder )
      return sliceOffset + segmentInSliceIdx * segmentSize + localIdx;
   else
      return sliceOffset + segmentInSliceIdx + SliceSize * localIdx;
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
__cuda_callable__
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
getSegmentAndLocalIndex( const Index globalIdx, Index& segmentIdx, Index& localIdx ) const
{
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
   template< typename Function, typename... Args >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
forSegments( IndexType first, IndexType last, Function& f, Args... args ) const
{
   const auto sliceSegmentSizes_view = this->sliceSegmentSizes.getConstView();
   const auto sliceOffsets_view = this->sliceOffsets.getConstView();
   if( RowMajorOrder )
   {
      auto l = [=] __cuda_callable__ ( const IndexType segmentIdx, Args... args ) {
         const IndexType sliceIdx = segmentIdx / SliceSize;
         const IndexType segmentInSliceIdx = segmentIdx % SliceSize;
         const IndexType segmentSize = sliceSegmentSizes_view[ sliceIdx ];
         const IndexType begin = sliceOffsets_view[ sliceIdx ] + segmentInSliceIdx * segmentSize;
         const IndexType end = begin + segmentSize;
         for( IndexType globalIdx = begin; globalIdx < end; globalIdx++  )
            if( ! f( segmentIdx, globalIdx, args... ) )
               break;
      };
      Algorithms::ParallelFor< Device >::exec( first, last, l, args... );
   }
   else
   {
      auto l = [=] __cuda_callable__ ( const IndexType segmentIdx, Args... args ) {
         const IndexType sliceIdx = segmentIdx / SliceSize;
         const IndexType segmentInSliceIdx = segmentIdx % SliceSize;
         const IndexType segmentSize = sliceSegmentSizes_view[ sliceIdx ];
         const IndexType begin = sliceOffsets_view[ sliceIdx ] + segmentInSliceIdx;
         const IndexType end = sliceOffsets_view[ sliceIdx + 1 ];
         for( IndexType globalIdx = begin; globalIdx < end; globalIdx += SliceSize )
            if( ! f( segmentIdx, globalIdx, args... ) )
               break;
      };
      Algorithms::ParallelFor< Device >::exec( first, last, l, args... );
   }
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
   template< typename Function, typename... Args >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
forAll( Function& f, Args... args ) const
{
   this->forSegments( 0, this->getSegmentsCount(), f, args... );
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
   template< typename Fetch, typename Reduction, typename ResultKeeper, typename Real, typename... Args >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
segmentsReduction( IndexType first, IndexType last, Fetch& fetch, Reduction& reduction, ResultKeeper& keeper, const Real& zero, Args... args ) const
{
   using RealType = decltype( fetch( IndexType(), IndexType() ) );
   const auto sliceSegmentSizes_view = this->sliceSegmentSizes.getConstView();
   const auto sliceOffsets_view = this->sliceOffsets.getConstView();
   if( RowMajorOrder )
   {
      auto l = [=] __cuda_callable__ ( const IndexType segmentIdx, Args... args ) mutable {
         const IndexType sliceIdx = segmentIdx / SliceSize;
         const IndexType segmentInSliceIdx = segmentIdx % SliceSize;
         const IndexType segmentSize = sliceSegmentSizes_view[ sliceIdx ];
         const IndexType begin = sliceOffsets_view[ sliceIdx ] + segmentInSliceIdx * segmentSize;
         const IndexType end = begin + segmentSize;
         RealType aux( zero );
         for( IndexType globalIdx = begin; globalIdx< end; globalIdx++  )
            reduction( aux, fetch( segmentIdx, globalIdx, args... ) );
         keeper( segmentIdx, aux );
      };
      Algorithms::ParallelFor< Device >::exec( first, last, l, args... );
   }
   else
   {
      auto l = [=] __cuda_callable__ ( const IndexType segmentIdx, Args... args ) mutable {
         const IndexType sliceIdx = segmentIdx / SliceSize;
         const IndexType segmentInSliceIdx = segmentIdx % SliceSize;
         const IndexType segmentSize = sliceSegmentSizes_view[ sliceIdx ];
         const IndexType begin = sliceOffsets_view[ sliceIdx ] + segmentInSliceIdx;
         const IndexType end = sliceOffsets_view[ sliceIdx + 1 ];
         RealType aux( zero );
         for( IndexType globalIdx = begin; globalIdx < end; globalIdx += SliceSize  )
            reduction( aux, fetch( segmentIdx, globalIdx, args... ) );
         keeper( segmentIdx, aux );
      };
      Algorithms::ParallelFor< Device >::exec( first, last, l, args... );
   }
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
   template< typename Fetch, typename Reduction, typename ResultKeeper, typename Real, typename... Args >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
allReduction( Fetch& fetch, Reduction& reduction, ResultKeeper& keeper, const Real& zero, Args... args ) const
{
   this->segmentsReduction( 0, this->getSegmentsCount(), fetch, reduction, keeper, zero, args... );
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
save( File& file ) const
{
   file.save( &size );
   file.save( &alignedSize );
   file.save( &segmentsCount );
   this->sliceOffsets.save( file );
   this->sliceSegmentSizes.save( file );
}

template< typename Device,
          typename Index,
          bool RowMajorOrder,
          int SliceSize >
void
SlicedEllpack< Device, Index, RowMajorOrder, SliceSize >::
load( File& file )
{
   file.load( &size );
   file.load( &alignedSize );
   file.load( &segmentsCount );
   this->sliceOffsets.load( file );
   this->sliceSegmentSizes.load( file );
}

      } // namespace Segments
   }  // namespace Conatiners
} // namespace TNL
