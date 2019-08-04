/***************************************************************************
                          DistributedVector_impl.h  -  description
                             -------------------
    begin                : Sep 7, 2018
    copyright            : (C) 2018 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

// Implemented by: Jakub Klinkovský

#pragma once

#include "DistributedVector.h"
#include <TNL/Containers/Algorithms/ReductionOperations.h>
#include <TNL/Exceptions/NotImplementedError.h>

namespace TNL {
namespace Containers {

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
typename DistributedVector< Real, Device, Index, Communicator >::LocalViewType
DistributedVector< Real, Device, Index, Communicator >::
getLocalView()
{
   return BaseType::getLocalView();
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
typename DistributedVector< Real, Device, Index, Communicator >::ConstLocalViewType
DistributedVector< Real, Device, Index, Communicator >::
getConstLocalView() const
{
   return BaseType::getConstLocalView();
}

template< typename Value,
          typename Device,
          typename Index,
          typename Communicator >
typename DistributedVector< Value, Device, Index, Communicator >::ViewType
DistributedVector< Value, Device, Index, Communicator >::
getView()
{
   return ViewType( this->getLocalRange(), this->getSize(), this->getCommunicationGroup(), this->getLocalView() );
}

template< typename Value,
          typename Device,
          typename Index,
          typename Communicator >
typename DistributedVector< Value, Device, Index, Communicator >::ConstViewType
DistributedVector< Value, Device, Index, Communicator >::
getConstView() const
{
   return ConstViewType( this->getLocalRange(), this->getSize(), this->getCommunicationGroup(), this->getConstLocalView() );
}

template< typename Value,
          typename Device,
          typename Index,
          typename Communicator >
DistributedVector< Value, Device, Index, Communicator >::
operator ViewType()
{
   return getView();
}

template< typename Value,
          typename Device,
          typename Index,
          typename Communicator >
DistributedVector< Value, Device, Index, Communicator >::
operator ConstViewType() const
{
   return getConstView();
}


template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
String
DistributedVector< Real, Device, Index, Communicator >::
getType()
{
   return String( "Containers::DistributedVector< " ) +
          TNL::getType< Real >() + ", " +
          Device::getDeviceType() + ", " +
          TNL::getType< Index >() + ", " +
          // TODO: communicators don't have a getType method
          "<Communicator> >";
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
String
DistributedVector< Real, Device, Index, Communicator >::
getTypeVirtual() const
{
   return getType();
}


/*
 * Usual Vector methods follow below.
 */

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
void
DistributedVector< Real, Device, Index, Communicator >::
addElement( IndexType i,
            RealType value )
{
   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const IndexType li = this->getLocalRange().getLocalIndex( i );
      LocalViewType view = getLocalView();
      view.addElement( li, value );
   }
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Scalar >
void
DistributedVector< Real, Device, Index, Communicator >::
addElement( IndexType i,
            RealType value,
            Scalar thisElementMultiplicator )
{
   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      const IndexType li = this->getLocalRange().getLocalIndex( i );
      LocalViewType view = getLocalView();
      view.addElement( li, value, thisElementMultiplicator );
   }
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Vector >
DistributedVector< Real, Device, Index, Communicator >&
DistributedVector< Real, Device, Index, Communicator >::
operator-=( const Vector& vector )
{
   TNL_ASSERT_EQ( this->getSize(), vector.getSize(),
                  "Vector sizes must be equal." );
   TNL_ASSERT_EQ( this->getLocalRange(), vector.getLocalRange(),
                  "Multiary operations are supported only on vectors which are distributed the same way." );
   TNL_ASSERT_EQ( this->getCommunicationGroup(), vector.getCommunicationGroup(),
                  "Multiary operations are supported only on vectors within the same communication group." );

   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      getLocalView() -= vector.getConstLocalView();
   }
   return *this;
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Vector >
DistributedVector< Real, Device, Index, Communicator >&
DistributedVector< Real, Device, Index, Communicator >::
operator+=( const Vector& vector )
{
   TNL_ASSERT_EQ( this->getSize(), vector.getSize(),
                  "Vector sizes must be equal." );
   TNL_ASSERT_EQ( this->getLocalRange(), vector.getLocalRange(),
                  "Multiary operations are supported only on vectors which are distributed the same way." );
   TNL_ASSERT_EQ( this->getCommunicationGroup(), vector.getCommunicationGroup(),
                  "Multiary operations are supported only on vectors within the same communication group." );

   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      getLocalView() += vector.getConstLocalView();
   }
   return *this;
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Scalar >
DistributedVector< Real, Device, Index, Communicator >&
DistributedVector< Real, Device, Index, Communicator >::
operator*=( Scalar c )
{
   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      getLocalView() *= c;
   }
   return *this;
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Scalar >
DistributedVector< Real, Device, Index, Communicator >&
DistributedVector< Real, Device, Index, Communicator >::
operator/=( Scalar c )
{
   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      getLocalView() /= c;
   }
   return *this;
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename ResultType >
ResultType
DistributedVector< Real, Device, Index, Communicator >::
sum() const
{
   const auto group = this->getCommunicationGroup();
   ResultType result = Containers::Algorithms::ParallelReductionSum< Real, ResultType >::initialValue();
   if( group != CommunicatorType::NullGroup ) {
      const ResultType localResult = getConstLocalView().sum();
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_SUM, group );
   }
   return result;
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Vector >
Real
DistributedVector< Real, Device, Index, Communicator >::
scalarProduct( const Vector& v ) const
{
   const auto group = this->getCommunicationGroup();
   Real result = Containers::Algorithms::ParallelReductionScalarProduct< Real, typename Vector::RealType >::initialValue();
   if( group != CommunicatorType::NullGroup ) {
      const Real localResult = getConstLocalView().scalarProduct( v.getConstLocalView() );
      CommunicatorType::Allreduce( &localResult, &result, 1, MPI_SUM, group );
   }
   return result;
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Vector, typename Scalar1, typename Scalar2 >
void
DistributedVector< Real, Device, Index, Communicator >::
addVector( const Vector& x,
           Scalar1 alpha,
           Scalar2 thisMultiplicator )
{
   TNL_ASSERT_EQ( this->getSize(), x.getSize(),
                  "Vector sizes must be equal." );
   TNL_ASSERT_EQ( this->getLocalRange(), x.getLocalRange(),
                  "Multiary operations are supported only on vectors which are distributed the same way." );
   TNL_ASSERT_EQ( this->getCommunicationGroup(), x.getCommunicationGroup(),
                  "Multiary operations are supported only on vectors within the same communication group." );

   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      getLocalView().addVector( x.getConstLocalView(), alpha, thisMultiplicator );
   }
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
   template< typename Vector1, typename Vector2, typename Scalar1, typename Scalar2, typename Scalar3 >
void
DistributedVector< Real, Device, Index, Communicator >::
addVectors( const Vector1& v1,
            Scalar1 multiplicator1,
            const Vector2& v2,
            Scalar2 multiplicator2,
            Scalar3 thisMultiplicator )
{
   TNL_ASSERT_EQ( this->getSize(), v1.getSize(),
                  "Vector sizes must be equal." );
   TNL_ASSERT_EQ( this->getLocalRange(), v1.getLocalRange(),
                  "Multiary operations are supported only on vectors which are distributed the same way." );
   TNL_ASSERT_EQ( this->getCommunicationGroup(), v1.getCommunicationGroup(),
                  "Multiary operations are supported only on vectors within the same communication group." );
   TNL_ASSERT_EQ( this->getSize(), v2.getSize(),
                  "Vector sizes must be equal." );
   TNL_ASSERT_EQ( this->getLocalRange(), v2.getLocalRange(),
                  "Multiary operations are supported only on vectors which are distributed the same way." );
   TNL_ASSERT_EQ( this->getCommunicationGroup(), v2.getCommunicationGroup(),
                  "Multiary operations are supported only on vectors within the same communication group." );

   if( this->getCommunicationGroup() != CommunicatorType::NullGroup ) {
      getLocalView().addVectors( v1.getConstLocalView(),
                                       multiplicator1,
                                       v2.getConstLocalView(),
                                       multiplicator2,
                                       thisMultiplicator );
   }
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
void
DistributedVector< Real, Device, Index, Communicator >::
computePrefixSum()
{
   throw Exceptions::NotImplementedError("Distributed prefix sum is not implemented yet.");
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
void
DistributedVector< Real, Device, Index, Communicator >::
computePrefixSum( IndexType begin, IndexType end )
{
   throw Exceptions::NotImplementedError("Distributed prefix sum is not implemented yet.");
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
void
DistributedVector< Real, Device, Index, Communicator >::
computeExclusivePrefixSum()
{
   throw Exceptions::NotImplementedError("Distributed prefix sum is not implemented yet.");
}

template< typename Real,
          typename Device,
          typename Index,
          typename Communicator >
void
DistributedVector< Real, Device, Index, Communicator >::
computeExclusivePrefixSum( IndexType begin, IndexType end )
{
   throw Exceptions::NotImplementedError("Distributed prefix sum is not implemented yet.");
}

} // namespace Containers
} // namespace TNL
