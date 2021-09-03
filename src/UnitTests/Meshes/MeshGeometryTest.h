#pragma once

#ifdef HAVE_GTEST
#include <gtest/gtest.h>

#include <sstream>

#include <TNL/Meshes/Mesh.h>
#include <TNL/Meshes/MeshEntity.h>
#include <TNL/Meshes/DefaultConfig.h>
#include <TNL/Meshes/Topologies/Vertex.h>
#include <TNL/Meshes/Topologies/Edge.h>
#include <TNL/Meshes/Topologies/Polygon.h>
#include <TNL/Meshes/Topologies/Wedge.h>
#include <TNL/Meshes/Topologies/Pyramid.h>
#include <TNL/Meshes/Topologies/Polyhedron.h>
#include <TNL/Meshes/MeshBuilder.h>

#include <TNL/Meshes/Geometry/getEntityMeasure.h>
#include <TNL/Meshes/Geometry/isPlanar.h>
#include <TNL/Meshes/Geometry/getDecomposedMesh.h>
#include <TNL/Meshes/Geometry/getPlanarMesh.h>

#include <TNL/Meshes/Writers/VTKWriter.h>

namespace MeshGeometryTest {

using namespace TNL;
using namespace TNL::Meshes;

class TestPolygon2DMeshConfig : public DefaultConfig< Topologies::Polygon >
{
public:
   static constexpr bool subentityStorage( int entityDimension, int subentityDimension ) { return true; }
   static constexpr bool superentityStorage( int entityDimension, int superentityDimension ) { return true; }
};

class TestPolygon3DMeshConfig : public DefaultConfig< Topologies::Polygon >
{
public:
   static constexpr int spaceDimension = 3;
   static constexpr bool subentityStorage( int entityDimension, int subentityDimension ) { return true; }
   static constexpr bool superentityStorage( int entityDimension, int superentityDimension ) { return true; }
};

class TestWedgeMeshConfig : public DefaultConfig< Topologies::Wedge >
{
public:
   static constexpr bool subentityStorage( int entityDimension, int subentityDimension ) { return true; }
   static constexpr bool superentityStorage( int entityDimension, int superentityDimension ) { return true; }
};

class TestPyramidMeshConfig : public DefaultConfig< Topologies::Pyramid >
{
public:
   static constexpr bool subentityStorage( int entityDimension, int subentityDimension ) { return true; }
   static constexpr bool superentityStorage( int entityDimension, int superentityDimension ) { return true; }
};

class TestPolyhedronMeshConfig : public DefaultConfig< Topologies::Polyhedron >
{
public:
   static constexpr bool subentityStorage( int entityDimension, int subentityDimension ) { return true; }
   static constexpr bool superentityStorage( int entityDimension, int superentityDimension ) { return true; }
};

TEST( MeshGeometryTest, Polygon2DAreaTest )
{
   using PolygonTestMesh = Mesh< TestPolygon2DMeshConfig >;
   using PolygonMeshEntityType = MeshEntity< TestPolygon2DMeshConfig, Devices::Host, Topologies::Polygon >;
   using VertexMeshEntityType = typename PolygonMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   // Set up a non-convex 2D polygon.

   PointType point0(  3.0,  4.0 ),
             point1(  5.0, 11.0 ),
             point2( 12.0,  8.0 ),
             point3(  9.0,  5.0 ),
             point4(  5.0,  6.0 );

   PolygonTestMesh mesh;
   MeshBuilder< PolygonTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 5 );
   meshBuilder.setPoint( 0, point0 );
   meshBuilder.setPoint( 1, point1 );
   meshBuilder.setPoint( 2, point2 );
   meshBuilder.setPoint( 3, point3 );
   meshBuilder.setPoint( 4, point4 );

   meshBuilder.setCellsCount( 1 );

   meshBuilder.getCellSeed( 0 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   EXPECT_NEAR( getEntityMeasure( mesh, mesh.template getEntity< 2 >( 0 ) ),
                30.0,
                1e-6 );
}

TEST( MeshGeometryTest, Polygon3DAreaTest )
{
   using PolygonTestMesh = Mesh< TestPolygon3DMeshConfig >;
   using PolygonMeshEntityType = MeshEntity< TestPolygon3DMeshConfig, Devices::Host, Topologies::Polygon >;
   using VertexMeshEntityType = typename PolygonMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   // Set up a non-convex 3D polygon, that is not aligned to any of the axis.

   /*
   PointType point0(  3.0,  4.0, 1.0 ),
             point1(  5.0, 11.0, 1.0 ),
             point2( 12.0,  8.0, 1.0 ),
             point3(  9.0,  5.0, 1.0 ),
             point4(  5.0,  6.0, 1.0 );*/

   // Above points rotated:
   PointType point0(  1.25245,  4.16176, 2.66667 ),
             point1(  0.08280, 10.41230, 6.21105 ),
             point2(  7.16272, 11.94879, 3.86289 ),
             point3(  5.86983,  8.12036, 2.56999 ),
             point4(  2.11438,  6.71405, 3.52860 );

   PolygonTestMesh mesh;
   MeshBuilder< PolygonTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 5 );
   meshBuilder.setPoint(  0, point0  );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );

   meshBuilder.setCellsCount( 1 );
   meshBuilder.getCellSeed( 0 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   EXPECT_NEAR( getEntityMeasure( mesh, mesh.template getEntity< 2 >( 0 ) ),
                30.0,
                1e-4 );
}

