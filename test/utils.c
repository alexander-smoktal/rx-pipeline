#include "utils.h"

/**
 * @brief Pipe. Broadcasts pushed data.
 */
typedef struct {
    Observable base;
} Pipe;

void pipe_destroy_cb(Observable *observable) {
    Pipe *pipe = (Pipe *) observable;

    observable_deinit(observable);

    free(pipe);
}

Observable *observable_pipe_create() {
    Pipe *result = malloc(sizeof(Pipe));
    observable_init(&result->base);
    result->base.destroy_cb = pipe_destroy_cb;

    return (Observable *) result;
}

void observable_pipe_push(Observable *pipe, void *data) {
    observable_broadcast(pipe, data);
}


