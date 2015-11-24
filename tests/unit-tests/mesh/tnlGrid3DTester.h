/***************************************************************************
                          tnlGrid3DTester.h  -  description
                             -------------------
    begin                : Feb 13, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
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

#ifndef TESTS_UNIT_TESTS_MESH_TNLGRID3DTESTER_H_
#define TESTS_UNIT_TESTS_MESH_TNLGRID3DTESTER_H_

template< typename RealType, typename Device, typename IndexType >
class tnlGridTester< 3, RealType, Device, IndexType >: public CppUnit :: TestCase
{
   public:
   typedef tnlGridTester< 3, RealType, Device, IndexType > TesterType;
   typedef typename CppUnit::TestCaller< TesterType > TestCallerType;
   typedef tnlGrid< 3, RealType, Device, IndexType > GridType;
   typedef typename GridType::CoordinatesType CoordinatesType;
   typedef typename GridType::VertexType VertexType;


   tnlGridTester(){};

   virtual
   ~tnlGridTester(){};

   static CppUnit :: Test* suite()
   {
      CppUnit :: TestSuite* suiteOfTests = new CppUnit :: TestSuite( "tnlGridTester" );
      CppUnit :: TestResult result;

      suiteOfTests -> addTest( new TestCallerType( "setDomainTest", &TesterType::setDomainTest ) );
      suiteOfTests -> addTest( new TestCallerType( "cellIndexingTest", &TesterType::cellIndexingTest ) );
      suiteOfTests -> addTest( new TestCallerType( "faceIndexingTest", &TesterType::faceIndexingTest ) );
      suiteOfTests -> addTest( new TestCallerType( "edgeIndexingTest", &TesterType::edgeIndexingTest ) );
      suiteOfTests -> addTest( new TestCallerType( "vertexIndexingTest", &TesterType::vertexIndexingTest ) );
      suiteOfTests -> addTest( new TestCallerType( "getCellNextToCellTest", &TesterType::getCellNextToCellTest ) );
      suiteOfTests -> addTest( new TestCallerType( "getFaceNextToCellTest", &TesterType::getFaceNextToCellTest ) );
      suiteOfTests -> addTest( new TestCallerType( "getCellNextToFaceTest", &TesterType::getCellNextToFaceTest ) );


      return suiteOfTests;
   }

   void setDomainTest()
   {
      GridType grid;
      grid.setDomain( VertexType( 0.0, 0.0, 0.0 ), VertexType( 1.0, 1.0, 1.0 ) );
      grid.setDimensions( 10, 20, 40 );

      CPPUNIT_ASSERT( grid.getCellProportions().x() == 0.1 );
      CPPUNIT_ASSERT( grid.getCellProportions().y() == 0.05 );
      CPPUNIT_ASSERT( grid.getCellProportions().z() == 0.025 );
   }

   void cellIndexingTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
      typename GridType::template GridEntity< GridType::Cells > cell;
      for( cell.getCoordinates().z() = 0;
           cell.getCoordinates().z() < zSize;
           cell.getCoordinates().z()++ )
         for( cell.getCoordinates().y() = 0;
              cell.getCoordinates().y() < ySize;
              cell.getCoordinates().y()++ )
            for( cell.getCoordinates().x() = 0;
                 cell.getCoordinates().x() < xSize;
                 cell.getCoordinates().x()++ )
            {
               const IndexType cellIndex = grid.template getEntityIndex< GridType::Cells >( cell );
               CPPUNIT_ASSERT( cellIndex >= 0 );
               CPPUNIT_ASSERT( cellIndex < grid.template getEntitiesCount< GridType::Cells >() );
               CPPUNIT_ASSERT( grid.template getEntity< GridType::Cells >( cellIndex ).getCoordinates() == cell.getCoordinates() );
            }
   }

   void faceIndexingTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
            
      typedef typename GridType::template GridEntity< 2 > FaceType;
      typedef typename FaceType::EntityOrientationType OrientationType;
      typedef typename FaceType::EntityBasisType BasisType;
      FaceType face;
      
      face.setOrientation( OrientationType( 1, 0, 0 ) );
      for( face.getCoordinates().z() = 0;
            face.getCoordinates().z() < zSize;
            face.getCoordinates().z()++ )
         for( face.getCoordinates().y() = 0;
              face.getCoordinates().y() < ySize;
              face.getCoordinates().y()++ )
            for( face.getCoordinates().x() = 0;
                 face.getCoordinates().x() < xSize + 1;
                 face.getCoordinates().x()++ )
            {
               const IndexType faceIndex = grid.template getEntityIndex( face );
               CPPUNIT_ASSERT( faceIndex >= 0 );
               CPPUNIT_ASSERT( faceIndex < grid.template getEntitiesCount< GridType::Dimensions - 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getCoordinates() == face.getCoordinates() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getOrientation() == OrientationType( 1, 0, 0 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }

      face.setOrientation( OrientationType( 0, 1, 0 ) );
      for( face.getCoordinates().z() = 0;
            face.getCoordinates().z() < zSize;
            face.getCoordinates().z()++ )
         for( face.getCoordinates().y() = 0;
              face.getCoordinates().y() < ySize + 1;
              face.getCoordinates().y()++ )
            for( face.getCoordinates().x() = 0;
                 face.getCoordinates().x() < xSize;
                 face.getCoordinates().x()++ )
            {
               const IndexType faceIndex = grid.template getEntityIndex( face );
               CPPUNIT_ASSERT( faceIndex >= 0 );
               CPPUNIT_ASSERT( faceIndex < grid.template getEntitiesCount< GridType::Dimensions - 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getCoordinates() == face.getCoordinates() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getOrientation() == OrientationType( 0, 1, 0 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }

      face.setOrientation( OrientationType( 0, 0, 1 ) );
      for( face.getCoordinates().z() = 0;
            face.getCoordinates().z() < zSize + 1;
            face.getCoordinates().z()++ )
         for( face.getCoordinates().y() = 0;
              face.getCoordinates().y() < ySize;
              face.getCoordinates().y()++ )
            for( face.getCoordinates().x() = 0;
                 face.getCoordinates().x() < xSize;
                 face.getCoordinates().x()++ )
            {
               const IndexType faceIndex = grid.template getEntityIndex( face );
               CPPUNIT_ASSERT( faceIndex >= 0 );
               CPPUNIT_ASSERT( faceIndex < grid.template getEntitiesCount< GridType::Dimensions - 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getCoordinates() == face.getCoordinates() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getOrientation() == OrientationType( 0, 0, 1 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }


   }

   void edgeIndexingTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
      
      typedef typename GridType::template GridEntity< 1 > EdgeType;
      typedef typename EdgeType::EntityOrientationType OrientationType;
      typedef typename EdgeType::EntityBasisType BasisType;
      EdgeType edge;
      
      edge.setBasis( OrientationType( 1, 0, 0 ) );
      for( edge.getCoordinates().z() = 0;
           edge.getCoordinates().z() < zSize + 1;
           edge.getCoordinates().z()++ )
         for( edge.getCoordinates().y() = 0;
              edge.getCoordinates().y() < ySize + 1;
              edge.getCoordinates().y()++ )
            for( edge.getCoordinates().x() = 0;
                 edge.getCoordinates().x() < xSize;
                 edge.getCoordinates().x()++ )
            {
               const IndexType edgeIndex = grid.template getEntityIndex( edge );
               CPPUNIT_ASSERT( edgeIndex >= 0 );
               CPPUNIT_ASSERT( edgeIndex < grid.template getEntitiesCount< 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getCoordinates() == edge.getCoordinates() );
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getOrientation() == OrientationType( 1, 0, 0 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }
      
      edge.setBasis( OrientationType( 0, 1, 0 ) );
      for( edge.getCoordinates().z() = 0;
           edge.getCoordinates().z() < zSize + 1;
           edge.getCoordinates().z()++ )
         for( edge.getCoordinates().y() = 0;
              edge.getCoordinates().y() < ySize;
              edge.getCoordinates().y()++ )
            for( edge.getCoordinates().x() = 0;
                 edge.getCoordinates().x() < xSize + 1;
                 edge.getCoordinates().x()++ )
            {
               const IndexType edgeIndex = grid.template getEntityIndex( edge );
               CPPUNIT_ASSERT( edgeIndex >= 0 );
               CPPUNIT_ASSERT( edgeIndex < grid.template getEntitiesCount< 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getCoordinates() == edge.getCoordinates() );
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getOrientation() == OrientationType( 1, 0, 0 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }

      edge.setBasis( OrientationType( 0, 0, 1 ) );
      for( edge.getCoordinates().z() = 0;
           edge.getCoordinates().z() < zSize;
           edge.getCoordinates().z()++ )
         for( edge.getCoordinates().y() = 0;
              edge.getCoordinates().y() < ySize + 1;
              edge.getCoordinates().y()++ )
            for( edge.getCoordinates().x() = 0;
                 edge.getCoordinates().x() < xSize + 1;
                 edge.getCoordinates().x()++ )
            {
               const IndexType edgeIndex = grid.template getEntityIndex( edge );
               CPPUNIT_ASSERT( edgeIndex >= 0 );
               CPPUNIT_ASSERT( edgeIndex < grid.template getEntitiesCount< 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getCoordinates() == edge.getCoordinates() );
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getOrientation() == OrientationType( 1, 0, 0 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }      
   }

   void vertexIndexingTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
      
      typedef typename GridType::template GridEntity< 0 > VertexType;
      typedef typename VertexType::EntityOrientationType OrientationType;
      typedef typename VertexType::EntityBasisType BasisType;
      VertexType vertex;
      
      for( vertex.getCoordinates().z() = 0;
           vertex.getCoordinates().z() < zSize + 1;
           vertex.getCoordinates().z()++ )
         for( vertex.getCoordinates().y() = 0;
              vertex.getCoordinates().y() < ySize + 1;
              vertex.getCoordinates().y()++ )
            for( vertex.getCoordinates().x() = 0;
                 vertex.getCoordinates().x() < xSize + 1;
                 vertex.getCoordinates().x()++ )
            {
               const IndexType vertexIndex = grid.getEntityIndex( vertex );
               CPPUNIT_ASSERT( vertexIndex >= 0 );
               CPPUNIT_ASSERT( vertexIndex < grid.template getEntitiesCount< 1 >() );
               CPPUNIT_ASSERT( grid.template getEntity< 1 >( vertexIndex ).getCoordinates() == vertex.getCoordinates() );
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( edgeIndex ).getOrientation() == OrientationType( 1, 0, 0 ) );
               // TODO: fix this - gives undefined reference - I do not know why
               //CPPUNIT_ASSERT( grid.template getEntity< 1 >( faceIndex ).getBasis() == BasisType( 0, 1 ) );
            }
   }

   void getCellNextToCellTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
      
      typedef typename GridType::template GridEntity< GridType::Cells > CellType;
      CellType cell;
      for( cell.getCoordinates().z() = 0;
           cell.getCoordinates().z() < zSize;
           cell.getCoordinates().z()++ )
         for( cell.getCoordinates().y() = 0;
              cell.getCoordinates().y() < ySize;
              cell.getCoordinates().y()++ )
            for( cell.getCoordinates().x() = 0;
                 cell.getCoordinates().x() < xSize;
                 cell.getCoordinates().x()++ )
            {
               const IndexType cellIndex = grid.getEntityIndex( cell );
               if( cell.getCoordinates().x() > 0 )
               {
                  CellType auxCell( cell.getCoordinates() + CoordinatesType( -1, 0, 0 ) );
                  const IndexType auxCellIndex = grid.getEntityIndex( auxCell );
                  CPPUNIT_ASSERT( ( auxCellIndex == grid.template getCellNextToCell< -1, 0, 0 >( cellIndex ) ) );
               }
               if( cell.getCoordinates().x() < xSize - 1 )
               {
                  CellType auxCell( cell.getCoordinates() + CoordinatesType( 1, 0, 0 ) );
                  const IndexType auxCellIndex = grid.getEntityIndex( auxCell );
                  CPPUNIT_ASSERT( ( auxCellIndex == grid.template getCellNextToCell< 1, 0, 0 >( cellIndex ) ) );
               }
               if( cell.getCoordinates().y() > 0 )
               {
                  CellType auxCell( cell.getCoordinates() + CoordinatesType( 0, -1, 0 ) );
                  const IndexType auxCellIndex = grid.getEntityIndex( auxCell );
                  CPPUNIT_ASSERT( ( auxCellIndex == grid.template getCellNextToCell< 0, -1, 0 >( cellIndex ) ) );
               }
               if( cell.getCoordinates().y() < ySize - 1 )
               {
                  CellType auxCell( cell.getCoordinates() + CoordinatesType( 0, 1, 0 ) );
                  const IndexType auxCellIndex = grid.getEntityIndex( auxCell );
                  CPPUNIT_ASSERT( ( auxCellIndex == grid.template getCellNextToCell< 0, 1, 0 >( cellIndex ) ) );
               }
               if( cell.getCoordinates().z() > 0 )
               {
                  CellType auxCell( cell.getCoordinates() + CoordinatesType( 0, 0, -1 ) );
                  const IndexType auxCellIndex = grid.getEntityIndex( auxCell );
                  CPPUNIT_ASSERT( ( auxCellIndex == grid.template getCellNextToCell< 0, 0, -1 >( cellIndex ) ) );
               }
               if( cell.getCoordinates().z() < zSize - 1 )
               {
                  CellType auxCell( cell.getCoordinates() + CoordinatesType( 0, 0, 1 ) );
                  const IndexType auxCellIndex = grid.getEntityIndex( auxCell );
                  CPPUNIT_ASSERT( ( auxCellIndex == grid.template getCellNextToCell< 0, 0, 1 >( cellIndex ) ) );
               }
            }
   }

   void getFaceNextToCellTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
     
      typedef typename GridType::template GridEntity< GridType::Cells > CellType;
      typedef typename GridType::template GridEntity< GridType::Faces > FaceType;
      typedef typename FaceType::EntityOrientationType EntityOrientationType;
      CellType cell;
      FaceType face;
      for( cell.getCoordinates().z() = 0;
           cell.getCoordinates().z() < zSize;
           cell.getCoordinates().z()++ )
         for( cell.getCoordinates().y() = 0;
              cell.getCoordinates().y() < ySize;
              cell.getCoordinates().y()++ )
            for( cell.getCoordinates().x() = 0;
                 cell.getCoordinates().x() < xSize;
                 cell.getCoordinates().x()++ )
            {
               const IndexType cellIndex = grid.getEntityIndex( cell );

               face.setCoordinates( cell.getCoordinates() );
               face.setOrientation( EntityOrientationType( 1, 0, 0 ) );
               //CoordinatesType faceCoordinates( i, j, k );
               IndexType faceIndex = grid.getEntityIndex( face );
               CPPUNIT_ASSERT( ( faceIndex == grid.template getFaceNextToCell< -1, 0, 0 >( cellIndex ) ) );

               //faceCoordinates = CoordinatesType( i + 1, j, k );
               face.setCoordinates( cell.getCoordinates() + CoordinatesType( 1, 0, 0 ) );
               face.setOrientation( EntityOrientationType( 1, 0 , 0 ) );
               faceIndex = grid.getEntityIndex( face );
               CPPUNIT_ASSERT( ( faceIndex == grid.template getFaceNextToCell< 1, 0, 0 >( cellIndex ) ) );

               //faceCoordinates = CoordinatesType( i, j, k );
               face.setCoordinates( cell.getCoordinates() );
               face.setOrientation( EntityOrientationType( 0, -1, 0 ) );
               faceIndex = grid.getEntityIndex( face );
               CPPUNIT_ASSERT( ( faceIndex == grid.template getFaceNextToCell< 0, -1, 0 >( cellIndex ) ) );

               //faceCoordinates = CoordinatesType( i, j + 1, k );
               face.setCoordinates( cell.getCoordinates() + CoordinatesType( 0, 1, 0 ) );
               face.setOrientation( EntityOrientationType( 0, 1, 0 ) );
               faceIndex = grid.getEntityIndex( face );
               CPPUNIT_ASSERT( ( faceIndex == grid.template getFaceNextToCell< 0, 1, 0 >( cellIndex ) ) );

               //faceCoordinates = CoordinatesType( i, j, k );
               face.setCoordinates( cell.getCoordinates() );
               face.setOrientation( EntityOrientationType( 0, 0, -1 ) );
               faceIndex = grid.getEntityIndex( face );
               CPPUNIT_ASSERT( ( faceIndex == grid.template getFaceNextToCell< 0, 0, -1 >( cellIndex ) ) );

               //faceCoordinates = CoordinatesType( i, j, k + 1 );
               face.setCoordinates( cell.getCoordinates() + CoordinatesType( 0, 0, 1 ) );
               face.setOrientation( EntityOrientationType( 0, 0, 1 ) );
               faceIndex = grid.getEntityIndex( face );
               CPPUNIT_ASSERT( ( faceIndex == grid.template getFaceNextToCell< 0, 0, 1 >( cellIndex ) ) );
            }
   }

   void getCellNextToFaceTest()
   {
      const IndexType xSize( 13 );
      const IndexType ySize( 17 );
      const IndexType zSize( 19 );
      GridType grid;
      grid.setDimensions( xSize, ySize, zSize );
      
      typedef typename GridType::template GridEntity< GridType::Cells > CellType;
      typedef typename GridType::template GridEntity< GridType::Faces > FaceType;
      typedef typename FaceType::EntityOrientationType EntityOrientationType;
      CellType cell;
      FaceType face;

      for( face.getCoordinates().z() = 0;
           face.getCoordinates().z() <= zSize;
           face.getCoordinates().z()++ )
         for( face.getCoordinates().y() = 0;
              face.getCoordinates().y() <= ySize;
              face.getCoordinates().y()++ )
            for( face.getCoordinates().x() = 0;
                 face.getCoordinates().x() <= xSize;
                 face.getCoordinates().x()++ )
            {
               //const CoordinatesType faceCoordinates( i, j, k );
               if( face.getCoordinates().y() < ySize && face.getCoordinates().z() < zSize )
               {
                  face.setOrientation( EntityOrientationType( 1, 0, 0  ) );
                  const IndexType faceIndex = grid.getEntityIndex( face );

                  if( face.getCoordinates().x() > 0 )
                  {
                     CellType cell( face.getCoordinates() + CoordinatesType( -1, 0, 0 ) );
                     IndexType cellIndex = grid.getEntityIndex( cell );
                     CPPUNIT_ASSERT( ( cellIndex == grid.template getCellNextToFace< -1, 0, 0 >( faceIndex ) ) );
                  }
                  if( face.getCoordinates().x() < xSize )
                  {
                     CellType cell( face.getCoordinates() );                     
                     IndexType cellIndex = grid.getEntityIndex( cell );
                     CPPUNIT_ASSERT( ( cellIndex == grid.template getCellNextToFace< 1, 0, 0 >( faceIndex ) ) );
                  }
               }
               if( face.getCoordinates().x() < xSize && face.getCoordinates().z() < zSize )
               {
                  face.setOrientation( EntityOrientationType( 0, 1, 0  ) );
                  const IndexType faceIndex = grid.getEntityIndex( face );
                  if( face.getCoordinates().y() > 0 )
                  {
                     CellType cell( face.getCoordinates() + CoordinatesType( 0, -1, 0 ) );
                     IndexType cellIndex = grid.getEntityIndex( cell );
                     CPPUNIT_ASSERT( ( cellIndex == grid.template getCellNextToFace< 0, -1, 0 >( faceIndex ) ) );
                  }
                  if( face.getCoordinates().y() < ySize )
                  {
                     //CoordinatesType cellCoordinates( i, j, k );
                     CellType cell( face.getCoordinates() );
                     IndexType cellIndex = grid.getEntityIndex( cell );
                     CPPUNIT_ASSERT( ( cellIndex == grid.template getCellNextToFace< 0, 1, 0 >( faceIndex ) ) );
                  }
               }
               if( face.getCoordinates().x() < xSize && face.getCoordinates().y() < ySize )
               {
                  face.setOrientation( EntityOrientationType( 0, 0, 1  ) );
                  const IndexType faceIndex = grid.getEntityIndex( face );
                  if( face.getCoordinates().z() > 0 )
                  {
                     CellType cell( face.getCoordinates() + CoordinatesType( 0, 0, -1 ) );
                     IndexType cellIndex = grid.getEntityIndex( cell );
                     CPPUNIT_ASSERT( ( cellIndex == grid.template getCellNextToFace< 0, 0, -1 >( faceIndex ) ) );
                  }
                  if( face.getCoordinates().z() < zSize )
                  {
                     CellType cell( face.getCoordinates() );
                     IndexType cellIndex = grid.getEntityIndex( cell );
                     CPPUNIT_ASSERT( ( cellIndex == grid.template getCellNextToFace< 0, 0, 1 >( faceIndex ) ) );
                  }
               }
            }
   }
};

#endif /* TESTS_UNIT_TESTS_MESH_TNLGRID3DTESTER_H_ */
