#include <iostream>
#include <TNL/File.h>

using namespace TNL;
using namespace std;

int main()
{
   const int size = 3;
   double doubleArray[] = {  3.1415926535897932384626433,
                             2.7182818284590452353602874,
                             1.6180339887498948482045868 };
   float floatArray[ 3 ];
   int intArray[ 3 ];

   /***
    * Save the array of doubles as floats.
    */
   File file;
   file.open( "test-file.tnl", File::Mode::Out | File::Mode::Truncate );
   file.save< double, float, Devices::Host >( doubleArray, size );
   file.close();

   /***
    * Load the array of floats from the file.
    */
   file.open( "test-file.tnl", File::Mode::In );
   file.load< float, float, Devices::Host >( floatArray, size );
   file.close();

   /***
    * Load the array of floats from the file and convert them to integers.
    */
   file.open( "test-file.tnl", File::Mode::In );
   file.load< int, float, Devices::Host >( intArray, size );
   file.close();

   /***
    * Print all arrays.
    */
   std::cout.precision( 15 );
   for( int i = 0; i < size; i++ )
      std::cout << doubleArray[ i ] << " -- "
                << floatArray[ i ] << " -- "
                << intArray[ i ] << std::endl;
}


