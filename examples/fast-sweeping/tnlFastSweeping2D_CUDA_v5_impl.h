/***************************************************************************
                          tnlFastSweeping2D_CUDA_v5_impl.h  -  description
                             -------------------
    begin                : Oct 15 , 2015
    copyright            : (C) 2015 by Tomas Sobotik
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef TNLFASTSWEEPING2D_IMPL_H_
#define TNLFASTSWEEPING2D_IMPL_H_

#include "tnlFastSweeping.h"

__device__
double fabsMin( double x, double y)
{
	double fx = abs(x);

	if(Min(fx,abs(y)) == fx)
		return x;
	else
		return y;
}

__device__
double atomicFabsMin(double* address, double val)
{
	unsigned long long int* address_as_ull =
						  (unsigned long long int*)address;
	unsigned long long int old = *address_as_ull, assumed;
	do {
		assumed = old;
			old = atomicCAS(address_as_ull, assumed,__double_as_longlong( fabsMin(assumed,val) ));
	} while (assumed != old);
	return __longlong_as_double(old);
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
tnlString tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: getType()
{
	   return tnlString( "tnlFastSweeping< " ) +
	          MeshType::getType() + ", " +
	          ::getType< Real >() + ", " +
	          ::getType< Index >() + " >";
}




template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
bool tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: init( const tnlParameterContainer& parameters )
{
	const tnlString& meshFile = parameters.getParameter< tnlString >( "mesh" );

	if( ! Mesh.load( meshFile ) )
	{
		   cerr << "I am not able to load the mesh from the file " << meshFile << "." << endl;
		   return false;
	}


	const tnlString& initialCondition = parameters.getParameter <tnlString>("initial-condition");
	if( ! dofVector.load( initialCondition ) )
	{
		   cerr << "I am not able to load the initial condition from the file " << meshFile << "." << endl;
		   return false;
	}

	h = Mesh.getHx();
	counter = 0;

	const tnlString& exact_input = parameters.getParameter< tnlString >( "exact-input" );

	if(exact_input == "no")
		exactInput=false;
	else
		exactInput=true;


#ifdef HAVE_CUDA

	cudaMalloc(&(cudaDofVector), this->dofVector.getSize()*sizeof(double));
	cudaMemcpy(cudaDofVector, this->dofVector.getData(), this->dofVector.getSize()*sizeof(double), cudaMemcpyHostToDevice);

	cudaMalloc(&(cudaDofVector2), this->dofVector.getSize()*sizeof(double));
	cudaMemcpy(cudaDofVector2, this->dofVector.getData(), this->dofVector.getSize()*sizeof(double), cudaMemcpyHostToDevice);


	cudaMalloc(&(this->cudaSolver), sizeof(tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index >));
	cudaMemcpy(this->cudaSolver, this,sizeof(tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index >), cudaMemcpyHostToDevice);

#endif

	int n = Mesh.getDimensions().x();
	dim3 threadsPerBlock(16, 16);
	dim3 numBlocks(n/16 + 1 ,n/16 +1);

	initCUDA<<<numBlocks,threadsPerBlock>>>(this->cudaSolver);
	cudaDeviceSynchronize();
	checkCudaDevice;

	return true;
}





template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
bool tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: run()
{
//
//	for(Index i = 0; i < Mesh.getDimensions().x(); i++)
//	{
//		for(Index j = 0; j < Mesh.getDimensions().y(); j++)
//		{
//			updateValue(i,j);
//		}
//	}
//
///*---------------------------------------------------------------------------------------------------------------------------*/
//
//	for(Index i = Mesh.getDimensions().x() - 1; i > -1; i--)
//	{
//		for(Index j = 0; j < Mesh.getDimensions().y(); j++)
//		{
//			updateValue(i,j);
//		}
//	}
//
///*---------------------------------------------------------------------------------------------------------------------------*/
//
//	for(Index i = Mesh.getDimensions().x() - 1; i > -1; i--)
//	{
//		for(Index j = Mesh.getDimensions().y() - 1; j > -1; j--)
//		{
//			updateValue(i,j);
//		}
//	}
//
///*---------------------------------------------------------------------------------------------------------------------------*/
//	for(Index i = 0; i < Mesh.getDimensions().x(); i++)
//	{
//		for(Index j = Mesh.getDimensions().y() - 1; j > -1; j--)
//		{
//			updateValue(i,j);
//		}
//	}
//
///*---------------------------------------------------------------------------------------------------------------------------*/
//
//
//	dofVector.save("u-00001.tnl");

	int n = Mesh.getDimensions().x();
	dim3 threadsPerBlock(1, 512);
	dim3 numBlocks(4,1);


	runCUDA<<<numBlocks,threadsPerBlock,3*(512+1)*sizeof(double)>>>(this->cudaSolver,0,0);

	cudaDeviceSynchronize();
	checkCudaDevice;

	cudaMemcpy(this->dofVector.getData(), cudaDofVector, this->dofVector.getSize()*sizeof(double), cudaMemcpyDeviceToHost);
	cudaDeviceSynchronize();
	cudaFree(cudaDofVector);
	cudaFree(cudaDofVector2);
	cudaFree(cudaSolver);
	dofVector.save("u-00001.tnl");
	cudaDeviceSynchronize();
	return true;
}




