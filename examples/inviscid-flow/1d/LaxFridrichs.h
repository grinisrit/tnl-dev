#ifndef LaxFridrichs_H
#define LaxFridrichs_H

#include <TNL/Containers/Vector.h>
#include <TNL/Meshes/Grid.h>

#include "LaxFridrichsContinuity.h"
#include "LaxFridrichsMomentum.h"
#include "LaxFridrichsEnergy.h"
#include "EulerVelGetter.h"
#include "EulerPressureGetter.h"

namespace TNL {

template< typename Mesh,
          typename Real = typename Mesh::RealType,
          typename Index = typename Mesh::IndexType >
class LaxFridrichs
{
   public:
      typedef Real RealType;
      typedef typename Mesh::DeviceType DeviceType;
      typedef Index IndexType;
      typedef Functions::MeshFunction< Mesh > MeshFunctionType;
 
      typedef LaxFridrichsContinuity< Mesh, Real, Index > Continuity;
      typedef LaxFridrichsMomentum< Mesh, Real, Index > Momentum;
      typedef LaxFridrichsEnergy< Mesh, Real, Index > Energy;
      typedef EulerVelGetter< Mesh, Real, Index > Velocity;
      typedef EulerPressureGetter< Mesh, Real, Index > Pressure;
   
};

} // namespace TNL

#endif	/* LaxFridrichs_H */