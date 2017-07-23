cmake_minimum_required(VERSION 2.8)

if (NOT UNIX)
    message (FATAL_ERROR "Not Unix!")
endif ()

project(ArenaEngine_Test)

set(AE_LIBRARIES_DIR ${CMAKE_SOURCE_DIR}/../build/lib/)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/)
#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Linux/)

#SGML-, TGUI-headers
include_directories(${CMAKE_SOURCE_DIR}/../include/)
include_directories(${CMAKE_SOURCE_DIR}/include/)

link_directories(${AE_LIBRARIES_DIR})

file(GLOB HEADERS
  "${SOURCE_DIR}/include/*.hpp")

file(GLOB SOURCES
  "${SOURCE_DIR}/src/*.cpp")

find_library(AE_GRAPHICS    sfml-graphics    ${AE_LIBRARIES_DIR})
find_library(AE_SYSTEM      sfml-system      ${AE_LIBRARIES_DIR})

message("\nCheckings libraries ..." )
message(STATUS "   ae_graphics: " ${AE_GRAPHICS})
message(STATUS "     ae_system: " ${AE_SYSTEM})
message("")

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall")

add_executable(AE_Test ${SOURCES} ${HEADERS})

target_link_libraries(AE_Test
  sfml-system
  sfml-graphics)