#ifdef HAVE_CUDA


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
__device__
void tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: updateValue( Index i, Index j)
{
	Index index = Mesh.getCellIndex(CoordinatesType(i,j));
	Real value = cudaDofVector[index];
	Real a,b, tmp;

	if( i == 0 )
		a = cudaDofVector[Mesh.template getCellNextToCell<1,0>(index)];
	else if( i == Mesh.getDimensions().x() - 1 )
		a = cudaDofVector[Mesh.template getCellNextToCell<-1,0>(index)];
	else
	{
		a = fabsMin( cudaDofVector[Mesh.template getCellNextToCell<-1,0>(index)],
				 cudaDofVector[Mesh.template getCellNextToCell<1,0>(index)] );
	}

	if( j == 0 )
		b = cudaDofVector[Mesh.template getCellNextToCell<0,1>(index)];
	else if( j == Mesh.getDimensions().y() - 1 )
		b = cudaDofVector[Mesh.template getCellNextToCell<0,-1>(index)];
	else
	{
		b = fabsMin( cudaDofVector[Mesh.template getCellNextToCell<0,-1>(index)],
				 cudaDofVector[Mesh.template getCellNextToCell<0,1>(index)] );
	}


	if(abs(a-b) >= h)
		tmp = fabsMin(a,b) + Sign(value)*h;
	else
		tmp = 0.5 * (a + b + Sign(value)*sqrt(2.0 * h * h - (a - b) * (a - b) ) );

	cudaDofVector[index]  = fabsMin(value, tmp);

}









template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
__device__
void tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: updateValue( Index i, Index j, double** sharedMem, int k3)
{
	Index index = Mesh.getCellIndex(CoordinatesType(i,j));
	Real value = sharedMem[k3+1][threadIdx.y];
	Real a,b, tmp;

	if( i == 0 )
		a = sharedMem[k3][threadIdx.y];
	else if( i == Mesh.getDimensions().x() - 1 )
		a = sharedMem[k3+2][threadIdx.y];
	else
	{
		a = fabsMin( sharedMem[k3][threadIdx.y],
				sharedMem[k3+2][threadIdx.y] );
	}

	if( j == 0 )
		b = sharedMem[k3][threadIdx.y+1];
	else if( j == Mesh.getDimensions().y() - 1 )
		b = sharedMem[k3+2][threadIdx.y-1];
	else
	{
		b = fabsMin( sharedMem[k3][threadIdx.y+1],
				sharedMem[k3+2][threadIdx.y-1] );
	}


	if(abs(a-b) >= h)
		tmp = fabsMin(a,b) + Sign(value)*h;
	else
		tmp = 0.5 * (a + b + Sign(value)*sqrt(2.0 * h * h - (a - b) * (a - b) ) );

//	sharedMem[k3+1][threadIdx.y] = this->fabsMin(value, tmp);
//	atomicFabsMin(&(cudaDofVector[index]), tmp);
	cudaDofVector[index]  = tmp;
	this->fabsMin(value, tmp);

}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
__device__
bool tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: initGrid()
{
	int gx = threadIdx.x + blockDim.x*blockIdx.x;
	int gy = blockDim.y*blockIdx.y + threadIdx.y;
	int gid = Mesh.getCellIndex(CoordinatesType(gx,gy));

	int total = blockDim.x*gridDim.x;



	Real tmp = 0.0;
	int flag = 0;
	counter = 0;
	tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))]);


	if(!exactInput)
	{
		cudaDofVector[gid]=cudaDofVector[gid]=0.5*h*Sign(cudaDofVector[gid]);
	}
	__threadfence();
//	printf("-----------------------------------------------------------------------------------\n");

	__threadfence();

	if(gx > 0 && gx < Mesh.getDimensions().x()-1)
	{
		if(gy > 0 && gy < Mesh.getDimensions().y()-1)
		{

			Index j = gy;
			Index i = gx;
//			 tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))]);

			if(tmp == 0.0)
			{}
			else if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i+1,j))]*tmp < 0.0 ||
					cudaDofVector[Mesh.getCellIndex(CoordinatesType(i-1,j))]*tmp < 0.0 ||
					cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j+1))]*tmp < 0.0 ||
					cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j-1))]*tmp < 0.0 )
			{}
			else
				flag=1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))] = tmp*INT_MAX;
		}
	}