TEST( MeshGeometryTest, WedgeAreaTest )
{
   using WedgeTestMesh = Mesh< TestWedgeMeshConfig >;
   using WedgeMeshEntityType = MeshEntity< TestWedgeMeshConfig, Devices::Host, Topologies::Wedge >;
   using VertexMeshEntityType = typename WedgeMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   PointType point0( 10.0, 10.0, 10.0 ),
             point1( 14.0, 10.0, 10.0 ),
             point2( 12.0, 10.0, 16.0 ),
             point3( 10.0, 19.0, 10.0 ),
             point4( 14.0, 19.0, 10.0 ),
             point5( 12.0, 19.0, 16.0 );

   WedgeTestMesh mesh;
   MeshBuilder< WedgeTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 6 );
   meshBuilder.setPoint(  0, point0  );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );
   meshBuilder.setPoint(  5, point5  );

   meshBuilder.setCellsCount( 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 5, 5 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   EXPECT_NEAR( getEntityMeasure( mesh, mesh.template getEntity< 3 >( 0 ) ),
                108.0,
                1e-6 );
}

TEST( MeshGeometryTest, PyramidAreaTest )
{
   using PyramidTestMesh = Mesh< TestPyramidMeshConfig >;
   using PyramidMeshEntityType = MeshEntity< TestPyramidMeshConfig, Devices::Host, Topologies::Pyramid >;
   using VertexMeshEntityType = typename PyramidMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   // Set up a pyramid of height 10 with a square base of size 10.

   PointType point0( 10.0, 10.0, 10.0 ),
             point1( 20.0, 10.0, 10.0 ),
             point2( 20.0, 20.0, 10.0 ),
             point3( 10.0, 20.0, 10.0 ),
             point4( 15.0, 15.0, 20.0 );

   PyramidTestMesh mesh;
   MeshBuilder< PyramidTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 5 );
   meshBuilder.setPoint(  0, point0  );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );

   meshBuilder.setCellsCount( 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   EXPECT_NEAR( getEntityMeasure( mesh, mesh.template getEntity< 3 >( 0 ) ),
                1000.0 / 3.0,
                1e-6 );
}

TEST( MeshGeometryTest, PolyhedronAreaTest )
{
   using PolyhedronTestMesh = Mesh< TestPolyhedronMeshConfig >;
   using PolyhedronMeshEntityType = MeshEntity< TestPolyhedronMeshConfig, Devices::Host, Topologies::Polyhedron >;
   using VertexMeshEntityType = typename PolyhedronMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   // Set up a cube of size 10 with a pyramid of height 10 sitting on top.

   PointType point0( 10.0, 10.0, 10.0 ),
             point1( 20.0, 10.0, 10.0 ),
             point2( 10.0, 20.0, 10.0 ),
             point3( 20.0, 20.0, 10.0 ),
             point4( 10.0, 10.0, 20.0 ),
             point5( 20.0, 10.0, 20.0 ),
             point6( 10.0, 20.0, 20.0 ),
             point7( 20.0, 20.0, 20.0 ),
             point8( 15.0, 15.0, 30.0 );

   PolyhedronTestMesh mesh;
   MeshBuilder< PolyhedronTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 9 );
   meshBuilder.setPoint(  0, point0 );
   meshBuilder.setPoint(  1, point1 );
   meshBuilder.setPoint(  2, point2 );
   meshBuilder.setPoint(  3, point3 );
   meshBuilder.setPoint(  4, point4 );
   meshBuilder.setPoint(  5, point5 );
   meshBuilder.setPoint(  6, point6 );
   meshBuilder.setPoint(  7, point7 );
   meshBuilder.setPoint(  8, point8 );

   meshBuilder.setFacesCount( 9 );
   meshBuilder.setFaceCornersCount( 0, 4 );
   meshBuilder.setFaceCornersCount( 1, 4 );
   meshBuilder.setFaceCornersCount( 2, 4 );
   meshBuilder.setFaceCornersCount( 3, 4 );
   meshBuilder.setFaceCornersCount( 4, 4 );
   meshBuilder.setFaceCornersCount( 5, 3 );
   meshBuilder.setFaceCornersCount( 6, 3 );
   meshBuilder.setFaceCornersCount( 7, 3 );
   meshBuilder.setFaceCornersCount( 8, 3 );
   meshBuilder.initializeFaceSeeds();

   meshBuilder.getFaceSeed( 0 ).setCornerId( 0, 2 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 1, 3 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 2, 1 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 3, 0 );

   meshBuilder.getFaceSeed( 1 ).setCornerId( 0, 1 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 1, 3 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 2, 7 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 3, 5 );

   meshBuilder.getFaceSeed( 2 ).setCornerId( 0, 2 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 1, 0 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 2, 4 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 3, 6 );

   meshBuilder.getFaceSeed( 3 ).setCornerId( 0, 0 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 1, 1 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 2, 5 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 3, 4 );

   meshBuilder.getFaceSeed( 4 ).setCornerId( 0, 3 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 1, 2 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 2, 6 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 3, 7 );

   meshBuilder.getFaceSeed( 5 ).setCornerId( 0, 4 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 1, 5 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 2, 8 );

   meshBuilder.getFaceSeed( 6 ).setCornerId( 0, 5 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 2, 8 );

   meshBuilder.getFaceSeed( 7 ).setCornerId( 0, 7 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 1, 6 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 2, 8 );

   meshBuilder.getFaceSeed( 8 ).setCornerId( 0, 6 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 1, 4 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 2, 8 );

   meshBuilder.setCellsCount( 1 );
   meshBuilder.getCellSeed( 0 ).setCornersCount( 9 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 5, 5 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 6, 6 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 7, 7 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 8, 8 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   EXPECT_NEAR( getEntityMeasure( mesh, mesh.template getEntity< 3 >( 0 ) ),
                1000.0 + ( 1000.0 / 3.0 ),
                1e-6 );
}

