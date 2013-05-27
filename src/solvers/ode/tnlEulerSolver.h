/***************************************************************************
                          tnlEulerSolver.h  -  description
                             -------------------
    begin                : 2008/04/01
    copyright            : (C) 2008 by Tomas Oberhuber
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

#ifndef tnlEulerSolverH
#define tnlEulerSolverH

#include <math.h>
#include <solvers/ode/tnlExplicitSolver.h>

template< typename Problem >
class tnlEulerSolver : public tnlExplicitSolver< Problem >
{
   public:

   typedef Problem  ProblemType;
   typedef typename Problem :: DofVectorType DofVectorType;
   typedef typename Problem :: RealType RealType;
   typedef typename Problem :: DeviceType DeviceType;
   typedef typename Problem :: IndexType IndexType;


   tnlEulerSolver();

   tnlString getType() const;

   void setCFLCondition( const RealType& cfl );

   const RealType& getCFLCondition() const;

   bool solve( DofVectorType& u );

   protected:
   void computeNewTimeLevel( DofVectorType& u,
                             RealType tau,
                             RealType& currentResidue );

   
   DofVectorType k1;

   RealType cflCondition;
};

#include <implementation/solvers/ode/tnlEulerSolver_impl.h>

#endif