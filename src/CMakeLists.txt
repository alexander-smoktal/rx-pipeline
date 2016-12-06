
set(SRC_LIST
    log.c
    main.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} uv)
