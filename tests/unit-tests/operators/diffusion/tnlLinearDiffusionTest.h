/***************************************************************************
                          LinearDiffusionTest.h  -  description
                             -------------------
    begin                : Feb 1, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLLINEARDIFFUSIONTEST_H
#define	TNLLINEARDIFFUSIONTEST_H

#include <TNL/Operators/diffusion/LinearDiffusion.h>
#include <TNL/Operators/diffusion/ExactLinearDiffusion.h>
#include <TNL/mesh/tnlGrid.h>
#include "../tnlPDEOperatorEocUnitTest.h"
#include "../tnlPDEOperatorEocTest.h"
#include "../../tnlUnitTestStarter.h"

using namespace TNL;

template< typename ApproximateOperator,
          typename TestFunction,
          bool write = false,
          bool verbose = false >
class LinearDiffusionTest
   : public tnlPDEOperatorEocTest< ApproximateOperator, TestFunction >
{
   public:
 
      typedef ApproximateOperator ApproximateOperatorType;
      typedef typename ApproximateOperator::ExactOperatorType ExactOperatorType;
      typedef typename ApproximateOperator::MeshType MeshType;
      typedef typename ApproximateOperator::RealType RealType;
      typedef typename ApproximateOperator::IndexType IndexType;
 
      const IndexType coarseMeshSize[ 3 ] = { 1024, 256, 64 };
 
      const RealType  eoc[ 3 ] =       { 2.0,  2.0,  2.0 };
      const RealType  tolerance[ 3 ] = { 0.05, 0.05, 0.05 };
 
      static String getType()
      {
         return String( "LinearDiffusionTest< " ) +
                ApproximateOperator::getType() + ", " +
                TestFunction::getType() + " >";
      }
 
      void setupTest()
      {
         this->setupFunction();
      }
 
      void getApproximationError( const IndexType meshSize,
                                  RealType errors[ 3 ] )
      {
         this->setupMesh( meshSize );
         this->performTest( this->approximateOperator,
                            this->exactOperator,
                            errors,
                            write,
                            verbose );

      }
 
      void runUnitTest()
      {
         RealType coarseErrors[ 3 ], fineErrors[ 3 ];
         this->getApproximationError( coarseMeshSize[ MeshType::getMeshDimensions() - 1 ], coarseErrors );
         this->getApproximationError( 2 * coarseMeshSize[ MeshType::getMeshDimensions() - 1 ], fineErrors );
         this->checkEoc( coarseErrors, fineErrors, eoc, tolerance, verbose );
      }
 
   protected:

      ApproximateOperator approximateOperator;
 
      ExactOperatorType exactOperator;

};


template< typename Mesh,
          typename Function,
          bool write,
          bool verbose >
bool runTest()
{
   typedef Operators::LinearDiffusion< Mesh > ApproximateOperator;
   typedef LinearDiffusionTest< ApproximateOperator, Function, write, verbose > OperatorTest;
#ifdef HAVE_CPPUNIT
   if( ! tnlUnitTestStarter::run< tnlPDEOperatorEocUnitTest< OperatorTest > >() )
      return false;
   return true;
#else
   return false;
#endif
}

template< typename Mesh,
          bool write,
          bool verbose >
bool setTestFunction()
{
   return runTest< Mesh, Functions::Analytic::ExpBumpFunction< Mesh::getMeshDimensions(), double >, write, verbose >();
}

template< typename Device,
          bool write,
          bool verbose >
bool setMesh()
{
   return ( setTestFunction< tnlGrid< 1, double, Device, int >, write, verbose >() &&
            setTestFunction< tnlGrid< 2, double, Device, int >, write, verbose >() &&
            setTestFunction< tnlGrid< 3, double, Device, int >, write, verbose >() );
}

int main( int argc, char* argv[] )
{
   const bool verbose( false );
   const bool write( false );
 
   if( ! setMesh< Devices::Host, write, verbose  >() )
      return EXIT_FAILURE;
#ifdef HAVE_CUDA
   if( ! setMesh< Devices::Cuda, write, verbose >() )
      return EXIT_FAILURE;
#endif
   return EXIT_SUCCESS;
}

#endif	/* TNLLINEARDIFFUSIONTEST_H */

