
add_executable(mdl_core-calls)
target_sources(mdl_core-calls
    PRIVATE
        example_calls.cpp
)
target_link_libraries(mdl_core-calls
    PRIVATE
        mdl_core-shared
        unofficial::mdl::mdl_core
)
