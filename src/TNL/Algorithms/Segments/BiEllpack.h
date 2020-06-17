/***************************************************************************
                          BiEllpack.h -  description
                             -------------------
    begin                : Apr 5, 2020
    copyright            : (C) 2020 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Allocators/Default.h>
#include <TNL/Containers/Vector.h>
#include <TNL/Algorithms/Segments/BiEllpackView.h>
#include <TNL/Algorithms/Segments/SegmentView.h>

namespace TNL {
   namespace Algorithms {
      namespace Segments {

template< typename Device,
          typename Index,
          typename IndexAllocator = typename Allocators::Default< Device >::template Allocator< Index >,
          ElementsOrganization Organization = Algorithms::Segments::DefaultElementsOrganization< Device >::getOrganization(),
          int WarpSize = 32 >
class BiEllpack
{
   public:

      using DeviceType = Device;
      using IndexType = std::remove_const_t< Index >;
      using OffsetsHolder = Containers::Vector< Index, DeviceType, IndexType, IndexAllocator >;
      static constexpr bool getOrganization() { return Organization; }
      using ViewType = BiEllpackView< Device, Index, Organization >;
      template< typename Device_, typename Index_ >
      using ViewTemplate = BiEllpackView< Device_, Index_, Organization >;
      using ConstViewType = BiEllpackView< Device, std::add_const_t< IndexType >, Organization >;
      using SegmentViewType = BiEllpackSegmentView< IndexType, Organization >;

      BiEllpack() = default;

      BiEllpack( const Containers::Vector< IndexType, DeviceType, IndexType >& sizes );

      BiEllpack( const BiEllpack& segments );

      BiEllpack( const BiEllpack&& segments );

      static String getSerializationType();

      static String getSegmentsType();

      ViewType getView();

      const ConstViewType getConstView() const;

      /**
       * \brief Number of segments.
       */
      __cuda_callable__
      IndexType getSegmentsCount() const;

      /**
       * \brief Set sizes of particular segments.
       */
      template< typename SizesHolder = OffsetsHolder >
      void setSegmentsSizes( const SizesHolder& sizes );

      void reset();

      IndexType getSegmentSize( const IndexType segmentIdx ) const;

      /**
       * \brief Number segments.
       */
      __cuda_callable__
      IndexType getSize() const;

      __cuda_callable__
      IndexType getStorageSize() const;

      __cuda_callable__
      IndexType getGlobalIndex( const IndexType segmentIdx, const IndexType localIdx ) const;

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

      BiEllpack& operator=( const BiEllpack& source ) = default;

      template< typename Device_, typename Index_, typename IndexAllocator_, ElementsOrganization Organization_ >
      BiEllpack& operator=( const BiEllpack< Device_, Index_, IndexAllocator_, Organization_, WarpSize >& source );

      void save( File& file ) const;

      void load( File& file );

      void printStructure( std::ostream& str ) const;

      // TODO: nvcc needs this public because of lambda function used inside
      template< typename SizesHolder = OffsetsHolder >
      void performRowBubbleSort( const SizesHolder& segmentsSize );

      // TODO: the same as  above
      template< typename SizesHolder = OffsetsHolder >
      void computeColumnSizes( const SizesHolder& segmentsSizes );

   protected:

      static constexpr int getWarpSize() { return WarpSize; };

      static constexpr int getLogWarpSize() { return std::log2( WarpSize ); };

      template< typename SizesHolder = OffsetsHolder >
      void verifyRowPerm( const SizesHolder& segmentsSizes );

      template< typename SizesHolder = OffsetsHolder >
      void verifyRowLengths( const SizesHolder& segmentsSizes );

      IndexType getStripLength( const IndexType stripIdx ) const;

      IndexType getGroupLength( const IndexType strip, const IndexType group ) const;

      IndexType size = 0, storageSize = 0;

      IndexType virtualRows = 0;

      OffsetsHolder rowPermArray;

      OffsetsHolder groupPointers;

      // TODO: Replace later
      __cuda_callable__ Index power( const IndexType number, const IndexType exponent ) const
      {
          if( exponent >= 0 )
          {
              IndexType result = 1;
              for( IndexType i = 0; i < exponent; i++ )
                  result *= number;
              return result;
          }
          return 0;
      };

      template< typename Device_, typename Index_, typename IndexAllocator_, ElementsOrganization Organization_, int WarpSize_ >
      friend class BiEllpack;
};

      } // namespace Segments
   }  // namespace Algorithms
} // namespace TNL

#include <TNL/Algorithms/Segments/BiEllpack.hpp>