/***************************************************************************
                          main.h  -  description
                             -------------------
    begin                : Mar 30 , 2015
    copyright            : (C) 2015 by Tomas Sobotik
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "tnlParallelEikonalSolver.h"
#include "parallelEikonalConfig.h"
#include "MainBuildConfig.h"
#include <solvers/tnlBuildConfigTags.h>
#include <operators/hamilton-jacobi/godunov-eikonal/parallelGodunovEikonal.h>
#include <mesh/tnlGrid.h>
#include <core/tnlDevice.h>
#include <time.h>
#include <ctime>

typedef MainBuildConfig BuildConfig;

int main( int argc, char* argv[] )
{
	time_t start;
	time_t stop;
	time(&start);
	std::clock_t start2= std::clock();
   Config::ParameterContainer parameters;
   tnlConfigDescription configDescription;
   parallelEikonalConfig< BuildConfig >::configSetup( configDescription );

   if( ! parseCommandLine( argc, argv, configDescription, parameters ) )
      return false;

   //if (parameters.GetParameter <String>("scheme") == "godunov")
   //{
   tnlDeviceEnum device;
   device = TNL::Devices::HostDevice;

   const int& dim = parameters.getParameter< int >( "dim" );

  if(dim == 2)
  {

	   typedef parallelGodunovEikonalScheme< tnlGrid<2,double,TNL::Devices::Host, int>, double, int > SchemeTypeHost;
		/*#ifdef HAVE_CUDA
		   typedef parallelGodunovEikonalScheme< tnlGrid<2,double,tnlCuda, int>, double, int > SchemeTypeDevice;
		#endif
		#ifndef HAVE_CUDA*/
	   typedef parallelGodunovEikonalScheme< tnlGrid<2,double,TNL::Devices::Host, int>, double, int > SchemeTypeDevice;
		/*#endif*/

	   if(device==TNL::Devices::HostDevice)
	   {
		   typedef TNL::Devices::Host Device;


		   tnlParallelEikonalSolver<2,SchemeTypeHost,SchemeTypeDevice, Device> solver;
		   if(!solver.init(parameters))
		   {
			   cerr << "Solver failed to initialize." << endl;
			   return EXIT_FAILURE;
		   }
		   cout << "-------------------------------------------------------------" << endl;
		   cout << "Starting solver loop..." << endl;
		   solver.run();
	   }
	   else if(device==tnlCudaDevice )
	   {
		   typedef tnlCuda Device;
		   //typedef parallelGodunovEikonalScheme< tnlGrid<2,double,Device, int>, double, int > SchemeType;

		   tnlParallelEikonalSolver<2,SchemeTypeHost,SchemeTypeDevice, Device> solver;
		   if(!solver.init(parameters))
		   {
			   cerr << "Solver failed to initialize." << endl;
			   return EXIT_FAILURE;
		   }
		   cout << "-------------------------------------------------------------" << endl;
		   cout << "Starting solver loop..." << endl;
		   solver.run();
	   }
  // }
  }
  else if(dim == 3)
  {

	   typedef parallelGodunovEikonalScheme< tnlGrid<3,double,TNL::Devices::Host, int>, double, int > SchemeTypeHost;
		/*#ifdef HAVE_CUDA
		   typedef parallelGodunovEikonalScheme< tnlGrid<2,double,tnlCuda, int>, double, int > SchemeTypeDevice;
		#endif
		#ifndef HAVE_CUDA*/
	   typedef parallelGodunovEikonalScheme< tnlGrid<3,double,TNL::Devices::Host, int>, double, int > SchemeTypeDevice;
		/*#endif*/

	   if(device==TNL::Devices::HostDevice)
	   {
		   typedef TNL::Devices::Host Device;


		   tnlParallelEikonalSolver<3,SchemeTypeHost,SchemeTypeDevice, Device> solver;
		   if(!solver.init(parameters))
		   {
			   cerr << "Solver failed to initialize." << endl;
			   return EXIT_FAILURE;
		   }
		   cout << "-------------------------------------------------------------" << endl;
		   cout << "Starting solver loop..." << endl;
		   solver.run();
	   }
	   else if(device==tnlCudaDevice )
	   {
		   typedef tnlCuda Device;
		   //typedef parallelGodunovEikonalScheme< tnlGrid<2,double,Device, int>, double, int > SchemeType;

		   tnlParallelEikonalSolver<3,SchemeTypeHost,SchemeTypeDevice, Device> solver;
		   if(!solver.init(parameters))
		   {
			   cerr << "Solver failed to initialize." << endl;
			   return EXIT_FAILURE;
		   }
		   cout << "-------------------------------------------------------------" << endl;
		   cout << "Starting solver loop..." << endl;
		   solver.run();
	   }
 // }
  }

   time(&stop);
   cout << endl;
   cout << "Running time was: " << difftime(stop,start) << " .... " << (std::clock() - start2) / (double)(CLOCKS_PER_SEC) << endl;
   return EXIT_SUCCESS;
}

