#include <TNL/Functions/FunctionAdapter.h>

#include "1DDensityBoundaryCondition.h"
#include "1DMomentumXBoundaryCondition.h"
#include "1DMomentumYBoundaryCondition.h"
#include "1DEnergyBoundaryCondition.h"

namespace TNL {

template< typename Mesh,
          typename Function,
          typename Real = typename Mesh::RealType,
          typename Index = typename Mesh::IndexType >
class BoundaryConditions
{
   public:
      typedef Mesh MeshType;
      typedef Real RealType;
      typedef Index IndexType;
      typedef Function FunctionType;
      typedef Functions::MeshFunction< Mesh > MeshFunctionType;
      typedef typename Mesh::DeviceType DeviceType;

      typedef TNL::Operators::DensityBoundaryConditions< MeshType, FunctionType, RealType, IndexType > DensityBoundaryConditionsType;
      typedef TNL::Operators::MomentumXBoundaryConditions< MeshType, FunctionType, RealType, IndexType > MomentumXBoundaryConditionsType;
      typedef TNL::Operators::MomentumYBoundaryConditions< MeshType, FunctionType, RealType, IndexType > MomentumYBoundaryConditionsType;
      typedef TNL::Operators::EnergyBoundaryConditions< MeshType, FunctionType, RealType, IndexType > EnergyBoundaryConditionsType;
      typedef CompressibleConservativeVariables< MeshType > CompressibleConservativeVariablesType;

      typedef SharedPointer< DensityBoundaryConditionsType > DensityBoundaryConditionsTypePointer;
      typedef SharedPointer< MomentumXBoundaryConditionsType > MomentumXBoundaryConditionsTypePointer;
      typedef SharedPointer< MomentumYBoundaryConditionsType > MomentumYBoundaryConditionsTypePointer;
      typedef SharedPointer< EnergyBoundaryConditionsType > EnergyBoundaryConditionsTypePointer;
      typedef SharedPointer< CompressibleConservativeVariablesType > CompressibleConservativeVariablesPointer;
      typedef SharedPointer< MeshType > MeshPointer;
      typedef SharedPointer< MeshFunctionType, DeviceType > MeshFunctionPointer;

      static void configSetup( Config::ConfigDescription& config,
                               const String& prefix = "" )
      {
      }

      bool setup( const MeshPointer& meshPointer,
                  const Config::ParameterContainer& parameters,
                  const String& prefix = "" )
      {
         this->densityBoundaryConditionsPointer->setup( meshPointer, parameters, prefix);
         this->momentumXBoundaryConditionsPointer->setup( meshPointer, parameters, prefix);
         this->momentumYBoundaryConditionsPointer->setup( meshPointer, parameters, prefix);
         this->energyBoundaryConditionsPointer->setup( meshPointer, parameters, prefix);
         return true;
      }

      void setCompressibleConservativeVariables(const CompressibleConservativeVariablesPointer& compressibleConservativeVariables)
      {
         this->densityBoundaryConditionsPointer->setCompressibleConservativeVariables(compressibleConservativeVariables);
         this->momentumXBoundaryConditionsPointer->setCompressibleConservativeVariables(compressibleConservativeVariables);
         this->momentumYBoundaryConditionsPointer->setCompressibleConservativeVariables(compressibleConservativeVariables);
         this->energyBoundaryConditionsPointer->setCompressibleConservativeVariables(compressibleConservativeVariables);
      }

      void setTimestep(const RealType timestep)
      {
         this->densityBoundaryConditionsPointer->setTimestep(timestep);
         this->momentumXBoundaryConditionsPointer->setTimestep(timestep);
         this->momentumYBoundaryConditionsPointer->setTimestep(timestep);
         this->energyBoundaryConditionsPointer->setTimestep(timestep);   
      }

      void setGamma(const RealType gamma)
      {
         this->densityBoundaryConditionsPointer->setGamma(gamma);
         this->momentumXBoundaryConditionsPointer->setGamma(gamma);
         this->momentumYBoundaryConditionsPointer->setGamma(gamma);
         this->energyBoundaryConditionsPointer->setGamma(gamma);
      }

      void setPressure(const MeshFunctionPointer& pressure)
      {
         this->densityBoundaryConditionsPointer->setPressure(pressure);
         this->momentumXBoundaryConditionsPointer->setPressure(pressure);
         this->momentumYBoundaryConditionsPointer->setPressure(pressure);
         this->energyBoundaryConditionsPointer->setPressure(pressure);
      }

      void setCavitySpeed(const RealType cavitySpeed)
      {
         this->momentumXBoundaryConditionsPointer->setCavitySpeed(cavitySpeed);
         this->momentumYBoundaryConditionsPointer->setCavitySpeed(cavitySpeed);
         this->energyBoundaryConditionsPointer->setCavitySpeed(cavitySpeed);
      }

      DensityBoundaryConditionsTypePointer& getDensityBoundaryCondition()
      {
         return this->densityBoundaryConditionsPointer;
      }

      MomentumXBoundaryConditionsTypePointer& getMomentumXBoundaryCondition()
      {
         return this->momentumXBoundaryConditionsPointer;
      }

      MomentumYBoundaryConditionsTypePointer& getMomentumYBoundaryCondition()
      {
         return this->momentumYBoundaryConditionsPointer;
      }

      EnergyBoundaryConditionsTypePointer& getEnergyBoundaryCondition()
      {
         return this->energyBoundaryConditionsPointer;
      }


   protected:
      DensityBoundaryConditionsTypePointer densityBoundaryConditionsPointer;
      MomentumXBoundaryConditionsTypePointer momentumXBoundaryConditionsPointer;
      MomentumYBoundaryConditionsTypePointer momentumYBoundaryConditionsPointer;
      EnergyBoundaryConditionsTypePointer energyBoundaryConditionsPointer;

};

} //namespace TNL
