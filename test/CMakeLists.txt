
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
include_directories(${GLIB2_INCLUDE_DIRS} "/usr/include")

# First let's make object library from atom sources, to test
list(REMOVE_ITEM ATOM_SOURCES main.c)
foreach(SOURCE ${ATOM_SOURCES})
    set(OBJECT_SOURCES "../src/${SOURCE}" ${OBJECT_SOURCES})
endforeach()
# Add utils
list(APPEND OBJECT_SOURCES "utils.c")
add_library(atom_object_library OBJECT ${OBJECT_SOURCES})

include_directories(../src)

# Global test
set(TEST_SRC
    test_join_observer
    test_map_observer)

# Libuv tests
if(ENABLE_LIBUV)
    set(TEST_SRC ${TEST_SRC}
        test_file_observer
        test_timer_observer
        test_udp_socket_observer)
endif()

foreach(SRC ${TEST_SRC})
    add_executable(${SRC} "${SRC}" $<TARGET_OBJECTS:atom_object_library>)
    target_link_libraries(${SRC} ${GLIB2_LIBRARIES} ${ATOM_LIBS})
    add_test(${SRC} "${SRC}" COMMAND "$<TARGET_FILE:${SRC}>")
endforeach()
