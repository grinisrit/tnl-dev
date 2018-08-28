/***************************************************************************
                          Cuda_impl.h  -  description
                             -------------------
    begin                : Jan 21, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Devices/Cuda.h>
#include <TNL/Exceptions/CudaBadAlloc.h>
#include <TNL/Exceptions/CudaSupportMissing.h>
#include <TNL/CudaSharedMemory.h>

namespace TNL {
namespace Devices {

__cuda_callable__
inline constexpr int Cuda::getMaxGridSize()
{
   return 65535;
}

__cuda_callable__
inline constexpr int Cuda::getMaxBlockSize()
{
   return 1024;
}

__cuda_callable__
inline constexpr int Cuda::getWarpSize()
{
   return 32;
}

__cuda_callable__
inline constexpr int Cuda::getNumberOfSharedMemoryBanks()
{
   return 32;
}

inline constexpr int Cuda::getGPUTransferBufferSize()
{
   return 1 << 20;
}

#ifdef HAVE_CUDA
__device__ inline int Cuda::getGlobalThreadIdx( const int gridIdx, const int gridSize )
{
   return ( gridIdx * gridSize + blockIdx.x ) * blockDim.x + threadIdx.x;
}

__device__ inline int Cuda::getGlobalThreadIdx_x( const dim3& gridIdx )
{
   return ( gridIdx.x * getMaxGridSize() + blockIdx.x ) * blockDim.x + threadIdx.x;
}

__device__ inline int Cuda::getGlobalThreadIdx_y( const dim3& gridIdx )
{
   return ( gridIdx.y * getMaxGridSize() + blockIdx.y ) * blockDim.y + threadIdx.y;
}

__device__ inline int Cuda::getGlobalThreadIdx_z( const dim3& gridIdx )
{
   return ( gridIdx.z * getMaxGridSize() + blockIdx.z ) * blockDim.z + threadIdx.z;
}
#endif


template< typename ObjectType >
ObjectType* Cuda::passToDevice( const ObjectType& object )
{
#ifdef HAVE_CUDA
   ObjectType* deviceObject;
   if( cudaMalloc( ( void** ) &deviceObject,
                   ( size_t ) sizeof( ObjectType ) ) != cudaSuccess )
      throw Exceptions::CudaBadAlloc();
   if( cudaMemcpy( ( void* ) deviceObject,
                   ( void* ) &object,
                   sizeof( ObjectType ),
                   cudaMemcpyHostToDevice ) != cudaSuccess )
   {
      TNL_CHECK_CUDA_DEVICE;
      cudaFree( ( void* ) deviceObject );
      TNL_CHECK_CUDA_DEVICE;
      return 0;
   }
   return deviceObject;
#else
   throw Exceptions::CudaSupportMissing();
#endif
}

template< typename ObjectType >
ObjectType Cuda::passFromDevice( const ObjectType* object )
{
#ifdef HAVE_CUDA
   ObjectType aux;
   cudaMemcpy( ( void* ) aux,
               ( void* ) &object,
               sizeof( ObjectType ),
               cudaMemcpyDeviceToHost );
   TNL_CHECK_CUDA_DEVICE;
   return aux;
#else
   throw Exceptions::CudaSupportMissing();
#endif
}

template< typename ObjectType >
void Cuda::passFromDevice( const ObjectType* deviceObject,
                           ObjectType& hostObject )
{
#ifdef HAVE_CUDA
   cudaMemcpy( ( void* ) &hostObject,
               ( void* ) deviceObject,
               sizeof( ObjectType ),
               cudaMemcpyDeviceToHost );
   TNL_CHECK_CUDA_DEVICE;
#else
   throw Exceptions::CudaSupportMissing();
#endif
}

template< typename ObjectType >
void Cuda::print( const ObjectType* deviceObject, std::ostream& str )
{
#ifdef HAVE_CUDA
   ObjectType hostObject;
   passFromDevice( deviceObject, hostObject );
   str << hostObject;
#endif
}


template< typename ObjectType >
void Cuda::freeFromDevice( ObjectType* deviceObject )
{
#ifdef HAVE_CUDA
   cudaFree( ( void* ) deviceObject );
   TNL_CHECK_CUDA_DEVICE;
#else
   throw Exceptions::CudaSupportMissing();
#endif
}

#ifdef HAVE_CUDA
template< typename Index >
__device__ Index Cuda::getInterleaving( const Index index )
{
   return index + index / Cuda::getNumberOfSharedMemoryBanks();
}

template< typename Element >
__device__ Element* Cuda::getSharedMemory()
{
   return CudaSharedMemory< Element >();
}

// TODO: This is only for Kepler and older architectures. Fix it.
__device__ 
inline double atomicAdd(double* address, double val)
{
    unsigned long long int* address_as_ull = ( unsigned long long int* ) address;
    unsigned long long int old = *address_as_ull, assumed;
    do 
    {
        assumed = old;
        old = atomicCAS(address_as_ull, assumed, __double_as_longlong(val + __longlong_as_double( assumed ) ) );
    } 
    while( assumed != old );
    return __longlong_as_double( old );
}
#endif /* HAVE_CUDA */

} // namespace Devices
} // namespace TNL
