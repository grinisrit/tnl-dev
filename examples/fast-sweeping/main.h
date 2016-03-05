/***************************************************************************
                          main.h  -  description
                             -------------------
    begin                : Oct 15 , 2015
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


#include "MainBuildConfig.h"
	//for HOST versions:
#include "tnlFastSweeping.h"
	//for DEVICE versions:
//#include "tnlFastSweeping_CUDA.h"
#include "fastSweepingConfig.h"
#include <solvers/tnlBuildConfigTags.h>

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
   tnlParameterContainer parameters;
   tnlConfigDescription configDescription;
   fastSweepingConfig< BuildConfig >::configSetup( configDescription );

   if( ! parseCommandLine( argc, argv, configDescription, parameters ) )
      return false;

    tnlFastSweeping<tnlGrid<3,double,tnlHost, int>, double, int> solver;
    if(!solver.init(parameters))
   {
    	cerr << "Solver failed to initialize." << endl;
   		return EXIT_FAILURE;
   }
    checkCudaDevice;
   cout << "-------------------------------------------------------------" << endl;
   cout << "Starting solver..." << endl;
   solver.run();


   time(&stop);
   cout << "Solver stopped..." << endl;
   cout << endl;
   cout << "Running time was: " << difftime(stop,start) << " .... " << (std::clock() - start2) / (double)(CLOCKS_PER_SEC) << endl;
   return EXIT_SUCCESS;
}


