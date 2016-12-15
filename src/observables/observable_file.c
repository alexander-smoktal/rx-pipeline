#include "observable_file.h"

#include "../common.h"

typedef struct {
    Observable base;
    uv_fs_t open_req, read_req;
    uv_buf_t iov;
    int buffer[1];
} File;

static void file_destroy_callback(Observable *observable) {
    if (observable) {
        observable_deinit(observable);

        File *file = (File *) observable;
        uv_fs_t close_req;
        uv_fs_close(file->open_req.loop, &close_req, file->open_req.file, NULL);
        uv_fs_req_cleanup(&file->open_req);
        uv_fs_req_cleanup(&file->read_req);

        free(file);
    }
}

static void libuv_file_read_callback(uv_fs_t *req) {
    Observable *observable = (Observable *) req->data;
    File *file = (File *) observable;

    // Got some data
    if (req->result > 0) {
        void *data = observable->callback(observable, file->buffer);

        if (data) {
            observable_broadcast(observable, data);
        }

        uv_fs_read(req->loop, &file->read_req, file->open_req.result, &file->iov, 1, -1, libuv_file_read_callback);
        // EOF or Error
    } else {
        observable->callback(observable, GINT_TO_POINTER(0xE0D));
        file_destroy_callback(observable);

        log_error("File %s EOF or error: %s", req->path, uv_strerror(req->result));
    }
}

static void libuv_file_open_callback(uv_fs_t *req) {
    File *observable = (File *) req->data;

    if (req->result >= 0) {
        uv_fs_read(req->loop, &observable->read_req, req->result, &observable->iov, 1, -1, libuv_file_read_callback);
    } else {
        log_error("Failed to open a file '%s' due to: %s", req->path, uv_strerror(req->result));

        Observable *obs_to_pass = (Observable *) observable;
        obs_to_pass->callback(obs_to_pass, GINT_TO_POINTER(0xBAADF00D));
        file_destroy_callback(obs_to_pass);
    }
}

Observable *observable_file_create(Loop *loop, const char *path, observable_cb callback) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(path, NULL);

    File *result = malloc(sizeof(File));
    result->iov.base = (char *) result->buffer;
    result->iov.len = sizeof(result->buffer);
    result->open_req.data = result;
    result->read_req.data = result;

    observable_init(&result->base);
    result->base.destroy_cb = file_destroy_callback;
    result->base.callback = callback;

    int uv_res = uv_fs_open(loop->loop, &result->open_req, path, O_RDONLY, 0, libuv_file_open_callback);

    if (uv_res >= 0) {
        return (Observable *) result;
    }

    log_error("Error: cant create file %s observable: %s", path, uv_strerror(uv_res));
    return NULL;
}

