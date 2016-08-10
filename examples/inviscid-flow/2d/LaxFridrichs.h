#ifndef LaxFridrichs_H
#define LaxFridrichs_H

#include <TNL/Vectors/Vector.h>
#include <TNL/Meshes/Grid.h>

#include "LaxFridrichsContinuity.h"
#include "LaxFridrichsEnergy.h"
#include "LaxFridrichsMomentumX.h"
#include "LaxFridrichsMomentumY.h"
#include "EulerPressureGetter.h"
#include "EulerVelXGetter.h"
#include "EulerVelYGetter.h"
#include "EulerVelGetter.h"

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
      typedef LaxFridrichsMomentumX< Mesh, Real, Index > MomentumX;
      typedef LaxFridrichsMomentumY< Mesh, Real, Index > MomentumY;
      typedef LaxFridrichsEnergy< Mesh, Real, Index > Energy;
      typedef EulerVelXGetter< Mesh, Real, Index > VelocityX;
      typedef EulerVelYGetter< Mesh, Real, Index > VelocityY;
      typedef EulerVelGetter< Mesh, Real, Index > Velocity;
      typedef EulerPressureGetter< Mesh, Real, Index > Pressure;
   
};

} //namespace TNL

#endif	/* LaxFridrichs_H */
