#include "loop.h"
#include "utils/utils.h"

Loop *loop_create() {
    Loop *result = xmalloc(sizeof(Loop));
    result->loop = xmalloc(sizeof(uv_loop_t));
    uv_loop_init(result->loop);

    return result;
}

bool loop_run(Loop *loop) {
    CHECK_NULL_RETURN(loop, -1);

    return uv_run(loop->loop, UV_RUN_DEFAULT);
}

void loop_close(Loop *loop) {
    if (loop) {
        int uv_ret = uv_loop_close(loop->loop);
        if (uv_ret == UV_EBUSY) {
            log_error("Trying to close loop with handlers");
        }
        free(loop->loop);
        free(loop);
    }
}

