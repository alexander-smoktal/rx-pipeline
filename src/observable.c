
#include "observable.h"

static void observable_destroy_gwrapper(gpointer data) {
    observable_destroy((Observable *) data);
}

void observable_init(Observable *observable) {
    if (observable) {
        observable->subscribers = g_hash_table_new_full(g_direct_hash, g_direct_equal,
                                                    NULL,
                                                    observable_destroy_gwrapper);
        observable->destroy_cb = NULL;
        observable->data = NULL;
    }
}

static void send_to_subscriber(gpointer key, gpointer value, gpointer user_data) {
    Observable *obs = (Observable *) value;

    if (obs->callback && user_data) {
        // If result of our callback non-NULL, broadcast to subscribers.
        void *result = obs->callback(obs, user_data);
        if (result) {
            observable_broadcast(obs, result);
        }
    }
}

void observable_broadcast(Observable *observable, void *data) {
    if (data) {
        g_hash_table_foreach(observable->subscribers, send_to_subscriber, data);
    }
}

bool observable_destroy(Observable *observable) {
    CHECK_NULL_RETURN(observable, false);

    if (g_hash_table_size(observable->subscribers) == 0) {
        observable->destroy_cb(observable);
        return true;
    }

    return false;
}


