/***************************************************************************
                          SharedVector.h  -  description
                             -------------------
    begin                : Nov 8, 2012
    copyright            : (C) 2012 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Containers/SharedVector.h>
#include <TNL/Containers/Algorithms/VectorOperations.h>

namespace TNL {
namespace Containers {   

template< typename Real,
          typename Device,
          typename Index >
#ifndef HAVE_MIC
__cuda_callable__
#endif
SharedVector< Real, Device, Index >::SharedVector()
{
}

template< typename Real,
          typename Device,
          typename Index >
#ifndef HAVE_MIC
__cuda_callable__
#endif
SharedVector< Real, Device, Index >::SharedVector( Real* data,
                                                         const Index size )
: Containers::SharedArray< Real, Device, Index >( data, size )
{
}

template< typename Real,
          typename Device,
          typename Index >
#ifndef HAVE_MIC
__cuda_callable__
#endif
SharedVector< Real, Device, Index >::SharedVector( Vector< Real, Device, Index >& vector )
: Containers::SharedArray< Real, Device, Index >( vector )
{
}

template< typename Real,
          typename Device,
          typename Index >
#ifndef HAVE_MIC
__cuda_callable__
#endif
SharedVector< Real, Device, Index >::SharedVector( SharedVector< Real, Device, Index >& vector )
: Containers::SharedArray< Real, Device, Index >( vector )
{
}

template< typename Real,
          typename Device,
          typename Index >
String SharedVector< Real, Device, Index > :: getType()
{
   return String( "Containers::SharedVector< " ) +
                    TNL::getType< Real >() + ", " +
                     Device :: getDeviceType() + ", " +
                    TNL::getType< Index >() + " >";
};

template< typename Real,
          typename Device,
          typename Index >
String SharedVector< Real, Device, Index > :: getTypeVirtual() const
{
   return this->getType();
};

template< typename Real,
          typename Device,
          typename Index >
String SharedVector< Real, Device, Index > :: getSerializationType()
{
   return Vector< Real, Devices::Host, Index >::getType();
};

template< typename Real,
          typename Device,
          typename Index >
String SharedVector< Real, Device, Index > :: getSerializationTypeVirtual() const
{
   return this->getSerializationType();
};

template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index >::addElement( const IndexType i,
                                                         const RealType& value )
{
   Algorithms::VectorOperations< Device >::addElement( *this, i, value );
}

template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index >::addElement( const IndexType i,
                                                         const RealType& value,
                                                         const RealType& thisElementMultiplicator )
{
   Algorithms::VectorOperations< Device >::addElement( *this, i, value, thisElementMultiplicator );
}

template< typename Real,
           typename Device,
           typename Index >
SharedVector< Real, Device, Index >&
   SharedVector< Real, Device, Index > :: operator = ( const SharedVector< Real, Device, Index >& vector )
{
   Containers::SharedArray< Real, Device, Index > :: operator = ( vector );
   return ( *this );
};

template< typename Real,
           typename Device,
           typename Index >
   template< typename Vector >
SharedVector< Real, Device, Index >&
   SharedVector< Real, Device, Index > :: operator = ( const Vector& vector )
{
   Containers::SharedArray< Real, Device, Index > :: operator = ( vector );
   return ( *this );
};

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vector >
bool SharedVector< Real, Device, Index > :: operator == ( const Vector& vector ) const
{
   return Containers::SharedArray< Real, Device, Index > :: operator == ( vector );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vector >
bool SharedVector< Real, Device, Index > :: operator != ( const Vector& vector ) const
{
   return Containers::SharedArray< Real, Device, Index > :: operator != ( vector );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vector >
SharedVector< Real, Device, Index >& SharedVector< Real, Device, Index > :: operator -= ( const Vector& vector )
{
   this->addVector( vector, -1.0 );
   return ( *this );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vector >
SharedVector< Real, Device, Index >& SharedVector< Real, Device, Index > :: operator += ( const Vector& vector )
{
   this->addVector( vector );
   return ( *this );
}

template< typename Real,
          typename Device,
          typename Index >
SharedVector< Real, Device, Index >& SharedVector< Real, Device, Index > :: operator *= ( const RealType& c )
{
   Algorithms::VectorOperations< Device >::vectorScalarMultiplication( *this, c );
   return *this;
}

template< typename Real,
          typename Device,
          typename Index >
SharedVector< Real, Device, Index >& SharedVector< Real, Device, Index > :: operator /= ( const RealType& c )
{
   Algorithms::VectorOperations< Device >::vectorScalarMultiplication( *this, 1.0/ c );
   return *this;
}

template< typename Real,
          typename Device,
          typename Index >
Real SharedVector< Real, Device, Index > :: max() const
{
   return Algorithms::VectorOperations< Device > :: getVectorMax( *this );
}

template< typename Real,
          typename Device,
          typename Index >
Real SharedVector< Real, Device, Index > :: min() const
{
   return Algorithms::VectorOperations< Device > :: getVectorMin( *this );
}


template< typename Real,
          typename Device,
          typename Index >
Real SharedVector< Real, Device, Index > :: absMax() const
{
   return Algorithms::VectorOperations< Device > :: getVectorAbsMax( *this );
}

template< typename Real,
          typename Device,
          typename Index >
Real SharedVector< Real, Device, Index > :: absMin() const
{
   return Algorithms::VectorOperations< Device > :: getVectorAbsMin( *this );
}

template< typename Real,
          typename Device,
          typename Index >
Real SharedVector< Real, Device, Index > :: lpNorm( const Real& p ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorLpNorm( *this, p );
}


template< typename Real,
          typename Device,
          typename Index >
Real SharedVector< Real, Device, Index > :: sum() const
{
   return Algorithms::VectorOperations< Device > :: getVectorSum( *this );
}


template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: differenceMax( const Vector& v ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorDifferenceMax( *this, v );
}


template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: differenceMin( const Vector& v ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorDifferenceMin( *this, v );
}


template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: differenceAbsMax( const Vector& v ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorDifferenceAbsMax( *this, v );
}

template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: differenceAbsMin( const Vector& v ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorDifferenceAbsMin( *this, v );
}

template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: differenceLpNorm( const Vector& v, const Real& p ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorDifferenceLpNorm( *this, v, p );
}


template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: differenceSum( const Vector& v ) const
{
   return Algorithms::VectorOperations< Device > :: getVectorDifferenceSum( *this, v );
}


template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index > :: scalarMultiplication( const Real& alpha )
{
   Algorithms::VectorOperations< Device > :: vectorScalarMultiplication( *this, alpha );
}


template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
Real SharedVector< Real, Device, Index > :: scalarProduct( const Vector& v )
{
   return Algorithms::VectorOperations< Device > :: getScalarProduct( *this, v );
}

template< typename Real,
          typename Device,
          typename Index >
template< typename Vector >
void SharedVector< Real, Device, Index > :: addVector( const Vector& x,
                                                          const Real& alpha,
                                                          const Real& thisMultiplicator )
{
   Algorithms::VectorOperations< Device > :: addVector( *this, x, alpha, thisMultiplicator );
}

template< typename Real,
          typename Device,
          typename Index >
   template< typename Vector >
void
SharedVector< Real, Device, Index >::
addVectors( const Vector& v1,
            const Real& multiplicator1,
            const Vector& v2,
            const Real& multiplicator2,
            const Real& thisMultiplicator )
{
   Algorithms::VectorOperations< Device >::addVectors( *this, v1, multiplicator1, v2, multiplicator2, thisMultiplicator );
}

template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index > :: computePrefixSum()
{
   Algorithms::VectorOperations< Device >::computePrefixSum( *this, 0, this->getSize() );
}

template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index > :: computePrefixSum( const IndexType begin,
                                                                 const IndexType end )
{
   Algorithms::VectorOperations< Device >::computePrefixSum( *this, begin, end );
}

template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index > :: computeExclusivePrefixSum()
{
   Algorithms::VectorOperations< Device >::computeExclusivePrefixSum( *this, 0, this->getSize() );
}

template< typename Real,
          typename Device,
          typename Index >
void SharedVector< Real, Device, Index > :: computeExclusivePrefixSum( const IndexType begin,
                                                                          const IndexType end )
{
   Algorithms::VectorOperations< Device >::computeExclusivePrefixSum( *this, begin, end );
}

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

#ifdef INSTANTIATE_FLOAT
extern template class SharedVector< float, Devices::Host, int >;
#endif
extern template class SharedVector< double, Devices::Host, int >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class SharedVector< long double, Devices::Host, int >;
#endif
#ifdef INSTANTIATE_LONG_INT
#ifdef INSTANTIATE_FLOAT
extern template class SharedVector< float, Devices::Host, long int >;
#endif
extern template class SharedVector< double, Devices::Host, long int >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class SharedVector< long double, Devices::Host, long int >;
#endif
#endif

#ifdef HAVE_CUDA
// TODO: fix this - it does not work with CUDA 5.5
/*
#ifdef INSTANTIATE_FLOAT
extern template class SharedVector< float, Devices::Cuda, int >;
#endif
extern template class SharedVector< double, Devices::Cuda, int >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class SharedVector< long double, Devices::Cuda, int >;
#endif
#ifdef INSTANTIATE_LONG_INT
#ifdef INSTANTIATE_FLOAT
extern template class SharedVector< float, Devices::Cuda, long int >;
#endif
extern template class SharedVector< double, Devices::Cuda, long int >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class SharedVector< long double, Devices::Cuda, long int >;
#endif
 #endif
 */
#endif

#endif

} // namespace Containers
} // namespace TNL
