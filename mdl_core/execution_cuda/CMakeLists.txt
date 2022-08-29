
add_library(mdl_core-execution_cuda_ptx OBJECT)
target_sources(mdl_core-execution_cuda_ptx
    PRIVATE
        example_execution_cuda.cu
        example_execution_cuda_derivatives.cu
)
target_link_libraries(mdl_core-execution_cuda_ptx
    PRIVATE
        mdl_core-shared
        unofficial::mdl::mdl_core
)
set_target_properties(mdl_core-execution_cuda_ptx
    PROPERTIES
        CUDA_PTX_COMPILATION ON
        CUDA_SEPARABLE_COMPILATION ON
)

add_executable(mdl_core-execution_cuda)
target_sources(mdl_core-execution_cuda
    PRIVATE
        example_execution_cuda.cpp
)
target_link_libraries(mdl_core-execution_cuda
    PRIVATE
        mdl_core-shared
        unofficial::mdl::mdl_core
        CUDA::cudart
        CUDA::cuda_driver
        freeimage::FreeImage
)

add_custom_command(TARGET mdl_core-execution_cuda
    PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:mdl_core-execution_cuda_ptx> $<TARGET_FILE_DIR:mdl_core-execution_cuda>
    COMMAND_EXPAND_LISTS
    VERBATIM
)

add_dependencies(mdl_core-execution_cuda mdl_core-execution_cuda_ptx)
