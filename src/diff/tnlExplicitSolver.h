/***************************************************************************
                          tnlExplicitSolver.h  -  description
                             -------------------
    begin                : 2007/06/17
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

#ifndef tnlExplicitSolverH
#define tnlExplicitSolverH

#include <iomanip>
#include <core/tnlTimerCPU.h>
#include <core/tnlTimerRT.h>

template< class GRID, class SCHEME, typename T = double > class tnlExplicitSolver : public tnlObject
{
   public:
   
   tnlExplicitSolver()
   :  iteration( 0 ), 
      time( 0.0 ),
      tau( 0.0 ),
      residue( 0.0 ),
      solver_comm( MPI_COMM_WORLD ),
      verbosity( 0 ),
      cpu_timer( &default_mcore_cpu_timer ),
      rt_timer( &default_mcore_rt_timer )
      {
      };


   void SetTime( const T& t )
   {
      time = t;
   };

   const T& GetTime() const
   {
      return time;
   };

   //void SetFinalTime( const double& t );

   int GetIterationNumber() const
   {
      return iteration;
   };
   
   void SetTau( const T& t )
   {
      tau = t;
   };
   
   const T& GetTau() const
   {
      return tau;
   };

   const T& GetResidue() const
   {
      return residue;
   };

   void SetMPIComm( MPI_Comm comm )
   {
      solver_comm = comm;
   };
  
   void SetVerbosity( int v )
   {
      verbosity = v;
   };

   void SetTimerCPU( const tnlTimerCPU* timer )
   {
      cpu_timer = timer;
   };

   void SetTimerRT( const tnlTimerRT* timer )
   {
      rt_timer = timer;
   };
  
   void PrintOut()
   {
      if( verbosity > 0 )
      {
         int cpu_time = 0;
         if( cpu_timer ) cpu_time = cpu_timer -> GetTime( 0, solver_comm );
         if( MPIGetRank() != 0 ) return;
         // TODO: add EST
         //cout << " EST: " << estimated;
         cout << " ITER:" << setw( 8 ) << GetIterationNumber()
              << " TAU:" << setprecision( 5 ) << setw( 12 ) << GetTau()
              << " T:" << setprecision( 5 ) << setw( 12 ) << GetTime()
              << " RES:" << setprecision( 5 ) << setw( 12 ) << GetResidue();
         if( cpu_timer )
            cout << " CPU: " << setw( 8 ) << cpu_time;
         if( rt_timer )
            cout << " ELA: " << setw( 8 ) << rt_timer -> GetTime();
         cout << "   \r" << flush;
      }
   }

   virtual int GetRungeKuttaIndex() const { return 0; };
   
   virtual bool Solve( SCHEME& scheme,
                       GRID& u,
                       const T& stop_time,
                       const T& max_res,
                       const int max_iter ) = 0;
     
   protected:
    
   int iteration;

   T time;

   T tau;

   T residue;

   MPI_Comm solver_comm;

   int verbosity;

   const tnlTimerCPU* cpu_timer;
   
   const tnlTimerRT* rt_timer;
};

#endif
