set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
project(Navigate)

# aux_source_directory(./src SOURCES)
add_executable(${PROJECT_NAME} main.cpp) # ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main")

# target_include_directories(${PROJECT_NAME} PUBLIC include)

# target_include_directories(${PROJECT_NAME} PUBLIC /home/hanzosx/workspace/SFUI/include)
# target_link_directories(${PROJECT_NAME} PUBLIC /home/hanzosx/workspace/SFUI/bin)
# target_link_libraries(${PROJECT_NAME} PUBLIC SFUI)

# target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)

