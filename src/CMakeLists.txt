
set(SRC_LIST
    connector.c
    log.c
    loop.c
    main.c
    observable.c
    observable_pipe.c
    observable_join.c
    observable_timer.c
    pipeline.c
    utils.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES} uv)
