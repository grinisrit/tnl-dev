
#pragma once

#include "NonlinearDiffusion.h"
#include <TNL/Meshes/Grid.h>

namespace TNL {
namespace Operators {

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
String
NonlinearDiffusion< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
getType()
{
   return String( "NonlinearDiffusion< " ) +
          MeshType::getType() + ", " +
         TNL::getType< Real >() + ", " +
         TNL::getType< Index >() + "," +
          NonlinearDiffusionOperator::getType() + " >";
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
template< typename MeshEntity,
          typename Vector >
__cuda_callable__
Real
NonlinearDiffusion< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
operator()( const MeshEntity& entity,
            const Vector& u,
            const Real& time ) const
{
    return nonlinearDiffusionOperator( u, entity, time );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
   template< typename MeshEntity >          
__cuda_callable__
Index
NonlinearDiffusion< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
getLinearSystemRowLength( const MeshType& mesh,
                          const IndexType& index,
                          const MeshEntity& entity ) const
{
   return nonlinearDiffusionOperator.getLinearSystemRowLength( mesh, index, entity );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
template< typename MeshEntity,
          typename MeshFunction,
          typename Vector,           
          typename Matrix >
__cuda_callable__
void
NonlinearDiffusion< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
setMatrixElements( const RealType& time,
                    const RealType& tau,
                    const MeshType& mesh,
                    const IndexType& index,
                    const MeshEntity& entity,
                    const MeshFunction& u,
                    Vector& b,
                    Matrix& matrix ) const
{
    nonlinearDiffusionOperator.setMatrixElements( time, tau, mesh, index, entity, u, b, matrix );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
String
NonlinearDiffusion< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
getType()
{
   return String( "NonlinearDiffusion< " ) +
          MeshType::getType() + ", " +
         TNL::getType< Real >() + ", " +
         TNL::getType< Index >() + "," +
          NonlinearDiffusionOperator::getType() + " >";
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
template< typename MeshEntity,
          typename Vector >
__cuda_callable__
Real
NonlinearDiffusion< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
operator()( const MeshEntity& entity,
            const Vector& u,
            const Real& time ) const
{
    return nonlinearDiffusionOperator( u, entity, time );
}
       
template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
   template< typename MeshEntity >
__cuda_callable__
Index
NonlinearDiffusion< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
getLinearSystemRowLength( const MeshType& mesh,
                          const IndexType& index,
                          const MeshEntity& entity ) const
{
   return nonlinearDiffusionOperator.getLinearSystemRowLength( mesh, index, entity );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
template< typename MeshEntity,
          typename MeshFunction,
          typename Vector,           
          typename Matrix >
__cuda_callable__
void
NonlinearDiffusion< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
setMatrixElements( const RealType& time,
                    const RealType& tau,
                    const MeshType& mesh,
                    const IndexType& index,
                    const MeshEntity& entity,
                    const MeshFunction& u,
                    Vector& b,
                    Matrix& matrix ) const
{
    nonlinearDiffusionOperator.setMatrixElements( time, tau, mesh, index, entity, u, b, matrix );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
String
NonlinearDiffusion< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
getType()
{
   return String( "NonlinearDiffusion< " ) +
          MeshType::getType() + ", " +
         TNL::getType< Real >() + ", " +
         TNL::getType< Index >() + "," +
          NonlinearDiffusionOperator::getType() + " >";
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
template< typename MeshEntity,
          typename Vector >
__cuda_callable__
Real
NonlinearDiffusion< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
operator()( const MeshEntity& entity,
            const Vector& u,
            const Real& time ) const
{
    return nonlinearDiffusionOperator( u, entity, time );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
   template< typename MeshEntity >
__cuda_callable__
Index
NonlinearDiffusion< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
getLinearSystemRowLength( const MeshType& mesh,
                          const IndexType& index,
                          const MeshEntity& entity ) const
{
   return nonlinearDiffusionOperator.getLinearSystemRowLength( mesh, index, entity );
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index,
          typename NonlinearDiffusionOperator >
template< typename MeshEntity,
          typename MeshFunction,
          typename Vector,          
          typename Matrix >
__cuda_callable__
void
NonlinearDiffusion< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, NonlinearDiffusionOperator, Real, Index >::
setMatrixElements( const RealType& time,
                    const RealType& tau,
                    const MeshType& mesh,
                    const IndexType& index,
                    const MeshEntity& entity,
                    const MeshFunction& u,
                    Vector& b,
                    Matrix& matrix ) const
{
    nonlinearDiffusionOperator.setMatrixElements( time, tau, mesh, index, entity, u, b, matrix );
}

} // namespace Operators
} // namespace TNL
