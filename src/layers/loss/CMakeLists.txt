# Add the source files for this directory
set_full_path(THIS_DIR_SOURCES
  categorical_accuracy.cpp
  cross_entropy.cpp
  entrywise.cpp
  l2_norm2.cpp
  mean_squared_error.cpp
  top_k_categorical_accuracy.cpp
  )

if (LBANN_HAS_CUDA)
  # Add the CUDA source files for this directory
  set_full_path(THIS_DIR_CU_SOURCES
    categorical_accuracy.cu
    cross_entropy.cu
    entrywise.cu
    l2_norm2.cu
    mean_squared_error.cu
    top_k_categorical_accuracy.cu
    )
endif ()

# Propagate the files up the tree
set(SOURCES "${SOURCES}" "${THIS_DIR_SOURCES}" PARENT_SCOPE)
set(CUDA_SOURCES "${CUDA_SOURCES}" "${THIS_DIR_CU_SOURCES}" PARENT_SCOPE)
