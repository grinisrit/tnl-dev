/***************************************************************************
                          NoTimeDiscretisation.h  -  description
                             -------------------
    begin                : Apr 4, 2015
    copyright            : (C) 2015 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#pragma once

#include <TNL/Devices/Cuda.h>

namespace TNL {
namespace Solvers {
namespace PDE {   

class NoTimeDiscretisation
{
    public:
 
        template< typename RealType,
                  typename IndexType,
                  typename MatrixType >
        __cuda_callable__ static void applyTimeDiscretisation( MatrixType& matrix,
                                                               RealType& b,
                                                               const IndexType index,
                                                               const RealType& u,
                                                               const RealType& tau,
                                                               const RealType& rhs )
        {
            b += rhs;
        }
};

} // namespace PDE
} // namespace Solvers
} // namespace TNL

