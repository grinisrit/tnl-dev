/***************************************************************************
                          DistributedGridTest.cpp  -  description
                             -------------------
    begin                : Sep 6, 2017
    copyright            : (C) 2017 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/


#ifdef HAVE_GTEST  
#include <gtest/gtest.h>

#ifdef HAVE_MPI    

#include <TNL/Communicators/MpiCommunicator.h>
#include <TNL/Functions/MeshFunction.h>
#include <TNL/Meshes/DistributedMeshes/DistributedMesh.h>
#include <TNL/Meshes/DistributedMeshes/SubdomainOverlapsGetter.h>

#include "../../Functions/Functions.h"

using namespace TNL;
using namespace TNL::Containers;
using namespace TNL::Meshes;
using namespace TNL::Meshes::DistributedMeshes;
using namespace TNL::Functions;
using namespace TNL::Devices;
using namespace TNL::Communicators;


template<typename DofType>
void setDof_1D(DofType &dof, typename DofType::RealType value)
{
    for(int i=0;i<dof.getSize();i++)
        dof[i]=value;
}

template<typename DofType>
void check_Boundary_1D(int rank, int nproc, DofType dof, typename DofType::RealType expectedValue)
{
    if(rank==0)//Left
    {
        EXPECT_EQ( dof[0], expectedValue) << "Left boundary test failed";
        return;
    }
    
    if(rank==(nproc-1))//Right
    {
        EXPECT_EQ( dof[dof.getSize()-1], expectedValue) << "Right boundary test failed";
        return;
    }
    
};

template<typename DofType>
void check_Overlap_1D(int rank, int nproc, DofType dof, typename DofType::RealType expectedValue)
{
    if(rank==0)//Left
    {
        EXPECT_EQ( dof[dof.getSize()-1], expectedValue) << "Left boundary node overlap test failed";
        return;
    }
    
    if(rank==(nproc-1))
    {
        EXPECT_EQ( dof[0], expectedValue) << "Right boundary node overlap test failed";
        return;
    }
    
    EXPECT_EQ( dof[0], expectedValue) << "left overlap test failed";
    EXPECT_EQ( dof[dof.getSize()-1], expectedValue)<< "right overlap test failed";    
};

template<typename DofType>
void check_Inner_1D(int rank, int nproc, DofType dof, typename DofType::RealType expectedValue)
{
    for(int i=1;i<(dof.getSize()-2);i++) //buď je vlevo hranice, nebo overlap
        EXPECT_EQ( dof[i], expectedValue) << " "<< i;
};

/*
 * Light check of 1D distriover grid and its synchronization. 
 * Number of process is not limitated.
 * Overlap is limitated to 1
 * Only double is tested as dof Real type -- it may be changed, extend test
 * Global size is hardcoded as 10 -- it can be changed, extend test
 */

typedef MpiCommunicator CommunicatorType;
typedef Grid<1,double,Host,int> GridType;
typedef MeshFunction<GridType> MeshFunctionType;
typedef Vector<double,Host,int> DofType;
typedef typename GridType::Cell Cell;
typedef typename GridType::IndexType IndexType; 
typedef typename GridType::PointType PointType; 
typedef DistributedMesh<GridType> DistributedGridType;
     
class DistributedGirdTest_1D : public ::testing::Test
{
   protected:

      static DistributedMesh< GridType > *distrgrid;
      static DofType *dof;

      static SharedPointer< GridType > gridptr;
      static SharedPointer< MeshFunctionType > meshFunctionptr;

      static MeshFunctionEvaluator< MeshFunctionType, ConstFunction< double, 1 > > constFunctionEvaluator;
      static SharedPointer< ConstFunction< double, 1 >, Host > constFunctionPtr;

      static MeshFunctionEvaluator< MeshFunctionType, LinearFunction< double, 1 > > linearFunctionEvaluator;
      static SharedPointer< LinearFunction< double, 1 >, Host > linearFunctionPtr;

