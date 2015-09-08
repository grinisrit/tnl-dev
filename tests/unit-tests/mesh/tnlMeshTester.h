/***************************************************************************
                          tnlMeshTester.h  -  description
                             -------------------
    begin                : Feb 18, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TNLMESHTESTER_H_
#define TNLMESHTESTER_H_

#ifdef HAVE_CPPUNIT
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/Message.h>
#include <mesh/tnlMesh.h>
#include <mesh/tnlMeshEntity.h>
#include <mesh/config/tnlMeshConfigBase.h>
#include <mesh/topologies/tnlMeshVertexTopology.h>
#include <mesh/topologies/tnlMeshEdgeTopology.h>
#include <mesh/topologies/tnlMeshTriangleTopology.h>
#include <mesh/topologies/tnlMeshQuadrilateralTopology.h>
#include <mesh/topologies/tnlMeshTetrahedronTopology.h>
#include <mesh/topologies/tnlMeshHexahedronTopology.h>
#include <mesh/initializer/tnlMeshInitializer.h>
#include <mesh/tnlMeshBuilder.h>

class TestTriangleMeshConfig : public tnlMeshConfigBase< tnlMeshTriangleTopology >
{
   public:

      static constexpr bool entityStorage( int dimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityStorage( MeshEntity, int SubentityDimensions ) { return true; };
      //template< typename MeshEntity > static constexpr bool subentityOrientationStorage( MeshEntity, int SubentityDimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool superentityStorage( MeshEntity, int SuperentityDimensions ) { return true; };
};

class TestQuadrilateralMeshConfig : public tnlMeshConfigBase< tnlMeshQuadrilateralTopology >
{
   public:
      
      static constexpr bool entityStorage( int dimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityStorage( MeshEntity, int SubentityDimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityOrientationStorage( MeshEntity, int SubentityDimensions ) { return ( SubentityDimensions % 2 != 0 ); };
      template< typename MeshEntity > static constexpr bool superentityStorage( MeshEntity, int SuperentityDimensions ) { return true; };
};

class TestTetrahedronMeshConfig : public tnlMeshConfigBase< tnlMeshTetrahedronTopology >
{
   public:

      static constexpr bool entityStorage( int dimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityStorage( MeshEntity, int SubentityDimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityOrientationStorage( MeshEntity, int SubentityDimensions ) {  return ( SubentityDimensions % 2 != 0 ); };
      template< typename MeshEntity > static constexpr bool superentityStorage( MeshEntity, int SuperentityDimensions ) { return true; };
};

class TestHexahedronMeshConfig : public tnlMeshConfigBase< tnlMeshHexahedronTopology >
{
   public:

      static constexpr bool entityStorage( int dimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityStorage( MeshEntity, int SubentityDimensions ) { return true; };
      template< typename MeshEntity > static constexpr bool subentityOrientationStorage( MeshEntity, int SubentityDimensions ) {  return ( SubentityDimensions % 2 != 0 ); };
      template< typename MeshEntity > static constexpr bool superentityStorage( MeshEntity, int SuperentityDimensions ) { return true; };
};

template< typename RealType, typename Device, typename IndexType >
class tnlMeshTester : public CppUnit :: TestCase
{
   public:
   typedef tnlMeshTester< RealType, Device, IndexType > TesterType;
   typedef typename CppUnit::TestCaller< TesterType > TestCallerType;

   tnlMeshTester(){};

   virtual
   ~tnlMeshTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlMeshTester" );
      CppUnit :: TestResult result;

      suiteOfTests->addTest( new TestCallerType( "twoTrianglesTest", &TesterType::twoTrianglesTest ) );
      suiteOfTests->addTest( new TestCallerType( "tetrahedronsTest", &TesterType::tetrahedronsTest ) );
      suiteOfTests->addTest( new TestCallerType( "regularMeshOfTrianglesTest", &TesterType::regularMeshOfTrianglesTest ) );
      suiteOfTests->addTest( new TestCallerType( "regularMeshOfQuadrilateralsTest", &TesterType::regularMeshOfQuadrilateralsTest ) );
      suiteOfTests->addTest( new TestCallerType( "regularMeshOfHexahedronsTest", &TesterType::regularMeshOfHexahedronsTest ) );
      return suiteOfTests;
   }

   void twoTrianglesTest()
   {
       typedef tnlMeshEntity< TestTriangleMeshConfig, tnlMeshTriangleTopology > TriangleMeshEntityType;
       typedef tnlMeshEntity< TestTriangleMeshConfig, tnlMeshEdgeTopology > EdgeMeshEntityType;
       typedef tnlMeshEntity< TestTriangleMeshConfig, tnlMeshVertexTopology > VertexMeshEntityType;
       typedef typename VertexMeshEntityType::PointType PointType;
       CPPUNIT_ASSERT( PointType::getType() == ( tnlStaticVector< 2, RealType >::getType() ) );

       /****
        * We set-up the following situation
                point2   edge3       point3
                   |\-------------------|
                   | \                  |
                   |  \   triangle1     |
                   |   \                |

                          ....
                edge1     edge0        edge4
                          ....


                   |   triangle0     \  |
                   |                  \ |
                   ---------------------|
                point0   edge2        point1
        */

       typedef tnlMesh< TestTriangleMeshConfig > TriangleTestMesh;
       TriangleTestMesh mesh, mesh2;
       mesh.setName( "mesh" );
       tnlMeshBuilder< TriangleTestMesh > meshBuilder;
       meshBuilder.setPointsCount( 4 );
       meshBuilder.setPoint( 0, PointType( 0.0, 0.0 ) );
       meshBuilder.setPoint( 1, PointType( 1.0, 0.0 ) );
       meshBuilder.setPoint( 2, PointType( 0.0, 1.0 ) );
       meshBuilder.setPoint( 3, PointType( 1.0, 1.0 ) );
       
       meshBuilder.setCellsCount( 2 );
       meshBuilder.getCellSeed( 0 ).setCornerId( 0, 0 );
       meshBuilder.getCellSeed( 0 ).setCornerId( 1, 1 );
       meshBuilder.getCellSeed( 0 ).setCornerId( 2, 2 );
       meshBuilder.getCellSeed( 1 ).setCornerId( 0, 1 );
       meshBuilder.getCellSeed( 1 ).setCornerId( 1, 2 );
       meshBuilder.getCellSeed( 1 ).setCornerId( 2, 3 );
       meshBuilder.build( mesh );
      
       CPPUNIT_ASSERT( mesh.getNumberOfEntities< 2 >() == 2 );
       CPPUNIT_ASSERT( mesh.getNumberOfEntities< 1 >() == 5 );
       CPPUNIT_ASSERT( mesh.getNumberOfEntities< 0 >() == 4 );

       //CPPUNIT_ASSERT( mesh.save( "mesh.tnl" ) );
       //CPPUNIT_ASSERT( mesh2.load( "mesh.tnl" ) );
       //CPPUNIT_ASSERT( mesh == mesh2 );

       //mesh2.setName( "mesh2" );
       //mesh.print( cout );
       //mesh2.print( cout );
    };

   void tetrahedronsTest()
   {
      typedef tnlMeshEntity< TestTetrahedronMeshConfig, tnlMeshTriangleTopology > TriangleMeshEntityType;
      typedef tnlMeshEntity< TestTetrahedronMeshConfig, tnlMeshEdgeTopology > EdgeMeshEntityType;
      typedef tnlMeshEntity< TestTetrahedronMeshConfig, tnlMeshVertexTopology > VertexMeshEntityType;
      typedef typename VertexMeshEntityType::PointType PointType;
      typedef tnlMesh< TestTetrahedronMeshConfig > TestTetrahedronMesh;      
      TestTetrahedronMesh mesh;
      tnlMeshBuilder< TestTetrahedronMesh > meshBuilder;
      meshBuilder.setPointsCount( 13 );
      meshBuilder.setPoint(  0, PointType(  0.000000, 0.000000, 0.000000 ) );
      meshBuilder.setPoint(  1, PointType(  0.000000, 0.000000, 8.000000 ) );
      meshBuilder.setPoint(  2, PointType(  0.000000, 8.000000, 0.000000 ) );
      meshBuilder.setPoint(  3, PointType( 15.000000, 0.000000, 0.000000 ) );
      meshBuilder.setPoint(  4, PointType(  0.000000, 8.000000, 8.000000 ) );
      meshBuilder.setPoint(  5, PointType( 15.000000, 0.000000, 8.000000 ) );
      meshBuilder.setPoint(  6, PointType( 15.000000, 8.000000, 0.000000 ) );
      meshBuilder.setPoint(  7, PointType( 15.000000, 8.000000, 8.000000 ) );
      meshBuilder.setPoint(  8, PointType(  7.470740, 8.000000, 8.000000 ) );
      meshBuilder.setPoint(  9, PointType(  7.470740, 0.000000, 8.000000 ) );
      meshBuilder.setPoint( 10, PointType(  7.504125, 8.000000, 0.000000 ) );
      meshBuilder.setPoint( 11, PointType(  7.212720, 0.000000, 0.000000 ) );
      meshBuilder.setPoint( 12, PointType( 11.184629, 3.987667, 3.985835 ) );

      /****
       * Setup the following tetrahedrons:
       * ( Generated by Netgen )
       *
       *  12        8        7        5
       *  12        7        8       10
       *  12       11        8        9
       *  10       11        2        8
       *  12        7        6        5
       *   9       12        5        8
       *  12       11        9        3
       *   9        4       11        8
       *  12        9        5        3
       *   1        2        0       11
       *   8       11        2        4
       *   1        2       11        4
       *   9        4        1       11
       *  10       11        8       12
       *  12        6        7       10
       *  10       11       12        3
       *  12        6        3        5
       *  12        3        6       10
       */
      
      meshBuilder.setCellsCount( 18 );
       //  12        8        7        5
      meshBuilder.getCellSeed( 0 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 0 ).setCornerId( 1, 8 );
      meshBuilder.getCellSeed( 0 ).setCornerId( 2, 7 );
      meshBuilder.getCellSeed( 0 ).setCornerId( 3, 5 );

       //  12        7        8       10
      meshBuilder.getCellSeed( 1 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 1 ).setCornerId( 1, 7 );
      meshBuilder.getCellSeed( 1 ).setCornerId( 2, 8 );
      meshBuilder.getCellSeed( 1 ).setCornerId( 3, 10 );
                 
       //  12       11        8        9
      meshBuilder.getCellSeed( 2 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 2 ).setCornerId( 1, 11 );
      meshBuilder.getCellSeed( 2 ).setCornerId( 2, 8 );
      meshBuilder.getCellSeed( 2 ).setCornerId( 3, 9 );
                 
       //  10       11        2        8
      meshBuilder.getCellSeed( 3 ).setCornerId( 0, 10 );
      meshBuilder.getCellSeed( 3 ).setCornerId( 1, 11 );
      meshBuilder.getCellSeed( 3 ).setCornerId( 2, 2 );
      meshBuilder.getCellSeed( 3 ).setCornerId( 3, 8 );
                 
       //  12        7        6        5
      meshBuilder.getCellSeed( 4 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 4 ).setCornerId( 1, 7 );
      meshBuilder.getCellSeed( 4 ).setCornerId( 2, 6 );
      meshBuilder.getCellSeed( 4 ).setCornerId( 3, 5 );
                 
       //   9       12        5        8
      meshBuilder.getCellSeed( 5 ).setCornerId( 0, 9 );
      meshBuilder.getCellSeed( 5 ).setCornerId( 1, 12 );
      meshBuilder.getCellSeed( 5 ).setCornerId( 2, 5 );
      meshBuilder.getCellSeed( 5 ).setCornerId( 3, 8 );
                 
       //  12       11        9        3
      meshBuilder.getCellSeed( 6 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 6 ).setCornerId( 1, 11 );
      meshBuilder.getCellSeed( 6 ).setCornerId( 2, 9 );
      meshBuilder.getCellSeed( 6 ).setCornerId( 3, 3 );
                 
       //   9        4       11        8
      meshBuilder.getCellSeed( 7 ).setCornerId( 0, 9 );
      meshBuilder.getCellSeed( 7 ).setCornerId( 1, 4 );
      meshBuilder.getCellSeed( 7 ).setCornerId( 2, 11 );
      meshBuilder.getCellSeed( 7 ).setCornerId( 3, 8 );
                
       //  12        9        5        3
      meshBuilder.getCellSeed( 8 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 8 ).setCornerId( 1, 9 );
      meshBuilder.getCellSeed( 8 ).setCornerId( 2, 5 );
      meshBuilder.getCellSeed( 8 ).setCornerId( 3, 3 );
                 
       //   1        2        0       11
      meshBuilder.getCellSeed( 9 ).setCornerId( 0, 1 );
      meshBuilder.getCellSeed( 9 ).setCornerId( 1, 2 );
      meshBuilder.getCellSeed( 9 ).setCornerId( 2, 0 );
      meshBuilder.getCellSeed( 9 ).setCornerId( 3, 11 );
                 
       //   8       11        2        4
      meshBuilder.getCellSeed( 10 ).setCornerId( 0, 8 );
      meshBuilder.getCellSeed( 10 ).setCornerId( 1, 11 );
      meshBuilder.getCellSeed( 10 ).setCornerId( 2, 2 );
      meshBuilder.getCellSeed( 10 ).setCornerId( 3, 4 );
                 
       //   1        2       11        4
      meshBuilder.getCellSeed( 11 ).setCornerId( 0, 1 );
      meshBuilder.getCellSeed( 11 ).setCornerId( 1, 2 );
      meshBuilder.getCellSeed( 11 ).setCornerId( 2, 11 );
      meshBuilder.getCellSeed( 11 ).setCornerId( 3, 4 );
                 
       //   9        4        1       11
      meshBuilder.getCellSeed( 12 ).setCornerId( 0, 9 );
      meshBuilder.getCellSeed( 12 ).setCornerId( 1, 4 );
      meshBuilder.getCellSeed( 12 ).setCornerId( 2, 1 );
      meshBuilder.getCellSeed( 12 ).setCornerId( 3, 11 );
                 
       //  10       11        8       12
      meshBuilder.getCellSeed( 13 ).setCornerId( 0, 10 );
      meshBuilder.getCellSeed( 13 ).setCornerId( 1, 11 );
      meshBuilder.getCellSeed( 13 ).setCornerId( 2, 8 );
      meshBuilder.getCellSeed( 13 ).setCornerId( 3, 12 );
                 
       //  12        6        7       10
      meshBuilder.getCellSeed( 14 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 14 ).setCornerId( 1, 6 );
      meshBuilder.getCellSeed( 14 ).setCornerId( 2, 7 );
      meshBuilder.getCellSeed( 14 ).setCornerId( 3, 10 );
                 
       //  10       11       12        3
      meshBuilder.getCellSeed( 15 ).setCornerId( 0, 10 );
      meshBuilder.getCellSeed( 15 ).setCornerId( 1, 11 );
      meshBuilder.getCellSeed( 15 ).setCornerId( 2, 12 );
      meshBuilder.getCellSeed( 15 ).setCornerId( 3, 3 );

       //  12        6        3        5
      meshBuilder.getCellSeed( 16 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 16 ).setCornerId( 1, 6 );
      meshBuilder.getCellSeed( 16 ).setCornerId( 2, 3 );
      meshBuilder.getCellSeed( 16 ).setCornerId( 3, 5 );
                 
       //  12        3        6       10
      meshBuilder.getCellSeed( 17 ).setCornerId( 0, 12 );
      meshBuilder.getCellSeed( 17 ).setCornerId( 1, 3 );
      meshBuilder.getCellSeed( 17 ).setCornerId( 2, 6 );
      meshBuilder.getCellSeed( 17 ).setCornerId( 3, 10 );
      
      meshBuilder.build( mesh );

      /*CPPUNIT_ASSERT( mesh.save( "mesh.tnl" ) );
      CPPUNIT_ASSERT( mesh2.load( "mesh.tnl" ) );
      CPPUNIT_ASSERT( mesh == mesh2 );*/
      //mesh.print( cout );
   }

   void regularMeshOfTrianglesTest()
   {
      typedef tnlMeshEntity< TestTriangleMeshConfig, tnlMeshTriangleTopology > TriangleMeshEntityType;
      typedef tnlMeshEntity< TestTriangleMeshConfig, tnlMeshEdgeTopology > EdgeMeshEntityType;
      typedef tnlMeshEntity< TestTriangleMeshConfig, tnlMeshVertexTopology > VertexMeshEntityType;
      typedef typename VertexMeshEntityType::PointType PointType;

      const IndexType xSize( 5 ), ySize( 5 );
      const RealType width( 1.0 ), height( 1.0 );
      const RealType hx( width / ( RealType ) xSize ),
                     hy( height / ( RealType ) ySize );
      const IndexType numberOfCells = 2*xSize * ySize;
      const IndexType numberOfVertices = ( xSize + 1 ) * ( ySize + 1 );

      typedef tnlMesh< TestTriangleMeshConfig > TestTriangleMesh;
      tnlMesh< TestTriangleMeshConfig > mesh;
      tnlMeshBuilder< TestTriangleMesh > meshBuilder;      
      meshBuilder.setPointsCount( numberOfVertices );
      meshBuilder.setCellsCount( numberOfCells );

      /****
       * Setup vertices
       */
      for( IndexType i = 0; i <= xSize; i++ )
         for( IndexType j = 0; j <= ySize; j++ )
            meshBuilder.setPoint(  j*xSize + i, PointType( i * hx, j * hy ) );

      /****
       * Setup cells
       */
      IndexType cellIdx( 0 );
      for( IndexType i = 0; i < xSize; i++ )
         for( IndexType j = 0; j < ySize; j++ )
         {
            IndexType vertex0 = j * xSize + i;
            IndexType vertex1 = j * xSize + i + 1;
            IndexType vertex2 = ( j + 1 ) * xSize + i;
            IndexType vertex3 = ( j + 1 ) * xSize + i + 1;
            meshBuilder.getCellSeed( cellIdx   ).setCornerId( 0, vertex0 );
            meshBuilder.getCellSeed( cellIdx   ).setCornerId( 1, vertex1 );
            meshBuilder.getCellSeed( cellIdx++ ).setCornerId( 2, vertex2 );
            meshBuilder.getCellSeed( cellIdx   ).setCornerId( 0, vertex1 );
            meshBuilder.getCellSeed( cellIdx   ).setCornerId( 1, vertex2 );
            meshBuilder.getCellSeed( cellIdx++ ).setCornerId( 2, vertex3 );
         }

      meshBuilder.build( mesh );
      //CPPUNIT_ASSERT( mesh.save( "mesh-test.tnl" ) );
      //CPPUNIT_ASSERT( mesh2.load( "mesh-test.tnl" ) );
      //CPPUNIT_ASSERT( mesh == mesh2 );
      //mesh.print( cout );
   }

   void regularMeshOfQuadrilateralsTest()
   {
#ifdef UNDEF      
      typedef tnlMeshEntity< TestQuadrilateralMeshConfig, tnlMeshQuadrilateralTopology > QuadrilateralMeshEntityType;
      typedef tnlMeshEntity< TestQuadrilateralMeshConfig, tnlMeshEdgeTopology > EdgeMeshEntityType;
      typedef tnlMeshEntity< TestQuadrilateralMeshConfig, tnlMeshVertexTopology > VertexMeshEntityType;
      typedef typename VertexMeshEntityType::PointType PointType;

      const IndexType xSize( 5 ), ySize( 5 );
      const RealType width( 1.0 ), height( 1.0 );
      const RealType hx( width / ( RealType ) xSize ),
                     hy( height / ( RealType ) ySize );
      const IndexType numberOfCells = xSize * ySize;
      const IndexType numberOfVertices = ( xSize + 1 ) * ( ySize + 1 );

      tnlMesh< TestQuadrilateralMeshConfig > mesh, mesh2;
      mesh.setNumberOfCells( numberOfCells );
      mesh.setNumberOfVertices( numberOfVertices );

      /****
       * Setup vertices
       */
      for( IndexType i = 0; i <= xSize; i++ )
         for( IndexType j = 0; j <= ySize; j++ )
            mesh.setVertex(  j*xSize + i, PointType( i * hx, j * hy ) );

      /****
       * Setup cells
       */
      IndexType cellIdx( 0 );
      for( IndexType i = 0; i < xSize; i++ )
         for( IndexType j = 0; j < ySize; j++ )
         {
            IndexType vertex0 = j * xSize + i;
            IndexType vertex1 = j * xSize + i + 1;
            IndexType vertex2 = ( j + 1 ) * xSize + i;
            IndexType vertex3 = ( j + 1 ) * xSize + i + 1;
            mesh.getEntities< 2 >()[ cellIdx   ].getVerticesIndices()[ 0 ] = vertex0;
            mesh.getEntities< 2 >()[ cellIdx   ].getVerticesIndices()[ 1 ] = vertex1;
            mesh.getEntities< 2 >()[ cellIdx   ].getVerticesIndices()[ 2 ] = vertex2;
            mesh.getEntities< 2 >()[ cellIdx++ ].getVerticesIndices()[ 3 ] = vertex3;
         }

      tnlMeshInitializer< TestQuadrilateralMeshConfig > meshInitializer;
      //meshInitializer.initMesh( mesh );
      CPPUNIT_ASSERT( mesh.save( "mesh-test.tnl" ) );
      CPPUNIT_ASSERT( mesh2.load( "mesh-test.tnl" ) );
      CPPUNIT_ASSERT( mesh == mesh2 );
      //mesh.print( cout );
#endif      
   }

   void regularMeshOfHexahedronsTest()
   {
#ifdef UNDEF      
      typedef tnlMeshEntity< TestHexahedronMeshConfig, tnlMeshHexahedronTopology > HexahedronMeshEntityType;
      typedef tnlMeshEntity< TestHexahedronMeshConfig, tnlMeshEdgeTopology > EdgeMeshEntityType;
      typedef tnlMeshEntity< TestHexahedronMeshConfig, tnlMeshVertexTopology > VertexMeshEntityType;
      typedef typename VertexMeshEntityType::PointType PointType;

      const IndexType xSize( 5 ), ySize( 5 ), zSize( 5 );
      const RealType width( 1.0 ), height( 1.0 ), depth( 1.0 );
      const RealType hx( width / ( RealType ) xSize ),
                     hy( height / ( RealType ) ySize ),
                     hz( depth / ( RealType ) zSize );
      const IndexType numberOfCells = xSize * ySize * zSize;
      const IndexType numberOfVertices = ( xSize + 1 ) * ( ySize + 1 ) * ( zSize + 1 );

      tnlMesh< TestHexahedronMeshConfig > mesh, mesh2;
      mesh.setNumberOfCells( numberOfCells );
      mesh.setNumberOfVertices( numberOfVertices );

      /****
       * Setup vertices
       */
      for( IndexType i = 0; i <= xSize; i++ )
         for( IndexType j = 0; j <= ySize; j++ )
            for( IndexType k = 0; k <= zSize; k++ )
               mesh.setVertex(  k * xSize * ySize + j * xSize + i, PointType( i * hx, j * hy, k * hz ) );

      /****
       * Setup cells
       */
      IndexType cellIdx( 0 );
      for( IndexType i = 0; i < xSize; i++ )
         for( IndexType j = 0; j < ySize; j++ )
            for( IndexType k = 0; k < zSize; k++ )
            {
               IndexType vertex0 = k * xSize * ySize + j * xSize + i;
               IndexType vertex1 = k * xSize * ySize + j * xSize + i + 1;
               IndexType vertex2 = k * xSize * ySize + ( j + 1 ) * xSize + i;
               IndexType vertex3 = k * xSize * ySize + ( j + 1 ) * xSize + i + 1;
               IndexType vertex4 = ( k + 1 ) * xSize * ySize + j * xSize + i;
               IndexType vertex5 = ( k + 1 ) * xSize * ySize + j * xSize + i + 1;
               IndexType vertex6 = ( k + 1 ) * xSize * ySize + ( j + 1 ) * xSize + i;
               IndexType vertex7 = ( k + 1 )* xSize * ySize + ( j + 1 ) * xSize + i + 1;

               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 0 ] = vertex0;
               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 1 ] = vertex1;
               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 2 ] = vertex2;
               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 3 ] = vertex3;
               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 4 ] = vertex4;
               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 5 ] = vertex5;
               mesh.getEntities< 3 >()[ cellIdx   ].getVerticesIndices()[ 6 ] = vertex6;
               mesh.getEntities< 3 >()[ cellIdx++ ].getVerticesIndices()[ 7 ] = vertex7;
            }

      tnlMeshInitializer< TestHexahedronMeshConfig > meshInitializer;
      //meshInitializer.initMesh( mesh );
      /*CPPUNIT_ASSERT( mesh.save( "mesh-test.tnl" ) );
      CPPUNIT_ASSERT( mesh2.load( "mesh-test.tnl" ) );
      CPPUNIT_ASSERT( mesh == mesh2 );*/
      //mesh.print( cout );
#endif      
   }


};

#endif





#endif /* TNLMESHTESTER_H_ */
