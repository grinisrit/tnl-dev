/***************************************************************************
                          CSRKernels.h -  description
                             -------------------
    begin                : Jan 20, 2021 -> Joe Biden inauguration
    copyright            : (C) 2021 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Assert.h>
#include <TNL/Cuda/LaunchHelpers.h>
#include <TNL/Containers/VectorView.h>
#include <TNL/Algorithms/ParallelFor.h>
#include <TNL/Algorithms/Segments/details/LambdaAdapter.h>

namespace TNL {
   namespace Algorithms {
      namespace Segments {

enum class Type {
   /* LONG = 0!!! Non zero value rewrites index[1] */
   LONG = 0,
   STREAM = 1,
   VECTOR = 2
};

template<typename Index>
union Block {
   Block(Index row, Type type = Type::VECTOR, Index index = 0) noexcept {
      this->index[0] = row;
      this->index[1] = index;
      this->byte[sizeof(Index) == 4 ? 7 : 15] = (uint8_t)type;
   }

   Block(Index row, Type type, Index nextRow, Index maxID, Index minID) noexcept {
      this->index[0] = row;
      this->index[1] = 0;
      this->twobytes[sizeof(Index) == 4 ? 2 : 4] = maxID - minID;

      if (type == Type::STREAM)
         this->twobytes[sizeof(Index) == 4 ? 3 : 5] = nextRow - row;

      if (type == Type::STREAM)
         this->byte[sizeof(Index) == 4 ? 7 : 15] |= 0b1000000;
      else if (type == Type::VECTOR)
         this->byte[sizeof(Index) == 4 ? 7 : 15] |= 0b10000000;
   }

   Block() = default;

   Index index[2]; // index[0] is row pointer, index[1] is index in warp
   uint8_t byte[sizeof(Index) == 4 ? 8 : 16]; // byte[7/15] is type specificator
   uint16_t twobytes[sizeof(Index) == 4 ? 4 : 8]; //twobytes[2/4] is maxID - minID
                                                //twobytes[3/5] is nextRow - row
};

#ifdef HAVE_CUDA

template< typename Real,
          typename Index,
          int warpSize,
          int WARPS,
          int SHARED_PER_WARP,
          int MAX_ELEM_PER_WARP >
