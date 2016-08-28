/***************************************************************************
                          sparse-matrix-benchmark.h  -  description
                             -------------------
    begin                : Jul 27, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef SPARSEMATRIXBENCHMARK_H_
#define SPARSEMATRIXBENCHMARK_H_

#include <fstream>
#include <iomanip>
#include <TNL/Config/ConfigDescription.h>
#include <TNL/Config/ParameterContainer.h>
#include <TNL/Matrices/Dense.h>
#include <TNL/Matrices/Ellpack.h>
#include <TNL/Matrices/SlicedEllpack.h>
#include <TNL/Matrices/ChunkedEllpack.h>
#include <TNL/Matrices/CSR.h>
#include <TNL/Matrices/MatrixReader.h>
#include <TNL/mfuncs.h>
#include "tnlSpmvBenchmark.h"

#include "tnlConfig.h"
const char configFile[] = TNL_CONFIG_DIRECTORY "tnl-sparse-matrix-benchmark.cfg.desc";


/*
double bestCudaRgCSRGflops( 0 );

template< typename Real >
void benchmarkRgCSRFormat( const CSR< Real, Devices::Host, int >& csrMatrix,
                           const Vector< Real, Devices::Host >& refX,
                           const Vector< Real, Devices::Cuda >& cudaX,
                           Vector< Real, Devices::Host >& refB,
                           bool formatTest,
                           const int maxIterations,
                           const bool useAdaptiveGroupSize,
                           const tnlAdaptiveGroupSizeStrategy adaptiveGroupSizeStrategy,
                           const tnlSpmvBenchmarkCSR< Real, int >& csrMatrixBenchmark,
                           bool verbose,
                           const String& inputMtxFile,
                           const String& logFileName,
                           std::fstream& logFile )
{
   tnlSpmvBenchmarkRgCSR< Real, Devices::Host, int > hostRgCsrMatrixBenchmark;
   for( int groupSize = 16; groupSize <= 64; groupSize *= 2 )
   {

      hostRgCsrMatrixBenchmark. setGroupSize( groupSize );
      hostRgCsrMatrixBenchmark. setUseAdaptiveGroupSize( useAdaptiveGroupSize );
      hostRgCsrMatrixBenchmark. setAdaptiveGroupSizeStrategy( adaptiveGroupSizeStrategy );
      hostRgCsrMatrixBenchmark. setup( csrMatrix );
      if( formatTest )
         hostRgCsrMatrixBenchmark. testMatrix( csrMatrix, verbose );
      hostRgCsrMatrixBenchmark. setMaxIterations( maxIterations );
      //hostRgCsrMatrixBenchmark. runBenchmark( refX, refB, verbose );
      hostRgCsrMatrixBenchmark. tearDown();

      if( logFileName )
         hostRgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                    csrMatrixBenchmark. getGflops(),
                                                    inputMtxFile,
                                                    csrMatrix,
                                                    true );

      tnlSpmvBenchmarkRgCSR< Real, Devices::Cuda, int > cudaRgCsrMatrixBenchmark;
      cudaRgCsrMatrixBenchmark. setGroupSize( groupSize );
      cudaRgCsrMatrixBenchmark. setup( csrMatrix );
      cudaRgCsrMatrixBenchmark. setMaxIterations( maxIterations );
      for( int cudaBlockSize = 32; cudaBlockSize <= 256; cudaBlockSize *= 2 )
      {
         cudaRgCsrMatrixBenchmark. setCudaBlockSize( cudaBlockSize );
         if( formatTest )
            cudaRgCsrMatrixBenchmark. testMatrix( csrMatrix, verbose );
         cudaRgCsrMatrixBenchmark. runBenchmark( cudaX, refB, verbose );
         if( logFileName )
            cudaRgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                       csrMatrixBenchmark. getGflops(),
                                                       inputMtxFile,
                                                       csrMatrix,
                                                       false );
         bestCudaRgCSRGflops = max( bestCudaRgCSRGflops, cudaRgCsrMatrixBenchmark. getGflops() );
      }
      cudaRgCsrMatrixBenchmark. tearDown();
   }
}
*/

