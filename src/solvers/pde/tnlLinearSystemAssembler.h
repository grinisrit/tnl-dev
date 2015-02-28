/***************************************************************************
                          tnlLinearSystemAssembler.h  -  description
                             -------------------
    begin                : Oct 11, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
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

#ifndef TNLLINEARSYSTEMASSEMBLER_H_
#define TNLLINEARSYSTEMASSEMBLER_H_

#include <functions/tnlFunctionAdapter.h>

template< typename Real,
          typename DofVector,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename RightHandSide,
          typename Matrix >
class tnlLinearSystemAssemblerTraversalUserData
{
   public:
      typedef Matrix MatrixType;
      typedef typename Matrix::DeviceType DeviceType;

      const Real* time;

      const Real* tau;

      const DifferentialOperator* differentialOperator;

      const BoundaryConditions* boundaryConditions;

      const RightHandSide* rightHandSide;

      DofVector *u, *b;

      Matrix *matrix;

      tnlLinearSystemAssemblerTraversalUserData( const Real& time,
                                                 const Real& tau,
                                                 const DifferentialOperator& differentialOperator,
                                                 const BoundaryConditions& boundaryConditions,
                                                 const RightHandSide& rightHandSide,
                                                 DofVector& u,
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
          typename DofVector,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename RightHandSide,
          typename Matrix >
class tnlLinearSystemAssembler
{
   public:
   typedef Mesh MeshType;
   typedef typename DofVector::RealType RealType;
   typedef typename DofVector::DeviceType DeviceType;
   typedef typename DofVector::IndexType IndexType;
   typedef Matrix MatrixType;
   typedef tnlLinearSystemAssemblerTraversalUserData< RealType,
                                                      DofVector,
                                                      DifferentialOperator,
                                                      BoundaryConditions,
                                                      RightHandSide,
                                                      MatrixType > TraversalUserData;

   template< int EntityDimensions >
   void assembly( const RealType& time,
                  const RealType& tau,
                  const MeshType& mesh,
                  const DifferentialOperator& differentialOperator,
                  const BoundaryConditions& boundaryConditions,
                  const RightHandSide& rightHandSide,
                  DofVector& u,
                  MatrixType& matrix,
                  DofVector& b ) const;

   class TraversalBoundaryEntitiesProcessor
   {
      public:

         template< int EntityDimension >
#ifdef HAVE_CUDA
         __host__ __device__
#endif
         static void processEntity( const MeshType& mesh,
                                    TraversalUserData& userData,
                                    const IndexType index )
         {
            typename MatrixType::MatrixRow matrixRow = userData.matrix->getRow( index );
            userData.boundaryConditions->updateLinearSystem( *userData.time,
                                                            mesh,
                                                            index,
                                                            *userData.u,
                                                            *userData.b,
                                                            matrixRow );
            /*userData.matrix->setRowFast( index,
                                         userData.columns->getData(),
                                         userData.values->getData(),
                                         rowLength );*/
         }

   };

   class TraversalInteriorEntitiesProcessor
   {
      public:

         template< int EntityDimensions >
#ifdef HAVE_CUDA
         __host__ __device__
#endif
         static void processEntity( const MeshType& mesh,
                                    TraversalUserData& userData,
                                    const IndexType index )
         {
            typedef tnlFunctionAdapter< MeshType, RightHandSide > FunctionAdapter;
            ( *userData.b )[ index ] = ( *userData.u )[ index ] +
                     ( *userData.tau ) * FunctionAdapter::getValue( mesh,
                                                                    *userData.rightHandSide,
                                                                    index,
                                                                    *userData.time );

            typename MatrixType::MatrixRow matrixRow = userData.matrix->getRow( index );
            userData.differentialOperator->updateLinearSystem( *userData.time,
                                                               *userData.tau,
                                                               mesh,
                                                               index,
                                                               *userData.u,
                                                               *userData.b,
                                                               matrixRow );
            /*userData.matrix->setRowFast( index,
                                         userData.columns->getData(),
                                         userData.values->getData(),
                                         rowLength );*/
            userData.matrix->addElement( index, index, 1.0, 1.0 );
         }

   };
};

template< int Dimensions,
          typename Real,
          typename Device,
          typename Index,
          typename DofVector,
          typename DifferentialOperator,
          typename BoundaryConditions,
          typename RightHandSide,
          typename Matrix >
