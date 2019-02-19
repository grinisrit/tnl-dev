/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tnlFastSweepingMethod2D_impl.h
 * Author: oberhuber
 *
 * Created on July 14, 2016, 10:32 AM
 */

#pragma once

#include "tnlFastSweepingMethod.h"
#include "tnlDirectEikonalProblem.h"
#include <TNL/Devices/Cuda.h>
#include <TNL/Communicators/MpiDefs.h>
#include "tnlDirectEikonalProblem.h"




#include <string.h>
#include <iostream>
#include <fstream>

template< typename Real,
        typename Device,
        typename Index,
        typename Communicator,
        typename Anisotropy >
FastSweepingMethod< Meshes::Grid< 2, Real, Device, Index >, Communicator, Anisotropy >::
FastSweepingMethod()
: maxIterations( 1 )
{
  
}

template< typename Real,
        typename Device,
        typename Index,
        typename Communicator,
        typename Anisotropy >
const Index&
FastSweepingMethod< Meshes::Grid< 2, Real, Device, Index >, Communicator, Anisotropy >::
getMaxIterations() const
{
  
}

template< typename Real,
        typename Device,
        typename Index,
        typename Communicator,
        typename Anisotropy >
void
FastSweepingMethod< Meshes::Grid< 2, Real, Device, Index >, Communicator, Anisotropy >::
setMaxIterations( const IndexType& maxIterations )
{
  
}

template< typename Real,
        typename Device,
        typename Index,
        typename Communicator,
        typename Anisotropy > 
