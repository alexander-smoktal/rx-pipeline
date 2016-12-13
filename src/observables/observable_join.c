#include "observable_join.h"

#include "observable_pipe.h"

typedef struct {
    Observable base;
    Observable *left_source;
    Observable *right_source;
    observable_join_cb callback;
} Join;


static void *left_join_callback(Observable *observable, void *data) {
    Observable *join_point = (Observable *) observable->data;

    void *result = ((Join *)join_point)->callback(join_point, NULL, data);
    observable_broadcast(join_point, result);

    return NULL;
}

static void *right_join_callback(Observable *observable, void *data) {
    Observable *join_point = (Observable *) observable->data;

    void *result = ((Join *)join_point)->callback(NULL, join_point, data);
    observable_broadcast(join_point, result);

    return NULL;
}

static void join_destroy_callback(Observable *observable) {
    Join *join = (Join *) observable;
    observable_deinit(observable);
    observable_unsubscribe_from(observable, join->left_source);
    observable_unsubscribe_from(observable, join->right_source);
}

Observable *observable_join(Observable *left, Observable *right, observable_join_cb callback) {
    CHECK_NULL_RETURN(left, NULL);
    CHECK_NULL_RETURN(right, NULL);

    Join *result = malloc(sizeof(Join));

    observable_init(&result->base);
    result->base.destroy_cb = join_destroy_callback;
    result->callback = callback;

    result->left_source = observable_pipe_create(left, left_join_callback);
    result->left_source->data = result;
    g_hash_table_insert(result->left_source->subscribers, result, result);

    result->right_source = observable_pipe_create(right, right_join_callback);
    result->right_source->data = result;
    g_hash_table_insert(result->right_source->subscribers, result, result);

    return (Observable *) result;
}
