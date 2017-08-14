cmake_minimum_required(VERSION 2.8)

if (NOT UNIX)
    message (FATAL_ERROR "Not Unix!")
endif ()

project(AE_Test)

set(AE_LIBRARIES_DIR    ${CMAKE_SOURCE_DIR}/../build/lib/)
set(GTEST_LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/gtest/lib/)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/)

#SGML, TGUI, GTest
include_directories(${CMAKE_SOURCE_DIR}/../include/)
include_directories(${CMAKE_SOURCE_DIR}/src/TestCase/)
include_directories(${CMAKE_SOURCE_DIR}/gtest/include/)

link_directories(${AE_LIBRARIES_DIR})
link_directories(${GTEST_LIBRARIES_DIR})

file(GLOB HEADERS_FOR_UNIT_TESTS
  "${SOURCE_DIR}/include/*.hpp")

file(GLOB HEADERS_FOR_VISUAL_TESTS
  "${SOURCE_DIR}/include/*.hpp")

file(GLOB SOURCES_FOR_UNIT_TESTS
  "${SOURCE_DIR}/TestCase/*.cpp"
  "${SOURCE_DIR}/unitTestsMain.cpp")

file(GLOB SOURCES_FOR_VISUAL_TESTS
  "${SOURCE_DIR}/visualTestsMain.cpp")

find_library(AE_GRAPHICS sfml-graphics ${AE_LIBRARIES_DIR})
find_library(AE_SYSTEM   sfml-system   ${AE_LIBRARIES_DIR})
find_library(AE_WINDOW   sfml-window   ${AE_LIBRARIES_DIR})
find_library(GTEST_MAIN  gtest_main    ${GTEST_LIBRARIES_DIR})
find_library(GTEST       gtest         ${GTEST_LIBRARIES_DIR})

message("\nCheckings libraries ..." )
message(STATUS "   ae_graphics: " ${AE_GRAPHICS})
message(STATUS "     ae_system: " ${AE_SYSTEM})
message(STATUS "     ae_window: " ${AE_WINDOW})
message(STATUS "    gtest_main: " ${GTEST_MAIN})
message(STATUS "         gtest: " ${GTEST})
message("")

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

add_executable(unitTests ${SOURCES_FOR_UNIT_TESTS} ${HEADERS_FOR_UNIT_TESTS})

add_executable(visualTests ${SOURCES_FOR_VISUAL_TESTS} ${HEADERS_FOR_VISUAL_TESTS})

target_link_libraries(unitTests
  sfml-system
  sfml-window
  sfml-graphics
  gtest_main
  gtest)

enable_testing()

add_test(unitTests unitTests)

target_link_libraries(visualTests
  sfml-system
  sfml-window
  sfml-graphics)

