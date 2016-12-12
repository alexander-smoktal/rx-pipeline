#include "atom.h"

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

static Buffer timed_generator(Observable *observable, Buffer data) {
    Buffer result = {
        .data = GINT_TO_POINTER(rand() % 256),
        .size = 0
    };

    return result;
}


static Buffer sum_callback(Observable *left, Observable *right, Buffer data) {
    if (left) {
        log_error("Got left: %d", GPOINTER_TO_INT(data.data));
    } else {
        log_error("Got right timer");
    }

    return END();
}


int main(int argc, char *argv[])
{
    srand (time(NULL));
    log_init();

    Loop *loop = loop_create();

    Observable *generator = observable_timed_generator_create(loop, 100, timed_generator);
    Observable *sum_timer = observable_timer_create(loop, 1000);
    Observable *join = observable_join(generator, sum_timer, sum_callback);
    (void) join;
    (void) loop_run(loop);
    loop_close(loop);

    log_deinit();
    return 0;
}
