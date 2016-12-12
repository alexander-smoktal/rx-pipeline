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


static void alloc_cb(uv_handle_t* handle,
                         size_t suggested_size,
                         uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

static void libuv_tty_read_callback(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    Connector *connector = (Connector *) stream->data;

    connector->callback(connector, nread, buf->base);
    free(buf->base);
}

Connector *loop_tty_create(Loop *loop, src_read_cb callback, void *ctxt) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(callback, NULL);

    Connector *result = malloc(sizeof(Connector));
    uv_tty_t *tty = malloc(sizeof(uv_tty_t));

    int uv_ret = uv_tty_init(loop->loop, tty, 0, true);
    if (uv_ret >= 0) {
        uv_ret = uv_read_start((uv_stream_t*) tty, alloc_cb, libuv_tty_read_callback);
        if (uv_ret >= 0) {
            tty->data = result;
            result->handler = tty;
            result->callback = callback;
            result->type = CT_TTY;
            result->ctxt = ctxt;

            return result;
        } else {
            log_error("Can't start reading tty: %s", uv_strerror(uv_ret));
        }
    } else {
        log_error("Can't create tty: %s", uv_strerror(uv_ret));
    }

    free(tty);
    free(result);
    return NULL;
}

static void libuv_file_read_callback(uv_fs_t* req) {
    Connector *connector = (Connector *) req->data;

    if (req->result >= 0) {
        // File close
        if (req->result == 0) {
            connector->callback(connector, -1, NULL);
            log_error("EOF");
        } else {
            connector->callback(connector, (int)req->result, req->bufs[0].base);
            uv_buf_t *buf = malloc(sizeof(uv_buf_t));
            uv_fs_read(req->loop, req, req->result, buf, 1, -1, libuv_file_read_callback);
        }
    } else {
        log_error("Error reading file: %s", uv_strerror(req->result));
        connector->callback(connector, -1, NULL);
    }
}

static void libuv_file_open_callback(uv_fs_t* req) {
    Connector *connector = (Connector *) req->data;

    if (req->result >= 0) {
        uv_buf_t *buf = malloc(sizeof(uv_buf_t));
        uv_fs_read(connector->loop->loop, req, req->result, buf, 1, -1, libuv_file_read_callback);
    } else {
        log_error("Error opening file: %s", uv_strerror(req->result));
        connector->callback(connector, -1, NULL);
    }
}

Connector *loop_file_create(Loop *loop, const char *path, src_read_cb callback, void *ctxt) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(callback, NULL);
    CHECK_NULL_RETURN(path, NULL);

    Connector *result = malloc(sizeof(Connector));
    uv_fs_t *request = malloc(sizeof(uv_fs_t));

    int uv_ret = uv_fs_open(loop->loop, request, path, O_RDONLY, S_IRUSR, libuv_file_open_callback);
    if (uv_ret >= 0) {
        request->data = result;
        result->handler = request;
        result->callback = callback;
        result->type = CT_FILE;
        result->loop = loop;
        result->ctxt = ctxt;

        return result;
    } else {
        log_error("Can't open file at %s: %s", path, uv_strerror(uv_ret));
    }

    free(request);
    free(result);
    return NULL;
}

bool connector_shutdown(Connector *connector) {
    CHECK_NULL_RETURN(connector, false);
    int uv_ret = 0;

    switch (connector->type) {
    case CT_TTY:
        uv_ret = uv_read_stop(connector->handler);
    case CT_FILE:
        uv_ret = uv_fs_close(connector->loop->loop, NULL, ((uv_fs_t *)connector->handler)->result, NULL);
    default:
        log_error("Invalid connector type");
    }

    free(connector->handler);
    free(connector);

    return uv_ret;
}

void *connector_get_context(Connector *connector) {
    CHECK_NULL_RETURN(connector, NULL);

    return connector->ctxt;
}