class tnlLinearSystemAssembler< tnlGrid< Dimensions, Real, Device, Index >,
                                DofVector,
                                DifferentialOperator,
                                BoundaryConditions,
                                RightHandSide,
                                Matrix >
{
   public:
   typedef tnlGrid< Dimensions, Real, Device, Index > MeshType;
   typedef typename DofVector::RealType RealType;
   typedef typename DofVector::DeviceType DeviceType;
   typedef typename DofVector::IndexType IndexType;
   typedef Matrix MatrixType;
   typedef typename MeshType::CoordinatesType CoordinatesType;
   typedef tnlLinearSystemAssemblerTraversalUserData< RealType,
                                                      DofVector,
                                                      DifferentialOperator,
                                                      BoundaryConditions,
                                                      RightHandSide,
                                                      MatrixType > TraversalUserData;

   template< int EntityDimensions >
   void assembly( const RealType& time,
                  const RealType& tau,
                  const MeshType& mesh,
                  const DifferentialOperator& differentialOperator,
                  const BoundaryConditions& boundaryConditions,
                  const RightHandSide& rightHandSide,
                  DofVector& u,
                  MatrixType& matrix,
                  DofVector& b ) const;

   class TraversalBoundaryEntitiesProcessor
   {
      public:

#ifdef HAVE_CUDA
         __host__ __device__
#endif
         static void processCell( const MeshType& mesh,
                                  TraversalUserData& userData,
                                  const IndexType index,
                                  const CoordinatesType& coordinates )
         {
            //printf( "index = %d \n", index );
            typename MatrixType::MatrixRow matrixRow = userData.matrix->getRow( index );
            userData.boundaryConditions->updateLinearSystem( *userData.time,
                                                             mesh,
                                                             index,
                                                             coordinates,
                                                             *userData.u,
                                                             *userData.b,
                                                             matrixRow );
         }

#ifdef HAVE_CUDA
         __host__ __device__
#endif
         static void processFace( const MeshType& mesh,
                                  TraversalUserData& userData,
                                  const IndexType index,
                                  const CoordinatesType& coordinates )
         {
            //printf( "index = %d \n", index );
            typename MatrixType::MatrixRow matrixRow = userData.matrix->getRow( index );
            userData.boundaryConditions->updateLinearSystem( *userData.time,
                                                             mesh,
                                                             index,
                                                             coordinates,
                                                             *userData.u,
                                                             *userData.b,
                                                             matrixRow );
         }


   };

   class TraversalInteriorEntitiesProcessor
   {
      public:

#ifdef HAVE_CUDA
         __host__ __device__
#endif
         static void processCell( const MeshType& mesh,
                                  TraversalUserData& userData,
                                  const IndexType index,
                                  const CoordinatesType& coordinates )
         {
            //printf( "index = %d \n", index );
            typedef tnlFunctionAdapter< MeshType, RightHandSide > FunctionAdapter;
            ( *userData.b )[ index ] = ( *userData.u )[ index ] +
                                  ( *userData.tau ) * FunctionAdapter::getValue( mesh,
                                                             *userData.rightHandSide,
                                                             index,
                                                             coordinates,
                                                             *userData.time );
            
            typename MatrixType::MatrixRow matrixRow = userData.matrix->getRow( index );
            userData.differentialOperator->updateLinearSystem( *userData.time,
                                                               *userData.tau,
                                                               mesh,
                                                               index,
                                                               coordinates,
                                                               *userData.u,
                                                               *userData.b,
                                                               matrixRow );
            userData.matrix->addElementFast( index, index, 1.0, 1.0 );
         }

#ifdef HAVE_CUDA
         __host__ __device__
#endif
         static void processFace( const MeshType& mesh,
                                  TraversalUserData& userData,
                                  const IndexType index,
                                  const CoordinatesType& coordinates )
         {
            //printf( "index = %d \n", index );
            typedef tnlFunctionAdapter< MeshType, RightHandSide > FunctionAdapter;
            ( *userData.b )[ index ] = ( *userData.u )[ index ] +
                                  ( *userData.tau ) * FunctionAdapter::getValue( mesh,
                                                             *userData.rightHandSide,
                                                             index,
                                                             coordinates,
                                                             *userData.time );

            typename MatrixType::MatrixRow matrixRow = userData.matrix->getRow( index );
            userData.differentialOperator->updateLinearSystem( *userData.time,
                                                               *userData.tau,
                                                               mesh,
                                                               index,
                                                               coordinates,
                                                               *userData.u,
                                                               *userData.b,
                                                               matrixRow );
            userData.matrix->addElementFast( index, index, 1.0, 1.0 );
         }
   };
};

#include <solvers/pde/tnlLinearSystemAssembler_impl.h>

#endif /* TNLLINEARSYSTEMASSEMBLER_H_ */
