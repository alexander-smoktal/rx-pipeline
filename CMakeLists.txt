project(riot-atom)
cmake_minimum_required(VERSION 2.8)

find_package (PkgConfig REQUIRED)

if (USE_MUSL)
    message(STATUS "Using Musl compiler")
    set(CMAKE_C_COMPILER musl-gcc)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
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

add_custom_target(astyle
    COMMAND astyle --options=astylerc.sh \"src/*.c\" \"src/*.h\" \"test/*.c\" \"test/*.h\"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
