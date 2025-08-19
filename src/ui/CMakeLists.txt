# UI Components Library
# This is only built when BUILD_UI=ON and Qt6 is found

if(BUILD_UI AND Qt6_FOUND)
    add_library(ui-components STATIC
        main_window.cpp
        canvas_view.cpp
        layer_panel.cpp
        tool_panel.cpp
        color_panel.cpp
        brush_panel.cpp
    )
    
    target_sources(ui-components PRIVATE ${UI_SOURCES})
    
    # Include directories
    target_include_directories(ui-components PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../core
    )
    
    # Link libraries
    target_link_libraries(ui-components
        core-engine
        Qt6::Core
        Qt6::Widgets
        Qt6::Gui
    )
    
    # Set properties
    set_target_properties(ui-components PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
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
