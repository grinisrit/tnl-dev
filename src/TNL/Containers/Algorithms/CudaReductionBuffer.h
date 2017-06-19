/***************************************************************************
                          CudaReductionBuffer.h  -  description
                             -------------------
    begin                : June 17, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky

#pragma once

#include <stdlib.h>

#include <TNL/Devices/Cuda.h>
#include <TNL/Exceptions/CudaBadAlloc.h>
#include <TNL/Exceptions/CudaSupportMissing.h>

namespace TNL {
namespace Containers {
namespace Algorithms {

class CudaReductionBuffer
{
   public:
      inline static CudaReductionBuffer& getInstance()
      {
         static CudaReductionBuffer instance;
         return instance;
      }

      inline void setSize( size_t size )
      {
#ifdef HAVE_CUDA
         if( size > this->size )
         {
            this->free();
            if( cudaMalloc( ( void** ) &this->data, size ) != cudaSuccess ) {
               this->data = 0;
               throw Exceptions::CudaBadAlloc();
            }
            this->size = size;
         }
#else
         throw Exceptions::CudaSupportMissing();
#endif
      }

      template< typename Type >
      Type* getData()
      {
         return ( Type* ) this->data;
      }

   private:
      // stop the compiler generating methods of copy the object
      CudaReductionBuffer( CudaReductionBuffer const& copy );            // Not Implemented
      CudaReductionBuffer& operator=( CudaReductionBuffer const& copy ); // Not Implemented

      // private constructor of the singleton
      inline CudaReductionBuffer( size_t size = 0 )
      {
#ifdef HAVE_CUDA
         setSize( size );
         atexit( CudaReductionBuffer::free_atexit );
#endif
      }

      inline static void free_atexit( void )
      {
         CudaReductionBuffer::getInstance().free();
      }

   protected:
      inline void free( void )
      {
#ifdef HAVE_CUDA
         if( data )
         {
            cudaFree( data );
            data = nullptr;
            checkCudaDevice;
         }
#endif
      }

      void* data = nullptr;

      size_t size = 0;
};

} // namespace Algorithms
} // namespace Containers
} // namespace TNL