__global__
void SpMVCSRAdaptive( const Real *inVector,
                      Real *outVector,
                      const Index* rowPointers,
                      const Index* columnIndexes,
                      const Real* values,
                      const Block<Index> *blocks,
                      Index blocksSize,
                      Index gridID) {
   __shared__ Real shared[WARPS][SHARED_PER_WARP];
   const Index index = (gridID * MAX_X_DIM) + (blockIdx.x * blockDim.x) + threadIdx.x;
   const Index blockIdx = index / warpSize;
   if (blockIdx >= blocksSize)
      return;

   Real result = 0.0;
   const Index laneID = threadIdx.x & 31; // & is cheaper than %
   Block<Index> block = blocks[blockIdx];
   const Index minID = rowPointers[block.index[0]/* minRow */];
   Index i, to, maxID;
   if (block.byte[sizeof(Index) == 4 ? 7 : 15] & 0b1000000) {
      /////////////////////////////////////* CSR STREAM *//////////////
      const Index warpID = threadIdx.x / 32;
      maxID = minID + /* maxID - minID */block.twobytes[sizeof(Index) == 4 ? 2 : 4];

      /* Stream data to shared memory */
      for (i = laneID + minID; i < maxID; i += warpSize)
         shared[warpID][i - minID] = values[i] * inVector[columnIndexes[i]];

      const Index maxRow = block.index[0]/* minRow */ +
         /* maxRow - minRow */(block.twobytes[sizeof(Index) == 4 ? 3 : 5] & 0x3FFF);
      /* Calculate result */
      for (i = block.index[0]/* minRow */ + laneID; i < maxRow; i += warpSize) {
         to = rowPointers[i + 1] - minID; // end of preprocessed data
         result = 0;
         /* Scalar reduction */
         for (Index sharedID = rowPointers[i] - minID; sharedID < to; ++sharedID)
            result += shared[warpID][sharedID];

         outVector[i] = result; // Write result
      }
   } else if (block.byte[sizeof(Index) == 4 ? 7 : 15] & 0b10000000) {
      /////////////////////////////////////* CSR VECTOR *//////////////
      maxID = minID + /* maxID - minID */block.twobytes[sizeof(Index) == 4 ? 2 : 4];

      for (i = minID + laneID; i < maxID; i += warpSize)
         result += values[i] * inVector[columnIndexes[i]];

      /* Parallel reduction */
      result += __shfl_down_sync(0xFFFFFFFF, result, 16);
      result += __shfl_down_sync(0xFFFFFFFF, result, 8);
      result += __shfl_down_sync(0xFFFFFFFF, result, 4);
      result += __shfl_down_sync(0xFFFFFFFF, result, 2);
      result += __shfl_down_sync(0xFFFFFFFF, result, 1);
      if (laneID == 0) outVector[block.index[0]/* minRow */] = result; // Write result
   } else {
      /////////////////////////////////////* CSR VECTOR L */////////////
      /* Number of elements processed by previous warps */
      const Index offset = block.index[1]/* warpInRow */ * MAX_ELEM_PER_WARP;
      to = minID + (block.index[1]/* warpInRow */ + 1) * MAX_ELEM_PER_WARP;
      maxID = rowPointers[block.index[0]/* minRow */ + 1];
      if (to > maxID) to = maxID;
      for (i = minID + offset + laneID; i < to; i += warpSize)
         result += values[i] * inVector[columnIndexes[i]];

      /* Parallel reduction */
      result += __shfl_down_sync(0xFFFFFFFF, result, 16);
      result += __shfl_down_sync(0xFFFFFFFF, result, 8);
      result += __shfl_down_sync(0xFFFFFFFF, result, 4);
      result += __shfl_down_sync(0xFFFFFFFF, result, 2);
      result += __shfl_down_sync(0xFFFFFFFF, result, 1);
      if (laneID == 0) atomicAdd(&outVector[block.index[0]/* minRow */], result);
   }
}
#endif


template< typename Index,
          typename Device >
struct CSRKernelAdaptiveView
{
    using IndexType = Index;
    using DeviceType = Device;
    using ViewType = CSRKernelAdaptiveView< Index, Device >;
    using ConstViewType = CSRKernelAdaptiveView< Index, Device >;

    ViewType getView() { return *this; };

    ConstViewType getConstView() const { return *this; };

    template< typename OffsetsView,
              typename Fetch,
              typename Reduction,
              typename ResultKeeper,
              typename Real,
              typename... Args >
    void segmentsReduction( const OffsetsView& offsets,
                        Index first,
                        Index last,
                        Fetch& fetch,
                        const Reduction& reduction,
                        ResultKeeper& keeper,
                        const Real& zero,
                        Args... args ) const
    {

            Index blocks;
   const Index threads = matrix.THREADS_ADAPTIVE;

   /* Fill blocks */
   size_t neededThreads = matrix.blocks.getSize() * warpSize; // one warp per block
   /* Execute kernels on device */
   for (Index grid = 0; neededThreads != 0; ++grid) {
      if (MAX_X_DIM * threads >= neededThreads) {
         blocks = roundUpDivision(neededThreads, threads);
         neededThreads = 0;
      } else {
         blocks = MAX_X_DIM;
         neededThreads -= MAX_X_DIM * threads;
      }

      SpMVCSRAdaptive< Real, Index, warpSize,
            matrix.WARPS,
            matrix.SHARED_PER_WARP, 
            matrix.MAX_ELEMENTS_PER_WARP_ADAPT >
         <<<blocks, threads>>>(
               inVector,
               outVector,
               matrix.getRowPointers().getData(),
               matrix.getColumnIndexes().getData(),
               matrix.getValues().getData(),
               matrix.blocks.getData(),
               matrix.blocks.getSize() - 1, // last block shouldn't be used
               grid
      );
   }
    }
};

