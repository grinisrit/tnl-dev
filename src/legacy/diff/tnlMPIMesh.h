/***************************************************************************
                          tnlMPIMesh.h  -  description
                             -------------------
    begin                : Dec 15, 2010
    copyright            : (C) 2010 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#ifndef TNLMPIMESH_H_
#define TNLMPIMESH_H_

#include <legacy/mesh/tnlGridOld.h>

template< int Dimensions, typename Real = double, typename Device = tnlHost, typename Index = int >
class tnlMPIMesh
{
};

#include <legacy/diff/tnlMPIMesh2D.h>
#include <legacy/diff/tnlMPIMesh3D.h>

#endif /* TNLMPIMESH_H_ */