template< typename RealType >
bool benchmarkMatrix( const Config::ParameterContainer& parameters )
{
   /****
    * Read the CSR matrix ...
    */
   typedef CSR< RealType, Devices::Host, int > CsrMatrix;
   CsrMatrix csrMatrix;

   const String& inputFileName = parameters.getParameter< String >( "input-file" );
   const String& inputMtxFileName = parameters.getParameter< String >( "input-mtx-file" );
   const String& logFileName = parameters.getParameter< String >( "log-file" );
   const String& pdfFileName = parameters.getParameter< String >( "pdf-file" );
   bool verbose = parameters.getParameter< bool >( "verbose" );
   const int maxIterations = parameters.getParameter< int >( "max-iterations" );

   std::fstream inputFile;
   inputFile.open( inputMtxFileName.getString(), std::ios::in );
   if( ! inputFile )
   {
      std::cerr << "I am not able to open the file " << inputMtxFileName << "." << std::endl;
      return false;
   }
   if( ! MatrixReader< CsrMatrix >::readMtxFile( inputFile, csrMatrix ) )
      return false;

   /****
    * Check the number of the non-zero elements
    */
   const long int nonzeroElements = csrMatrix. getNumberOfNonzeroMatrixElements();
   if( verbose )
     std::cout << "Matrix rows: " << csrMatrix.getRows()
           << " Matrix columns: " << csrMatrix.getColumns()
           << " Non-zero elements: " << nonzeroElements << std::endl;

   const long int rows = csrMatrix.getRows();
   const long int columns = csrMatrix.getColumns();
   Vector< RealType, Devices::Host > refX( "ref-x", columns ), refB( "ref-b", rows );
   Vector< RealType, Devices::Cuda > cudaX( "cudaX", columns );
   refX. setValue( 0.0 );
   for( int i = 0; i < columns; i ++ )
      refX[ i ] = 1.0; //( Real ) i * 1.0 / ( Real ) size;
   cudaX = refX;
   csrMatrix. vectorProduct( refX, refB );

   /****
    * CSR format benchmark
    */
   tnlSpmvBenchmark< CSR< RealType, Devices::Host, int > > csrMatrixBenchmark;

   /****
    * Use the first instance of tnlSpmvBenchmark which we have
    * to write the progress-table header.
    */
   if( verbose )
      csrMatrixBenchmark. writeProgressTableHeader();

   csrMatrixBenchmark. setup( csrMatrix );
   csrMatrixBenchmark. setMaxIterations( maxIterations );
   csrMatrixBenchmark. runBenchmark( refX, refB, verbose );
   csrMatrixBenchmark. tearDown();

   /****
    * Open and write one line to the log file
    */
   std::fstream logFile;
   if( logFileName )
   {
      logFile. open( logFileName. getString(), std::ios::out | std::ios::app );
      if( ! logFile )
      {
         std::cerr << "Unable to open log file " << logFileName << " for appending logs." << std::endl;
         return false;
      }
      /****
       * Open new line of the table and write basic matrix information
       */
      long int allElements = csrMatrix. getRows() * csrMatrix. getColumns();
      logFile << "          <tr>" << std::endl;
      logFile << "             <td> <a href=\"" << pdfFileName << "\">" << inputFile << "</a> </td>" << std::endl;
      logFile << "             <td> " << csrMatrix. getRows() << "</td>" << std::endl;
      logFile << "             <td> " << nonzeroElements << "</td>" << std::endl;
      logFile << "             <td> " << ( double ) nonzeroElements / allElements * 100.0 << "</td>" << std::endl;
      csrMatrixBenchmark. writeToLogTable( logFile,
                                           csrMatrixBenchmark. getGflops(),
                                           inputMtxFileName,
                                           csrMatrix,
                                           false );
   }

#ifdef UNDEF
   /****
    * Cusparse CSR format benchmark
    */
   tnlSpmvBenchmarkCusparseCSR< Real, int > cusparseCSRBenchmark;
   cusparseCSRBenchmark. setup( csrMatrix );
   cusparseCSRBenchmark. setMaxIterations( maxIterations );
   cusparseCSRBenchmark. runBenchmark( cudaX, refB, verbose );
   cusparseCSRBenchmark. tearDown();

   if( logFileName )
       cusparseCSRBenchmark. writeToLogTable( logFile,
                                                    csrMatrixBenchmark. getGflops(),
                                                    inputMtxFile,
                                                    csrMatrix,
                                                    true );

   /****
    * Hybrid format benchmark
    */
   tnlSpmvBenchmarkHybridMatrix< Real, int > hybridMatrixBenchmark;
   hybridMatrixBenchmark. setFileName( inputMtxFile );
   hybridMatrixBenchmark. setup( csrMatrix );
   hybridMatrixBenchmark. setMaxIterations( maxIterations );
   hybridMatrixBenchmark. setNonzeroElements( nonzeroElements );
   hybridMatrixBenchmark. runBenchmark( refX, refB, verbose );
   hybridMatrixBenchmark. tearDown();

   if( logFileName )
   {
      hybridMatrixBenchmark. writeToLogTable( logFile,
                                              csrMatrixBenchmark. getGflops(),
                                              inputMtxFile,
                                              csrMatrix,
                                              false );
   }

   /****
    * Row-Grouped CSR format
    */
   bestCudaRgCSRGflops = 0.0;
   benchmarkRgCSRFormat( csrMatrix,
                         refX,
                         cudaX,
                         refB,
                         formatTest,
                         maxIterations,
                         false,
                         tnlAdaptiveGroupSizeStrategyByAverageRowSize,
                         csrMatrixBenchmark,
                         verbose,
                         inputMtxFile,
                         logFileName,
                         logFile );

   tnlSpmvBenchmarkRgCSR< Real, Devices::Host, int > hostRgCsrMatrixBenchmark;
   hostRgCsrMatrixBenchmark. setGroupSize( 16 );
   hostRgCsrMatrixBenchmark. setUseAdaptiveGroupSize( true );
   hostRgCsrMatrixBenchmark. setAdaptiveGroupSizeStrategy( tnlAdaptiveGroupSizeStrategyByAverageRowSize );
   hostRgCsrMatrixBenchmark. setup( csrMatrix );
   if( formatTest )
      hostRgCsrMatrixBenchmark. testMatrix( csrMatrix, verbose );
   hostRgCsrMatrixBenchmark. setMaxIterations( maxIterations );
   //hostRgCsrMatrixBenchmark. runBenchmark( refX, refB, verbose );
   hostRgCsrMatrixBenchmark. tearDown();
   if( logFileName )
      hostRgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                 csrMatrixBenchmark. getGflops(),
                                                 inputMtxFile,
                                                 csrMatrix,
                                                 true );
   tnlSpmvBenchmarkRgCSR< Real, Devices::Cuda, int > cudaRgCsrMatrixBenchmark;
   for( int cudaBlockSize = 32; cudaBlockSize <= 256; cudaBlockSize *= 2 )
   {
      cudaRgCsrMatrixBenchmark. setCudaBlockSize( cudaBlockSize );
      cudaRgCsrMatrixBenchmark. setGroupSize( 16 );
      cudaRgCsrMatrixBenchmark. setUseAdaptiveGroupSize( true );
      cudaRgCsrMatrixBenchmark. setAdaptiveGroupSizeStrategy( tnlAdaptiveGroupSizeStrategyByAverageRowSize );
      cudaRgCsrMatrixBenchmark. setMaxIterations( maxIterations );
      cudaRgCsrMatrixBenchmark. setup( csrMatrix );
      if( formatTest )
         cudaRgCsrMatrixBenchmark. testMatrix( csrMatrix, verbose );
      cudaRgCsrMatrixBenchmark. runBenchmark( cudaX, refB, verbose );
      if( logFileName )
         cudaRgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                    csrMatrixBenchmark. getGflops(),
                                                    inputMtxFile,
                                                    csrMatrix,
                                                    false );
   }
   cudaRgCsrMatrixBenchmark. tearDown();

   /****
    * Row-Grouped CSR format with reordered rows
    * The rows are now sorted decreasingly by the number of the nonzero elements
    */
   if( verbose )
     std::cout << "          ------------------------------- Test with sorted matrix ----------------------------------          " << std::endl;

   Vector< int, Devices::Host > rowPermutation( "rowPermutation" );
   {
      CSR< Real, Devices::Host > orderedCsrMatrix( "orderedCsrMatrix" );
      csrMatrix. sortRowsDecreasingly( rowPermutation );

      /****
       * Check if the ordering is OK.
       */
      int rowSize = csrMatrix. getNonzeroElementsInRow( rowPermutation[ 0 ] );
      for( int i = 1; i < csrMatrix. getSize(); i ++ )
      {
         if( rowSize < csrMatrix. getNonzeroElementsInRow( rowPermutation[ i ] ) )
         {
            std::cerr << "The rows are not sorted properly. Error is at row number " << i << std::endl;
         }
         rowSize = csrMatrix. getNonzeroElementsInRow( rowPermutation[ i ] );
      }
      orderedCsrMatrix. reorderRows( rowPermutation, csrMatrix );
      orderedCsrMatrix. vectorProduct( refX, refB );
      benchmarkRgCSRFormat( orderedCsrMatrix,
                            refX,
                            cudaX,
                            refB,
                            formatTest,
                            maxIterations,
                            false,
                            tnlAdaptiveGroupSizeStrategyByAverageRowSize,
                            csrMatrixBenchmark,
                            verbose,
                            inputMtxSortedFile,
                            logFileName,
                            logFile );

      tnlSpmvBenchmarkRgCSR< Real, Devices::Host, int > hostRgCsrMatrixBenchmark;
      hostRgCsrMatrixBenchmark. setGroupSize( 16 );
      hostRgCsrMatrixBenchmark. setUseAdaptiveGroupSize( true ); // TODO: fix with true - not implemented yet
      hostRgCsrMatrixBenchmark. setAdaptiveGroupSizeStrategy( tnlAdaptiveGroupSizeStrategyByFirstGroup );
      hostRgCsrMatrixBenchmark. setMaxIterations( maxIterations );
      hostRgCsrMatrixBenchmark. setup( orderedCsrMatrix );
      if( formatTest )
         hostRgCsrMatrixBenchmark. testMatrix( orderedCsrMatrix, verbose );
      //hostRgCsrMatrixBenchmark. runBenchmark( refX, refB, verbose );
      hostRgCsrMatrixBenchmark. tearDown();
      if( logFileName )
         hostRgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                    csrMatrixBenchmark. getGflops(),
                                                    inputMtxSortedFile,
                                                    csrMatrix,
                                                    true );
      for( int cudaBlockSize = 32; cudaBlockSize <= 256; cudaBlockSize *= 2 )
      {
         tnlSpmvBenchmarkRgCSR< Real, Devices::Cuda, int > cudaRgCsrMatrixBenchmark;
         cudaRgCsrMatrixBenchmark. setCudaBlockSize( cudaBlockSize );
         cudaRgCsrMatrixBenchmark. setGroupSize( 16 );
         cudaRgCsrMatrixBenchmark. setUseAdaptiveGroupSize( true );
         cudaRgCsrMatrixBenchmark. setAdaptiveGroupSizeStrategy( tnlAdaptiveGroupSizeStrategyByFirstGroup );
         cudaRgCsrMatrixBenchmark. setup( orderedCsrMatrix );
         cudaRgCsrMatrixBenchmark. setMaxIterations( maxIterations );

         if( formatTest )
            cudaRgCsrMatrixBenchmark. testMatrix( orderedCsrMatrix, verbose );
         cudaRgCsrMatrixBenchmark. runBenchmark( cudaX, refB, verbose );
         if( logFileName )
            cudaRgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                       csrMatrixBenchmark. getGflops(),
                                                       inputMtxSortedFile,
                                                       csrMatrix,
                                                       false );
      }
      cudaRgCsrMatrixBenchmark. tearDown();
   }
   csrMatrix. vectorProduct( refX, refB );

   /****
    * Adaptive Row-Grouped CSR format
    */

   for( int desiredChunkSize = 1; desiredChunkSize <= 32; desiredChunkSize *= 2 )
   {
      tnlSpmvBenchmarkAdaptiveRgCSR< Real, Devices::Cuda, int > cudaArgCsrMatrixBenchmark;
      cudaArgCsrMatrixBenchmark. setDesiredChunkSize( desiredChunkSize );
      for( int cudaBlockSize = 32; cudaBlockSize <= 256; cudaBlockSize *= 2 )
      {
         cudaArgCsrMatrixBenchmark. setCudaBlockSize( cudaBlockSize );
         cudaArgCsrMatrixBenchmark. setup( csrMatrix );
         if( formatTest )
            cudaArgCsrMatrixBenchmark. testMatrix( csrMatrix, verbose );
         cudaArgCsrMatrixBenchmark. setMaxIterations( maxIterations );
         cudaArgCsrMatrixBenchmark. runBenchmark( cudaX, refB, verbose );
         cudaArgCsrMatrixBenchmark. setBestRgCSRGflops( bestCudaRgCSRGflops );
         if( logFileName )
            cudaArgCsrMatrixBenchmark. writeToLogTable( logFile,
                                                        csrMatrixBenchmark. getGflops(),
                                                        inputMtxFile,
                                                        csrMatrix,
                                                        true );
      }
      cudaRgCsrMatrixBenchmark. tearDown();
   }

#endif


   if( logFileName )
   {
      logFile << "          </tr>" << std::endl;
      logFile. close();
   }
   return true;

}

int main( int argc, char* argv[] )
{
   Config::ParameterContainer parameters;
   Config::ConfigDescription conf_desc;

   if( conf_desc.parseConfigDescription( configFile ) != 0 )
      return 1;
   if( ! parseCommandLine( argc, argv, conf_desc, parameters ) )
   {
      conf_desc.printUsage( argv[ 0 ] );
      return 1;
   }
   const String& precision = parameters.getParameter< String >( "precision" );
   if( precision == "float" )
      if( ! benchmarkMatrix< float >( parameters ) )
         return EXIT_FAILURE;
   if( precision == "double" )
      if( ! benchmarkMatrix< double >( parameters ) )
         return EXIT_FAILURE;
   return EXIT_SUCCESS;
}

#endif /* SPARSEMATRIXBENCHMARK_H_ */
