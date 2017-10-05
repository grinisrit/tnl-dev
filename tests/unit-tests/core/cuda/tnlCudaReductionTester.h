/***************************************************************************
                          CudaReductionTester.h  -  description
                             -------------------
    begin                : Mar 22, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */


#ifndef CudaReductionTESTER_H_
#define CudaReductionTESTER_H_

#include <TNL/tnlConfig.h>

#ifdef HAVE_CPPUNIT
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/Message.h>
#include <TNL/Devices/Cuda.h>
#include <TNL/Containers/Algorithms/cuda-reduction.h>

using namespace TNL;

class CudaReductionTester : public CppUnit :: TestCase
{
   public:
   CudaReductionTester(){};

   virtual
   ~CudaReductionTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "CudaReductionTester" );
      CppUnit :: TestResult result;

      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "shortConstantSequenceTest",
                                &CudaReductionTester :: shortConstantSequenceTest< double > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "longConstantSequenceTest",
                                &CudaReductionTester :: longConstantSequenceTest< double > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "linearSequenceTest",
                                &CudaReductionTester :: linearSequenceTest< double > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "shortLogicalOperationsTest",
                                &CudaReductionTester :: shortLogicalOperationsTest< int > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "longLogicalOperationsTest",
                                &CudaReductionTester :: longLogicalOperationsTest< int > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "shortComparisonTest",
                                &CudaReductionTester :: shortComparisonTest< int > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "longComparisonTest",
                                &CudaReductionTester :: longComparisonTest< int > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "shortScalarProductTest",
                                &CudaReductionTester :: shortScalarProductTest< double > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "longScalarProductTest",
                                &CudaReductionTester :: longScalarProductTest< double > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "shortDiffTest",
                                &CudaReductionTester :: shortDiffTest< double > )
                               );
      suiteOfTests -> addTest( new CppUnit :: TestCaller< CudaReductionTester >(
                                "longDiffTest",
                                &CudaReductionTester :: longDiffTest< double > )
                               );

      return suiteOfTests;
   }

   template< typename RealType >
   void setConstantSequence( const int size,
                             const RealType& value,
                             RealType*& hostData,
                             RealType*& deviceData )
   {
      for( int i = 0; i < size; i ++ )
         hostData[ i ] = value;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< RealType, RealType, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
   }

   template< typename RealType >
   void shortConstantSequenceTest()
   {
      const int shortSequence( 128 );
      RealType *hostData, *deviceData;
      ArrayOperations< Devices::Host >::allocateMemory( hostData, shortSequence );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData, shortSequence );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      RealType result;

      setConstantSequence( shortSequence, ( RealType ) -1, hostData, deviceData );
      tnlParallelReductionSum< RealType, int > sumOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( sumOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -shortSequence );

      tnlParallelReductionMin< RealType, int > minOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( minOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      tnlParallelReductionMax< RealType, int > maxOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( maxOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      tnlParallelReductionAbsSum< RealType, int > absSumOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absSumOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == shortSequence );

      tnlParallelReductionAbsMin< RealType, int > absMinOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMinOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      tnlParallelReductionAbsMax< RealType, int > absMaxOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMaxOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      tnlParallelReductionLpNorm< RealType, int > lpNormOperation;
      lpNormOperation. setPower( 2.0 );
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( lpNormOperation, shortSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == shortSequence );


      ArrayOperations< Devices::Host >::freeMemory( hostData );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceData );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
   }

   template< typename RealType >
   void longConstantSequenceTest()
   {
      const int longSequence( 172892 );
      RealType *hostData, *deviceData;
      ArrayOperations< Devices::Host >::allocateMemory( hostData, longSequence );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData, longSequence );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      RealType result;

      setConstantSequence( longSequence, ( RealType ) -1, hostData, deviceData );
      tnlParallelReductionSum< RealType, int > sumOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( sumOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -longSequence );

      tnlParallelReductionMin< RealType, int > minOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( minOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      tnlParallelReductionMax< RealType, int > maxOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( maxOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      tnlParallelReductionAbsSum< RealType, int > absSumOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absSumOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == longSequence );

      tnlParallelReductionAbsMin< RealType, int > absMinOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMinOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      tnlParallelReductionAbsMax< RealType, int > absMaxOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMaxOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      tnlParallelReductionLpNorm< RealType, int > lpNormOperation;
      lpNormOperation. setPower( 2.0 );
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( lpNormOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == longSequence );

      setConstantSequence( longSequence, ( RealType ) 2, hostData, deviceData );
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( sumOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 2 * longSequence );

      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( minOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 2 );

      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( maxOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 2 );

      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absSumOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 2 * longSequence );

      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMinOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 2 );

      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMaxOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 2 );

      lpNormOperation. setPower( 2.0 );
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( lpNormOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 4 * longSequence );
      lpNormOperation. setPower( 3.0 );
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( lpNormOperation, longSequence, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 8 * longSequence );


      ArrayOperations< Devices::Host >::freeMemory( hostData );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceData );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
   }

   template< typename RealType >
   void linearSequenceTest()
   {
      const int size( 10245 );
      RealType *hostData, *deviceData;
      ArrayOperations< Devices::Host >::allocateMemory( hostData, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      RealType sum( 0.0 );
      for( int i = 0; i < size; i ++ )
      {
         hostData[ i ] = -i - 1;
         sum += hostData[ i ];
      }
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< RealType, RealType, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
      tnlParallelReductionSum< RealType, int > sumOperation;
      RealType result;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( sumOperation, size, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == sum );
      tnlParallelReductionMin< RealType, int > minOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( minOperation, size, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -size );

      tnlParallelReductionMax< RealType, int > maxOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( maxOperation, size, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      tnlParallelReductionAbsSum< RealType, int > absSumOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absSumOperation, size, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == abs( sum ) );

      tnlParallelReductionAbsMin< RealType, int > absMinOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMinOperation, size, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      tnlParallelReductionAbsMax< RealType, int > absMaxOperation;
      CPPUNIT_ASSERT(
         ( reductionOnCudaDevice( absMaxOperation, size, deviceData, ( RealType* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == size );

      ArrayOperations< Devices::Host >::freeMemory( hostData );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceData );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
   }

   template< typename Type >
   void shortLogicalOperationsTest()
   {
      int size( 125 );
      Type *hostData, *deviceData;
      ArrayOperations< Devices::Host >::allocateMemory( hostData, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      for( int i = 0; i < size; i ++ )
         hostData[ i ] = 1;

      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      tnlParallelReductionLogicalAnd< Type, int > andOperation;
      tnlParallelReductionLogicalOr< Type, int > orOperation;
      Type result;
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( andOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( orOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      hostData[ 0 ] = 0;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( andOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 0 );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( orOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      for( int i = 0; i < size; i ++ )
         hostData[ i ] = 0;

      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( andOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 0 );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( orOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 0 );
   }

   template< typename Type >
   void longLogicalOperationsTest()
   {
      int size( 7628198 );
      Type *hostData, *deviceData;
      ArrayOperations< Devices::Host >::allocateMemory( hostData, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      for( int i = 0; i < size; i ++ )
         hostData[ i ] = 1;

      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      tnlParallelReductionLogicalAnd< Type, int > andOperation;
      tnlParallelReductionLogicalOr< Type, int > orOperation;
      Type result;
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( andOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( orOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      hostData[ 0 ] = 0;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( andOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 0 );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( orOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      for( int i = 0; i < size; i ++ )
         hostData[ i ] = 0;

      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData, hostData, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( andOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 0 );
      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( orOperation, size, deviceData, ( Type* ) 0, result ) ) );
      CPPUNIT_ASSERT( result == 0 );
   }

   template< typename Type >
   void shortComparisonTest()
   {
      const int size( 125 );
      Type *hostData1, *hostData2,
           *deviceData1, *deviceData2;
      ArrayOperations< Devices::Host >::allocateMemory( hostData1, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostData2, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData1, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      for( int i = 0; i < size; i ++ )
         hostData1[ i ] = hostData2[ i ] = 1;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData2, hostData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      bool result( false );
      tnlParallelReductionEqualities< Type, int > equalityOperation;
      tnlParallelReductionInequalities< Type, int > inequalityOperation;

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( equalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == true );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( inequalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      hostData1[ 0 ] = 0;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( equalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( inequalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      for( int i = 0; i < size; i ++ )
         hostData1[ i ] = 0;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( equalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( inequalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == true );
   }

   template< typename Type >
   void longComparisonTest()
   {
      const int size( 1258976 );
      Type *hostData1, *hostData2,
           *deviceData1, *deviceData2;
      ArrayOperations< Devices::Host >::allocateMemory( hostData1, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostData2, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData1, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      for( int i = 0; i < size; i ++ )
         hostData1[ i ] = hostData2[ i ] = 1;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData2, hostData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      bool result( false );
      tnlParallelReductionEqualities< Type, int > equalityOperation;
      tnlParallelReductionInequalities< Type, int > inequalityOperation;

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( equalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == true );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( inequalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      hostData1[ 0 ] = 0;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( equalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( inequalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      for( int i = 0; i < size; i ++ )
         hostData1[ i ] = 0;
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( equalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == false );

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( inequalityOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == true );
   }

   template< typename Type >
   void shortScalarProductTest()
   {
      const int size( 125 );
      Type *hostData1, *hostData2,
           *deviceData1, *deviceData2;
      ArrayOperations< Devices::Host >::allocateMemory( hostData1, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostData2, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData1, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      hostData1[ 0 ] = 0;
      hostData2[ 0 ] = 1;
      Type scalarProduct( 0.0 );
      for( int i = 1; i < size; i ++ )
      {
         hostData1[ i ] = i;
         hostData2[ i ] = -hostData2[ i - 1 ];
         scalarProduct += hostData1[ i ] * hostData2[ i ];
      }
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData2, hostData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      Type result( 0.0 );
      tnlParallelReductionScalarProduct< Type, int > scalarProductOperation;

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( scalarProductOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == scalarProduct );
   }


   template< typename Type >
   void longScalarProductTest()
   {
      const int size( 125789 );
      Type *hostData1, *hostData2,
           *deviceData1, *deviceData2;
      ArrayOperations< Devices::Host >::allocateMemory( hostData1, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostData2, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData1, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      hostData1[ 0 ] = 0;
      hostData2[ 0 ] = 1;
      Type scalarProduct( 0.0 );
      for( int i = 1; i < size; i ++ )
      {
         hostData1[ i ] = i;
         hostData2[ i ] = -hostData2[ i - 1 ];
         scalarProduct += hostData1[ i ] * hostData2[ i ];
      }
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData1, hostData1, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceData2, hostData2, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      Type result( 0.0 );
      tnlParallelReductionScalarProduct< Type, int > scalarProductOperation;

      CPPUNIT_ASSERT(
          ( reductionOnCudaDevice( scalarProductOperation, size, deviceData1, deviceData2, result ) ) );
      CPPUNIT_ASSERT( result == scalarProduct );
   }

   template< typename Type >
   void shortDiffTest()
   {
      const int size( 123 );
      Type *hostZeros, *hostOnes, *hostLinear,
           *deviceZeros, *deviceOnes, *deviceLinear;
      ArrayOperations< Devices::Host >::allocateMemory( hostZeros, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostOnes, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostLinear, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceZeros, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceOnes, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceLinear, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      for( int i = 0; i < size; i ++ )
      {
         hostZeros[ i ] = 0;
         hostOnes[ i ] = 1;
         hostLinear[ i ] = i;
      }

      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceZeros, hostZeros, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceOnes, hostOnes, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceLinear, hostLinear, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      tnlParallelReductionDiffSum< Type, int > diffSumOp;
      tnlParallelReductionDiffMin< Type, int > diffMinOp;
      tnlParallelReductionDiffMax< Type, int > diffMaxOp;
      tnlParallelReductionDiffAbsSum< Type, int > diffAbsSumOp;
      tnlParallelReductionDiffAbsMin< Type, int > diffAbsMinOp;
      tnlParallelReductionDiffAbsMax< Type, int > diffAbsMaxOp;
      tnlParallelReductionDiffLpNorm< Type, int > diffLpNormOp;

      Type result;

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceZeros, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 0 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == -size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceOnes, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMaxOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceOnes, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMaxOp, size, deviceOnes, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsMinOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsMaxOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsSumOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffLpNormOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceLinear, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == size * ( size - 1 ) / 2 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == - size * ( size - 1 ) / 2 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceLinear, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == size * ( size - 1 ) / 2 - size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceLinear, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 0 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == -size+1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMaxOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == 0 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsMaxOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == size - 1 );

      ArrayOperations< Devices::Host >::freeMemory( hostZeros );
      ArrayOperations< Devices::Host >::freeMemory( hostOnes );
      ArrayOperations< Devices::Host >::freeMemory( hostLinear );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceZeros );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceOnes );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceLinear );
   }


   template< typename Type >
   void longDiffTest()
   {
      const int size( 12387 );
      Type *hostZeros, *hostOnes, *hostLinear,
           *deviceZeros, *deviceOnes, *deviceLinear;
      ArrayOperations< Devices::Host >::allocateMemory( hostZeros, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostOnes, size );
      ArrayOperations< Devices::Host >::allocateMemory( hostLinear, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceZeros, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceOnes, size );
      ArrayOperations< Devices::Cuda >::allocateMemory( deviceLinear, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      for( int i = 0; i < size; i ++ )
      {
         hostZeros[ i ] = 0;
         hostOnes[ i ] = 1;
         hostLinear[ i ] = i;
      }

      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceZeros, hostZeros, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceOnes, hostOnes, size );
      ArrayOperations< Devices::Cuda, Devices::Host >::copyMemory< Type, Type, int >( deviceLinear, hostLinear, size );
      CPPUNIT_ASSERT( TNL_CHECK_CUDA_DEVICE );

      tnlParallelReductionDiffSum< Type, int > diffSumOp;
      tnlParallelReductionDiffMin< Type, int > diffMinOp;
      tnlParallelReductionDiffMax< Type, int > diffMaxOp;
      tnlParallelReductionDiffAbsSum< Type, int > diffAbsSumOp;
      tnlParallelReductionDiffAbsMin< Type, int > diffAbsMinOp;
      tnlParallelReductionDiffAbsMax< Type, int > diffAbsMaxOp;
      tnlParallelReductionDiffLpNorm< Type, int > diffLpNormOp;

      Type result;

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceZeros, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 0 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceZeros, deviceOnes, result ) ) );

      CPPUNIT_ASSERT( result == -size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceOnes, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMaxOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == -1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceOnes, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMaxOp, size, deviceOnes, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsMinOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsMaxOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsSumOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffLpNormOp, size, deviceZeros, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceLinear, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == size * ( size - 1 ) / 2 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == - size * ( size - 1 ) / 2 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffSumOp, size, deviceLinear, deviceOnes, result ) ) );
      CPPUNIT_ASSERT( result == size * ( size - 1 ) / 2 - size );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceLinear, deviceZeros, result ) ) );
      CPPUNIT_ASSERT( result == 0 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMinOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == -size + 1 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffMaxOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == 0 );

      CPPUNIT_ASSERT(
                ( reductionOnCudaDevice( diffAbsMaxOp, size, deviceZeros, deviceLinear, result ) ) );
      CPPUNIT_ASSERT( result == size - 1 );

      ArrayOperations< Devices::Host >::freeMemory( hostZeros );
      ArrayOperations< Devices::Host >::freeMemory( hostOnes );
      ArrayOperations< Devices::Host >::freeMemory( hostLinear );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceZeros );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceOnes );
      ArrayOperations< Devices::Cuda >::freeMemory( deviceLinear );
   }

};

#else
class CudaReductionTester
{};
#endif /* HAVE_CPPUNIT */

#endif /* CudaReductionTESTER_H_ */
