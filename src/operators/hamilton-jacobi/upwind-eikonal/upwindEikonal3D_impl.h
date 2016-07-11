/***************************************************************************
                          upwindEikonal3D_impl.h  -  description
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

#pragma once

#include <functions/tnlFunctions.h>

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
bool upwindEikonalScheme< tnlGrid< 3,MeshReal, Device, MeshIndex >, Real, Index > :: init( const tnlParameterContainer& parameters )
{

	   const tnlString& meshFile = parameters.getParameter< tnlString >( "mesh" );
	   if( ! this->originalMesh.load( meshFile ) )
	   {
		   cerr << "I am not able to load the mesh from the file " << meshFile << "." << endl;
		   return false;
	   }


	   hx = originalMesh.getSpaceSteps().x();
	   hy = originalMesh.getSpaceSteps().y();
	   hz = originalMesh.getSpaceSteps().z();

	   epsilon = parameters. getParameter< double >( "epsilon" );

	   if(epsilon != 0.0)
		   epsilon *=sqrt( hx*hx + hy*hy +hz*hz );

	//   dofVector. setSize( this->mesh.getDofs() );

	   return true;

}


template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
tnlString upwindEikonalScheme< tnlGrid< 3, MeshReal, Device, MeshIndex >, Real, Index > :: getType()
{
   return tnlString( "tnlLinearDiffusion< " ) +
          MeshType::getType() + ", " +
          ::getType< Real >() + ", " +
          ::getType< Index >() + " >";
}

template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Real,
          typename Index >
template< typename PreimageFunction,
          typename MeshEntity >
__cuda_callable__
inline
Real
upwindEikonalScheme< tnlGrid< 3, MeshReal, Device, MeshIndex >, Real, Index >::
operator()( const PreimageFunction& u,
            const MeshEntity& entity,
            const Real& signU ) const
{
	RealType nabla, xb, xf, yb, yf, zb, zf; //, signui;
   //signui = sign( u( entity ), epsilon );
   const RealType& hx_inv = entity.getMesh().template getSpaceStepsProducts< -1,  0,  0 >();
   const RealType& hy_inv = entity.getMesh().template getSpaceStepsProducts<  0, -1,  0 >();
   const RealType& hz_inv = entity.getMesh().template getSpaceStepsProducts<  0,  0, -1 >();

   
   const typename MeshEntity::template NeighbourEntities< 3 >& neighbourEntities = entity.getNeighbourEntities();
   const RealType& u_c = u[ entity.getIndex() ];
   const RealType& u_e = u[ neighbourEntities.template getEntityIndex<  1,  0,  0 >() ];
   const RealType& u_w = u[ neighbourEntities.template getEntityIndex< -1,  0,  0 >() ];
   const RealType& u_n = u[ neighbourEntities.template getEntityIndex<  0,  1,  0 >() ];
   const RealType& u_s = u[ neighbourEntities.template getEntityIndex<  0, -1,  0 >() ];
   const RealType& u_t = u[ neighbourEntities.template getEntityIndex<  0,  0,  1 >() ];
   const RealType& u_b = u[ neighbourEntities.template getEntityIndex<  0,  0, -1 >() ];
      
   if( signU > 0.0 )
   {
      xf = negativePart( ( u_e - u_c ) * hx_inv );
      xb = positivePart( ( u_c - u_w ) * hx_inv );
      yf = negativePart( ( u_n - u_c ) * hy_inv );
      yb = positivePart( ( u_c - u_s ) * hy_inv );
      zf = negativePart( ( u_t - u_c ) * hz_inv );
      zb = positivePart( ( u_c - u_b ) * hz_inv );
      nabla = sqrt( xf * xf + xb * xb + yf * yf + yb * yb + zf * zf + zb * zb );
      //return signui * ( 1.0 - nabla );
      return nabla;
   }
   else if( signU < 0.0 )
   {
      xf = positivePart( ( u_e - u_c ) * hx_inv );
      xb = negativePart( ( u_c - u_w ) * hx_inv );
      yf = positivePart( ( u_n - u_c ) * hy_inv );
      yb = negativePart( ( u_c - u_s ) * hy_inv );
      zf = positivePart( ( u_t - u_c ) * hz_inv );
      zb = negativePart( ( u_c - u_b ) * hz_inv );
      nabla = sqrt( xf * xf + xb * xb + yf * yf + yb * yb + zf * zf + zb * zb );
      //return signui * ( 1.0 - nabla );
      return nabla;
   }
   else
      return 0.0;
}

