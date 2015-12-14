/***************************************************************************
                          tnlParallelEikonalSolver2D_impl.h  -  description
                             -------------------
    begin                : Nov 28 , 2014
    copyright            : (C) 2014 by Tomas Sobotik
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef TNLPARALLELEIKONALSOLVER3D_IMPL_H_
#define TNLPARALLELEIKONALSOLVER3D_IMPL_H_


#include "tnlParallelEikonalSolver.h"
#include <core/mfilename.h>

template< typename SchemeHost, typename SchemeDevice, typename Device>
tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::tnlParallelEikonalSolver()
{
	cout << "a" << endl;
	this->device = tnlCudaDevice;  /////////////// tnlCuda Device --- vypocet na GPU, tnlHostDevice   ---    vypocet na CPU

#ifdef HAVE_CUDA
	if(this->device == tnlCudaDevice)
	{
	run_host = 1;
	}
#endif

	cout << "b" << endl;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::test()
{
/*
	for(int i =0; i < this->subgridValues.getSize(); i++ )
	{
		insertSubgrid(getSubgrid(i), i);
	}
*/
}

template< typename SchemeHost, typename SchemeDevice, typename Device>

bool tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::init( const tnlParameterContainer& parameters )
{
	cout << "Initializating solver..." << endl;
	const tnlString& meshLocation = parameters.getParameter <tnlString>("mesh");
	this->mesh.load( meshLocation );

	this->n = parameters.getParameter <int>("subgrid-size");
	cout << "Setting N to " << this->n << endl;

	this->subMesh.setDimensions( this->n, this->n );
	this->subMesh.setDomain( tnlStaticVector<3,double>(0.0, 0.0),
							 tnlStaticVector<3,double>(this->mesh.getHx()*(double)(this->n), this->mesh.getHy()*(double)(this->n),this->mesh.getHz()*(double)(this->n)) );

	this->subMesh.save("submesh.tnl");

	const tnlString& initialCondition = parameters.getParameter <tnlString>("initial-condition");
	this->u0.load( initialCondition );

	//cout << this->mesh.getCellCenter(0) << endl;

	this->delta = parameters.getParameter <double>("delta");
	this->delta *= this->mesh.getHx()*this->mesh.getHy();

	cout << "Setting delta to " << this->delta << endl;

	this->tau0 = parameters.getParameter <double>("initial-tau");
	cout << "Setting initial tau to " << this->tau0 << endl;
	this->stopTime = parameters.getParameter <double>("stop-time");

	this->cflCondition = parameters.getParameter <double>("cfl-condition");
	this -> cflCondition *= sqrt(this->mesh.getHx()*this->mesh.getHy());
	cout << "Setting CFL to " << this->cflCondition << endl;

	stretchGrid();
	this->stopTime /= (double)(this->gridCols);
	this->stopTime *= (1.0+1.0/((double)(this->n) - 2.0));
	cout << "Setting stopping time to " << this->stopTime << endl;
	//this->stopTime = 1.5*((double)(this->n))*parameters.getParameter <double>("stop-time")*this->mesh.getHx();
	//cout << "Setting stopping time to " << this->stopTime << endl;

	cout << "Initializating scheme..." << endl;
	if(!this->schemeHost.init(parameters))
	{
		cerr << "SchemeHost failed to initialize." << endl;
		return false;
	}
	cout << "Scheme initialized." << endl;

	test();

	VectorType* tmp = new VectorType[subgridValues.getSize()];
	bool containsCurve = false;

#ifdef HAVE_CUDA

	if(this->device == tnlCudaDevice)
	{
	/*cout << "Testing... " << endl;
	if(this->device == tnlCudaDevice)
	{
	if( !initCUDA3D(parameters, gridRows, gridCols) )
		return false;
	}*/
		//cout << "s" << endl;
	cudaMalloc(&(this->cudaSolver), sizeof(tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >));
	//cout << "s" << endl;
	cudaMemcpy(this->cudaSolver, this,sizeof(tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >), cudaMemcpyHostToDevice);
	//cout << "s" << endl;
	double** tmpdev = NULL;
	cudaMalloc(&tmpdev, sizeof(double*));
	//double* tmpw;
	cudaMalloc(&(this->tmpw), this->work_u.getSize()*sizeof(double));
	cudaMalloc(&(this->runcuda), sizeof(int));
	cudaDeviceSynchronize();
	checkCudaDevice;
	int* tmpUC;
	cudaMalloc(&(tmpUC), this->work_u.getSize()*sizeof(int));
	cudaMemcpy(tmpUC, this->unusedCell.getData(), this->unusedCell.getSize()*sizeof(int), cudaMemcpyHostToDevice);

	initCUDA3D<SchemeTypeHost, SchemeTypeDevice, DeviceType><<<1,1>>>(this->cudaSolver, (this->tmpw), (this->runcuda),tmpUC);
	cudaDeviceSynchronize();
	checkCudaDevice;
	//cout << "s " << endl;
	//cudaMalloc(&(cudaSolver->work_u_cuda), this->work_u.getSize()*sizeof(double));
	double* tmpu = NULL;

	cudaMemcpy(&tmpu, tmpdev,sizeof(double*), cudaMemcpyDeviceToHost);
	//printf("%p %p \n",tmpu,tmpw);
	cudaMemcpy((this->tmpw), this->work_u.getData(), this->work_u.getSize()*sizeof(double), cudaMemcpyHostToDevice);
	cudaDeviceSynchronize();
	checkCudaDevice;
	//cout << "s "<< endl;

	}
#endif

	if(this->device == tnlHostDevice)
	{
	for(int i = 0; i < this->subgridValues.getSize(); i++)
	{

		if(! tmp[i].setSize(this->n * this->n))
			cout << "Could not allocate tmp["<< i <<"] array." << endl;
			tmp[i] = getSubgrid(i);
		containsCurve = false;

		for(int j = 0; j < tmp[i].getSize(); j++)
		{
			if(tmp[i][0]*tmp[i][j] <= 0.0)
			{
				containsCurve = true;
				j=tmp[i].getSize();
			}

		}
		if(containsCurve)
		{
			//cout << "Computing initial SDF on subgrid " << i << "." << endl;
			insertSubgrid(runSubgrid(0, tmp[i],i), i);
			setSubgridValue(i, 4);
			//cout << "Computed initial SDF on subgrid " << i  << "." << endl;
		}
		containsCurve = false;

	}
	}
#ifdef HAVE_CUDA
	else if(this->device == tnlCudaDevice)
	{
//		cout << "pre 1 kernel" << endl;
		cudaDeviceSynchronize();
		checkCudaDevice;
		dim3 threadsPerBlock(this->n, this->n);
		dim3 numBlocks(this->gridCols,this->gridRows);
		cudaDeviceSynchronize();
		checkCudaDevice;
		initRunCUDA3D<SchemeTypeHost,SchemeTypeDevice, DeviceType><<<numBlocks,threadsPerBlock,3*this->n*this->n*sizeof(double)>>>(this->cudaSolver);
		cudaDeviceSynchronize();
//		cout << "post 1 kernel" << endl;

	}
#endif


	this->currentStep = 1;
	if(this->device == tnlHostDevice)
		synchronize();
#ifdef HAVE_CUDA
	else if(this->device == tnlCudaDevice)
	{
		dim3 threadsPerBlock(this->n, this->n);
		dim3 numBlocks(this->gridCols,this->gridRows);
		//double * test = (double*)malloc(this->work_u.getSize()*sizeof(double));
		//cout << test[0] <<"   " << test[1] <<"   " << test[2] <<"   " << test[3] << endl;
		//cudaMemcpy(/*this->work_u.getData()*/ test, (this->tmpw), this->work_u.getSize()*sizeof(double), cudaMemcpyDeviceToHost);
		//cout << this->tmpw << "   " <<  test[0] <<"   " << test[1] << "   " <<test[2] << "   " <<test[3] << endl;

		checkCudaDevice;

		synchronizeCUDA3D<SchemeTypeHost, SchemeTypeDevice, DeviceType><<<numBlocks,threadsPerBlock>>>(this->cudaSolver);
		cudaDeviceSynchronize();
		checkCudaDevice;
		synchronize2CUDA3D<SchemeTypeHost, SchemeTypeDevice, DeviceType><<<numBlocks,1>>>(this->cudaSolver);
		cudaDeviceSynchronize();
		checkCudaDevice;
		//cout << test[0] << "   " <<test[1] <<"   " << test[2] << "   " <<test[3] << endl;
		//cudaMemcpy(/*this->work_u.getData()*/ test, (this->tmpw), this->work_u.getSize()*sizeof(double), cudaMemcpyDeviceToHost);
		//checkCudaDevice;
		//cout << this->tmpw << "   " <<  test[0] << "   " <<test[1] << "   " <<test[2] <<"   " << test[3] << endl;
		//free(test);

	}

#endif
	cout << "Solver initialized." << endl;

	return true;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::run()
{
	if(this->device == tnlHostDevice)
	{

	bool end = false;
	while ((this->boundaryConditions.max() > 0 ) || !end)
	{
		if(this->boundaryConditions.max() == 0 )
			end=true;
		else
			end=false;
#ifdef HAVE_OPENMP
#pragma omp parallel for num_threads(4) schedule(dynamic)
#endif
		for(int i = 0; i < this->subgridValues.getSize(); i++)
		{
			if(getSubgridValue(i) != INT_MAX)
			{
				//cout << "subMesh: " << i << ", BC: " << getBoundaryCondition(i) << endl;

				if(getSubgridValue(i) == currentStep+4)
				{

				if(getBoundaryCondition(i) & 1)
				{
					insertSubgrid( runSubgrid(1, getSubgrid(i),i), i);
					this->calculationsCount[i]++;
				}
				if(getBoundaryCondition(i) & 2)
				{
					insertSubgrid( runSubgrid(2, getSubgrid(i),i), i);
					this->calculationsCount[i]++;
				}
				if(getBoundaryCondition(i) & 4)
				{
					insertSubgrid( runSubgrid(4, getSubgrid(i),i), i);
					this->calculationsCount[i]++;
				}
				if(getBoundaryCondition(i) & 8)
				{
					insertSubgrid( runSubgrid(8, getSubgrid(i),i), i);
					this->calculationsCount[i]++;
				}
				}

				if( ((getBoundaryCondition(i) & 2) )|| (getBoundaryCondition(i) & 1)//)
					/*	&&(!(getBoundaryCondition(i) & 5) && !(getBoundaryCondition(i) & 10)) */)
				{
					//cout << "3 @ " << getBoundaryCondition(i) << endl;
					insertSubgrid( runSubgrid(3, getSubgrid(i),i), i);
				}
				if( ((getBoundaryCondition(i) & 4) )|| (getBoundaryCondition(i) & 1)//)
					/*	&&(!(getBoundaryCondition(i) & 3) && !(getBoundaryCondition(i) & 12)) */)
				{
					//cout << "5 @ " << getBoundaryCondition(i) << endl;
					insertSubgrid( runSubgrid(5, getSubgrid(i),i), i);
				}
				if( ((getBoundaryCondition(i) & 2) )|| (getBoundaryCondition(i) & 8)//)
					/*	&&(!(getBoundaryCondition(i) & 12) && !(getBoundaryCondition(i) & 3))*/ )
				{
					//cout << "10 @ " << getBoundaryCondition(i) << endl;
					insertSubgrid( runSubgrid(10, getSubgrid(i),i), i);
				}
				if(   ((getBoundaryCondition(i) & 4) )|| (getBoundaryCondition(i) & 8)//)
					/*&&(!(getBoundaryCondition(i) & 10) && !(getBoundaryCondition(i) & 5)) */)
				{
					//cout << "12 @ " << getBoundaryCondition(i) << endl;
					insertSubgrid( runSubgrid(12, getSubgrid(i),i), i);
				}


				/*if(getBoundaryCondition(i))
				{
					insertSubgrid( runSubgrid(15, getSubgrid(i),i), i);
				}*/

				setBoundaryCondition(i, 0);

				setSubgridValue(i, getSubgridValue(i)-1);

			}
		}
		synchronize();
	}
	}
#ifdef HAVE_CUDA
	else if(this->device == tnlCudaDevice)
	{
		//cout << "fn" << endl;
		bool end_cuda = false;
		dim3 threadsPerBlock(this->n, this->n);
		dim3 numBlocks(this->gridCols,this->gridRows);
		cudaDeviceSynchronize();
		checkCudaDevice;
		//cudaMalloc(&runcuda,sizeof(bool));
		//cudaMemcpy(runcuda, &run_host, sizeof(bool), cudaMemcpyHostToDevice);
		//cout << "fn" << endl;
		bool* tmpb;
		//cudaMemcpy(tmpb, &(cudaSolver->runcuda),sizeof(bool*), cudaMemcpyDeviceToHost);
		//cudaDeviceSynchronize();
		//checkCudaDevice;
		cudaMemcpy(&(this->run_host),this->runcuda,sizeof(int), cudaMemcpyDeviceToHost);
		cudaDeviceSynchronize();
		checkCudaDevice;
		//cout << "fn" << endl;
		int i = 1;
		time_diff = 0.0;
		while (run_host || !end_cuda)
		{
			cout << "Computing at step "<< i++ << endl;
			if(run_host != 0 )
				end_cuda = true;
			else
				end_cuda = false;
			//cout << "a" << endl;
			cudaDeviceSynchronize();
			checkCudaDevice;
			start = std::clock();
			runCUDA3D<SchemeTypeHost, SchemeTypeDevice, DeviceType><<<numBlocks,threadsPerBlock,3*this->n*this->n*sizeof(double)>>>(this->cudaSolver);
			//cout << "a" << endl;
			cudaDeviceSynchronize();
			time_diff += (std::clock() - start) / (double)(CLOCKS_PER_SEC);

			//start = std::clock();
			synchronizeCUDA3D<SchemeTypeHost, SchemeTypeDevice, DeviceType><<<numBlocks,threadsPerBlock>>>(this->cudaSolver);
			cudaDeviceSynchronize();
			checkCudaDevice;
			synchronize2CUDA3D<SchemeTypeHost, SchemeTypeDevice, DeviceType><<<numBlocks,1>>>(this->cudaSolver);
			cudaDeviceSynchronize();
			checkCudaDevice;
			//time_diff += (std::clock() - start) / (double)(CLOCKS_PER_SEC);


			//cout << "a" << endl;
			//run_host = false;
			//cout << "in kernel loop" << run_host << endl;
			//cudaMemcpy(tmpb, &(cudaSolver->runcuda),sizeof(bool*), cudaMemcpyDeviceToHost);
			cudaMemcpy(&run_host, (this->runcuda),sizeof(int), cudaMemcpyDeviceToHost);
			//cout << "in kernel loop" << run_host << endl;
		}
		cout << "Solving time was: " << time_diff << endl;
		//cout << "b" << endl;

		//double* tmpu;
		//cudaMemcpy(tmpu, &(cudaSolver->work_u_cuda),sizeof(double*), cudaMemcpyHostToDevice);
		//cudaMemcpy(this->work_u.getData(), tmpu, this->work_u.getSize()*sizeof(double), cudaMemcpyDeviceToHost);
		//cout << this->work_u.getData()[0] << endl;

		//double * test = (double*)malloc(this->work_u.getSize()*sizeof(double));
		//cout << test[0] << test[1] << test[2] << test[3] << endl;
		cudaMemcpy(this->work_u.getData()/* test*/, (this->tmpw), this->work_u.getSize()*sizeof(double), cudaMemcpyDeviceToHost);
		//cout << this->tmpw << "   " <<  test[0] << test[1] << test[2] << test[3] << endl;
		//free(test);

		cudaDeviceSynchronize();
	}
#endif
	contractGrid();
	this->u0.save("u-00001.tnl");
	cout << "Maximum number of calculations on one subgrid was " << this->calculationsCount.absMax() << endl;
	cout << "Average number of calculations on one subgrid was " << ( (double) this->calculationsCount.sum() / (double) this->calculationsCount.getSize() ) << endl;
	cout << "Solver finished" << endl;

#ifdef HAVE_CUDA
	if(this->device == tnlCudaDevice)
	{
		cudaFree(this->runcuda);
		cudaFree(this->tmpw);
		cudaFree(this->cudaSolver);
	}
#endif

}

//north - 1, east - 2, west - 4, south - 8
template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::synchronize() //needs fix ---- maybe not anymore --- but frankly: yeah, it does -- aaaa-and maybe fixed now
{
	cout << "Synchronizig..." << endl;
	int tmp1, tmp2;
	int grid1, grid2;

	if(this->currentStep & 1)
	{
		for(int j = 0; j < this->gridRows - 1; j++)
		{
			for (int i = 0; i < this->gridCols*this->n; i++)
			{
				tmp1 = this->gridCols*this->n*((this->n-1)+j*this->n) + i;
				tmp2 = this->gridCols*this->n*((this->n)+j*this->n) + i;
				grid1 = getSubgridValue(getOwner(tmp1));
				grid2 = getSubgridValue(getOwner(tmp2));
				if(getOwner(tmp1)==getOwner(tmp2))
					cout << "i, j" << i << "," << j << endl;
				if ((fabs(this->work_u[tmp1]) < fabs(this->work_u[tmp2]) - this->delta || grid2 == INT_MAX || grid2 == -INT_MAX) && (grid1 != INT_MAX && grid1 != -INT_MAX))
				{
					this->work_u[tmp2] = this->work_u[tmp1];
					this->unusedCell[tmp2] = 0;
					if(grid2 == INT_MAX)
					{
						setSubgridValue(getOwner(tmp2), -INT_MAX);
					}
					if(! (getBoundaryCondition(getOwner(tmp2)) & 8) )
						setBoundaryCondition(getOwner(tmp2), getBoundaryCondition(getOwner(tmp2))+8);
				}
				else if ((fabs(this->work_u[tmp1]) > fabs(this->work_u[tmp2]) + this->delta || grid1 == INT_MAX || grid1 == -INT_MAX) && (grid2 != INT_MAX && grid2 != -INT_MAX))
				{
					this->work_u[tmp1] = this->work_u[tmp2];
					this->unusedCell[tmp1] = 0;
					if(grid1 == INT_MAX)
					{
						setSubgridValue(getOwner(tmp1), -INT_MAX);
					}
					if(! (getBoundaryCondition(getOwner(tmp1)) & 1) )
						setBoundaryCondition(getOwner(tmp1), getBoundaryCondition(getOwner(tmp1))+1);
				}
			}
		}

	}
	else
	{
		for(int i = 1; i < this->gridCols; i++)
		{
			for (int j = 0; j < this->gridRows*this->n; j++)
			{
				tmp1 = this->gridCols*this->n*j + i*this->n - 1;
				tmp2 = this->gridCols*this->n*j + i*this->n ;
				grid1 = getSubgridValue(getOwner(tmp1));
				grid2 = getSubgridValue(getOwner(tmp2));
				if(getOwner(tmp1)==getOwner(tmp2))
					cout << "i, j" << i << "," << j << endl;
				if ((fabs(this->work_u[tmp1]) < fabs(this->work_u[tmp2]) - this->delta || grid2 == INT_MAX || grid2 == -INT_MAX) && (grid1 != INT_MAX && grid1 != -INT_MAX))
				{
					this->work_u[tmp2] = this->work_u[tmp1];
					this->unusedCell[tmp2] = 0;
					if(grid2 == INT_MAX)
					{
						setSubgridValue(getOwner(tmp2), -INT_MAX);
					}
					if(! (getBoundaryCondition(getOwner(tmp2)) & 4) )
						setBoundaryCondition(getOwner(tmp2), getBoundaryCondition(getOwner(tmp2))+4);
				}
				else if ((fabs(this->work_u[tmp1]) > fabs(this->work_u[tmp2]) + this->delta || grid1 == INT_MAX || grid1 == -INT_MAX) && (grid2 != INT_MAX && grid2 != -INT_MAX))
				{
					this->work_u[tmp1] = this->work_u[tmp2];
					this->unusedCell[tmp1] = 0;
					if(grid1 == INT_MAX)
					{
						setSubgridValue(getOwner(tmp1), -INT_MAX);
					}
					if(! (getBoundaryCondition(getOwner(tmp1)) & 2) )
						setBoundaryCondition(getOwner(tmp1), getBoundaryCondition(getOwner(tmp1))+2);
				}
			}
		}
	}


	this->currentStep++;
	int stepValue = this->currentStep + 4;
	for (int i = 0; i < this->subgridValues.getSize(); i++)
	{
		if( getSubgridValue(i) == -INT_MAX )
			setSubgridValue(i, stepValue);
	}

	cout << "Grid synchronized at step " << (this->currentStep - 1 ) << endl;

}


template< typename SchemeHost, typename SchemeDevice, typename Device>
int tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getOwner(int i) const
{

	return (i / (this->gridCols*this->n*this->n))*this->gridCols + (i % (this->gridCols*this->n))/this->n;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
int tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getSubgridValue( int i ) const
{
	return this->subgridValues[i];
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::setSubgridValue(int i, int value)
{
	this->subgridValues[i] = value;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
int tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getBoundaryCondition( int i ) const
{
	return this->boundaryConditions[i];
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::setBoundaryCondition(int i, int value)
{
	this->boundaryConditions[i] = value;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::stretchGrid()
{
	cout << "Stretching grid..." << endl;


	this->gridCols = ceil( ((double)(this->mesh.getDimensions().x()-1)) / ((double)(this->n-1)) );
	this->gridRows = ceil( ((double)(this->mesh.getDimensions().y()-1)) / ((double)(this->n-1)) );
	this->gridLevels = ceil( ((double)(this->mesh.getDimensions().z()-1)) / ((double)(this->n-1)) );

	//this->gridCols = (this->mesh.getDimensions().x()-1) / (this->n-1) ;
	//this->gridRows = (this->mesh.getDimensions().y()-1) / (this->n-1) ;

	cout << "Setting gridCols to " << this->gridCols << "." << endl;
	cout << "Setting gridRows to " << this->gridRows << "." << endl;
	cout << "Setting gridLevels to " << this->gridLevels << "." << endl;

	this->subgridValues.setSize(this->gridCols*this->gridRows*this->gridLevels);
	this->subgridValues.setValue(0);
	this->boundaryConditions.setSize(this->gridCols*this->gridRows*this->gridLevels);
	this->boundaryConditions.setValue(0);
	this->calculationsCount.setSize(this->gridCols*this->gridRows*this->gridLevels);
	this->calculationsCount.setValue(0);

	for(int i = 0; i < this->subgridValues.getSize(); i++ )
	{
		this->subgridValues[i] = INT_MAX;
		this->boundaryConditions[i] = 0;
	}

	int levelSize = this->n*this->n*this->gridCols*this->gridRows;
	int stretchedSize = this->n*levelSize*this->gridLevels;

	if(!this->work_u.setSize(stretchedSize))
		cerr << "Could not allocate memory for stretched grid." << endl;
	if(!this->unusedCell.setSize(stretchedSize))
		cerr << "Could not allocate memory for supporting stretched grid." << endl;
	int idealStretch =this->mesh.getDimensions().x() + (this->mesh.getDimensions().x()-2)/(this->n-1);
	cout << idealStretch << endl;




	for(int i = 0; i < levelSize; i++)
	{
		this->unusedCell[i] = 1;
		int diff =(this->n*this->gridCols) - idealStretch ;

		int k = i/this->n - i/(this->n*this->gridCols) + this->mesh.getDimensions().x()*(i/(this->n*this->n*this->gridCols)) + (i/(this->n*this->gridCols))*diff;

		if(i%(this->n*this->gridCols) - idealStretch  >= 0)
		{
			k+= i%(this->n*this->gridCols) - idealStretch +1 ;
		}

		if(i/(this->n*this->gridCols) - idealStretch + 1  > 0)
		{
			k+= (i/(this->n*this->gridCols) - idealStretch +1 )* this->mesh.getDimensions().x() ;
		}


		for( int j = 0; j<this->n*this->gridLevels; j++)
		{

			int l = j/this->n;

			if(j%(this->n*this->gridLevels) - idealStretch  >= 0)
			{
				l+= j%(this->n*this->gridLevels) - idealStretch + 1;
			}

			this->work_u[i+(j-l)*levelSize] = this->u0[i+(j-l)*levelSize-k];
		}

	}



	cout << "Grid stretched." << endl;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::contractGrid()
{
	cout << "Contracting grid..." << endl;
	int levelSize = this->n*this->n*this->gridCols*this->gridRows;
	int stretchedSize = this->n*levelSize*this->gridLevels;

	int idealStretch =this->mesh.getDimensions().x() + (this->mesh.getDimensions().x()-2)/(this->n-1);
	cout << idealStretch << endl;


	for(int i = 0; i < levelSize; i++)
	{
		int diff =(this->n*this->gridCols) - idealStretch ;
		int k = i/this->n - i/(this->n*this->gridCols) + this->mesh.getDimensions().x()*(i/(this->n*this->n*this->gridCols)) + (i/(this->n*this->gridCols))*diff;

		if((i%(this->n*this->gridCols) - idealStretch  < 0) && (i/(this->n*this->gridCols) - idealStretch + 1  <= 0) )
		{
			for( int j = 0; j<this->n*this->gridLevels; j++)
			{
				int l = j/this->n;

				this->u0[i+(j-l)*levelSize-k] = this->work_u[i+(j-l)*levelSize];
			}
		}

	}

	cout << "Grid contracted" << endl;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
typename tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::VectorType
tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getSubgrid( const int i ) const
{
	VectorType u;
	u.setSize(this->n*this->n);

	for( int j = 0; j < u.getSize(); j++)
	{
		u[j] = this->work_u[ (i / this->gridCols) * this->n*this->n*this->gridCols
		                     + (i % this->gridCols) * this->n
		                     + (j/this->n) * this->n*this->gridCols
		                     + (j % this->n) ];
	}
	return u;
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::insertSubgrid( VectorType u, const int i )
{

	for( int j = 0; j < this->n*this->n; j++)
	{
		int index = (i / this->gridCols)*this->n*this->n*this->gridCols + (i % this->gridCols)*this->n + (j/this->n)*this->n*this->gridCols + (j % this->n);
		//OMP LOCK index
		if( (fabs(this->work_u[index]) > fabs(u[j])) || (this->unusedCell[index] == 1) )
		{
			this->work_u[index] = u[j];
			this->unusedCell[index] = 0;
		}
		//OMP UNLOCK index
	}
}

template< typename SchemeHost, typename SchemeDevice, typename Device>
typename tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::VectorType
tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::runSubgrid( int boundaryCondition, VectorType u, int subGridID)
{

	VectorType fu;

	fu.setLike(u);
	fu.setValue( 0.0 );

/*
 *          Insert Euler-Solver Here
 */

	/**/

	/*for(int i = 0; i < u.getSize(); i++)
	{
		int x = this->subMesh.getCellCoordinates(i).x();
		int y = this->subMesh.getCellCoordinates(i).y();

		if(x == 0 && (boundaryCondition & 4) && y ==0)
		{
			if((u[subMesh.getCellYSuccessor( i )] - u[i])/subMesh.getHy() > 1.0)
			{
				//cout << "x = 0; y = 0" << endl;
				u[i] = u[subMesh.getCellYSuccessor( i )] - subMesh.getHy();
			}
		}
		else if(x == 0 && (boundaryCondition & 4) && y == subMesh.getDimensions().y() - 1)
		{
			if((u[subMesh.getCellYPredecessor( i )] - u[i])/subMesh.getHy() > 1.0)
			{
				//cout << "x = 0; y = n" << endl;
				u[i] = u[subMesh.getCellYPredecessor( i )] - subMesh.getHy();
			}
		}


		else if(x == subMesh.getDimensions().x() - 1 && (boundaryCondition & 2) && y ==0)
		{
			if((u[subMesh.getCellYSuccessor( i )] - u[i])/subMesh.getHy() > 1.0)
			{
				//cout << "x = n; y = 0" << endl;
				u[i] = u[subMesh.getCellYSuccessor( i )] - subMesh.getHy();
			}
		}
		else if(x == subMesh.getDimensions().x() - 1 && (boundaryCondition & 2) && y == subMesh.getDimensions().y() - 1)
		{
			if((u[subMesh.getCellYPredecessor( i )] - u[i])/subMesh.getHy() > 1.0)
			{
				//cout << "x = n; y = n" << endl;
				u[i] = u[subMesh.getCellYPredecessor( i )] - subMesh.getHy();
			}
		}


		else if(y == 0 && (boundaryCondition & 8) && x ==0)
		{
			if((u[subMesh.getCellXSuccessor( i )] - u[i])/subMesh.getHx() > 1.0)
			{
				//cout << "y = 0; x = 0" << endl;
				u[i] = u[subMesh.getCellXSuccessor( i )] - subMesh.getHx();
			}
		}
		else if(y == 0 && (boundaryCondition & 8) && x == subMesh.getDimensions().x() - 1)
		{
			if((u[subMesh.getCellXPredecessor( i )] - u[i])/subMesh.getHx() > 1.0)
			{
				//cout << "y = 0; x = n" << endl;
				u[i] = u[subMesh.getCellXPredecessor( i )] - subMesh.getHx();
			}
		}


		else if(y == subMesh.getDimensions().y() - 1 && (boundaryCondition & 1) && x ==0)
		{
			if((u[subMesh.getCellXSuccessor( i )] - u[i])/subMesh.getHx() > 1.0)			{
				//cout << "y = n; x = 0" << endl;
				u[i] = u[subMesh.getCellXSuccessor( i )] - subMesh.getHx();
			}
		}
		else if(y == subMesh.getDimensions().y() - 1 && (boundaryCondition & 1) && x == subMesh.getDimensions().x() - 1)
		{
			if((u[subMesh.getCellXPredecessor( i )] - u[i])/subMesh.getHx() > 1.0)
			{
				//cout << "y = n; x = n" << endl;
				u[i] = u[subMesh.getCellXPredecessor( i )] - subMesh.getHx();
			}
		}
	}*/

	/**/


/*	bool tmp = false;
	for(int i = 0; i < u.getSize(); i++)
	{
		if(u[0]*u[i] <= 0.0)
			tmp=true;
	}


	if(tmp)
	{}
	else if(boundaryCondition == 4)
	{
		int i;
		for(i = 0; i < u.getSize() - subMesh.getDimensions().x() ; i=subMesh.getCellYSuccessor(i))
		{
			int j;
			for(j = i; j < subMesh.getDimensions().x() - 1; j=subMesh.getCellXSuccessor(j))
			{
				u[j] = u[i];
			}
			u[j] = u[i];
		}
		int j;
		for(j = i; j < subMesh.getDimensions().x() - 1; j=subMesh.getCellXSuccessor(j))
		{
			u[j] = u[i];
		}
		u[j] = u[i];
	}
	else if(boundaryCondition == 8)
	{
		int i;
		for(i = 0; i < subMesh.getDimensions().x() - 1; i=subMesh.getCellXSuccessor(i))
		{
			int j;
			for(j = i; j < u.getSize() - subMesh.getDimensions().x(); j=subMesh.getCellYSuccessor(j))
			{
				u[j] = u[i];
			}
			u[j] = u[i];
		}
		int j;
		for(j = i; j < u.getSize() - subMesh.getDimensions().x(); j=subMesh.getCellYSuccessor(j))
		{
			u[j] = u[i];
		}
		u[j] = u[i];

	}
	else if(boundaryCondition == 2)
	{
		int i;
		for(i = subMesh.getDimensions().x() - 1; i < u.getSize() - subMesh.getDimensions().x() ; i=subMesh.getCellYSuccessor(i))
		{
			int j;
			for(j = i; j > (i-1)*subMesh.getDimensions().x(); j=subMesh.getCellXPredecessor(j))
			{
				u[j] = u[i];
			}
			u[j] = u[i];
		}
		int j;
		for(j = i; j > (i-1)*subMesh.getDimensions().x(); j=subMesh.getCellXPredecessor(j))
		{
			u[j] = u[i];
		}
		u[j] = u[i];
	}
	else if(boundaryCondition == 1)
	{
		int i;
		for(i = (subMesh.getDimensions().y() - 1)*subMesh.getDimensions().x(); i < u.getSize() - 1; i=subMesh.getCellXSuccessor(i))
		{
			int j;
			for(j = i; j >=subMesh.getDimensions().x(); j=subMesh.getCellYPredecessor(j))
			{
				u[j] = u[i];
			}
			u[j] = u[i];
		}
		int j;
		for(j = i; j >=subMesh.getDimensions().x(); j=subMesh.getCellYPredecessor(j))
		{
			u[j] = u[i];
		}
		u[j] = u[i];
	}
*/
	/**/



	bool tmp = false;
	for(int i = 0; i < u.getSize(); i++)
	{
		if(u[0]*u[i] <= 0.0)
			tmp=true;
		int centreGID = (this->n*(subGridID / this->gridRows)+ (this->n >> 1))*(this->n*this->gridCols) + this->n*(subGridID % this->gridRows) + (this->n >> 1);
		if(this->unusedCell[centreGID] == 0 || boundaryCondition == 0)
			tmp = true;
	}
	//if(this->currentStep + 3 < getSubgridValue(subGridID))
		//tmp = true;


	double value = Sign(u[0]) * u.absMax();

	if(tmp)
	{}


	//north - 1, east - 2, west - 4, south - 8
	else if(boundaryCondition == 4)
	{
		for(int i = 0; i < this->n; i++)
			for(int j = 1;j < this->n; j++)
				//if(fabs(u[i*this->n + j]) <  fabs(u[i*this->n]))
				u[i*this->n + j] = value;// u[i*this->n];
	}
	else if(boundaryCondition == 2)
	{
		for(int i = 0; i < this->n; i++)
			for(int j =0 ;j < this->n -1; j++)
				//if(fabs(u[i*this->n + j]) < fabs(u[(i+1)*this->n - 1]))
				u[i*this->n + j] = value;// u[(i+1)*this->n - 1];
	}
	else if(boundaryCondition == 1)
	{
		for(int j = 0; j < this->n; j++)
			for(int i = 0;i < this->n - 1; i++)
				//if(fabs(u[i*this->n + j]) < fabs(u[j + this->n*(this->n - 1)]))
				u[i*this->n + j] = value;// u[j + this->n*(this->n - 1)];
	}
	else if(boundaryCondition == 8)
	{
		for(int j = 0; j < this->n; j++)
			for(int i = 1;i < this->n; i++)
				//if(fabs(u[i*this->n + j]) < fabs(u[j]))
				u[i*this->n + j] = value;// u[j];
	}

/*

	else if(boundaryCondition == 5)
	{
		for(int i = 0; i < this->n - 1; i++)
			for(int j = 1;j < this->n; j++)
				//if(fabs(u[i*this->n + j]) <  fabs(u[i*this->n]))
				u[i*this->n + j] = value;// u[i*this->n];
	}
	else if(boundaryCondition == 10)
	{
		for(int i = 1; i < this->n; i++)
			for(int j =0 ;j < this->n -1; j++)
				//if(fabs(u[i*this->n + j]) < fabs(u[(i+1)*this->n - 1]))
				u[i*this->n + j] = value;// u[(i+1)*this->n - 1];
	}
	else if(boundaryCondition == 3)
	{
		for(int j = 0; j < this->n - 1; j++)
			for(int i = 0;i < this->n - 1; i++)
				//if(fabs(u[i*this->n + j]) < fabs(u[j + this->n*(this->n - 1)]))
				u[i*this->n + j] = value;// u[j + this->n*(this->n - 1)];
	}
	else if(boundaryCondition == 12)
	{
		for(int j = 1; j < this->n; j++)
			for(int i = 1;i < this->n; i++)
				//if(fabs(u[i*this->n + j]) < fabs(u[j]))
				u[i*this->n + j] = value;// u[j];
	}
*/


	/**/

	/*if (u.max() > 0.0)
		this->stopTime *=(double) this->gridCols;*/


   double time = 0.0;
   double currentTau = this->tau0;
   double finalTime = this->stopTime;// + 3.0*(u.max() - u.min());
   if( time + currentTau > finalTime ) currentTau = finalTime - time;

   double maxResidue( 1.0 );
   //double lastResidue( 10000.0 );
   while( time < finalTime /*|| maxResidue > subMesh.getHx()*/)
   {
      /****
       * Compute the RHS
       */

      for( int i = 0; i < fu.getSize(); i ++ )
      {
    	  fu[ i ] = schemeHost.getValue( this->subMesh, i, this->subMesh.getCellCoordinates(i), u, time, boundaryCondition );
      }
      maxResidue = fu. absMax();


      if( this -> cflCondition * maxResidue != 0.0)
    	  currentTau =  this -> cflCondition / maxResidue;

     /* if (maxResidue < 0.05)
    	  cout << "Max < 0.05" << endl;*/
      if(currentTau > 1.0 * this->subMesh.getHx())
      {
    	  //cout << currentTau << " >= " << 2.0 * this->subMesh.getHx() << endl;
    	  currentTau = 1.0 * this->subMesh.getHx();
      }
      /*if(maxResidue > lastResidue)
    	  currentTau *=(1.0/10.0);*/


      if( time + currentTau > finalTime ) currentTau = finalTime - time;
//      for( int i = 0; i < fu.getSize(); i ++ )
//      {
//    	  //cout << "Too big RHS! i = " << i << ", fu = " << fu[i] << ", u = " << u[i] << endl;
//    	  if((u[i]+currentTau * fu[ i ])*u[i] < 0.0 && fu[i] != 0.0 && u[i] != 0.0 )
//    		  currentTau = fabs(u[i]/(2.0*fu[i]));
//
//      }


      for( int i = 0; i < fu.getSize(); i ++ )
      {
    	  double add = u[i] + currentTau * fu[ i ];
    	  //if( fabs(u[i]) < fabs(add) or (this->subgridValues[subGridID] == this->currentStep +4) )
    		  u[ i ] = add;
      }
      time += currentTau;

      //cout << '\r' << flush;
     //cout << maxResidue << "   " << currentTau << " @ " << time << flush;
     //lastResidue = maxResidue;
   }
   //cout << "Time: " << time << ", Res: " << maxResidue <<endl;
	/*if (u.max() > 0.0)
		this->stopTime /=(double) this->gridCols;*/

	VectorType solution;
	solution.setLike(u);
    for( int i = 0; i < u.getSize(); i ++ )
  	{
    	solution[i]=u[i];
   	}
	return solution;
}


#ifdef HAVE_CUDA


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getSubgridCUDA3D( const int i ,tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* caller, double* a)
{
	//int j = threadIdx.x + threadIdx.y * blockDim.x;
	int th = (blockIdx.z*caller->n + threadIdx.z) * caller->n*caller->n*caller->gridCols*caller->gridRows
			 + (blockIdx.y) * caller->n*caller->n*caller->gridCols
             + (blockIdx.x) * caller->n
             + threadIdx.y * caller->n*caller->gridCols
             + threadIdx.x;
	//printf("i= %d,j= %d,th= %d\n",i,j,th);
	*a = caller->work_u_cuda[th];
	//printf("Hi %f \n", *a);
	//return ret;
}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::updateSubgridCUDA3D( const int i ,tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* caller, double* a)
{
//	int j = threadIdx.x + threadIdx.y * blockDim.x;
	int index = (blockIdx.z*caller->n + threadIdx.z) * caller->n*caller->n*caller->gridCols*caller->gridRows
				+ (blockIdx.y) * caller->n*caller->n*caller->gridCols
				+ (blockIdx.x) * caller->n
				+ threadIdx.y * caller->n*caller->gridCols
				+ threadIdx.x;

	if( (fabs(caller->work_u_cuda[index]) > fabs(*a)) || (caller->unusedCell_cuda[index] == 1) )
	{
		caller->work_u_cuda[index] = *a;
		caller->unusedCell_cuda[index] = 0;

	}

	*a = caller->work_u_cuda[index];
}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::insertSubgridCUDA3D( double u, const int i )
{


//	int j = threadIdx.x + threadIdx.y * blockDim.x;
	//printf("j = %d, u = %f\n", j,u);

		int index = (blockIdx.z*caller->n + threadIdx.z) * caller->n*caller->n*caller->gridCols*caller->gridRows
				 + (blockIdx.y) * caller->n*caller->n*caller->gridCols
	             + (blockIdx.x) * caller->n
	             + threadIdx.y * caller->n*caller->gridCols
	             + threadIdx.x;

		//printf("i= %d,j= %d,index= %d\n",i,j,index);
		if( (fabs(this->work_u_cuda[index]) > fabs(u)) || (this->unusedCell_cuda[index] == 1) )
		{
			this->work_u_cuda[index] = u;
			this->unusedCell_cuda[index] = 0;

		}


}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::runSubgridCUDA3D( int boundaryCondition, double* u, int subGridID)
{

	__shared__ int tmp;
	__shared__ double value;
	//double tmpRes = 0.0;
	volatile double* sharedTau = &u[blockDim.x*blockDim.y*blockDim.z];
	volatile double* absVal = &u[2*blockDim.x*blockDim.y*blockDim.z];
	int i = threadIdx.x;
	int j = threadIdx.y;
	int k = threadIdx.z;
	int l = threadIdx.y * blockDim.x + threadIdx.x + blockDim.x*blockDim.y*threadIdx.z;
	bool computeFU = !((i == 0 && (boundaryCondition & 4)) or
			 (i == blockDim.x - 1 && (boundaryCondition & 2)) or
			 (j == 0 && (boundaryCondition & 8)) or
			 (j == blockDim.y - 1  && (boundaryCondition & 1))or
			 (k == 0 && (boundaryCondition & 32)) or
			 (k == blockDim.z - 1  && (boundaryCondition & 16)));

	if(l == 0)
	{
		tmp = 0;
		int centreGID = (blockDim.y*blockIdx.y + (blockDim.y>>1) )*(blockDim.x*gridDim.x) + blockDim.x*blockIdx.x + (blockDim.x>>1)
				      + ((blockDim.z>>1)+blockDim.z*blockIdx.z)*blockDim.x*blockDim.y*gridDim.x*gridDim.y;
		if(this->unusedCell_cuda[centreGID] == 0 || boundaryCondition == 0)
			tmp = 1;
	}
	__syncthreads();

	/*if(!tmp && (u[0]*u[l] <= 0.0))
		atomicMax( &tmp, 1);*/

	__syncthreads();
	if(tmp !=1)
	{
//		if(computeFU)
//			absVal[l]=0.0;
//		else
//			absVal[l] = fabs(u[l]);
//
//		__syncthreads();
//
//	      if((blockDim.x == 16) && (l < 128))		absVal[l] = Max(absVal[l],absVal[l+128]);
//	      __syncthreads();
//	      if((blockDim.x == 16) && (l < 64))		absVal[l] = Max(absVal[l],absVal[l+64]);
//	      __syncthreads();
//	      if(l < 32)    							absVal[l] = Max(absVal[l],absVal[l+32]);
//	      if(l < 16)								absVal[l] = Max(absVal[l],absVal[l+16]);
//	      if(l < 8)									absVal[l] = Max(absVal[l],absVal[l+8]);
//	      if(l < 4)									absVal[l] = Max(absVal[l],absVal[l+4]);
//	      if(l < 2)									absVal[l] = Max(absVal[l],absVal[l+2]);
//	      if(l < 1)									value   = Sign(u[0])*Max(absVal[l],absVal[l+1]);
//		__syncthreads();
//
//		if(computeFU)
//			u[l] = value;
		if(computeFU)
		{
			if(boundaryCondition == 4)
				u[l] = u[this->subMesh.getCellCoordinates(0,j,k)] + Sign(u[0])*this->subMesh.getHx()*(threadIdx.x) ;//+  2*Sign(u[0])*this->subMesh.getHx()*(threadIdx.x+this->n);
			else if(boundaryCondition == 2)
				u[l] = u[this->subMesh.getCellCoordinates(blockDim.x - 1,j,k)] + Sign(u[0])*this->subMesh.getHx()*(this->n - 1 - threadIdx.x);//+ 2*Sign(u[0])*this->subMesh.getHx()*(blockDim.x - threadIdx.x - 1+this->n);
			else if(boundaryCondition == 8)
				u[l] = u[this->subMesh.getCellCoordinates(i,0,k)] + Sign(u[0])*this->subMesh.getHx()*(threadIdx.y) ;//+ 2*Sign(u[0])*this->subMesh.getHx()*(threadIdx.y+this->n);
			else if(boundaryCondition == 1)
				u[l] = u[this->subMesh.getCellCoordinates(i,blockDim.y - 1,k)] + Sign(u[0])*this->subMesh.getHx()*(this->n - 1 - threadIdx.y) ;//+ Sign(u[0])*this->subMesh.getHx()*(blockDim.y - threadIdx.y  - 1 +this->n);
			else if(boundaryCondition == 32)
				u[l] = u[this->subMesh.getCellCoordinates(i,j,0)] + Sign(u[0])*this->subMesh.getHx()*(threadIdx.z);
			else if(boundaryCondition == 16)
				u[l] = u[this->subMesh.getCellCoordinates(i,j,blockDim.z - 1)] + Sign(u[0])*this->subMesh.getHx()*(this->n - 1 - threadIdx.z) ;
		}
	}

   double time = 0.0;
   __shared__ double currentTau;
   double cfl = this->cflCondition;
   double fu = 0.0;
//   if(threadIdx.x * threadIdx.y == 0)
//   {
//	   currentTau = this->tau0;
//   }
   double finalTime = this->stopTime;
   __syncthreads();
   if( time + currentTau > finalTime ) currentTau = finalTime - time;


   while( time < finalTime )
   {
	  if(computeFU)
		  fu = schemeHost.getValueDev( this->subMesh, l, tnlStaticVector<3,int>(i,j)/*this->subMesh.getCellCoordinates(l)*/, u, time, boundaryCondition);

	  sharedTau[l]=cfl/fabs(fu);

      if(l == 0)
      {
    	  if(sharedTau[0] > 1.0 * this->subMesh.getHx())	sharedTau[0] = 1.0 * this->subMesh.getHx();
      }
      else if(l == blockDim.x*blockDim.y*blockDim.z - 1)
    	  if( time + sharedTau[l] > finalTime )		sharedTau[l] = finalTime - time;



      if((blockDim.x == 8) && (l < 128))		sharedTau[l] = Min(sharedTau[l],sharedTau[l+128]);
      __syncthreads();
      if((blockDim.x == 8) && (l < 64))		sharedTau[l] = Min(sharedTau[l],sharedTau[l+64]);
      __syncthreads();
      if(l < 32)    							sharedTau[l] = Min(sharedTau[l],sharedTau[l+32]);
      if(l < 16)								sharedTau[l] = Min(sharedTau[l],sharedTau[l+16]);
      if(l < 8)									sharedTau[l] = Min(sharedTau[l],sharedTau[l+8]);
      if(l < 4)									sharedTau[l] = Min(sharedTau[l],sharedTau[l+4]);
      if(l < 2)									sharedTau[l] = Min(sharedTau[l],sharedTau[l+2]);
      if(l < 1)									currentTau   = Min(sharedTau[l],sharedTau[l+1]);
	__syncthreads();

      u[l] += currentTau * fu;
      time += currentTau;
   }


}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
int tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getOwnerCUDA3D(int i) const
{
	int j = i % (this->gridCols*this->gridRows*this->n*this->n)

	return ( (i / (this->gridCols*this->gridRows*this->n*this->n))/this->n
			+ (j / (this->gridCols*this->n*this->n))*this->gridCols
			+ (i % (this->gridCols*this->n))/this->n);
}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
int tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getSubgridValueCUDA3D( int i ) const
{
	return this->subgridValues_cuda[i];
}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::setSubgridValueCUDA3D(int i, int value)
{
	this->subgridValues_cuda[i] = value;
}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
int tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::getBoundaryConditionCUDA3D( int i ) const
{
	return this->boundaryConditions_cuda[i];
}


template< typename SchemeHost, typename SchemeDevice, typename Device>
__device__
void tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::setBoundaryConditionCUDA3D(int i, int value)
{
	this->boundaryConditions_cuda[i] = value;
}



//north - 1, east - 2, west - 4, south - 8, up -16, down - 32

template <typename SchemeHost, typename SchemeDevice, typename Device>
__global__
void /*tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::*/synchronizeCUDA3D(tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* cudaSolver) //needs fix ---- maybe not anymore --- but frankly: yeah, it does -- aaaa-and maybe fixed now
{

	__shared__ int boundary[4]; // north,east,west,south
	__shared__ int subgridValue;
	__shared__ int newSubgridValue;


	int gid = (blockDim.y*blockIdx.y + threadIdx.y)*blockDim.x*gridDim.x + blockDim.x*blockIdx.x + threadIdx.x;
	double u = cudaSolver->work_u_cuda[gid];
	double u_cmp;
	int subgridValue_cmp=INT_MAX;
	int boundary_index=0;


	if(threadIdx.x+threadIdx.y == 0)
	{
		subgridValue = cudaSolver->getSubgridValueCUDA3D(blockIdx.y*gridDim.x + blockIdx.x);
		boundary[0] = 0;
		boundary[1] = 0;
		boundary[2] = 0;
		boundary[3] = 0;
		newSubgridValue = 0;
		//printf("%d   %d\n", blockDim.x, gridDim.x);
	}
	__syncthreads();



	if(		(threadIdx.x == 0 				/*				&& !(cudaSolver->currentStep & 1)*/) 		||
			(threadIdx.y == 0 				 	/*			&& (cudaSolver->currentStep & 1)*/) 		||
			(threadIdx.x == blockDim.x - 1 	 /*	&& !(cudaSolver->currentStep & 1)*/) 		||
			(threadIdx.y == blockDim.y - 1 	 /*	&& (cudaSolver->currentStep & 1)*/) 		)
	{
		if(threadIdx.x == 0 && (blockIdx.x != 0)/* && !(cudaSolver->currentStep & 1)*/)
		{
			u_cmp = cudaSolver->work_u_cuda[gid - 1];
			subgridValue_cmp = cudaSolver->getSubgridValueCUDA3D(blockIdx.y*gridDim.x + blockIdx.x - 1);
			boundary_index = 2;
		}

		if(threadIdx.x == blockDim.x - 1 && (blockIdx.x != gridDim.x - 1)/* && !(cudaSolver->currentStep & 1)*/)
		{
			u_cmp = cudaSolver->work_u_cuda[gid + 1];
			subgridValue_cmp = cudaSolver->getSubgridValueCUDA3D(blockIdx.y*gridDim.x + blockIdx.x + 1);
			boundary_index = 1;
		}
//		if(threadIdx.y == 0 && (blockIdx.y != 0) && (cudaSolver->currentStep & 1))
//		{
//			u_cmp = cudaSolver->work_u_cuda[gid - blockDim.x*gridDim.x];
//			subgridValue_cmp = cudaSolver->getSubgridValueCUDA3D((blockIdx.y - 1)*gridDim.x + blockIdx.x);
//			boundary_index = 3;
//		}
//		if(threadIdx.y == blockDim.y - 1 && (blockIdx.y != gridDim.y - 1) && (cudaSolver->currentStep & 1))
//		{
//			u_cmp = cudaSolver->work_u_cuda[gid + blockDim.x*gridDim.x];
//			subgridValue_cmp = cudaSolver->getSubgridValueCUDA3D((blockIdx.y + 1)*gridDim.x + blockIdx.x);
//			boundary_index = 0;
//		}

		__threadfence();
		if((subgridValue == INT_MAX || fabs(u_cmp) + cudaSolver->delta < fabs(u) ) && (subgridValue_cmp != INT_MAX && subgridValue_cmp != -INT_MAX))
		{
			cudaSolver->unusedCell_cuda[gid] = 0;
			atomicMax(&newSubgridValue, INT_MAX);
			atomicMax(&boundary[boundary_index], 1);
			cudaSolver->work_u_cuda[gid] = u_cmp;
			u=u_cmp;
		}
		//__threadfence();
		if(threadIdx.y == 0 && (blockIdx.y != 0)/* && (cudaSolver->currentStep & 1)*/)
		{
			u_cmp = cudaSolver->work_u_cuda[gid - blockDim.x*gridDim.x];
			subgridValue_cmp = cudaSolver->getSubgridValueCUDA3D((blockIdx.y - 1)*gridDim.x + blockIdx.x);
			boundary_index = 3;
		}
		if(threadIdx.y == blockDim.y - 1 && (blockIdx.y != gridDim.y - 1)/* && (cudaSolver->currentStep & 1)*/)
		{
			u_cmp = cudaSolver->work_u_cuda[gid + blockDim.x*gridDim.x];
			subgridValue_cmp = cudaSolver->getSubgridValueCUDA3D((blockIdx.y + 1)*gridDim.x + blockIdx.x);
			boundary_index = 0;
		}

		__threadfence();
		if((subgridValue == INT_MAX || fabs(u_cmp) + cudaSolver->delta < fabs(u) ) && (subgridValue_cmp != INT_MAX && subgridValue_cmp != -INT_MAX))
		{
			cudaSolver->unusedCell_cuda[gid] = 0;
			atomicMax(&newSubgridValue, INT_MAX);
			atomicMax(&boundary[boundary_index], 1);
			cudaSolver->work_u_cuda[gid] = u_cmp;
		}
	}
	__syncthreads();

	if(threadIdx.x+threadIdx.y == 0)
	{
		if(subgridValue == INT_MAX && newSubgridValue !=0)
			cudaSolver->setSubgridValueCUDA3D(blockIdx.y*gridDim.x + blockIdx.x, -INT_MAX);

		cudaSolver->setBoundaryConditionCUDA3D(blockIdx.y*gridDim.x + blockIdx.x, 	boundary[0] +
																				2 * boundary[1] +
																				4 * boundary[2] +
																				8 * boundary[3]);
	}


	/*
	//printf("I am not an empty kernel!\n");
	//cout << "Synchronizig..." << endl;
	int tmp1, tmp2;
	int grid1, grid2;

	if(cudaSolver->currentStep & 1)
	{
		//printf("I am not an empty kernel! 1\n");
		for(int j = 0; j < cudaSolver->gridRows - 1; j++)
		{
			//printf("I am not an empty kernel! 3\n");
			for (int i = 0; i < cudaSolver->gridCols*cudaSolver->n; i++)
			{
				tmp1 = cudaSolver->gridCols*cudaSolver->n*((cudaSolver->n-1)+j*cudaSolver->n) + i;
				tmp2 = cudaSolver->gridCols*cudaSolver->n*((cudaSolver->n)+j*cudaSolver->n) + i;
				grid1 = cudaSolver->getSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1));
				grid2 = cudaSolver->getSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2));

				if ((fabs(cudaSolver->work_u_cuda[tmp1]) < fabs(cudaSolver->work_u_cuda[tmp2]) - cudaSolver->delta || grid2 == INT_MAX || grid2 == -INT_MAX) && (grid1 != INT_MAX && grid1 != -INT_MAX))
				{
					//printf("%d %d %d %d \n",tmp1,tmp2,cudaSolver->getOwnerCUDA3D(tmp1),cudaSolver->getOwnerCUDA3D(tmp2));
					cudaSolver->work_u_cuda[tmp2] = cudaSolver->work_u_cuda[tmp1];
					cudaSolver->unusedCell_cuda[tmp2] = 0;
					if(grid2 == INT_MAX)
					{
						cudaSolver->setSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2), -INT_MAX);
					}
					if(! (cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2)) & 8) )
						cudaSolver->setBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2), cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2))+8);
				}
				else if ((fabs(cudaSolver->work_u_cuda[tmp1]) > fabs(cudaSolver->work_u_cuda[tmp2]) + cudaSolver->delta || grid1 == INT_MAX || grid1 == -INT_MAX) && (grid2 != INT_MAX && grid2 != -INT_MAX))
				{
					//printf("%d %d %d %d \n",tmp1,tmp2,cudaSolver->getOwnerCUDA3D(tmp1),cudaSolver->getOwnerCUDA3D(tmp2));
					cudaSolver->work_u_cuda[tmp1] = cudaSolver->work_u_cuda[tmp2];
					cudaSolver->unusedCell_cuda[tmp1] = 0;
					if(grid1 == INT_MAX)
					{
						cudaSolver->setSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1), -INT_MAX);
					}
					if(! (cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1)) & 1) )
						cudaSolver->setBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1), cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1))+1);
				}
			}
		}

	}
	else
	{
		//printf("I am not an empty kernel! 2\n");
		for(int i = 1; i < cudaSolver->gridCols; i++)
		{
			//printf("I am not an empty kernel! 4\n");
			for (int j = 0; j < cudaSolver->gridRows*cudaSolver->n; j++)
			{

				tmp1 = cudaSolver->gridCols*cudaSolver->n*j + i*cudaSolver->n - 1;
				tmp2 = cudaSolver->gridCols*cudaSolver->n*j + i*cudaSolver->n ;
				grid1 = cudaSolver->getSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1));
				grid2 = cudaSolver->getSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2));

				if ((fabs(cudaSolver->work_u_cuda[tmp1]) < fabs(cudaSolver->work_u_cuda[tmp2]) - cudaSolver->delta || grid2 == INT_MAX || grid2 == -INT_MAX) && (grid1 != INT_MAX && grid1 != -INT_MAX))
				{
					//printf("%d %d %d %d \n",tmp1,tmp2,cudaSolver->getOwnerCUDA3D(tmp1),cudaSolver->getOwnerCUDA3D(tmp2));
					cudaSolver->work_u_cuda[tmp2] = cudaSolver->work_u_cuda[tmp1];
					cudaSolver->unusedCell_cuda[tmp2] = 0;
					if(grid2 == INT_MAX)
					{
						cudaSolver->setSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2), -INT_MAX);
					}
					if(! (cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2)) & 4) )
						cudaSolver->setBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2), cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp2))+4);
				}
				else if ((fabs(cudaSolver->work_u_cuda[tmp1]) > fabs(cudaSolver->work_u_cuda[tmp2]) + cudaSolver->delta || grid1 == INT_MAX || grid1 == -INT_MAX) && (grid2 != INT_MAX && grid2 != -INT_MAX))
				{
					//printf("%d %d %d %d \n",tmp1,tmp2,cudaSolver->getOwnerCUDA3D(tmp1),cudaSolver->getOwnerCUDA3D(tmp2));
					cudaSolver->work_u_cuda[tmp1] = cudaSolver->work_u_cuda[tmp2];
					cudaSolver->unusedCell_cuda[tmp1] = 0;
					if(grid1 == INT_MAX)
					{
						cudaSolver->setSubgridValueCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1), -INT_MAX);
					}
					if(! (cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1)) & 2) )
						cudaSolver->setBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1), cudaSolver->getBoundaryConditionCUDA3D(cudaSolver->getOwnerCUDA3D(tmp1))+2);
				}
			}
		}
	}
	//printf("I am not an empty kernel! 5 cudaSolver->currentStep : %d \n", cudaSolver->currentStep);

	cudaSolver->currentStep = cudaSolver->currentStep + 1;
	int stepValue = cudaSolver->currentStep + 4;
	for (int i = 0; i < cudaSolver->gridRows * cudaSolver->gridCols; i++)
	{
		if( cudaSolver->getSubgridValueCUDA3D(i) == -INT_MAX )
			cudaSolver->setSubgridValueCUDA3D(i, stepValue);
	}

	int maxi = 0;
	for(int q=0; q < cudaSolver->gridRows*cudaSolver->gridCols;q++)
	{
		//printf("%d : %d\n", q, cudaSolver->boundaryConditions_cuda[q]);
		maxi=Max(maxi,cudaSolver->getBoundaryConditionCUDA3D(q));
	}
	//printf("I am not an empty kernel! %d\n", maxi);
	*(cudaSolver->runcuda) = (maxi > 0);
	//printf("I am not an empty kernel! 7 %d\n", cudaSolver->boundaryConditions_cuda[0]);
	//cout << "Grid synchronized at step " << (this->currentStep - 1 ) << endl;
*/
}



template <typename SchemeHost, typename SchemeDevice, typename Device>
__global__
void synchronize2CUDA3D(tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* cudaSolver)
{
	if(blockIdx.x+blockIdx.y ==0)
	{
		cudaSolver->currentStep = cudaSolver->currentStep + 1;
		*(cudaSolver->runcuda) = 0;
	}

	int stepValue = cudaSolver->currentStep + 4;
	if( cudaSolver->getSubgridValueCUDA3D(blockIdx.y*gridDim.x + blockIdx.x) == -INT_MAX )
			cudaSolver->setSubgridValueCUDA3D(blockIdx.y*gridDim.x + blockIdx.x, stepValue);

	atomicMax((cudaSolver->runcuda),cudaSolver->getBoundaryConditionCUDA3D(blockIdx.y*gridDim.x + blockIdx.x));
}








template< typename SchemeHost, typename SchemeDevice, typename Device>
__global__
void /*tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::*/initCUDA3D( tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* cudaSolver, double* ptr , int* ptr2, int* ptr3)
{
	//cout << "Initializating solver..." << endl;
	//const tnlString& meshLocation = parameters.getParameter <tnlString>("mesh");
	//this->mesh_cuda.load( meshLocation );

	//this->n_cuda = parameters.getParameter <int>("subgrid-size");
	//cout << "Setting N << this->n_cuda << endl;

	//this->subMesh_cuda.setDimensions( this->n_cuda, this->n_cuda );
	//this->subMesh_cuda.setDomain( tnlStaticVector<3,double>(0.0, 0.0),
							 //tnlStaticVector<3,double>(this->mesh_cuda.getHx()*(double)(this->n_cuda), this->mesh_cuda.getHy()*(double)(this->n_cuda)) );

	//this->subMesh_cuda.save("submesh.tnl");

//	const tnlString& initialCondition = parameters.getParameter <tnlString>("initial-condition");
//	this->u0.load( initialCondition );

	//cout << this->mesh.getCellCenter(0) << endl;

	//this->delta_cuda = parameters.getParameter <double>("delta");
	//this->delta_cuda *= this->mesh_cuda.getHx()*this->mesh_cuda.getHy();

	//cout << "Setting delta to " << this->delta << endl;

	//this->tau0_cuda = parameters.getParameter <double>("initial-tau");
	//cout << "Setting initial tau to " << this->tau0_cuda << endl;
	//this->stopTime_cuda = parameters.getParameter <double>("stop-time");

	//this->cflCondition_cuda = parameters.getParameter <double>("cfl-condition");
	//this -> cflCondition_cuda *= sqrt(this->mesh_cuda.getHx()*this->mesh_cuda.getHy());
	//cout << "Setting CFL to " << this->cflCondition << endl;
////
////

//	this->gridRows_cuda = gridRows;
//	this->gridCols_cuda = gridCols;

	cudaSolver->work_u_cuda = ptr;//(double*)malloc(cudaSolver->gridCols*cudaSolver->gridRows*cudaSolver->n*cudaSolver->n*sizeof(double));
	cudaSolver->unusedCell_cuda = ptr3;//(int*)malloc(cudaSolver->gridCols*cudaSolver->gridRows*cudaSolver->n*cudaSolver->n*sizeof(int));
	cudaSolver->subgridValues_cuda =(int*)malloc(cudaSolver->gridCols*cudaSolver->gridRows*sizeof(int));
	cudaSolver->boundaryConditions_cuda =(int*)malloc(cudaSolver->gridCols*cudaSolver->gridRows*sizeof(int));
	cudaSolver->runcuda = ptr2;//(bool*)malloc(sizeof(bool));
	*(cudaSolver->runcuda) = 1;
	cudaSolver->currentStep = 1;
	//cudaMemcpy(ptr,&(cudaSolver->work_u_cuda), sizeof(double*),cudaMemcpyDeviceToHost);
	//ptr = cudaSolver->work_u_cuda;
	printf("GPU memory allocated.\n");

	for(int i = 0; i < cudaSolver->gridCols*cudaSolver->gridRows; i++)
	{
		cudaSolver->subgridValues_cuda[i] = INT_MAX;
		cudaSolver->boundaryConditions_cuda[i] = 0;
	}

	/*for(long int j = 0; j < cudaSolver->n*cudaSolver->n*cudaSolver->gridCols*cudaSolver->gridRows; j++)
	{
		printf("%d\n",j);
		cudaSolver->unusedCell_cuda[ j] = 1;
	}*/
	printf("GPU memory initialized.\n");


	//cudaSolver->work_u_cuda[50] = 32.153438;
////
////
	//stretchGrid();
	//this->stopTime_cuda /= (double)(this->gridCols_cuda);
	//this->stopTime_cuda *= (1.0+1.0/((double)(this->n_cuda) - 1.0));
	//cout << "Setting stopping time to " << this->stopTime << endl;
	//this->stopTime_cuda = 1.5*((double)(this->n_cuda))*parameters.getParameter <double>("stop-time")*this->mesh_cuda.getHx();
	//cout << "Setting stopping time to " << this->stopTime << endl;

	//cout << "Initializating scheme..." << endl;
	//if(!this->schemeDevice.init(parameters))
//	{
		//cerr << "Scheme failed to initialize." << endl;
//		return false;
//	}
	//cout << "Scheme initialized." << endl;

	//test();

//	this->currentStep_cuda = 1;
	//return true;
}




//extern __shared__ double array[];
template< typename SchemeHost, typename SchemeDevice, typename Device >
__global__
void /*tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::*/initRunCUDA3D(tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* caller)

{


	extern __shared__ double u[];
	//printf("%p\n",caller->work_u_cuda);

	int i = blockIdx.y * gridDim.x + blockIdx.x;
	int l = threadIdx.y * blockDim.x + threadIdx.x;

	__shared__ int containsCurve;
	if(l == 0)
		containsCurve = 0;

	//double a;
	caller->getSubgridCUDA3D(i,caller, &u[l]);
	//printf("%f   %f\n",a , u[l]);
	//u[l] = a;
	//printf("Hi %f \n", u[l]);
	__syncthreads();
	//printf("hurewrwr %f \n", u[l]);
	if(u[0] * u[l] <= 0.0)
	{
		//printf("contains %d \n",i);
		atomicMax( &containsCurve, 1);
	}

	__syncthreads();
	//printf("hu");
	//printf("%d : %f\n", l, u[l]);
	if(containsCurve == 1)
	{
		//printf("have curve \n");
		caller->runSubgridCUDA3D(0,u,i);
		//printf("%d : %f\n", l, u[l]);
		__syncthreads();
		caller->insertSubgridCUDA3D(u[l],i);
		__syncthreads();
		if(l == 0)
			caller->setSubgridValueCUDA3D(i, 4);
	}


}





template< typename SchemeHost, typename SchemeDevice, typename Device >
__global__
void /*tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int>::*/runCUDA3D(tnlParallelEikonalSolver<3,SchemeHost, SchemeDevice, Device, double, int >* caller)
{
	extern __shared__ double u[];
	int i = blockIdx.y * gridDim.x + blockIdx.x;
	int l = threadIdx.y * blockDim.x + threadIdx.x;
	int bound = caller->getBoundaryConditionCUDA3D(i);

	if(caller->getSubgridValueCUDA3D(i) != INT_MAX && bound != 0 && caller->getSubgridValueCUDA3D(i) > 0)
	{
		caller->getSubgridCUDA3D(i,caller, &u[l]);

		//if(l == 0)
			//printf("i = %d, bound = %d\n",i,caller->getSubgridValueCUDA3D(i));
		if(caller->getSubgridValueCUDA3D(i) == caller->currentStep+4)
		{
			if(bound & 1)
			{
				caller->runSubgridCUDA3D(1,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}
			if(bound & 2 )
			{
				caller->runSubgridCUDA3D(2,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}
			if(bound & 4)
			{
				caller->runSubgridCUDA3D(4,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}
			if(bound & 8)
			{
				caller->runSubgridCUDA3D(8,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}





			if( ((bound & 3 )))
				{
					caller->runSubgridCUDA3D(3,u,i);
					//__syncthreads();
					//caller->insertSubgridCUDA3D(u[l],i);
					//__syncthreads();
					//caller->getSubgridCUDA3D(i,caller, &u[l]);
					caller->updateSubgridCUDA3D(i,caller, &u[l]);
					__syncthreads();
				}
				if( ((bound & 5 )))
				{
					caller->runSubgridCUDA3D(5,u,i);
					//__syncthreads();
					//caller->insertSubgridCUDA3D(u[l],i);
					//__syncthreads();
					//caller->getSubgridCUDA3D(i,caller, &u[l]);
					caller->updateSubgridCUDA3D(i,caller, &u[l]);
					__syncthreads();
				}
				if( ((bound & 10 )))
				{
					caller->runSubgridCUDA3D(10,u,i);
					//__syncthreads();
					//caller->insertSubgridCUDA3D(u[l],i);
					//__syncthreads();
					//caller->getSubgridCUDA3D(i,caller, &u[l]);
					caller->updateSubgridCUDA3D(i,caller, &u[l]);
					__syncthreads();
				}
				if(   (bound & 12 ))
				{
					caller->runSubgridCUDA3D(12,u,i);
					//__syncthreads();
					//caller->insertSubgridCUDA3D(u[l],i);
					//__syncthreads();
					//caller->getSubgridCUDA3D(i,caller, &u[l]);
					caller->updateSubgridCUDA3D(i,caller, &u[l]);
					__syncthreads();
				}





		}


		else
		{









			if( ((bound == 2)))
						{
							caller->runSubgridCUDA3D(2,u,i);
							//__syncthreads();
							//caller->insertSubgridCUDA3D(u[l],i);
							//__syncthreads();
							//caller->getSubgridCUDA3D(i,caller, &u[l]);
							caller->updateSubgridCUDA3D(i,caller, &u[l]);
							__syncthreads();
						}
						if( ((bound == 1) ))
						{
							caller->runSubgridCUDA3D(1,u,i);
							//__syncthreads();
							//caller->insertSubgridCUDA3D(u[l],i);
							//__syncthreads();
							//caller->getSubgridCUDA3D(i,caller, &u[l]);
							caller->updateSubgridCUDA3D(i,caller, &u[l]);
							__syncthreads();
						}
						if( ((bound == 8) ))
						{
							caller->runSubgridCUDA3D(8,u,i);
							//__syncthreads();
							//caller->insertSubgridCUDA3D(u[l],i);
							//__syncthreads();
							//caller->getSubgridCUDA3D(i,caller, &u[l]);
							caller->updateSubgridCUDA3D(i,caller, &u[l]);
							__syncthreads();
						}
						if(   (bound == 4))
						{
							caller->runSubgridCUDA3D(4,u,i);
							//__syncthreads();
							//caller->insertSubgridCUDA3D(u[l],i);
							//__syncthreads();
							//caller->getSubgridCUDA3D(i,caller, &u[l]);
							caller->updateSubgridCUDA3D(i,caller, &u[l]);
							__syncthreads();
						}










			if( ((bound & 3) ))
			{
				caller->runSubgridCUDA3D(3,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}
			if( ((bound & 5) ))
			{
				caller->runSubgridCUDA3D(5,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}
			if( ((bound & 10) ))
			{
				caller->runSubgridCUDA3D(10,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}
			if(   (bound & 12) )
			{
				caller->runSubgridCUDA3D(12,u,i);
				//__syncthreads();
				//caller->insertSubgridCUDA3D(u[l],i);
				//__syncthreads();
				//caller->getSubgridCUDA3D(i,caller, &u[l]);
				caller->updateSubgridCUDA3D(i,caller, &u[l]);
				__syncthreads();
			}












		}
		/*if( bound )
		{
			caller->runSubgridCUDA3D(15,u,i);
			__syncthreads();
			//caller->insertSubgridCUDA3D(u[l],i);
			//__syncthreads();
			//caller->getSubgridCUDA3D(i,caller, &u[l]);
			caller->updateSubgridCUDA3D(i,caller, &u[l]);
			__syncthreads();
		}*/

		if(l==0)
		{
			caller->setBoundaryConditionCUDA3D(i, 0);
			caller->setSubgridValueCUDA3D(i, caller->getSubgridValueCUDA3D(i) - 1 );
		}


	}



}

#endif /*HAVE_CUDA*/

#endif /* TNLPARALLELEIKONALSOLVER3D_IMPL_H_ */
