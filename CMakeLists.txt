cmake_minimum_required(VERSION 3.15 FATAL_ERROR) 

project(test-vcpkg-mdl
    LANGUAGES
        CXX CUDA
)

include(CTest)

find_package(CUDAToolkit REQUIRED)
find_package(freeimage CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-mdl CONFIG REQUIRED)

get_target_property(MDL_CORE_LIBRARY unofficial::mdl::mdl_core IMPORTED_LOCATION)
if(NOT MDL_CORE_LIBRARY)
    get_target_property(MDL_CORE_LIBRARY unofficial::mdl::mdl_core IMPORTED_LOCATION_RELEASE)
endif()
if(NOT MDL_CORE_LIBRARY)
    get_target_property(MDL_CORE_LIBRARY unofficial::mdl::mdl_core IMPORTED_LOCATION_DEBUG)
endif()

get_target_property(MDL_SDK_LIBRARY unofficial::mdl::mdl_sdk IMPORTED_LOCATION)
if(NOT MDL_SDK_LIBRARY)
    get_target_property(MDL_SDK_LIBRARY unofficial::mdl::mdl_sdk IMPORTED_LOCATION_RELEASE)
endif()
if(NOT MDL_SDK_LIBRARY)
    get_target_property(MDL_SDK_LIBRARY unofficial::mdl::mdl_sdk IMPORTED_LOCATION_DEBUG)
endif()

message(STATUS "MDL SDK: ${MDL_CORE_LIBRARY}")
message(STATUS "MDL Core: ${MDL_SDK_LIBRARY}")

add_executable(sample-core main-core.cpp)
target_compile_definitions(sample-core
    PRIVATE
        MDL_CORE_LIBRARY="${MDL_CORE_LIBRARY}"
)
target_link_libraries(sample-core
    PRIVATE
        unofficial::mdl::mdl_core
)

add_test(NAME test_sample-core COMMAND sample-core)

add_executable(sample-sdk main-sdk.cpp)
target_compile_definitions(sample-sdk
    PRIVATE
        MDL_SDK_LIBRARY="${MDL_SDK_LIBRARY}"
)
target_link_libraries(sample-sdk
    PRIVATE
        unofficial::mdl::mdl_sdk
        unofficial::mdl::dds
        unofficial::mdl::nv_freeimage
)

add_test(NAME test_sample-sdk COMMAND sample-sdk)

cmake_path(SET MDL_SAMPLES_ROOT_PATH NORMALIZE "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "MDL_SAMPLES_ROOT: ${MDL_SAMPLES_ROOT_PATH}")

add_subdirectory(mdl_core/calls)
add_subdirectory(mdl_core/df_cuda)
add_subdirectory(mdl_core/execution_cuda)
add_subdirectory(mdl_core/shared)

# add_subdirectory(mdl_sdk/archives)                
# add_subdirectory(mdl_sdk/calls)                   
# add_subdirectory(mdl_sdk/code_gen)                
# add_subdirectory(mdl_sdk/compilation)             
# add_subdirectory(mdl_sdk/create_module)           
# add_subdirectory(mdl_sdk/df_cuda)                 
# add_subdirectory(mdl_sdk/df_native)               
# add_subdirectory(mdl_sdk/discovery)               
# add_subdirectory(mdl_sdk/dxr)                     
# add_subdirectory(mdl_sdk/execution_cuda)          
# add_subdirectory(mdl_sdk/execution_native)        
# add_subdirectory(mdl_sdk/generate_mdl_identifier) 
# add_subdirectory(mdl_sdk/instantiation)           
# add_subdirectory(mdl_sdk/mdl_browser)             
# add_subdirectory(mdl_sdk/mdle)                    
# add_subdirectory(mdl_sdk/modules)                 
# add_subdirectory(mdl_sdk/optix7)                  
# add_subdirectory(mdl_sdk/shared)                  
# add_subdirectory(mdl_sdk/start_shutdown)          
# add_subdirectory(mdl_sdk/traversal)               