/***************************************************************************
                          SpmvBenchmarkResult.h  -  description
                             -------------------
    begin                : Mar 5, 2020
    copyright            : (C) 2020 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include "../Benchmarks.h"

namespace TNL {
namespace Benchmarks {

template< typename Real,
          typename Device,
          typename Index,
          typename ResultReal = Real,
          typename Logger = JsonLogging >
struct SpmvBenchmarkResult
: public BenchmarkResult
{
   using RealType = Real;
   using DeviceType = Device;
   using IndexType = Index;
   using HostVector = Containers::Vector< Real, Devices::Host, Index >;
   using BenchmarkVector = Containers::Vector< ResultReal, Device, Index >;

   using typename BenchmarkResult::HeaderElements;
   using typename BenchmarkResult::RowElements;
   using BenchmarkResult::stddev;
   using BenchmarkResult::bandwidth;
   using BenchmarkResult::speedup;
   using BenchmarkResult::time;


   SpmvBenchmarkResult( const String& format,
                        const HostVector& csrResult,
                        const BenchmarkVector& benchmarkResult,
                        const IndexType nonzeros )
   : format( format ), csrResult( csrResult ), benchmarkResult( benchmarkResult ), nonzeros( nonzeros ){};

   virtual HeaderElements getTableHeader() const override
   {
      return HeaderElements({ "format", "device", "non-zeros", "time", "stddev", "stddev/time", "bandwidth", "speedup", "CSR Diff.Max", "CSR Diff.L2" });
   }

   virtual std::vector< int > getColumnWidthHints() const override
   {
      return std::vector< int >({ 35, 12, 12, 12, 12, 14, 12, 12, 14, 14 });
   }

   void setFormat( const String& format ) { this->format = format; };

   virtual RowElements getRowElements() const override
   {
      HostVector benchmarkResultCopy;
      benchmarkResultCopy = benchmarkResult;
      auto diff = csrResult - benchmarkResultCopy;
      RowElements elements;
      elements << format
               << ( std::is_same< Device, Devices::Host >::value ? "CPU" : "GPU" )
               << nonzeros << time << stddev << stddev/time << bandwidth;
      if( speedup != 0.0 )
         elements << speedup;
      else elements << "N/A";
      elements << max( abs( diff ) ) << lpNorm( diff, 2.0 );
      return elements;
   }

   String format;
   const HostVector& csrResult;
   const BenchmarkVector& benchmarkResult;
   const IndexType nonzeros;
};

} //namespace Benchmarks
} //namespace TNL
