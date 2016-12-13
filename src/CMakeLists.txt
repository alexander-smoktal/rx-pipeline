
set(SRC_LIST
    log.c
    loop.c
    main.c
    observable.c
    observables/observable_file.c
    observables/observable_pipe.c
    observables/observable_join.c
    observables/observable_timer.c
    pipeline.c
    utils.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES} uv)
