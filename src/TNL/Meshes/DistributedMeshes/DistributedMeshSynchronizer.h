/***************************************************************************
                          DistributedMeshSynchronizer.h  -  description
                             -------------------
    begin                : January 8, 2017
    copyright            : (C) 2017 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

namespace TNL {
namespace Meshes { 
namespace DistributedMeshes {

template <typename MeshFunctionType>  
class DistributedMeshSynchronizer
{
    public:
        typedef typename MeshFunctionType::DistributedMeshType DistributedMeshType;
        void SetDistributedGrid(DistributedMeshType *distrgrid)
        {
            TNL_ASSERT_TRUE(false,"Distribution of this type of mesh is NOT implemented");
        }; 

};


} // namespace DistributedMeshes
} // namespace Meshes
} // namespace TNL

#include <TNL/Meshes/DistributedMeshes/DistributedGridSynchronizer.h>