/***************************************************************************
                          tnlSharedArray_impl.cu  -  description
                             -------------------
    begin                : Jan 20, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <TNL/Arrays/SharedArray.h>

namespace TNL {
namespace Arrays {

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

#ifdef HAVE_CUDA
#ifdef INSTANTIATE_FLOAT
template class tnlSharedArray< float, Devices::Cuda, int >;
#endif
template class tnlSharedArray< double, Devices::Cuda, int >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class tnlSharedArray< long double, Devices::Cuda, int >;
#endif

#ifdef INSTANTIATE_LONG_INT
#ifdef INSTANTIATE_FLOAT
template class tnlSharedArray< float, Devices::Cuda, long int >;
#endif
template class tnlSharedArray< double, Devices::Cuda, long int >;
#ifdef INSTANTIATE_LONG_DOUBLE
extern template class tnlSharedArray< long double, Devices::Cuda, long int >;
#endif
#endif
#endif

#endif

} // namespace Arrays
} // namespace TNL
