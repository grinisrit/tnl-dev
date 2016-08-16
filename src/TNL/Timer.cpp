/***************************************************************************
                          Timer.cpp  -  description
                             -------------------
    begin                : Mar 14, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <TNL/Timer.h>

#include <TNL/tnlConfig.h>
#ifdef HAVE_SYS_RESOURCE_H
   #include <sys/resource.h>
#endif
#ifdef HAVE_SYS_TIME_H
   #include <stddef.h>
   #include <sys/time.h>
   #define HAVE_TIME
#endif

namespace TNL {

Timer defaultTimer;

Timer::Timer()
{
   reset();
}

void Timer::reset()
{
   this->initialCPUTime = 0;
   this->totalCPUTime = 0.0;
   this->initialRealTime = 0;
   this->totalRealTime = 0.0;
   this->initialCPUCycles = 0;
   this->totalCPUCycles = 0;
   this->stopState = true;
}

void Timer::stop()
{

   if( ! this->stopState )
   {
      /****
       * Real time
       */
#ifdef HAVE_TIME
      struct timeval tp;
      int rtn = gettimeofday( &tp, NULL );
      this->totalRealTime += ( double ) tp. tv_sec + 1.0e-6 * ( double ) tp. tv_usec - this->initialRealTime;
#endif
 
      /****
       * CPU time
       */
#ifdef HAVE_SYS_RESOURCE_H
      rusage initUsage;
      getrusage(  RUSAGE_SELF, &initUsage );
      this->totalCPUTime += initUsage. ru_utime. tv_sec + 1.0e-6 * ( double ) initUsage. ru_utime. tv_usec - this->initialCPUTime;
#endif
 
      /****
       * CPU cycles
       */
      this->totalCPUCycles += this->rdtsc() - this->initialCPUCycles;
      this->stopState = true;
   }
}

void Timer::start()
{
   /****
    * Real time
    */
#ifdef HAVE_TIME
   struct timeval tp;
   int rtn = gettimeofday( &tp, NULL );
   this->initialRealTime = ( double ) tp. tv_sec + 1.0e-6 * ( double ) tp. tv_usec;
#endif

   /****
    * CPU Time
    */
#ifdef HAVE_SYS_RESOURCE_H
   rusage initUsage;
   getrusage( RUSAGE_SELF, &initUsage );
   this->initialCPUTime = initUsage. ru_utime. tv_sec + 1.0e-6 * ( double ) initUsage. ru_utime. tv_usec;
#endif
 
   /****
    * CPU cycles
    */
   this->initialCPUCycles = this->rdtsc();
 
   this->stopState = false;
}

double Timer::getRealTime()
{
#ifdef HAVE_TIME
   if( ! this->stopState )
   {
      this->stop();
      this->start();
   }
   return this->totalRealTime;
#else
   return -1;
#endif
}

double Timer::getCPUTime()
{
#ifdef HAVE_SYS_RESOURCE_H
   if( ! this->stopState )
   {
      this->stop();
      this->start();
   }
   return this->totalCPUTime;
#else
   return -1;
#endif
}

unsigned long long int Timer::getCPUCycles()
{
   if( ! this->stopState )
   {
      this->stop();
      this->start();
   }
   return this->totalCPUCycles;
}

bool Timer::writeLog( Logger& logger, int logLevel )
{
   logger.writeParameter< double                 >( "Real time:",  this->getRealTime(),  logLevel );
   logger.writeParameter< double                 >( "CPU time:",   this->getCPUTime(),   logLevel );
   logger.writeParameter< unsigned long long int >( "CPU Cycles:", this->getCPUCycles(), logLevel );

}

} // namespace TNL