      static int rank;
      static int nproc;    

      // Per-test-case set-up.
      // Called before the first test in this test case.
      // Can be omitted if not needed.
      static void SetUpTestCase()
      {
         int size=10;
         rank=CommunicatorType::GetRank(CommunicatorType::AllGroup);
         nproc=CommunicatorType::GetSize(CommunicatorType::AllGroup);

         PointType globalOrigin;
         PointType globalProportions;
         GridType globalGrid;

         globalOrigin.x()=-0.5;    
         globalProportions.x()=size;


         globalGrid.setDimensions(size);
         globalGrid.setDomain(globalOrigin,globalProportions);

         typename DistributedGridType::CoordinatesType overlap;
         overlap.setValue(1);
         distrgrid=new DistributedGridType();

         typename DistributedGridType::SubdomainOverlapsType lowerOverlap, upperOverlap;
         distrgrid->template setGlobalGrid<CommunicatorType>( globalGrid );
         distrgrid->setupGrid(*gridptr);    
         SubdomainOverlapsGetter< GridType, CommunicatorType >::getOverlaps( distrgrid, lowerOverlap, upperOverlap, 1 );
         distrgrid->setOverlaps( lowerOverlap, upperOverlap );

         //distrgrid->template setGlobalGrid<CommunicatorType>(globalGrid,overlap,overlap); // TODO: fix this

         distrgrid->setupGrid(*gridptr);
         dof=new DofType(gridptr->template getEntitiesCount< Cell >());

         meshFunctionptr->bind(gridptr,*dof);

         constFunctionPtr->Number=rank;
      }

  // Per-test-case tear-down.
  // Called after the last test in this test case.
  // Can be omitted if not needed.
  static void TearDownTestCase() {
      delete dof;
      delete distrgrid;
  }
};

DistributedMesh<GridType> *DistributedGirdTest_1D::distrgrid=NULL;
DofType *DistributedGirdTest_1D::dof=NULL;
SharedPointer<GridType> DistributedGirdTest_1D::gridptr;
SharedPointer<MeshFunctionType> DistributedGirdTest_1D::meshFunctionptr;
MeshFunctionEvaluator< MeshFunctionType, ConstFunction<double,1> > DistributedGirdTest_1D::constFunctionEvaluator;
SharedPointer< ConstFunction<double,1>, Host > DistributedGirdTest_1D::constFunctionPtr;
MeshFunctionEvaluator< MeshFunctionType, LinearFunction<double,1> > DistributedGirdTest_1D::linearFunctionEvaluator;
SharedPointer< LinearFunction<double,1>, Host > DistributedGirdTest_1D::linearFunctionPtr;
int DistributedGirdTest_1D::rank;
int DistributedGirdTest_1D::nproc;

TEST_F(DistributedGirdTest_1D, evaluateAllEntities)
{
    //Check Traversars
    //All entities, witout overlap
    setDof_1D(*dof,-1);
    constFunctionEvaluator.evaluateAllEntities( meshFunctionptr , constFunctionPtr );
    //Printer<GridType,DofType>::print_dof(rank,*gridptr,*dof);
    check_Boundary_1D(rank, nproc, *dof, rank);
    check_Overlap_1D(rank, nproc, *dof, -1);
    check_Inner_1D(rank, nproc, *dof, rank);
}

TEST_F(DistributedGirdTest_1D, evaluateBoundaryEntities)
{
    //Boundary entities, witout overlap
    setDof_1D(*dof,-1);
    constFunctionEvaluator.evaluateBoundaryEntities( meshFunctionptr , constFunctionPtr );
    check_Boundary_1D(rank, nproc, *dof, rank);
    check_Overlap_1D(rank, nproc, *dof, -1);
    check_Inner_1D(rank, nproc, *dof, -1);
}

