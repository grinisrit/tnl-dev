/***************************************************************************
                          MeshEdgeTopology.h  -  description
                             -------------------
    begin                : Feb 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Meshes/Topologies/MeshEntityTopology.h>
#include <TNL/Meshes/Topologies/MeshVertexTopology.h>

namespace TNL {
namespace Meshes {
   
struct MeshEdgeTopology
{
   static const int dimensions = 1;
};


template<>
struct MeshSubtopology< MeshEdgeTopology, 0 >
{
   typedef MeshVertexTopology Topology;

   static const int count = 2;
};

} // namespace Meshes
} // namespace TNL
