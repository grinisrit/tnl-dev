/***************************************************************************
                          tnlSharedArray_impl.cpp  -  description
                             -------------------
    begin                : Mar 18, 2013
    copyright            : (C) 2013 by Tomas Oberhuber
    email                : tomas.oberhuber@fjfi.cvut.cz
 ***************************************************************************/

/* See Copyright Notice in tnl/Copyright */

#include <core/arrays/tnlSharedArray.h>

namespace TNL {

#ifdef TEMPLATE_EXPLICIT_INSTANTIATION

#ifdef INSTANTIATE_FLOAT
template class tnlSharedArray< float, tnlHost, int >;
#endif
template class tnlSharedArray< double, tnlHost, int >;
#ifdef INSTANTIATE_LONG_DOUBLE
template class tnlSharedArray< long double, tnlHost, int >;
#endif

#ifdef INSTANTIATE_LONG_INT
#ifdef INSTANTIATE_FLOAT
template class tnlSharedArray< float, tnlHost, long int >;
#endif
template class tnlSharedArray< double, tnlHost, long int >;
#ifdef INSTANTIATE_LONG_DOUBLE
template class tnlSharedArray< long double, tnlHost, long int >;
#endif
#endif

/*#ifdef HAVE_CUDA
#ifdef INSTANTIATE_FLOAT
template class tnlSharedArray< float, tnlCuda, int >;
#endif
template class tnlSharedArray< double, tnlCuda, int >;
#ifdef INSTANTIATE_LONG_DOUBLE
template class tnlSharedArray< long double, tnlCuda, int >;
#endif

#ifdef INSTANTIATE_LONG_INT
#ifdef INSTANTIATE_FLOAT
template class tnlSharedArray< float, tnlCuda, long int >;
#endif
template class tnlSharedArray< double, tnlCuda, long int >;
#ifdef INSTANTIATE_LONG_DOUBLE
template class tnlSharedArray< long double, tnlCuda, long int >;
#endif
#endif
#endif*/

#endif

} // namespace TNL