TEST_F(DistributedGirdTest_1D, evaluateInteriorEntities)
{
    //Inner entities, witout overlap
    setDof_1D(*dof,-1);
    constFunctionEvaluator.evaluateInteriorEntities( meshFunctionptr , constFunctionPtr );
    check_Boundary_1D(rank, nproc, *dof, -1);
    check_Overlap_1D(rank, nproc, *dof, -1);
    check_Inner_1D(rank, nproc, *dof, rank);
}    

TEST_F(DistributedGirdTest_1D, LinearFunctionTest)
{
    //fill meshfunction with linear function (physical center of cell corresponds with its coordinates in grid) 
    setDof_1D(*dof,-1);
    linearFunctionEvaluator.evaluateAllEntities(meshFunctionptr, linearFunctionPtr);
    meshFunctionptr->template synchronize<CommunicatorType>();

    auto entite= gridptr->template getEntity< Cell >(0);
    entite.refresh();
    EXPECT_EQ(meshFunctionptr->getValue(entite), (*linearFunctionPtr)(entite)) << "Linear function Overlap error on left Edge.";

    auto entite2= gridptr->template getEntity< Cell >((*dof).getSize()-1);
    entite2.refresh();
    EXPECT_EQ(meshFunctionptr->getValue(entite), (*linearFunctionPtr)(entite)) << "Linear function Overlap error on right Edge.";
}

TEST_F(DistributedGirdTest_1D, SynchronizerNeighborTest)
{
    setDof_1D(*dof,-1);
    constFunctionEvaluator.evaluateAllEntities( meshFunctionptr , constFunctionPtr );
    meshFunctionptr->template synchronize<CommunicatorType>();

    if(rank!=0)
        EXPECT_EQ((*dof)[0],rank-1)<< "Left Overlap was filled by wrong process.";
    if(rank!=nproc-1)
        EXPECT_EQ((*dof)[dof->getSize()-1],rank+1)<< "Right Overlap was filled by wrong process.";
}
    
#else
TEST(NoMPI, NoTest)
{
    ASSERT_TRUE(true) << ":-(";
}
#endif

#endif


#if (defined(HAVE_GTEST) && defined(HAVE_MPI))
#include <sstream>

  class MinimalistBufferedPrinter : public ::testing::EmptyTestEventListener {
      
  private:
      std::stringstream sout;
      
  public:
      
    // Called before a test starts.
    virtual void OnTestStart(const ::testing::TestInfo& test_info) {
      sout<< test_info.test_case_name() <<"." << test_info.name() << " Start." <<std::endl;
    }

    // Called after a failed assertion or a SUCCEED() invocation.
    virtual void OnTestPartResult(
        const ::testing::TestPartResult& test_part_result) {
      sout << (test_part_result.failed() ? "====Failure=== " : "===Success=== ") 
              << test_part_result.file_name() << " "
              << test_part_result.line_number() <<std::endl
              << test_part_result.summary() <<std::endl;
    }

    // Called after a test ends.
    virtual void OnTestEnd(const ::testing::TestInfo& test_info) 
    {
		int rank=CommunicatorType::GetRank(CommunicatorType::AllGroup);
        sout<< test_info.test_case_name() <<"." << test_info.name() << " End." <<std::endl;
        std::cout << rank << ":" << std::endl << sout.str()<< std::endl;
        sout.str( std::string() );
        sout.clear();
    }
  };
#endif

#include "../../src/UnitTests/GtestMissingError.h"
int main( int argc, char* argv[] )
{
#ifdef HAVE_GTEST
   ::testing::InitGoogleTest( &argc, argv );

    #ifdef HAVE_MPI
       ::testing::TestEventListeners& listeners =
          ::testing::UnitTest::GetInstance()->listeners();

       delete listeners.Release(listeners.default_result_printer());
       listeners.Append(new MinimalistBufferedPrinter);

       CommunicatorType::Init(argc,argv);
    #endif
       int result= RUN_ALL_TESTS();

    #ifdef HAVE_MPI
       CommunicatorType::Finalize();
    #endif
       return result;
#else
   
   throw GtestMissingError();
#endif
}

