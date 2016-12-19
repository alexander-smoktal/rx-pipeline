#include "observable_join.h"

#include "observable_map.h"

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
    observable_unsubscribe(join->left_source, observable);
    observable_unsubscribe(join->right_source, observable);
    observable_deinit(observable);

    free(join);
}

Observable *observable_join(Observable *left, Observable *right, observable_join_cb callback) {
    CHECK_NULL_RETURN(left, NULL);
    CHECK_NULL_RETURN(right, NULL);

    Join *result = malloc(sizeof(Join));

    observable_init(&result->base);
    result->base.destroy_cb = join_destroy_callback;
    result->callback = callback;

    result->left_source = observable_map_create(left, left_join_callback);
    result->left_source->data = result;
    observable_subscribe(result->left_source, (Observable *) result);

    result->right_source = observable_map_create(right, right_join_callback);
    result->right_source->data = result;
    observable_subscribe(result->right_source, (Observable *) result);

    return (Observable *) result;
}
