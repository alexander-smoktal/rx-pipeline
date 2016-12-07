#include "loop.h"

// Connector type for runtime polymorphism
typedef enum {
    CT_TTY,
    CT_FILE,
    CT_SOCKET
} ConnectorType;

/**
 * @brief The Connector struct. Polymorphic connector type, to pull from multiple
 * sources with same callbacks.
 */
struct Connector {
    void *handler;        // Libuv uv_handle_t
    ConnectorType type;   // Connector type for run-time polymorphysm
    src_read_cb callback; // User callback
    void *ctxt;           // User context. May be retrieved by 'connector_get_context' inside user callback.
};

/**
 * @brief The Loop struct. Loop abstraction above Libuv loop.
 */
struct Loop {
    uv_loop_t *loop;      // Internal Libuv loop
};

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
        free(loop);
    }
}

static uv_buf_t alloc_cb(uv_handle_t *handle, size_t suggested_size) {
  return uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

static void read_cb(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
    Connector *connector = (Connector *) stream->data;

    connector->callback(connector, nread, buf.base);
    free(buf.base);
}

Connector *loop_tty_create(Loop *loop, src_read_cb callback, void *ctxt) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(callback, NULL);

    Connector *result = malloc(sizeof(Connector));
    uv_tty_t *tty = malloc(sizeof(uv_tty_t));

    if (!uv_tty_init(loop->loop, tty, 0, true)) {
        if (!uv_read_start((uv_stream_t*) tty, alloc_cb, read_cb)) {
            tty->data = result;
            result->handler = tty;
            result->callback = callback;
            result->type = CT_TTY;
            result->ctxt = ctxt;

            return result;
        } else {
            log_error("Can't start reading tty");
        }
    } else {
        log_error("Can't create tty");
    }

    free(tty);
    free(result);
    return NULL;
}

Connector *loop_file_create(Loop *loop, const char *path, src_read_cb callback, void *ctxt) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(callback, NULL);
    CHECK_NULL_RETURN(path, NULL);

    // TODO: Implement
    return NULL;
}

bool connector_shutdown(Connector *connector) {
    CHECK_NULL_RETURN(connector, false);
    bool ret = false;

    switch (connector->type) {
    case CT_TTY:
        ret = uv_read_stop(connector->handler);
    default:
        log_error("Invalid connector type");
    }

    free(connector->handler);
    free(connector);

    return ret;
}

void *connector_get_context(Connector *connector) {
    CHECK_NULL_RETURN(connector, NULL);

    return connector->ctxt;
}
