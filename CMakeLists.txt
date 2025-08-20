cmake_minimum_required(VERSION 3.20)
project(AdvancedImageEditor VERSION 1.0.0 LANGUAGES CXX)

# Enable detailed automoc/autogen output for debugging
set(CMAKE_AUTOMOC_VERBOSE ON)
set(CMAKE_AUTOUIC_VERBOSE ON)
set(CMAKE_AUTORCC_VERBOSE ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Vulkan/MSVC compatibility definitions for Windows
if(WIN32)
    add_compile_definitions(
        VK_USE_PLATFORM_WIN32_KHR
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

# Enable Qt6 specific features
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build options
option(BUILD_UI "Build user interface components" ON)
option(BUILD_GPU "Build GPU acceleration components" ON)
option(BUILD_AI "Build AI integration components" OFF)
option(BUILD_PLUGINS "Build plugin system" OFF)
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Find Qt6 with specific components for Qt6.9.1 compatibility
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Gui OpenGL OpenGLWidgets Concurrent)

# Enable Qt6 specific features
qt6_standard_project_setup()

# MSVC-specific compiler flags for Qt6
if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
    # Fix for Qt6 MOC compatibility
    add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)
endif()

# GPU acceleration packages
if(BUILD_GPU)
    # Find Vulkan (preferred for modern GPU acceleration)
    find_package(Vulkan REQUIRED)
    if(Vulkan_FOUND)
        message(STATUS "Vulkan found - Modern GPU acceleration enabled")
        message(STATUS "Vulkan version: ${Vulkan_VERSION}")
    endif()
    
    # Fallback to OpenGL if Vulkan not available
    find_package(OpenGL REQUIRED)
    if(OpenGL_FOUND)
        message(STATUS "OpenGL found - Fallback GPU acceleration enabled")
    endif()
endif()

# Platform-specific package finding
if(NOT WIN32)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(STB REQUIRED stb)
        pkg_check_modules(OPENEXR REQUIRED OpenEXR)
    endif()
endif()

# Add subdirectories
add_subdirectory(src/core)

if(BUILD_UI AND Qt6_FOUND)
    add_subdirectory(src/ui)
endif()

if(BUILD_GPU)
    add_subdirectory(src/gpu)
endif()

if(BUILD_AI)
    add_subdirectory(src/ai)
endif()

if(BUILD_PLUGINS)
    add_subdirectory(src/plugins)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()

# Add tools directory
add_subdirectory(src/tools)

# Create main executable using Qt's helper function
qt_add_executable(image-editor
    src/main.cpp
    src/crash_handler.cpp
    src/crash_handler.h
)

# Set properties
set_target_properties(image-editor PROPERTIES
    WIN32_EXECUTABLE FALSE
    OUTPUT_NAME "image-editor"
)

# Link libraries - ui-components already links to gpu-acceleration and core-engine
if(BUILD_UI)
    target_link_libraries(image-editor PRIVATE ui-components)
endif()

# Link Windows crash dump libraries when building on Windows
if(WIN32)
    target_link_libraries(image-editor PRIVATE Dbghelp Shlwapi)
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "=== Build Configuration ===")
message(STATUS "BUILD_UI: ${BUILD_UI}")
message(STATUS "BUILD_GPU: ${BUILD_GPU}")
message(STATUS "BUILD_AI: ${BUILD_AI}")
message(STATUS "BUILD_PLUGINS: ${BUILD_PLUGINS}")
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
message(STATUS "BUILD_DOCS: ${BUILD_DOCS}")
message(STATUS "Qt6: ${Qt6_FOUND}")
if(BUILD_GPU)
    message(STATUS "OpenGL: ${OpenGL_FOUND}")
endif()
message(STATUS "==========================")
message(STATUS "")
