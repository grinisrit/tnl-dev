/***************************************************************************
                          TimeIndependentPDESolver_impl.h  -  description
                             -------------------
    begin                : Jan 15, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#pragma once 

#include <TNL/Solvers/PDE/TimeIndependentPDESolver.h>

namespace TNL {
namespace Solvers {
namespace PDE {   


template< typename Problem,
          typename DiscreteSolver >
TimeIndependentPDESolver< Problem, DiscreteSolver >::
TimeIndependentPDESolver()
: problem( 0 )
{
}

template< typename Problem,
          typename DiscreteSolver >
void
TimeIndependentPDESolver< Problem, DiscreteSolver >::
configSetup( Config::ConfigDescription& config,
             const String& prefix )
{
}

template< typename Problem,
          typename DiscreteSolver >
bool
TimeIndependentPDESolver< Problem, DiscreteSolver >::
setup( const Config::ParameterContainer& parameters,
       const String& prefix )
{
   /****
    * Load the mesh from the mesh file
    */
   const String& meshFile = parameters.getParameter< String >( "mesh" );
   std::cout << "Loading a mesh from the file " << meshFile << "...";
   if( ! this->mesh->load( meshFile ) )
   {
      std::cerr << std::endl;
      std::cerr << "I am not able to load the mesh from the file " << meshFile << "." << std::endl;
      std::cerr << " You may create it with tools like tnl-grid-setup or tnl-mesh-convert." << std::endl;
      return false;
   }
   std::cout << " [ OK ] " << std::endl;

   /****
    * Set DOFs (degrees of freedom)
    */
   TNL_ASSERT_GT( problem->getDofs( this->mesh ), 0, "number of DOFs must be positive" );
   this->dofs->setSize( problem->getDofs( this->mesh ) );
   this->dofs->setValue( 0.0 );
   this->problem->bindDofs( this->mesh, this->dofs );   
   
   /****
    * Set mesh dependent data
    */
   this->problem->setMeshDependentData( this->mesh, this->meshDependentData );
   this->problem->bindMeshDependentData( this->mesh, this->meshDependentData );
   
   /***
    * Set-up the initial condition
    */
   std::cout << "Setting up the initial condition ... ";
   typedef typename Problem :: DofVectorType DofVectorType;
   if( ! this->problem->setInitialCondition( parameters, this->mesh, this->dofs, this->meshDependentData ) )
      return false;
   std::cout << " [ OK ]" << std::endl;
   
   return true;
}

template< typename Problem,
          typename DiscreteSolver >
bool
TimeIndependentPDESolver< Problem, DiscreteSolver >::
writeProlog( Logger& logger,
             const Config::ParameterContainer& parameters )
{
   logger.writeHeader( problem->getPrologHeader() );
   problem->writeProlog( logger, parameters );
   logger.writeSeparator();
   mesh->writeProlog( logger );
   logger.writeSeparator();
   const String& solverName = parameters. getParameter< String >( "discrete-solver" );
   logger.writeParameter< String >( "Discrete solver:", "discrete-solver", parameters );
   if( solverName == "sor" )
      logger.writeParameter< double >( "Omega:", "sor-omega", parameters, 1 );
   if( solverName == "gmres" )
      logger.writeParameter< int >( "Restarting:", "gmres-restarting", parameters, 1 );
   logger.writeParameter< double >( "Convergence residue:", "convergence-residue", parameters );
   logger.writeParameter< double >( "Divergence residue:", "divergence-residue", parameters );
   logger.writeParameter< int >( "Maximal number of iterations:", "max-iterations", parameters );
   logger.writeParameter< int >( "Minimal number of iterations:", "min-iterations", parameters );
   logger.writeSeparator();
   logger.writeParameter< String >( "Real type:", "real-type", parameters, 0 );
   logger.writeParameter< String >( "Index type:", "index-type", parameters, 0 );
   logger.writeParameter< String >( "Device:", "device", parameters, 0 );
   logger.writeSeparator();
   logger.writeSystemInformation( parameters );
   logger.writeSeparator();
   logger.writeCurrentTime( "Started at:" );
   logger.writeSeparator();
   return true;
}

template< typename Problem,
          typename DiscreteSolver >
void
TimeIndependentPDESolver< Problem, DiscreteSolver >::
setProblem( ProblemType& problem )
{
   this->problem = &problem;
}

template< typename Problem,
          typename DiscreteSolver >
bool
TimeIndependentPDESolver< Problem, DiscreteSolver >::
solve()
{
   TNL_ASSERT_TRUE( problem, "No problem was set in tnlPDESolver." );

   this->computeTimer->reset();
   this->computeTimer->start();
   if( ! this->problem->solve( this->mesh, this->dofs ) )
   {
      this->computeTimer->stop();
      return false;
   }
   this->computeTimer->stop();
   return true;
}

template< typename Problem,
          typename DiscreteSolver >
bool
TimeIndependentPDESolver< Problem, DiscreteSolver >::
writeEpilog( Logger& logger ) const
{
   return this->problem->writeEpilog( logger );
}

} // namespace PDE
} // namespace Solvers
} // namespace TNL