template< typename Index,
          typename Device >
struct CSRKernelAdaptive
{
    using IndexType = Index;
    using DeviceType = Device;
    using ViewType = CSRKernelAdaptiveView< Index, Device >;
    using ConstViewType = CSRKernelAdaptiveView< Index, Device >;

    static constexpr Index THREADS_ADAPTIVE = sizeof(Index) == 8 ? 128 : 256;

   /* How many shared memory use per block in CSR Adaptive kernel */
   static constexpr Index SHARED_PER_BLOCK = 24576;

   /* Number of elements in shared memory */
   static constexpr Index SHARED = SHARED_PER_BLOCK/sizeof(double);

   /* Number of warps in block for CSR Adaptive */
   static constexpr Index WARPS = THREADS_ADAPTIVE / 32;

   /* Number of elements in shared memory per one warp */
   static constexpr Index SHARED_PER_WARP = SHARED / WARPS;

    template< typename Offsets >
    Index findLimit(const Index start,
                const Offsets& offsets,
                const Index size,
                Type &type,
                Index &sum) {
    sum = 0;
    for (Index current = start; current < size - 1; ++current) {
        Index elements = offsets.getElement(current + 1) -
                         offsets.getElement(current);
        sum += elements;
        if (sum > matrix.SHARED_PER_WARP) {
            if (current - start > 0) { // extra row
                type = Type::STREAM;
                return current;
            } else {                  // one long row
                if (sum <= 2 * matrix.MAX_ELEMENTS_PER_WARP_ADAPT)
                type = Type::VECTOR;
                else
                type = Type::LONG;
                return current + 1;
            }
        }
    }

    type = Type::STREAM;
    return size - 1; // return last row pointer
    }

    template< typename Offsets >
    void init( const Offsets& offsets )
    {
        const Index rows = offsets.getSize();
        Index sum, start = 0, nextStart = 0;

        // Fill blocks
        std::vector<Block<Index>> inBlock;
        inBlock.reserve(rows);

        while (nextStart != rows - 1)
        {
            Type type;
            nextStart = findLimit<Real, Index, Device, KernelType>(
                start, *this, rows, type, sum );

            if (type == Type::LONG)
            {
                Index parts = roundUpDivision(sum, this->SHARED_PER_WARP);
                for (Index index = 0; index < parts; ++index)
                {
                    inBlock.emplace_back(start, Type::LONG, index);
                }
            }
            else
            {
                inBlock.emplace_back(start, type,
                    nextStart,
                    this->rowPointers.getElement(nextStart),
                    this->rowPointers.getElement(start) );
            }
            start = nextStart;
        }
        inBlock.emplace_back(nextStart);

        // Copy values
        this->blocks.setSize(inBlock.size());
        for (size_t i = 0; i < inBlock.size(); ++i)
            this->blocks.setElement(i, inBlock[i]);
    };

    ViewType getView() { return view; };

    ConstViewType getConstView() const { return ConstViewType(); };

    template< typename OffsetsView,
              typename Fetch,
              typename Reduction,
              typename ResultKeeper,
              typename Real,
              typename... Args >
    void segmentsReduction( const OffsetsView& offsets,
                        Index first,
                        Index last,
                        Fetch& fetch,
                        const Reduction& reduction,
                        ResultKeeper& keeper,
                        const Real& zero,
                        Args... args ) const
    {
        view.segmentsReduction( offsets, first, last, fetch, reduction, keeper, zero, args... );
    }

    ViewType view;
};



      } // namespace Segments
   }  // namespace Algorithms
} // namespace TNL
