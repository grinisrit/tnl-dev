#ifndef HeatEquationBenchmarkRHS_H_
#define HeatEquationBenchmarkRHS_H_
#include <TNL/Functions/Domain.h>
template< typename Mesh, typename Real >class HeatEquationBenchmarkRhs
  : public Functions::Domain< Mesh::meshDimensions, Functions::MeshDomain > 
 {
   public:

      typedef Mesh MeshType;
      typedef Real RealType;

      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" )
      {
         return true;
      }

      template< typename MeshEntity >
      __cuda_callable__
      Real operator()( const MeshEntity& entity,
                       const Real& time = 0.0 ) const
      {
         typedef typename MeshEntity::MeshType::VertexType VertexType;
         VertexType v = entity.getCenter();
         return 0.0;
      }
};

#endif /* HeatEquationBenchmarkRHS_H_ */