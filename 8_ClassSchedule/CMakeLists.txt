set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
project(ClassSchedule)

# Find GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

# Include directories
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

aux_source_directory(./src SOURCES)
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main")

# target_include_directories(${PROJECT_NAME} PUBLIC .)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_include_directories(${PROJECT_NAME} PUBLIC /home/hanzosx/workspace/SFUI/include)
target_link_directories(${PROJECT_NAME} PUBLIC /home/hanzosx/workspace/SFUI/bin)
target_link_libraries(${PROJECT_NAME} PUBLIC SFUI)

target_link_libraries(${PROJECT_NAME} PUBLIC sfml-graphics sfml-window sfml-system)


target_link_libraries(${PROJECT_NAME} PUBLIC ${GTK3_LIBRARIES})
