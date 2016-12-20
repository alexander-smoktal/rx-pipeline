#include "observable_proxy.h"

/**
 * @brief Proxy. Source observable. Broadcasts pushed data.
 */
typedef struct {
    Observable base; /**< Parent observable structure */
} Proxy;

static void proxy_destroy_cb(Observable *observable) {
    Proxy *proxy = (Proxy *) observable;

    observable_deinit(observable);

    free(proxy);
}

Observable *observable_proxy_create() {
    Proxy *result = malloc(sizeof(Proxy));
    observable_init(&result->base);
    result->base.destroy_cb = proxy_destroy_cb;

    return (Observable *) result;
}

void observable_proxy_push(Observable *proxy, Buffer data) {
    observable_broadcast(proxy, data);
}
