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
          typename Index = typename Mesh::GlobalIndexType >
class MomentumXBoundaryConditionsCavity
{

};

/****
 * Base
 */
template< typename Function >
class MomentumXBoundaryConditionsCavityBase
{
   public:
      
      typedef Function FunctionType;
      
      static void configSetup( const Config::ConfigDescription& config,
                               const String& prefix = "" )
      {
         Function::configSetup( config, prefix );
      }
      
      template< typename MeshPointer >
      bool setup( const MeshPointer& meshPointer, 
                  const Config::ParameterContainer& parameters,
                  const String& prefix = "" )
      {
         return Functions::FunctionAdapter< typename MeshPointer::ObjectType, FunctionType >::setup( this->function, meshPointer, parameters, prefix );
      }

      static void configSetup( Config::ConfigDescription& config,
                               const String& prefix = "" )
      {
         Function::configSetup( config, prefix );
      };

      bool setup( const Config::ParameterContainer& parameters,
                  const String& prefix = "" )
      {
          return this->function.setup( parameters, prefix );
      };

      void setFunction( const FunctionType& function )
      {
         this->function = function;
      };
      
      FunctionType& getFunction()
      {
         return this->function;
      }

      const FunctionType& getFunction() const
      {
         return this->function;
      };

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
class MomentumXBoundaryConditionsCavity< Meshes::Grid< 1, MeshReal, Device, MeshIndex >, Function, Real, Index >
   : public MomentumXBoundaryConditionsCavityBase< Function >,
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
   typedef Functions::MeshFunction< MeshType > MeshFunctionType;
   typedef Containers::Vector< RealType, DeviceType, IndexType> DofVectorType;
   typedef Containers::StaticVector< 1, RealType > PointType;
   typedef typename MeshType::CoordinatesType CoordinatesType;
   typedef MomentumXBoundaryConditionsCavity< MeshType, Function, Real, Index > ThisType;
   typedef MomentumXBoundaryConditionsCavityBase< Function > BaseType;
   typedef CompressibleConservativeVariables< MeshType > CompressibleConservativeVariablesType;
   typedef SharedPointer< CompressibleConservativeVariablesType > CompressibleConservativeVariablesPointer;
   typedef SharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;

