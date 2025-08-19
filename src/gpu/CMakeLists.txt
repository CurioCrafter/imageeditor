# GPU acceleration library

# Only build if GPU is enabled
if(BUILD_GPU)
    # Create GPU acceleration library
    add_library(gpu-acceleration STATIC
        # Vulkan renderer source files
        vulkan_renderer.cpp
        vulkan_shader.cpp
        vulkan_texture.cpp
        vulkan_buffer.cpp
        vulkan_command_pool.cpp
        vulkan_sync.cpp
        # OpenGL fallback source files
        opengl_renderer.cpp
        opengl_shader.cpp
        opengl_texture.cpp
        # Common GPU utilities
        gpu_utils.cpp
        render_target.cpp
        shader_manager.cpp
    )
    
    # Set properties
    set_target_properties(gpu-acceleration PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    # Include directories
    target_include_directories(gpu-acceleration PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
    
    # Link GPU libraries - prefer Vulkan, fallback to OpenGL
    if(Vulkan_FOUND)
        target_link_libraries(gpu-acceleration Vulkan::Vulkan)
        target_compile_definitions(gpu-acceleration PRIVATE VULKAN_AVAILABLE=1)
        message(STATUS "GPU: Vulkan support enabled")
    endif()
    
    if(OpenGL_FOUND)
        target_link_libraries(gpu-acceleration OpenGL::GL)
        target_compile_definitions(gpu-acceleration PRIVATE OPENGL_AVAILABLE=1)
        message(STATUS "GPU: OpenGL support enabled")
    endif()
    
    # Link core engine
    target_link_libraries(gpu-acceleration core-engine)
else()
    # Create empty library if GPU is disabled
    add_library(gpu-acceleration INTERFACE)
    target_include_directories(gpu-acceleration INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
