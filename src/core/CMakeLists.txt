# Core Engine Library
add_library(core-engine STATIC)

# Source files
set(CORE_SOURCES
    engine.cpp
    layer.cpp
    document.cpp
    brush_engine.cpp
    tool.cpp
)

target_sources(core-engine PRIVATE ${CORE_SOURCES})

# Include directories
target_include_directories(core-engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(core-engine
    Qt6::Core
    Qt6::Gui
)

# Set properties
set_target_properties(core-engine PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    AUTOMOC ON
)
