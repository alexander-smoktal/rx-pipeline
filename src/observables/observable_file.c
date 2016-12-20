#include "observable_file.h"

#include "../common.h"

/**
 * @brief File observervable
 */
typedef struct {
    Observable base;   /**< Parent observable structure */
    uv_fs_t open_req;  /**< Libuv open request handle */
    uv_fs_t read_req;  /**< Libuv read request handle */
    uv_buf_t iov;      /**< Libuv buffer to pass for reading */
} File;

static void libuv_file_close_callback(uv_fs_t *req) {
        File *file = (File *) req->data;

        observable_deinit(&file->base);
        uv_fs_req_cleanup(&file->open_req);
        uv_fs_req_cleanup(&file->read_req);
        free(file->iov.base);

        uv_fs_req_cleanup(req);
        free(req);
        free(file);
}

static void file_destroy_callback(Observable *observable) {
    File *file = (File *) observable;
    uv_fs_t *close_req = xmalloc(sizeof(uv_fs_t));
    close_req->data = observable;

    uv_fs_close(file->open_req.loop, close_req, file->open_req.file, libuv_file_close_callback);
}

static void libuv_file_read_callback(uv_fs_t *req) {
    File *file = (File *) req->data;

    // Got some data
    if (req->result > 0) {
        void *data = file->base.callback(&file->base, file->iov.base);

        if (data) {
            observable_broadcast(&file->base, data);
        }

        uv_fs_read(req->loop, &file->read_req, file->open_req.result, &file->iov, 1, -1, libuv_file_read_callback);
        // EOF or Error
    } else {
        if (req->result < 0) {
            log_error("File %s EOF or error: %s", req->path, uv_strerror(req->result));
        }

        file->base.callback(&file->base, end_of_data());
        file_destroy_callback(&file->base);
    }
}

static void libuv_file_open_callback(uv_fs_t *req) {
    File *file = (File *) req->data;

    if (req->result >= 0) {
        uv_fs_read(req->loop, &file->read_req, req->result, &file->iov, 1, -1, libuv_file_read_callback);
    } else {
        log_error("Failed to open a file '%s' due to: %s", req->path, uv_strerror(req->result));

        file->base.callback(&file->base, end_of_data());
        file_destroy_callback(&file->base);
    }
}

Observable *observable_file_create(Loop *loop, const char *path, size_t buffer_size, observable_cb callback) {
    CHECK_NULL_RETURN(loop, NULL);
    CHECK_NULL_RETURN(path, NULL);

    File *result = xmalloc(sizeof(File));
    result->iov.base = xmalloc(buffer_size);
    result->iov.len = buffer_size;
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

