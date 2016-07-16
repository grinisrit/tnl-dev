/***************************************************************************
                          tnlSORSolverOld.h  -  description
                             -------------------
    begin                : 2007/07/30
    copyright            : (C) 2007 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef tnlSORSolverOldH
#define tnlSORSolverOldH

#include <math.h>
#include <legacy/solvers/tnlMatrixSolver.h>

template< typename Real, typename Device = tnlHost, typename Index = int >
class tnlSORSolverOld : public tnlMatrixSolver< Real, Device, Index >
{
   public:
 
   tnlSORSolverOld( const tnlString& name );

   tnlString getType() const;

   void setSOROmega( const Real& omega );

   Real getSOROmega( ) const;

   bool solve( const tnlMatrix< Real, Device, Index >& A,
               const tnlVector< Real, Device, Index >& b,
               tnlVector< Real, Device, Index >& x,
               const Real& max_residue,
               const Index max_iterations,
               tnlPreconditioner< Real >* precond = 0 );

   protected:

   Real sorOmega;
};

template< typename Real, typename Device, typename Index >
tnlSORSolverOld< Real, Device, Index > :: tnlSORSolverOld( const tnlString& name )
: tnlMatrixSolver< Real, Device, Index >( name ),
  sorOmega( 1.0 )
  {
  }


template< typename Real, typename Device, typename Index >
tnlString tnlSORSolverOld< Real, Device, Index > :: getType() const
{
   return tnlString( "tnlSORSolverOld< " ) +
          tnlString( getType( ( Real ) 0.0 ) ) +
          tnlString( ", " ) +
          Device :: getDeviceType() +
          tnlString( ", " ) +
          tnlString( getType( ( Index ) 0 ) ) +
          tnlString( " >" );
}

template< typename Real, typename Device, typename Index >
void tnlSORSolverOld< Real, Device, Index > :: setSOROmega( const Real& omega )
{
   this->sorOmega = omega;
}

template< typename Real, typename Device, typename Index >
Real tnlSORSolverOld< Real, Device, Index > :: getSOROmega( ) const
{
   return this->sorOmega;
}

template< typename Real, typename Device, typename Index >
bool tnlSORSolverOld< Real, Device, Index > :: solve( const tnlMatrix< Real, Device, Index >& A,
                                                   const tnlVector< Real, Device, Index >& b,
                                                   tnlVector< Real, Device, Index >& x,
                                                   const Real& max_residue,
                                                   const Index max_iterations,
                                                   tnlPreconditioner< Real >* precond )
{
   const Index size = A. getSize();

   this->iteration = 0;
   this->residue = max_residue + 1.0;;

   Real bNorm = b. lpNorm( ( Real ) 2.0 );

   while( this->iteration < max_iterations &&
          max_residue < this->residue )
   {
      A. performSORIteration( this->sorOmega,
                              b,
                              x,
                              0,
                              size );
      if( this->iteration % 10 == 0 )
      {
         this->residue = this->getResidue( A, b, x, bNorm );
         if( this->verbosity > 1 )
            this->printOut();
      }
      this->iteration ++;
   }
   if( this->verbosity > 0 )
   {
      this->residue = this->getResidue( A, b, x, bNorm );
      this->printOut();
   }
   if( this->iteration <= max_iterations ) return true;
   return false;
};

#endif
