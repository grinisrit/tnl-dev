/***************************************************************************
                          navierStokesProblem_impl.h  -  description
                             -------------------
    begin                : Feb 13, 2017
    copyright            : (C) 2017 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/FileName.h>
#include <TNL/Matrices/MatrixSetter.h>
#include <TNL/Solvers/PDE/ExplicitUpdater.h>
#include <TNL/Solvers/PDE/LinearSystemAssembler.h>
#include <TNL/Solvers/PDE/BackwardTimeDiscretisation.h>
#include <TNL/Functions/Analytic/VectorNorm.h>

#include "RiemannProblemInitialCondition.h"
#include "CompressibleConservativeVariables.h"
#include "PhysicalVariablesGetter.h"
#include "navierStokesProblem.h"

#include "UpwindContinuity.h"
#include "UpwindEnergy.h"
#include "UpwindMomentumX.h"
#include "UpwindMomentumY.h"
#include "UpwindMomentumZ.h"

namespace TNL {

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
String
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
getTypeStatic()
{
   return String( "navierStokesProblem< " ) + Mesh :: getTypeStatic() + " >";
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
String
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
getPrologHeader() const
{
   return String( "Inviscid flow solver" );
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
void
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
writeProlog( Logger& logger, const Config::ParameterContainer& parameters ) const
{
   /****
    * Add data you want to have in the computation report (log) as follows:
    * logger.writeParameter< double >( "Parameter description", parameter );
    */
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
bool
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
   if( ! this->inviscidOperatorsPointer->setup( this->getMesh(), parameters, prefix + "inviscid-operators-" ) ||
       ! this->boundaryConditionPointer->setup( this->getMesh(), parameters, prefix + "boundary-conditions-" ) ||
       ! this->rightHandSidePointer->setup( parameters, prefix + "right-hand-side-" ) )
      return false;
   this->gamma = parameters.getParameter< double >( "gamma" );
   velocity->setMesh( this->getMesh() );
   pressure->setMesh( this->getMesh() );
   return true;
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
typename navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::IndexType
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
getDofs() const
{
   /****
    * Return number of  DOFs (degrees of freedom) i.e. number
    * of unknowns to be resolved by the main solver.
    */
   return this->conservativeVariables->getDofs( this->getMesh() );
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
void
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
bindDofs( DofVectorPointer& dofVector )
{
   this->conservativeVariables->bind( this->getMesh(), dofVector );
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
bool
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
setInitialCondition( const Config::ParameterContainer& parameters,
                     DofVectorPointer& dofs )
{
   CompressibleConservativeVariables< MeshType > conservativeVariables;
   conservativeVariables.bind( this->getMesh(), dofs );
   const String& initialConditionType = parameters.getParameter< String >( "initial-condition" );
   this->speedIncrementUntil = parameters.getParameter< RealType >( "speed-increment-until" );
   this->speedIncrement = parameters.getParameter< RealType >( "speed-increment" );
   this->cavitySpeed = parameters.getParameter< RealType >( "cavity-speed" );
   if( initialConditionType == "riemann-problem" )
   {
      RiemannProblemInitialCondition< MeshType > initialCondition;
      if( ! initialCondition.setup( parameters ) )
         return false;
      initialCondition.setInitialCondition( conservativeVariables );
      return true;
   }
   std::cerr << "Unknown initial condition " << initialConditionType << std::endl;
   return false;
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
   template< typename Matrix >
bool
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
setupLinearSystem( Matrix& matrix )
{
/*   const IndexType dofs = this->getDofs( mesh );
   typedef typename Matrix::CompressedRowLengthsVector CompressedRowLengthsVectorType;
   CompressedRowLengthsVectorType rowLengths;
   if( ! rowLengths.setSize( dofs ) )
      return false;
   MatrixSetter< MeshType, DifferentialOperator, BoundaryCondition, CompressedRowLengthsVectorType > matrixSetter;
   matrixSetter.template getCompressedRowLengths< typename Mesh::Cell >( mesh,
                                                                          differentialOperator,
                                                                          boundaryCondition,
                                                                          rowLengths );
   matrix.setDimensions( dofs, dofs );
   if( ! matrix.setCompressedRowLengths( rowLengths ) )
      return false;*/
   return true;
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
bool
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
makeSnapshot( const RealType& time,
              const IndexType& step,
              DofVectorPointer& dofs )
{
  std::cout << std::endl << "Writing output at time " << time << " step " << step << "." << std::endl;
  
  this->bindDofs( dofs );
  PhysicalVariablesGetter< MeshType > physicalVariablesGetter;
  physicalVariablesGetter.getVelocity( this->conservativeVariables, this->velocity );
  physicalVariablesGetter.getPressure( this->conservativeVariables, this->gamma, this->pressure );
  
   FileName fileName;
   fileName.setExtension( "tnl" );
   fileName.setIndex( step );
   fileName.setFileNameBase( "density-" );
   if( ! this->conservativeVariables->getDensity()->save( fileName.getFileName() ) )
      return false;
   
   fileName.setFileNameBase( "velocity-" );
   if( ! this->velocity->save( fileName.getFileName() ) )
      return false;

   fileName.setFileNameBase( "pressure-" );
   if( ! this->pressure->save( fileName.getFileName() ) )
      return false;

   /*fileName.setFileNameBase( "energy-" );
   if( ! this->conservativeVariables->getEnergy()->save( fileName.getFileName() ) )
      return false;

   fileName.setFileNameBase( "momentum-" );
   if( ! this->conservativeVariables->getMomentum()->save( fileName.getFileName() ) )
      return false;*/
   
   return true;
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
void
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
getExplicitUpdate( const RealType& time,
                   const RealType& tau,
                   DofVectorPointer& _u,
                   DofVectorPointer& _fu )
{
    typedef typename MeshType::Cell Cell;
    const MeshPointer& mesh = this->getMesh();
    
    /****
     * Bind DOFs
     */
    this->conservativeVariables->bind( mesh, _u );
    this->conservativeVariablesRHS->bind( mesh, _fu );
    this->velocity->setMesh( mesh );
    this->pressure->setMesh( mesh );

//   this->pressure->write( "pressure1", "gnuplot" );
//   getchar();
    /****
     * Resolve the physical variables
     */
    PhysicalVariablesGetter< typename MeshPointer::ObjectType > physicalVariables;
    physicalVariables.getVelocity( this->conservativeVariables, this->velocity );
    physicalVariables.getPressure( this->conservativeVariables, this->gamma, this->pressure );
    
   /****
    * Set-up operators
    */
   typedef typename InviscidOperators::ContinuityOperatorType ContinuityOperatorType;
   typedef typename InviscidOperators::MomentumXOperatorType MomentumXOperatorType;
   typedef typename InviscidOperators::MomentumYOperatorType MomentumYOperatorType;
   typedef typename InviscidOperators::MomentumZOperatorType MomentumZOperatorType;
   typedef typename InviscidOperators::EnergyOperatorType EnergyOperatorType;
    
    this->inviscidOperatorsPointer->setTau( tau );
    this->inviscidOperatorsPointer->setVelocity( this->velocity );
    this->inviscidOperatorsPointer->setPressure( this->pressure );
    this->inviscidOperatorsPointer->setDensity( this->conservativeVariables->getDensity() );
    this->inviscidOperatorsPointer->setGamma( this->gamma );

   /****
    * Set Up Boundary Conditions
    */

   typedef typename BoundaryCondition::DensityBoundaryConditionsType DensityBoundaryConditionsType;
   typedef typename BoundaryCondition::MomentumXBoundaryConditionsType MomentumXBoundaryConditionsType;
   typedef typename BoundaryCondition::MomentumYBoundaryConditionsType MomentumYBoundaryConditionsType;
   typedef typename BoundaryCondition::MomentumZBoundaryConditionsType MomentumZBoundaryConditionsType;
   typedef typename BoundaryCondition::EnergyBoundaryConditionsType EnergyBoundaryConditionsType;

   /****
    * Update Boundary Conditions
    */
   if(this->speedIncrementUntil > time )
   {
      this->boundaryConditionPointer->setTimestep(this->speedIncrement);
   }
   else
   {
      this->boundaryConditionPointer->setTimestep(0);
   }
   this->boundaryConditionPointer->setSpeed(this->cavitySpeed);
   this->boundaryConditionPointer->setCompressibleConservativeVariables(this->conservativeVariables);
   this->boundaryConditionPointer->setGamma(this->gamma);
   this->boundaryConditionPointer->setPressure(this->pressure);


   /****
    * Continuity equation
    */ 
   Solvers::PDE::ExplicitUpdater< Mesh, MeshFunctionType, ContinuityOperatorType, DensityBoundaryConditionsType, RightHandSide > explicitUpdaterContinuity; 
   explicitUpdaterContinuity.setDifferentialOperator( this->inviscidOperatorsPointer->getContinuityOperator() );
   explicitUpdaterContinuity.setBoundaryConditions( this->boundaryConditionPointer->getDensityBoundaryCondition() );
   explicitUpdaterContinuity.setRightHandSide( this->rightHandSidePointer );
   explicitUpdaterContinuity.template update< typename Mesh::Cell, CommunicatorType >(
      time, tau, mesh, 
      this->conservativeVariables->getDensity(),
      this->conservativeVariablesRHS->getDensity() );

   /****
    * Momentum equations
    */
   Solvers::PDE::ExplicitUpdater< Mesh, MeshFunctionType, MomentumXOperatorType, MomentumXBoundaryConditionsType, RightHandSide > explicitUpdaterMomentumX; 
   explicitUpdaterMomentumX.setDifferentialOperator( this->inviscidOperatorsPointer->getMomentumXOperator() );
   explicitUpdaterMomentumX.setBoundaryConditions( this->boundaryConditionPointer->getMomentumXBoundaryCondition() );
   explicitUpdaterMomentumX.setRightHandSide( this->rightHandSidePointer );   
   explicitUpdaterMomentumX.template update< typename Mesh::Cell, CommunicatorType >(
      time, tau, mesh,
      ( *this->conservativeVariables->getMomentum() )[ 0 ], // uRhoVelocityX,
      ( *this->conservativeVariablesRHS->getMomentum() )[ 0 ] ); //, fuRhoVelocityX );

   if( Dimensions > 1 )
   {
      Solvers::PDE::ExplicitUpdater< Mesh, MeshFunctionType, MomentumYOperatorType, MomentumYBoundaryConditionsType, RightHandSide > explicitUpdaterMomentumY;
      explicitUpdaterMomentumY.setDifferentialOperator( this->inviscidOperatorsPointer->getMomentumYOperator() );
      explicitUpdaterMomentumY.setBoundaryConditions( this->boundaryConditionPointer->getMomentumYBoundaryCondition() );
      explicitUpdaterMomentumY.setRightHandSide( this->rightHandSidePointer );         
      explicitUpdaterMomentumY.template update< typename Mesh::Cell, CommunicatorType >(
         time, tau, mesh,
         ( *this->conservativeVariables->getMomentum() )[ 1 ], // uRhoVelocityX,
         ( *this->conservativeVariablesRHS->getMomentum() )[ 1 ] ); //, fuRhoVelocityX );
   }
   
   if( Dimensions > 2 )
   {
      Solvers::PDE::ExplicitUpdater< Mesh, MeshFunctionType, MomentumZOperatorType, MomentumZBoundaryConditionsType, RightHandSide > explicitUpdaterMomentumZ;
      explicitUpdaterMomentumZ.setDifferentialOperator( this->inviscidOperatorsPointer->getMomentumZOperator() );
      explicitUpdaterMomentumZ.setBoundaryConditions( this->boundaryConditionPointer->getMomentumZBoundaryCondition() );
      explicitUpdaterMomentumZ.setRightHandSide( this->rightHandSidePointer );               
      explicitUpdaterMomentumZ.template update< typename Mesh::Cell, CommunicatorType >(
         time, tau, mesh,
         ( *this->conservativeVariables->getMomentum() )[ 2 ], // uRhoVelocityX,
         ( *this->conservativeVariablesRHS->getMomentum() )[ 2 ] ); //, fuRhoVelocityX );
   }
   
  
   /****
    * Energy equation
    */
   Solvers::PDE::ExplicitUpdater< Mesh, MeshFunctionType, EnergyOperatorType, EnergyBoundaryConditionsType, RightHandSide > explicitUpdaterEnergy;
   explicitUpdaterEnergy.setDifferentialOperator( this->inviscidOperatorsPointer->getEnergyOperator() );
   explicitUpdaterEnergy.setBoundaryConditions( this->boundaryConditionPointer->getEnergyBoundaryCondition() );
   explicitUpdaterEnergy.setRightHandSide( this->rightHandSidePointer );                  
   explicitUpdaterEnergy.template update< typename Mesh::Cell, CommunicatorType >(
      time, tau, mesh,
      this->conservativeVariablesRHS->getEnergy(), // uRhoVelocityX,
      this->conservativeVariablesRHS->getEnergy() ); //, fuRhoVelocityX );

/*   this->pressure->write( "pressure3", "gnuplot" );
   getchar();   
   this->conservativeVariablesRHS->getDensity()->write( "density", "gnuplot" );
   this->conservativeVariablesRHS->getEnergy()->write( "energy", "gnuplot" );
   this->conservativeVariablesRHS->getMomentum()->write( "momentum", "gnuplot", 0.05 );
   getchar();*/

}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
   template< typename Matrix >
void
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
assemblyLinearSystem( const RealType& time,
                      const RealType& tau,
                      DofVectorPointer& _u,
                      Matrix& matrix,
                      DofVectorPointer& b )
{
/*   LinearSystemAssembler< Mesh,
                             MeshFunctionType,
                             InviscidOperators,
                             BoundaryCondition,
                             RightHandSide,
                             BackwardTimeDiscretisation,
                             Matrix,
                             DofVectorType > systemAssembler;

   MeshFunction< Mesh > u( mesh, _u );
   systemAssembler.template assembly< typename Mesh::Cell >( time,
                                                             tau,
                                                             mesh,
                                                             this->differentialOperator,
                                                             this->boundaryCondition,
                                                             this->rightHandSide,
                                                             u,
                                                             matrix,
                                                             b );*/
}

template< typename Mesh,
          typename BoundaryCondition,
          typename RightHandSide,
          typename InviscidOperators,
          typename Communicator >
bool
navierStokesProblem< Mesh, BoundaryCondition, RightHandSide, InviscidOperators, Communicator >::
postIterate( const RealType& time,
             const RealType& tau,
             DofVectorPointer& dofs )
{
   /*
    typedef typename MeshType::Cell Cell;
    int count = mesh->template getEntitiesCount< Cell >()/4;
	//bind _u
    this->_uRho.bind( *dofs, 0, count);
    this->_uRhoVelocityX.bind( *dofs, count, count);
    this->_uRhoVelocityY.bind( *dofs, 2 * count, count);
    this->_uEnergy.bind( *dofs, 3 * count, count);

   MeshFunctionType velocity( mesh, this->velocity );
   MeshFunctionType velocityX( mesh, this->velocityX );
   MeshFunctionType velocityY( mesh, this->velocityY );
   MeshFunctionType pressure( mesh, this->pressure );
   MeshFunctionType uRho( mesh, _uRho ); 
   MeshFunctionType uRhoVelocityX( mesh, _uRhoVelocityX ); 
   MeshFunctionType uRhoVelocityY( mesh, _uRhoVelocityY ); 
   MeshFunctionType uEnergy( mesh, _uEnergy ); 
   //Generating differential operators
   Velocity navierStokes2DVelocity;
   VelocityX navierStokes2DVelocityX;
   VelocityY navierStokes2DVelocityY;
   Pressure navierStokes2DPressure;

   //velocityX
   navierStokes2DVelocityX.setRhoVelX(uRhoVelocityX);
   navierStokes2DVelocityX.setRho(uRho);
//   OperatorFunction< VelocityX, MeshFunction, void, true > OFVelocityX;
//   velocityX = OFVelocityX;

   //velocityY
   navierStokes2DVelocityY.setRhoVelY(uRhoVelocityY);
   navierStokes2DVelocityY.setRho(uRho);
//   OperatorFunction< VelocityY, MeshFunction, void, time > OFVelocityY;
//   velocityY = OFVelocityY;

   //velocity
   navierStokes2DVelocity.setVelX(velocityX);
   navierStokes2DVelocity.setVelY(velocityY);
//   OperatorFunction< Velocity, MeshFunction, void, time > OFVelocity;
//   velocity = OFVelocity;

   //pressure
   navierStokes2DPressure.setGamma(gamma);
   navierStokes2DPressure.setVelocity(velocity);
   navierStokes2DPressure.setEnergy(uEnergy);
   navierStokes2DPressure.setRho(uRho);
//   OperatorFunction< navierStokes2DPressure, MeshFunction, void, time > OFPressure;
//   pressure = OFPressure;
    */
   return true;
}

} // namespace TNL

