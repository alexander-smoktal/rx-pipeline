
#include "obs_timer.h"

static void call_subscriber(gpointer key, gpointer value, gpointer user_data) {
    Observable *obs = (Observable *) value;
    Buffer *data = (Buffer *) user_data;

    if (obs->callback) {
        if (data) {
            obs->callback(obs, *data);
        } else {
            obs->callback(obs, END());
        }
    }
}

static void libuv_timer_callback(uv_timer_t* handle, int status) {
    Observable *obs = (Observable *) handle->data;

    if (obs->callback) {
        Buffer result = obs->callback(obs, END());
        g_hash_table_foreach (obs->subscribers, call_subscriber, &result);
    } else {
        g_hash_table_foreach (obs->subscribers, call_subscriber, NULL);
    }
}

Observable *observable_timer_create(Loop *loop, uint64_t msec) {
    CHECK_NULL_RETURN(loop, NULL);

    Observable *result = observable_create();
    uv_timer_t *timer = malloc(sizeof(uv_timer_t));

    if (uv_timer_init(loop->loop, timer) >= 0) {
        if (uv_timer_start(timer, libuv_timer_callback, msec, msec) >= 0) {
            timer->data = result;
            result->data = timer;
            return result;
        } else {
            log_error("Error starting timer: %s", uv_strerror(uv_last_error(loop->loop)));
        }
    } else {
        log_error("Error creating timer: %s", uv_strerror(uv_last_error(loop->loop)));
    }

    return NULL;
}

Observable *observable_timed_generator_create(Loop *loop, uint64_t msec, observable_cb callback) {
    CHECK_NULL_RETURN(loop, NULL);

    Observable *result = observable_create();
    uv_timer_t *timer = malloc(sizeof(uv_timer_t));

    if (uv_timer_init(loop->loop, timer) >= 0) {
        if (uv_timer_start(timer, libuv_timer_callback, msec, msec) >= 0) {
            timer->data = result;
            result->data = timer;
            result->callback = callback;
            return result;
        } else {
            log_error("Error starting timer: %s", uv_strerror(uv_last_error(loop->loop)));
        }
    } else {
        log_error("Error creating timer: %s", uv_strerror(uv_last_error(loop->loop)));
    }

    return NULL;
}
