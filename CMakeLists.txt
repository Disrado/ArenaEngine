cmake_minimum_required(VERSION 2.8)

if (NOT UNIX)
    message (FATAL_ERROR "Not Unix!")
endif ()

project(ArenaEngine)

set(SDL2_LIB_DIR ${CMAKE_SOURCE_DIR}/extlibs/lib/SDL2/)
set(SDL2_HEADERS_DIR ${CMAKE_SOURCE_DIR}/extlibs/include/)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src/)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/)

include_directories(${SDL2_HEADERS_DIR})
include_directories(${SOURCE_DIR})

link_directories(${SDL2_LIB_DIR}/)

file(GLOB HEADERS "${SOURCE_DIR}/AE/System/*.hpp")
file(GLOB SOURCES "${SOURCE_DIR}/AE/System/*.cpp")

find_library(libSDL2 SDL2 ${SDL2_LIB_DIR})
find_library(libSDL2_image SDL2_image ${SDL2_LIB_DIR})
find_library(libSDL2_mixer SDL2_mixer ${SDL2_LIB_DIR})
find_library(libSDL2_net SDL2_net ${SDL2_LIB_DIR})
find_library(libSDL2_ttf SDL2_ttf ${SDL2_LIB_DIR})
find_library(libSDL2_gfx SDL2_gfx ${SDL2_LIB_DIR})

message("\nCheckings libraries ..." )
message(STATUS "SDL2: " ${libSDL2})
message(STATUS "SDL2_image: " ${libSDL2_image})
message(STATUS "SDL2_mixer: " ${libSDL2_mixer})
message(STATUS "SDL2_net: " ${libSDL2_net})
message(STATUS "SDL2_ttf: " ${libSDL2_ttf})
message(STATUS "SDL2_gfx: " ${libSDL2_gfx})
message("")

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -pedantic")

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

ADD_LIBRARY(AESystem SHARED ${SOURCES} ${HEADERS})

target_link_libraries(AESystem PUBLIC 
    ${libSDL2}
    ${libSDL2_image}
    ${libSDL2_mixer}
    ${libSDL2_net}
    ${libSDL2_ttf}
    ${libSDL2_gfx}
)