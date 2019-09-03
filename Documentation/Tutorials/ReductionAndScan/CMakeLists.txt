IF( BUILD_CUDA )
   CUDA_ADD_EXECUTABLE( SumExample SumExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND SumExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/SumExample.out OUTPUT SumExample.out )
   CUDA_ADD_EXECUTABLE( ProductExample ProductExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND ProductExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/ProductExample.out OUTPUT ProductExample.out )
   CUDA_ADD_EXECUTABLE( ScalarProductExample ScalarProductExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND ScalarProductExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/ScalarProductExample.out OUTPUT ScalarProductExample.out )
   CUDA_ADD_EXECUTABLE( MaximumNormExample MaximumNormExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND MaximumNormExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/MaximumNormExample.out OUTPUT MaximumNormExample.out )
   CUDA_ADD_EXECUTABLE( ComparisonExample ComparisonExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND ComparisonExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/ComparisonExample.out OUTPUT ComparisonExample.out )
   CUDA_ADD_EXECUTABLE( UpdateAndResidueExample UpdateAndResidueExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND UpdateAndResidueExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/UpdateAndResidueExample.out OUTPUT UpdateAndResidueExample.out )
   CUDA_ADD_EXECUTABLE( MapReduceExample-1 MapReduceExample-1.cu )
   ADD_CUSTOM_COMMAND( COMMAND MapReduceExample-1 > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/MapReduceExample-1.out OUTPUT MapReduceExample-1.out )
   CUDA_ADD_EXECUTABLE( MapReduceExample-2 MapReduceExample-2.cu )
   ADD_CUSTOM_COMMAND( COMMAND MapReduceExample-2 > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/MapReduceExample-2.out OUTPUT MapReduceExample-2.out )
   CUDA_ADD_EXECUTABLE( MapReduceExample-3 MapReduceExample-3.cu )
   ADD_CUSTOM_COMMAND( COMMAND MapReduceExample-3 > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/MapReduceExample-3.out OUTPUT MapReduceExample-3.out )
   CUDA_ADD_EXECUTABLE( ReductionWithArgument ReductionWithArgument.cu )
   ADD_CUSTOM_COMMAND( COMMAND ReductionWithArgument > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/ReductionWithArgument.out OUTPUT ReductionWithArgument.out )
   CUDA_ADD_EXECUTABLE( ScanExample ScanExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND ScanExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/ScanExample.out OUTPUT ScanExample.out )
   CUDA_ADD_EXECUTABLE( ExclusiveScanExample ExclusiveScanExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND ExclusiveScanExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/ExclusiveScanExample.out OUTPUT ExclusiveScanExample.out )
   CUDA_ADD_EXECUTABLE( SegmentedScanExample SegmentedScanExample.cu )
   ADD_CUSTOM_COMMAND( COMMAND SegmentedScanExample > ${TNL_DOCUMENTATION_OUTPUT_SNIPPETS_PATH}/SegmentedScanExample.out OUTPUT SegmentedScanExample.out )
ENDIF()

IF( BUILD_CUDA )
ADD_CUSTOM_TARGET( TutorialsReduction-cuda ALL DEPENDS
   SumExample.out
   ProductExample.out
   ScalarProductExample.out
   MaximumNormExample.out
   ComparisonExample.out
   UpdateAndResidueExample.out
   MapReduceExample-1.out
   MapReduceExample-2.out
   MapReduceExample-3.out
   ReductionWithArgument.out
   ScanExample.out
   ExclusiveScanExample.out
   SegmentedScanExample.out )
ENDIF()