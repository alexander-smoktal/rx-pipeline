
# First let's make object library from atom sources, to test
list(REMOVE_ITEM ATOM_SOURCES main.c)
foreach(SOURCE ${ATOM_SOURCES})
    set(OBJECT_SOURCES "../src/${SOURCE}" ${OBJECT_SOURCES})
endforeach()
# Add utils
list(APPEND OBJECT_SOURCES "utils.c")
add_library(atom_object_library OBJECT ${OBJECT_SOURCES})

include_directories(../src)

# Create tests for each test source file
file(GLOB TESTS "test_*.c")
foreach(TEST ${TESTS})
  get_filename_component(EXENAME ${TEST} NAME_WE)
  add_executable(${EXENAME} ${TEST} $<TARGET_OBJECTS:atom_object_library>)
  target_link_libraries(${EXENAME} ${GLIB2_LIBRARIES} uv)
  add_test(NAME "${EXENAME}" COMMAND "$<TARGET_FILE:${EXENAME}>")
endforeach()
