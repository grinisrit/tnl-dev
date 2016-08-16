/***************************************************************************
                          IdentityOperator.h  -  description
                             -------------------
    begin                : Nov 17, 2014
    copyright            : (C) 2014 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */


#pragma once

#include <TNL/Functions/FunctionAdapter.h>

namespace TNL {
namespace Operators {

template< typename Mesh,
          typename Function,
          typename Real = typename Mesh::RealType,
          typename Index = typename Mesh::IndexType >
class NeumannBoundaryConditions
{

};

/****
 * Base
 */
template< typename Function >
class NeumannBoundaryConditionsBase
{
   public:
      
      typedef Function FunctionType;

      static void configSetup( Config::ConfigDescription& config,
                               const String& prefix = "" );

      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" );

      void setFunction( const FunctionType& function );
      
      FunctionType& getFunction();

      const FunctionType& getFunction() const;

   protected:

      FunctionType function;

};

/****
 * 1D grid
 */
template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Function,
          typename Real,
          typename Index >
class NeumannBoundaryConditions< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, Function, Real, Index >
   : public NeumannBoundaryConditionsBase< Function >,
     public Operator< Meshes::Grid< 1, MeshReal, Device, MeshIndex >,
                         Functions::MeshBoundaryDomain,
                         1, 1,
                         Real,
                         Index >
{
   public:

   typedef Meshes::Grid< 1, MeshReal, Device, MeshIndex > MeshType;
   typedef Real RealType;
   typedef Device DeviceType;
   typedef Index IndexType;

   typedef Function FunctionType;
   typedef Containers::Vector< RealType, DeviceType, IndexType> DofVectorType;
   typedef Containers::StaticVector< 1, RealType > VertexType;
   typedef typename MeshType::CoordinatesType CoordinatesType;
   typedef NeumannBoundaryConditions< MeshType, Function, Real, Index > ThisType;
   typedef NeumannBoundaryConditionsBase< Function > BaseType;

   template< typename EntityType,
             typename MeshFunction >
   __cuda_callable__
   const RealType operator()( const MeshFunction& u,
                              const EntityType& entity,   
                              const RealType& time = 0 ) const
   {
      const MeshType& mesh = entity.getMesh();
      const auto& neighbourEntities = entity.getNeighbourEntities();
      const IndexType& index = entity.getIndex();
      if( entity.getCoordinates().x() == 0 )
         return u[ neighbourEntities.template getEntityIndex< 1 >() ] - entity.getMesh().getSpaceSteps().x() * 
            Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
      else
         return u[ neighbourEntities.template getEntityIndex< -1 >() ] + entity.getMesh().getSpaceSteps().x() * 
            Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );   

   }


   template< typename EntityType >
   __cuda_callable__
   Index getLinearSystemRowLength( const MeshType& mesh,
                                   const IndexType& index,
                                   const EntityType& entity ) const
   {
      return 2;
   }

