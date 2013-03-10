/***************************************************************************
                          simpleProblemSolver_impl.h  -  description
                             -------------------
    begin                : Mar 10, 2013
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

#ifndef SIMPLEPROBLEMSOLVER_IMPL_H_
#define SIMPLEPROBLEMSOLVER_IMPL_H_

#include <core/mfilename.h>

template< typename Mesh >
tnlString simpleProblemSolver< Mesh> :: getTypeStatic()
{
   /****
    * Replace 'simpleProblemSolver' by the name of your solver.
    */
   return tnlString( "simpleProblemSolver< " ) + Mesh :: getTypeStatic() + " >";
}

template< typename Mesh >
tnlString simpleProblemSolver< Mesh> :: getPrologHeader() const
{
   /****
    * Replace 'Simple Problem' by the your desired title of the log table.
    */
   return tnlString( "Simple Problem" );
}

template< typename Mesh >
void simpleProblemSolver< Mesh> :: writeProlog( tnlLogger& logger,
                                                const tnlParameterContainer& parameters ) const
{
   /****
    * In prolog, write all input parameters which define the numerical simulation.
    * Use methods:
    *
    *    logger. writeParameters< Type >( "Label:", "name", parameters );
    *
    *  or
    *
    *    logger. writeParameter< Type >( "Label:", value );
    *
    *  See tnlLogger.h for more details.
    */

   logger. WriteParameter< tnlString >( "Problem name:", "problem-name", parameters );
   logger. WriteParameter< int >( "Simple parameter:", 1 );
}

template< typename Mesh >
bool simpleProblemSolver< Mesh> :: init( const tnlParameterContainer& parameters )
{
   /****
    * Set-up your solver here. It means:
    * 1. Read input parameters and model coefficients like these
    */
   const tnlString& problemName = parameters. GetParameter< tnlString >( "problem-name" );

   /****
    * 2. Set-up geometry of the problem domain using some mesh like tnlGrid.
    */

   /****
    * 3. Set-up DOFs and supporting grid functions
    */
   dofVector. setSize( 100 );
   /****
    * You may use tnlSharedVector if you need to split the dofVector into more
    * grid functions like the following example:
    */

   const IndexType& dofs = 50;
   /****
    * Usually you will replace this by:
    *
    *    const IndexType& dofs = mesh. getDofs();
    *
    */

   this -> u. bind( & dofVector. getData()[ 0 * dofs ], dofs );
   this -> v. bind( & dofVector. getData()[ 1 * dofs ], dofs );
   /****
    * You may now treat u and v as usual vectors and indirectly work with this -> dofVector.
    */

   return true;
}

template< typename Mesh >
bool simpleProblemSolver< Mesh> :: setInitialCondition( const tnlParameterContainer& parameters )
{
   /****
    * Set the initial condition here. Manipulate only this -> dofVector.
    */

   return true;
}

template< typename Mesh >
bool simpleProblemSolver< Mesh> :: makeSnapshot( const RealType& time, const IndexType& step )
{
   /****
    * Use this method to write state of the solver to file(s).
    * All data are stored in this -> dofVector. You may use
    * supporting vectors and bind them with the dofVector as before.
    */
   cout << endl << "Writing output at time " << time << " step " << step << "." << endl;

   /****
    * Now write them to files.
    */
   tnlString fileName;
   FileNameBaseNumberEnding( "u-", step, 5, ".tnl", fileName );
   if( ! this -> u. save( fileName ) )
      return false;

   FileNameBaseNumberEnding( "v-", step, 5, ".tnl", fileName );
   if( ! this -> v. save( fileName ) )
      return false;

   return true;
}

template< typename Mesh >
typename simpleProblemSolver< Mesh> :: DofVectorType& simpleProblemSolver< Mesh> :: getDofVector()
{
   /****
    * You do not need to change this usually.
    */
   return dofVector;
}

template< typename Mesh >
void simpleProblemSolver< Mesh> :: GetExplicitRHS( const RealType& time,
                                                   const RealType& tau,
                                                   DofVectorType& _u,
                                                   DofVectorType& _fu )
{
   /****
    * If you use an explicit solver like tnlEulerSolver or tnlMersonSolver, you
    * need to implement this method. Compute the right-hand side of
    *
    *   d/dt u(x) = fu( x, u )
    *
    * You may use supporting vectors again if you need.
    */

   if( DeviceType :: getDevice() == tnlHostDevice )
   {
      /****
       *  Write the host solver here.
       */
   }
#ifdef HAVE_CUDA
   if( DeviceType :: getDevice() == tnlCudaDevice )
   {
      /****
       * Write the CUDA solver here.
       */
   }
#endif
}

#endif /* SIMPLEPROBLEM_IMPL_H_ */
