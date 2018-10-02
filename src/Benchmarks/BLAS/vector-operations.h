/***************************************************************************
                          vector-operations.h  -  description
                             -------------------
    begin                : Dec 30, 2015
    copyright            : (C) 2015 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky

#pragma once

#include <stdlib.h> // srand48

#include "../Benchmarks.h"

#include <TNL/Containers/Vector.h>

#ifdef HAVE_CUDA
#include "cublasWrappers.h"
#endif

namespace TNL {
namespace Benchmarks {

template< typename Real = double,
          typename Index = int >
bool
benchmarkVectorOperations( Benchmark & benchmark,
                           const int & loops,
                           const long & size )
{
   typedef Containers::Vector< Real, Devices::Host, Index > HostVector;
   typedef Containers::Vector< Real, Devices::Cuda, Index > CudaVector;
   using namespace std;

   double datasetSize = ( double ) ( loops * size ) * sizeof( Real ) / oneGB;

   HostVector hostVector, hostVector2;
   CudaVector deviceVector, deviceVector2;
   hostVector.setSize( size );
   hostVector2.setSize( size );
#ifdef HAVE_CUDA
   deviceVector.setSize( size );
   deviceVector2.setSize( size );
#endif

   Real resultHost, resultDevice;

#ifdef HAVE_CUDA
   cublasHandle_t cublasHandle;
   cublasCreate( &cublasHandle );
#endif


   // reset functions
   // (Make sure to always use some in benchmarks, even if it's not necessary
   // to assure correct result - it helps to clear cache and avoid optimizations
   // of the benchmark loop.)
   auto reset1 = [&]() {
      hostVector.setValue( 1.0 );
#ifdef HAVE_CUDA
      deviceVector.setValue( 1.0 );
#endif
      // A relatively harmless call to keep the compiler from realizing we
      // don't actually do any useful work with the result of the reduciton.
      srand48(resultHost);
      resultHost = resultDevice = 0.0;
   };
   auto reset2 = [&]() {
      hostVector2.setValue( 1.0 );
#ifdef HAVE_CUDA
      deviceVector2.setValue( 1.0 );
#endif
   };
   auto reset12 = [&]() {
      reset1();
      reset2();
   };


   reset12();


   auto maxHost = [&]() {
      resultHost = hostVector.max();
   };
   auto maxCuda = [&]() {
      resultDevice = deviceVector.max();
   };
   benchmark.setOperation( "max", datasetSize );
   benchmark.time( reset1, "CPU", maxHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", maxCuda );
#endif


   auto minHost = [&]() {
      resultHost = hostVector.min();
   };
   auto minCuda = [&]() {
      resultDevice = deviceVector.min();
   };
   benchmark.setOperation( "min", datasetSize );
   benchmark.time( reset1, "CPU", minHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", minCuda );
#endif


   auto absMaxHost = [&]() {
      resultHost = hostVector.absMax();
   };
   auto absMaxCuda = [&]() {
      resultDevice = deviceVector.absMax();
   };
#ifdef HAVE_CUDA
   auto absMaxCublas = [&]() {
      int index = 0;
      cublasIgamax( cublasHandle, size,
                    deviceVector.getData(), 1,
                    &index );
      resultDevice = deviceVector.getElement( index );
   };
#endif
   benchmark.setOperation( "absMax", datasetSize );
   benchmark.time( reset1, "CPU", absMaxHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", absMaxCuda );
   benchmark.time( reset1, "cuBLAS", absMaxCublas );
#endif


   auto absMinHost = [&]() {
      resultHost = hostVector.absMin();
   };
   auto absMinCuda = [&]() {
      resultDevice = deviceVector.absMin();
   };
#ifdef HAVE_CUDA
   auto absMinCublas = [&]() {
      int index = 0;
      cublasIgamin( cublasHandle, size,
                    deviceVector.getData(), 1,
                    &index );
      resultDevice = deviceVector.getElement( index );
   };
#endif
   benchmark.setOperation( "absMin", datasetSize );
   benchmark.time( reset1, "CPU", absMinHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", absMinCuda );
   benchmark.time( reset1, "cuBLAS", absMinCublas );
#endif


   auto sumHost = [&]() {
      resultHost = hostVector.sum();
   };
   auto sumCuda = [&]() {
      resultDevice = deviceVector.sum();
   };
   benchmark.setOperation( "sum", datasetSize );
   benchmark.time( reset1, "CPU", sumHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", sumCuda );
#endif


   auto l1normHost = [&]() {
      resultHost = hostVector.lpNorm( 1.0 );
   };
   auto l1normCuda = [&]() {
      resultDevice = deviceVector.lpNorm( 1.0 );
   };
#ifdef HAVE_CUDA
   auto l1normCublas = [&]() {
      cublasGasum( cublasHandle, size,
                   deviceVector.getData(), 1,
                   &resultDevice );
   };
#endif
   benchmark.setOperation( "l1 norm", datasetSize );
   benchmark.time( reset1, "CPU", l1normHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", l1normCuda );
   benchmark.time( reset1, "cuBLAS", l1normCublas );
#endif


   auto l2normHost = [&]() {
      resultHost = hostVector.lpNorm( 2.0 );
   };
   auto l2normCuda = [&]() {
      resultDevice = deviceVector.lpNorm( 2.0 );
   };
#ifdef HAVE_CUDA
   auto l2normCublas = [&]() {
      cublasGnrm2( cublasHandle, size,
                   deviceVector.getData(), 1,
                   &resultDevice );
   };
#endif
   benchmark.setOperation( "l2 norm", datasetSize );
   benchmark.time( reset1, "CPU", l2normHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", l2normCuda );
   benchmark.time( reset1, "cuBLAS", l2normCublas );
#endif


   auto l3normHost = [&]() {
      resultHost = hostVector.lpNorm( 3.0 );
   };
   auto l3normCuda = [&]() {
      resultDevice = deviceVector.lpNorm( 3.0 );
   };
   benchmark.setOperation( "l3 norm", datasetSize );
   benchmark.time( reset1, "CPU", l3normHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", l3normCuda );
#endif


   auto scalarProductHost = [&]() {
      resultHost = hostVector.scalarProduct( hostVector2 );
   };
   auto scalarProductCuda = [&]() {
      resultDevice = deviceVector.scalarProduct( deviceVector2 );
   };
#ifdef HAVE_CUDA
   auto scalarProductCublas = [&]() {
      cublasGdot( cublasHandle, size,
                  deviceVector.getData(), 1,
                  deviceVector2.getData(), 1,
                  &resultDevice );
   };
#endif
   benchmark.setOperation( "scalar product", 2 * datasetSize );
   benchmark.time( reset1, "CPU", scalarProductHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", scalarProductCuda );
   benchmark.time( reset1, "cuBLAS", scalarProductCublas );
#endif

   /*
   std::cout << "Benchmarking prefix-sum:" << std::endl;
   timer.reset();
   timer.start();
   hostVector.computePrefixSum();
   timer.stop();
   timeHost = timer.getTime();
   bandwidth = 2 * datasetSize / loops / timer.getTime();
   std::cout << "  CPU: bandwidth: " << bandwidth << " GB/sec, time: " << timer.getTime() << " sec." << std::endl;

   timer.reset();
   timer.start();
   deviceVector.computePrefixSum();
   timer.stop();
   timeDevice = timer.getTime();
   bandwidth = 2 * datasetSize / loops / timer.getTime();
   std::cout << "  GPU: bandwidth: " << bandwidth << " GB/sec, time: " << timer.getTime() << " sec." << std::endl;
   std::cout << "  CPU/GPU speedup: " << timeHost / timeDevice << std::endl;

   HostVector auxHostVector;
   auxHostVector.setLike( deviceVector );
   auxHostVector = deviceVector;
   for( int i = 0; i < size; i++ )
      if( hostVector.getElement( i ) != auxHostVector.getElement( i ) )
      {
         std::cerr << "Error in prefix sum at position " << i << ":  " << hostVector.getElement( i ) << " != " << auxHostVector.getElement( i ) << std::endl;
      }
   */


   auto multiplyHost = [&]() {
      hostVector *= 0.5;
   };
   auto multiplyCuda = [&]() {
      deviceVector *= 0.5;
   };
