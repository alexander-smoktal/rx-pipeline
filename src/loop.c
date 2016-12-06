#include "loop.h"

typedef enum {
    ST_TTY,
    ST_FILE,
    ST_SOCKET
} StreamType;

struct Stream {
    void *handler;
    src_read_cb callback;
};

struct Loop {
    uv_loop_t *loop;
};

Loop *loop_create() {
    Loop *result = malloc(sizeof(Loop));
    result->loop = uv_loop_new();

    return result;
}

int loop_run(Loop *loop) {
    return uv_run(loop->loop, UV_RUN_DEFAULT);
}

static uv_buf_t alloc_cb(uv_handle_t *handle, size_t suggested_size) {
  return uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

static void read_cb(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
    Stream *internal_stream = (Stream *) stream->data;

    internal_stream->callback(nread, buf.base);
    free(buf.base);
}

Stream *loop_tty_create(Loop *loop, src_read_cb callback) {
    Stream *result = malloc(sizeof(Stream));
    uv_tty_t *tty = malloc(sizeof(uv_tty_t));

    if (!uv_tty_init(loop->loop, tty, 0, true)) {
        if (!uv_read_start((uv_stream_t*) tty, alloc_cb, read_cb)) {
            tty->data = result;
            result->handler = tty;
            result->callback = callback;

            return result;
        } else {
            log_error("Can't start reading tty");
        }
    }
    else {
        log_error("Can't create tty");
    }

    free(tty);
    free(result);
    return NULL;
}

Stream *loop_file_create(Loop *loop, const char *path, src_read_cb callback) {
    // TODO: Implement
    return NULL;
}

void loop_stream_chutdown(Stream *stream) {
    uv_read_stop(stream->handler);
    free(stream->handler);
    free(stream);
}