void
FastSweepingMethod< Meshes::Grid< 2, Real, Device, Index >, Communicator, Anisotropy >::
solve( const MeshPointer& mesh,
        MeshFunctionPointer& Aux,
        const AnisotropyPointer& anisotropy,
        const MeshFunctionPointer& u )
{  
  MeshFunctionPointer auxPtr;
  InterfaceMapPointer interfaceMapPtr;
  auxPtr->setMesh( mesh );
  interfaceMapPtr->setMesh( mesh );
  std::cout << "Initiating the interface cells ..." << std::endl;
  BaseType::initInterface( u, auxPtr, interfaceMapPtr );
  
  auxPtr->save( "aux-ini.tnl" );
  
  typename MeshType::Cell cell( *mesh );
  
  IndexType iteration( 0 );
  InterfaceMapType interfaceMap = *interfaceMapPtr;
  MeshFunctionType aux = *auxPtr;
  aux.template synchronize< Communicator >();
  
  
#ifdef HAVE_MPI
  int i = Communicators::MpiCommunicator::GetRank( Communicators::MpiCommunicator::AllGroup );
  //printf( "Hello world from rank: %d ", i );
  //Communicators::MpiCommunicator::Request r = Communicators::MpiCommunicator::ISend( auxPtr, 0, 0, Communicators::MpiCommunicator::AllGroup );
  if( i == 1 ) {
    /*for( int k = 0; k < 16*16; k++ )
      aux[ k ] = 10;*/
    printf( "1: mesh x: %d\n", mesh->getDimensions().x() );
    printf( "1: mesh y: %d\n", mesh->getDimensions().y() );
    //aux.save("aux_proc1.tnl");
  }
  if( i == 0 ) {
    printf( "0: mesh x: %d\n", mesh->getDimensions().x() );
    printf( "0: mesh y: %d\n", mesh->getDimensions().y() );
    //aux.save("aux_proc0.tnl");
    /*for( int k = 0; k < mesh->getDimensions().x()*mesh->getDimensions().y(); k++ )
      aux[ k ] = 10;
    for( int k = 0; k < mesh->getDimensions().x(); k++ ){
      for( int l = 0; l < mesh->getDimensions().y(); l++ )
        printf("%f.2\t",aux[ k * 16 + l ] );
    printf("\n");
    }*/
  }
    
  /*bool a = Communicators::MpiCommunicator::IsInitialized();
  if( a )
    printf("Je Init\n");
  else
    printf("Neni Init\n");*/
#endif
  
  while( iteration < this->maxIterations )
  {
#ifdef HAVE_MPI
    int i = MPI::GetRank( MPI::AllGroup );
    
    /*if( i == 0 )
    {
      for( int k = 0; k < mesh->getDimensions().y(); k++ ){
        for( int l = 0; l < mesh->getDimensions().x(); l++ )
          printf("%.2f\t",aux[ k * mesh->getDimensions().x() + l ] );
        printf("\n");
      }
    }*/
    aux.template synchronize< Communicator >();
    Meshes::DistributedMeshes::DistributedMesh< MeshType >* meshPom = mesh->getDistributedMesh();
        
    const int *neigh = meshPom->getNeighbors();
    MPI::Request *req;
    req = new MPI::Request[meshPom->getNeighborsCount()];
    int WhileCount = 0;
#endif
    
    Containers::StaticVector< 2, IndexType > vLower = meshPom->getLowerOverlap();
    Containers::StaticVector< 2, IndexType > vUpper = meshPom->getUpperOverlap();
    printf( "%d: meshDimensions are (x,y) = (%d,%d).\n",i, mesh->getDimensions().x(), mesh->getDimensions().y() );
    printf( "%d: owerlaps are ([x1,x2],[y1,y2]) = ([%d,%d],[%d,%d]).\n",i, vLower[0], vUpper[0], vLower[1], vUpper[1] );
    int calculated = 1;
    int calculate = 1;
    
    while( calculated )
    {
      calculated = 0;
      WhileCount++;
      
      if( std::is_same< DeviceType, Devices::Host >::value && calculate )
      {
        calculate = 0;
        
        /**--HERE-IS-PARALLEL-OMP-CODE--!!!WITHOUT MPI!!!--------------------**/
        /*
         int numThreadsPerBlock = -1;
         
         numThreadsPerBlock = ( mesh->getDimensions().x()/2 + (mesh->getDimensions().x() % 2 != 0 ? 1:0));
         //printf("numThreadsPerBlock = %d\n", numThreadsPerBlock);
         if( numThreadsPerBlock <= 16 )
         numThreadsPerBlock = 16;
         else if(numThreadsPerBlock <= 32 )
         numThreadsPerBlock = 32;
         else if(numThreadsPerBlock <= 64 )
         numThreadsPerBlock = 64;
         else if(numThreadsPerBlock <= 128 )
         numThreadsPerBlock = 128;
         else if(numThreadsPerBlock <= 256 )
         numThreadsPerBlock = 256;
         else if(numThreadsPerBlock <= 512 )
         numThreadsPerBlock = 512;
         else
         numThreadsPerBlock = 1024;
         //printf("numThreadsPerBlock = %d\n", numThreadsPerBlock);
         
         if( numThreadsPerBlock == -1 ){
         printf("Fail in setting numThreadsPerBlock.\n");
         break;
         }
         
         
         
         int numBlocksX = mesh->getDimensions().x() / numThreadsPerBlock + (mesh->getDimensions().x() % numThreadsPerBlock != 0 ? 1:0);
         int numBlocksY = mesh->getDimensions().y() / numThreadsPerBlock + (mesh->getDimensions().y() % numThreadsPerBlock != 0 ? 1:0);
         
         //std::cout << "numBlocksX = " << numBlocksX << std::endl;
         
         //Real **sArray = new Real*[numBlocksX*numBlocksY];
         //for( int i = 0; i < numBlocksX * numBlocksY; i++ )
         // sArray[ i ] = new Real [ (numThreadsPerBlock + 2)*(numThreadsPerBlock + 2)];
         
         ArrayContainer BlockIterHost;
         BlockIterHost.setSize( numBlocksX * numBlocksY );
         BlockIterHost.setValue( 1 );
         int IsCalculationDone = 1;
         
         MeshFunctionPointer helpFunc( mesh );
         MeshFunctionPointer helpFunc1( mesh );
         helpFunc1 = auxPtr;
         auxPtr = helpFunc;
         helpFunc = helpFunc1;
         //std::cout<< "Size = " << BlockIterHost.getSize() << std::endl;
         //for( int k = numBlocksX-1; k >-1; k-- ){
         // for( int l = 0; l < numBlocksY; l++ ){
         // std::cout<< BlockIterHost[ l*numBlocksX  + k ];
         // }
         // std::cout<<std::endl;
         // }
         // std::cout<<std::endl;
         unsigned int numWhile = 0;
         while( IsCalculationDone )
         {      
         IsCalculationDone = 0;
         helpFunc1 = auxPtr;
         auxPtr = helpFunc;
         helpFunc = helpFunc1;
         switch ( numThreadsPerBlock ){
         case 16:
         this->template updateBlocks< 18 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         case 32:
         this->template updateBlocks< 34 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         case 64:
         this->template updateBlocks< 66 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         case 128:
         this->template updateBlocks< 130 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         case 256:
         this->template updateBlocks< 258 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         case 512:
         this->template updateBlocks< 514 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         default:
         this->template updateBlocks< 1028 >( interfaceMap, *auxPtr, *helpFunc, BlockIterHost, numThreadsPerBlock );
         }
         
         
         //Reduction      
         for( int i = 0; i < BlockIterHost.getSize(); i++ ){
         if( IsCalculationDone == 0 ){
         IsCalculationDone = IsCalculationDone || BlockIterHost[ i ];
         //break;
         }
         }
         numWhile++;
         //std::cout <<"numWhile = "<< numWhile <<std::endl;
         
         // for( int j = numBlocksY-1; j>-1; j-- ){
         // for( int i = 0; i < numBlocksX; i++ )
         // std::cout << BlockIterHost[ j * numBlocksX + i ];
         // std::cout << std::endl;
         // }
         // std::cout << std::endl;
         
         this->getNeighbours( BlockIterHost, numBlocksX, numBlocksY );
         
         //std::cout<<std::endl;
         //String s( "aux-"+ std::to_string(numWhile) + ".tnl");
         //aux.save( s );
         }
         if( numWhile == 1 ){
         auxPtr = helpFunc;
         }
         */
        /**-END-OF-OMP-PARALLEL------------------------------------------------**/
        
        
        /*if( i == 1 )
         {
         for( int k = 0; k < mesh->getDimensions().y(); k++ ){
         for( int l = 0; l < mesh->getDimensions().x(); l++ )
         printf("%.2f\t",aux[ k * mesh->getDimensions().x() + l ] );
         printf("\n");
         }
         }*/
        
        for( cell.getCoordinates().y() = 0 + vLower[1];
                cell.getCoordinates().y() < mesh->getDimensions().y() - vUpper[1];
                cell.getCoordinates().y()++ )
        {
          for( cell.getCoordinates().x() = 0 + vLower[0];
                  cell.getCoordinates().x() < mesh->getDimensions().x() - vUpper[0];
                  cell.getCoordinates().x()++ )
          {
            cell.refresh();
            if( ! interfaceMap( cell ) )
              calculated = this->updateCell( aux, cell ) || calculated;
          }
        }
        //if( i == 0 )
        //{
        //  for( int k = 0; k < mesh->getDimensions().y(); k++ ){
        //    for( int l = 0; l < mesh->getDimensions().x(); l++ )
        //      printf("%.2f\t",aux[ k * mesh->getDimensions().x() + l ] );
        //    printf("\n");
        //  }
        //}
        
        //aux.save( "aux-1.tnl" );
        
        for( cell.getCoordinates().y() = 0 + vLower[1];
                cell.getCoordinates().y() < mesh->getDimensions().y()-vUpper[1];
                cell.getCoordinates().y()++ )
        {
          for( cell.getCoordinates().x() = mesh->getDimensions().x() - 1 - vUpper[0];
                  cell.getCoordinates().x() >= 0 + vLower[0];
                  cell.getCoordinates().x()-- )		
          {
            //std::cerr << "2 -> ";
            cell.refresh();
            if( ! interfaceMap( cell ) )            
              this->updateCell( aux, cell );
          }
        }
        
        //aux.save( "aux-2.tnl" );
        
        for( cell.getCoordinates().y() = mesh->getDimensions().y() - 1 -vUpper[1];
                cell.getCoordinates().y() >= 0 + vLower[1] ;
                cell.getCoordinates().y()-- )
        {
          for( cell.getCoordinates().x() = 0 + vLower[0];
                  cell.getCoordinates().x() < mesh->getDimensions().x() - vUpper[0];
                  cell.getCoordinates().x()++ )
          {
            //std::cerr << "3 -> ";
            cell.refresh();
            if( ! interfaceMap( cell ) )            
              this->updateCell( aux, cell );
          }
        }
        
        //aux.save( "aux-3.tnl" );
        
        for( cell.getCoordinates().y() = mesh->getDimensions().y() - 1 - vUpper[1];
                cell.getCoordinates().y() >= 0 + vLower[1];
                cell.getCoordinates().y()-- )
        {
          for( cell.getCoordinates().x() = mesh->getDimensions().x() - 1 - vUpper[0];
                  cell.getCoordinates().x() >= 0 + vLower[0];
                  cell.getCoordinates().x()-- )		
          {
            //std::cerr << "4 -> ";
            cell.refresh();
            if( ! interfaceMap( cell ) )            
              this->updateCell( aux, cell );
          }
        }
        
      }
      if( std::is_same< DeviceType, Devices::Cuda >::value && calculate )
      {
        // TODO: CUDA code
        
        calculate = 0;
        //if( i == 0 )
        //  printf("%d: We are in Cuda code start.\n", i);
#ifdef HAVE_CUDA
        
        TNL_CHECK_CUDA_DEVICE;
        // Maximum cudaBlockSite is 32. Because of maximum num. of threads in kernel.
        const int cudaBlockSize( 16 );
        
        int numBlocksX = Devices::Cuda::getNumberOfBlocks( mesh->getDimensions().x() - vLower[0] - vUpper[0], cudaBlockSize );
        int numBlocksXbez = Devices::Cuda::getNumberOfBlocks( mesh->getDimensions().x(), cudaBlockSize );
        int numBlocksY = Devices::Cuda::getNumberOfBlocks( mesh->getDimensions().y() - vLower[1] - vUpper[1], cudaBlockSize );
        int numBlocksYbez = Devices::Cuda::getNumberOfBlocks( mesh->getDimensions().y(), cudaBlockSize );
        dim3 blockSize( cudaBlockSize, cudaBlockSize );
        dim3 gridSizeBez( numBlocksXbez, numBlocksYbez );
        dim3 gridSize( numBlocksX, numBlocksY );
        
        tnlDirectEikonalMethodsBase< Meshes::Grid< 2, Real, Device, Index > > ptr;
        
        int BlockIterD = 1;
        /*auxPtr = helpFunc;
         
         CudaUpdateCellCaller<18><<< gridSize, blockSize >>>( ptr,
         interfaceMapPtr.template getData< Device >(),
         auxPtr.template getData< Device>(),
         helpFunc.template modifyData< Device>(),
         BlockIterDevice,
         oddEvenBlock.getView() );
         cudaDeviceSynchronize();
         TNL_CHECK_CUDA_DEVICE;
         auxPtr = helpFunc;
         
         oddEvenBlock= (oddEvenBlock == 0) ? 1: 0;
         
         CudaUpdateCellCaller<18><<< gridSize, blockSize >>>( ptr,
         interfaceMapPtr.template getData< Device >(),
         auxPtr.template getData< Device>(),
         helpFunc.template modifyData< Device>(),
         BlockIterDevice,
         oddEvenBlock.getView() );
         cudaDeviceSynchronize();
         TNL_CHECK_CUDA_DEVICE;
         auxPtr = helpFunc;
         
         oddEvenBlock= (oddEvenBlock == 0) ? 1: 0;
         
         CudaParallelReduc<<< nBlocks , 1024 >>>( BlockIterDevice.getView(), dBlock.getView(), ( numBlocksX * numBlocksY ) );
         cudaDeviceSynchronize();
         TNL_CHECK_CUDA_DEVICE;
         CudaParallelReduc<<< 1, nBlocks >>>( dBlock.getView(), dBlock.getView(), nBlocks );
         cudaDeviceSynchronize();
         TNL_CHECK_CUDA_DEVICE;
         
         BlockIterD = dBlock.getElement( 0 );*/
        
        TNL::Containers::Array< int, Devices::Cuda, IndexType > BlockIterDevice;
        BlockIterDevice.setSize( numBlocksX * numBlocksY );
        BlockIterDevice.setValue( 1 );
        TNL_CHECK_CUDA_DEVICE;
        
        
        TNL::Containers::Array< int, Devices::Cuda, IndexType > BlockIterPom;
        BlockIterPom.setSize( numBlocksX * numBlocksY  );
        BlockIterPom.setValue( 0 );
        TNL::Containers::Array< int, Devices::Host, IndexType > BlockIterPom1;
        BlockIterPom1.setSize( numBlocksX * numBlocksY  );
        BlockIterPom1.setValue( 0 );
        /*int *BlockIterDevice;
         cudaMalloc((void**) &BlockIterDevice, ( numBlocksX * numBlocksY ) * sizeof( int ) );*/
        int nBlocksNeigh = ( numBlocksX * numBlocksY )/1024 + ((( numBlocksX * numBlocksY )%1024 != 0) ? 1:0);
        //std::cout << "nBlocksNeigh = " << nBlocksNeigh << std::endl;
        //free( BlockIter );
        /*int *BlockIterPom;
         cudaMalloc((void**) &BlockIterPom, ( numBlocksX * numBlocksY ) * sizeof( int ) );*/
        
        int nBlocks = ( numBlocksX * numBlocksY )/1024 + ((( numBlocksX * numBlocksY )%1024 != 0) ? 1:0);
        
        TNL::Containers::Array< int, Devices::Cuda, IndexType > dBlock;
        dBlock.setSize( nBlocks );
        TNL_CHECK_CUDA_DEVICE;
        /*int *dBlock;
         cudaMalloc((void**) &dBlock, nBlocks * sizeof( int ) );*/
        
        
        MeshFunctionPointer helpFunc1( mesh );      
        MeshFunctionPointer helpFunc( mesh );
        //helpFunc->bind( auxPtr->getData() );
        DeepCopy<<< gridSizeBez, blockSize >>>( auxPtr.template getData< Device>(),
           helpFunc.template modifyData< Device>() );
        
        helpFunc1 = auxPtr;
        auxPtr = helpFunc;
        helpFunc = helpFunc1;
        
        //int pocBloku = 0;
        Devices::Cuda::synchronizeDevice();
        CudaUpdateCellCaller<18><<< gridSize, blockSize >>>( ptr,
                interfaceMapPtr.template getData< Device >(),
                auxPtr.template modifyData< Device>(),
                helpFunc.template modifyData< Device>(),
                BlockIterDevice.getView() );
        cudaDeviceSynchronize();
        TNL_CHECK_CUDA_DEVICE;
        
        //int oddEvenBlock = 0;
        //int numberWhile = 0;
        while( BlockIterD /*numberWhile < 10*/)
        {
          //numberWhile++;
          /** HERE IS CHESS METHOD **/
          
          /*auxPtr = helpFunc;
           
           CudaUpdateCellCaller<18><<< gridSize, blockSize >>>( ptr,
           interfaceMapPtr.template getData< Device >(),
           auxPtr.template getData< Device>(),
           helpFunc.template modifyData< Device>(),
           BlockIterDevice,
           oddEvenBlock );
           cudaDeviceSynchronize();
           TNL_CHECK_CUDA_DEVICE;
           auxPtr = helpFunc;
           
           oddEvenBlock= (oddEvenBlock == 0) ? 1: 0;
           
           CudaUpdateCellCaller<18><<< gridSize, blockSize >>>( ptr,
           interfaceMapPtr.template getData< Device >(),
           auxPtr.template getData< Device>(),
           helpFunc.template modifyData< Device>(),
           BlockIterDevice, vLower, vUpper, 
           oddEvenBlock );
           cudaDeviceSynchronize();
           TNL_CHECK_CUDA_DEVICE;
           auxPtr = helpFunc;
           
           oddEvenBlock= (oddEvenBlock == 0) ? 1: 0;
           
           CudaParallelReduc<<< nBlocks , 1024 >>>( BlockIterDevice, dBlock, ( numBlocksX * numBlocksY ) );
           cudaDeviceSynchronize();
           TNL_CHECK_CUDA_DEVICE;
           CudaParallelReduc<<< 1, nBlocks >>>( dBlock, dBlock, nBlocks );
           cudaDeviceSynchronize();
           TNL_CHECK_CUDA_DEVICE;
           
           BlockIterD = dBlock.getElement( 0 );*/
          
          /**------------------------------------------------------------------------------------------------*/
          
          
          /** HERE IS FIM **/
          
          helpFunc1 = auxPtr;
          auxPtr = helpFunc;
          helpFunc = helpFunc1;
          TNL_CHECK_CUDA_DEVICE;
          
          //int pocBloku = 0;
          Devices::Cuda::synchronizeDevice();
          CudaUpdateCellCaller<18><<< gridSize, blockSize >>>( ptr,
                  interfaceMapPtr.template getData< Device >(),
                  auxPtr.template modifyData< Device>(),
                  helpFunc.template modifyData< Device>(),
                  BlockIterDevice, vLower, vUpper, i );
          cudaDeviceSynchronize();
          TNL_CHECK_CUDA_DEVICE;
        
        cudaDeviceSynchronize();
        TNL_CHECK_CUDA_DEVICE;
        
        
        aux = *auxPtr;
        interfaceMap = *interfaceMapPtr;
#endif
      }

      
/**----------------------MPI-TO-DO---------------------------------------------**/
        
#ifdef HAVE_MPI
        //int i = MPI::GetRank( MPI::AllGroup );
        //TNL::Meshes::DistributedMeshes::DistributedMesh< MeshType > Mesh;
        int neighCount = 0; // should this thread calculate again?
        int calculpom[4] = {0,0,0,0};
        
          if( i == 0 ){
            BlockIterPom1 = BlockIterDevice;
            for( int i =0; i< numBlocksX; i++ ){
              for( int j = 0; j < numBlocksY; j++ )
              {
                std::cout << BlockIterPom1[j*numBlocksX + i];
              }
              std::cout << std::endl;
            }
            std::cout << std::endl;
          }
          GetNeighbours<<< nBlocksNeigh, 1024 >>>( BlockIterDevice, BlockIterPom, numBlocksX, numBlocksY );
          cudaDeviceSynchronize();
          TNL_CHECK_CUDA_DEVICE;
          BlockIterDevice = BlockIterPom;
          
          if( i == 0 ){
            BlockIterPom1 = BlockIterDevice;
            for( int i =0; i< numBlocksX; i++ ){
              for( int j = 0; j < numBlocksY; j++ )
              {
                std::cout << BlockIterPom1[j*numBlocksX + i];
              }
              std::cout << std::endl;
            }
            std::cout << std::endl;
          }
          //std::cout<< "Probehlo" << std::endl;
          
          //TNL::swap( auxPtr, helpFunc );
          
          CudaParallelReduc<<< nBlocks , 1024 >>>( BlockIterDevice, dBlock, ( numBlocksX * numBlocksY ) );
          TNL_CHECK_CUDA_DEVICE;
          
          CudaParallelReduc<<< 1, nBlocks >>>( dBlock, dBlock, nBlocks );
          TNL_CHECK_CUDA_DEVICE;
          
          
          //if( i == 0 )
          //  printf("%d: We did parallel reduction.\n", i);
          BlockIterD = dBlock.getElement( 0 );
          
          //cudaMemcpy( &BlockIterD, &dBlock[0], sizeof( int ), cudaMemcpyDeviceToHost);
          cudaDeviceSynchronize();
          TNL_CHECK_CUDA_DEVICE;
          //if( i == 0 )
          //  printf("%d: BlockIterD = %d.\n", i, BlockIterD);
          
#ifdef HAVE_MPI
          if( BlockIterD ){
            calculated = 1;
            //printf( "calculated = %d\n",calculated );
          }
#endif
          /**-----------------------------------------------------------------------------------------------------------*/
          /*for( int i = 1; i < numBlocksX * numBlocksY; i++ )
           BlockIter[ 0 ] = BlockIter[ 0 ] || BlockIter[ i ];*/
          numIter ++;
        }
        if( numIter%2  == 1 ){
          auxPtr = helpFunc;
        }
        /*cudaFree( BlockIterDevice );
         cudaFree( dBlock );
         delete BlockIter;*/
        
        if( neigh[1] != -1 )
        {
          req[neighCount] = MPI::ISend( &calculated, 1, neigh[1], 0, MPI::AllGroup ); 
          neighCount++;
          
          
          req[neighCount] = MPI::IRecv( &calculpom[1], 1, neigh[1], 0, MPI::AllGroup );
          neighCount++;
        }
        
        if( neigh[2] != -1 )
        {
          req[neighCount] = MPI::ISend( &calculated, 1, neigh[2], 0, MPI::AllGroup );
          neighCount++;
          
          req[neighCount] = MPI::IRecv( &calculpom[2], 1, neigh[2], 0, MPI::AllGroup  );
          neighCount++;
        }
        
        if( neigh[5] != -1 )
        {
          req[neighCount] = MPI::ISend( &calculated, 1, neigh[5], 0, MPI::AllGroup );
          neighCount++;
          
          req[neighCount] = MPI::IRecv( &calculpom[3], 1, neigh[5], 0, MPI::AllGroup );
          neighCount++;
        }
        
        MPI::WaitAll(req,neighCount);
        MPI::Allreduce( &calculated, &calculated, 1, MPI_LOR,  MPI::AllGroup );
        aux.template synchronize< Communicator >();
        calculate = calculpom[0] || calculpom[1] || calculpom[2] || calculpom[3];
        aux.template synchronize< Communicator >();
        
        //printf( "%d: Receved reduced info about Calculated = %d.\n", i,calculated);
        
        
        if( i == 0 )
          printf("WhileCount = %d\n",WhileCount);
        //calculated = 0; /// DEBUG;
      }
      
      String s( "aux-" + std::to_string( i ) + ".tnl" );
      aux.save( s );   
      Aux=auxPtr;
      
      /*if( i == 0 )
      {
        for( int k = 0; k < mesh->getDimensions().y(); k++ ){
          for( int l = 0; l < mesh->getDimensions().x(); l++ )
            printf("%.2f\t",aux[ k * mesh->getDimensions().x() + l ] );
          printf("\n");
        }
          printf("\n");
        for( int k = 0; k < mesh->getDimensions().y(); k++ ){
          for( int l = 0; l < mesh->getDimensions().x(); l++ )
            printf("%.2f\t",(*Aux)[ k * mesh->getDimensions().x() + l ] );
          printf("\n");
        }
      }*/
#endif
      iteration++;
  }
  aux.save("aux-final.tnl");
}


