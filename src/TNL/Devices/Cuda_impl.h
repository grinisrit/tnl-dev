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

namespace TNL {
namespace Devices {   

__cuda_callable__ 
inline constexpr int Cuda::getMaxGridSize()
{
   // TODO: make it preprocessor macro constant defined in tnlConfig
   return 65535;
};

__cuda_callable__
inline constexpr int Cuda::getMaxBlockSize()
{
   // TODO: make it preprocessor macro constant defined in tnlConfig
   return 1024;
};

__cuda_callable__ 
inline constexpr int Cuda::getWarpSize()
{
   // TODO: make it preprocessor macro constant defined in tnlConfig
   return 32;
}

__cuda_callable__
inline constexpr int Cuda::getNumberOfSharedMemoryBanks()
{
   // TODO: make it preprocessor macro constant defined in tnlConfig
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
      checkCudaDevice;
      cudaFree( ( void* ) deviceObject );
      checkCudaDevice;
      return 0;
   }
   return deviceObject;
#else
   TNL_ASSERT( false, std::cerr << "CUDA support is missing." );
   return 0;
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
   checkCudaDevice;
   return aux;
#else
   TNL_ASSERT( false, std::cerr << "CUDA support is missing." );
   return 0;
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
   checkCudaDevice;
#else
   TNL_ASSERT( false, std::cerr << "CUDA support is missing." );
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
   checkCudaDevice;
#else
   TNL_ASSERT( false, std::cerr << "CUDA support is missing." );
#endif
}

#ifdef HAVE_CUDA
template< typename Index >
__device__ Index Cuda::getInterleaving( const Index index )
{
   return index + index / Cuda::getNumberOfSharedMemoryBanks();
}

template< typename Element, size_t Alignment >
__device__ Element* Cuda::getSharedMemory()
{
   extern __shared__ __align__ ( Alignment ) unsigned char __sdata[];
   return reinterpret_cast< Element* >( __sdata );
}
#endif /* HAVE_CUDA */

} // namespace Devices
} // namespace TNL
