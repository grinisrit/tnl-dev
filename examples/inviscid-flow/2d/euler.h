#include <TNL/tnlConfig.h>
#include <TNL/Solvers/Solver.h>
#include <TNL/Solvers/BuildConfigTags.h>
#include <TNL/Operators/DirichletBoundaryConditions.h>
#include <TNL/Operators/NeumannBoundaryConditions.h>
#include <TNL/Functions/Analytic/Constant.h>
#include "eulerProblem.h"
#include "LaxFridrichs2D.h"
#include "eulerRhs.h"
#include "eulerBuildConfigTag.h"
#include "MyMixedBoundaryConditions.h"
#include "MyNeumannBoundaryConditions.h"

using namespace TNL;

typedef eulerBuildConfigTag BuildConfig;

/****
 * Uncoment the following (and comment the previous line) for the complete build.
 * This will include support for all floating point precisions, all indexing types
 * and more solvers. You may then choose between them from the command line.
 * The compile time may, however, take tens of minutes or even several hours,
 * esppecially if CUDA is enabled. Use this, if you want, only for the final build,
 * not in the development phase.
 */
//typedef tnlDefaultConfigTag BuildConfig;

template< typename ConfigTag >class eulerConfig
{
   public:
      static void configSetup( Config::ConfigDescription & config )
      {
         config.addDelimiter( "euler2D settings:" );
         config.addEntry< String >( "boundary-conditions-type", "Choose the boundary conditions type.", "dirichlet");
            config.addEntryEnum< String >( "dirichlet" );
            config.addEntryEnum< String >( "neumann" );
            config.addEntryEnum< String >( "mymixed" );
            config.addEntryEnum< String >( "myneumann" );
         config.addEntry< double >( "boundary-conditions-constant", "This sets a value in case of the constant boundary conditions." );
         config.addEntry< double >( "NW-density", "This sets a value of northwest density." );
         config.addEntry< double >( "NW-velocityX", "This sets a value of northwest x velocity." );
         config.addEntry< double >( "NW-velocityY", "This sets a value of northwest y velocity." );
         config.addEntry< double >( "NW-pressure", "This sets a value of northwest pressure." );
         config.addEntry< double >( "SW-density", "This sets a value of southwest density." );
         config.addEntry< double >( "SW-velocityX", "This sets a value of southwest x velocity." );
         config.addEntry< double >( "SW-velocityY", "This sets a value of southwest y velocity." );
         config.addEntry< double >( "SW-pressure", "This sets a value of southwest pressure." );
         config.addEntry< double >( "riemann-border", "This sets a position of discontinuity cross." );
         config.addEntry< double >( "NE-density", "This sets a value of northeast density." );
         config.addEntry< double >( "NE-velocityX", "This sets a value of northeast x velocity." );
         config.addEntry< double >( "NE-velocityY", "This sets a value of northeast y velocity." );
         config.addEntry< double >( "NE-pressure", "This sets a value of northeast pressure." );
         config.addEntry< double >( "SE-density", "This sets a value of southeast density." );
         config.addEntry< double >( "SE-velocityX", "This sets a value of southeast x velocity." );
         config.addEntry< double >( "SE-velocityY", "This sets a value of southeast y velocity." );
         config.addEntry< double >( "SE-pressure", "This sets a value of southeast pressure." );
         config.addEntry< double >( "gamma", "This sets a value of gamma constant." );

         /****
          * Add definition of your solver command line arguments.
          */

      }
};

template< typename Real,
          typename Device,
          typename Index,
          typename MeshType,
          typename ConfigTag,
          typename SolverStarter >
class eulerSetter
{
   public:

      typedef Real RealType;
      typedef Device DeviceType;
      typedef Index IndexType;

      static bool run( const Config::ParameterContainer & parameters )
      {
std::cout <<"run";
          enum { Dimensions = MeshType::getMeshDimensions() };
          typedef LaxFridrichs2D< MeshType, Real, Index > ApproximateOperator;
          typedef eulerRhs< MeshType, Real > RightHandSide;
          typedef Containers::StaticVector < MeshType::getMeshDimensions(), Real > Vertex;

         /****
          * Resolve the template arguments of your solver here.
          * The following code is for the Dirichlet and the Neumann boundary conditions.
          * Both can be constant or defined as descrete values of Vector.
          */
          String boundaryConditionsType = parameters.getParameter< String >( "boundary-conditions-type" );
          if( parameters.checkParameter( "boundary-conditions-constant" ) )
          {
             typedef Functions::Analytic::Constant< Dimensions, Real > Constant;
             if( boundaryConditionsType == "dirichlet" )
             {
                typedef Operators::DirichletBoundaryConditions< MeshType, Constant, MeshType::getMeshDimensions(), Real, Index > BoundaryConditions;
                typedef eulerProblem< MeshType, BoundaryConditions, RightHandSide, ApproximateOperator > Problem;
                SolverStarter solverStarter;
                return solverStarter.template run< Problem >( parameters );
             }
             typedef Operators::NeumannBoundaryConditions< MeshType, Constant, Real, Index > BoundaryConditions;
             typedef eulerProblem< MeshType, BoundaryConditions, RightHandSide, ApproximateOperator > Problem;
             SolverStarter solverStarter;
             return solverStarter.template run< Problem >( parameters );
          }
          typedef Functions::MeshFunction< MeshType > MeshFunction;
          if( boundaryConditionsType == "dirichlet" )
          {
             typedef Operators::DirichletBoundaryConditions< MeshType, MeshFunction, MeshType::getMeshDimensions(), Real, Index > BoundaryConditions;
             typedef eulerProblem< MeshType, BoundaryConditions, RightHandSide, ApproximateOperator > Problem;
             SolverStarter solverStarter;
             return solverStarter.template run< Problem >( parameters );
          }
          if( boundaryConditionsType == "mymixed" )
          {
             typedef Operators::MyMixedBoundaryConditions< MeshType, MeshFunction, MeshType::getMeshDimensions(), Real, Index > BoundaryConditions;
             typedef eulerProblem< MeshType, BoundaryConditions, RightHandSide, ApproximateOperator > Problem;
             SolverStarter solverStarter;
             return solverStarter.template run< Problem >( parameters );
          }
          if( boundaryConditionsType == "myneumann" )
          {
             typedef Operators::MyNeumannBoundaryConditions< MeshType, MeshFunction, MeshType::getMeshDimensions(), Real, Index > BoundaryConditions;
             typedef eulerProblem< MeshType, BoundaryConditions, RightHandSide, ApproximateOperator > Problem;
             SolverStarter solverStarter;
             return solverStarter.template run< Problem >( parameters );
          }
          if( boundaryConditionsType == "neumann" )
          {
             typedef Operators::NeumannBoundaryConditions< MeshType, MeshFunction, Real, Index > BoundaryConditions;
             typedef eulerProblem< MeshType, BoundaryConditions, RightHandSide, ApproximateOperator > Problem;
             SolverStarter solverStarter;
             return solverStarter.template run< Problem >( parameters );
          }
    return true;}
};

int main( int argc, char* argv[] )
{
   Solvers::Solver< eulerSetter, eulerConfig, BuildConfig > solver;
   if( ! solver. run( argc, argv ) )
      return EXIT_FAILURE;
   return EXIT_SUCCESS;
}
