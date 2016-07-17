/***************************************************************************
                          tnlBackwardFiniteDifference.h  -  description
                             -------------------
    begin                : Jan 9, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <operators/fdm/tnlFiniteDifferences.h>
#include <operators/fdm/tnlExactDifference.h>
#include <operators/tnlOperator.h>

namespace TNL {

template< typename Mesh,
          int Xdifference = 0,
          int YDifference = 0,
          int ZDifference = 0,
          typename RealType = typename Mesh::RealType,
          typename IndexType = typename Mesh::IndexType >
class tnlBackwardFiniteDifference
{
};

template< int Dimensions,
          typename MeshReal,
          typename MeshDevice,
          typename MeshIndex,
          int XDifference,
          int YDifference,
          int ZDifference,
          typename Real,
          typename Index >
class tnlBackwardFiniteDifference< tnlGrid< Dimensions, MeshReal, MeshDevice, MeshIndex >, XDifference, YDifference, ZDifference, Real, Index >
: public tnlOperator< tnlGrid< Dimensions, MeshReal, MeshDevice, MeshIndex >,
                      MeshInteriorDomain, Dimensions, Dimensions, Real, Index >
{
   public:
 
      typedef tnlGrid< Dimensions, MeshReal, MeshDevice, MeshIndex > MeshType;
      typedef Real RealType;
      typedef MeshDevice DeviceType;
      typedef Index IndexType;
      typedef tnlExactDifference< Dimensions, XDifference, YDifference, ZDifference > ExactOperatorType;
 
      static constexpr int getMeshDimensions() { return Dimensions; }
 
      static tnlString getType()
      {
         return tnlString( "tnlBackwardFiniteDifference< " ) +
            MeshType::getType() + ", " +
            tnlString( XDifference ) + ", " +
            tnlString( YDifference ) + ", " +
            tnlString( ZDifference ) + ", " +
            ::getType< RealType >() + ", " +
            ::getType< IndexType >() + " >";
      }
 
      template< typename MeshFunction, typename MeshEntity >
      __cuda_callable__
      inline Real operator()( const MeshFunction& u,
                              const MeshEntity& entity,
                              const RealType& time = 0.0 ) const
      {
         static_assert( MeshFunction::getEntitiesDimensions() == Dimensions,
            "Finite differences can be evaluate only on mesh cells, i.e. the dimensions count of the mesh entities of mesh function must be the same as mesh dimensions count." );
         const int XDirection = -1 * ( XDifference != 0 );
         const int YDirection = -1 * ( YDifference != 0 );
         const int ZDirection = -1 * ( ZDifference != 0 );
         return tnlFiniteDifferences<
            MeshType,
            Real,
            Index,
            XDifference,
            YDifference,
            ZDifference,
            XDirection,
            YDirection,
            ZDirection >::getValue( u, entity );
      };
};

} // namespace TNL