//	printf("gx: %d, gy: %d, gid: %d \n", gx, gy,gid);
//	printf("****************************************************************\n");
//	printf("gx: %d, gy: %d, gid: %d \n", gx, gy,gid);
	if(gx > 0 && gx < Mesh.getDimensions().x()-1 && gy == 0)
	{
//		printf("gx: %d, gy: %d, gid: %d \n", gx, gy,gid);
		Index j = 0;
		Index i = gx;
//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))]);


		if(tmp == 0.0)
		{}
		else if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i+1,j))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i-1,j))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j+1))]*tmp < 0.0 )
		{}
		else
			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))] = tmp*INT_MAX;
	}

//	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	if(gx > 0 && gx < Mesh.getDimensions().x()-1 && gy == Mesh.getDimensions().y() - 1)
	{
		Index i = gx;
		Index j = Mesh.getDimensions().y() - 1;
//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))]);


		if(tmp == 0.0)
		{}
		else if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i+1,j))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i-1,j))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j-1))]*tmp < 0.0 )
		{}
		else
			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))] = tmp*INT_MAX;
	}

//	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	if(gy > 0 && gy < Mesh.getDimensions().y()-1 && gx == 0)
	{
		Index j = gy;
		Index i = 0;
//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))]);


		if(tmp == 0.0)
		{}
		else if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i+1,j))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j+1))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j-1))]*tmp < 0.0 )
		{}
		else
			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))] = tmp*INT_MAX;
	}
//	printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
	if(gy > 0 && gy < Mesh.getDimensions().y()-1  && gx == Mesh.getDimensions().x() - 1)
	{
		Index j = gy;
		Index i = Mesh.getDimensions().x() - 1;
//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))]);


		if(tmp == 0.0)
		{}
		else if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(i-1,j))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j+1))]*tmp < 0.0 ||
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j-1))]*tmp < 0.0 )
		{}
		else
			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(i,j))] = tmp*INT_MAX;
	}

//	printf("##################################################################################################\n");
	if(gx == Mesh.getDimensions().x() - 1 &&
	   gy == Mesh.getDimensions().y() - 1)
	{

//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))]);
		if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx-1,gy))]*tmp > 0.0 &&
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy-1))]*tmp > 0.0)

			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))] = tmp*INT_MAX;
	}
	if(gx == Mesh.getDimensions().x() - 1 &&
	   gy == 0)
	{

//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))]);
		if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx-1,gy))]*tmp > 0.0 &&
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy+1))]*tmp > 0.0)

			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))] = tmp*INT_MAX;
	}
//	printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	if(gx == 0 &&
	   gy == Mesh.getDimensions().y() - 1)
	{

//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))]);
		if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx+1,gy))]*tmp > 0.0 &&
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy-1))]*tmp > 0.0)

			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))] = tmp*INT_MAX;
	}
	if(gx == 0 &&
	   gy == 0)
	{
//		tmp = Sign(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))]);
		if(cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx+1,gy))]*tmp > 0.0 &&
				cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy+1))]*tmp > 0.0)

			flag = 1;//cudaDofVector[Mesh.getCellIndex(CoordinatesType(gx,gy))] = tmp*INT_MAX;
	}

	__threadfence();

	if(flag==1)
		cudaDofVector[gid] =  tmp*3;
}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
__device__
Real tnlFastSweeping< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: fabsMin( Real x, Real y)
{
	Real fx = abs(x);
	//Real fy = abs(y);

	//Real tmpMin = Min(fx,abs(y));

	if(Min(fx,abs(y)) == fx)
		return x;
	else
		return y;


}



__global__ void runCUDA(tnlFastSweeping< tnlGrid< 2,double, tnlHost, int >, double, int >* solver, int sweep, int i)
{

	extern __shared__ double u[];
	double* sharedMem[5];
	sharedMem[0] = u;
	sharedMem[1] = &(u[blockDim.y+1]);
	sharedMem[2] = &(sharedMem[1][blockDim.y+1]);
	sharedMem[3] = sharedMem[1];
	sharedMem[4] = sharedMem[2];

	int gx = 0;
	int gy = threadIdx.y;
	//if(solver->Mesh.getDimensions().x() <= gx || solver->Mesh.getDimensions().y() <= gy)
	//	return;
	int n = solver->Mesh.getDimensions().x();
	int blockCount = n/blockDim.y +1;
	//int gid = solver->Mesh.getDimensions().x() * gy + gx;
	//int max = solver->Mesh.getDimensions().x()*solver->Mesh.getDimensions().x();

	//int id1 = gx+gy;
	//int id2 = (solver->Mesh.getDimensions().x() - gx - 1) + gy;


	if(blockIdx.x==0)
	{
		if(threadIdx.y==0)
			sharedMem[1][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(0,0))];

		for(int k = 0; k < n*blockCount + blockDim.y; k++)
		{
			if(threadIdx.y  < k+1 && gy < n)
			{
				int k3=k%3;

				if(threadIdx.y==0)
				{
					if(gx==n-1)
						sharedMem[k3][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(0,gy+blockDim.y))];
					else
						sharedMem[k3][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx+1,gy))];
				}
