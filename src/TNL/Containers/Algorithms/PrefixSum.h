/***************************************************************************
                          PrefixSum.h  -  description
                             -------------------
    begin                : May 9, 2019
    copyright            : (C) 2019 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Tomas Oberhuber, Jakub Klinkovsky

#pragma once

#include <TNL/Devices/Host.h>
#include <TNL/Devices/Cuda.h>
#include <TNL/Devices/MIC.h>

namespace TNL {
namespace Containers {
namespace Algorithms {

template< typename Device >
class PrefixSum
{
};

template<>
class PrefixSum< Devices::Host >
{
   public:
      template< typename Index,
                typename Result,
                typename PrefixSumOperation,
                typename VolatilePrefixSumOperation >
      static Result
      inclusive( const Index size,
                 PrefixSumOperation& reduction,
                 VolatilePrefixSumOperation& volatilePrefixSum,
                 const Result& zero );

      template< typename Index,
                typename Result,
                typename PrefixSumOperation,
                typename VolatilePrefixSumOperation >
      static Result
      exclusive( const Index size,
                 PrefixSumOperation& reduction,
                 VolatilePrefixSumOperation& volatilePrefixSum,
                 const Result& zero );
};

template<>
class PrefixSum< Devices::Cuda >
{
   public:
      template< typename Index,
                typename Result,
                typename PrefixSumOperation,
                typename VolatilePrefixSumOperation >
      static Result
      inclusive( const Index size,
                 PrefixSumOperation& reduction,
                 VolatilePrefixSumOperation& volatilePrefixSum,
                 const Result& zero );

      template< typename Index,
                typename Result,
                typename PrefixSumOperation,
                typename VolatilePrefixSumOperation >
      static Result
      exclusive( const Index size,
                 PrefixSumOperation& reduction,
                 VolatilePrefixSumOperation& volatilePrefixSum,
                 const Result& zero );
};

template<>
class PrefixSum< Devices::MIC >
{
   public:
      template< typename Index,
                typename Result,
                typename PrefixSumOperation,
                typename VolatilePrefixSumOperation >
      static Result
      inclusive( const Index size,
                 PrefixSumOperation& reduction,
                 VolatilePrefixSumOperation& volatilePrefixSum,
                 const Result& zero );

      template< typename Index,
                typename Result,
                typename PrefixSumOperation,
                typename VolatilePrefixSumOperation >
      static Result
      exclusive( const Index size,
                 PrefixSumOperation& reduction,
                 VolatilePrefixSumOperation& volatilePrefixSum,
                 const Result& zero );
};

} // namespace Algorithms
} // namespace Containers
} // namespace TNL

#include <TNL/Conatainers/Algorithms/PrefixSum.hpp>