TEST( MeshGeometryTest, Polygon3DIsPlanarTest )
{
   using PolygonTestMesh = Mesh< TestPolygon3DMeshConfig >;
   using PolygonMeshEntityType = MeshEntity< TestPolygon3DMeshConfig, Devices::Host, Topologies::Polygon >;
   using VertexMeshEntityType = typename PolygonMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;
   using RealType = typename PolygonTestMesh::RealType;

   const PointType offset( 0.100, 0.125, 0.150 );

   // Set up 1 planar and 5 non-planar non-convex 3D polygons.

   /*
   PointType point0(  3.0,  4.0, 1.0 ),
             point1(  5.0, 11.0, 1.0 ),
             point2( 12.0,  8.0, 1.0 ),
             point3(  9.0,  5.0, 1.0 ),
             point4(  5.0,  6.0, 1.0 );*/

   // Above points rotated:
   PointType point0(  1.25245,  4.16176, 2.66667 ),
             point1(  0.08280, 10.41230, 6.21105 ),
             point2(  7.16272, 11.94879, 3.86289 ),
             point3(  5.86983,  8.12036, 2.56999 ),
             point4(  2.11438,  6.71405, 3.52860 );

   // Previous 5 points deviated by offset:
   PointType point0_( point0 + offset ),
             point1_( point1 + offset ),
             point2_( point2 + offset ),
             point3_( point3 + offset ),
             point4_( point4 + offset );

   PolygonTestMesh mesh;
   MeshBuilder< PolygonTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 10 );
   meshBuilder.setPoint(  0, point0  );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );
   meshBuilder.setPoint(  5, point0_ );
   meshBuilder.setPoint(  6, point1_ );
   meshBuilder.setPoint(  7, point2_ );
   meshBuilder.setPoint(  8, point3_ );
   meshBuilder.setPoint(  9, point4_ );

   meshBuilder.setCellsCount( 6 );

   // Planar cell with non-deviated points
   meshBuilder.getCellSeed( 0 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );

   // Non-Planar cell with 0th point deviated
   meshBuilder.getCellSeed( 1 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 0, 5 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 4, 4 );

   // Non-Planar cell with 1th point deviated
   meshBuilder.getCellSeed( 2 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 1, 6 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 4, 4 );

   // Non-Planar cell with 2th point deviated
   meshBuilder.getCellSeed( 3 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 2, 7 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 4, 4 );

   // Non-Planar cell with 3th point deviated
   meshBuilder.getCellSeed( 4 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 3, 8 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 4, 4 );

   // Non-Planar cell with 4th point deviated
   meshBuilder.getCellSeed( 5 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 4, 9 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   EXPECT_EQ( isPlanar( mesh, mesh.template getEntity< 2 >( 0 ), 1e-4 ), true );
   EXPECT_EQ( isPlanar( mesh, mesh.template getEntity< 2 >( 1 ), 1e-4 ), false );
   EXPECT_EQ( isPlanar( mesh, mesh.template getEntity< 2 >( 2 ), 1e-4 ), false );
   EXPECT_EQ( isPlanar( mesh, mesh.template getEntity< 2 >( 3 ), 1e-4 ), false );
   EXPECT_EQ( isPlanar( mesh, mesh.template getEntity< 2 >( 4 ), 1e-4 ), false );
   EXPECT_EQ( isPlanar( mesh, mesh.template getEntity< 2 >( 5 ), 1e-4 ), false );
}

