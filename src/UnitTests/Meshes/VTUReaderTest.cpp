#ifdef HAVE_GTEST
#include <gtest/gtest.h>

#include <TNL/Meshes/Readers/VTUReader.h>
#include <TNL/Meshes/Writers/VTUWriter.h>
#include <TNL/Meshes/TypeResolver/resolveMeshType.h>

#include "data/loader.h"
#include "MeshReaderTest.h"

using namespace TNL::Meshes;

static const char* TEST_FILE_NAME = "test_VTUReaderTest.vtu";

struct MyConfigTag {};

namespace TNL {
namespace Meshes {
namespace BuildConfigTags {

// disable all grids
template< int Dimension, typename Real, typename Device, typename Index >
struct GridTag< MyConfigTag, Grid< Dimension, Real, Device, Index > >{ enum { enabled = false }; };

// enable meshes used in the tests
//template<> struct MeshCellTopologyTag< MyConfigTag, Topologies::Edge > { enum { enabled = true }; };
template<> struct MeshCellTopologyTag< MyConfigTag, Topologies::Triangle > { enum { enabled = true }; };
template<> struct MeshCellTopologyTag< MyConfigTag, Topologies::Tetrahedron > { enum { enabled = true }; };

} // namespace BuildConfigTags
} // namespace Meshes
} // namespace TNL

// TODO: test case for 1D mesh of edges

TEST( VTUReaderTest, mrizka_1 )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Triangle > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "triangles/mrizka_1.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 142 );
   EXPECT_EQ( cells, 242 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

TEST( VTUReaderTest, tetrahedrons )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Tetrahedron > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "tetrahedrons/cube1m_1.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 395 );
   EXPECT_EQ( cells, 1312 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

// ASCII data, produced by TNL writer
TEST( VTUReaderTest, triangles_2x2x2_minimized_ascii )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Triangle > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "triangles_2x2x2/minimized_ascii.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 9 );
   EXPECT_EQ( cells, 8 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

// encoded data, produced by TNL writer
TEST( VTUReaderTest, triangles_2x2x2_minimized_encoded_tnl )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Triangle > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "triangles_2x2x2/minimized_encoded_tnl.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 9 );
   EXPECT_EQ( cells, 8 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

// encoded data, produced by Paraview
TEST( VTUReaderTest, triangles_2x2x2_minimized_encoded_paraview )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Triangle > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "triangles_2x2x2/minimized_encoded_paraview.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 9 );
   EXPECT_EQ( cells, 8 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

// compressed data, produced by TNL
TEST( VTUReaderTest, triangles_2x2x2_minimized_compressed_tnl )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Triangle > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "triangles_2x2x2/minimized_compressed_tnl.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 9 );
   EXPECT_EQ( cells, 8 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

// compressed data, produced by Paraview
TEST( VTUReaderTest, triangles_2x2x2_minimized_compressed_paraview )
{
   using MeshType = Mesh< DefaultConfig< Topologies::Triangle > >;
   const MeshType mesh = loadMeshFromFile< MeshType, Readers::VTUReader >( "triangles_2x2x2/minimized_compressed_paraview.vtu" );

   // test that the mesh was actually loaded
   const auto vertices = mesh.template getEntitiesCount< 0 >();
   const auto cells = mesh.template getEntitiesCount< MeshType::getMeshDimension() >();
   EXPECT_EQ( vertices, 9 );
   EXPECT_EQ( cells, 8 );

   test_reader< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME );
   test_resolveAndLoadMesh< Writers::VTUWriter, MyConfigTag >( mesh, TEST_FILE_NAME );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "PointData" );
   test_meshfunction< Readers::VTUReader, Writers::VTUWriter >( mesh, TEST_FILE_NAME, "CellData" );
}

// TODO: test cases for the appended data block: minimized_appended_binary_compressed.vtu, minimized_appended_binary.vtu, minimized_appended_encoded_compressed.vtu, minimized_appended_encoded.vtu
#endif

#include "../main.h"
