/***************************************************************************
                          tnlExplicitTimeStepper.h  -  description
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

#ifndef TNLEXPLICITTIMESTEPPER_H_
#define TNLEXPLICITTIMESTEPPER_H_

#include <solvers/ode/tnlODESolverMonitor.h>

template< typename Problem,
          template < typename OdeProblem > class OdeSolver >
class tnlExplicitTimeStepper
{
   public:

   typedef Problem ProblemType;
   typedef OdeSolver< ProblemType > OdeSolverType;
   typedef typename Problem :: RealType RealType;
   typedef typename Problem :: DeviceType DeviceType;
   typedef typename Problem :: IndexType IndexType;
   typedef typename ProblemType :: DofVectorType DofVectorType;

   tnlExplicitTimeStepper();

   void setSolver( OdeSolverType& odeSolver );

   void setProblem( ProblemType& problem );

   ProblemType* getProblem() const;

   bool setTau( const RealType& tau );

   const RealType& getTau() const;

   bool solve( const RealType& time,
               const RealType& stopTime );

   protected:

   OdeSolverType* odeSolver;

   Problem* problem;

   RealType tau;
};

#include <implementation/solvers/pde/tnlExplicitTimeStepper_impl.h>

#endif /* TNLEXPLICITTIMESTEPPER_H_ */
