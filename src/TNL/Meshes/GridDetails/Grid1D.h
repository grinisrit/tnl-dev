// Copyright (c) 2004-2022 Tomáš Oberhuber et al.
//
// This file is part of TNL - Template Numerical Library (https://tnl-project.org/)
//
// SPDX-License-Identifier: MIT

#pragma once

#include <TNL/Logger.h>
#include <TNL/Meshes/Grid.h>
#include <TNL/Meshes/GridDetails/GridEntityTopology.h>
#include <TNL/Meshes/GridDetails/GridEntityGetter.h>
#include <TNL/Meshes/GridDetails/NeighborGridEntityGetter.h>
#include <TNL/Meshes/GridEntity.h>
#include <TNL/Meshes/GridEntityConfig.h>

namespace TNL {
namespace Meshes {

template< typename Real, typename Device, typename Index >
class Grid< 1, Real, Device, Index >
{
public:
   using RealType = Real;
   using DeviceType = Device;
   using GlobalIndexType = Index;
   using PointType = Containers::StaticVector< 1, Real >;
   using CoordinatesType = Containers::StaticVector< 1, Index >;

   // TODO: deprecated and to be removed (GlobalIndexType shall be used instead)
   using IndexType = Index;

   /**
    * \brief Returns number of this mesh grid dimensions.
    */
   static constexpr int
   getMeshDimension()
   {
      return 1;
   };

   template< int EntityDimension, typename Config = GridEntityCrossStencilStorage< 1 > >
   using EntityType = GridEntity< Grid, EntityDimension, Config >;

   using Cell = EntityType< getMeshDimension(), GridEntityCrossStencilStorage< 1 > >;
   using Face = EntityType< 0 >;
   using Vertex = EntityType< 0 >;

   /**
    * \brief Basic constructor.
    */
   Grid() = default;

   Grid( Index xSize );

   // empty destructor is needed only to avoid crappy nvcc warnings
   ~Grid() = default;

   /**
    * \brief Sets the size of dimensions.
    * \param xSize Size of dimesion x.
    */
   void
   setDimensions( Index xSize );

   /**
    * \brief Sets the number of dimensions.
    * \param dimensions Number of dimensions.
    */
   void
   setDimensions( const CoordinatesType& dimensions );

   __cuda_callable__
   const CoordinatesType&
   getDimensions() const;

   void
   setLocalBegin( const CoordinatesType& begin );

   __cuda_callable__
   const CoordinatesType&
   getLocalBegin() const;

   void
   setLocalEnd( const CoordinatesType& end );

   __cuda_callable__
   const CoordinatesType&
   getLocalEnd() const;

   void
   setInteriorBegin( const CoordinatesType& begin );

   __cuda_callable__
   const CoordinatesType&
   getInteriorBegin() const;

   void
   setInteriorEnd( const CoordinatesType& end );

   __cuda_callable__
   const CoordinatesType&
   getInteriorEnd() const;

   /**
    * \brief Sets the origin.
    * \param origin Starting point of this grid.
    */
   void
   setOrigin( const PointType& origin );

   /**
    * \brief Sets the origin and proportions of this grid.
    * \param origin Point where this grid starts.
    * \param proportions Total length of this grid.
    */
   void
   setDomain( const PointType& origin, const PointType& proportions );

   /**
    * \brief Returns the origin.
    * \param origin Starting point of this grid.
    */
   __cuda_callable__
   inline const PointType&
   getOrigin() const;

   /**
    * \brief Gets length of one entity of this grid.
    */
   __cuda_callable__
   inline const PointType&
   getProportions() const;

   /**
    * \brief Gets number of entities in this grid.
    * \tparam EntityDimension Integer specifying dimension of the entity.
    */
   template< int EntityDimension >
   __cuda_callable__
   IndexType
   getEntitiesCount() const;

   /**
    * \brief Gets number of entities in this grid.
    * \tparam Entity Type of the entity.
    */
   template< typename Entity >
   __cuda_callable__
   IndexType
   getEntitiesCount() const;

   /**
    * \brief Gets entity type using entity index.
    * \param entityIndex Index of entity.
    * \tparam Entity Type of the entity.
    */
   template< typename Entity >
   __cuda_callable__
   inline Entity
   getEntity( const IndexType& entityIndex ) const;

   /**
    * \brief Gets entity index using entity type.
    * \param entity Type of entity.
    * \tparam Entity Type of the entity.
    */
   template< typename Entity >
   __cuda_callable__
   inline Index
   getEntityIndex( const Entity& entity ) const;

   /**
    * \brief Returns the length of one step.
    */
   __cuda_callable__
   inline const PointType&
   getSpaceSteps() const;

   /**
    * \brief Sets the length of steps.
    * \param steps Length of one step.
    */
   inline void
   setSpaceSteps( const PointType& steps );

   /**
    * \brief Returns product of space steps to the xPow.
    * \tparam xPow Exponent.
    */
   template< int xPow >
   __cuda_callable__
   const RealType&
   getSpaceStepsProducts() const;

   /**
    * \breif Returns the measure (length) of a cell in this grid.
    */
   __cuda_callable__
   inline const RealType&
   getCellMeasure() const;

   /**
    * \brief Returns the smallest length of step out of all coordinates (axes).
    */
   __cuda_callable__
   inline RealType
   getSmallestSpaceStep() const;

   void
   writeProlog( Logger& logger ) const;

protected:
   void
   computeProportions();

   void
   computeSpaceStepPowers();

   void
   computeSpaceSteps();

   CoordinatesType dimensions = { 0 };
   CoordinatesType localBegin = { 0 };
   CoordinatesType localEnd = { 0 };
   CoordinatesType interiorBegin = { 0 };
   CoordinatesType interiorEnd = { 0 };

   IndexType numberOfCells = 0;
   IndexType numberOfVertices = 0;

   PointType origin = { 0 };
   PointType proportions = { 0 };
   PointType spaceSteps = { 0 };

   RealType spaceStepsProducts[ 5 ];
};

}  // namespace Meshes
}  // namespace TNL

#include <TNL/Meshes/GridDetails/Grid1D_impl.h>
