/***************************************************************************
                          Grid2D.h  -  description
                             -------------------
    begin                : Feb 13, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Logger.h>
#include <TNL/Meshes/Grid.h>
#include <TNL/Meshes/GridDetails/GridEntityTopology.h>
#include <TNL/Meshes/GridDetails/GridEntityGetter.h>
#include <TNL/Meshes/GridDetails/NeighborGridEntityGetter.h>
#include <TNL/Meshes/GridEntity.h>
#include <TNL/Meshes/GridEntityConfig.h>
#include <TNL/Meshes/DistributedMeshes/DistributedMesh.h>

namespace TNL {
namespace Meshes {

template< typename Real,
          typename Device,
          typename Index >
class Grid< 2, Real, Device, Index > : public Object
{
   public:

   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index GlobalIndexType;
   typedef Containers::StaticVector< 2, Real > PointType;
   typedef Containers::StaticVector< 2, Index > CoordinatesType;
   typedef Grid< 2, Real, Devices::Host, Index > HostType;
   typedef Grid< 2, Real, Devices::Cuda, Index > CudaType;

   typedef DistributedMeshes::DistributedMesh <Grid> DistributedMeshType;
 
   // TODO: deprecated and to be removed (GlobalIndexType shall be used instead)
   typedef Index IndexType;

   static constexpr int getMeshDimension() { return 2; };

   template< int EntityDimension,
             typename Config = GridEntityCrossStencilStorage< 1 > >
   using EntityType = GridEntity< Grid, EntityDimension, Config >;

   typedef EntityType< getMeshDimension(), GridEntityCrossStencilStorage< 1 > > Cell;
   typedef EntityType< getMeshDimension() - 1 > Face;
   typedef EntityType< 0 > Vertex;

   /**
    * \brief See Grid1D::Grid().
    */
   Grid();

   /**
    * \brief See Grid1D::getType().
    */
   Grid( const Index xSize, const Index ySize );

   static String getType();

   /**
    * \brief See Grid1D::getTypeVirtual().
    */
   String getTypeVirtual() const;

   /**
    * \brief See Grid1D::getSerializationType().
    */
   static String getSerializationType();

   /**
    * \brief See Grid1D::getSerializationTypeVirtual().
    */
   virtual String getSerializationTypeVirtual() const;

   /**
    * \brief Sets the size of dimensions.
    * \param xSize Size of dimension x.
    * \param ySize Size of dimension y.
    */
   void setDimensions( const Index xSize, const Index ySize );

   /**
    * \brief See Grid1D::setDimensions( const CoordinatesType& dimensions ).
    */
   void setDimensions( const CoordinatesType& dimensions );

   /**
    * \brief See Grid1D::getDimensions().
    */
   __cuda_callable__
   const CoordinatesType& getDimensions() const;

   /**
    * \brief See Grid1D::setDomain().
    */
   void setDomain( const PointType& origin,
                   const PointType& proportions );

   /**
    * \brief See Grid1D::setOrigin()
    */
   void setOrigin( const PointType& origin);

   /**
    * \brief See Grid1D::getOrigin().
    */
   __cuda_callable__
   inline const PointType& getOrigin() const;

   /**
    * \brief See Grid1D::getProportions().
    */
   __cuda_callable__
   inline const PointType& getProportions() const;

   /**
    * \brief Gets number of entities in this grid.
    * \tparam EntityDimension Integer specifying dimension of the entity.
    */
   template< int EntityDimension >
   __cuda_callable__
   IndexType getEntitiesCount() const;

   /**
    * \brief Gets number of entities in this grid.
    * \tparam Entity Type of the entity.
    */
   template< typename Entity >
   __cuda_callable__
   inline IndexType getEntitiesCount() const;

   /**
    * \brief See Grid1D::getEntity().
    */
   template< typename Entity >
   __cuda_callable__
   inline Entity getEntity( const IndexType& entityIndex ) const;

   /**
    * \brief See Grid1D::getEntityIndex().
    */
   template< typename Entity >
   __cuda_callable__
   inline Index getEntityIndex( const Entity& entity ) const;

   /**
    * \brief See Grid1D::getSpaceSteps().
    */
   __cuda_callable__
   inline const PointType& getSpaceSteps() const;

   /**
    * \brief See Grid1D::setSpaceSteps().
    */
   inline void setSpaceSteps(const PointType& steps);

   /**
    * \brief Returns product of space steps to the xPow.
    * \tparam xPow Exponent for dimension x.
    * \tparam yPow Exponent for dimension y.
    */
   template< int xPow, int yPow >
   __cuda_callable__
   const RealType& getSpaceStepsProducts() const;

   /**
    * \breif Returns the measure (area) of a cell in this grid.
    */
   __cuda_callable__
   inline const RealType& getCellMeasure() const;

   /**
    * \brief See Grid1D::getSmallestSpaceStep().
    */
   __cuda_callable__
   inline RealType getSmallestSpaceStep() const;


   template< typename GridFunction >
   typename GridFunction::RealType getAbsMax( const GridFunction& f ) const;

   template< typename GridFunction >
   typename GridFunction::RealType getLpNorm( const GridFunction& f,
                                              const typename GridFunction::RealType& p ) const;

   template< typename GridFunction >
   typename GridFunction::RealType getDifferenceAbsMax( const GridFunction& f1,
                                                        const GridFunction& f2 ) const;

   template< typename GridFunction >
   typename GridFunction::RealType getDifferenceLpNorm( const GridFunction& f1,
                                                        const GridFunction& f2,
                                                        const typename GridFunction::RealType& p ) const;
   
   void setDistMesh(DistributedMeshType * distGrid);
   
   DistributedMeshType * getDistributedMesh() const;

   /**
    * \brief See Grid1D::save( File& file ) const.
    */
   void save( File& file ) const;

   /**
    * \brief See Grid1D::load( File& file ).
    */
   void load( File& file );

   /**
    * \brief See Grid1D::save( const String& fileName ) const.
    */
   void save( const String& fileName ) const;

   /**
    * \brief See Grid1D::load( const String& fileName ).
    */
   void load( const String& fileName );

   void writeProlog( Logger& logger ) const;

   protected:
   
   void computeProportions();
       
   __cuda_callable__
   void computeSpaceStepPowers();

   __cuda_callable__
   void computeSpaceSteps();

   CoordinatesType dimensions;

   IndexType numberOfCells, numberOfNxFaces, numberOfNyFaces, numberOfFaces, numberOfVertices;

   PointType origin, proportions;

   PointType spaceSteps;

   RealType spaceStepsProducts[ 5 ][ 5 ];
   
   DistributedMeshType *distGrid;
 
   template< typename, typename, int >
   friend class GridEntityGetter;
};

} // namespace Meshes
} // namespace TNL

#include <TNL/Meshes/GridDetails/Grid2D_impl.h>
