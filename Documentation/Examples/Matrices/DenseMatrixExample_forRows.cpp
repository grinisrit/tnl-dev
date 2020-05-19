#include <iostream>
#include <TNL/Matrices/DenseMatrix.h>
#include <TNL/Devices/Host.h>
#include <TNL/Devices/Cuda.h>

template< typename Device >
void forRowsExample()
{
   TNL::Matrices::DenseMatrix< double, Device > matrix( 5, 5 );

   auto f = [=] __cuda_callable__ ( int rowIdx, int columnIdx, int globalIdx, double& value, bool& compute ) {
      if( rowIdx < columnIdx )
         compute = false;
      else
         value = rowIdx + columnIdx;
   };

   matrix.forRows( 0, matrix.getRows(), f );
   std::cout << matrix << std::endl;
}

int main( int argc, char* argv[] )
{
   std::cout << "Creating matrix on host: " << std::endl;
   forRowsExample< TNL::Devices::Host >();

#ifdef HAVE_CUDA
   std::cout << "Creating matrix on CUDA device: " << std::endl;
   forRowsExample< TNL::Devices::Cuda >();
#endif
}