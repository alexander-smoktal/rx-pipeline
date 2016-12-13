#include "loop.h"
#include "utils.h"

Loop *loop_create() {
    Loop *result = malloc(sizeof(Loop));
    result->loop = uv_loop_new();

    return result;
}

bool loop_run(Loop *loop) {
    CHECK_NULL_RETURN(loop, -1);

    return uv_run(loop->loop, UV_RUN_DEFAULT);
}

void loop_close(Loop *loop) {
    if (loop) {
        uv_stop(loop->loop);
        uv_loop_delete(loop->loop);
        free(loop);
    }
}

