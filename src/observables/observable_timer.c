
#include "observable_timer.h"

/**
 * Timer Timer observable
 * base Parent struct
 * timer Libuv timer handle
 */
typedef struct {
    Observable base;
    uv_timer_t *timer;
} Timer;

void libuv_close_callback(uv_handle_t* handle) {
    Timer *timer = (Timer *) handle->data;

    observable_deinit(&timer->base);
    free(timer->timer);
    free(timer);
}

static void timer_destroy_callback(Observable *observable) {
    Timer *timer = (Timer *) observable;

    uv_timer_stop(timer->timer);
    uv_close((uv_handle_t *) timer->timer, libuv_close_callback);
}

static void libuv_timer_callback(uv_timer_t *handle) {
    Observable *observable = (Observable *) handle->data;

    // Timer doesn't produce any meaningful data. Push a magic value
    observable_broadcast(observable, GINT_TO_POINTER(0xBAADF00D));
}

Observable *observable_timer_create(Loop *loop, uint64_t msec) {
    CHECK_NULL_RETURN(loop, NULL);

    Timer *result = malloc(sizeof(Timer));
    observable_init(&result->base);
    result->base.destroy_cb = timer_destroy_callback;
    result->timer = malloc(sizeof(uv_timer_t));

    int ret = uv_timer_init(loop->loop, result->timer);

    if (ret >= 0) {
        ret = uv_timer_start(result->timer, libuv_timer_callback, msec, msec);

        if (ret >= 0) {
            result->timer->data = result;
            return (Observable *) result;
        } else {
            log_error("Error starting timer: %s", uv_strerror(ret));
        }
    } else {
        log_error("Error creating timer: %s", uv_strerror(ret));
    }

    free(result->timer);
    free(result);
    return NULL;
}
