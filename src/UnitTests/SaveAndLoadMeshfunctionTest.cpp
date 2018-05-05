/***************************************************************************
                          SaveAndLoadMeshfunctionTest.cpp  -  description
                             -------------------
    begin                : Dec 2, 2017
    copyright            : (C) 2017 by Tomas Oberhuber et al.
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

#include <TNL/Functions/MeshFunction.h>

#ifdef HAVE_GTEST 
#include <gtest/gtest.h>

#include "Mpi/Functions.h"

using namespace TNL::Containers;
using namespace TNL::Meshes;
using namespace TNL::Functions;
using namespace TNL::Devices;


//=====================================TEST CLASS==============================================

template <int dim>
class TestSaveAndLoadMeshfunction
{
    public:
        static void Test()
        {

            typedef Grid<dim,double,Host,int> MeshType;
            typedef MeshFunction<MeshType> MeshFunctionType;
            typedef Vector<double,Host,int> DofType;
            typedef typename MeshType::Cell Cell;
            typedef typename MeshType::IndexType IndexType; 
            typedef typename MeshType::PointType PointType; 
        
            typedef typename MeshType::CoordinatesType CoordinatesType;
            typedef LinearFunction<double,dim> LinearFunctionType;

            SharedPointer< LinearFunctionType, Host > linearFunctionPtr;
            MeshFunctionEvaluator< MeshFunctionType, LinearFunctionType > linearFunctionEvaluator;    


            PointType localOrigin;
            localOrigin.setValue(-0.5);
            PointType localProportions;
            localProportions.setValue(10);
            
            SharedPointer<MeshType>  localGridptr;
            localGridptr->setDimensions(localProportions);
            localGridptr->setDomain(localOrigin,localProportions);

            DofType localDof(localGridptr->template getEntitiesCount< Cell >());

            SharedPointer<MeshFunctionType> localMeshFunctionptr;
            localMeshFunctionptr->bind(localGridptr,localDof);
            linearFunctionEvaluator.evaluateAllEntities(localMeshFunctionptr , linearFunctionPtr);

            File file;
            file.open( String( "/tmp/test-file.tnl"), IOMode::write );        
            localMeshFunctionptr->save(file);        
            file.close();

            //load other meshfunction on same localgrid from created file
            SharedPointer<MeshType>  loadGridptr;
            loadGridptr->setDimensions(localProportions);
            loadGridptr->setDomain(localOrigin,localProportions);

            DofType loadDof(loadGridptr->template getEntitiesCount< Cell >());
            SharedPointer<MeshFunctionType> loadMeshFunctionptr;
            loadMeshFunctionptr->bind(loadGridptr,loadDof);

            for(int i=0;i<loadDof.getSize();i++)
            {
                loadDof[i]=-1;
            }

            file.open( String( "/tmp/test-file.tnl" ), IOMode::read );
            loadMeshFunctionptr->boundLoad(file);
            file.close();

            for(int i=0;i<localDof.getSize();i++)
            {
                EXPECT_EQ( localDof[i], loadDof[i]) << "Compare Loaded and evaluated Dof Failed for: "<< i;
            }

        };
};

TEST( CopyEntitiesTest, 1D )
{
    TestSaveAndLoadMeshfunction<1>::Test();
}

TEST( CopyEntitiesTest, 2D )
{
    TestSaveAndLoadMeshfunction<2>::Test();
}

TEST( CopyEntitiesTest, 3D )
{
    TestSaveAndLoadMeshfunction<3>::Test();
}


#endif

#include "GtestMissingError.h"
int main( int argc, char* argv[] )
{
#ifdef HAVE_GTEST
   ::testing::InitGoogleTest( &argc, argv );
   return RUN_ALL_TESTS();
#else
   throw GtestMissingError();
#endif
}
