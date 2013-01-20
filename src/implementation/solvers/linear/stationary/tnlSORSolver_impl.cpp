/***************************************************************************
                          tnlSORSolver_impl.cpp  -  description
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

#include <solvers/linear/stationary/tnlSORSolver.h>
#include <matrix/tnlCSRMatrix.h>
#include <matrix/tnlEllpackMatrix.h>
#include <matrix/tnlMultiDiagonalMatrix.h>

template class tnlSORSolver< tnlCSRMatrix< float,  tnlHost, int > >;
template class tnlSORSolver< tnlCSRMatrix< double, tnlHost, int > >;
template class tnlSORSolver< tnlCSRMatrix< float,  tnlHost, long int > >;
template class tnlSORSolver< tnlCSRMatrix< double, tnlHost, long int > >;

template class tnlSORSolver< tnlEllpackMatrix< float,  tnlHost, int > >;
template class tnlSORSolver< tnlEllpackMatrix< double, tnlHost, int > >;
template class tnlSORSolver< tnlEllpackMatrix< float,  tnlHost, long int > >;
template class tnlSORSolver< tnlEllpackMatrix< double, tnlHost, long int > >;

template class tnlSORSolver< tnlMultiDiagonalMatrix< float,  tnlHost, int > >;
template class tnlSORSolver< tnlMultiDiagonalMatrix< double, tnlHost, int > >;
template class tnlSORSolver< tnlMultiDiagonalMatrix< float,  tnlHost, long int > >;
template class tnlSORSolver< tnlMultiDiagonalMatrix< double, tnlHost, long int > >;


#ifdef HAVE_CUDA
template class tnlSORSolver< tnlCSRMatrix< float,  tnlCuda, int > >;
template class tnlSORSolver< tnlCSRMatrix< double, tnlCuda, int > >;
template class tnlSORSolver< tnlCSRMatrix< float,  tnlCuda, long int > >;
template class tnlSORSolver< tnlCSRMatrix< double, tnlCuda, long int > >;

template class tnlSORSolver< tnlEllpackMatrix< float,  tnlCuda, int > >;
template class tnlSORSolver< tnlEllpackMatrix< double, tnlCuda, int > >;
template class tnlSORSolver< tnlEllpackMatrix< float,  tnlCuda, long int > >;
template class tnlSORSolver< tnlEllpackMatrix< double, tnlCuda, long int > >;

template class tnlSORSolver< tnlMultiDiagonalMatrix< float,  tnlCuda, int > >;
template class tnlSORSolver< tnlMultiDiagonalMatrix< double, tnlCuda, int > >;
template class tnlSORSolver< tnlMultiDiagonalMatrix< float,  tnlCuda, long int > >;
template class tnlSORSolver< tnlMultiDiagonalMatrix< double, tnlCuda, long int > >;
#endif




