#include "atom.h"
#include "common.h"

static void *sensor_handler(Observable *observable, void *data) {
    int *sensor_data = (int *) data;

    return INT_TO_POINTER(*sensor_data);
}

static void *take_random_callback(Observable *left, Observable *right, void *data) {
    static int duration = 0;
    static int last_value = 0;

    // Data
    if (left) {
        last_value = POINTER_TO_INT(data);
        return NULL;
        // Timer
    } else {
        log_error("Value at %d.%d: %d", duration / 10, duration % 10, last_value);
    }

    ++duration;
    return NULL;
}

typedef kvec_t(int) k_int_vec;

static void *sum_callback(Observable *left, Observable *right, void *data) {
    static k_int_vec *data_array = NULL;
    static bool clear = false;

    if (!data_array) {
        data_array = calloc(sizeof(k_int_vec), 1);
    }

    if (clear) {
        kv_clear(*data_array);
        clear = false;
    }

    // Data
    if (left) {
        int value = POINTER_TO_INT(data);
        kv_push(int, *data_array, value);
        return NULL;
        // Timer
    } else {
        clear = true;
        return data_array;
    }

    return NULL;
}

static void *average_terminator(Observable *observable, void *data) {
    k_int_vec *values = (k_int_vec *) data;

    int result = 0;

    for (size_t i = 0; i < kv_size(*values); ++i) {
        result += kv_A(*values, i);
    }

    return INT_TO_POINTER(result / kv_size(*values));
}

static void *int_logger(Observable *observable, void *data) {
    int value = POINTER_TO_INT(data);

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

    pipemanager_add_pipeline(manager, every_100_ms_printer, 42);
    pipemanager_add_pipeline(manager, average_printer, 17);

    (void) loop_run(loop);
    loop_close(loop);

    log_deinit();
    return 0;
}
