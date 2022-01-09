// Copyright (c) 2004-2022 Tomáš Oberhuber et al.
//
// This file is part of TNL - Template Numerical Library (https://tnl-project.org/)
//
// SPDX-License-Identifier: MIT

#pragma once

#include <TNL/Config/ParameterContainer.h>
#include <TNL/Timer.h>
#include <TNL/Solvers/SolverMonitor.h>
#include <ostream>

namespace TNL {
namespace Solvers {

template< typename ConfigTag >
class SolverStarter
{
public:
   SolverStarter();

   template< typename Problem >
   static bool
   run( const Config::ParameterContainer& parameters );

   template< typename Solver >
   bool
   writeEpilog( std::ostream& str, const Solver& solver );

   template< typename Problem, typename TimeStepper >
   bool
   runPDESolver( Problem& problem, const Config::ParameterContainer& parameters );

protected:
   int logWidth;

   Timer ioTimer, computeTimer, totalTimer;
};

}  // namespace Solvers
}  // namespace TNL

#include <TNL/Solvers/SolverStarter.hpp>
