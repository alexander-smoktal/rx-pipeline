#include "atom.h"
#include "common.h"

#include "unistd.h"
#include "time.h"

typedef kvec_t(int) k_int_vec;

static void *random_data_generator() {
    usleep(100e3);

    return INT_TO_POINTER(rand());
}

static void *pack_10_callback(Observable *source, void *data) {
    static k_int_vec *data_array = NULL;

    if (!data_array) {
        data_array = calloc(sizeof(k_int_vec), 1);
    }

    if (kv_size(*data_array) == 10) {
        kv_clear(*data_array);
    }

    int value = POINTER_TO_INT(data);
    kv_push(int, *data_array, value);

    if (kv_size(*data_array) == 10) {
        return data_array;
    } else {
        return NULL;
    }
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

    PipelineManager *manager = pipemanager_create();

    /*
     * |---------------------|
     * | Random 100ms sensor |
     * |---------------------|
     *               |
     * |-------------------------|
     * | Pack data of 10         |
     * |-------------------------|
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
    Observable *random_sensor = observable_generator_create(random_data_generator);

    // Pakc data of 10
    Observable *pack_10 = observable_map_create(random_sensor, pack_10_callback);

    // Returns average for every group
    Observable *average = observable_map_create(pack_10, average_terminator);

    // Prints average
    Observable *average_printer = observable_map_create(average, int_logger);

    pipemanager_add_pipeline(manager, average_printer, 17);

    observable_generator_run(random_sensor);

    log_deinit();
    return 0;
}
