/***************************************************************************
                          DistributedVerticalOperations.h  -  description
                             -------------------
    begin                : Aug 6, 2019
    copyright            : (C) 2019 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/Expressions/VerticalOperations.h>
#include <TNL/Communicators/MpiDefs.h>

namespace TNL {
namespace Containers {
namespace Expressions {

////
// Vertical operations
template< typename Expression >
auto DistributedExpressionMin( const Expression& expression ) -> std::decay_t< decltype( expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = std::numeric_limits< ResultType >::max();
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionMin( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_MIN, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionArgMin( const Expression& expression )
-> std::pair< typename Expression::IndexType, std::decay_t< decltype( expression[0] ) > >
{
   using RealType = std::decay_t< decltype( expression[0] ) >;
   using IndexType = typename Expression::IndexType;
   using ResultType = std::pair< IndexType, RealType >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result( -1, std::numeric_limits< RealType >::max() );
   const auto group = expression.getCommunicationGroup();
   if( group != CommunicatorType::NullGroup ) {
      // compute local argMin
      ResultType localResult = ExpressionArgMin( expression.getConstLocalView() );
      // transform local index to global index
      localResult.first += expression.getLocalRange().getBegin();

      // scatter local result to all processes and gather their results
      const int nproc = CommunicatorType::GetSize( group );
      ResultType dataForScatter[ nproc ];
      for( int i = 0; i < nproc; i++ ) dataForScatter[ i ] = localResult;
      ResultType gatheredResults[ nproc ];
      // NOTE: exchanging general data types does not work with MPI
      //CommunicatorType::Alltoall( dataForScatter, 1, gatheredResults, 1, group );
      CommunicatorType::Alltoall( (char*) dataForScatter, sizeof(ResultType), (char*) gatheredResults, sizeof(ResultType), group );

      // reduce the gathered data
      const auto* _data = gatheredResults;  // workaround for nvcc which does not allow to capture variable-length arrays (even in pure host code!)
      auto fetch = [_data] ( IndexType i ) { return _data[ i ].second; };
      auto reduction = [] ( IndexType& aIdx, const IndexType& bIdx, RealType& a, const RealType& b ) {
         if( a > b ) {
            a = b;
            aIdx = bIdx;
         }
         else if( a == b && bIdx < aIdx )
            aIdx = bIdx;
      };
      result = Algorithms::Reduction< Devices::Host >::reduceWithArgument( (IndexType) nproc, reduction, fetch, std::numeric_limits< RealType >::max() );
      result.first = gatheredResults[ result.first ].first;
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionMax( const Expression& expression ) -> std::decay_t< decltype( expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = std::numeric_limits< ResultType >::lowest();
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionMax( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_MAX, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionArgMax( const Expression& expression )
-> std::pair< typename Expression::IndexType, std::decay_t< decltype( expression[0] ) > >
{
   using RealType = std::decay_t< decltype( expression[0] ) >;
   using IndexType = typename Expression::IndexType;
   using ResultType = std::pair< IndexType, RealType >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result( -1, std::numeric_limits< RealType >::lowest() );
   const auto group = expression.getCommunicationGroup();
   if( group != CommunicatorType::NullGroup ) {
      // compute local argMax
      ResultType localResult = ExpressionArgMax( expression.getConstLocalView() );
      // transform local index to global index
      localResult.first += expression.getLocalRange().getBegin();

      // scatter local result to all processes and gather their results
      const int nproc = CommunicatorType::GetSize( group );
      ResultType dataForScatter[ nproc ];
      for( int i = 0; i < nproc; i++ ) dataForScatter[ i ] = localResult;
      ResultType gatheredResults[ nproc ];
      // NOTE: exchanging general data types does not work with MPI
      //CommunicatorType::Alltoall( dataForScatter, 1, gatheredResults, 1, group );
      CommunicatorType::Alltoall( (char*) dataForScatter, sizeof(ResultType), (char*) gatheredResults, sizeof(ResultType), group );

      // reduce the gathered data
      const auto* _data = gatheredResults;  // workaround for nvcc which does not allow to capture variable-length arrays (even in pure host code!)
      auto fetch = [_data] ( IndexType i ) { return _data[ i ].second; };
      auto reduction = [] ( IndexType& aIdx, const IndexType& bIdx, RealType& a, const RealType& b ) {
         if( a < b ) {
            a = b;
            aIdx = bIdx;
         }
         else if( a == b && bIdx < aIdx )
            aIdx = bIdx;
      };
      result = Algorithms::Reduction< Devices::Host >::reduceWithArgument( (IndexType) nproc, reduction, fetch, std::numeric_limits< RealType >::lowest() );
      result.first = gatheredResults[ result.first ].first;
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionSum( const Expression& expression ) -> std::decay_t< decltype( expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] ) >;
   using IndexType = typename Expression::IndexType;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 0;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionSum( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_SUM, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionL1Norm( const Expression& expression ) -> std::decay_t< decltype( expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] ) >;
   using IndexType = typename Expression::IndexType;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 0;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionL1Norm( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_SUM, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionL2Norm( const Expression& expression ) -> std::decay_t< decltype( expression[0] * expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] * expression[0] ) >;
   using IndexType = typename Expression::IndexType;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 0;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionL2Norm( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_SUM, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression, typename Real >
auto DistributedExpressionLpNorm( const Expression& expression, const Real& p ) -> std::decay_t< decltype( TNL::pow( expression[0], p ) ) >
{
   using ResultType = std::decay_t< decltype( TNL::pow( expression[0], p ) ) >;
   using IndexType = typename Expression::IndexType;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 0;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionLpNorm( expression.getConstLocalView(), p );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_SUM, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionProduct( const Expression& expression ) -> std::decay_t< decltype( expression[0] * expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 1;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionProduct( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_PROD, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionLogicalAnd( const Expression& expression ) -> std::decay_t< decltype( expression[0] && expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] && expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = std::numeric_limits< ResultType >::max();
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionLogicalAnd( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_LAND, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionLogicalOr( const Expression& expression ) -> std::decay_t< decltype( expression[0] || expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] || expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 0;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionLogicalOr( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_LOR, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionBinaryAnd( const Expression& expression ) -> std::decay_t< decltype( expression[0] | expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] & expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = std::numeric_limits< ResultType >::max();
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionLogicalBinaryAnd( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_BAND, expression.getCommunicationGroup() );
   }
   return result;
}

template< typename Expression >
auto DistributedExpressionBinaryOr( const Expression& expression ) -> std::decay_t< decltype( expression[0] | expression[0] ) >
{
   using ResultType = std::decay_t< decltype( expression[0] | expression[0] ) >;
   using CommunicatorType = typename Expression::CommunicatorType;

   ResultType result = 0;
   if( expression.getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const ResultType localResult = ExpressionBinaryOr( expression.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_BOR, expression.getCommunicationGroup() );
   }
   return result;
}

} // namespace Expressions
} // namespace Containers
} // namespace TNL
