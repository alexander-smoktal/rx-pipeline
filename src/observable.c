
#include "observable.h"

// Eond of data
static void* EOD = INT_TO_POINTER(0xE0D);

void *end_of_data() {
    return EOD;
}

void observable_init(Observable *observable) {
    if (observable) {
        observable->subscribers = kh_init(ptr_hash_map);
        observable->destroy_cb = NULL;
        observable->data = NULL;
    }
}

void observable_deinit(Observable *observable) {
    kh_destroy(ptr_hash_map, observable->subscribers);
}

static inline void send_to_subscriber(Observable *observable, void *data) {
    if (observable->callback && data) {
        // If result of our callback non-NULL, broadcast to subscribers.
        void *result = observable->callback(observable, data);

        if (result) {
            observable_broadcast(observable, result);
        }
    }
}

void observable_broadcast(Observable *observable, void *data) {
    if (data) {
        Observable *current_subscriber = NULL;
        kh_foreach_value(observable->subscribers, current_subscriber, {
                            send_to_subscriber(current_subscriber, data);
                         });
    }
}

void observable_subscribe(Observable *listener, Observable *subscriber) {
    int khash_res;
    unsigned int key_iter = kh_put(ptr_hash_map, listener->subscribers, POINTER_TO_INT(subscriber), &khash_res);
    kh_val(listener->subscribers, key_iter) = subscriber;
}

void observable_unsubscribe(Observable *listener, Observable *subscriber) {
    unsigned int key_iter = kh_get(ptr_hash_map, listener->subscribers, POINTER_TO_INT(subscriber));

    if (key_iter == kh_end(listener->subscribers)) {
        log_warning("Unknown subscriber is trying to unsubscribe");
    } else {
        kh_del(ptr_hash_map, listener->subscribers, key_iter);

        if (kh_size(listener->subscribers) == 0) {
            observable_destroy(listener);
        }
    }
}

bool observable_destroy(Observable *observable) {
    CHECK_NULL_RETURN(observable, false);

    // If we still have subscribers, don't delete observable.
    // It will be deleted, once last subscriber unsubscribe.
    if (kh_size(observable->subscribers) == 0) {
        observable->destroy_cb(observable);
        return true;
    }

    return false;
}


