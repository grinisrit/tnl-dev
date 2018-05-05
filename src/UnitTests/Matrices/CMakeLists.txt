IF( BUILD_CUDA )
   CUDA_ADD_EXECUTABLE( SparseMatrixCopyTest SparseMatrixCopyTest.h SparseMatrixCopyTest.cu OPTIONS ${CXX_TESTS_FLAGS} )
   TARGET_LINK_LIBRARIES( SparseMatrixCopyTest ${GTEST_BOTH_LIBRARIES}
                                                           tnl )
ELSE(  BUILD_CUDA )
   ADD_EXECUTABLE( SparseMatrixCopyTest SparseMatrixCopyTest.h SparseMatrixCopyTest.cpp )
   TARGET_COMPILE_OPTIONS( SparseMatrixCopyTest PRIVATE ${CXX_TESTS_FLAGS} )
   TARGET_LINK_LIBRARIES( SparseMatrixCopyTest ${GTEST_BOTH_LIBRARIES}
                                                           tnl )
ENDIF( BUILD_CUDA )


ADD_TEST( SparseMatrixCopyTest ${EXECUTABLE_OUTPUT_PATH}/SparseMatrixCopyTest${CMAKE_EXECUTABLE_SUFFIX} )