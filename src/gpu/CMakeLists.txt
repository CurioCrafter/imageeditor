# GPU acceleration library

# Only build if GPU is enabled
if(BUILD_GPU)
    # Create GPU acceleration library
    add_library(gpu-acceleration STATIC
        # Add source files here when they exist
        # renderer.cpp
        # shaders.cpp
        # textures.cpp
    )
    
    # Set properties
    set_target_properties(gpu-acceleration PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    
    # Include directories
    target_include_directories(gpu-acceleration PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    # Link GPU libraries
    target_link_libraries(gpu-acceleration
        ${Vulkan_LIBRARIES}
        ${OPENGL_LIBRARIES}
    )
    
    # Link core engine
    target_link_libraries(gpu-acceleration core-engine)
else()
    # Create empty library if GPU is disabled
    add_library(gpu-acceleration INTERFACE)
    target_include_directories(gpu-acceleration INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
