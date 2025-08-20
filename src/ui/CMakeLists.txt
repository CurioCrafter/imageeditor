# UI Components Library
# This is only built when BUILD_UI=ON and Qt6 is found

if(BUILD_UI AND Qt6_FOUND)
    # Use Qt's helper function to create the library with proper MOC support
    qt_add_library(ui-components STATIC
        main_window.cpp
        main_window.h
        main_window_vulkan.cpp
        main_window_vulkan.h
        canvas_view.cpp
        canvas_view.h
        canvas_widget.cpp
        canvas_widget.h
        layer_panel.cpp
        layer_panel.h
        tool_panel.cpp
        tool_panel.h
        color_panel.cpp
        color_panel.h
        brush_panel.cpp
        brush_panel.h
    )
    
    # Include directories
    target_include_directories(ui-components PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
    )
    
    # Link libraries - conditionally link to GPU module
    target_link_libraries(ui-components
        PUBLIC Qt6::Widgets
        PRIVATE core-engine
    )
    
    # Only link to GPU module if it's enabled
    if(BUILD_GPU)
        target_link_libraries(ui-components PRIVATE gpu-acceleration)
        target_compile_definitions(ui-components PRIVATE GPU_AVAILABLE=1)
    else()
        target_compile_definitions(ui-components PRIVATE GPU_AVAILABLE=0)
    endif()
    
    # Set properties
    set_target_properties(ui-components PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    # Add to global UI library list
    set(UI_LIBRARIES ${UI_LIBRARIES} ui-components PARENT_SCOPE)
    
else()
    # Create interface library when UI is disabled
    add_library(ui-components INTERFACE)
    target_include_directories(ui-components INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
