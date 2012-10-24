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
#include <core/tnlVector.h>
#include <solvers/preconditioners/tnlDummyPreconditioner.h>
#include <solvers/tnlIterativeSolver.h>

template< typename Matrix,
           typename Preconditioner = tnlDummyPreconditioner< typename Matrix :: RealType,
                                                                typename Matrix :: Device,
                                                                typename Matrix :: IndexType> >
class tnlGMRESSolver : public tnlObject,
                        public tnlIterativeSolver< typename Matrix :: RealType,
                                                    typename Matrix :: IndexType >
{

   typedef typename Matrix :: RealType RealType;
   typedef typename Matrix :: IndexType IndexType;
   typedef typename Matrix :: Device Device;
   typedef Matrix MatrixType;
   typedef Preconditioner PreconditionerType;

   public:

   tnlGMRESSolver();

   tnlString getType() const;

   void setRestarting( IndexType rest );

   void setMatrix( const MatrixType& matrix );

   void setPreconditioner( const Preconditioner& preconditioner );

   template< typename Vector >
   bool solve( const Vector& b, Vector& x );

   ~tnlGMRESSolver();

   protected:

   /*!**
    * Here the parameter v is not constant because setSharedData is used 
    * on it inside of this method. It is not changed however.
    */
   void update( IndexType k,
                 IndexType m,
                 const tnlVector< RealType, tnlHost, IndexType >& H,
                 const tnlVector< RealType, tnlHost, IndexType >& s,
                 tnlVector< RealType, Device, IndexType >& v,
                 tnlVector< RealType, Device, IndexType >& x );

   void generatePlaneRotation( RealType &dx,
                                  RealType &dy,
                                  RealType &cs,
                                  RealType &sn );

   void applyPlaneRotation( RealType &dx,
                               RealType &dy,
                               RealType &cs,
                               RealType &sn );


   bool setSize( IndexType _size, IndexType m );

   tnlVector< RealType, Device, IndexType > _r, _w, _v, _M_tmp;
   tnlVector< RealType, tnlHost, IndexType > _s, _cs, _sn, _H;

   IndexType size, restarting, maxIterations;

   RealType maxResidue;

   const MatrixType* matrix;
   const PreconditionerType* preconditioner;
};

template< typename Matrix,
           typename Preconditioner >
tnlGMRESSolver< Matrix, Preconditioner > :: tnlGMRESSolver()
: tnlObject( "no-name" ),
  _r( "tnlGMRESSolver::_r" ),
  _w( "tnlGMRESSolver::_w" ),
  _v( "tnlGMRESSolver::_v" ),
  _M_tmp( "tnlGMRESSolver::_M_tmp" ),
  _s( "tnlGMRESSolver::_s" ),
  _cs( "tnlGMRESSolver::_cs" ),
  _sn( "tnlGMRESSolver::_sn" ),
  _H( "tnlGMRESSolver:_H" ),
  size( 0 ),
  restarting( 0 ),
  matrix( 0 ),
  preconditioner( 0 )
{
};
   
template< typename Matrix,
           typename Preconditioner >
tnlString tnlGMRESSolver< Matrix, Preconditioner > :: getType() const
{
   return tnlString( "tnlGMRESSolver< " ) +
           tnlString( GetParameterType( ( RealType ) 0.0 ) ) +
           tnlString( ", " ) +
           Device :: getDeviceType() +
           tnlString( ", " ) +
           tnlString( GetParameterType( ( IndexType ) 0 ) ) +
           tnlString( " >" );
}

template< typename Matrix,
           typename Preconditioner >
void tnlGMRESSolver< Matrix, Preconditioner > :: setRestarting( IndexType rest )
{
   if( size != 0 )
      setSize( size, rest );
   restarting = rest;
};

template< typename Matrix,
          typename Preconditioner >
void tnlGMRESSolver< Matrix, Preconditioner > :: setMatrix( const MatrixType& matrix )
{
   this -> matrix = &matrix;
}

template< typename Matrix,
           typename Preconditioner >
void tnlGMRESSolver< Matrix, Preconditioner > :: setPreconditioner( const Preconditioner& preconditioner )
{
   this -> preconditioner = &preconditioner;
}

template< typename Matrix,
          typename Preconditioner >
 template< typename Vector >
