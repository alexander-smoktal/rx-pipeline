#include "atom.h"
#include "common.h"

#include "unistd.h"
#include "time.h"

typedef kvec_t(int) k_int_vec;

static Buffer random_data_generator() {
    usleep(100e3);

    return buffer_create(INT_TO_POINTER(rand()), sizeof(int));
}

static Buffer pack_10_callback(Observable *source, Buffer data) {
    static k_int_vec *data_array = NULL;

    if (!data_array) {
        data_array = calloc(sizeof(k_int_vec), 1);
    }

    if (kv_size(*data_array) == 10) {
        kv_clear(*data_array);
    }

    int value = POINTER_TO_INT(data.data);
    kv_push(int, *data_array, value);

    if (kv_size(*data_array) == 10) {
        return buffer_create(data_array, sizeof(int) * kv_size(*data_array));
    } else {
        return buffer_no_data();
    }
}

static Buffer average_terminator(Observable *observable, Buffer data) {
    k_int_vec *values = (k_int_vec *) data.data;

    int result = 0;

    for (size_t i = 0; i < kv_size(*values); ++i) {
        result += kv_A(*values, i);
    }

    return buffer_create(INT_TO_POINTER(result / kv_size(*values)), sizeof(int));
}

static Buffer int_logger(Observable *observable, Buffer data) {
    int value = POINTER_TO_INT(data.data);

    log_error("last average: %d", value);
    return buffer_no_data();
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
