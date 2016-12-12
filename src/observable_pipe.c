#include "observable_pipe.h"

typedef struct {
    Observable base;
    Observable *source;
} Pipe;

static void pipe_unsubscribe_from(Observable *listener, Observable *subscriber) {
    g_hash_table_remove(listener->subscribers, subscriber);
    if (g_hash_table_size(listener->subscribers) == 0) {
        observable_destroy(listener);
    }
}

static void pipe_destroy_callback(Observable *observable) {
    if (g_hash_table_size(observable->subscribers) == 0) {
        Pipe *pipe = (Pipe *) observable;
        if (pipe->source) {
            pipe_unsubscribe_from(pipe->source, observable);
        }

        g_hash_table_destroy(observable->subscribers);
    }
}

Observable *observable_pipe_create(Observable *observable, observable_cb callback) {
    CHECK_NULL_RETURN(observable, NULL);

    Pipe *result = malloc(sizeof(Pipe));

    observable_init(&result->base);
    result->base.destroy_cb = pipe_destroy_callback;
    result->source = observable;
    result->base.callback = callback;
    g_hash_table_insert(observable->subscribers, result, result);

    return (Observable *) result;
}
