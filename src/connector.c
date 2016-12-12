#include "connector.h"

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
    Loop *loop;           // Uv loop to use
    void *ctxt;           // User context. May be retrieved by 'connector_get_context' inside user callback.
};


static uv_buf_t alloc_cb(uv_handle_t *handle, size_t suggested_size) {
  return uv_buf_init((char*) malloc(suggested_size), suggested_size);
}

static void libuv_tty_read_callback(uv_stream_t* stream, ssize_t nread, uv_buf_t buf) {
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
        if (!uv_read_start((uv_stream_t*) tty, alloc_cb, libuv_tty_read_callback)) {
            tty->data = result;
            result->handler = tty;
            result->callback = callback;
            result->type = CT_TTY;
            result->ctxt = ctxt;

            return result;
        } else {
            log_error("Can't start reading tty: %s", uv_strerror(uv_last_error(loop->loop)));
        }
    } else {
        log_error("Can't create tty: %s", uv_strerror(uv_last_error(loop->loop)));
    }

    free(tty);
    free(result);
    return NULL;
}

static void libuv_file_read_callback(uv_fs_t* req) {
    Connector *connector = (Connector *) req->data;
    static size_t offset = 0;

    if (req->result >= 0) {
        if (req->result == 0) {
            log_error("EOF");
        }
        connector->callback(connector, req->len, req->buf);
    } else {
        log_error("Error reading file: %s", uv_strerror(uv_last_error(connector->loop->loop)));
        connector->callback(connector, -1, NULL);
    }
    ++offset;
}

static void libuv_file_open_callback(uv_fs_t* req) {
    Connector *connector = (Connector *) req->data;

    if (req->result >= 0) {
        char *buffer = malloc(sizeof(char) * 1);
        uv_fs_read(connector->loop->loop, req, req->result, buffer, 1, -1, libuv_file_read_callback);
    } else {
        log_error("Error opening file: %s", uv_strerror(uv_last_error(connector->loop->loop)));
        connector->callback(connector, -1, NULL);
    }
}

Connector *loop_file_create(Loop *loop, const char *path, src_read_cb callback, void *ctxt) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(callback, NULL);
    CHECK_NULL_RETURN(path, NULL);

    Connector *result = malloc(sizeof(Connector));
    uv_fs_t *request = malloc(sizeof(uv_fs_t));

    if (!uv_fs_open(loop->loop, request, path, O_RDONLY, S_IRUSR, libuv_file_open_callback)) {
        request->data = result;
        result->handler = request;
        result->callback = callback;
        result->type = CT_FILE;
        result->loop = loop;
        result->ctxt = ctxt;

        return result;
    } else {
        log_error("Can't open file at %s.", path);
    }

    free(request);
    free(result);
    return NULL;
}

bool connector_shutdown(Connector *connector) {
    CHECK_NULL_RETURN(connector, false);
    bool ret = false;

    switch (connector->type) {
    case CT_TTY:
        ret = uv_read_stop(connector->handler);
    case CT_FILE:
        ret = uv_fs_close(connector->loop->loop, NULL, ((uv_fs_t *)connector->handler)->result, NULL);
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
