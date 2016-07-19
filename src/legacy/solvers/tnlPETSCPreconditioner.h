/***************************************************************************
                          tnlPETSCPreconditioner.h  -  description
                             -------------------
    begin                : 2008/05/13
    copyright            : (C) 2008 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef tnlPETSCPreconditionerH
#define tnlPETSCPreconditionerH

#include <legacy/solvers/tnlPreconditioner.h>

#ifdef HAVE_PETSC
#include <petscksp.h>
#else
struct PC;
#endif

#include <iostream>

using namespace std;

//! This class wraps PETSc preconditioners
template< typename T > class tnlPETSCPreconditioner : public tnlPreconditioner< T >
{
#ifdef HAVE_PETSC
   PC petsc_preconditioner;
#endif

   public:

   tnlPETSCPreconditioner( const char* type )
   {
#ifdef HAVE_PETSC
      PCCreate( MPI_COMM_SELF, &petsc_preconditioner );
      if( strcmp( type, "none" ) == 0 )
         PCSetType( petsc_preconditioner, PCNONE );
      if( strcmp( type, "lu" ) == 0 )
         PCSetType( petsc_preconditioner, PCLU );
      if( strcmp( type, "ilu" ) == 0 )
         PCSetType( petsc_preconditioner, PCILU );
#else
      std::cerr << "Missing support for PETSC at the file " << __FILE__ << " line " << __LINE__ << std::endl;
#endif
   };
 
   void GetData( PC& precond ) const
   {
#ifdef HAVE_PETSC
      precond = petsc_preconditioner;
#else
      std::cerr << "Missing support for PETSC at the file " << __FILE__ << " line " << __LINE__ << std::endl;
#endif
   };
 
   bool Solve( const T* b, T* x ) const
   {
      std::cerr << "Do not call tnlPETSCPreconditioner :: Solve. Connect it to tnlPETSCSolver using tnlPETSCPreconditioner :: GetData( PC& precond )." << std::endl;
      abort();
   };

   ~tnlPETSCPreconditioner()
   {
#ifdef HAVE_PETSC
      PCDestroy( petsc_preconditioner );
#else
      std::cerr << "Missing support for PETSC at the file " << __FILE__ << " line " << __LINE__ << std::endl;
#endif
   };
};

#endif