//				else
//					solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy-1))]=sharedMem[k3+2][threadIdx.y-1];

				if(gy<n-1)
					sharedMem[k3][threadIdx.y+1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy+1))];

				solver->updateValue(gx,gy,sharedMem,k3);
				gx++;
				if(gx==n)
				{
					gx=0;
					gy+=blockDim.y;
				}
			}


			__syncthreads();
		}
	}
//	else if(blockIdx.x==1)
//	{
//		gx=n-1;
//		gy=threadIdx.y;
//
//		if(threadIdx.y==0)
//					sharedMem[1][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(n-1,0))];
//
//		for(int k = 0; k < n*blockCount + blockDim.y; k++)
//		{
//			if(threadIdx.y  < k+1 && gy < n)
//			{
//				int k3=k%3;
//
//				if(threadIdx.y==0)
//					if(gx==0)
//						sharedMem[k3+2][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(n-1,gy+blockDim.y))];
//					else
//						sharedMem[k3+2][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx-1,gy))];
//				else
//					solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy-1))]=sharedMem[k3][threadIdx.y-1];
//
//				if(gy<n-1)
//					sharedMem[k3+2][threadIdx.y+1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy+1))];
//
//
//				solver->updateValue(gx,gy,sharedMem,k3);
//				gx--;
//				if(gx==-1)
//				{
//					gx=n-1;
//					gy+=blockDim.y;
//				}
//			}
//
//
//			__syncthreads();
//		}
//	}
//	else if(blockIdx.x==2)
//	{
//		gx=0;
//		gy=n-blockDim.y-1+threadIdx.y;
//
//		if(threadIdx.y==0)
//					sharedMem[1][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(0,n-1))];
//
//		for(int k = 0; k < n*blockCount + blockDim.y; k++)
//		{
//			if(blockDim.y-threadIdx.y  < k+1 && gy > -1)
//			{
//				int k3=k%3;
//
//				if(threadIdx.y==blockDim.y-1)
//					if(gx==n-1)
//						sharedMem[k3][n-1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(0,gy-blockDim.y))];
//					else
//						sharedMem[k3][n-1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx+1,gy))];
//				else
//					solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy-1))]=sharedMem[k3+2][threadIdx.y-1];
//
//				if(gy<n-1)
//					sharedMem[k3][threadIdx.y+1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy+1))];
//
//
//				solver->updateValue(gx,gy,sharedMem,k3);
//				gx++;
//				if(gx==n)
//				{
//					gx=0;
//					gy-=blockDim.y;
//				}
//			}
//
//
//			__syncthreads();
//		}
//	}
//	else if(blockIdx.x==3)
//	{
//		gx=n-1;
//		gy=n-blockDim.y-1+threadIdx.y;
//
//		if(threadIdx.y==0)
//					sharedMem[1][0]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(n-1,n-1))];
//
//
//		for(int k = 0; k < n*blockCount + blockDim.y; k++)
//		{
//			if(blockDim.y-threadIdx.y  < k+1 && gy > -1)
//			{
//				int k3=k%3;
//
//				if(threadIdx.y==blockDim.y-1)
//					if(gx==n-1)
//						sharedMem[k3+2][n-1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(n-1,gy-blockDim.y))];
//					else
//						sharedMem[k3+2][n-1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx+1,gy))];
//				else
//					solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy-1))]=sharedMem[k3][threadIdx.y-1];
//
//				if(gy<n-1)
//					sharedMem[k3+2][threadIdx.y+1]=solver->cudaDofVector[solver->Mesh.getCellIndex(tnlStaticVector<2,int>(gx,gy+1))];
//
//
//				solver->updateValue(gx,gy,sharedMem,k3);
//				gx--;
//				if(gx==-1)
//				{
//					gx=n-1;
//					gy-=blockDim.y;
//				}
//			}
//
//
//			__syncthreads();
//		}
//	}




}


__global__ void initCUDA(tnlFastSweeping< tnlGrid< 2,double, tnlHost, int >, double, int >* solver)
{
	int gx = threadIdx.x + blockDim.x*blockIdx.x;
	int gy = blockDim.y*blockIdx.y + threadIdx.y;

	if(solver->Mesh.getDimensions().x() > gx && solver->Mesh.getDimensions().y() > gy)
	{
		solver->initGrid();
	}


}
#endif




#endif /* TNLFASTSWEEPING_IMPL_H_ */
