#include "obs_join.h"

static Buffer left_join_callback(Observable *observable, Buffer data) {
    Observable *join = (Observable *) observable->data;
    observable_join_cb join_cb = (observable_join_cb) join->data;
    return join_cb(join, NULL, data);
}

static Buffer right_join_callback(Observable *observable, Buffer data) {
    Observable *join = (Observable *) observable->data;
    observable_join_cb join_cb = (observable_join_cb) join->data;
    return join_cb(NULL, join, data);
}

/*static bool join_destroy_cb(Observable *observable) {

}*/

Observable *observable_join(Observable *left, Observable *right, observable_join_cb callback) {
    Observable *result = observable_create();
    result->data = callback;

    Observable *left_join = observable_subscribe(left, left_join_callback);
    g_hash_table_insert(left_join->subscribers, result, result);
    left_join->data = result;

    Observable *right_join = observable_subscribe(right, right_join_callback);
    g_hash_table_insert(right_join->subscribers, result, result);
    right_join->data = result;

    return result;
}
