/***************************************************************************
                          tnlSpmvBenchmark.h  -  description
                             -------------------
    begin                : Dec 29, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLSPMVBENCHMARK_H_
#define TNLSPMVBENCHMARK_H_

#include "tnlSpmvBenchmarkBase.h"
#include <TNL/Matrices/CSR.h>


template< typename Matrix >
class tnlSpmvBenchmark
{
};

template< typename Real, typename Device, typename Index >
class tnlSpmvBenchmark< CSR< Real, Device, Index > > : public tnlSpmvBenchmarkBase< CSR< Real, Device, Index > >
{
   public:

   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;

   bool setup( const CSR< RealType, Devices::Host, IndexType >& matrix );

   void tearDown();

   void writeProgress() const;

   void writeToLogTable( std::ostream& logFile,
                                    const double& csrGflops,
                                    const String& inputMtxFile,
                                    const CSR< RealType, Devices::Host, IndexType >& csrMatrix,
                                    bool writeMatrixInfo  ) const;
};

#include "tnlSpmvBenchmark_impl.h"

#endif /* TNLSPMVBENCHMARK_H_ */