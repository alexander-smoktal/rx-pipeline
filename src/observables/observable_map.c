#include "observable_map.h"

typedef struct {
    Observable base;
    Observable *source;
} Map;

static void map_destroy_callback(Observable *observable) {
    if (g_hash_table_size(observable->subscribers) == 0) {
        observable_deinit(observable);

        Map *map = (Map *) observable;
        if (map->source) {
            observable_unsubscribe(map->source, observable);
        }
    }
}

Observable *observable_map_create(Observable *observable, observable_cb callback) {
    CHECK_NULL_RETURN(observable, NULL);

    Map *result = malloc(sizeof(Map));

    observable_init(&result->base);
    result->base.destroy_cb = map_destroy_callback;
    result->source = observable;
    result->base.callback = callback;
    observable_subscribe(observable, (Observable *) result);

    return (Observable *) result;
}
