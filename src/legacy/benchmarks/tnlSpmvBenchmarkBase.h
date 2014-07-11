/***************************************************************************
                          tnlSpmvBenchmarkBase.h  -  description
                             -------------------
    begin                : May 15, 2011
    copyright            : (C) 2011 by Tomas Oberhuber
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

#ifndef TNLSPMVBENCHMARKBASE_H_
#define TNLSPMVBENCHMARKBASE_H_

#include <matrices/tnlCSRMatrix.h>
#include <core/tnlTimerRT.h>
#include <core/mfuncs.h>


double tnlSpmvBenchmarkPrecision( const double& ) { return 1.0e-12; }
float tnlSpmvBenchmarkPrecision( const float& ) { return 1.0e-4; }

template< typename Matrix >
class tnlSpmvBenchmarkBase
{
   public:

   tnlSpmvBenchmarkBase();
   
   typedef typename Matrix::RealType RealType;
   typedef typename Matrix::DeviceType DeviceType;
   typedef typename Matrix::IndexType IndexType;

   bool getBenchmarkWasSuccesful() const;

   double getGflops() const;

   double getTime() const;

   void setMaxIterations( const int maxIterations );

   int getIterations() const;

   IndexType getArtificialZeros() const;

   RealType getMaxError() const;

   void writeProgressTableHeader();

   virtual bool setup( const tnlCSRMatrix< RealType, tnlHost, IndexType >& matrix ) = 0;

   virtual void tearDown() = 0;

   virtual void writeProgress() const = 0;

   /****
    * This is virtual only the purpose of testing external formats like
    * the Hybrid format from the CUSP library. This format is not wrapped
    * in tnlMatrix.
    */
   virtual void runBenchmark( const tnlVector< RealType, DeviceType, IndexType >& x,
                              const tnlVector< RealType, tnlHost, IndexType >& refB,
                              bool verbose );

   virtual void writeToLogTable( ostream& logFile,
                                 const double& csrGflops,
                                 const tnlString& inputMtxFile,
                                 const tnlCSRMatrix< RealType, tnlHost, IndexType >& csrMatrix,
                                 bool writeMatrixInfo  ) const = 0;

   protected:

   /****
    * This is helper method for generating HTML table with benchmark results
    */
   tnlString getBgColorBySpeedUp( const double& speedUp ) const;

   /****
    * Helper method for writing matrix statistics and information to HTML
    */
   bool printMatrixInHtml( const tnlString& fileName,
                           tnlMatrix< RealType, tnlHost, IndexType >& matrix ) const;


   bool benchmarkWasSuccesful;

   bool setupOk;

   double gflops;

   double time;

   /****
    * Max number of SpMV repetitions.
    */
   int maxIterations;

   /****
    * Real number of repetitions.
    */
   int iterations;

   IndexType artificialZeros;

   RealType maxError;

   IndexType firstErrorOccurence;

   Matrix matrix;

   /****
    * Parameters for the progress table columns
    */

   int formatColumnWidth;

   int timeColumnWidth;

   int iterationsColumnWidth;

   int gflopsColumnWidth;

   int benchmarkStatusColumnWidth;

   int infoColumnWidth;
};


#include "tnlSpmvBenchmarkBase_impl.h"
#endif /* TNLSPMVBENCHMARKBASE_H_ */