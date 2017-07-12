/***************************************************************************
                          godunov2D_impl.h  -  description
                             -------------------
    begin                : Jul 8 , 2014
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

#ifndef GODUNOVEIKONAL2D_IMPL_H_
#define GODUNOVEIKONAL2D_IMPL_H_


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
Real godunovEikonalScheme< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index >:: positivePart(const Real arg) const
{
	if(arg > 0.0)
		return arg;
	return 0.0;
}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
Real  godunovEikonalScheme< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: negativePart(const Real arg) const
{
	if(arg < 0.0)
		return arg;
	return 0.0;
}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
Real godunovEikonalScheme< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: sign(const Real x, const Real eps) const
{
	if(x > eps)
		return 1.0;
	else if (x < -eps)
		return (-1.0);

	if ( eps == 0.0)
		return 0.0;

	return sin((M_PI*x)/(2.0*eps));
}





template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
bool godunovEikonalScheme< tnlGrid< 2,MeshReal, Device, MeshIndex >, Real, Index > :: init( const Config::ParameterContainer& parameters )
{

	   const String& meshFile = parameters.getParameter< String >( "mesh" );
	   if( ! this->originalMesh.load( meshFile ) )
	   {
		   cerr << "I am not able to load the mesh from the file " << meshFile << "." << endl;
		   return false;
	   }




	   hx = originalMesh.template getSpaceStepsProducts< 1, 0 >();
	   hy = originalMesh.template getSpaceStepsProducts< 0, 1 >();

	   epsilon = parameters. getParameter< double >( "epsilon" );

	   if(epsilon != 0.0)
		   epsilon *=sqrt( hx*hx + hy*hy );

//	   dofVector. setSize( this->mesh.getDofs() );

	   return true;

}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
String godunovEikonalScheme< tnlGrid< 2, MeshReal, Device, MeshIndex >, Real, Index > :: getType()
{
   return String( "tnlLinearDiffusion< " ) +
          MeshType::getType() + ", " +
          ::getType< Real >() + ", " +
          ::getType< Index >() + " >";
}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
template< typename Vector >
#ifdef HAVE_CUDA
__device__ __host__
#endif
Real godunovEikonalScheme< tnlGrid< 2, MeshReal, Device, MeshIndex >, Real, Index >:: getValue( const MeshType& mesh,
          	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	 const IndexType cellIndex,
          	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	 const CoordinatesType& coordinates,
          	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	 const Vector& u,
          	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	 const Real& time ) const
{
/*
	RealType nabla, xb, xf, yb, yf, signui;

	signui = sign(u[cellIndex],epsilon);

	   if(signui > 0.0)
	   {
		   xf = negativePart((u[mesh.getCellXSuccessor( cellIndex )] - u[cellIndex])/hx);
		   xb = positivePart((u[cellIndex] - u[mesh.getCellXPredecessor( cellIndex )])/hx);
		   yf = negativePart((u[mesh.getCellYSuccessor( cellIndex )] - u[cellIndex])/hy);
		   yb = positivePart((u[cellIndex] - u[mesh.getCellYPredecessor( cellIndex )])/hy);

		   if(xb + xf > 0.0)
			   xf = 0.0;
		   else
			   xb = 0.0;

		   if(yb + yf > 0.0)
			   yf = 0.0;
		   else
			   yb = 0.0;

		   nabla = sqrt (xf*xf + xb*xb + yf*yf + yb*yb );

		   return signui*(1.0 - nabla);
	   }
	   else if (signui < 0.0)
	   {
		   xf = positivePart((u[mesh.getCellXSuccessor( cellIndex )] - u[cellIndex])/hx);
		   xb = negativePart((u[cellIndex] - u[mesh.getCellXPredecessor( cellIndex )])/hx);
		   yf = positivePart((u[mesh.getCellYSuccessor( cellIndex )] - u[cellIndex])/hy);
		   yb = negativePart((u[cellIndex] - u[mesh.getCellYPredecessor( cellIndex )])/hy);

		   if(xb + xf > 0.0)
			   xb = 0.0;
		   else
			   xf = 0.0;

		   if(yb + yf > 0.0)
			   yb = 0.0;
		   else
			   yf = 0.0;

		   nabla = sqrt (xf*xf + xb*xb + yf*yf + yb*yb );

		   return signui*(1.0 - nabla);
	   }
	   else
*/	   {
		   return 0.0;
	   }

}


#endif /* GODUNOVEIKONAL2D_IMPL_H_ */
