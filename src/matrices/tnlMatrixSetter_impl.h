/***************************************************************************
                          tnlMatrixSetter_impl.h  -  description
                             -------------------
    begin                : Oct 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
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

#ifndef TNLMATRIXSETTER_IMPL_H_
#define TNLMATRIXSETTER_IMPL_H_

#include <mesh/tnlTraverser.h>

template< typename Mesh,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename CompressedRowsLengthsVector >
   template< int EntityDimensions >
void
tnlMatrixSetter< Mesh, DifferentialOperator, BoundaryConditions, CompressedRowsLengthsVector >::
getCompressedRowsLengths( const Mesh& mesh,
               DifferentialOperator& differentialOperator,
               BoundaryConditions& boundaryConditions,
               CompressedRowsLengthsVector& rowLengths ) const
{
   if( DeviceType::DeviceType == tnlHostDevice )
   {
      TraversalUserData userData( differentialOperator, boundaryConditions, rowLengths );
      tnlTraverser< MeshType, EntityDimensions > meshTraversal;
      meshTraversal.template processBoundaryEntities< TraversalUserData,
                                                      TraversalBoundaryEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
      meshTraversal.template processInteriorEntities< TraversalUserData,
                                                      TraversalInteriorEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
   }
   if( DeviceType::DeviceType == tnlCudaDevice )
   {
      DifferentialOperator* kernelDifferentialOperator = tnlCuda::passToDevice( differentialOperator );
      BoundaryConditions* kernelBoundaryConditions = tnlCuda::passToDevice( boundaryConditions );
      CompressedRowsLengthsVector* kernelCompressedRowsLengths = tnlCuda::passToDevice( rowLengths );
      TraversalUserData userData( *kernelDifferentialOperator, *kernelBoundaryConditions, *kernelCompressedRowsLengths );
      checkCudaDevice;
      tnlTraverser< MeshType, EntityDimensions > meshTraversal;
      meshTraversal.template processBoundaryEntities< TraversalUserData,
                                                      TraversalBoundaryEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
      meshTraversal.template processInteriorEntities< TraversalUserData,
                                                      TraversalInteriorEntitiesProcessor >
                                                    ( mesh,
                                                      userData );

      checkCudaDevice;
      tnlCuda::freeFromDevice( kernelDifferentialOperator );
      tnlCuda::freeFromDevice( kernelBoundaryConditions );
      tnlCuda::freeFromDevice( kernelCompressedRowsLengths );
      checkCudaDevice;
   }
}

template< int Dimensions,
          typename Real,
          typename Device,
          typename Index,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename CompressedRowsLengthsVector >
   template< int EntityDimensions >
void
tnlMatrixSetter< tnlGrid< Dimensions, Real, Device, Index >, DifferentialOperator, BoundaryConditions, CompressedRowsLengthsVector >::
getCompressedRowsLengths( const MeshType& mesh,
               const DifferentialOperator& differentialOperator,
               const BoundaryConditions& boundaryConditions,
               CompressedRowsLengthsVector& rowLengths ) const
{
   if( DeviceType::DeviceType == ( int ) tnlHostDevice )
   {
      TraversalUserData userData( differentialOperator, boundaryConditions, rowLengths );
      tnlTraverser< MeshType, EntityDimensions > meshTraversal;
      meshTraversal.template processBoundaryEntities< TraversalUserData,
                                                      TraversalBoundaryEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
      meshTraversal.template processInteriorEntities< TraversalUserData,
                                                      TraversalInteriorEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
   }
   if( DeviceType::DeviceType == ( int ) tnlCudaDevice )
   {
      DifferentialOperator* kernelDifferentialOperator = tnlCuda::passToDevice( differentialOperator );
      BoundaryConditions* kernelBoundaryConditions = tnlCuda::passToDevice( boundaryConditions );
      CompressedRowsLengthsVector* kernelCompressedRowsLengths = tnlCuda::passToDevice( rowLengths );
      TraversalUserData userData( *kernelDifferentialOperator, *kernelBoundaryConditions, *kernelCompressedRowsLengths );
      checkCudaDevice;
      tnlTraverser< MeshType, EntityDimensions > meshTraversal;
      meshTraversal.template processBoundaryEntities< TraversalUserData,
                                                      TraversalBoundaryEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
      meshTraversal.template processInteriorEntities< TraversalUserData,
                                                      TraversalInteriorEntitiesProcessor >
                                                    ( mesh,
                                                      userData );

      checkCudaDevice;
      tnlCuda::freeFromDevice( kernelDifferentialOperator );
      tnlCuda::freeFromDevice( kernelBoundaryConditions );
      tnlCuda::freeFromDevice( kernelCompressedRowsLengths );
      checkCudaDevice;
   }
}


#endif /* TNLMATRIXSETTER_IMPL_H_ */
