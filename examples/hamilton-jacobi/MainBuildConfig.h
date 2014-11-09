/***************************************************************************
                          MainBuildConfig.h  -  description
                             -------------------
    begin                : Jul 7, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
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

#ifndef MAINBUILDCONFIG_H_
#define MAINBUILDCONFIG_H_

#include <solvers/tnlConfigTags.h>

class MainBuildConfig
{
   public:

      static void print() { cerr << "MainBuildConfig" << endl; }
};

/****
 * Turn off support for float and long double.
 */
template<> struct tnlConfigTagReal< MainBuildConfig, float > { enum { enabled = false }; };
template<> struct tnlConfigTagReal< MainBuildConfig, long double > { enum { enabled = false }; };

/****
 * Turn off support for short int and long int indexing.
 */
template<> struct tnlConfigTagIndex< MainBuildConfig, short int >{ enum { enabled = false }; };
template<> struct tnlConfigTagIndex< MainBuildConfig, long int >{ enum { enabled = false }; };

/****
 * Use of tnlGrid is enabled for allowed dimensions and Real, Device and Index types.
 */
template< int Dimensions, typename Real, typename Device, typename Index >
   struct tnlConfigTagMesh< MainBuildConfig, tnlGrid< Dimensions, Real, Device, Index > >
      { enum { enabled = tnlConfigTagDimensions< MainBuildConfig, Dimensions >::enabled  &&
                         tnlConfigTagReal< MainBuildConfig, Real >::enabled &&
                         tnlConfigTagDevice< MainBuildConfig, Device >::enabled &&
                         tnlConfigTagIndex< MainBuildConfig, Index >::enabled }; };

/****
 * Please, chose your preferred time discretisation  here.
 */
template<> struct tnlConfigTagTimeDiscretisation< MainBuildConfig, tnlExplicitTimeDiscretisationTag >{ enum { enabled = true }; };
template<> struct tnlConfigTagTimeDiscretisation< MainBuildConfig, tnlSemiImplicitTimeDiscretisationTag >{ enum { enabled = false}; };
template<> struct tnlConfigTagTimeDiscretisation< MainBuildConfig, tnlImplicitTimeDiscretisationTag >{ enum { enabled = false }; };

/****
 * Only the Runge-Kutta-Merson solver is enabled by default.
 */
template<> struct tnlConfigTagExplicitSolver< MainBuildConfig, tnlExplicitEulerSolverTag >{ enum { enabled = false }; };

#endif /* MAINBUILDCONFIG_H_ */