#ifdef HAVE_CUDA
// DeepCopy nebo pracne kopirovat kraje v zavislosti na vLower,vUpper z sArray do helpFunc.
template< typename Real, typename Device, typename Index >
__global__ void DeepCopy( const Functions::MeshFunction< Meshes::Grid< 2, Real, Device, Index > >& aux,
        Functions::MeshFunction< Meshes::Grid< 2, Real, Device, Index > >& helpFunc )
{
  int i = threadIdx.x + blockDim.x*blockIdx.x;
  int j = blockDim.y*blockIdx.y + threadIdx.y;
  const Meshes::Grid< 2, Real, Device, Index >& mesh = aux.template getMesh< Devices::Cuda >();
  if( i < mesh.getDimensions().x() && j < mesh.getDimensions().y() )
    helpFunc[ j * mesh.getDimensions().x() + i ] = aux[ j * mesh.getDimensions().x() + i ];
}

template < typename Index >
__global__ void GetNeighbours( TNL::Containers::ArrayView< int, Devices::Cuda, Index > BlockIterDevice,
        TNL::Containers::ArrayView< int, Devices::Cuda, Index > BlockIterPom, int numBlockX, int numBlockY )
{
  int i = blockIdx.x * 1024 + threadIdx.x;
  
  if( i < numBlockX * numBlockY )
  {
    int pom = 0;//BlockIterPom[ i ] = 0;
    int m=0, k=0;
    m = i%numBlockX;
    k = i/numBlockX;
    if( m > 0 && BlockIterDevice[ i - 1 ] ){
      pom = 1;//BlockIterPom[ i ] = 1;
    }else if( m < numBlockX -1 && BlockIterDevice[ i + 1 ] ){
      pom = 1;//BlockIterPom[ i ] = 1;
    }else if( k > 0 && BlockIterDevice[ i - numBlockX ] ){
      pom = 1;// BlockIterPom[ i ] = 1;
    }else if( k < numBlockY -1 && BlockIterDevice[ i + numBlockX ] ){
      pom = 1;//BlockIterPom[ i ] = 1;
    }
    
    BlockIterPom[ i ] = pom;//BlockIterPom[ i ];
  }
}