bool tnlGMRESSolver< Matrix, Preconditioner > :: solve( const Vector& b, Vector& x )
{
   tnlAssert( matrix, cerr << "No matrix was set in tnlGMRESSolver. Call setMatrix() before solve()." << endl );
   if( restarting <= 0 )
   {
      cerr << "I have wrong value for the restarting of the GMRES solver. It is set to " << restarting
           << ". Please set some positive value using the SetRestarting method." << endl;
      return false;
   }
   if( ! setSize( matrix -> getSize(), restarting ) ) return false;


   IndexType i, j = 1, k, l;
   
   IndexType _size = size;

   RealType *r = _r. getData();
   RealType *w = _w. getData();
   RealType *s = _s. getData();
   RealType *cs = _cs. getData();
   RealType *sn = _sn. getData();
   RealType *v = _v. getData();
   RealType *H = _H. getData();
   RealType *M_tmp = _M_tmp. getData();

   RealType normb( 0.0 ), beta( 0.0 );
   //T normb( 0.0 ), beta( 0.0 ); does not work with openmp yet
   /****
    * 1. Solve r from M r = b - A x_0
    */
   if( preconditioner )
   {
      //precond -> Solve( b, M_tmp );
      for( i = 0; i < _size; i ++ )
         normb += M_tmp[ i ] * M_tmp[ i ];

      matrix -> vectorProduct( x, _M_tmp );
      for( i = 0; i < size; i ++ )
         M_tmp[ i ] = b[ i ] - M_tmp[ i ];

      //precond -> Solve( M_tmp, r );
      for( i = 0; i < size; i ++ )
         beta += r[ i ] * r[ i ];
   }
   else
   {
      matrix -> vectorProduct( x, _r );
      normb = b. lpNorm( ( RealType ) 2.0 );
      _r. saxmy( ( RealType ) 1.0, b );
      beta = _r. lpNorm( ( RealType ) 2.0 );
   }

   if( normb == 0.0 ) normb = 1.0;

   this -> resetIterations();
   this -> setResidue( beta / normb );

   tnlVector< RealType, Device, IndexType > vi( "tnlGMRESSolver::vi" );
   tnlVector< RealType, Device, IndexType > vk( "tnlGMRESSolver::vk" );
   while( this -> getIterations() < this -> getMaxIterations() &&
          this -> getResidue() > this -> getMaxResidue() )
   {
      const IndexType m = restarting;
      for( i = 0; i < m + 1; i ++ )
         H[ i ] = s[ i ] = cs[ i ] = sn[ i ] = 0.0;

      /****
       * v = 0
       */   
      _v. setValue( ( RealType ) 0.0 );

      /***
       * v_0 = r / | r | =  1.0 / beta * r
       */
      vi. setSharedData( _v. getData(), size );
      vi. saxpy( ( RealType ) 1.0 / beta, _r );
                
      _s. setValue( ( RealType ) 0.0 );
      _s[ 0 ] = beta;
      


      //dbgCout( " ----------- Starting m-loop -----------------" );
      for( i = 0; i < m && this -> getIterations() <= this -> getMaxIterations(); i++ )
      {
         vi. setSharedData( &( _v. getData()[ i * size ] ), size );
         /****
          * Solve w from M w = A v_i
          */
         if( preconditioner )
         {
            matrix -> vectorProduct( vi, _M_tmp );            
            //preconditioner -> Solve( M_tmp, w );
         }
         else
             matrix -> vectorProduct( vi, _w );
         
         for( k = 0; k <= i; k++ )
         {
            vk. setSharedData( &( _v. getData()[ k * _size ] ), _size );
            /***
             * H_{k,i} = ( w, v_k )
             */
            RealType H_k_i = _w. sdot( vk );
            H[ k + i * ( m + 1 ) ] = H_k_i;
            
            /****
             * w = w - H_{k,i} v_k
             */
            _w. saxpy( -H_k_i, vk );
         }
         /***
          * H_{i+1,i} = |w|
          */
         RealType normw = _w. lpNorm( ( RealType ) 2.0 );
         H[ i + 1 + i * ( m + 1 ) ] = normw;

         /***
          * v_{i+1} = w / |w|
          */
         vi. setSharedData( &( _v. getData()[ ( i + 1 ) * size ] ), size );
         vi. saxpy( ( RealType ) 1.0 / normw, _w );


         //dbgCout( "Applying rotations" );
         for( k = 0; k < i; k++ )
            applyPlaneRotation( H[ k + i * ( m + 1 )],
                                H[ k + 1 + i * ( m + 1 ) ],
                                cs[ k ],
                                sn[ k ] );

         generatePlaneRotation( H[ i + i * ( m + 1 ) ],
                                H[ i + 1 + i * ( m + 1 ) ],
                                cs[ i ],
                                sn[ i ]);
         applyPlaneRotation( H[ i + i * ( m + 1 ) ],
                             H[ i + 1 + i * ( m + 1 ) ],
                             cs[ i ],
                             sn[ i ]);
         applyPlaneRotation( s[ i ],
                             s[ i + 1 ],
                             cs[ i ],
                             sn[ i ] );

         this -> setResidue( fabs( s[ i + 1 ] ) / normb );

         if( this -> getResidue() < this -> getMaxResidue() )
         {
            update( i, m, _H, _s, _v, x );
            //if( this -> verbosity > 0 )
            //   this -> printOut();
            return true;
         }
         //DBG_WAIT;
         this -> nextIteration();
      }
      update( m - 1, m, _H, _s, _v, x );
      //dbgCout_ARRAY( x, size );

      // r = M.solve(b - A * x);
      beta = 0.0;
      if( preconditioner )
      {
         matrix -> vectorProduct( x, _M_tmp );
         for( i = 0; i < _size; i ++ )
            M_tmp[ i ] = b[ i ] - M_tmp[ i ];
         //preconditioner -> solve( M_tmp, r );
         for( i = 0; i < _size; i ++ )
            beta += r[ i ] * r[ i ];
      }
      else
      {
         matrix -> vectorProduct( x, _r );
         _r. saxmy( ( RealType ) 1.0, b );
         beta = _r. lpNorm( ( RealType ) 2.0 );
      }
      this -> setResidue( beta / normb );
      this -> nextIteration();
   }
   this -> refreshSolverMonitor();
   if( this -> getIterations() == this -> getMaxIterations() ) return false;
   return true;
};

