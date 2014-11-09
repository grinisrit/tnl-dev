/***************************************************************************
                          hamiltonJacobiProblemConfig.h  -  description
                             -------------------
    begin                : Oct 5, 2014
    copyright            : (C) 2014 by Tomas Sobotik
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef HAMILTONJACOBIPROBLEMCONFIG_H_
#define HAMILTONJACOBIPROBLEMCONFIG_H_

#include <config/tnlConfigDescription.h>

template< typename ConfigTag >
class hamiltonJacobiProblemConfig
{
   public:
      static void configSetup( tnlConfigDescription& config )
      {
         config.addDelimiter( "Hamilton-Jacobi solver settings:" );
         config.addEntry        < tnlString > ( "problem-name", "This defines particular problem.", "hamilton-jacobi" );
         config.addEntry       < tnlString > ( "scheme", "This defines scheme used for discretization.", "godunov" );
         config.addEntryEnum( "godunov" );
         config.addEntryEnum( "upwind" );
         config.addEntry        < double > ( "epsilon", "This defines epsilon for smoothening oh sign().", 0.0 );
         config.addEntry        < double > ( "-value", "Constant value of RHS.", 0.0 );
      }
};

#endif /* HAMILTONJACOBIPROBLEMCONFIG_H_ */
