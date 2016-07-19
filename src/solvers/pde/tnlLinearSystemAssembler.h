/***************************************************************************
                          tnlLinearSystemAssembler.h  -  description
                             -------------------
    begin                : Oct 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <functions/tnlFunctionAdapter.h>

namespace TNL {

template< typename Real,
          typename MeshFunction,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename RightHandSide,
          typename Matrix,
          typename DofVector >
class tnlLinearSystemAssemblerTraverserUserData
{
   public:
      typedef Matrix MatrixType;
      typedef typename Matrix::DeviceType DeviceType;

      const Real* time;

      const Real* tau;

      const Real* timeDiscretisationCoefficient;

      const DifferentialOperator* differentialOperator;

      const BoundaryConditions* boundaryConditions;

      const RightHandSide* rightHandSide;
 
      const MeshFunction *u;
 
      DofVector *b;

      Matrix *matrix;

      tnlLinearSystemAssemblerTraverserUserData( const Real& time,
                                                 const Real& tau,
                                                 const DifferentialOperator& differentialOperator,
                                                 const BoundaryConditions& boundaryConditions,
                                                 const RightHandSide& rightHandSide,
                                                 const MeshFunction& u,
                                                 Matrix& matrix,
                                                 DofVector& b )
      : time( &time ),
        tau( &tau ),
        differentialOperator( &differentialOperator ),
        boundaryConditions( &boundaryConditions ),
        rightHandSide( &rightHandSide ),
        u( &u ),
        b( &b ),
        matrix( &matrix )
      {};

   protected:

};


template< typename Mesh,
          typename MeshFunction,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename RightHandSide,
          typename TimeDiscretisation,
          typename Matrix,
          typename DofVector >
class tnlLinearSystemAssembler
{
   public:
   typedef Mesh MeshType;
   typedef typename MeshFunction::RealType RealType;
   typedef typename MeshFunction::DeviceType DeviceType;
   typedef typename MeshFunction::IndexType IndexType;
   typedef Matrix MatrixType;
   typedef tnlLinearSystemAssemblerTraverserUserData< RealType,
                                                      MeshFunction,
                                                      DifferentialOperator,
                                                      BoundaryConditions,
                                                      RightHandSide,
                                                      MatrixType,
                                                      DofVector > TraverserUserData;
 
   template< typename EntityType >
   void assembly( const RealType& time,
                  const RealType& tau,
                  const MeshType& mesh,
                  const DifferentialOperator& differentialOperator,
                  const BoundaryConditions& boundaryConditions,
                  const RightHandSide& rightHandSide,
                  const MeshFunction& u,
                  MatrixType& matrix,
                  DofVector& b ) const;

 
      class TraverserBoundaryEntitiesProcessor
   {
      public:
 
         template< typename EntityType >
         __cuda_callable__
         static void processEntity( const MeshType& mesh,
                                    TraverserUserData& userData,
                                    const EntityType& entity )
         {
             ( *userData.b )[ entity.getIndex() ] = 0.0;
             userData.boundaryConditions->setMatrixElements
               ( *userData.u,
                 entity,
                 *userData.time + *userData.tau,
                 *userData.tau,
                 *userData.matrix,
                 *userData.b );
         }
   };

   class TraverserInteriorEntitiesProcessor
   {
      public:

         template< typename EntityType >
         __cuda_callable__
         static void processEntity( const MeshType& mesh,
                                    TraverserUserData& userData,
                                    const EntityType& entity )
         {
            ( *userData.b )[ entity.getIndex() ] = 0.0;
            userData.differentialOperator->setMatrixElements
               ( *userData.u,
                 entity,
                 *userData.time + *userData.tau,
                 *userData.tau,
                 *userData.matrix,
                 *userData.b );
 
            typedef tnlFunctionAdapter< MeshType, RightHandSide > RhsFunctionAdapter;
            typedef tnlFunctionAdapter< MeshType, MeshFunction > MeshFunctionAdapter;
            const RealType& rhs = RhsFunctionAdapter::getValue
               ( *userData.rightHandSide,
                 entity,
                 *userData.time );
            TimeDiscretisation::applyTimeDiscretisation( *userData.matrix,
                                                         ( *userData.b )[ entity.getIndex() ],
                                                         entity.getIndex(),
                                                         MeshFunctionAdapter::getValue( *userData.u, entity, *userData.time ),
                                                         ( *userData.tau ),
                                                         rhs );
         }
   };
};

} // namespace TNL

#include <solvers/pde/tnlLinearSystemAssembler_impl.h>
