/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************
                          tnlDeviceObjectsContainer.h  -  description
                             -------------------
    begin                : Apr 29, 2016
    copyright            : (C) 2016 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

#pragma once

#include <vector>
#include <list>
#include <core/tnlDeviceObjectBase.h>
#include <core/tnlAssert.h>

class tnlDeviceObjectsContainer
{   
  
   public:
   
      tnlDeviceObjectsContainer( int devicesCount = 1 );
      
      void push( tnlDeviceObjectBase* object, int deviceId );
      
      bool synchronize();
      
   protected:
      
      typedef std::list< tnlDeviceObjectBase* > ListType;   
      
      std::vector< ListType > objectsOnDevices;
      
      int devicesCount;
};