template < typename Index >
__global__ void CudaParallelReduc( TNL::Containers::ArrayView< int, Devices::Cuda, Index > BlockIterDevice,
        TNL::Containers::ArrayView< int, Devices::Cuda, Index > dBlock, int nBlocks )
{
  int i = threadIdx.x;
  int blId = blockIdx.x;
  int blockSize = blockDim.x;
  /*if ( i == 0 && blId == 0 ){
   printf( "nBlocks = %d \n", nBlocks );
   for( int j = nBlocks-1; j > -1 ; j--){
   printf( "cislo = %d \n", BlockIterDevice[ j ] );
   }
   }*/
  __shared__ int sArray[ 1024 ];
  sArray[ i ] = 0;
  if( blId * 1024 + i < nBlocks )
    sArray[ i ] = BlockIterDevice[ blId * 1024 + i ];
  __syncthreads();
  /*extern __shared__ volatile int sArray[];
   unsigned int i = threadIdx.x;
   unsigned int gid = blockIdx.x * blockSize * 2 + threadIdx.x;
   unsigned int gridSize = blockSize * 2 * gridDim.x;
   sArray[ i ] = 0;
   while( gid < nBlocks )
   {
   sArray[ i ] += BlockIterDevice[ gid ] + BlockIterDevice[ gid + blockSize ];
   gid += gridSize;
   }
   __syncthreads();*/
  
  if ( blockSize == 1024) {
    if (i < 512)
      sArray[ i ] += sArray[ i + 512 ];
  }
  __syncthreads();
  if (blockSize >= 512) {
    if (i < 256) {
      sArray[ i ] += sArray[ i + 256 ];
    }
  }
  __syncthreads();
  if (blockSize >= 256) {
    if (i < 128) {
      sArray[ i ] += sArray[ i + 128 ];
    }
  }
  __syncthreads();
  if (blockSize >= 128) {
    if (i < 64) {
      sArray[ i ] += sArray[ i + 64 ];
    }
  }
  __syncthreads();
  if (i < 32 )
  {
    if(  blockSize >= 64 ) sArray[ i ] += sArray[ i + 32 ];
    if(  blockSize >= 32 )  sArray[ i ] += sArray[ i + 16 ];
    if(  blockSize >= 16 )  sArray[ i ] += sArray[ i + 8 ];
    if(  blockSize >= 8 )  sArray[ i ] += sArray[ i + 4 ];
    if(  blockSize >= 4 )  sArray[ i ] += sArray[ i + 2 ];
    if(  blockSize >= 2 )  sArray[ i ] += sArray[ i + 1 ];
  }
  
  if( i == 0 )
    dBlock[ blId ] = sArray[ 0 ];
}