#ifdef HAVE_CUDA
   auto multiplyCublas = [&]() {
      const Real alpha = 0.5;
      cublasGscal( cublasHandle, size,
                   &alpha,
                   deviceVector.getData(), 1 );
   };
#endif
   benchmark.setOperation( "scalar multiplication", 2 * datasetSize );
   benchmark.time( reset1, "CPU", multiplyHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", multiplyCuda );
   benchmark.time( reset1, "cuBLAS", multiplyCublas );
#endif


   auto addVectorHost = [&]() {
      hostVector.addVector( hostVector2 );
   };
   auto addVectorCuda = [&]() {
      deviceVector.addVector( deviceVector2 );
   };
#ifdef HAVE_CUDA
   auto addVectorCublas = [&]() {
      const Real alpha = 1.0;
      cublasGaxpy( cublasHandle, size,
                   &alpha,
                   deviceVector2.getData(), 1,
                   deviceVector.getData(), 1 );
   };
#endif
   benchmark.setOperation( "vector addition", 3 * datasetSize );
   benchmark.time( reset1, "CPU", addVectorHost );
#ifdef HAVE_CUDA
   benchmark.time( reset1, "GPU", addVectorCuda );
   benchmark.time( reset1, "cuBLAS", addVectorCublas );
#endif


#ifdef HAVE_CUDA
   cublasDestroy( cublasHandle );
#endif

   return true;
}

} // namespace Benchmarks
} // namespace TNL