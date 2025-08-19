# Plugin system library

# Only build if plugins are enabled
if(BUILD_PLUGINS)
    # Create plugin system library
    add_library(plugin-system STATIC
        # Add source files here when they exist
        # plugin.cpp
        # manager.cpp
        # api.cpp
    )
    
    # Set properties
    set_target_properties(plugin-system PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    
    # Include directories
    target_include_directories(plugin-system PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    # Link core engine
    target_link_libraries(plugin-system core-engine)
else()
    # Create empty library if plugins are disabled
    add_library(plugin-system INTERFACE)
    target_include_directories(plugin-system INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
