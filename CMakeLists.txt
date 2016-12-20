project(riot-atom)
cmake_minimum_required(VERSION 2.8)

find_package (PkgConfig REQUIRED)

if (USE_MUSL)
    message(STATUS "Using Musl compiler")
    set(CMAKE_C_COMPILER musl-gcc)
endif()

if(ENABLE_LIBUV)
    find_library(LIBUV NAMES uv libuv)
    if(LIBUV)
        message(STATUS "Found libuv: " ${LIBUV})
    else()
        message(FATAL_ERROR "Can't find libuv")
    endif()
    add_definitions(-DENABLE_LIBUV)
    set(ATOM_LIBS uv)
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -static")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -flto")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -g")

if (NOT DEFINED CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Release" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

add_subdirectory(src)

if (WITH_TESTS)
    include(CTest)
    add_subdirectory(test)
endif()
