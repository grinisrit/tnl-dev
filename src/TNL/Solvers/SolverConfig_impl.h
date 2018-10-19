/***************************************************************************
                          SolverConfig_impl.h  -  description
                             -------------------
    begin                : Jul 8, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/tnlConfig.h>
#include <TNL/Solvers/SolverConfig.h>
#include <TNL/Solvers/BuildConfigTags.h>
#include <TNL/Solvers/DummyProblem.h>
#include <TNL/Solvers/PDE/ExplicitTimeStepper.h>
#include <TNL/Solvers/PDE/TimeDependentPDESolver.h>
#include <TNL/Solvers/Linear/SOR.h>
#include <TNL/Solvers/Linear/CG.h>
#include <TNL/Solvers/Linear/BICGStab.h>
#include <TNL/Solvers/Linear/BICGStabL.h>
#include <TNL/Solvers/Linear/GMRES.h>
#include <TNL/Solvers/Linear/TFQMR.h>
#include <TNL/Solvers/Linear/UmfpackWrapper.h>
#include <TNL/Solvers/Linear/Preconditioners/Diagonal.h>
#include <TNL/Solvers/Linear/Preconditioners/ILU0.h>
#include <TNL/Solvers/Linear/Preconditioners/ILUT.h>
#include <TNL/Matrices/CSR.h>
#include <TNL/Meshes/DistributedMeshes/DistributedGrid.h>

namespace TNL {
namespace Solvers {

template< typename ConfigTag,
          typename ProblemConfig >
bool SolverConfig< ConfigTag, ProblemConfig >::configSetup( Config::ConfigDescription& config )
{
   typedef DummyProblem< double, Devices::Host, int > DummyProblemType;

   config.addDelimiter( " === General parameters ==== " );
   config.addEntry< bool >( "catch-exceptions",
                            "Catch C++ exceptions. Disabling it allows the program to drop into the debugger "
                            "and track the origin of the exception.",
                            true );
   /****
    * Setup real type
    */
   config.addEntry< String >( "real-type",
                                 "Precision of the floating point arithmetics.",
                                 "double" );
   if( ConfigTagReal< ConfigTag, float >::enabled )
      config.addEntryEnum( "float" );
   if( ConfigTagReal< ConfigTag, double >::enabled )
      config.addEntryEnum( "double" );
   if( ConfigTagReal< ConfigTag, long double >::enabled )
      config.addEntryEnum( "long-double" );

   /****
    * Setup device.
    */
   config.addEntry< String >( "device",
                                 "Device to use for the computations.",
                                 "host" );
   if( ConfigTagDevice< ConfigTag, Devices::Host >::enabled )
      config.addEntryEnum( "host" );
#ifdef HAVE_CUDA
   if( ConfigTagDevice< ConfigTag, Devices::Cuda >::enabled )
      config.addEntryEnum( "cuda" );
#endif
   
#ifdef HAVE_MIC
   if( ConfigTagDevice< ConfigTag, Devices::MIC >::enabled )
      config.addEntryEnum( "mic" );
