/***************************************************************************
                          tnlCudaDeviceInfo.h  -  description
                             -------------------
    begin                : Jun 21, 2015
    copyright            : (C) 2007 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLCUDADEVICEINFO_H
#define	TNLCUDADEVICEINFO_H

#include <stdlib.h>
#include <core/tnlCuda.h>

class tnlCudaDeviceInfo
{
   public:

      static int getNumberOfDevices();

      static int getActiveDevice();

      static tnlString getDeviceName( int deviceNum );

      static int getArchitectureMajor( int deviceNum );

      static int getArchitectureMinor( int deviceNum );

      static int getClockRate( int deviceNum );

      static size_t getGlobalMemory( int deviceNum );

      static int getMemoryClockRate( int deviceNum );

      static bool getECCEnabled( int deviceNum );

      static int getCudaMultiprocessors( int deviceNum );

      static int getCudaCoresPerMultiprocessors( int deviceNum );

      static int getCudaCores( int deviceNum );

};




#endif	/* TNLCUDADEVICEINFO_H */

