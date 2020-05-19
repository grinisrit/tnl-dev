/***************************************************************************
                          SlicedEllpackView.h -  description
                             -------------------
    begin                : Dec 12, 2019
    copyright            : (C) 2019 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <type_traits>

#include <TNL/Containers/Vector.h>
#include <TNL/Containers/Segments/ElementsOrganization.h>
#include <TNL/Containers/Segments/SegmentView.h>

namespace TNL {
   namespace Containers {
      namespace Segments {

template< typename Device,
          typename Index,
          ElementsOrganization Organization = Containers::Segments::DefaultElementsOrganization< Device >::getOrganization(),
          int SliceSize = 32 >
class SlicedEllpackView
{
   public:

      using DeviceType = Device;
      using IndexType = std::remove_const_t< Index >;
      using OffsetsView = typename Containers::VectorView< Index, DeviceType, IndexType >;
      static constexpr int getSliceSize() { return SliceSize; }
      static constexpr bool getOrganization() { return Organization; }
      template< typename Device_, typename Index_ >
      using ViewTemplate = SlicedEllpackView< Device_, Index_, Organization, SliceSize >;
      using ViewType = SlicedEllpackView;
      using ConstViewType = ViewType;
      using SegmentViewType = SegmentView< IndexType, Organization >;

      __cuda_callable__
      SlicedEllpackView();

      __cuda_callable__
      SlicedEllpackView( IndexType size,
                         IndexType alignedSize,
                         IndexType segmentsCount,
                         OffsetsView&& sliceOffsets,
                         OffsetsView&& sliceSegmentSizes );

      __cuda_callable__
      SlicedEllpackView( const SlicedEllpackView& slicedEllpackView );

      __cuda_callable__
      SlicedEllpackView( const SlicedEllpackView&& slicedEllpackView );

      static String getSerializationType();

      static String getSegmentsType();

      __cuda_callable__
      ViewType getView();

      __cuda_callable__
      const ConstViewType getConstView() const;

      __cuda_callable__
      IndexType getSegmentsCount() const;

      __cuda_callable__
      IndexType getSegmentSize( const IndexType segmentIdx ) const;

      /**
       * \brief Number segments.
       */
      __cuda_callable__
      IndexType getSize() const;

      __cuda_callable__
      IndexType getStorageSize() const;

      __cuda_callable__
      IndexType getGlobalIndex( const Index segmentIdx, const Index localIdx ) const;

      __cuda_callable__
      void getSegmentAndLocalIndex( const Index globalIdx, Index& segmentIdx, Index& localIdx ) const;

      __cuda_callable__
      SegmentViewType getSegmentView( const IndexType segmentIdx ) const;

      /***
       * \brief Go over all segments and for each segment element call
       * function 'f' with arguments 'args'. The return type of 'f' is bool.
       * When its true, the for-loop continues. Once 'f' returns false, the for-loop
       * is terminated.
       */
      template< typename Function, typename... Args >
      void forSegments( IndexType first, IndexType last, Function& f, Args... args ) const;

      template< typename Function, typename... Args >
      void forAll( Function& f, Args... args ) const;


      /***
       * \brief Go over all segments and perform a reduction in each of them.
       */
      template< typename Fetch, typename Reduction, typename ResultKeeper, typename Real, typename... Args >
      void segmentsReduction( IndexType first, IndexType last, Fetch& fetch, const Reduction& reduction, ResultKeeper& keeper, const Real& zero, Args... args ) const;

      template< typename Fetch, typename Reduction, typename ResultKeeper, typename Real, typename... Args >
      void allReduction( Fetch& fetch, const Reduction& reduction, ResultKeeper& keeper, const Real& zero, Args... args ) const;

      SlicedEllpackView& operator=( const SlicedEllpackView& view );

      void save( File& file ) const;

      void load( File& file );

   protected:

      IndexType size, alignedSize, segmentsCount;

      OffsetsView sliceOffsets, sliceSegmentSizes;
};

      } // namespace Segements
   }  // namespace Conatiners
} // namespace TNL

#include <TNL/Containers/Segments/SlicedEllpackView.hpp>