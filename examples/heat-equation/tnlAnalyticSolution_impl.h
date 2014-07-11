#ifndef TNLANALYTICSOLUTION_IMPL_H
#define	TNLANALYTICSOLUTION_IMPL_H

#include "tnlAnalyticSolution.h"

template<typename Real, typename Device, typename Index>
template< typename TimeFunction, typename AnalyticSpaceFunction>
void AnalyticSolution<tnlGrid<1,Real,Device,Index,tnlIdenticalGridGeometry>>::
computeAnalyticSolution(const MeshType& mesh, const RealType& time, SharedVector& output, 
        const TimeFunction timeFunction, const AnalyticSpaceFunction analyticSpaceFunction)
{  
      RealType timeFunctionValue = timeFunction.getTimeValue(time);
      
      VertexType vertex;
      CoordinatesType coordinates;

      for(IndexType i=0; i<output.getSize(); i++)
      {
         mesh.getElementCoordinates(i,coordinates);
         mesh.getElementCenter(coordinates,vertex);
         
         output[i] = timeFunctionValue*analyticSpaceFunction.getF(vertex);
      }
}

template<typename Real, typename Device, typename Index>
template< typename TimeFunction, typename AnalyticSpaceFunction>
void AnalyticSolution<tnlGrid<1,Real,Device,Index,tnlIdenticalGridGeometry>>::
computeLaplace(const MeshType& mesh, const RealType& time, DofVectorType& output,
                const TimeFunction timeFunction, const AnalyticSpaceFunction analyticSpaceFunction)
{        
      RealType timeFunctionValue = timeFunction.getTimeValue(time);   
   
      VertexType vertex;
      CoordinatesType coordinates;

      #ifdef HAVE_OPENMP
         #pragma omp parallel for private(coordinates,vertex)
      #endif
      for(IndexType i=1; i<(output.getSize()-1); i++)
      {         
         mesh.getElementCoordinates(i,coordinates);
         mesh.getElementCenter(coordinates,vertex);

      output[i] = timeFunctionValue*analyticSpaceFunction.template getF<2,0,0>(vertex);
   }  
}

template<typename Real, typename Device, typename Index>
template< typename TimeFunction, typename AnalyticSpaceFunction>
void AnalyticSolution<tnlGrid<2,Real,Device,Index,tnlIdenticalGridGeometry>>::
computeAnalyticSolution(const MeshType& mesh, const RealType& time, SharedVector& output, 
        const TimeFunction timeFunction, const AnalyticSpaceFunction analyticSpaceFunction)
{ 
   RealType timeFunctionValue = timeFunction.getTimeValue(time);
   
   VertexType vertex;
   CoordinatesType coordinates;
   
   #ifdef HAVE_OPENMP
    #pragma omp parallel for private(coordinates,vertex)
   #endif
   for(IndexType i=0; i<output.getSize(); i++)
   {      
      mesh.getElementCoordinates(i,coordinates);
      mesh.getElementCenter(coordinates,vertex);
      
      output[i] = timeFunctionValue*analyticSpaceFunction.getF(vertex);
   }   
   
}

template<typename Real, typename Device, typename Index>
template< typename TimeFunction, typename AnalyticSpaceFunction>
void AnalyticSolution<tnlGrid<2,Real,Device,Index,tnlIdenticalGridGeometry>>::
computeLaplace(const MeshType& mesh, const RealType& time, DofVectorType& output,
                const TimeFunction timeFunction, const AnalyticSpaceFunction analyticSpaceFunction)
{
   RealType timeFunctionValue = timeFunction.getTimeValue(time);
   
   VertexType vertex;
   CoordinatesType coordinates;
   CoordinatesType dimensions = mesh.getDimensions();

   #ifdef HAVE_OPENMP
    #pragma omp parallel for private(coordinates,vertex)
   #endif
   for(IndexType i=1; i<(dimensions.x()-1); i++)
   {
      for(IndexType j=1; j<(dimensions.y()-1); j++)
      {  
         coordinates.x()=i;
         coordinates.y()=j;
         
         mesh.getElementCenter(coordinates,vertex);
         
         output[j*mesh.getDimensions().x()+i] = 
                    timeFunctionValue*(analyticSpaceFunction.template getF<2,0,0>(vertex)+
                    analyticSpaceFunction.template getF<0,2,0>(vertex));
      } 
   }
}
   
template<typename Real, typename Device, typename Index>
template< typename TimeFunction, typename AnalyticSpaceFunction>
void AnalyticSolution<tnlGrid<3,Real,Device,Index,tnlIdenticalGridGeometry>>::
computeAnalyticSolution(const MeshType& mesh, const RealType& time, SharedVector& output,
        const TimeFunction timeFunction, const AnalyticSpaceFunction analyticSpaceFunction)
{
   RealType timeFunctionValue = timeFunction.getTimeValue(time);
   
   VertexType vertex;
   CoordinatesType coordinates;

   #ifdef HAVE_OPENMP
    #pragma omp parallel for private(coordinates,vertex)
   #endif
   for(IndexType i=0; i<output.getSize(); i++)
   {      
      mesh.getElementCoordinates(i,coordinates);
      mesh.getElementCenter(coordinates,vertex);
      
      output[i] = timeFunctionValue*analyticSpaceFunction.getF(vertex);
   }   
}

template<typename Real, typename Device, typename Index>
template< typename TimeFunction, typename AnalyticSpaceFunction>
void AnalyticSolution<tnlGrid<3,Real,Device,Index,tnlIdenticalGridGeometry>>::
computeLaplace(const MeshType& mesh, const RealType& time, DofVectorType& output,
                const TimeFunction timeFunction, const AnalyticSpaceFunction analyticSpaceFunction)
{ 
   RealType timeFunctionValue = timeFunction.getTimeValue(time);
      
   VertexType vertex;
   CoordinatesType coordinates;
   CoordinatesType dimensions = mesh.getDimensions();

   #ifdef HAVE_OPENMP
    #pragma omp parallel for private(coordinates,vertex)
   #endif
   for(IndexType i=1; i<(dimensions.x()-1); i++)
   {
      for(IndexType j=1; j<(dimensions.y()-1); j++)
      {  
         for(IndexType k=1; k<(dimensions.y()-1); k++)
         {
            coordinates.x()=i;
            coordinates.y()=j;
            coordinates.z()=k;
            
            mesh.getElementCenter(coordinates,vertex);
         
            output[mesh.getElementIndex(i,j,k)] = 
                       timeFunctionValue*(analyticSpaceFunction.template getF<2,0,0>(vertex)+
                       analyticSpaceFunction.template getF<0,2,0>(vertex)+
                       analyticSpaceFunction.template getF<0,0,2>(vertex));
         }
      } 
   }
}
#endif	/* TNLANALYTICSOLUTION_IMPL_H */