   template< typename EntityType,
             typename MeshFunction >
   __cuda_callable__
   const RealType operator()( const MeshFunction& u,
                              const EntityType& entity,   
                              const RealType& time = 0 ) const
   {
      const MeshType& mesh = entity.getMesh();
      const auto& neighborEntities = entity.getNeighborEntities();
      const IndexType& index = entity.getIndex();
      if( entity.getCoordinates().x() == 0 )
         return (* this->compressibleConservativeVariables->getDensity())[neighborEntities.template getEntityIndex< 0 >()] 
              * ( (* (* this->compressibleConservativeVariables->getMomentum())[ 0 ])[neighborEntities.template getEntityIndex< 0 >()]
                / (* this->compressibleConservativeVariables->getDensity())[neighborEntities.template getEntityIndex< 0 >()] 
                + this->timestep
                );
      else
         return u[ neighborEntities.template getEntityIndex< -1 >() ];   

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
         const auto& neighborEntities = entity.getNeighborEntities();
         const IndexType& index = entity.getIndex();
         typename Matrix::MatrixRow matrixRow = matrix.getRow( index );
         if( entity.getCoordinates().x() == 0 )
         {
            matrixRow.setElement( 0, index, 1.0 );
            matrixRow.setElement( 1, neighborEntities.template getEntityIndex< 1 >(), -1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() * 
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         else
         {
            matrixRow.setElement( 0, neighborEntities.template getEntityIndex< -1 >(), -1.0 );
            matrixRow.setElement( 1, index, 1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }         
      }

      void setTimestep(const RealType timestep )
      {
         this->timestep = timestep;
      }

      void setGamma(const RealType gamma )
      {
         this->gamma = gamma;
      }

      void setCompressibleConservativeVariables(const CompressibleConservativeVariablesPointer& compressibleConservativeVariables)
      {
         this->compressibleConservativeVariables = compressibleConservativeVariables;
      }

      void setPressure(const MeshFunctionPointer& pressure)
      {
         this->pressure = pressure;
      }

      void setCavitySpeed(const RealType cavitySpeed)
      {
         this->cavitySpeed = cavitySpeed;
      }

   private:
      CompressibleConservativeVariablesPointer compressibleConservativeVariables;
      RealType timestep;
      RealType cavitySpeed;
      RealType gamma;
      MeshFunctionPointer pressure;
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
class MomentumXBoundaryConditionsCavity< Meshes::Grid< 2, MeshReal, Device, MeshIndex >, Function, Real, Index >
   : public MomentumXBoundaryConditionsCavityBase< Function >,
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
      typedef Functions::MeshFunction< MeshType > MeshFunctionType;
      typedef Containers::Vector< RealType, DeviceType, IndexType> DofVectorType;
      typedef Containers::StaticVector< 2, RealType > PointType;
      typedef typename MeshType::CoordinatesType CoordinatesType;
      typedef MomentumXBoundaryConditionsCavity< MeshType, Function, Real, Index > ThisType;
      typedef MomentumXBoundaryConditionsCavityBase< Function > BaseType;
      typedef CompressibleConservativeVariables< MeshType > CompressibleConservativeVariablesType;
      typedef SharedPointer< CompressibleConservativeVariablesType > CompressibleConservativeVariablesPointer;
      typedef SharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;


      template< typename EntityType,
                typename MeshFunction >
      __cuda_callable__
      const RealType operator()( const MeshFunction& u,
                                 const EntityType& entity,                            
                                 const RealType& time = 0 ) const
      {
         const MeshType& mesh = entity.getMesh();
         const auto& neighborEntities = entity.getNeighborEntities();
         const IndexType& index = entity.getIndex();
         if( entity.getCoordinates().x() == 0 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0 >() ];
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0 >() ];
         }
         if( entity.getCoordinates().y() == 0 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0 >() ];
                 /*(* this->compressibleConservativeVariables->getDensity())[neighborEntities.template getEntityIndex< 0, 0 >()] 
              * ( 
                  (* (* this->compressibleConservativeVariables->getMomentum())[ 0 ])[neighborEntities.template getEntityIndex< 0, 1 >()]
                / (* this->compressibleConservativeVariables->getDensity())[neighborEntities.template getEntityIndex< 0, 1 >()]
                );*/
         }
         // The following line is commented to avoid compiler warning
         //if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            return (* this->compressibleConservativeVariables->getDensity())[neighborEntities.template getEntityIndex< 0, -1 >()] 
              * ( 
                   ( this->cavitySpeed/* 
                    * (
                        entity.getMesh().getDimensions().x() / 2 - std::abs( (entity.getCoordinates().x() - entity.getMesh().getDimensions().x() / 2 ) )
                      ) 
                   / ( entity.getMesh().getDimensions().x() / 2 )*/
                 )
                );
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
         const auto& neighborEntities = entity.getNeighborEntities();
         const IndexType& index = entity.getIndex();
         typename Matrix::MatrixRow matrixRow = matrix.getRow( index );
         if( entity.getCoordinates().x() == 0 )
         {
            matrixRow.setElement( 0, index,                                                1.0 );
            matrixRow.setElement( 1, neighborEntities.template getEntityIndex< 1, 0 >(), -1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            matrixRow.setElement( 0, neighborEntities.template getEntityIndex< -1, 0 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                 1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == 0 )
         {
            matrixRow.setElement( 0, index,                                                1.0 );
            matrixRow.setElement( 1, neighborEntities.template getEntityIndex< 0, 1 >(), -1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            matrixRow.setElement( 0, neighborEntities.template getEntityIndex< 0, -1 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                 1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }         
      }

      void setTimestep(const RealType timestep )
      {
         this->timestep = timestep;
      }

      void setGamma(const RealType gamma )
      {
         this->gamma = gamma;
      }

      void setCompressibleConservativeVariables(const CompressibleConservativeVariablesPointer& compressibleConservativeVariables)
      {
         this->compressibleConservativeVariables = compressibleConservativeVariables;
      }

      void setPressure(const MeshFunctionPointer& pressure)
      {
         this->pressure = pressure;
      }

      void setCavitySpeed(const RealType cavitySpeed)
      {
         this->cavitySpeed = cavitySpeed;
      }

   private:
      CompressibleConservativeVariablesPointer compressibleConservativeVariables;
      RealType timestep;
      RealType cavitySpeed;
      RealType gamma;
      MeshFunctionPointer pressure;
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
class MomentumXBoundaryConditionsCavity< Meshes::Grid< 3, MeshReal, Device, MeshIndex >, Function, Real, Index >
   : public MomentumXBoundaryConditionsCavityBase< Function >,
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
      typedef Functions::MeshFunction< MeshType > MeshFunctionType;
      typedef Containers::Vector< RealType, DeviceType, IndexType> DofVectorType;
      typedef Containers::StaticVector< 3, RealType > PointType;
      typedef typename MeshType::CoordinatesType CoordinatesType;
      typedef MomentumXBoundaryConditionsCavity< MeshType, Function, Real, Index > ThisType;
      typedef MomentumXBoundaryConditionsCavityBase< Function > BaseType;  
      typedef CompressibleConservativeVariables< MeshType > CompressibleConservativeVariablesType;
      typedef SharedPointer< CompressibleConservativeVariablesType > CompressibleConservativeVariablesPointer; 
      typedef SharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;

      template< typename EntityType,
                typename MeshFunction >
      __cuda_callable__
      const RealType operator()( const MeshFunction& u,
                                 const EntityType& entity,
                                 const RealType& time = 0 ) const
      {
         const MeshType& mesh = entity.getMesh();
         const auto& neighborEntities = entity.getNeighborEntities();
         const IndexType& index = entity.getIndex();
         if( entity.getCoordinates().x() == 0 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0, 0 >() ];
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0, 0 >() ];
         }
         if( entity.getCoordinates().y() == 0 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0, 0 >() ];
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0, 0 >() ];
         }
         if( entity.getCoordinates().z() == 0 )
         {
            return u[ neighborEntities.template getEntityIndex< 0, 0, 0 >() ];
         }
         // The following line is commented to avoid compiler warning
         //if( entity.getCoordinates().z() == entity.getMesh().getDimensions().z() - 1 )
         {
            return (* this->compressibleConservativeVariables->getDensity())[neighborEntities.template getEntityIndex< 0, 0, 0 >()] 
              * ( 
                   this->cavitySpeed
                );
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
         const auto& neighborEntities = entity.getNeighborEntities();
         const IndexType& index = entity.getIndex();
         typename Matrix::MatrixRow matrixRow = matrix.getRow( index );
         if( entity.getCoordinates().x() == 0 )
         {
            matrixRow.setElement( 0, index,                                                   1.0 );
            matrixRow.setElement( 1, neighborEntities.template getEntityIndex< 1, 0, 0 >(), -1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().x() == entity.getMesh().getDimensions().x() - 1 )
         {
            matrixRow.setElement( 0, neighborEntities.template getEntityIndex< -1, 0, 0 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                    1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().x() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == 0 )
         {
            matrixRow.setElement( 0, index,                                                   1.0 );
            matrixRow.setElement( 1, neighborEntities.template getEntityIndex< 0, 1, 0 >(), -1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().y() * 
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().y() == entity.getMesh().getDimensions().y() - 1 )
         {
            matrixRow.setElement( 0, neighborEntities.template getEntityIndex< 0, -1, 0 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                    1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().y() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().z() == 0 )
         {
            matrixRow.setElement( 0, index,                                                   1.0 );
            matrixRow.setElement( 1, neighborEntities.template getEntityIndex< 0, 0, 1 >(), -1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().z() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
         if( entity.getCoordinates().z() == entity.getMesh().getDimensions().z() - 1 )
         {
            matrixRow.setElement( 0, neighborEntities.template getEntityIndex< 0, 0, -1 >(), -1.0 );
            matrixRow.setElement( 1, index,                                                    1.0 );
            b[ index ] = entity.getMesh().getSpaceSteps().z() *
               Functions::FunctionAdapter< MeshType, FunctionType >::getValue( this->function, entity, time );
         }
      }

      void setTimestep(const RealType timestep )
      {
         this->timestep = timestep;
      }

      void setGamma(const RealType gamma )
      {
         this->gamma = gamma;
      }

      void setCompressibleConservativeVariables(const CompressibleConservativeVariablesPointer& compressibleConservativeVariables)
      {
         this->compressibleConservativeVariables = compressibleConservativeVariables;
      }

      void setPressure(const MeshFunctionPointer& pressure)
      {
         this->pressure = pressure;
      }

      void setCavitySpeed(const RealType cavitySpeed)
      {
         this->cavitySpeed = cavitySpeed;
      }

   private:
      CompressibleConservativeVariablesPointer compressibleConservativeVariables;
      RealType timestep;
      RealType cavitySpeed;
      RealType gamma;
      MeshFunctionPointer pressure;
};

template< typename Mesh,
          typename Function,
          typename Real,
          typename Index >
std::ostream& operator << ( std::ostream& str, const MomentumXBoundaryConditionsCavity< Mesh, Function, Real, Index >& bc )
{
   str << "Neumann boundary ConditionsCavity: function = " << bc.getFunction();
   return str;
}

} // namespace Operators
} // namespace TNL

