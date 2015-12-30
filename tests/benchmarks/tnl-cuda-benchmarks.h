/***************************************************************************
                          tnl-benchmarks.h  -  description
                             -------------------
    begin                : Jan 27, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TNLCUDABENCHMARKS_H_
#define TNLCUDBENCHMARKS_H_

#include <tnlConfig.h>
#include <core/vectors/tnlVector.h>
#include <core/tnlList.h>
#include <matrices/tnlSlicedEllpackMatrix.h>
#include <matrices/tnlEllpackMatrix.h>
#include <matrices/tnlCSRMatrix.h>

#include "benchmarks.h"

using namespace tnl::benchmarks;

#ifdef HAVE_CUBLAS
//#include <cublas.h>
#endif    

// silly alias to match the number of template parameters with other formats
template< typename Real, typename Device, typename Index >
using SlicedEllpackMatrix = tnlSlicedEllpackMatrix< Real, Device, Index >;

const double oneGB = 1024.0 * 1024.0 * 1024.0;


// TODO:
// check operations with the timer:
//   - reset() clears the timer and starts it again
//   - getTime() stops the timer and starts it again !!!
//   - data members are not zero-initialized - reset has to be called manually, but it immediately starts the timer
// FIXME: scalarProduct is not const method


template< typename Matrix >
int setHostTestMatrix( Matrix& matrix,
                       const int elementsPerRow )
{
   const int size = matrix.getRows();
   int elements( 0 );
   for( int row = 0; row < size; row++ )
   {
      if( row % 100 == 0 )
         cout << "Filling row " << row << "/" << size << "     \r" << flush;
      int col = row - elementsPerRow / 2;
      for( int element = 0; element < elementsPerRow; element++ )
      {
         if( col + element >= 0 &&
             col + element < size )
         {
            matrix.setElement( row, col + element, element + 1 );
            elements++;
         }
      }      
   }
   cout << endl;
   return elements;
}

template< typename Matrix >
__global__ void setCudaTestMatrixKernel( Matrix* matrix,
                                         const int elementsPerRow,
                                         const int gridIdx )
{
   const int rowIdx = ( gridIdx * tnlCuda::getMaxGridSize() + blockIdx.x ) * blockDim.x + threadIdx.x;
   if( rowIdx >= matrix->getRows() )
      return;
   int col = rowIdx - elementsPerRow / 2;   
   for( int element = 0; element < elementsPerRow; element++ )
   {
      if( col + element >= 0 &&
          col + element < matrix->getColumns() )
         matrix->setElementFast( rowIdx, col + element, element + 1 );
   }      
}

template< typename Matrix >
void setCudaTestMatrix( Matrix& matrix,
                        const int elementsPerRow )
{
   typedef typename Matrix::IndexType IndexType;
   typedef typename Matrix::RealType RealType;
   Matrix* kernel_matrix = tnlCuda::passToDevice( matrix );
   dim3 cudaBlockSize( 256 ), cudaGridSize( tnlCuda::getMaxGridSize() );
   const IndexType cudaBlocks = roundUpDivision( matrix.getRows(), cudaBlockSize.x );
   const IndexType cudaGrids = roundUpDivision( cudaBlocks, tnlCuda::getMaxGridSize() );
   for( IndexType gridIdx = 0; gridIdx < cudaGrids; gridIdx++ )
   {
      if( gridIdx == cudaGrids - 1 )
         cudaGridSize.x = cudaBlocks % tnlCuda::getMaxGridSize();
      setCudaTestMatrixKernel< Matrix >
       <<< cudaGridSize, cudaBlockSize >>>
       ( kernel_matrix, elementsPerRow, gridIdx );
      checkCudaDevice;
   }
   tnlCuda::freeFromDevice( kernel_matrix );
}


template< typename Real,
          template< typename, typename, typename > class Matrix,
          template< typename, typename, typename > class Vector = tnlVector >
bool
benchmarkSpMV( const int & loops,
               const int & size,
               const int elementsPerRow = 5 )
{
   typedef Matrix< Real, tnlHost, int > HostMatrix;
   typedef Matrix< Real, tnlCuda, int > DeviceMatrix;
   typedef tnlVector< Real, tnlHost, int > HostVector;
   typedef tnlVector< Real, tnlCuda, int > CudaVector;

   HostMatrix hostMatrix;
   DeviceMatrix deviceMatrix;
   tnlVector< int, tnlHost, int > hostRowLengths;
   tnlVector< int, tnlCuda, int > deviceRowLengths;
   HostVector hostVector, hostVector2;
   CudaVector deviceVector, deviceVector2;

   if( ! hostRowLengths.setSize( size ) ||
       ! deviceRowLengths.setSize( size ) ||
       ! hostMatrix.setDimensions( size, size ) ||
       ! deviceMatrix.setDimensions( size, size ) ||
       ! hostVector.setSize( size ) ||
       ! hostVector2.setSize( size ) ||
       ! deviceVector.setSize( size ) ||
       ! deviceVector2.setSize( size ) )
   {
      cerr << "Unable to allocate all matrices and vectors for the SpMV benchmark." << endl;
      return false;
   }

   hostRowLengths.setValue( elementsPerRow );
   deviceRowLengths.setValue( elementsPerRow );

   if( ! hostMatrix.setCompressedRowsLengths( hostRowLengths ) )
   {
      cerr << "Unable to allocate host matrix elements." << endl;
      return false;
   }
   if( ! deviceMatrix.setCompressedRowsLengths( deviceRowLengths ) )
   {
      cerr << "Unable to allocate device matrix elements." << endl;
      return false;
   }

   tnlList< tnlString > parsedType;
   parseObjectType( HostMatrix::getType(), parsedType );
   cout << "Benchmarking SpMV (matrix type: " << parsedType[ 0 ] << ", rows: " << size << ", elements per row: " << elementsPerRow << "):" << endl;

   const int elements = setHostTestMatrix< HostMatrix >( hostMatrix, elementsPerRow );
   setCudaTestMatrix< DeviceMatrix >( deviceMatrix, elementsPerRow );
   const double datasetSize = loops * elements * ( 2 * sizeof( Real ) + sizeof( int ) ) / oneGB;
   hostVector.setValue( 1.0 );
   deviceVector.setValue( 1.0 );

   // check and reset functions
   auto check = [&]() {
      return hostVector2 == deviceVector2;
   };
   auto reset = [&]() {
      hostVector2.setValue( 0.0 );
      deviceVector2.setValue( 0.0 );
   };

   // compute functions
   auto spmvHost = [&]() {
      hostMatrix.vectorProduct( hostVector, hostVector2 );
   };
   auto spmvCuda = [&]() {
      deviceMatrix.vectorProduct( deviceVector, deviceVector2 );
      // TODO: tnlCSRMatrix does not synchronize
      cudaThreadSynchronize();
   };

   benchmarkCuda( loops, datasetSize, spmvHost, spmvCuda, check, reset );

   return true;
}

int main( int argc, char* argv[] )
{
#ifdef HAVE_CUDA
   
   typedef double Real;
   typedef tnlVector< Real, tnlHost > HostVector;
   typedef tnlVector< Real, tnlCuda > CudaVector;

   
   /****
    * The first argument of this program is the size od data set to be reduced.
    * If no argument is given we use hardcoded default value.
    */
   int size = 1 << 22;
   if( argc > 1 )
      size = atoi( argv[ 1 ] );
   int loops = 10;
   if( argc > 2 )
      loops = atoi( argv[ 2 ] );
   int elementsPerRow = 5;
   if( argc > 3 )
      elementsPerRow = atoi( argv[ 3 ] );
   
   
   double datasetSize = ( double ) ( loops * size ) * sizeof( Real ) / oneGB;
   
   HostVector hostVector, hostVector2;
   CudaVector deviceVector, deviceVector2;
   hostVector.setSize( size );
   if( ! deviceVector.setSize( size ) )
      return EXIT_FAILURE;
   hostVector2.setLike( hostVector );
   if( ! deviceVector2.setLike( deviceVector ) )
      return EXIT_FAILURE;

   Real resultHost, resultDevice;


   // check functions
   auto compare1 = [&]() {
      return hostVector == deviceVector;
   };
   auto compare2 = [&]() {
      return hostVector2 == deviceVector2;
   };
   auto compare12 = [&]() {
      return compare1() && compare2();
   };
   auto compareScalars = [&]() {
      return resultHost == resultDevice;
   };

   // reset functions
   auto reset1 = [&]() {
      hostVector.setValue( 1.0 );
      deviceVector.setValue( 1.0 );
   };
   auto reset2 = [&]() {
      hostVector2.setValue( 1.0 );
      deviceVector2.setValue( 1.0 );
   };
   auto reset12 = [&]() {
      reset1();
      reset2();
   };


   reset12();

   cout << "Benchmarking CPU-GPU memory transfer:" << endl;
   auto copyAssign = [&]() {
      deviceVector = hostVector;
   };
   cout << "  ";
   benchmarkSingle( loops, datasetSize, copyAssign, compare1, reset1 );
    

   cout << "Benchmarking vector addition:" << endl;
   auto addVectorHost = [&]() {
      hostVector.addVector( hostVector2 );
   };
   auto addVectorCuda = [&]() {
      deviceVector.addVector( deviceVector2 );
      // TODO: synchronization should be part of addVector
      cudaThreadSynchronize();
   };
   benchmarkCuda( loops, 3 * datasetSize, addVectorHost, addVectorCuda, compare1, reset1 );


   cout << "Benchmarking scalar product:" << endl;
   auto scalarProductHost = [&]() {
      resultHost = hostVector.scalarProduct( hostVector2 );
   };
   auto scalarProductCuda = [&]() {
      resultDevice = deviceVector.scalarProduct( deviceVector2 );
   };
   benchmarkCuda( loops, 2 * datasetSize, scalarProductHost, scalarProductCuda, compareScalars, voidFunc );