#endif
   

   /****
    * Setup index type.
    */
   config.addEntry< String >( "index-type",
                                 "Indexing type for arrays, vectors, matrices etc.",
                                 "int" );
   if( ConfigTagIndex< ConfigTag, short int >::enabled )
      config.addEntryEnum( "short-int" );

   if( ConfigTagIndex< ConfigTag, int >::enabled )
      config.addEntryEnum( "int" );

   if( ConfigTagIndex< ConfigTag, long int >::enabled )
      config.addEntryEnum( "long-int" );

   /****
    * Mesh file parameter
    */
   config.addDelimiter( " === Space discretisation parameters ==== " );
   config.addEntry< String >( "mesh", "A file which contains the numerical mesh. You may create it with tools like tnl-grid-setup or tnl-mesh-convert.", "mesh.tnl" );
   
   /****
    * Domain decomposition
    */
   Meshes::DistributedMeshes::DistributedMesh< Meshes::Grid< 3 > >::configSetup( config );

   /****
    * Time discretisation
    */
   config.addDelimiter( " === Time discretisation parameters ==== " );
   typedef PDE::ExplicitTimeStepper< DummyProblemType, ODE::Euler > ExplicitTimeStepper;
   typedef Solvers::DummySolver DiscreteSolver;
   PDE::TimeDependentPDESolver< DummyProblemType, ExplicitTimeStepper >::configSetup( config );
   ExplicitTimeStepper::configSetup( config );
   if( ConfigTagTimeDiscretisation< ConfigTag, ExplicitTimeDiscretisationTag >::enabled ||
       ConfigTagTimeDiscretisation< ConfigTag, SemiImplicitTimeDiscretisationTag >::enabled ||
       ConfigTagTimeDiscretisation< ConfigTag, ImplicitTimeDiscretisationTag >::enabled )
   {
      config.addRequiredEntry< String >( "time-discretisation", "Discratisation in time.");
      if( ConfigTagTimeDiscretisation< ConfigTag, ExplicitTimeDiscretisationTag >::enabled )
         config.addEntryEnum( "explicit" );
      if( ConfigTagTimeDiscretisation< ConfigTag, SemiImplicitTimeDiscretisationTag >::enabled )
         config.addEntryEnum( "semi-implicit" );
      if( ConfigTagTimeDiscretisation< ConfigTag, ImplicitTimeDiscretisationTag >::enabled )
         config.addEntryEnum( "implicit" );
   }
   config.addRequiredEntry< String >( "discrete-solver", "The solver of the discretised problem:" );
   if( ConfigTagTimeDiscretisation< ConfigTag, ExplicitTimeDiscretisationTag >::enabled )
   {
      if( ConfigTagExplicitSolver< ConfigTag, ExplicitEulerSolverTag >::enabled )
         config.addEntryEnum( "euler" );
      if( ConfigTagExplicitSolver< ConfigTag, ExplicitMersonSolverTag >::enabled )
         config.addEntryEnum( "merson" );
   }
   if( ConfigTagTimeDiscretisation< ConfigTag, SemiImplicitTimeDiscretisationTag >::enabled )
   {
      config.addEntryEnum( "cg" );
      config.addEntryEnum( "bicgstab" );
      config.addEntryEnum( "bicgstabl" );
      config.addEntryEnum( "gmres" );
      config.addEntryEnum( "tfqmr" );
      config.addEntryEnum( "sor" );
#ifdef HAVE_UMFPACK
      config.addEntryEnum( "umfpack" );
#endif
      config.addEntry< String >( "preconditioner", "The preconditioner for the discrete solver:", "none" );
      config.addEntryEnum( "none" );
      config.addEntryEnum( "diagonal" );
   // TODO: implement parallel ILU or device-dependent build config tags for preconditioners
#ifndef HAVE_CUDA
      config.addEntryEnum( "ilu0" );
      config.addEntryEnum( "ilut" );
#endif
   }
   if( ConfigTagTimeDiscretisation< ConfigTag, ExplicitTimeDiscretisationTag >::enabled ||
       ConfigTagTimeDiscretisation< ConfigTag, SemiImplicitTimeDiscretisationTag >::enabled )
   {
      config.addDelimiter( " === Iterative solvers parameters === " );
      IterativeSolver< double, int >::configSetup( config );
   }
   if( ConfigTagTimeDiscretisation< ConfigTag, ExplicitTimeDiscretisationTag >::enabled )
   {
      config.addDelimiter( " === Explicit solvers parameters === " );
      ODE::ExplicitSolver< DummyProblem< double, Devices::Host, int > >::configSetup( config );
      if( ConfigTagExplicitSolver< ConfigTag, ExplicitEulerSolverTag >::enabled )
         ODE::Euler< DummyProblem< double, Devices::Host, int > >::configSetup( config );

      if( ConfigTagExplicitSolver< ConfigTag, ExplicitMersonSolverTag >::enabled )
         ODE::Merson< DummyProblem< double, Devices::Host, int > >::configSetup( config );
   }
   if( ConfigTagTimeDiscretisation< ConfigTag, SemiImplicitTimeDiscretisationTag >::enabled )
   {
      config.addDelimiter( " === Semi-implicit solvers parameters === " );
      typedef Matrices::CSR< double, Devices::Host, int > MatrixType;
      Linear::CG< MatrixType >::configSetup( config );
      Linear::BICGStab< MatrixType >::configSetup( config );
      Linear::BICGStabL< MatrixType >::configSetup( config );
      Linear::GMRES< MatrixType >::configSetup( config );
      Linear::TFQMR< MatrixType >::configSetup( config );
      Linear::SOR< MatrixType >::configSetup( config );

      Linear::Preconditioners::Diagonal< MatrixType >::configSetup( config );
      Linear::Preconditioners::ILU0< MatrixType >::configSetup( config );
      Linear::Preconditioners::ILUT< MatrixType >::configSetup( config );
   }

   config.addDelimiter( " === Logs and messages ===" );
   config.addEntry< int >( "verbose", "Set the verbose mode. The higher number the more messages are generated.", 1 );
   config.addEntry< String >( "log-file", "Log file for the computation.", "log.txt" );
   config.addEntry< int >( "log-width", "Number of columns of the log table.", 80 );
   return true;

}

} // namespace Solvers
} // namespace TNL
