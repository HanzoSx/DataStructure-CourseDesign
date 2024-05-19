set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
project(Hfm)
add_executable(${PROJECT_NAME} main.cpp osopt.cpp hfm.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "main")

target_include_directories(${PROJECT_NAME} PUBLIC .)
