/***************************************************************************
                          DistributedNDArray.h  -  description
                             -------------------
    begin                : Dec 27, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky

#pragma once

#include <TNL/Communicators/MpiCommunicator.h>
#include <TNL/Containers/NDArray.h>
#include <TNL/Containers/Subrange.h>
#include <TNL/Containers/DistributedNDArrayView.h>

namespace TNL {
namespace Containers {

template< typename NDArray,
          typename Communicator = Communicators::MpiCommunicator,
          typename Overlaps = __ndarray_impl::make_constant_index_sequence< NDArray::getDimension(), 0 > >
class DistributedNDArray
{
   using CommunicationGroup = typename Communicator::CommunicationGroup;
public:
   using ValueType = typename NDArray::ValueType;
   using DeviceType = typename NDArray::DeviceType;
   using IndexType = typename NDArray::IndexType;
   using SizesHolderType = typename NDArray::SizesHolderType;
   using PermutationType = typename NDArray::PermutationType;
   using CommunicatorType = Communicator;
   using LocalBeginsType = __ndarray_impl::LocalBeginsHolder< typename NDArray::SizesHolderType >;
   using LocalRangeType = Subrange< IndexType >;
   using OverlapsType = Overlaps;

   using ViewType = DistributedNDArrayView< typename NDArray::ViewType, Communicator, Overlaps >;
   using ConstViewType = DistributedNDArrayView< typename NDArray::ConstViewType, Communicator, Overlaps >;

   static_assert( Overlaps::size() == NDArray::getDimension(), "invalid overlaps" );

   // all methods from NDArrayView

   DistributedNDArray() = default;

   // The copy-constructor of TNL::Containers::Array makes shallow copy so our
   // copy-constructor cannot be default. Actually, we most likely don't need
   // it anyway, so let's just delete it.
   DistributedNDArray( const DistributedNDArray& ) = delete;

   // Standard copy-semantics with deep copy, just like regular 1D array.
   // Mismatched sizes cause reallocations.
   DistributedNDArray& operator=( const DistributedNDArray& other ) = default;

   // default move-semantics
   DistributedNDArray( DistributedNDArray&& ) = default;
   DistributedNDArray& operator=( DistributedNDArray&& ) = default;

   static constexpr std::size_t getDimension()
   {
      return NDArray::getDimension();
   }

   __cuda_callable__
   CommunicationGroup getCommunicationGroup() const
   {
      return group;
   }

   // Returns the *global* sizes
   __cuda_callable__
   const SizesHolderType& getSizes() const
   {
      return globalSizes;
   }

   // Returns the *global* size
   template< std::size_t level >
   __cuda_callable__
   IndexType getSize() const
   {
      return globalSizes.template getSize< level >();
   }

   __cuda_callable__
   LocalBeginsType getLocalBegins() const
   {
      return localBegins;
   }

   __cuda_callable__
   SizesHolderType getLocalEnds() const
   {
      return localEnds;
   }

   template< std::size_t level >
   __cuda_callable__
   LocalRangeType getLocalRange() const
   {
      return LocalRangeType( localBegins.template getSize< level >(), localEnds.template getSize< level >() );
   }

   // returns the local storage size
   __cuda_callable__
   IndexType getLocalStorageSize() const
   {
      return localArray.getStorageSize();
   }

   template< typename... IndexTypes >
   __cuda_callable__
   ValueType&
   operator()( IndexTypes&&... indices )
   {
      static_assert( sizeof...( indices ) == getDimension(), "got wrong number of indices" );
      __ndarray_impl::assertIndicesInRange( localBegins, localEnds, Overlaps{}, std::forward< IndexTypes >( indices )... );
      return __ndarray_impl::call_with_unshifted_indices< LocalBeginsType, Overlaps >( localBegins, localArray, std::forward< IndexTypes >( indices )... );
   }

   template< typename... IndexTypes >
   __cuda_callable__
   const ValueType&
   operator()( IndexTypes&&... indices ) const
   {
      static_assert( sizeof...( indices ) == getDimension(), "got wrong number of indices" );
      __ndarray_impl::assertIndicesInRange( localBegins, localEnds, Overlaps{}, std::forward< IndexTypes >( indices )... );
      return __ndarray_impl::call_with_unshifted_indices< LocalBeginsType, Overlaps >( localBegins, localArray, std::forward< IndexTypes >( indices )... );
   }

   // bracket operator for 1D arrays
   __cuda_callable__
   ValueType&
   operator[]( IndexType index )
   {
      static_assert( getDimension() == 1, "the access via operator[] is provided only for 1D arrays" );
      __ndarray_impl::assertIndicesInRange( localBegins, localEnds, Overlaps{}, std::forward< IndexType >( index ) );
      return localArray[ __ndarray_impl::get<0>( Overlaps{} ) + index - localBegins.template getSize< 0 >() ];
   }

   __cuda_callable__
   const ValueType&
   operator[]( IndexType index ) const
   {
      static_assert( getDimension() == 1, "the access via operator[] is provided only for 1D arrays" );
      __ndarray_impl::assertIndicesInRange( localBegins, localEnds, Overlaps{}, std::forward< IndexType >( index ) );
      return localArray[ __ndarray_impl::get<0>( Overlaps{} ) + index - localBegins.template getSize< 0 >() ];
   }

   __cuda_callable__
   ViewType getView()
   {
      return ViewType( localArray.getView(), globalSizes, localBegins, localEnds, group );
   }

   __cuda_callable__
   ConstViewType getConstView() const
   {
      return ConstViewType( localArray.getConstView(), globalSizes, localBegins, localEnds, group );
   }

   // TODO: overlaps should be skipped, otherwise it works only after synchronization
   bool operator==( const DistributedNDArray& other ) const
   {
      // we can't run allreduce if the communication groups are different
      if( group != other.getCommunicationGroup() )
         return false;
      const bool localResult =
            globalSizes == other.globalSizes &&
            localBegins == other.localBegins &&
            localEnds == other.localEnds &&
            localArray == other.localArray;
      bool result = true;
      if( group != CommunicatorType::NullGroup )
         CommunicatorType::Allreduce( &localResult, &result, 1, MPI_LAND, group );
      return result;
   }

   bool operator!=( const DistributedNDArray& other ) const
   {
      return ! (*this == other);
   }

   // iterate over all local elements
   template< typename Device2 = DeviceType, typename Func >
   void forAll( Func f ) const
   {
      __ndarray_impl::ExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( localBegins, localEnds, f );
   }

   // iterate over local elements which are not neighbours of *global* boundaries
   template< typename Device2 = DeviceType, typename Func >
   void forInternal( Func f ) const
   {
      // add static sizes
      using Begins = __ndarray_impl::LocalBeginsHolder< SizesHolderType, 1 >;
      // add dynamic sizes
      Begins begins;
      __ndarray_impl::SetSizesAddHelper< 1, Begins, SizesHolderType, Overlaps >::add( begins, SizesHolderType{} );
      __ndarray_impl::SetSizesMaxHelper< Begins, LocalBeginsType >::max( begins, localBegins );

      // subtract static sizes
      using Ends = typename __ndarray_impl::SubtractedSizesHolder< SizesHolderType, 1 >::type;
      // subtract dynamic sizes
      Ends ends;
      __ndarray_impl::SetSizesSubtractHelper< 1, Ends, SizesHolderType, Overlaps >::subtract( ends, globalSizes );
      __ndarray_impl::SetSizesMinHelper< Ends, SizesHolderType >::min( ends, localEnds );

      __ndarray_impl::ExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( begins, ends, f );
   }

   // iterate over local elements inside the given [begins, ends) range specified by global indices
   template< typename Device2 = DeviceType, typename Func, typename Begins, typename Ends >
   void forInternal( Func f, const Begins& begins, const Ends& ends ) const
   {
      // TODO: assert "localBegins <= begins <= localEnds", "localBegins <= ends <= localEnds"
      __ndarray_impl::ExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( begins, ends, f );
   }

   // iterate over local elements which are neighbours of *global* boundaries
   template< typename Device2 = DeviceType, typename Func >
   void forBoundary( Func f ) const
   {
      // add static sizes
      using SkipBegins = __ndarray_impl::LocalBeginsHolder< SizesHolderType, 1 >;
      // add dynamic sizes
      SkipBegins skipBegins;
      __ndarray_impl::SetSizesAddHelper< 1, SkipBegins, SizesHolderType, Overlaps >::add( skipBegins, SizesHolderType{} );
      __ndarray_impl::SetSizesMaxHelper< SkipBegins, LocalBeginsType >::max( skipBegins, localBegins );

      // subtract static sizes
      using SkipEnds = typename __ndarray_impl::SubtractedSizesHolder< SizesHolderType, 1 >::type;
      // subtract dynamic sizes
      SkipEnds skipEnds;
      __ndarray_impl::SetSizesSubtractHelper< 1, SkipEnds, SizesHolderType, Overlaps >::subtract( skipEnds, globalSizes );
      __ndarray_impl::SetSizesMinHelper< SkipEnds, SizesHolderType >::min( skipEnds, localEnds );

      __ndarray_impl::BoundaryExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( localBegins, skipBegins, skipEnds, localEnds, f );
   }

   // iterate over local elements outside the given [skipBegins, skipEnds) range specified by global indices
   template< typename Device2 = DeviceType, typename Func, typename SkipBegins, typename SkipEnds >
   void forBoundary( Func f, const SkipBegins& skipBegins, const SkipEnds& skipEnds ) const
   {
      // TODO: assert "localBegins <= skipBegins <= localEnds", "localBegins <= skipEnds <= localEnds"
      __ndarray_impl::BoundaryExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( localBegins, skipBegins, skipEnds, localEnds, f );
   }

   // iterate over local elements which are not neighbours of overlaps (if all overlaps are 0, it is equivalent to forAll)
   template< typename Device2 = DeviceType, typename Func >
   void forLocalInternal( Func f ) const
   {
      // add dynamic sizes
      LocalBeginsType begins;
      __ndarray_impl::SetSizesAddHelper< 1, LocalBeginsType, SizesHolderType, Overlaps >::add( begins, localBegins, false );

      // subtract dynamic sizes
      SizesHolderType ends;
      __ndarray_impl::SetSizesSubtractHelper< 1, SizesHolderType, SizesHolderType, Overlaps >::subtract( ends, localEnds, false );

      __ndarray_impl::ExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( begins, ends, f );
   }

   // iterate over local elements which are neighbours of overlaps (if all overlaps are 0, it has no effect)
   template< typename Device2 = DeviceType, typename Func >
   void forLocalBoundary( Func f ) const
   {
      // add dynamic sizes
      LocalBeginsType skipBegins;
      __ndarray_impl::SetSizesAddHelper< 1, LocalBeginsType, SizesHolderType, Overlaps >::add( skipBegins, localBegins, false );

      // subtract dynamic sizes
      SizesHolderType skipEnds;
      __ndarray_impl::SetSizesSubtractHelper< 1, SizesHolderType, SizesHolderType, Overlaps >::subtract( skipEnds, localEnds, false );

      __ndarray_impl::BoundaryExecutorDispatcher< PermutationType, Device2 > dispatch;
      dispatch( localBegins, skipBegins, skipEnds, localEnds, f );
   }


   // extra methods

   // Sets the *global* size, but does not allocate storage
   template< typename... IndexTypes >
   void setSizes( IndexTypes&&... sizes )
   {
      static_assert( sizeof...( sizes ) == getDimension(), "got wrong number of sizes" );
      __ndarray_impl::setSizesHelper( globalSizes, std::forward< IndexTypes >( sizes )... );
      // initialize localBegins and localEnds
      localBegins = LocalBeginsType{};
      localEnds = globalSizes;
   }

   template< std::size_t level >
   void setDistribution( IndexType begin, IndexType end, CommunicationGroup group = Communicator::AllGroup )
   {
      static_assert( SizesHolderType::template getStaticSize< level >() == 0, "NDArray cannot be distributed in static dimensions." );
      TNL_ASSERT_GE( begin, 0, "begin must be non-negative" );
      TNL_ASSERT_LE( end, globalSizes.template getSize< level >(), "end must not be greater than global size" );
      TNL_ASSERT_LT( begin, end, "begin must be lesser than end" );
      localBegins.template setSize< level >( begin );
      localEnds.template setSize< level >( end );
      TNL_ASSERT( this->group == Communicator::NullGroup || this->group == group,
                  std::cerr << "different groups cannot be combined for different dimensions" );
      this->group = group;
   }

   // Computes the distributed storage size and allocates the local array
   void allocate()
   {
      SizesHolderType localSizes;
      TemplateStaticFor< std::size_t, 0, SizesHolderType::getDimension(), LocalSizesSetter >::execHost( localSizes, globalSizes, localBegins, localEnds );
      localArray.setSize( localSizes );
   }

   void setLike( const DistributedNDArray& other )
   {
      localArray.setLike( other.localArray );
      group = other.getCommunicationGroup();
      globalSizes = other.getSizes();
      localBegins = other.localBegins;
      localEnds = other.localEnds;
   }

   void reset()
   {
      localArray.reset();
      group = CommunicatorType::NullGroup;
      globalSizes = SizesHolderType{};
      localBegins = LocalBeginsType{};
      localEnds = SizesHolderType{};
   }

   // "safe" accessor - will do slow copy from device
   template< typename... IndexTypes >
   ValueType
   getElement( IndexTypes&&... indices ) const
   {
      static_assert( sizeof...( indices ) == getDimension(), "got wrong number of indices" );
      __ndarray_impl::assertIndicesInRange( localBegins, localEnds, Overlaps{}, std::forward< IndexTypes >( indices )... );
      auto getElement = [this]( auto&&... indices )
      {
         return this->localArray.getElement( std::forward< decltype(indices) >( indices )... );
      };
      return __ndarray_impl::host_call_with_unshifted_indices< LocalBeginsType, Overlaps >( localBegins, getElement, std::forward< IndexTypes >( indices )... );
   }

   void setValue( ValueType value )
   {
      localArray.setValue( value );
   }

protected:
   NDArray localArray;
   CommunicationGroup group = Communicator::NullGroup;
   SizesHolderType globalSizes;
   // static sizes should have different type: localBegin is always 0, localEnd is always the full size
   LocalBeginsType localBegins;
   SizesHolderType localEnds;

private:
   template< std::size_t level >
   struct LocalSizesSetter
   {
      template< typename SizesHolder, typename LocalBegins >
      static void exec( SizesHolder& localSizes, const SizesHolder& globalSizes, const LocalBegins& localBegins, const SizesHolder& localEnds )
      {
         if( SizesHolder::template getStaticSize< level >() != 0 )
            return;

         const auto begin = localBegins.template getSize< level >();
         const auto end = localEnds.template getSize< level >();
         if( begin == end )
            localSizes.template setSize< level >( globalSizes.template getSize< level >() );
         else {
            TNL_ASSERT_GE( end - begin, (decltype(end)) __ndarray_impl::get<level>( Overlaps{} ), "local size is less than the size of overlaps" );
            localSizes.template setSize< level >( end - begin + 2 * __ndarray_impl::get<level>( Overlaps{} ) );
         }
      }
   };
};

} // namespace Containers
} // namespace TNL
