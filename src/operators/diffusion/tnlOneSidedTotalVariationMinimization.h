/***************************************************************************
                          tnlOneSidedTotalVariationMinimization.h  -  description
                             -------------------
    begin                : Feb 17, 2016
    copyright            : (C) 2016 by oberhuber
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

#ifndef TNLONESIDEDTOTALVARIATIONMINIMIZATION_H
#define	TNLONESIDEDTOTALVARIATIONMINIMIZATION_H

#include <operators/tnlOperator.h>
#include <operators/tnlFunctionInverseOperator.h>
#include <operators/geometric/tnlFDMGradientNorm.h>
#include <operators/tnlNeumannBoundaryConditions.h>
#include <operators/diffusion/tnlOneSidedNonlinearDiffusion.h>
#include <functions/tnlOperatorFunction.h>
#include <functions/tnlConstantFunction.h>

template< typename Mesh,
          typename Real = typename Mesh::RealType,
          typename Index = typename Mesh::IndexType,
          bool EvaluateNonlinearityOnFly = false >
class tnlOneSidedTotalVariationMinimization
   : public tnlOperator< Mesh, MeshInteriorDomain, Mesh::getMeshDimensions(), Mesh::getMeshDimensions(), Real, Index >
{
   public:
            
      typedef Mesh MeshType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef tnlFDMGradientNorm< MeshType, tnlForwardFiniteDifference, RealType, IndexType > GradientNorm;
      typedef tnlFunctionInverseOperator< GradientNorm > NonlinearityOperator;
      typedef tnlMeshFunction< MeshType, MeshType::getMeshDimensions(), RealType > NonlinearityMeshFunction;
      typedef tnlConstantFunction< MeshType::getMeshDimensions(), RealType > NonlinearityBoundaryConditionsFunction;
      typedef tnlNeumannBoundaryConditions< MeshType, NonlinearityBoundaryConditionsFunction > NonlinearityBoundaryConditions;
      typedef tnlOperatorFunction< NonlinearityOperator, NonlinearityMeshFunction, NonlinearityBoundaryConditions, EvaluateNonlinearityOnFly > Nonlinearity;
      typedef tnlOneSidedNonlinearDiffusion< Mesh, Nonlinearity, RealType, IndexType > NonlinearDiffusion;
      typedef typename NonlinearDiffusion::ExactOperatorType ExactOperatorType;
      
      tnlOneSidedTotalVariationMinimization()
      : nonlinearity( gradientNorm, nonlinearityBoundaryConditions ),
        nonlinearDiffusion( nonlinearity ){}
      
      static tnlString getType()
      {
         return tnlString( "tnlOneSidedTotalVariationMinimization< " ) +
            MeshType::getType() + ", " +
            ::getType< Real >() + ", " +
            ::getType< Index >() + " >";         
      }
      
      bool refresh( const RealType& time = 0.0 )
      {
         return this->nonlinearity.refresh( time );
      }
      
      bool deepRefresh( const RealType& time = 0.0 )
      {
         return this->nonlinearity.deepRefresh( time );
      }      
      
      template< typename MeshFunction,
                typename MeshEntity >
      __cuda_callable__
      Real operator()( const MeshFunction& u,
                       const MeshEntity& entity,
                       const RealType& time = 0.0 ) const
      {
         return this->nonlinearDiffusion( u, entity, time );
      }

      template< typename MeshEntity >
      __cuda_callable__
      Index getLinearSystemRowLength( const MeshType& mesh,
                                      const IndexType& index,
                                      const MeshEntity& entity ) const
      {
         return this->nonlinearDiffusion.getLinearSystemRowLength( mesh, index, entity );
      }

      template< typename MeshEntity,
                typename MeshFunction,
                typename Vector,
                typename Matrix >
      __cuda_callable__
      void updateLinearSystem( const RealType& time,
                               const RealType& tau,
                               const MeshType& mesh,
                               const IndexType& index,
                               const MeshEntity& entity,
                               const MeshFunction& u,
                               Vector& b,
                               Matrix& matrix ) const
      {
         this->nonlinearDiffusion.updateLinearSystem( time, tau, mesh, index, entity, u, b, matrix );
      }            
      
   protected:      
      
      NonlinearityBoundaryConditions nonlinearityBoundaryConditions;
      
      GradientNorm gradientNorm;
      
      Nonlinearity nonlinearity;
      
      NonlinearDiffusion nonlinearDiffusion;            
};

#endif	/* TNLONESIDEDTOTALVARIATIONMINIMIZATION_H */

