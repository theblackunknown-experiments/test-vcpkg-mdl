add_library(mdl_core-shared INTERFACE)
target_sources(mdl_core-shared
    PRIVATE
        example_cuda_shared.h
        example_shared.h
        texture_support_cuda.h
)
target_compile_definitions(mdl_core-shared
    INTERFACE
        MDL_CORE_LIBRARY="${MDL_CORE_LIBRARY}"
        MDL_SAMPLES_ROOT="${MDL_SAMPLES_ROOT_PATH}"
)
target_include_directories(mdl_core-shared
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
