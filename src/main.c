#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <uv.h>

#include "atom.h"

static uv_buf_t alloc_cb(uv_handle_t *handle, size_t suggested_size) {
  return uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

static void read_cb(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
    char *data = buf.base;
    if (nread > 0 && data[0] == '%') {
        uv_read_stop(stream);
    }

    data[nread - 1] = '\0';
    log_debug("stdin: %s", data);
}

int main(int argc, char *argv[])
{
    log_init();

    uv_tty_t tty;
    uv_loop_t *loop = uv_default_loop();
    if (uv_tty_init(loop, &tty, 0, true)) {
        log_error("Can't open stdin");
    }

    if (uv_read_start((uv_stream_t*) &tty, alloc_cb, read_cb)) {
        log_error("Can't start reading stding")
    }

    log_debug("Hello world");

    uv_run(loop, UV_RUN_DEFAULT);
    log_deinit();
    return 0;
}
