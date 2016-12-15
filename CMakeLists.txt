project(riot-atom)
cmake_minimum_required(VERSION 2.8)

find_package (PkgConfig REQUIRED)

pkg_check_modules(GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS} "/usr/include")

find_library(LIBUV NAMES uv libuv)
if(LIBUV)
    message(STATUS "Found libuv: " ${LIBUV})
else()
    message(FATAL_ERROR "Can't find libuv")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

if(NOT DEFINED CMAKE_BUILD_TYPE)
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