      template< typename PreimageFunction,
                typename MeshEntity,
                typename Matrix,
                typename Vector >
      __cuda_callable__
      void setMatrixElements( const PreimageFunction& u,
                                     const MeshEntity& entity,
                                     const RealType& time,
                                     const RealType& tau,
                                     Matrix& matrix,
                                     Vector& b ) const
      {
         const auto& neighbourEntities = entity.getNeighbourEntities();
         const IndexType& index = entity.getIndex();
         typename Matrix::MatrixRow matrixRow = matrix.getRow( index );
         if( entity.getCoordinates().x() == 0 )
         {
            matrixRow.setElement( 0, index, 1.0 );
            matrixRow.setElement( 1, neighbourEntities.template getEntityIndex< 1 >(), -1.0 );
            b[ index ] = - entity.getMesh().getSpaceSteps().x() * 
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         else
         {
            matrixRow.setElement( 0, neighbourEntities.template getEntityIndex< -1 >(), -1.0 );
            matrixRow.setElement( 1, index, 1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }         
      }
};

/****
 * 2D grid
 */
template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Function,
          typename Real,
          typename Index >
class NeumannBoundaryConditions< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, Function, Real, Index >
   : public NeumannBoundaryConditionsBase< Function >,
     public Operator< Meshes::Grid< 2, MeshReal, Device, MeshIndex >,
                         Functions::MeshBoundaryDomain,
                         2, 2,
                         Real,
                         Index >

{
   public:

      typedef Meshes::Grid< 2, MeshReal, Device, MeshIndex > MeshType;
      typedef Real RealType;
      typedef Device DeviceType;
      typedef Index IndexType;

      typedef Function FunctionType;
      typedef Containers::Vector< RealType, DeviceType, IndexType> DofVectorType;
      typedef Containers::StaticVector< 2, RealType > VertexType;
      typedef typename MeshType::CoordinatesType CoordinatesType;
      typedef NeumannBoundaryConditions< MeshType, Function, Real, Index > ThisType;
      typedef NeumannBoundaryConditionsBase< Function > BaseType;


      template< typename EntityType,
                typename MeshFunction >
      __cuda_callable__
      const RealType operator()( const MeshFunction& u,
                                 const EntityType& entity,                            
                                 const RealType& time = 0 ) const
      {
         const MeshType& mesh = entity.getMesh();
         const auto& neighbourEntities = entity.getNeighbourEntities();
         const IndexType& index = entity.getIndex();
         if( entity.getCoordinates().x() == 0 )
         {
            return u[ neighbourEntities.template getEntityIndex< 1, 0 >() ] - entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            return u[ neighbourEntities.template getEntityIndex< -1, 0 >() ] + entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == 0 )
         {
            return u[ neighbourEntities.template getEntityIndex< 0, 1 >() ] - entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            return u[ neighbourEntities.template getEntityIndex< 0, -1 >() ] + entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }         
      }

      template< typename EntityType >
      __cuda_callable__
      Index getLinearSystemRowLength( const MeshType& mesh,
                                      const IndexType& index,
                                      const EntityType& entity ) const
      {
         return 2;
      }

      template< typename PreimageFunction,
                typename MeshEntity,
                typename Matrix,
                typename Vector >
      __cuda_callable__
      void setMatrixElements( const PreimageFunction& u,
                              const MeshEntity& entity,
                              const RealType& time,
                              const RealType& tau,
                              Matrix& matrix,
                              Vector& b ) const
      {
         const auto& neighbourEntities = entity.getNeighbourEntities();
         const IndexType& index = entity.getIndex();
         typename Matrix::MatrixRow matrixRow = matrix.getRow( index );
         if( entity.getCoordinates().x() == 0 )
         {
            matrixRow.setElement( 0, index,                                                1.0 );
            matrixRow.setElement( 1, neighbourEntities.template getEntityIndex< 1, 0 >(), -1.0 );
            b[ index ] = - entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            matrixRow.setElement( 0, neighbourEntities.template getEntityIndex< -1, 0 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                 1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == 0 )
         {
            matrixRow.setElement( 0, index,                                                1.0 );
            matrixRow.setElement( 1, neighbourEntities.template getEntityIndex< 0, 1 >(), -1.0 );
            b[ index ] = - entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            matrixRow.setElement( 0, neighbourEntities.template getEntityIndex< 0, -1 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                 1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }         
      }
};

/****
 * 3D grid
 */
template< typename MeshReal,
          typename Device,
          typename MeshIndex,
          typename Function,
          typename Real,
          typename Index >
class NeumannBoundaryConditions< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, Function, Real, Index >
   : public NeumannBoundaryConditionsBase< Function >,
     public Operator< Meshes::Grid< 3, MeshReal, Device, MeshIndex >,
                         Functions::MeshBoundaryDomain,
                         3, 3,
                         Real,
                         Index >
{
   public:

      typedef Meshes::Grid< 3, MeshReal, Device, MeshIndex > MeshType;
      typedef Real RealType;
      typedef Device DeviceType;
      typedef Index IndexType;

      typedef Function FunctionType;
      typedef Containers::Vector< RealType, DeviceType, IndexType> DofVectorType;
      typedef Containers::StaticVector< 3, RealType > VertexType;
      typedef typename MeshType::CoordinatesType CoordinatesType;
      typedef NeumannBoundaryConditions< MeshType, Function, Real, Index > ThisType;
      typedef NeumannBoundaryConditionsBase< Function > BaseType;   

      template< typename EntityType,
                typename MeshFunction >
      __cuda_callable__
      const RealType operator()( const MeshFunction& u,
                                 const EntityType& entity,
                                 const RealType& time = 0 ) const
      {
         const MeshType& mesh = entity.getMesh();
         const auto& neighbourEntities = entity.getNeighbourEntities();
         const IndexType& index = entity.getIndex();
         if( entity.getCoordinates().x() == 0 )
         {
            return u[ neighbourEntities.template getEntityIndex< 1, 0, 0 >() ] - entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            return u[ neighbourEntities.template getEntityIndex< -1, 0, 0 >() ] + entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == 0 )
         {
            return u[ neighbourEntities.template getEntityIndex< 0, 1, 0 >() ] - entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            return u[ neighbourEntities.template getEntityIndex< 0, -1, 0 >() ] + entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().z() == 0 )
         {
            return u[ neighbourEntities.template getEntityIndex< 0, 0, 1 >() ] - entity.getMesh().getSpaceSteps().z() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().z() == entity.getMesh().getDimensions().z() - 1 )
         {
            return u[ neighbourEntities.template getEntityIndex< 0, 0, -1 >() ] + entity.getMesh().getSpaceSteps().z() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }   
      }


      template< typename EntityType >
      __cuda_callable__
      Index getLinearSystemRowLength( const MeshType& mesh,
                                      const IndexType& index,
                                      const EntityType& entity ) const
      {
         return 2;
      }

      template< typename PreimageFunction,
                typename MeshEntity,
                typename Matrix,
                typename Vector >
      __cuda_callable__
      void setMatrixElements( const PreimageFunction& u,
                                     const MeshEntity& entity,
                                     const RealType& time,
                                     const RealType& tau,
                                     Matrix& matrix,
                                     Vector& b ) const
      {
         const auto& neighbourEntities = entity.getNeighbourEntities();
         const IndexType& index = entity.getIndex();
         typename Matrix::MatrixRow matrixRow = matrix.getRow( index );
         if( entity.getCoordinates().x() == 0 )
         {
            matrixRow.setElement( 0, index,                                                   1.0 );
            matrixRow.setElement( 1, neighbourEntities.template getEntityIndex< 1, 0, 0 >(), -1.0 );
            b[ index ] = - entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            matrixRow.setElement( 0, neighbourEntities.template getEntityIndex< -1, 0, 0 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                    1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == 0 )
         {
            matrixRow.setElement( 0, index,                                                   1.0 );
            matrixRow.setElement( 1, neighbourEntities.template getEntityIndex< 0, 1, 0 >(), -1.0 );
            b[ index ] = - entity.getMesh().getSpaceSteps().y() * 
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            matrixRow.setElement( 0, neighbourEntities.template getEntityIndex< 0, -1, 0 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                    1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().z() == 0 )
         {
            matrixRow.setElement( 0, index,                                                   1.0 );
            matrixRow.setElement( 1, neighbourEntities.template getEntityIndex< 0, 0, 1 >(), -1.0 );
            b[ index ] = - entity.getMesh().getSpaceSteps().z() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().z() == entity.getMesh().getDimensions().z() - 1 )
         {
            matrixRow.setElement( 0, neighbourEntities.template getEntityIndex< 0, 0, -1 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                    1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().z() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
      }
};

template< typename Mesh,
          typename Function,
          typename Real,
          typename Index >
std::ostream& operator << ( std::ostream& str, const NeumannBoundaryConditions< Mesh, Function, Real, Index >& bc )
{
   str << "Neumann boundary conditions: function = " << bc.getFunction();
   return str;
}

} // namespace Operators
} // namespace TNL

#include <TNL/Operators/NeumannBoundaryConditions_impl.h>

