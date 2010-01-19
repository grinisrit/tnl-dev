/***************************************************************************
                          tnl-cuda-kernels.cu
                             -------------------
    begin                : Jan 14, 2010
    copyright            : (C) 2009 by Tomas Oberhuber
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

#include <iostream>
#include <core/mfuncs.h>
#include <core/tnl-cuda-kernels.h>

using namespace std;

int tnlCUDAReductionMin( const int size,
                         const int* input,
                         int& result,
                         int* device_aux_array = 0 )
{
   return tnlCUDAReduction< int, tnlMin >( size, input, result, device_aux_array );
}

int tnlCUDAReductionMax( const int size,
                         const int* input,
                         int& result,
                         int* device_aux_array = 0 )
{
   return tnlCUDAReduction< int, tnlMax >( size, input, result, device_aux_array );
}

int tnlCUDAReductionSum( const int size,
                         const int* input,
                         int& result,
                         int* device_aux_array = 0 )
{
   return tnlCUDAReduction< int, tnlSum >( size, input, result, device_aux_array );
}

bool tnlCUDAReductionMin( const int size,
                          const float* input,
                          float& result,
                          float* device_aux_array = 0 )
{
   return tnlCUDAReduction< float, tnlMin >( size, input, result, device_aux_array );
}

bool tnlCUDAReductionMax( const int size,
                          const float* input,
                          float& result,
                          float* device_aux_array = 0 )
{
   return tnlCUDAReduction< float, tnlMax >( size, input, result, device_aux_array );
}

bool tnlCUDAReductionSum( const int size,
                          const float* input,
                          float& result,
                          float* device_aux_array = 0 )
{
   return tnlCUDAReduction< float, tnlSum >( size, input, result, device_aux_array );
}
bool tnlCUDAReductionMin( const int size,
                          const double* input,
                          double& result,
                          double* device_aux_array = 0 )
{
   return tnlCUDAReduction< double, tnlMin >( size, input, result, device_aux_array );
}

bool tnlCUDAReductionMax( const int size,
                          const double* input,
                          double& result,
                          double* device_aux_array = 0 )
{
   return tnlCUDAReduction< double, tnlMax >( size, input, result, device_aux_array );
}

bool tnlCUDAReductionSum( const int size,
                          const double* input,
                          double& result,
                          double* device_aux_array = 0 )
{
   return tnlCUDAReduction< double, tnlSum >( size, input, result, device_aux_array );
}

/*
 * Simple redcution 5
 */

bool tnlCUDASimpleReduction5Min( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction5< int, tnlMin >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction5Max( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction5< int, tnlMax >( size,
                                                  device_input,
                                                  result );
}
bool tnlCUDASimpleReduction5Sum( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction5< int, tnlSum >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction5Min( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction5< float, tnlMin >( size,
                                                    device_input,
                                                    result );
}

bool tnlCUDASimpleReduction5Max( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction5< float, tnlMax >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction5Sum( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction5< float, tnlSum >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction5Min( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction5< double, tnlMin >( size,
                                                     device_input,
                                                     result );
}

bool tnlCUDASimpleReduction5Max( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction5< double, tnlMax >( size,
                                                     device_input,
                                                     result );
}
bool tnlCUDASimpleReduction5Sum( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction5< double, tnlSum >( size,
                                                     device_input,
                                                     result );
}


/*
 * Simple redcution 4
 */

bool tnlCUDASimpleReduction4Min( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction4< int, tnlMin >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction4Max( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction4< int, tnlMax >( size,
                                                  device_input,
                                                  result );
}
bool tnlCUDASimpleReduction4Sum( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction4< int, tnlSum >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction4Min( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction4< float, tnlMin >( size,
                                                    device_input,
                                                    result );
}

bool tnlCUDASimpleReduction4Max( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction4< float, tnlMax >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction4Sum( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction4< float, tnlSum >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction4Min( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction4< double, tnlMin >( size,
                                                     device_input,
                                                     result );
}

bool tnlCUDASimpleReduction4Max( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction4< double, tnlMax >( size,
                                                     device_input,
                                                     result );
}
bool tnlCUDASimpleReduction4Sum( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction4< double, tnlSum >( size,
                                                     device_input,
                                                     result );
}

/*
 * Simple redcution 3
 */

bool tnlCUDASimpleReduction3Min( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction3< int, tnlMin >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction3Max( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction3< int, tnlMax >( size,
                                                  device_input,
                                                  result );
}
bool tnlCUDASimpleReduction3Sum( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction3< int, tnlSum >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction3Min( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction3< float, tnlMin >( size,
                                                    device_input,
                                                    result );
}

bool tnlCUDASimpleReduction3Max( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction3< float, tnlMax >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction3Sum( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction3< float, tnlSum >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction3Min( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction3< double, tnlMin >( size,
                                                     device_input,
                                                     result );
}

bool tnlCUDASimpleReduction3Max( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction3< double, tnlMax >( size,
                                                     device_input,
                                                     result );
}
bool tnlCUDASimpleReduction3Sum( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction3< double, tnlSum >( size,
                                                     device_input,
                                                     result );
}

/*
 * Simple redcution 2
 */

bool tnlCUDASimpleReduction2Min( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction2< int, tnlMin >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction2Max( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction2< int, tnlMax >( size,
                                                  device_input,
                                                  result );
}
bool tnlCUDASimpleReduction2Sum( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction2< int, tnlSum >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction2Min( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction2< float, tnlMin >( size,
                                                    device_input,
                                                    result );
}

bool tnlCUDASimpleReduction2Max( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction2< float, tnlMax >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction2Sum( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction2< float, tnlSum >( size,
                                                    device_input,
                                                    result );
}
bool tnlCUDASimpleReduction2Min( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction2< double, tnlMin >( size,
                                                     device_input,
                                                     result );
}

bool tnlCUDASimpleReduction2Max( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction2< double, tnlMax >( size,
                                                     device_input,
                                                     result );
}
bool tnlCUDASimpleReduction2Sum( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction2< double, tnlSum >( size,
                                                     device_input,
                                                     result );
}


/*
 * Simple redcution 1
 */

bool tnlCUDASimpleReduction1Min( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction1< int, tnlMin >( size,
                                                  device_input,
                                                  result );
}

bool tnlCUDASimpleReduction1Max( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction1< int, tnlMax >( size,
                                   device_input,
                                   result );
}
bool tnlCUDASimpleReduction1Sum( const int size,
                                 const int* device_input,
                                 int& result )
{
   return tnlCUDASimpleReduction1< int, tnlSum >( size,
                                   device_input,
                                   result );
}

bool tnlCUDASimpleReduction1Min( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction1< float, tnlMin >( size,
                                   device_input,
                                   result );
}

bool tnlCUDASimpleReduction1Max( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction1< float, tnlMax >( size,
                                   device_input,
                                   result );
}
bool tnlCUDASimpleReduction1Sum( const int size,
                                 const float* device_input,
                                 float& result )
{
   return tnlCUDASimpleReduction1< float, tnlSum >( size,
                                   device_input,
                                   result );
}
bool tnlCUDASimpleReduction1Min( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction1< double, tnlMin >( size,
                                   device_input,
                                   result );
}

bool tnlCUDASimpleReduction1Max( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction1< double, tnlMax >( size,
                                   device_input,
                                   result );
}
bool tnlCUDASimpleReduction1Sum( const int size,
                                 const double* device_input,
                                 double& result )
{
   return tnlCUDASimpleReduction1< double, tnlSum >( size,
                                   device_input,
                                   result );
}
