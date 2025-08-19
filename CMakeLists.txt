cmake_minimum_required(VERSION 3.20)
project(AdvancedImageEditor VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Build options
option(BUILD_UI "Build user interface components" OFF)
option(BUILD_GPU "Build GPU acceleration components" OFF)
option(BUILD_AI "Build AI integration components" OFF)
option(BUILD_PLUGINS "Build plugin system" OFF)
option(BUILD_TESTS "Build test suite" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# Find packages
find_package(Qt6 COMPONENTS Core Widgets Gui QUIET)
if(Qt6_FOUND)
    message(STATUS "Qt6 found - UI support available")
    
    # MSVC-specific compiler flags for Qt6
    if(MSVC)
        add_compile_options(/Zc:__cplusplus /permissive-)
    endif()
else()
    message(STATUS "Qt6 not found - UI support unavailable")
endif()

# UI is now enabled with MSVC 2022 support
set(BUILD_UI ON)
message(STATUS "UI enabled with MSVC 2022 support")

# GPU acceleration packages
if(BUILD_GPU)
    find_package(Vulkan QUIET)
    find_package(OpenGL QUIET)
    if(Vulkan_FOUND)
        message(STATUS "Vulkan found - GPU acceleration enabled")
    endif()
    if(OpenGL_FOUND)
        message(STATUS "OpenGL found - fallback rendering available")
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

# Add tools directory (placeholder for now)
add_subdirectory(src/tools)

# Create main executable
add_executable(image-editor src/main.cpp)

# Set properties
set_target_properties(image-editor PROPERTIES
    WIN32_EXECUTABLE FALSE
    OUTPUT_NAME "image-editor"
)

# Link libraries
target_link_libraries(image-editor core-engine)

if(BUILD_UI)
    target_link_libraries(image-editor ui-components)
endif()

if(BUILD_GPU)
    if(Vulkan_FOUND)
        target_link_libraries(image-editor Vulkan::Vulkan)
    endif()
    if(OpenGL_FOUND)
        target_link_libraries(image-editor OpenGL::GL)
    endif()
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
    message(STATUS "Vulkan: ${Vulkan_FOUND}")
    message(STATUS "OpenGL: ${OpenGL_FOUND}")
endif()
message(STATUS "==========================")
message(STATUS "")
