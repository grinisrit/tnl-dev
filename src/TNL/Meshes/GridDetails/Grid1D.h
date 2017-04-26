/***************************************************************************
                          Grid1D.h  -  description
                             -------------------
    begin                : Feb 13, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Meshes/Grid.h>
#include <TNL/Logger.h>
#include <TNL/Meshes/GridDetails/GridEntityTopology.h>
#include <TNL/Meshes/GridDetails/GridEntityGetter.h>
#include <TNL/Meshes/GridDetails/NeighbourGridEntityGetter.h>
#include <TNL/Meshes/GridEntity.h>
#include <TNL/Meshes/GridEntityConfig.h>

namespace TNL {
namespace Meshes {

template< typename Real,
          typename Device,
          typename Index >
class Grid< 1, Real, Device, Index > : public Object
{
   public:

   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;
   typedef Containers::StaticVector< 1, Real > PointType;
   typedef Containers::StaticVector< 1, Index > CoordinatesType;
   typedef Grid< 1, Real, Devices::Host, Index > HostType;
   typedef Grid< 1, Real, Devices::Cuda, Index > CudaType;
   typedef Grid< 1, Real, Device, Index > ThisType;
 
   static const int meshDimension = 1;
 
   template< int EntityDimension,
             typename Config = GridEntityCrossStencilStorage< 1 > >
   using MeshEntity = GridEntity< ThisType, EntityDimension, Config >;
 
   typedef MeshEntity< meshDimension, GridEntityCrossStencilStorage< 1 > > Cell;
   typedef MeshEntity< 0 > Face;
   typedef MeshEntity< 0 > Vertex;

   static constexpr int getMeshDimension() { return meshDimension; };
 
   Grid();

   static String getType();

   String getTypeVirtual() const;

   static String getSerializationType();

   virtual String getSerializationTypeVirtual() const;

   void setDimensions( const Index xSize );

   void setDimensions( const CoordinatesType& dimensions );

   __cuda_callable__
   const CoordinatesType& getDimensions() const;

   void setDomain( const PointType& origin,
                   const PointType& proportions );

   __cuda_callable__
   inline const PointType& getOrigin() const;

   __cuda_callable__
   inline const PointType& getProportions() const;
 
   template< typename EntityType >
   __cuda_callable__
   IndexType getEntitiesCount() const;
   
   template< int EntiytDimensions >
   __cuda_callable__
   IndexType getEntitiesCount() const;
   
   
   template< typename EntityType >
   __cuda_callable__
   EntityType getEntity( const IndexType& entityIndex ) const;
   
   template< typename EntityType >
   __cuda_callable__
   Index getEntityIndex( const EntityType& entity ) const;
   
   template< typename EntityType >
   __cuda_callable__
   RealType getEntityMeasure( const EntityType& entity ) const;
 
   __cuda_callable__
   inline const RealType& getCellMeasure() const;
 
   __cuda_callable__
   inline const PointType& getSpaceSteps() const;

   template< int xPow >
   __cuda_callable__
   const RealType& getSpaceStepsProducts() const;
   
   __cuda_callable__
   RealType getSmallestSpaceStep() const;


   template< typename GridFunction >
   typename GridFunction::RealType getDifferenceAbsMax( const GridFunction& f1,
                                                        const GridFunction& f2 ) const;

   template< typename GridFunction >
   typename GridFunction::RealType getDifferenceLpNorm( const GridFunction& f1,
                                                        const GridFunction& f2,
                                                        const typename GridFunction::RealType& p ) const;

   /****
    *  Method for saving the object to a file as a binary data
    */
   bool save( File& file ) const;

   /****
    *  Method for restoring the object from a file
    */
   bool load( File& file );

   bool save( const String& fileName ) const;

   bool load( const String& fileName );

   bool writeMesh( const String& fileName,
                   const String& format ) const;

   template< typename MeshFunction >
   bool write( const MeshFunction& function,
               const String& fileName,
               const String& format ) const;

   void writeProlog( Logger& logger );

   protected:

   void computeSpaceSteps();

   CoordinatesType dimensions;
 
   IndexType numberOfCells, numberOfVertices;

   PointType origin, proportions;
 
   PointType spaceSteps;
 
   RealType spaceStepsProducts[ 5 ];
};

} // namespace Meshes
} // namespace TNL

#include <TNL/Meshes/GridDetails/Grid1D_impl.h>