/* TODO
#ifdef HAVE_CUBLAS
   cout << "Benchmarking scalar product on GPU with Cublas: " << endl;
   cublasHandle_t handle;
   cublasCreate( &handle );
   timer.reset();
   timer.start();   
   for( int i = 0; i < loops; i++ )
      cublasDdot( handle,
                  size,
                  deviceVector.getData(), 1,
                  deviceVector.getData(), 1,
                  &resultDevice );
   cudaThreadSynchronize();
   timer.stop();
   bandwidth = 2 * datasetSize / timer.getTime();
   cout << "bandwidth: " << bandwidth << " GB/sec, time: " << timer.getTime() << " sec." << endl;
#endif    
*/

   cout << "Benchmarking L2 norm: " << endl;
   auto l2normHost = [&]() {
      resultHost = hostVector.lpNorm( 2.0 );
   };
   auto l2normCuda = [&]() {
      resultDevice = deviceVector.lpNorm( 2.0 );
   };
   benchmarkCuda( loops, datasetSize, l2normHost, l2normCuda, compareScalars, voidFunc );


   /*
   cout << "Benchmarking prefix-sum:" << endl;
   timer.reset();
   timer.start();
   hostVector.computePrefixSum();
   timer.stop();
   timeHost = timer.getTime();
   bandwidth = 2 * datasetSize / loops / timer.getTime();
   cout << "  CPU: bandwidth: " << bandwidth << " GB/sec, time: " << timer.getTime() << " sec." << endl;
   
   timer.reset();
   timer.start();
   deviceVector.computePrefixSum();
   timer.stop();
   timeDevice = timer.getTime();
   bandwidth = 2 * datasetSize / loops / timer.getTime();
   cout << "  GPU: bandwidth: " << bandwidth << " GB/sec, time: " << timer.getTime() << " sec." << endl;
   cout << "  CPU/GPU speedup: " << timeHost / timeDevice << endl;

   HostVector auxHostVector;
   auxHostVector.setLike( deviceVector );
   auxHostVector = deviceVector;
   for( int i = 0; i < size; i++ )
      if( hostVector.getElement( i ) != auxHostVector.getElement( i ) )
      {
         cerr << "Error in prefix sum at position " << i << ":  " << hostVector.getElement( i ) << " != " << auxHostVector.getElement( i ) << endl;
      }
*/

   benchmarkSpMV< Real, tnlEllpackMatrix >( loops, size, elementsPerRow );
   benchmarkSpMV< Real, SlicedEllpackMatrix >( loops, size, elementsPerRow );
   benchmarkSpMV< Real, tnlCSRMatrix >( loops, size, elementsPerRow );
   
   return EXIT_SUCCESS;
#else
   tnlCudaSupportMissingMessage;
   return EXIT_FAILURE;
#endif
}

#endif /* TNLCUDABENCHMARKS_H_ */
