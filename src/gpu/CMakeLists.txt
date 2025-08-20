# GPU acceleration library

# Only build if GPU is enabled
if(BUILD_GPU)
    # Use Qt's helper function to create the library with proper MOC support
    qt_add_library(gpu-acceleration STATIC
        # Vulkan renderer source files
        vulkan_renderer.cpp
        vulkan_renderer.h
        vulkan_window.cpp
        vulkan_window.h
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
    
    # Enable verbose autogen output for this target
    set_property(TARGET gpu-acceleration PROPERTY AUTOGEN_VERBOSE ON)
    
    # Include directories
    target_include_directories(gpu-acceleration PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
    
    # Link Qt6 libraries (required for QVulkanWindow, QVulkanInstance)
    target_link_libraries(gpu-acceleration
        PUBLIC Qt6::Gui        # QVulkanWindow lives here
        PRIVATE core-engine    # if the renderer needs access to core-engine
    )
    
    # Link GPU libraries - prefer Vulkan, fallback to OpenGL
    if(Vulkan_FOUND)
        target_link_libraries(gpu-acceleration PRIVATE Vulkan::Vulkan)
        target_compile_definitions(gpu-acceleration PRIVATE VULKAN_AVAILABLE=1)
        message(STATUS "GPU: Vulkan support enabled")
    endif()
    
    if(OpenGL_FOUND)
        target_link_libraries(gpu-acceleration PRIVATE OpenGL::GL)
        target_compile_definitions(gpu-acceleration PRIVATE OPENGL_AVAILABLE=1)
        message(STATUS "GPU: OpenGL support enabled")
    endif()
    
    # Add Vulkan/MSVC compatibility definitions for Windows
    if(WIN32)
        target_compile_definitions(gpu-acceleration PRIVATE
            VK_USE_PLATFORM_WIN32_KHR
            WIN32_LEAN_AND_MEAN
            NOMINMAX
        )
    endif()
else()
    # Create empty library if GPU is disabled
    add_library(gpu-acceleration INTERFACE)
    target_include_directories(gpu-acceleration INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
endif()
