
#include "observable.h"

static void observable_unsubscribe_from(Observable *listener, Observable *subscriber) {
    g_hash_table_remove(listener->subscribers, subscriber);
    if (g_hash_table_size(listener->subscribers) == 0) {
        observable_destroy(listener);
    }
}

static bool observable_destroy_callback(Observable *observable) {
    if (g_hash_table_size(observable->subscribers) != 0) {
        return false;
    }

    if (observable->source) {
        observable_unsubscribe_from(observable->source, observable);
    }

    g_hash_table_destroy(observable->subscribers);
    return true;
}

static void observable_destroy_gwrapper(gpointer data) {
    observable_destroy_callback((Observable *) data);
}

Observable *observable_create() {
    Observable *result = malloc(sizeof(Observable));

    result->subscribers = g_hash_table_new_full(g_direct_hash, g_direct_equal,
                                                NULL,
                                                observable_destroy_gwrapper);
    result->source = NULL;
    result->callback = NULL;
    result->destroy_cb = observable_destroy_callback;

    return result;
}

Observable *observable_subscribe(Observable *observable, observable_cb callback) {
    Observable *result = observable_create();

    result->source = observable;
    result->callback = callback;
    g_hash_table_insert(observable->subscribers, result, result);

    return result;
}

bool observable_destroy(Observable *observable) {
    return observable->destroy_cb(observable);
}


