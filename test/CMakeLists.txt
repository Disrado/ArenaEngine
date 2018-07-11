cmake_minimum_required(VERSION 2.8)

if (NOT UNIX)
    message (FATAL_ERROR "Not Unix!")
endif ()

project(AE_Test)

set(AE_LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/../wrap/build/)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/)

include_directories(${CMAKE_SOURCE_DIR}/../wrap/src/)

link_directories(${AE_LIBRARIES_DIR})

file(GLOB HEADERS_FOR_VISUAL_TESTS
  "${SOURCE_DIR}/include/*.hpp")

file(GLOB SOURCES_FOR_VISUAL_TESTS
  "${SOURCE_DIR}/visualTestsMain.cpp")

find_library(libAE AE ${AE_LIBRARIES_DIR})

message("\nCheckings libraries ..." )
message(STATUS "   AE: " ${libAE})
message("")

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

add_executable(visualTests ${SOURCES_FOR_VISUAL_TESTS} ${HEADERS_FOR_VISUAL_TESTS})

target_link_libraries(visualTests AE)

