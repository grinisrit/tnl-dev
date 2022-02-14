// Copyright (c) 2004-2022 Tomáš Oberhuber et al.
//
// This file is part of TNL - Template Numerical Library (https://tnl-project.org/)
//
// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>

#include <TNL/Config/ConfigDescription.h>
#include <TNL/Solvers/ODE/StaticExplicitSolver.h>
#include <TNL/Config/ParameterContainer.h>
#include <TNL/Containers/StaticVector.h>

namespace TNL {
namespace Solvers {
namespace ODE {

/**
 * \brief Solver of ODEs with the first order of accuracy.
 *
 * This solver is based on the (Euler method)[https://en.wikipedia.org/wiki/Euler_method] for solving of
 * (ordinary differential equations)[https://en.wikipedia.org/wiki/Ordinary_differential_equation] having the
 * following form:
 *
 *  \f$ \frac{d \vec x}{dt} = \vec f( t, \vec x) \text{ on } (0,T) \f$
 *
 * \f$ \vec x( 0 )  = \vec x_{ini} \f$.
 *
 * It is supposed to be used when the unknown \f$ x \in R^n \f$ is expressed by a \ref Containers::StaticVector or it is a scalar, i.e.
 * \f$ x \in R \f$ expressed by a numeric type like `double` or `float`.
 *
 * For problems where \f$ \vec x\f$ is represented by \ref TNL::Containers::Vector,
 * see \ref TNL::Solvers::ODE::Euler.
 *
 * The following example demonstrates the use the solvers:
 *
 * \includelineno Solvers/ODE/StaticODESolver-SineExample.cpp
 *
 * The result looks as follows:
 *
 * \include StaticODESolver-SineExample.out
 *
 * Since this variant of the Euler solver is static, it can be used even inside of GPU kernels and so combined with \ref TNL::Algorithms::ParallelFor
 * as demonstrated by the following example:
 *
 * \includelineno Solvers/ODE/StaticODESolver-SineParallelExample.h
 *
 * The result looks as follows:
 *
 * \include StaticODESolver-SineParallelExample.out
 *
 * \tparam Real is floating point number type, it is type of \f$ x \f$ in this case.
 */
template< typename Real >
class StaticEuler : public StaticExplicitSolver< Real, int >
{
   public:

      /**
       * \brief Type of floating-point arithemtics.
       */
      using RealType = Real;

      /**
       * \brief Type for indexing.
       */
      using IndexType  = int;

      /**
       * \brief Type of unknown variable \f$ x \f$.
       */
      using VectorType = Real;

      /**
       * \brief Another alias for type of unknown variable \f$ x \f$.
       */
      using DofVectorType = VectorType;

      /**
       * \brief Default constructor.
       */
      __cuda_callable__
      StaticEuler() = default;

      /**
       * \brief Static method for setup of configuration parameters.
       * 
       * \param config is the config description.
       * \param prefix is the prefix of the configuration parameters for this solver.
       */
      static void configSetup( Config::ConfigDescription& config,
                               const String& prefix = "" );

      /**
       * \brief 
       * 
       * \param parameters is the container for configuration parameters.
       * \param prefix is the prefix of the configuration parameters for this solver.
       * \return true if the parameters where parsed sucessfuly.
       * \return false if the method did not succeed to read the configuration parameters.
       */
      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" );

      /**
       * \brief This method sets the Courant number in the (CFL condition)[https://en.wikipedia.org/wiki/Courant%E2%80%93Friedrichs%E2%80%93Lewy_condition].
       * 
       * This method sets the constant \f$ C \f$ in the Courant–Friedrichs–Lewy condition. It means that
       * 
       * \f[ \Delta t = \frac{C}{\| f( t,x )\|} \f],
       * 
       * if \f$ C > 0\f$. If \f$ C = 0 \f$ the time step stays fixed.
       * 
       * \param c is the Courant number.
       */
      __cuda_callable__
      void setCourantNumber( const RealType& c );

      /**
       * \brief Getter for the Courant number.
       * 
       * \return the Courant number.
       */
      __cuda_callable__
      const RealType& getCourantNumber() const;

      /**
       * \brief Solve ODE given by a lambda function.
       * 
       * \tparam RHSFunction 
       * \param u 
       * \param rhs 
       * \return 'true' if steady state solution has been reached, 'false' otherwise.
       */
      template< typename RHSFunction, typename... Args >
      __cuda_callable__
      bool solve( VectorType& u, RHSFunction&& rhs, Args... args );

   protected:
      DofVectorType k1;

      RealType courantNumber = 0.0;
};


template< int Size_,
          typename Real >
class StaticEuler< Containers::StaticVector< Size_, Real > >
    : public StaticExplicitSolver< Real, int >
{
   public:

      static constexpr int Size = Size_;
      using RealType = Real;
      using IndexType  = int;
      using VectorType = TNL::Containers::StaticVector< Size, Real >;
      using DofVectorType = VectorType;

      __cuda_callable__
      StaticEuler() = default;

      static void configSetup( Config::ConfigDescription& config,
                               const String& prefix = "" );

      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" );

      __cuda_callable__
      void setCourantNumber( const RealType& cfl );

      __cuda_callable__
      const RealType& getCourantNumber() const;

      template< typename RHSFunction, typename... Args >
      __cuda_callable__
      bool solve( VectorType& u, RHSFunction&& rhs, Args... args );

   protected:
      DofVectorType k1;

      RealType courantNumber = 0.0;
};

} // namespace ODE
} // namespace Solvers
} // namespace TNL

#include <TNL/Solvers/ODE/StaticEuler.hpp>
