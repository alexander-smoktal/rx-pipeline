#include "atom.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

static void *sensor_handler(Observable *observable, void *data) {
    int *sensor_data = (int *) data;

    return GINT_TO_POINTER(*sensor_data);
}


static void *sum_callback(Observable *left, Observable *right, void *data) {
    static GArray *data_array = NULL;
    static bool clear = false;

    if(!data_array) {
        data_array = g_array_new(false, false, sizeof(int));
    }

    if (clear) {
        g_array_free(data_array, false);
        clear = false;
    }

    if (left) {
        int value = GPOINTER_TO_INT(data);
        //log_error("Got value from sensor: %d", value);
        g_array_append_val(data_array, value);
        return NULL;
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

int main(int argc, char *argv[])
{
    srand (time(NULL));
    log_init();

    Loop *loop = loop_create();
    PipelineManager *manager = pipemanager_create();

    // Group elements for each second
    Observable *join = observable_join(observable_file_create(loop, "/dev/urandom", sensor_handler),
                                       observable_timer_create(loop, 1000, NULL),
                                       sum_callback);
    // Returns average for every group
    Observable *average = observable_pipe_create(join, average_terminator);
    // Prints int value
    Observable *printer = observable_pipe_create(average, int_logger);

    pipemanager_add_pipeline(manager, printer);

    (void) loop_run(loop);
    loop_close(loop);

    log_deinit();
    return 0;
}
