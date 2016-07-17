/***************************************************************************
                          tnlIterativeSolverMonitor_impl.h  -  description
                             -------------------
    begin                : Oct 19, 2012
    copyright            : (C) 2012 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLITERATIVESOLVERMONITOR_IMPL_H_
#define TNLITERATIVESOLVERMONITOR_IMPL_H_

#include <iomanip>

using namespace std;

template< typename Real, typename Index>
tnlIterativeSolverMonitor< Real, Index > :: tnlIterativeSolverMonitor()
: iterations( 0 ),
  residue( 0 ),
  refreshing( 0 ),
  refreshRate( 1 ),
  verbose( 1 )
{
}

template< typename Real, typename Index>
void tnlIterativeSolverMonitor< Real, Index > :: setIterations( const Index& iterations )
{
   this->iterations = iterations;
}

template< typename Real, typename Index>
const Index& tnlIterativeSolverMonitor< Real, Index > :: getIterations() const
{
   return this->iterations;
}

template< typename Real, typename Index>
void tnlIterativeSolverMonitor< Real, Index > :: setResidue( const Real& residue )
{
   this->residue = residue;
}

template< typename Real, typename Index>
const Real& tnlIterativeSolverMonitor< Real, Index > :: getResidue() const
{
   return this->residue;
}

template< typename Real, typename Index>
void tnlIterativeSolverMonitor< Real, Index > :: setVerbose( const Index& verbose )
{
   this->verbose = verbose;
}

template< typename Real, typename Index>
void tnlIterativeSolverMonitor< Real, Index > :: setRefreshRate( const Index& refreshRate )
{
   this->refreshRate = refreshRate;
}

template< typename Real, typename Index>
void tnlIterativeSolverMonitor< Real, Index > :: refresh( bool force )
{
   if( this->verbose > 0 && ( force || this->getIterations() % this->refreshRate == 0 ) )
   {
      cout << " ITER:" << setw( 8 ) << this->getIterations()
           << " RES:" << setprecision( 5 ) << setw( 12 ) << this->getResidue()
           << " CPU: " << setw( 8 ) << this->getCPUTime()
           << " ELA: " << setw( 8 ) << this->getRealTime()
           << "   \r" << flush;
   }
   this->refreshing ++;
}

template< typename Real, typename Index>
void tnlIterativeSolverMonitor< Real, Index > :: resetTimers()
{
   cpuTimer.reset();
   rtTimer.reset();
}

template< typename Real, typename Index>
double tnlIterativeSolverMonitor< Real, Index > :: getCPUTime()
{
   return cpuTimer.getTime();
}

template< typename Real, typename Index>
double tnlIterativeSolverMonitor< Real, Index > :: getRealTime()
{
   return rtTimer.getTime();
}


#endif /* TNLITERATIVESOLVERMONITOR_IMPL_H_ */
