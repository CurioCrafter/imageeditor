# AI integration library

# Only build if AI is enabled
if(BUILD_AI)
    # Create AI integration library
    add_library(ai-integration STATIC
        # Add source files here when they exist
        # aiservice.cpp
        # models.cpp
        # processing.cpp
    )
    
    # Set properties
    set_target_properties(ai-integration PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
    )
    
    # Include directories
    target_include_directories(ai-integration PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    # Link core engine
    target_link_libraries(ai-integration core-engine)
else()
    # Create empty library if AI is disabled
    add_library(ai-integration INTERFACE)
    target_include_directories(ai-integration INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
