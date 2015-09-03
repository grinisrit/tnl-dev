/***************************************************************************
                           tnlConstants.h -  description
                             -------------------
    begin                : June 17, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
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

#ifndef TNLCONSTANTS_H
#define	TNLCONSTANTS_H

#include <limits.h>
#include <float.h>
#include <cstdio>
#include <core/tnlAssert.h>
#include <core/tnlCuda.h>

template< typename T > __cuda_callable__ T tnlMinValue(){ tnlAssert( false,)};
template<> inline __cuda_callable__ char               tnlMinValue< char >() { return CHAR_MIN; }
template<> inline __cuda_callable__ unsigned char      tnlMinValue< unsigned char >() { return 0; }
template<> inline __cuda_callable__ short int          tnlMinValue< short int >() { return SHRT_MIN; }
template<> inline __cuda_callable__ unsigned short int tnlMinValue< unsigned short int >() { return 0; }
template<> inline __cuda_callable__ int                tnlMinValue< int >() { return INT_MIN; }
template<> inline __cuda_callable__ unsigned int       tnlMinValue< unsigned int >() { return 0; }
template<> inline __cuda_callable__ long int           tnlMinValue< long int >() { return LONG_MIN; }
template<> inline __cuda_callable__ unsigned long int  tnlMinValue< unsigned long int >() { return 0; }
template<> inline __cuda_callable__ float              tnlMinValue< float >() { return -FLT_MAX; }
template<> inline __cuda_callable__ double             tnlMinValue< double >() { return -DBL_MAX; }
template<> inline __cuda_callable__ long double        tnlMinValue< long double >() { return -LDBL_MAX; }

template< typename T > __cuda_callable__ T tnlMaxValue(){ tnlAssert( false,)};
template<> inline __cuda_callable__ char               tnlMaxValue< char >() { return CHAR_MAX; }
template<> inline __cuda_callable__ unsigned char      tnlMaxValue< unsigned char >() { return UCHAR_MAX; }
template<> inline __cuda_callable__ short int          tnlMaxValue< short int >() { return SHRT_MAX; }
template<> inline __cuda_callable__ unsigned short int tnlMaxValue< unsigned short int >() { return USHRT_MAX; }
template<> inline __cuda_callable__ int                tnlMaxValue< int >() { return INT_MAX; }
template<> inline __cuda_callable__ unsigned int       tnlMaxValue< unsigned int >() { return UINT_MAX; }
template<> inline __cuda_callable__ long int           tnlMaxValue< long int >() { return LONG_MAX; }
template<> inline __cuda_callable__ unsigned long int  tnlMaxValue< unsigned long int >() { return ULONG_MAX; }
template<> inline __cuda_callable__ float              tnlMaxValue< float >() { return FLT_MAX; }
template<> inline __cuda_callable__ double             tnlMaxValue< double >() { return DBL_MAX; }
template<> inline __cuda_callable__ long double        tnlMaxValue< long double >() { return LDBL_MAX; }




#endif	/* TNLCONSTANTS_H */