TEST( MeshGeometryTest, PolygonDecompositionTest )
{
   using PolygonTestMesh = Mesh< TestPolygon2DMeshConfig >;
   using PolygonMeshEntityType = MeshEntity< TestPolygon2DMeshConfig, Devices::Host, Topologies::Polygon >;
   using VertexMeshEntityType = typename PolygonMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   PolygonTestMesh mesh;
   MeshBuilder< PolygonTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 16 );
   meshBuilder.setPoint(  0, PointType( 0.250, 0.150 ) );
   meshBuilder.setPoint(  1, PointType( 0.150, 0.250 ) );
   meshBuilder.setPoint(  2, PointType( 0.900, 0.500 ) );
   meshBuilder.setPoint(  3, PointType( 0.750, 0.275 ) );
   meshBuilder.setPoint(  4, PointType( 0.500, 0.900 ) );
   meshBuilder.setPoint(  5, PointType( 0.275, 0.750 ) );
   meshBuilder.setPoint(  6, PointType( 0.000, 0.250 ) );
   meshBuilder.setPoint(  7, PointType( 0.250, 0.000 ) );
   meshBuilder.setPoint(  8, PointType( 0.000, 0.000 ) );
   meshBuilder.setPoint(  9, PointType( 0.750, 0.000 ) );
   meshBuilder.setPoint( 10, PointType( 0.000, 0.750 ) );
   meshBuilder.setPoint( 11, PointType( 1.000, 0.500 ) );
   meshBuilder.setPoint( 12, PointType( 1.000, 0.000 ) );
   meshBuilder.setPoint( 13, PointType( 0.500, 1.000 ) );
   meshBuilder.setPoint( 14, PointType( 1.000, 1.000 ) );
   meshBuilder.setPoint( 15, PointType( 0.000, 1.000 ) );

   /****
    * Setup the following polygons:
    *
    *   1     0     3     2     4     5
    *   8     7     0     1     6
    *   9     3     0     7
    *   6     1     5    10
    *  12    11     2     3     9
    *  13     4     2    11    14
    *  10     5     4    13    15
    */

   meshBuilder.setCellsCount( 7 );

   //   1     0     3     2     4     5
   meshBuilder.getCellSeed( 0 ).setCornersCount( 6 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0,  1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1,  0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2,  3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3,  2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4,  4 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 5,  5 );

   //   8     7     0     1     6
   meshBuilder.getCellSeed( 1 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 0,  8 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 1,  7 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 2,  0 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 3,  1 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 4,  6 );

   //   9     3     0     7
   meshBuilder.getCellSeed( 2 ).setCornersCount( 4 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 0,  9 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 1,  3 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 2,  0 );
   meshBuilder.getCellSeed( 2 ).setCornerId( 3,  7 );

   //   6     1     5    10
   meshBuilder.getCellSeed( 3 ).setCornersCount( 4 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 0,  6 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 1,  1 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 2,  5 );
   meshBuilder.getCellSeed( 3 ).setCornerId( 3, 10 );

   //  12    11     2     3     9
   meshBuilder.getCellSeed( 4 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 0, 12 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 1, 11 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 2,  2 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 3,  3 );
   meshBuilder.getCellSeed( 4 ).setCornerId( 4,  9 );

   //  13     4     2    11    14
   meshBuilder.getCellSeed( 5 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 0, 13 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 1,  4 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 2,  2 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 3, 11 );
   meshBuilder.getCellSeed( 5 ).setCornerId( 4, 14 );

   //  10     5     4    13    15
   meshBuilder.getCellSeed( 6 ).setCornersCount( 5 );
   meshBuilder.getCellSeed( 6 ).setCornerId( 0, 10 );
   meshBuilder.getCellSeed( 6 ).setCornerId( 1,  5 );
   meshBuilder.getCellSeed( 6 ).setCornerId( 2,  4 );
   meshBuilder.getCellSeed( 6 ).setCornerId( 3, 13 );
   meshBuilder.getCellSeed( 6 ).setCornerId( 4, 15 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   // Write original mesh
   {
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( mesh ) >;
      std::ofstream file( "polygon_decompositionTest_orig.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( mesh );
   }

   // Write decomposed mesh using 1st version
   {
      auto triangleMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToCentroid >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( triangleMesh ) >;
      std::ofstream file( "polygon_decompositionTest_c.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( triangleMesh );
   }

   // Write decomposed mesh using 2nd version
   {
      auto triangleMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToPoint >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( triangleMesh ) >;
      std::ofstream file( "polygon_decompositionTest_p.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( triangleMesh );
   }
}

TEST( MeshGeometryTest, PolyhedronDecompositionTest )
{
   using PolyhedronTestMesh = Mesh< TestPolyhedronMeshConfig >;
   using PolyhedronMeshEntityType = MeshEntity< TestPolyhedronMeshConfig, Devices::Host, Topologies::Polyhedron >;
   using VertexMeshEntityType = typename PolyhedronMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   PointType point0 ( -1.25000, 1.16650, 1.20300 ),
             point1 ( -1.20683, 1.16951, 1.20537 ),
             point2 ( -1.16843, 1.19337, 1.17878 ),
             point3 ( -1.21025, 1.21901, 1.15383 ),
             point4 ( -1.25000, 1.21280, 1.15670 ),
             point5 ( -1.20816, 1.25000, 1.16756 ),
             point6 ( -1.25000, 1.25000, 1.18056 ),
             point7 ( -1.14802, 1.21553, 1.21165 ),
             point8 ( -1.16186, 1.25000, 1.21385 ),
             point9 ( -1.20307, 1.17486, 1.25000 ),
             point10( -1.25000, 1.18056, 1.25000 ),
             point11( -1.15677, 1.22115, 1.25000 ),
             point12( -1.18056, 1.25000, 1.25000 ),
             point13( -1.25000, 1.25000, 1.25000 ),
             point14( -1.09277, 1.20806, 1.19263 ),
             point15( -1.07219, 1.22167, 1.17994 ),
             point16( -1.07215, 1.25000, 1.18679 ),
             point17( -1.05697, 1.21124, 1.19697 ),
             point18( -1.04607, 1.21508, 1.22076 ),
             point19( -1.02140, 1.25000, 1.22293 ),
             point20( -1.06418, 1.22115, 1.25000 ),
             point21( -1.04167, 1.25000, 1.25000 );

   PolyhedronTestMesh mesh;
   MeshBuilder< PolyhedronTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 22 );
   meshBuilder.setPoint(  0, point0  );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );
   meshBuilder.setPoint(  5, point5  );
   meshBuilder.setPoint(  6, point6  );
   meshBuilder.setPoint(  7, point7  );
   meshBuilder.setPoint(  8, point8  );
   meshBuilder.setPoint(  9, point9  );
   meshBuilder.setPoint( 10, point10 );
   meshBuilder.setPoint( 11, point11 );
   meshBuilder.setPoint( 12, point12 );
   meshBuilder.setPoint( 13, point13 );
   meshBuilder.setPoint( 14, point14 );
   meshBuilder.setPoint( 15, point15 );
   meshBuilder.setPoint( 16, point16 );
   meshBuilder.setPoint( 17, point17 );
   meshBuilder.setPoint( 18, point18 );
   meshBuilder.setPoint( 19, point19 );
   meshBuilder.setPoint( 20, point20 );
   meshBuilder.setPoint( 21, point21 );

   /****
    * Setup the following faces (polygons):
    *
    *   0     1     2     3     4
    *   4     3     5     6
    *   5     3     2     7     8
    *   9     1     0    10
    *  11     7     2     1     9
    *   8     7    11    12
    *  13    12    11     9    10
    *  13    10     0     4     6
    *  13     6     5     8    12
    *   8     7    14    15    16
    *  16    15    17    18    19
    *  20    18    17    14     7    11
    *  17    15    14
    *  21    19    18    20
    *  21    20    11    12
    *  12     8    16    19    21
    *
    * NOTE: indeces refer to the points
    */

   meshBuilder.setFacesCount( 16 );
   meshBuilder.setFaceCornersCount(  0, 5 );
   meshBuilder.setFaceCornersCount(  1, 4 );
   meshBuilder.setFaceCornersCount(  2, 5 );
   meshBuilder.setFaceCornersCount(  3, 4 );
   meshBuilder.setFaceCornersCount(  4, 5 );
   meshBuilder.setFaceCornersCount(  5, 4 );
   meshBuilder.setFaceCornersCount(  6, 5 );
   meshBuilder.setFaceCornersCount(  7, 5 );
   meshBuilder.setFaceCornersCount(  8, 5 );
   meshBuilder.setFaceCornersCount(  9, 5 );
   meshBuilder.setFaceCornersCount( 10, 5 );
   meshBuilder.setFaceCornersCount( 11, 6 );
   meshBuilder.setFaceCornersCount( 12, 3 );
   meshBuilder.setFaceCornersCount( 13, 4 );
   meshBuilder.setFaceCornersCount( 14, 4 );
   meshBuilder.setFaceCornersCount( 15, 5 );
   meshBuilder.initializeFaceSeeds();

   //   0     1     2     3     4
   meshBuilder.getFaceSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 4, 4 );

   //   4     3     5     6
   meshBuilder.getFaceSeed( 1 ).setCornerId( 0, 4 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 1, 3 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 2, 5 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 3, 6 );

   //   5     3     2     7     8
   meshBuilder.getFaceSeed( 2 ).setCornerId( 0, 5 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 1, 3 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 2, 2 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 3, 7 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 4, 8 );

   //   9     1     0    10
   meshBuilder.getFaceSeed( 3 ).setCornerId( 0, 9 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 1, 1 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 2, 0 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 3, 10 );

   //  11     7     2     1     9
   meshBuilder.getFaceSeed( 4 ).setCornerId( 0, 11 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 2, 2 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 3, 1 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 4, 9 );

   //   8     7    11    12
   meshBuilder.getFaceSeed( 5 ).setCornerId( 0, 8 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 2, 11 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 3, 12 );

   //  13    12    11     9    10
   meshBuilder.getFaceSeed( 6 ).setCornerId( 0, 13 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 1, 12 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 2, 11 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 3, 9 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 4, 10 );

   //  13    10     0     4     6
   meshBuilder.getFaceSeed( 7 ).setCornerId( 0, 13 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 1, 10 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 2, 0 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 3, 4 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 4, 6 );

   //  13     6     5     8    12
   meshBuilder.getFaceSeed( 8 ).setCornerId( 0, 13 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 1, 6 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 2, 5 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 3, 8 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 4, 12 );

   //   8     7    14    15    16
   meshBuilder.getFaceSeed( 9 ).setCornerId( 0, 8 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 2, 14 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 3, 15 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 4, 16 );

   //  16    15    17    18    19
   meshBuilder.getFaceSeed( 10 ).setCornerId( 0, 16 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 1, 15 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 2, 17 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 3, 18 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 4, 19 );

   //  20    18    17    14     7    11
   meshBuilder.getFaceSeed( 11 ).setCornerId( 0, 20 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 1, 18 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 2, 17 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 3, 14 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 4, 7 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 5, 11 );

   //  17    15    14
   meshBuilder.getFaceSeed( 12 ).setCornerId( 0, 17 );
   meshBuilder.getFaceSeed( 12 ).setCornerId( 1, 15 );
   meshBuilder.getFaceSeed( 12 ).setCornerId( 2, 14 );

   //  21    19    18    20
   meshBuilder.getFaceSeed( 13 ).setCornerId( 0, 21 );
   meshBuilder.getFaceSeed( 13 ).setCornerId( 1, 19 );
   meshBuilder.getFaceSeed( 13 ).setCornerId( 2, 18 );
   meshBuilder.getFaceSeed( 13 ).setCornerId( 3, 20 );

   //  21    20    11    12
   meshBuilder.getFaceSeed( 14 ).setCornerId( 0, 21 );
   meshBuilder.getFaceSeed( 14 ).setCornerId( 1, 20 );
   meshBuilder.getFaceSeed( 14 ).setCornerId( 2, 11 );
   meshBuilder.getFaceSeed( 14 ).setCornerId( 3, 12 );

   //  12     8    16    19    21
   meshBuilder.getFaceSeed( 15 ).setCornerId( 0, 12 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 1, 8 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 2, 16 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 3, 19 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 4, 21 );

   /****
    * Setup the following cells (polyhedrons):
    *
    *   0     1     2     3     4     5      6     7     8
    *   9    10    11    12    13     5     14    15
    *
    * NOTE: indeces refer to the faces
    */

   meshBuilder.setCellsCount( 2 );

   //   0     1     2     3     4     5      6     7     8
   meshBuilder.getCellSeed( 0 ).setCornersCount( 9 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 5, 5 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 6, 6 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 7, 7 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 8, 8 );

   //   9    10    11    12    13     5     14    15
   meshBuilder.getCellSeed( 1 ).setCornersCount( 8 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 0, 9 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 1, 10 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 2, 11 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 3, 12 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 4, 13 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 5, 5 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 6, 14 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 7, 15 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   // Write decomposed mesh using 1st version
   {
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToCentroid,
                                                EntityDecomposerVersion::ConnectEdgesToCentroid >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_decompositionTest_cc.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }

   // Write decomposed mesh using 2nd version
   {
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToCentroid,
                                                EntityDecomposerVersion::ConnectEdgesToPoint >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_decompositionTest_cp.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }

   // Write decomposed mesh using 3rd version
   {
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToPoint,
                                                EntityDecomposerVersion::ConnectEdgesToCentroid >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_decompositionTest_pc.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }

   // Write decomposed mesh using 4th version
   {
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToPoint,
                                                EntityDecomposerVersion::ConnectEdgesToPoint >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_decompositionTest_pp.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }
}

TEST( MeshGeometryTest, Polygon3DGetPlanarMeshTest )
{
   using PolygonTestMesh = Mesh< TestPolygon3DMeshConfig >;
   using PolygonMeshEntityType = MeshEntity< TestPolygon3DMeshConfig, Devices::Host, Topologies::Polygon >;
   using VertexMeshEntityType = typename PolygonMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;
   using RealType = typename PolygonTestMesh::RealType;

   const PointType offset( 0.100, 0.125, 0.150 );

   // Set up 1 planar and 1 non-planar polygons (quads)

   PointType point0(  0.0, 0.0, 0.0 ),
             point1(  1.0, 0.0, 0.0 ),
             point2(  1.0, 1.0, 0.0 ),
             point3(  0.0, 1.0, 0.0 ),
             point4(  2.0, 0.0, 0.0 ),
             point5(  2.0, 1.0, 0.0 );

   // 5th point deviated by offset:
   point5 += offset;

   PolygonTestMesh mesh;
   MeshBuilder< PolygonTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 6 );
   meshBuilder.setPoint(  0, point0 );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );
   meshBuilder.setPoint(  5, point5  );

   meshBuilder.setCellsCount( 2 );

   // Planar cell
   meshBuilder.getCellSeed( 0 ).setCornersCount( 4 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );

   meshBuilder.getCellSeed( 1 ).setCornersCount( 4 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 0, 1 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 1, 4 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 2, 5 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 3, 2 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   // Write original mesh
   {
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( mesh ) >;
      std::ofstream file( "polygon_planarTest_orig.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( mesh );
   }

   // Write decomposed mesh using 1st version
   {
      auto planarMesh = getPlanarMesh< EntityDecomposerVersion::ConnectEdgesToCentroid >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( planarMesh ) >;
      std::ofstream file( "polygon_planarTest_c.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( planarMesh );
   }

   // Write decomposed mesh using 2nd version
   {
      auto planarMesh = getPlanarMesh< EntityDecomposerVersion::ConnectEdgesToPoint >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( planarMesh ) >;
      std::ofstream file( "polygon_planarTest_p.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( planarMesh );
   }
}

TEST( MeshGeometryTest, PolyhedronGetPlanarMeshTest )
{
   using PolyhedronTestMesh = Mesh< TestPolyhedronMeshConfig >;
   using PolyhedronMeshEntityType = MeshEntity< TestPolyhedronMeshConfig, Devices::Host, Topologies::Polyhedron >;
   using VertexMeshEntityType = typename PolyhedronMeshEntityType::SubentityTraits< 0 >::SubentityType;
   using PointType = typename VertexMeshEntityType::PointType;

   PointType point0 ( -1.25000, 1.16650, 1.20300 ),
             point1 ( -1.20683, 1.16951, 1.20537 ),
             point2 ( -1.16843, 1.19337, 1.17878 ),
             point3 ( -1.21025, 1.21901, 1.15383 ),
             point4 ( -1.25000, 1.21280, 1.15670 ),
             point5 ( -1.20816, 1.25000, 1.16756 ),
             point6 ( -1.25000, 1.25000, 1.18056 ),
             point7 ( -1.14802, 1.21553, 1.21165 ),
             point8 ( -1.16186, 1.25000, 1.21385 ),
             point9 ( -1.20307, 1.17486, 1.25000 ),
             point10( -1.25000, 1.18056, 1.25000 ),
             point11( -1.15677, 1.22115, 1.25000 ),
             point12( -1.18056, 1.25000, 1.25000 ),
             point13( -1.25000, 1.25000, 1.25000 ),
             point14( -1.09277, 1.20806, 1.19263 ),
             point15( -1.07219, 1.22167, 1.17994 ),
             point16( -1.07215, 1.25000, 1.18679 ),
             point17( -1.05697, 1.21124, 1.19697 ),
             point18( -1.04607, 1.21508, 1.22076 ),
             point19( -1.02140, 1.25000, 1.22293 ),
             point20( -1.06418, 1.22115, 1.25000 ),
             point21( -1.04167, 1.25000, 1.25000 );

   PolyhedronTestMesh mesh;
   MeshBuilder< PolyhedronTestMesh > meshBuilder;

   meshBuilder.setPointsCount( 22 );
   meshBuilder.setPoint(  0, point0  );
   meshBuilder.setPoint(  1, point1  );
   meshBuilder.setPoint(  2, point2  );
   meshBuilder.setPoint(  3, point3  );
   meshBuilder.setPoint(  4, point4  );
   meshBuilder.setPoint(  5, point5  );
   meshBuilder.setPoint(  6, point6  );
   meshBuilder.setPoint(  7, point7  );
   meshBuilder.setPoint(  8, point8  );
   meshBuilder.setPoint(  9, point9  );
   meshBuilder.setPoint( 10, point10 );
   meshBuilder.setPoint( 11, point11 );
   meshBuilder.setPoint( 12, point12 );
   meshBuilder.setPoint( 13, point13 );
   meshBuilder.setPoint( 14, point14 );
   meshBuilder.setPoint( 15, point15 );
   meshBuilder.setPoint( 16, point16 );
   meshBuilder.setPoint( 17, point17 );
   meshBuilder.setPoint( 18, point18 );
   meshBuilder.setPoint( 19, point19 );
   meshBuilder.setPoint( 20, point20 );
   meshBuilder.setPoint( 21, point21 );

   /****
    * Setup the following faces (polygons):
    *
    *   0     1     2     3     4
    *   4     3     5     6
    *   5     3     2     7     8
    *   9     1     0    10
    *  11     7     2     1     9
    *   8     7    11    12
    *  13    12    11     9    10
    *  13    10     0     4     6
    *  13     6     5     8    12
    *   8     7    14    15    16
    *  16    15    17    18    19
    *  20    18    17    14     7    11
    *  17    15    14
    *  21    19    18    20
    *  21    20    11    12
    *  12     8    16    19    21
    *
    * NOTE: indeces refer to the points
    */

   meshBuilder.setFacesCount( 16 );
   meshBuilder.setFaceCornersCount(  0, 5 );
   meshBuilder.setFaceCornersCount(  1, 4 );
   meshBuilder.setFaceCornersCount(  2, 5 );
   meshBuilder.setFaceCornersCount(  3, 4 );
   meshBuilder.setFaceCornersCount(  4, 5 );
   meshBuilder.setFaceCornersCount(  5, 4 );
   meshBuilder.setFaceCornersCount(  6, 5 );
   meshBuilder.setFaceCornersCount(  7, 5 );
   meshBuilder.setFaceCornersCount(  8, 5 );
   meshBuilder.setFaceCornersCount(  9, 5 );
   meshBuilder.setFaceCornersCount( 10, 5 );
   meshBuilder.setFaceCornersCount( 11, 6 );
   meshBuilder.setFaceCornersCount( 12, 3 );
   meshBuilder.setFaceCornersCount( 13, 4 );
   meshBuilder.setFaceCornersCount( 14, 4 );
   meshBuilder.setFaceCornersCount( 15, 5 );
   meshBuilder.initializeFaceSeeds();

   //   0     1     2     3     4
   meshBuilder.getFaceSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getFaceSeed( 0 ).setCornerId( 4, 4 );

   //   4     3     5     6
   meshBuilder.getFaceSeed( 1 ).setCornerId( 0, 4 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 1, 3 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 2, 5 );
   meshBuilder.getFaceSeed( 1 ).setCornerId( 3, 6 );

   //   5     3     2     7     8
   meshBuilder.getFaceSeed( 2 ).setCornerId( 0, 5 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 1, 3 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 2, 2 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 3, 7 );
   meshBuilder.getFaceSeed( 2 ).setCornerId( 4, 8 );

   //   9     1     0    10
   meshBuilder.getFaceSeed( 3 ).setCornerId( 0, 9 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 1, 1 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 2, 0 );
   meshBuilder.getFaceSeed( 3 ).setCornerId( 3, 10 );

   //  11     7     2     1     9
   meshBuilder.getFaceSeed( 4 ).setCornerId( 0, 11 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 2, 2 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 3, 1 );
   meshBuilder.getFaceSeed( 4 ).setCornerId( 4, 9 );

   //   8     7    11    12
   meshBuilder.getFaceSeed( 5 ).setCornerId( 0, 8 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 2, 11 );
   meshBuilder.getFaceSeed( 5 ).setCornerId( 3, 12 );

   //  13    12    11     9    10
   meshBuilder.getFaceSeed( 6 ).setCornerId( 0, 13 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 1, 12 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 2, 11 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 3, 9 );
   meshBuilder.getFaceSeed( 6 ).setCornerId( 4, 10 );

   //  13    10     0     4     6
   meshBuilder.getFaceSeed( 7 ).setCornerId( 0, 13 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 1, 10 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 2, 0 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 3, 4 );
   meshBuilder.getFaceSeed( 7 ).setCornerId( 4, 6 );

   //  13     6     5     8    12
   meshBuilder.getFaceSeed( 8 ).setCornerId( 0, 13 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 1, 6 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 2, 5 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 3, 8 );
   meshBuilder.getFaceSeed( 8 ).setCornerId( 4, 12 );

   //   8     7    14    15    16
   meshBuilder.getFaceSeed( 9 ).setCornerId( 0, 8 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 1, 7 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 2, 14 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 3, 15 );
   meshBuilder.getFaceSeed( 9 ).setCornerId( 4, 16 );

   //  16    15    17    18    19
   meshBuilder.getFaceSeed( 10 ).setCornerId( 0, 16 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 1, 15 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 2, 17 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 3, 18 );
   meshBuilder.getFaceSeed( 10 ).setCornerId( 4, 19 );

   //  20    18    17    14     7    11
   meshBuilder.getFaceSeed( 11 ).setCornerId( 0, 20 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 1, 18 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 2, 17 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 3, 14 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 4, 7 );
   meshBuilder.getFaceSeed( 11 ).setCornerId( 5, 11 );

   //  17    15    14
   meshBuilder.getFaceSeed( 12 ).setCornerId( 0, 17 );
   meshBuilder.getFaceSeed( 12 ).setCornerId( 1, 15 );
   meshBuilder.getFaceSeed( 12 ).setCornerId( 2, 14 );

   //  21    19    18    20
   meshBuilder.getFaceSeed( 13 ).setCornerId( 0, 21 );
   meshBuilder.getFaceSeed( 13 ).setCornerId( 1, 19 );
   meshBuilder.getFaceSeed( 13 ).setCornerId( 2, 18 );
   meshBuilder.getFaceSeed( 13 ).setCornerId( 3, 20 );

   //  21    20    11    12
   meshBuilder.getFaceSeed( 14 ).setCornerId( 0, 21 );
   meshBuilder.getFaceSeed( 14 ).setCornerId( 1, 20 );
   meshBuilder.getFaceSeed( 14 ).setCornerId( 2, 11 );
   meshBuilder.getFaceSeed( 14 ).setCornerId( 3, 12 );

   //  12     8    16    19    21
   meshBuilder.getFaceSeed( 15 ).setCornerId( 0, 12 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 1, 8 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 2, 16 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 3, 19 );
   meshBuilder.getFaceSeed( 15 ).setCornerId( 4, 21 );

   /****
    * Setup the following cells (polyhedrons):
    *
    *   0     1     2     3     4     5      6     7     8
    *   9    10    11    12    13     5     14    15
    *
    * NOTE: indeces refer to the faces
    */

   meshBuilder.setCellsCount( 2 );

   //   0     1     2     3     4     5      6     7     8
   meshBuilder.getCellSeed( 0 ).setCornersCount( 9 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 3, 3 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 4, 4 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 5, 5 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 6, 6 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 7, 7 );
   meshBuilder.getCellSeed( 0 ).setCornerId( 8, 8 );

   //   9    10    11    12    13     5     14    15
   meshBuilder.getCellSeed( 1 ).setCornersCount( 8 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 0, 9 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 1, 10 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 2, 11 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 3, 12 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 4, 13 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 5, 5 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 6, 14 );
   meshBuilder.getCellSeed( 1 ).setCornerId( 7, 15 );

   ASSERT_TRUE( meshBuilder.build( mesh ) );

   // Write original decomposed mesh
   {
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToCentroid,
                                                EntityDecomposerVersion::ConnectEdgesToCentroid >( mesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_planarTest_orig.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }

   // Write planar decomposed mesh using 1st version
   {
      auto planarMesh = getPlanarMesh< EntityDecomposerVersion::ConnectEdgesToCentroid >( mesh );
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToPoint,
                                                EntityDecomposerVersion::ConnectEdgesToPoint >( planarMesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_planarTest_c.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }

   // Write planar decomposed mesh using 2nd version
   {
      auto planarMesh = getPlanarMesh< EntityDecomposerVersion::ConnectEdgesToPoint >( mesh );
      auto tetrahedronMesh = getDecomposedMesh< EntityDecomposerVersion::ConnectEdgesToCentroid,
                                                EntityDecomposerVersion::ConnectEdgesToCentroid >( planarMesh );
      using VTKWriter = Meshes::Writers::VTKWriter< decltype( tetrahedronMesh ) >;
      std::ofstream file( "polyhedron_planarTest_p.vtk" );
      VTKWriter writer( file, VTK::FileFormat::ascii );
      writer.template writeEntities( tetrahedronMesh );
   }
}

} // namespace MeshTest

#endif
