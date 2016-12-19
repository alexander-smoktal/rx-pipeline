#include "atom.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

static void *udp_socket_handler(Observable *observable, void *data) {
    // BEWARE: data may point not to null-terminated string.
    log_error("udp_sensor_handler: data: '%s'\n", (char *)data);
    return NULL;
}

static void *sensor_handler(Observable *observable, void *data) {
    int *sensor_data = (int *) data;

    return GINT_TO_POINTER(*sensor_data);
}

static void *take_random_callback(Observable *left, Observable *right, void *data) {
    static int duration = 0;
    static int last_value = 0;

    // Data
    if (left) {
        last_value = GPOINTER_TO_INT(data);
        return NULL;
        // Timer
    } else {
        log_error("Value at %d.%d: %d", duration / 10, duration % 10, last_value);
    }

    ++duration;
    return NULL;
}

static void *sum_callback(Observable *left, Observable *right, void *data) {
    static GArray *data_array = NULL;
    static bool clear = false;

    if (!data_array) {
        data_array = g_array_new(false, false, sizeof(int));
    }

    if (clear) {
        g_array_set_size(data_array, 0);
        clear = false;
    }

    // Data
    if (left) {
        int value = GPOINTER_TO_INT(data);
        g_array_append_val(data_array, value);
        return NULL;
        // Timer
    } else {
        clear = true;
        return data_array;
    }

    return NULL;
}

static void *average_terminator(Observable *observable, void *data) {
    GArray *values = (GArray *) data;

    int result = 0;

    for (size_t i = 0; i < values->len; ++i) {
        result += g_array_index(values, int, i);
    }

    return GINT_TO_POINTER(result / values->len);
}

static void *int_logger(Observable *observable, void *data) {
    int value = GPOINTER_TO_INT(data);

    log_error("last average: %d", value);
    return NULL;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    log_init();

    Loop *loop = loop_create();
    PipelineManager *manager = pipemanager_create();

    /*
     * |---------------------|  |-------------|
     * | Sensor /dev/urandom |  | Timer 100ms |
     * |---------------------|  |-------------|
     *    |            \              |
     *    |             \       |------------------------------|
     *    |              \----->| Print last value at timeeout |
     *    |                     |------------------------------|
     *    |
     * |-------------------------|  |----------|
     * | Pack data until timeout |<-| Timer 1s |
     * |-------------------------|  |----------|
     *               |
     * |---------------------------|
     * | Computer average of batch |
     * |---------------------------|
     *               |
     *     |---------------|
     *     | Print average |
     *     |---------------|
     */
    // Sensor data
    Observable *random_sensor = observable_file_create(loop, "/dev/urandom", sizeof(int), sensor_handler);

    // UDP socket sensor
    observable_udp_socket_create(loop, 3000, udp_socket_handler);


    // Prints element every 100ms
    Observable *every_100_ms_printer = observable_join(random_sensor,
                                       observable_timer_create(loop, 100),
                                       take_random_callback);

    // Group elements for each second
    Observable *each_1_second_grouper = observable_join(random_sensor,
                                        observable_timer_create(loop, 1000),
                                        sum_callback);
    // Returns average for every group
    Observable *average = observable_map_create(each_1_second_grouper, average_terminator);
    // Prints average
    Observable *average_printer = observable_map_create(average, int_logger);

    pipemanager_add_pipeline(manager, every_100_ms_printer);
    pipemanager_add_pipeline(manager, average_printer);

    (void) loop_run(loop);
    loop_close(loop);

    log_deinit();
    return 0;
}
