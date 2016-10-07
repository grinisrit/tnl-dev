#pragma once

//#define CUDA_REDUCTION_PROFILING

#ifdef HAVE_CUDA
#include <cuda.h>
#endif
#include <TNL/Assert.h>
#include <TNL/Containers/Algorithms/reduction-operations.h>
#include <TNL/Containers/ArrayOperations.h>
#include <TNL/Math.h>
#include <TNL/Containers/Algorithms/CudaReductionBuffer.h>
#include <TNL/Containers/Algorithms/CudaMultireductionKernel.h>
#include <TNL/Devices/CudaDeviceInfo.h>

#ifdef CUDA_REDUCTION_PROFILING
#include <TNL/Timer.h>
#include <iostream>
#endif

namespace TNL {
namespace Containers {
namespace Algorithms {

/****
 * Arrays smaller than the following constant are reduced on CPU.
 */
//static constexpr int Multireduction_minGpuDataSize = 16384;//65536; //16384;//1024;//256;
// TODO: benchmarks with different values
static constexpr int Multireduction_minGpuDataSize = 256;//65536; //16384;//1024;//256;

/*
 * Parameters:
 *    operation: the operation used for reduction
 *    n: number of datasets to be reduced
 *    size: the size of each dataset
 *    deviceInput1: input array of size = n * ldInput1
 *    ldInput1: leading dimension of the deviceInput1 array
 *    deviceInput2: either nullptr or input array of size = size
 *    hostResult: output array of size = n
 */
template< typename Operation >
bool multireductionOnCudaDevice( Operation& operation,
                                 int n,
                                 const typename Operation::IndexType size,
                                 const typename Operation::RealType* deviceInput1,
                                 const typename Operation::IndexType ldInput1,
                                 const typename Operation::RealType* deviceInput2,
                                 typename Operation::ResultType* hostResult )
{
#ifdef HAVE_CUDA
   Assert( n > 0, );

   typedef typename Operation::IndexType IndexType;
   typedef typename Operation::RealType RealType;
   typedef typename Operation::ResultType ResultType;
   typedef typename Operation::LaterReductionOperation LaterReductionOperation;

   /***
    * First check if the input array(s) is/are large enough for the multireduction on GPU.
    * Otherwise copy it/them to host and multireduce on CPU.
    */
   if( n * ldInput1 < Multireduction_minGpuDataSize ) {
      RealType hostArray1[ Multireduction_minGpuDataSize ];
      if( ! ArrayOperations< Devices::Host, Devices::Cuda >::copyMemory< RealType, RealType, IndexType >( hostArray1, deviceInput1, n * ldInput1 ) )
         return false;
      if( deviceInput2 ) {
         RealType hostArray2[ Multireduction_minGpuDataSize ];
         if( ! ArrayOperations< Devices::Host, Devices::Cuda >::copyMemory< RealType, RealType, IndexType >( hostArray2, deviceInput2, n * size ) )
            return false;
         return multireductionOnHostDevice( operation, n, size, hostArray1, ldInput1, hostArray2, hostResult );
      }
      else {
         return multireductionOnHostDevice( operation, n, size, hostArray1, ldInput1, nullptr, hostResult );
      }
   }

   #ifdef CUDA_REDUCTION_PROFILING
      Timer timer;
      timer.reset();
      timer.start();
   #endif

   /****
    * Reduce the data on the CUDA device.
    */
   ResultType* deviceAux1 = nullptr;
   const IndexType reducedSize = CudaMultireductionKernelLauncher( operation,
                                                                   n,
                                                                   size,
                                                                   deviceInput1,
                                                                   ldInput1,
                                                                   deviceInput2,
                                                                   deviceAux1 );
   #ifdef CUDA_REDUCTION_PROFILING
      timer.stop();
      cout << "   Multireduction of " << n << " datasets on GPU to size " << reducedSize << " took " << timer.getRealTime() << " sec. " << endl;
      timer.reset();
      timer.start();
   #endif

   /***
    * Transfer the reduced data from device to host.
    */
   ResultType resultArray[ n * reducedSize ];
   if( ! ArrayOperations< Devices::Host, Devices::Cuda >::copyMemory< ResultType, ResultType, IndexType >( resultArray, deviceAux1, n * reducedSize ) )
      return false;

   #ifdef CUDA_REDUCTION_PROFILING
      timer.stop();
      cout << "   Transferring data to CPU took " << timer.getRealTime() << " sec. " << endl;
      timer.reset();
      timer.start();
   #endif

//   cout << "resultArray = [";
//   for( int i = 0; i < n * reducedSize; i++ ) {
//      cout << resultArray[ i ];
//      if( i < n * reducedSize - 1 )
//         cout << ", ";
//   }
//   cout << "]" << endl;

   /***
    * Reduce the data on the host system.
    */
   LaterReductionOperation laterReductionOperation;
   multireductionOnHostDevice( laterReductionOperation, n, reducedSize, resultArray, reducedSize, (RealType*) nullptr, hostResult );

   #ifdef CUDA_REDUCTION_PROFILING
      timer.stop();
      cout << "   Multireduction of small data set on CPU took " << timer.getRealTime() << " sec. " << endl;
   #endif

   return checkCudaDevice;
#else
   CudaSupportMissingMessage;
   return false;
#endif
};

/*
 * Parameters:
 *    operation: the operation used for reduction
 *    n: number of datasets to be reduced
 *    size: the size of each dataset
 *    input1: input array of size = n * ldInput1
 *    ldInput1: leading dimension of the input1 array
 *    input2: either nullptr or input array of size = size
 *    hostResult: output array of size = n
 */
template< typename Operation >
bool multireductionOnHostDevice( Operation& operation,
                                 int n,
                                 const typename Operation::IndexType size,
                                 const typename Operation::RealType* input1,
                                 const typename Operation::IndexType ldInput1,
                                 const typename Operation::RealType* input2,
                                 typename Operation::ResultType* result )
{
   typedef typename Operation::IndexType IndexType;
   typedef typename Operation::RealType RealType;

   for( int k = 0; k < n; k++ ) {
      result[ k ] = operation.initialValue();
      const RealType* _input1 = input1 + k * ldInput1;
      for( IndexType i = 0; i < size; i++ )
         result[ k ] = operation.reduceOnHost( i, result[ k ], _input1, input2 );
   }

   return true;
}

} // namespace Algorithms
} // namespace Containers
} // namespace TNL