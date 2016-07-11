/* 
 * File:   tnlEikonalOperator.h
 * Author: oberhuber
 *
 * Created on July 11, 2016, 6:33 PM
 */

#pragma once

#include <functions/tnlConstantFunction.h>
#include <functions/tnlFunctionAdapter.h>

template< typename GradientNormOperator,
          typename Anisotropy = 
            tnlConstantFunction< GradientNormOperator::MeshType::getMeshDimensions(),
                                 typename GradientNormOperator::MeshType::RealType > >
class tnlEikonalOperator
   : public tnlOperator< typename GradientNormOperator::MeshType, MeshDomain >
{
      public:
         
         typedef typename GradientNormOperator::MeshType MeshType;
         typedef typename MeshType::RealType RealType;
         typedef typename MeshType::DeviceType DeviceType;
         typedef typename MeshType::IndexType IndexType;      
         typedef GradientNormOperator GradientNormOperatorType;
         typedef Anisotropy AnisotropyType;
         //typedef tnlExactLinearDiffusion< 1 > ExactOperatorType;
      
         static const int Dimensions = MeshType::meshDimensions;
      
         static constexpr int getMeshDimensions() { return Dimensions; }
      
         static tnlString getType();
         
         AnisotropyType& getAnisotropy() { return this->anisotropy; };
         
         const AnisotropyType& getAnisotropy() const { return this->anisotropy; };
         
         const RealType getSmoothing() const { return this->smoothing; };
         
         void setSmoothing( const RealType& smoothing ) { this->smoothing = smoothing; };
         
         template< typename PreimageFunction,
                   typename MeshEntity >
         __cuda_callable__
         RealType operator()( const PreimageFunction& u,
                              const MeshEntity& entity,
                              const RealType& time = 0.0 ) const
         {
            const RealType signU = sign( u( entity ), smoothing * entity.getMesh().getSmallestSpaceStep() );
            const RealType f = tnlFunctionAdapter< MeshType, AnisotropyType >::getValue( anisotropy, entity, 0.0 );
            return signU * ( f - gradientNorm( u, entity, signU ) );
         };

      protected:
         
         GradientNormOperatorType gradientNorm;
         
         AnisotropyType anisotropy;

         RealType smoothing;
         
};
 

