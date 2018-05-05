/***************************************************************************
                          TimeDependentPDESolver_impl.h  -  description
                             -------------------
    begin                : Jan 15, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include "TimeDependentPDESolver.h"

namespace TNL {
namespace Solvers {
namespace PDE {   

template< typename Problem,
          typename DiscreteSolver,
          typename TimeStepper >
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
TimeDependentPDESolver()
: problem( 0 ),
  initialTime( 0.0 ),
  finalTime( 0.0 ),
  snapshotPeriod( 0.0 ),
  timeStep( 1.0 ),
  timeStepOrder( 0.0 )
{
}


template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
void
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
configSetup( Config::ConfigDescription& config,
             const String& prefix )
{
   BaseType::configSetup( config, prefix );
   config.addEntry< String >( prefix + "initial-condition", "File name with the initial condition.", "init.tnl" );
   config.addRequiredEntry< double >( prefix + "final-time", "Stop time of the time dependent problem." );
   config.addEntry< double >( prefix + "initial-time", "Initial time of the time dependent problem.", 0 );
   config.addRequiredEntry< double >( prefix + "snapshot-period", "Time period for writing the problem status.");
   config.addEntry< double >( "time-step", "The time step for the time discretisation.", 1.0 );
   config.addEntry< double >( "time-step-order", "The time step is set to time-step*pow( space-step, time-step-order).", 0.0 );
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
   
   BaseType::setup( parameters, prefix );
   
   /****
    * Load the mesh from the mesh file
    */
   const String& meshFile = parameters.getParameter< String >( "mesh" );

   if(Problem::CommunicatorType::isDistributed())
   {
        std::cout << "Loading a global mesh from the file " << meshFile << "...";
        if( ! this->globalMeshPointer->load( meshFile ) )
        {
          std::cerr << std::endl;
          std::cerr << "I am not able to load the global mesh from the file " << meshFile << "." << std::endl;
          return false;
        }
        std::cout << " [ OK ] " << std::endl;
  
       typename Meshes::DistributedMeshes::DistributedMesh<MeshType>::CoordinatesType overlap;
       overlap.setValue(1);
       this->distrMesh.template setGlobalGrid<typename Problem::CommunicatorType>(*globalMeshPointer,overlap);
       distrMesh.SetupGrid(*meshPointer);
   }
   else
   {
       std::cout << "Loading a mesh from the file " << meshFile << "...";
       if( ! this->meshPointer->load( meshFile ) )
       {
          std::cerr << std::endl;
          std::cerr << "I am not able to load the mesh from the file " << meshFile << "." << std::endl;
          std::cerr << " You may create it with tools like tnl-grid-setup or tnl-mesh-convert." << std::endl;
          return false;
       }
       std::cout << " [ OK ] " << std::endl;
    }

   /****
    * Setup the problem
    */
   if( ! problem->setup( this->meshPointer, parameters, prefix ) )
   {
      std::cerr << "The problem initiation failed!" << std::endl;
      return false;
   }

   /****
    * Set DOFs (degrees of freedom)
    */
   TNL_ASSERT_GT( problem->getDofs( this->meshPointer ), 0, "number of DOFs must be positive" );
   this->dofsPointer->setSize( problem->getDofs( this->meshPointer ) );
   this->dofsPointer->setValue( 0.0 );
   this->problem->bindDofs( this->meshPointer, this->dofsPointer );
   
   /****
    * Set mesh dependent data
    */
   this->problem->setMeshDependentData( this->meshPointer, this->meshDependentDataPointer );
   this->problem->bindMeshDependentData( this->meshPointer, this->meshDependentDataPointer );
   
   /***
    * Set-up the initial condition
    */
   std::cout << "Setting up the initial condition ... ";
   typedef typename Problem :: DofVectorType DofVectorType;
   if( ! this->problem->setInitialCondition( parameters, meshPointer, this->dofsPointer, this->meshDependentDataPointer ) )
      return false;
   std::cout << " [ OK ]" << std::endl;

   /****
    * Initialize the time discretisation
    */
   this->setFinalTime( parameters.getParameter< double >( "final-time" ) );
   this->setInitialTime( parameters.getParameter< double >( "initial-time" ) );
   this->setSnapshotPeriod( parameters.getParameter< double >( "snapshot-period" ) );
   this->setTimeStep( parameters.getParameter< double >( "time-step") );
   this->setTimeStepOrder( parameters.getParameter< double >( "time-step-order" ) );

   /****
    * Set-up the discrete solver
    */
   if( ! this->discreteSolver.setup( parameters ) )
      return false;
   
   /****
    * Set-up the time stepper
    */
   if( ! this->timeStepper.setup( parameters ) )
      return false;
   this->timeStepper.setSolver( this->discreteSolver );
   this->timeStepper.setSolverMonitor( *this->solverMonitorPointer );      
   return true;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
