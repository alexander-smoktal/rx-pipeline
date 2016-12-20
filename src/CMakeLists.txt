
set(SRC_LIST
    log.c
    loop.c
    main.c
    observable.c
    observables/observable_file.c
    observables/observable_map.c
    observables/observable_join.c
    observables/observable_timer.c
    observables/observable_udp_socket.c
    pipeline.c
    utils/utils.c)

# For tests
set(ATOM_SOURCES ${SRC_LIST} PARENT_SCOPE)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES} uv)
