cmake_minimum_required(VERSION 3.0.0)
project(DataStructor VERSION 0.1.0 LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# set(CMAKE_VERBOSE_MAKEFILE ON)

add_subdirectory(1_PolynomialCalculator)
add_subdirectory(2_SqStack)
add_subdirectory(3_ExpressionClaculator)
add_subdirectory(4_AirlineBookingSystem)
# add_subdirectory(5_Tree)
add_subdirectory(6_Hfm)
add_subdirectory(7_MinimumSpanningTree)
add_subdirectory(8_ClassSchedule)
add_subdirectory(9_Navigate)
