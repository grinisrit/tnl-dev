/***************************************************************************
                          Benchmarks.h  -  description
                             -------------------
    begin                : Dec 30, 2015
    copyright            : (C) 2015 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovsky,
//                 Tomas Oberhuber

#pragma once

#include "CustomLogging.h"

#include <limits>

#include <TNL/String.h>
#include <TNL/Solvers/IterativeSolverMonitor.h>

namespace TNL {
namespace Benchmarks {

const double oneGB = 1024.0 * 1024.0 * 1024.0;

struct BenchmarkResult
{
   using HeaderElements = typename Logging::HeaderElements;
   using RowElements = typename Logging::RowElements;

   int loops = 0;
   double time = std::numeric_limits<double>::quiet_NaN();
   double stddev = std::numeric_limits<double>::quiet_NaN();
   double bandwidth = std::numeric_limits<double>::quiet_NaN();
   double speedup = std::numeric_limits<double>::quiet_NaN();

   virtual HeaderElements getTableHeader() const
   {
      return HeaderElements({ "time", "stddev", "stddev/time", "bandwidth", "speedup" });
   }

   virtual std::vector< int > getColumnWidthHints() const
   {
      return std::vector< int >({ 14, 14, 14, 14, 14 });
   }

   virtual RowElements getRowElements() const
   {
      RowElements elements;
      // write in scientific format to avoid precision loss
      elements << std::scientific << time << stddev << stddev / time << bandwidth;
      if( speedup != 0 )
         elements << speedup;
      else
         elements << "N/A";
      return elements;
   }
};

template< typename Logger = CustomLogging >
class Benchmark
{
   public:
      using MetadataElement = typename Logger::MetadataElement;
      using MetadataMap = typename Logger::MetadataMap;
      using MetadataColumns = typename Logger::MetadataColumns;
      using SolverMonitorType = Solvers::IterativeSolverMonitor< double, int >;

      Benchmark( int loops = 10, bool verbose = true );

      static void configSetup( Config::ConfigDescription& config );

      void setup( const Config::ParameterContainer& parameters );

      // TODO: ensure that this is not called in the middle of the benchmark
      // (or just remove it completely?)
      void setLoops( int loops );

      void setMinTime( const double& minTime );

      // Marks the start of a new benchmark
      void newBenchmark( const String & title );

      // Marks the start of a new benchmark (with custom metadata)
      void newBenchmark( const String & title,
                        MetadataMap metadata );

      // Sets metadata columns -- values used for all subsequent rows until
      // the next call to this function.
      void setMetadataColumns( const MetadataColumns & metadata );

      // Sets the value of one metadata column -- useful for iteratively
      // changing MetadataColumns that were set using the previous method.
      void setMetadataElement( const typename MetadataColumns::value_type & element );

      // Sets the width of metadata columns when printed to the terminal.
      void setMetadataWidths( const std::map< std::string, int > & widths );

      // Sets the dataset size and base time for the calculations of bandwidth
      // and speedup in the benchmarks result.
      void setDatasetSize( const double datasetSize = 0.0, // in GB
                           const double baseTime = 0.0 );

      // Sets current operation -- operations expand the table vertically
      //  - baseTime should be reset to 0.0 for most operations, but sometimes
      //    it is useful to override it
      //  - Order of operations inside a "Benchmark" does not matter, rows can be
      //    easily sorted while converting to HTML.)
      void
      setOperation( const String & operation,
                    const double datasetSize = 0.0, // in GB
                    const double baseTime = 0.0 );

      // Times a single ComputeFunction. Subsequent calls implicitly split
      // the current operation into sub-columns identified by "performer",
      // which are further split into "bandwidth", "time" and "speedup" columns.
      template< typename Device,
                typename ResetFunction,
                typename ComputeFunction >
      void time( ResetFunction reset,
                 const String & performer,
                 ComputeFunction & compute,
                 BenchmarkResult & result );

      template< typename Device,
                typename ResetFunction,
                typename ComputeFunction >
      BenchmarkResult time( ResetFunction reset,
                            const String & performer,
                            ComputeFunction & compute );

      // The same methods as above but without the reset function
      template< typename Device,
                typename ComputeFunction >
      void time( const String & performer,
                 ComputeFunction & compute,
                 BenchmarkResult & result );

      template< typename Device,
                typename ComputeFunction >
      BenchmarkResult time( const String & performer,
                            ComputeFunction & compute );

      // Adds an error message to the log. Should be called in places where the
      // "time" method could not be called (e.g. due to failed allocation).
      void addErrorMessage( const std::string& message );

      bool save( std::ostream& logFile );

      SolverMonitorType& getMonitor();

      double getBaseTime() const;

      bool isResetingOn() const;

   protected:
      Logger logger;

      int loops = 1;

      double minTime = 0.0;

      double datasetSize = 0.0;

      double baseTime = 0.0;

      bool reset = true;

      SolverMonitorType monitor;
};

} // namespace Benchmarks
} // namespace TNL

#include "Benchmarks.hpp"