writeProlog( Logger& logger,
             const Config::ParameterContainer& parameters )
{   
   logger.writeHeader( problem->getPrologHeader() );
   problem->writeProlog( logger, parameters );
   logger.writeSeparator();
   meshPointer->writeProlog( logger );
   logger.writeSeparator();
   logger.writeParameter< String >( "Time discretisation:", "time-discretisation", parameters );
   logger.writeParameter< double >( "Initial time step:", this->timeStep * std::pow( meshPointer->getSmallestSpaceStep(), this->timeStepOrder ) );
   logger.writeParameter< double >( "Initial time:", "initial-time", parameters );
   logger.writeParameter< double >( "Final time:", "final-time", parameters );
   logger.writeParameter< double >( "Snapshot period:", "snapshot-period", parameters );
   const String& solverName = parameters. getParameter< String >( "discrete-solver" );
   logger.writeParameter< String >( "Discrete solver:", "discrete-solver", parameters );
   if( solverName == "merson" )
      logger.writeParameter< double >( "Adaptivity:", "merson-adaptivity", parameters, 1 );
   if( solverName == "sor" )
      logger.writeParameter< double >( "Omega:", "sor-omega", parameters, 1 );
   if( solverName == "gmres" || solverName == "cwygmres" ) {
      logger.writeParameter< int >( "Restarting min:", "gmres-restarting-min", parameters, 1 );
      logger.writeParameter< int >( "Restarting max:", "gmres-restarting-max", parameters, 1 );
      logger.writeParameter< int >( "Restarting step min:", "gmres-restarting-step-min", parameters, 1 );
      logger.writeParameter< int >( "Restarting step max:", "gmres-restarting-step-max", parameters, 1 );
   }
   logger.writeParameter< double >( "Convergence residue:", "convergence-residue", parameters );
   logger.writeParameter< double >( "Divergence residue:", "divergence-residue", parameters );
   logger.writeParameter< int >( "Maximal number of iterations:", "max-iterations", parameters );
   logger.writeParameter< int >( "Minimal number of iterations:", "min-iterations", parameters );
   logger.writeSeparator();
   return BaseType::writeProlog( logger, parameters );
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
void
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setProblem( ProblemType& problem )
{
   this->problem = &problem;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
void
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setInitialTime( const RealType& initialTime )
{
   this->initialTime = initialTime;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
const typename Problem::RealType&
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
getInitialTime() const
{
   return this->initialTime;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setFinalTime( const RealType& finalTime )
{
   if( finalTime <= this->initialTime )
   {
      std::cerr << "Final time for TimeDependentPDESolver must larger than the initial time which is now " << this->initialTime << "." << std::endl;
      return false;
   }
   this->finalTime = finalTime;
   return true;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
const typename Problem::RealType&
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
getFinalTime() const
{
   return this->finalTime;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setSnapshotPeriod( const RealType& period )
{
   if( period <= 0 )
   {
      std::cerr << "Snapshot tau for TimeDependentPDESolver must be positive value." << std::endl;
      return false;
   }
   this->snapshotPeriod = period;
   return true;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
const typename Problem::RealType&
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
getSnapshotPeriod() const
{
   return this->snapshotPeriod;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setTimeStep( const RealType& timeStep )
{
   if( timeStep <= 0 )
   {
      std::cerr << "The time step for TimeDependentPDESolver must be positive value." << std::endl;
      return false;
   }
   this->timeStep = timeStep;
   return true;
}
 
template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
const typename Problem::RealType&
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
getTimeStep() const
{
   return this->timeStep;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
setTimeStepOrder( const RealType& timeStepOrder )
{
   if( timeStepOrder < 0 )
   {
      std::cerr << "The time step order for TimeDependentPDESolver must be zero or positive value." << std::endl;
      return false;
   }
   this->timeStepOrder = timeStepOrder;
   return true;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
const typename Problem::RealType&
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
getTimeStepOrder() const
{
   return this->timeStepOrder;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
solve()
{
   TNL_ASSERT_TRUE( problem, "No problem was set in PDESolver." );

   if( snapshotPeriod == 0 )
   {
      std::cerr << "No snapshot tau was set in TimeDependentPDESolver." << std::endl;
      return false;
   }
   RealType t( this->initialTime );
   IndexType step( 0 );
   IndexType allSteps = ceil( ( this->finalTime - this->initialTime ) / this->snapshotPeriod );

   this->ioTimer->reset();
   this->computeTimer->reset();
 
   this->ioTimer->start();
   if( ! this->problem->makeSnapshot( t, step, meshPointer, this->dofsPointer, this->meshDependentDataPointer ) )
   {
      std::cerr << "Making the snapshot failed." << std::endl;
      return false;
   }
   this->ioTimer->stop();
   this->computeTimer->start();

   /****
    * Initialize the time stepper
    */
   this->timeStepper.setProblem( * ( this->problem ) );
   this->timeStepper.init( this->meshPointer );
   this->timeStepper.setTimeStep( this->timeStep * std::pow( this->meshPointer.getData().getSmallestSpaceStep(), this->timeStepOrder ) );
   while( step < allSteps )
   {
      RealType tau = min( this->snapshotPeriod,
                          this->finalTime - t );
      if( ! this->timeStepper.solve( t, t + tau, this->meshPointer, this->dofsPointer, this->meshDependentDataPointer ) )
         return false;
      step ++;
      t += tau;

      this->ioTimer->start();
      this->computeTimer->stop();
      if( ! this->problem->makeSnapshot( t, step, this->meshPointer, this->dofsPointer, this->meshDependentDataPointer ) )
      {
         std::cerr << "Making the snapshot failed." << std::endl;
         return false;
      }
      this->ioTimer->stop();
      this->computeTimer->start();
   }
   this->computeTimer->stop();
   
   this->solverMonitorPointer->stopMainLoop();
   
   return true;
}

template< typename Problem,
          typename DiscreteSolver,   
          typename TimeStepper >
bool
TimeDependentPDESolver< Problem, DiscreteSolver, TimeStepper >::
writeEpilog( Logger& logger ) const
{
   return ( this->timeStepper.writeEpilog( logger ) &&
      this->problem->writeEpilog( logger ) );
}

} // namespace PDE
} // namespace Solvers
} // namespace TNL