
set(SRC_LIST
    buffer.c
    connector.c
    log.c
    loop.c
    main.c
    observable.c
    obs_join.c
    obs_timer.c
    utils.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${GLIB2_LIBRARIES} uv)
