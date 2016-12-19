#include "observable_join.h"

#include "observable_map.h"

/**
 * @brief Observable join
 */
typedef struct {
    Observable base;             /**< Parent observable structure */
    Observable *left_source;     /**< Left Observable to join */
    Observable *right_source;    /**< Right Observable to join */
    observable_join_cb callback; /**< Callback, which joins data */
} Join;


static void *left_join_callback(Observable *observable, void *data) {
    Join *join_point = (Join *) observable->data;

    void *result = join_point->callback(&join_point->base, NULL, data);
    observable_broadcast(&join_point->base, result);

    return NULL;
}

static void *right_join_callback(Observable *observable, void *data) {
    Join *join_point = (Join *) observable->data;

    void *result = join_point->callback(NULL, &join_point->base, data);
    observable_broadcast(&join_point->base, result);

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
    observable_subscribe(result->left_source, &result->base);

    result->right_source = observable_map_create(right, right_join_callback);
    result->right_source->data = result;
    observable_subscribe(result->right_source, &result->base);

    return (Observable *) result;
}