template< typename Matrix,
          typename Preconditioner >
tnlGMRESSolver< Matrix, Preconditioner > :: ~tnlGMRESSolver()
{
};

template< typename Matrix,
          typename Preconditioner >
void tnlGMRESSolver< Matrix, Preconditioner > :: update( IndexType k,
                                                         IndexType m,
                                                         const tnlVector< RealType, tnlHost, IndexType >& H,
                                                         const tnlVector< RealType, tnlHost, IndexType >& s,
                                                         tnlVector< RealType, Device, IndexType >& v,
                                                         tnlVector< RealType, Device, IndexType >& x )
{
   //dbgFunctionName( "tnlGMRESSolver", "Update" );
   tnlVector< RealType, tnlHost, IndexType > y( "tnlGMRESSolver::update:y" );
   y. setSize( m + 1 );

   IndexType i, j;
   for( i = 0; i <= m ; i ++ )
      y[ i ] = s[ i ];

   //dbgCout_ARRAY( y, m + 1 );
   // Backsolve:
   for( i = k; i >= 0; i--)
   {
      y[ i ] /= H[ i + i * ( m + 1 ) ];
      for( j = i - 1; j >= 0; j--)
         y[ j ] -= H[ j + i * ( m + 1 ) ] * y[ i ];
   }
   //dbgCout_ARRAY( y, m + 1 );

   tnlVector< RealType, Device, IndexType > vi( "tnlGMRESSolver::update:vi" );
   for( i = 0; i <= k; i++)
   {
      vi. setSharedData( &( v. getData()[ i * this -> size ] ), x. getSize() );
      x. saxpy( y[ i ], vi );
   }
};

template< typename Matrix,
          typename Preconditioner >
void tnlGMRESSolver< Matrix, Preconditioner > :: generatePlaneRotation( RealType &dx,
                                                                        RealType &dy,
                                                                        RealType &cs,
                                                                        RealType &sn )
{
   if( dy == 0.0 )
   {
      cs = 1.0;
      sn = 0.0;
   }
   else
      if( fabs( dy ) > fabs( dx ) )
      {
         RealType temp = dx / dy;
         sn = 1.0 / sqrt( 1.0 + temp * temp );
         cs = temp * sn;
      }
      else
      {
         RealType temp = dy / dx;
         cs = 1.0 / sqrt( 1.0 + temp * temp );
         sn = temp * cs;
      }
};

template< typename Matrix,
          typename Preconditioner >
void tnlGMRESSolver< Matrix, Preconditioner > :: applyPlaneRotation( RealType &dx,
                                                                     RealType &dy,
                                                                     RealType &cs,
                                                                     RealType &sn )
{
   RealType temp  =  cs * dx + sn * dy;
   dy =  cs * dy - sn * dx;
   dx = temp;
};

template< typename Matrix,
          typename Preconditioner >
bool tnlGMRESSolver< Matrix, Preconditioner > :: setSize( IndexType _size, IndexType m )
{
   if( size == _size && restarting == m ) return true;
   size = _size;
   restarting = m;
   if( ! _r. setSize( size ) ||
       ! _w. setSize( size ) ||
       ! _s. setSize( restarting + 1 ) ||
       ! _cs. setSize( restarting + 1 ) ||
       ! _sn. setSize( restarting + 1 ) ||
       ! _v. setSize( size * ( restarting + 1 ) ) ||
       ! _H. setSize( ( restarting + 1 ) * restarting ) ||
       ! _M_tmp. setSize( size ) )
   {
      cerr << "I could not allocated all supporting arrays for the CG solver." << endl;
      return false;
   }
   return true;
};

#endif