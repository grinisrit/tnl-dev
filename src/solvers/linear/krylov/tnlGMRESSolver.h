/***************************************************************************
                          tnlGMRESSolver.h  -  description
                             -------------------
    begin                : 2007/07/31
    copyright            : (C) 2007 by Tomas Oberhuber
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

#ifndef tnlGMRESSolverH
#define tnlGMRESSolverH


#include <math.h>
#include <core/tnlObject.h>
#include <core/vectors/tnlVector.h>
#include <core/vectors/tnlSharedVector.h>
#include <solvers/preconditioners/tnlDummyPreconditioner.h>
#include <solvers/tnlIterativeSolver.h>
#include <solvers/linear/tnlLinearResidueGetter.h>

template< typename Matrix,
          typename Preconditioner = tnlDummyPreconditioner< typename Matrix :: RealType,
                                                            typename Matrix :: DeviceType,
                                                            typename Matrix :: IndexType> >
class tnlGMRESSolver : public tnlObject,
                       public tnlIterativeSolver< typename Matrix :: RealType,
                                                  typename Matrix :: IndexType >
{
   public:

   typedef typename Matrix :: RealType RealType;
   typedef typename Matrix :: IndexType IndexType;
   typedef typename Matrix :: DeviceType DeviceType;
   typedef Matrix MatrixType;
   typedef Preconditioner PreconditionerType;

   tnlGMRESSolver();

   tnlString getType() const;

   void setRestarting( IndexType rest );

   void setMatrix( const MatrixType& matrix );

   void setPreconditioner( const Preconditioner& preconditioner );

#ifdef HAVE_NOT_CXX11
   template< typename Vector,
             typename ResidueGetter >
   bool solve( const Vector& b, Vector& x );
#else
   template< typename Vector,
             typename ResidueGetter = tnlLinearResidueGetter< Matrix, Vector >  >
   bool solve( const Vector& b, Vector& x );
#endif

   ~tnlGMRESSolver();

   protected:

   template< typename Vector >
   void update( IndexType k,
                IndexType m,
                const tnlVector< RealType, tnlHost, IndexType >& H,
                const tnlVector< RealType, tnlHost, IndexType >& s,
                tnlVector< RealType, DeviceType, IndexType >& v,
                Vector& x );

   void generatePlaneRotation( RealType &dx,
                               RealType &dy,
                               RealType &cs,
                               RealType &sn );

   void applyPlaneRotation( RealType &dx,
                            RealType &dy,
                            RealType &cs,
                            RealType &sn );


   bool setSize( IndexType _size, IndexType m );

   tnlVector< RealType, DeviceType, IndexType > _r, _w, _v, _M_tmp;
   tnlVector< RealType, tnlHost, IndexType > _s, _cs, _sn, _H;

   IndexType size, restarting, maxIterations;

   RealType maxResidue;

   const MatrixType* matrix;
   const PreconditionerType* preconditioner;
};

#include <implementation/solvers/linear/krylov/tnlGMRESSolver_impl.h>

#include <matrices/tnlCSRMatrix.h>
#include <matrices/tnlEllpackMatrix.h>
#include <matrices/tnlMultiDiagonalMatrix.h>

extern template class tnlGMRESSolver< tnlCSRMatrix< float,  tnlHost, int > >;
extern template class tnlGMRESSolver< tnlCSRMatrix< double, tnlHost, int > >;
extern template class tnlGMRESSolver< tnlCSRMatrix< float,  tnlHost, long int > >;
extern template class tnlGMRESSolver< tnlCSRMatrix< double, tnlHost, long int > >;

/*extern template class tnlGMRESSolver< tnlEllpackMatrix< float,  tnlHost, int > >;
extern template class tnlGMRESSolver< tnlEllpackMatrix< double, tnlHost, int > >;
extern template class tnlGMRESSolver< tnlEllpackMatrix< float,  tnlHost, long int > >;
extern template class tnlGMRESSolver< tnlEllpackMatrix< double, tnlHost, long int > >;

extern template class tnlGMRESSolver< tnlMultiDiagonalMatrix< float,  tnlHost, int > >;
extern template class tnlGMRESSolver< tnlMultiDiagonalMatrix< double, tnlHost, int > >;
extern template class tnlGMRESSolver< tnlMultiDiagonalMatrix< float,  tnlHost, long int > >;
extern template class tnlGMRESSolver< tnlMultiDiagonalMatrix< double, tnlHost, long int > >;*/


#ifdef HAVE_CUDA
extern template class tnlGMRESSolver< tnlCSRMatrix< float,  tnlCuda, int > >;
extern template class tnlGMRESSolver< tnlCSRMatrix< double, tnlCuda, int > >;
extern template class tnlGMRESSolver< tnlCSRMatrix< float,  tnlCuda, long int > >;
extern template class tnlGMRESSolver< tnlCSRMatrix< double, tnlCuda, long int > >;

/*extern template class tnlGMRESSolver< tnlEllpackMatrix< float,  tnlCuda, int > >;
extern template class tnlGMRESSolver< tnlEllpackMatrix< double, tnlCuda, int > >;
extern template class tnlGMRESSolver< tnlEllpackMatrix< float,  tnlCuda, long int > >;
extern template class tnlGMRESSolver< tnlEllpackMatrix< double, tnlCuda, long int > >;

extern template class tnlGMRESSolver< tnlMutliDiagonalMatrix< float,  tnlCuda, int > >;
extern template class tnlGMRESSolver< tnlMutliDiagonalMatrix< double, tnlCuda, int > >;
extern template class tnlGMRESSolver< tnlMutliDiagonalMatrix< float,  tnlCuda, long int > >;
extern template class tnlGMRESSolver< tnlMutliDiagonalMatrix< double, tnlCuda, long int > >;*/
#endif

#endif
