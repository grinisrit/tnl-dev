/***************************************************************************
                          tnlVector_impl.cpp  -  description
                             -------------------
    begin                : Jan 20, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
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

#include <core/vectors/tnlVector.h>

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

template class tnlVector< float, tnlHost, int >;
template tnlVector< float, tnlHost, int >& tnlVector< float, tnlHost, int >:: operator = ( const tnlVector< double, tnlHost, int >& vector );

template class tnlVector< double, tnlHost, int >;
#ifdef INSTANTIATE_LONG_DOUBLE
template class tnlVector< long double, tnlHost, int >;
#endif
#ifdef INSTANTIATE_LONG_INT
template class tnlVector< float, tnlHost, long int >;
template class tnlVector< double, tnlHost, long int >;
#ifdef INSTANTIATE_LONG_DOUBLE
template class tnlVector< long double, tnlHost, long int >;
#endif
#endif

#endif


