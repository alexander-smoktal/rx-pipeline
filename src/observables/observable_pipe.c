#include "observable_pipe.h"

typedef struct {
    Observable base;
    Observable *source;
} Pipe;

static void pipe_destroy_callback(Observable *observable) {
    if (g_hash_table_size(observable->subscribers) == 0) {
        observable_deinit(observable);

        Pipe *pipe = (Pipe *) observable;
        if (pipe->source) {
            observable_unsubscribe_from(pipe->source, observable);
        }
    }
}

Observable *observable_pipe_create(Observable *observable, observable_cb callback) {
    CHECK_NULL_RETURN(observable, NULL);

    Pipe *result = malloc(sizeof(Pipe));

    observable_init(&result->base);
    result->base.destroy_cb = pipe_destroy_callback;
    result->source = observable;
    result->base.callback = callback;
    observable_subscribe(observable, result);

    return (Observable *) result;
}
