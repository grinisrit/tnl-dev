/***************************************************************************
                          ExplicitTimeStepper_impl.h  -  description
                             -------------------
    begin                : Jan 15, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include "ExplicitTimeStepper.h"

namespace TNL {
namespace Solvers {
namespace PDE {   

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
ExplicitTimeStepper< Problem, OdeSolver >::
ExplicitTimeStepper()
: odeSolver( 0 ),
  problem( 0 ),
  timeStep( 0 ),
  allIterations( 0 )
{
};

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
void
ExplicitTimeStepper< Problem, OdeSolver >::
configSetup( Config::ConfigDescription& config,
             const String& prefix )
{
}

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
bool
ExplicitTimeStepper< Problem, OdeSolver >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
   return true;
}

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
bool
ExplicitTimeStepper< Problem, OdeSolver >::
init( const MeshPointer& meshPointer )
{
   this->explicitUpdaterTimer.reset();
   this->mainTimer.reset();
   this->preIterateTimer.reset();
   this->postIterateTimer.reset();
   return true;
}

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
void
ExplicitTimeStepper< Problem, OdeSolver >::
setSolver( typename ExplicitTimeStepper< Problem, OdeSolver >::OdeSolverType& odeSolver )
{
   this->odeSolver = &odeSolver;
};

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
void
ExplicitTimeStepper< Problem, OdeSolver >::
setProblem( ProblemType& problem )
{
   this->problem = &problem;
};

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
Problem*
ExplicitTimeStepper< Problem, OdeSolver >::
getProblem() const
{
    return this->problem;
};

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
bool
ExplicitTimeStepper< Problem, OdeSolver >::
setTimeStep( const RealType& timeStep )
{
   if( timeStep <= 0.0 )
   {
      std::cerr << "Tau for ExplicitTimeStepper must be positive. " << std::endl;
      return false;
   }
   this->timeStep = timeStep;
   return true;
};

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
bool
ExplicitTimeStepper< Problem, OdeSolver >::
solve( const RealType& time,
       const RealType& stopTime,
       const MeshPointer& mesh,
       DofVectorPointer& dofVector,
       MeshDependentDataType& meshDependentData )
{
   Assert( this->odeSolver, );
   mainTimer.start();
   this->odeSolver->setTau( this->timeStep );
   this->odeSolver->setProblem( * this );
   this->odeSolver->setTime( time );
   this->odeSolver->setStopTime( stopTime );
   if( this->odeSolver->getMinIterations() )
      this->odeSolver->setMaxTau( ( stopTime - time ) / ( typename OdeSolver< Problem >::RealType ) this->odeSolver->getMinIterations() );
   this->mesh = mesh;
   this->meshDependentData = &meshDependentData;
   if( ! this->odeSolver->solve( dofVector ) )
      return false;
   this->problem->setExplicitBoundaryConditions( stopTime, this->mesh, dofVector, *( this->meshDependentData ) );
   mainTimer.stop();
   this->allIterations += this->odeSolver->getIterations();
   return true;
}

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
void
ExplicitTimeStepper< Problem, OdeSolver >::
getExplicitRHS( const RealType& time,
                const RealType& tau,
                DofVectorPointer& u,
                DofVectorPointer& fu )
{
   this->preIterateTimer.start();
   if( ! this->problem->preIterate( time,
                                    tau,
                                    this->mesh,
                                    u,
                                    *( this->meshDependentData ) ) )
   {
      std::cerr << std::endl << "Preiteration failed." << std::endl;
      return;
      //return false; // TODO: throw exception
   }
   this->preIterateTimer.stop();
   this->explicitUpdaterTimer.start();
   this->problem->setExplicitBoundaryConditions( time, this->mesh, u, *( this->meshDependentData ) );
   this->problem->getExplicitRHS( time, tau, this->mesh, u, fu, *( this->meshDependentData ) );
   this->explicitUpdaterTimer.stop();
   this->postIterateTimer.start();
   if( ! this->problem->postIterate( time,
                                     tau,
                                     this->mesh,
                                     u,
                                     *( this->meshDependentData ) ) )
   {
      std::cerr << std::endl << "Postiteration failed." << std::endl;
      return;
      //return false; // TODO: throw exception
   }
   this->postIterateTimer.stop();
}

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
bool
ExplicitTimeStepper< Problem, OdeSolver >::
writeEpilog( Logger& logger )
{
   logger.writeParameter< long long int >( "Iterations count:", this->allIterations );
   logger.writeParameter< const char* >( "Pre-iterate time:", "" );
   this->preIterateTimer.writeLog( logger, 1 );
   logger.writeParameter< const char* >( "Explicit update computation:", "" );
   this->explicitUpdaterTimer.writeLog( logger, 1 );
   logger.writeParameter< const char* >( "Explicit time stepper time:", "" );
   this->mainTimer.writeLog( logger, 1 );
   logger.writeParameter< const char* >( "Post-iterate time:", "" );
   this->postIterateTimer.writeLog( logger, 1 );
   return true;
}

} // namespace PDE
} // namespace Solvers
} // namespace TNL