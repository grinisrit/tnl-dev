/***************************************************************************
                          tnl-benchmarks.h  -  description
                             -------------------
    begin                : Jan 27, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLBENCHMARKS_H_
#define TNLBENCHMARKS_H_

#include <TNL/core/mfuncs.h>

template< class T >
bool transferBenchmark( const int size,
                        double& host_to_host_band_width,
                        double& host_to_device_band_width,
                        double& device_to_host_band_width,
                        double& device_to_device_band_width )
{

  Vector< T > host_vector( "transferBenchmark:host-vector", size );
  Vector< T > host_vector2( "transferBenchmark:host-vector-2", size );
  VectorCUDA< T > device_vector( "transferBenchmark:device-vector", size );
  VectorCUDA< T > device_vector2( "transferBenchmark:device-vector-2", size );

   for( int i = 0; i < size; i ++ )
      host_vector[ i ] = i + 1;

   const long int cycles = 100;
   long int bytes = cycles * size * sizeof( int );
   long int mega_byte = 1 << 20;

   TimerRT timer;
   timer. Reset();
   for( int i = 0; i < cycles; i ++ )
      if( ! host_vector2. copyFrom( host_vector ) )
         return false;
   double time = timer. getTime();
   double giga_byte = ( double ) ( 1 << 30 );
   host_to_host_band_width = bytes / giga_byte / time;

  std::cout << "Transfering " << bytes / mega_byte << " MB from HOST to HOST took " << time << " seconds. Bandwidth is " << host_to_host_band_width << " GB/s." << std::endl;

   timer. Reset();
   for( int i = 0; i < cycles; i ++ )
      if( ! device_vector. copyFrom( host_vector ) )
         return false;
   time = timer. getTime();
   host_to_device_band_width = bytes / giga_byte / time;

  std::cout << "Transfering " << bytes / mega_byte << " MB from HOST to DEVICE took " << time << " seconds. Bandwidth is " << host_to_device_band_width << " GB/s." << std::endl;

   timer. Reset();
   for( int i = 0; i < cycles; i ++ )
      if( ! host_vector2. copyFrom( device_vector ) )
         return false;
   time = timer. getTime();
   device_to_host_band_width = bytes / giga_byte / time;

  std::cout << "Transfering " << bytes / mega_byte << " MB from DEVICE to HOST took " << time << " seconds. Bandwidth is " << device_to_host_band_width << " GB/s." << std::endl;

   timer. Reset();
   for( int i = 0; i < cycles; i ++ )
      if( ! device_vector2. copyFrom( device_vector ) )
         return false;

   time = timer. getTime();

   // Since we read and write tha data back we process twice as many bytes.
   bytes *= 2;
   device_to_device_band_width = bytes / giga_byte / time;

  std::cout << "Transfering " << bytes / mega_byte << " MB from DEVICE to DEVICE took " << time << " seconds. Bandwidth is " << device_to_device_band_width << " GB/s." << std::endl;
}

template< class T >
void tnlCPUReductionSum( const Vector< T >& host_vector,
                         T& sum )
{
   const T* data = host_vector. Data();
   const int size = host_vector. GetSize();
   sum = 0.0;
   for( int i = 0; i < size; i ++ )
      sum += data[ i ];
};

template< class T >
void tnlCPUReductionMin( const Vector< T >& host_vector,
                         T& min )
{
   const T* data = host_vector. Data();
   const int size = host_vector. GetSize();
   //Assert( data );
   min = data[ 0 ];
   for( int i = 1; i < size; i ++ )
      min = :: min( min,  data[ i ] );
};

template< class T >
void tnlCPUReductionMax( const Vector< T >& host_vector,
                         T& max )
{
   const T* data = host_vector. Data();
   const int size = host_vector. GetSize();
   //Assert( data );
   max = data[ 0 ];
   for( int i = 1; i < size; i ++ )
      max = :: max( max,  data[ i ] );
};

template< class T >
void reductionBenchmark( const int size,
                         const int algorithm )
{
   Vector< T > host_vector( "reductionBenchmark:host-vector", size );
   VectorCUDA< T > device_vector( "reductionBenchmark:device-vector", size );
   VectorCUDA< T > device_aux( "reductionBenchmark:device-aux", size / 2 );

   for( int i = 0; i < size; i ++ )
      host_vector[ i ] = i + 1;

   device_vector. copyFrom( host_vector );

   T sum, min, max;
   const long int reducing_cycles( 10 );

   TimerRT timer;
   timer. Reset();
   for( int i = 0; i < reducing_cycles; i ++ )
   {
      switch( algorithm )
      {
         case 0:  // reduction on CPU
            tnlCPUReductionSum( host_vector, sum );
            tnlCPUReductionMin( host_vector, sum );
            tnlCPUReductionMax( host_vector, sum );
         case 1:
            Devices::CudaSimpleReduction1Sum( size,
                                        device_vector. Data(),
                                        sum,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction1Min( size,
                                        device_vector. Data(),
                                        min,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction1Max( size,
                                        device_vector. Data(),
                                        max,
                                        device_aux. Data() );
            break;
         case 2:
            Devices::CudaSimpleReduction2Sum( size,
                                        device_vector. Data(),
                                        sum,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction2Min( size,
                                        device_vector. Data(),
                                        min,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction2Max( size,
                                        device_vector. Data(),
                                        max,
                                        device_aux. Data() );
            break;
         case 3:
            Devices::CudaSimpleReduction3Sum( size,
                                        device_vector. Data(),
                                        sum,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction3Min( size,
                                        device_vector. Data(),
                                        min,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction3Max( size,
                                        device_vector. Data(),
                                        max,
                                        device_aux. Data() );
            break;
         case 4:
            Devices::CudaSimpleReduction4Sum( size,
                                        device_vector. Data(),
                                        sum,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction4Min( size,
                                        device_vector. Data(),
                                        min,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction4Max( size,
                                        device_vector. Data(),
                                        max,
                                        device_aux. Data() );
            break;
         case 5:
            Devices::CudaSimpleReduction5Sum( size,
                                        device_vector. Data(),
                                        sum,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction5Min( size,
                                        device_vector. Data(),
                                        min,
                                        device_aux. Data() );
            Devices::CudaSimpleReduction5Max( size,
                                        device_vector. Data(),
                                        max,
                                        device_aux. Data() );
            break;
         default:
            tnlCudaReductionSum( size,
                                 device_vector. Data(),
                                 sum,
                                 device_aux. Data() );
            tnlCudaReductionMin( size,
                                 device_vector. Data(),
                                 min,
                                 device_aux. Data() );
            tnlCudaReductionMax( size,
                                 device_vector. Data(),
                                 max,
                                 device_aux. Data() );

      }
   }
   const double time = timer. getTime();
   double giga_byte = ( double ) ( 1 << 30 );
   long int mega_byte = 1 << 20;
   long int bytes_reduced = size * sizeof( T ) * reducing_cycles * 3;
   const double reduction_band_width = bytes_reduced / giga_byte / time;

  std::cout << "Reducing " << bytes_reduced / mega_byte
        << " MB on DEVICE using algorithm " << algorithm
        << " took " << time
        << " seconds. Bandwidth is " << reduction_band_width
        << " GB/s." << std::endl;
}

#endif /* TNLBENCHMARKS_H_ */