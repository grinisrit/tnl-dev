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
   template< typename EntityType >
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
      tnlTraverser< MeshType, EntityType > meshTraversal;
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
      tnlTraverser< MeshType, EntityType > meshTraversal;
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
   template< typename EntityType >
void
tnlMatrixSetter< tnlGrid< Dimensions, Real, Device, Index >, DifferentialOperator, BoundaryConditions, CompressedRowsLengthsVector >::
getCompressedRowsLengths( const MeshType& mesh,
               const DifferentialOperator& differentialOperator,
               const BoundaryConditions& boundaryConditions,
               CompressedRowsLengthsVector& rowLengths ) const
{
    
   //TODO issame:value 
   if( DeviceType::DeviceType == ( int ) tnlHostDevice )
   {
      /*TraversalUserData userData( differentialOperator, boundaryConditions, rowLengths );
      tnlTraverser< MeshType, EntityType > meshTraversal;
      meshTraversal.template processBoundaryEntities< TraversalUserData,
                                                      TraversalBoundaryEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
      meshTraversal.template processInteriorEntities< TraversalUserData,
                                                      TraversalInteriorEntitiesProcessor >
                                                    ( mesh,
                                                      userData );*/
       cout << "NOT impleneted YET - getCompressedRowsLength" <<endl;
   } 
    
    
   if( DeviceType::DeviceType == ( int ) tnlHostDevice )
   {
      TraversalUserData userData( differentialOperator, boundaryConditions, rowLengths );
      tnlTraverser< MeshType, EntityType > meshTraversal;
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
      tnlTraverser< MeshType, EntityType > meshTraversal;
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
   
   if( DeviceType::DeviceType == ( int ) tnlMICDevice )
   {
     /* const DifferentialOperator* kernelDifferentialOperator = tnlMIC::passToDevice( differentialOperator );
      const BoundaryConditions* kernelBoundaryConditions = tnlMIC::passToDevice( boundaryConditions );
      CompressedRowsLengthsVector* kernelCompressedRowsLengths = tnlMIC::passToDevice( rowLengths );
      TraversalUserData userData( *kernelDifferentialOperator, *kernelBoundaryConditions, *kernelCompressedRowsLengths );
      
      tnlTraverser< MeshType, EntityType > meshTraversal;
      meshTraversal.template processBoundaryEntities< TraversalUserData,
                                                      TraversalBoundaryEntitiesProcessor >
                                                    ( mesh,
                                                      userData );
      meshTraversal.template processInteriorEntities< TraversalUserData,
                                                      TraversalInteriorEntitiesProcessor >
                                                    ( mesh,
                                                      userData );

      tnlMIC::freeFromDevice( kernelDifferentialOperator );
      tnlMIC::freeFromDevice( kernelBoundaryConditions );
      tnlMIC::freeFromDevice( kernelCompressedRowsLengths );*/
       
       cout << "Not Implemented yet" << __FILE__ << __LINE__ <<endl;
      
   }
   
   
}


#endif /* TNLMATRIXSETTER_IMPL_H_ */
