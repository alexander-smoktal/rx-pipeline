
set(SRC_LIST
    log.c
    loop.c
    main.c
    utils.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} uv)
