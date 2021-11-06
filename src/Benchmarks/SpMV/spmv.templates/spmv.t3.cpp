#include "../spmv.h"
namespace TNL {
namespace Benchmarks {
namespace SpMV {
template void dispatchBinary< double >( BenchmarkType&, const Matrices::SparseMatrix< float, Devices::Host >&, const Containers::Vector< double, Devices::Host, int >&, const String&, bool, bool );
} // namespace TNL
} // namespace Benchmarks
} // namespace SpMV