template < int sizeSArray, typename Real, typename Device, typename Index >
__global__ void CudaUpdateCellCaller( tnlDirectEikonalMethodsBase< Meshes::Grid< 2, Real, Device, Index > > ptr,
        const Functions::MeshFunction< Meshes::Grid< 2, Real, Device, Index >, 2, bool >& interfaceMap,
        const Functions::MeshFunction< Meshes::Grid< 2, Real, Device, Index > >& aux,
        Functions::MeshFunction< Meshes::Grid< 2, Real, Device, Index > >& helpFunc,
        CudaParallelReduc<<< nBlocks , 1024 >>>( BlockIterDevice.getView(), dBlock.getView(), ( numBlocksX * numBlocksY ) );
        TNL_CHECK_CUDA_DEVICE;
        
        CudaParallelReduc<<< 1, nBlocks >>>( dBlock.getView(), dBlock.getView(), nBlocks );
        TNL_CHECK_CUDA_DEVICE;
{
  int thri = threadIdx.x; int thrj = threadIdx.y;
  int i = threadIdx.x + blockDim.x*blockIdx.x + vLower[0];
  int j = blockDim.y*blockIdx.y + threadIdx.y + vLower[1];
  const Meshes::Grid< 2, Real, Device, Index >& mesh = aux.template getMesh< Devices::Cuda >();
  /** FOR CHESS METHOD */
  //if( (blockIdx.y%2  + blockIdx.x) % 2 == oddEvenBlock )
  //{
  /**------------------------------------------*/
  
  
  /** FOR FIM METHOD */
  
  if( BlockIterDevice[ blockIdx.y * gridDim.x + blockIdx.x ] )
  { 
    __syncthreads();
    
    /**-----------------------------------------*/
    __shared__ int dimX;
    __shared__ int dimY;
    __shared__ Real hx;
    __shared__ Real hy;
    if( thri==0 && thrj == 0)
    {
      dimX = mesh.getDimensions().x();
      dimY = mesh.getDimensions().y();
      hx = mesh.getSpaceSteps().x();
      hy = mesh.getSpaceSteps().y();
      BlockIterDevice[ blockIdx.y * gridDim.x + blockIdx.x ] = 0;
    }
    __syncthreads();
    int numOfBlockx;
    int numOfBlocky;
    int xkolik;
    int ykolik;
    
    xkolik = blockDim.x + 1;
    ykolik = blockDim.y + 1;
    numOfBlocky = (dimY-vUpper[1]-vLower[1])/blockDim.y + (((dimY-vUpper[1]-vLower[1])%blockDim.y != 0) ? 1:0);
    numOfBlockx = (dimX-vUpper[0]-vLower[0])/blockDim.x + (((dimX-vUpper[0]-vLower[0])%blockDim.x != 0) ? 1:0);
    
    if( numOfBlockx - 1 == blockIdx.x )
      xkolik = (dimX-vUpper[0]-vLower[0]) - (blockIdx.x)*blockDim.x+1;
    
    if( numOfBlocky -1 == blockIdx.y )
      ykolik = (dimY-vUpper[1]-vLower[1]) - (blockIdx.y)*blockDim.y+1;
    __syncthreads();
    
    /*if( thri==0 && thrj == 0 )
    {
      printf("%d: DimX = %d, DimY = %d, xKolik = %d, yKolik = %d, numOfBlockX = %d, numOfBlockY = %d, blockIdx.x = %d, blockIdx.y = %d.\n",
              k, dimX, dimY, xkolik, ykolik, numOfBlockx, numOfBlocky, blockIdx.x, blockIdx.y);
    }*/
    
    int currentIndex = thrj * blockDim.x + thri;
    //__shared__ volatile bool changed[ blockDim.x*blockDim.y ];
    __shared__ volatile bool changed[ (sizeSArray-2)*(sizeSArray-2)];
    changed[ currentIndex ] = false;
    if( thrj == 0 && thri == 0 )
      changed[ 0 ] = true;
    
    
    //__shared__ volatile Real sArray[ blockDim.y+2 ][ blockDim.x+2 ];
    __shared__ volatile Real sArray[ sizeSArray * sizeSArray ];
    sArray[ (thrj+1) * sizeSArray + thri +1 ] = 10;//std::numeric_limits< Real >::max();
    
    /*if( thri==0 && thrj == 0 && blockIdx.x == 0 && blockIdx.y == 0 && k == 3 )
    {
      printf( "Kraje: \n");
      for( int k = sizeSArray-1; k>-1; k-- ){
        for( int l = 0; l < sizeSArray; l++ )
          printf( "%.4f ", sArray[k * sizeSArray + l]);
        printf( "\n");
      }
      printf( "\n");
    }
    __syncthreads();*/
    
        //filling sArray edges
    if( thri == 0 )
    {      
      if( dimX - vLower[ 0 ] > (blockIdx.x+1) * blockDim.x  && thrj+1 < ykolik )
        sArray[(thrj+1)*sizeSArray + xkolik] = aux[ (blockIdx.y*blockDim.y+vLower[1])*dimX - dimX + blockIdx.x*blockDim.x - 1 + (thrj+1)*dimX + xkolik + vLower[0] ];
      else
        sArray[(thrj+1)*sizeSArray + xkolik] = 10;//std::numeric_limits< Real >::max();
    }
        
    if( thri == 1 )
    { 
      if( ( blockIdx.x != 0 || vLower[0] != 0 ) && thrj+1 < ykolik )
        sArray[(thrj+1)*sizeSArray + 0] = aux[ (blockIdx.y*blockDim.y+vLower[1])*dimX - dimX + blockIdx.x*blockDim.x - 1 + (thrj+1)*dimX  + vLower[0] ];
      else
        sArray[(thrj+1)*sizeSArray + 0] = 10;//std::numeric_limits< Real >::max();
    }
    
    if( thri == 2 )
    {
      if( dimY - vLower[ 1 ] > (blockIdx.y+1) * blockDim.y  && thrj+1 < xkolik )
        sArray[ ykolik*sizeSArray + thrj+1 ] = aux[ (blockIdx.y*blockDim.y+vLower[1])*dimX - dimX + blockIdx.x*blockDim.x - 1 + ykolik*dimX + thrj+1 + vLower[0] ];
      else
        sArray[ ykolik*sizeSArray + thrj+1 ] = 10;//std::numeric_limits< Real >::max();
      
    }
        
    if( thri == 3 )
    {
      if( ( blockIdx.y != 0 || vLower[1] != 0 ) && thrj+1 < xkolik )
        sArray[0*sizeSArray + thrj+1] = aux[ (blockIdx.y*blockDim.y+vLower[1])*dimX - dimX + blockIdx.x*blockDim.x - 1 + thrj+1 + vLower[0] ];
      else
        sArray[0*sizeSArray + thrj+1] = 10;//std::numeric_limits< Real >::max();
    }
    /*__syncthreads();
    if( thri==0 && thrj == 0 && blockIdx.x == 0 && blockIdx.y == 0 && k == 1 )
    {
      printf( "Kraje: \n");
      for( int k = sizeSArray-1; k>-1; k-- ){
        for( int l = 0; l < sizeSArray; l++ )
          printf( "%.4f ", sArray[k * sizeSArray + l]);
        printf( "\n");
      }
      printf( "\n");
    }
    __syncthreads();*/
    
    
    if( i < dimX && j < dimY && thri+1 < xkolik && thrj+1 < ykolik )
    {  
      /*if( k == 3 && blockIdx.x == 0 && blockIdx.y == 0 )
        printf("at index = %d\n", j*dimX + i);*/
      sArray[(thrj+1)*sizeSArray + thri+1] = aux[ j*dimX + i ];
    }
    __syncthreads();  
    if( thri==0 && thrj == 0 && blockIdx.x == 0 && blockIdx.y == 0 && k == 3 )
    {
      printf( "všechno před výpočtem: \n");
      for( int k = sizeSArray-1; k>-1; k-- ){
        for( int l = 0; l < sizeSArray; l++ )
          printf( "%.4f ", sArray[k * sizeSArray + l]);
        printf( "\n");
      }
      printf( "\n");
    }
    
    if( thri==0 && thrj == 0 && blockIdx.x == 0 && blockIdx.y == 0 && k == 3 )
    {
      for( int k = mesh.getDimensions().y()-1; k>-1; k-- ){
        for( int l = 0; l < 17; l++ )
          printf( "%.2f ", aux[ k * mesh.getDimensions().x() + l ]);
        printf( "\n");
      }
      printf( "\n");
    }
    
    //main while cycle
    //if( i == 0 && j == 0 )
    //  printf("Overlaps [x1,y1],[x2,y2] = [%d,%d],[%d,%d]",vLower[0], vLower[1], vUpper[0], vUpper[1] );
    
    while( changed[ 0 ] )
    {
      __syncthreads();
      
      changed[ currentIndex] = false;
      
      //calculation of update cell
      if( i < dimX - vUpper[0] && j < dimY - vUpper[1] /*&& i > vLower[0]-1 && j > vLower[1]-1*/ )
      {
        if( ! interfaceMap[ j * dimX + i ] )
        {
          /*if( k == 1 && blockIdx.x == 1 && blockIdx.y == 0 )
            printf( "thri = %d, thrj = %d \n", thri, thrj );*/
          changed[ currentIndex ] = ptr.updateCell<sizeSArray>( sArray, thri+1, thrj+1, hx,hy);
        }
      }
      __syncthreads();
      
      //pyramid reduction
      if( blockDim.x*blockDim.y == 1024 )
      {
        if( currentIndex < 512 )
        {
          changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 512 ];
        }
      }
      __syncthreads();
      if( blockDim.x*blockDim.y >= 512 )
      {
        if( currentIndex < 256 )
        {
          changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 256 ];
        }
      }
      __syncthreads();
      if( blockDim.x*blockDim.y >= 256 )
      {
        if( currentIndex < 128 )
        {
          changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 128 ];
        }
      }
      __syncthreads();
      if( blockDim.x*blockDim.y >= 128 )
      {
        if( currentIndex < 64 )
        {
          changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 64 ];
        }
      }
      __syncthreads();
      if( currentIndex < 32 ) 
      {
        if( true ) changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 32 ];
        if( currentIndex < 16 ) changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 16 ];
        if( currentIndex < 8 ) changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 8 ];
        if( currentIndex < 4 ) changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 4 ];
        if( currentIndex < 2 ) changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 2 ];
        if( currentIndex < 1 ) changed[ currentIndex ] = changed[ currentIndex ] || changed[ currentIndex + 1 ];
      }
      if( thri == 0 && thrj == 0 && changed[ 0 ] ){
        BlockIterDevice[ blockIdx.y * gridDim.x + blockIdx.x ] = 1;
      }
      __syncthreads();
    }
    
    
      
    if( i < dimX && j < dimY && thri+1 < xkolik && thrj+1 < ykolik )
      helpFunc[ j * dimX + i ] = sArray[ ( thrj + 1 ) * sizeSArray + thri + 1 ];
    __syncthreads();
    /*if( thri==0 && thrj == 0 && blockIdx.x == 0 && blockIdx.y == 0 && k == 1 )
    {
      printf( "všechno po výpočtu: \n");
      for( int k = sizeSArray-1; k>-1; k-- ){
        for( int l = 0; l < sizeSArray; l++ )
          printf( "%.4f ", sArray[k * sizeSArray + l]);
        printf( "\n");
      }
      printf( "\n");
    }*/
    
    /*if( thri==0 && thrj == 0 && blockIdx.x == 1 && blockIdx.y == 1 && k == 1 )
    {
      printf( "8: \n");
      for( int k = mesh.getDimensions().y()-1; k>-1; k-- ){
        for( int l = 0; l < mesh.getDimensions().x(); l++ )
          printf( "%.2f\t", helpFunc[ k * mesh.getDimensions().x() + l ]);
        printf("\n");
      }
      printf( "\n");
    }*/
  }
  else
  {
    if( i < mesh.getDimensions().x() && j < mesh.getDimensions().y() )
      helpFunc[ j * mesh.getDimensions().x() + i ] = aux[ j * mesh.getDimensions().x() + i ];
  }
}
#endif
        TNL::Containers::ArrayView< int, Devices::Cuda, Index > BlockIterDevice, int oddEvenBlock )
