/***************************************************************************
                          tnlBoundaryConditionsSetter.h  -  description
                             -------------------
    begin                : Dec 30, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
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


#ifndef TNLBOUNDARYCONDITIONSSETTER_H
#define	TNLBOUNDARYCONDITIONSSETTER_H

#include <core/tnlCuda.h>
#include <functions/tnlFunctionAdapter.h>

template< typename Real,
          typename DofVector,
          typename BoundaryConditions >
class tnlBoundaryConditionsSetterTraverserUserData
{
   public:

      const Real *time;

      const BoundaryConditions* boundaryConditions;

      DofVector *u;

      tnlBoundaryConditionsSetterTraverserUserData( 
         const Real& time,
         const BoundaryConditions& boundaryConditions,
         DofVector& u )
      : time( &time ),
        boundaryConditions( &boundaryConditions ),
        u( &u )
      {};
};


template< typename Mesh,
          typename DofVector,
          typename BoundaryConditions >
class tnlBoundaryConditionsSetter
{
   public:
      typedef Mesh MeshType;
      typedef typename DofVector::RealType RealType;
      typedef typename DofVector::DeviceType DeviceType;
      typedef typename DofVector::IndexType IndexType;
      typedef tnlBoundaryConditionsSetterTraverserUserData< 
         RealType,
         DofVector,
         BoundaryConditions > TraverserUserData;

      template< typename EntityType >
      void apply( const RealType& time,
                   const MeshType& mesh,
                   const BoundaryConditions& boundaryConditions,
                   DofVector& u ) const;      
      
      class TraverserBoundaryEntitiesProcessor
      {
         public:
            
            template< typename GridEntity >
            __cuda_callable__
            static inline void processEntity( const MeshType& mesh,
                                              TraverserUserData& userData,
                                              const GridEntity& entity )
            {
               userData.boundaryConditions->getValue
               ( entity,
                 *userData.time,                 
                 *userData.u );
            }

      };
};

#include <solvers/pde/tnlBoundaryConditionsSetter_impl.h>

#endif	/* TNLBOUNDARYCONDITIONSSETTER_H */

