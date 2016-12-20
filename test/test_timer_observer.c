#include <unistd.h>
#include <string.h>

#include "atom.h"
#include "utils.h"

static bool timer_1 = false;
static bool timer_2 = false;
static bool timer_3 = false;

void *timer_1_observable_callback(Observable *observable, void *data) {
    g_assert(!timer_1);
    g_assert(!timer_2);
    g_assert(!timer_3);
    timer_1 = true;
    observable_destroy(observable);

    return NULL;
}

void *timer_2_observable_callback(Observable *observable, void *data) {
    g_assert(timer_1);
    g_assert(!timer_2);
    g_assert(!timer_3);

    timer_2 = true;
    observable_destroy(observable);

    return NULL;
}

void *timer_3_observable_callback(Observable *observable, void *data) {
    g_assert(timer_1);
    g_assert(timer_2);
    g_assert(!timer_3);

    timer_3 = true;
    observable_destroy(observable);

    return NULL;
}

static void test_timer_observer() {
    Loop *loop = loop_create();
    (void) observable_map_create(observable_timer_create(loop, 100), timer_1_observable_callback);
    (void) observable_map_create(observable_timer_create(loop, 200), timer_2_observable_callback);
    (void) observable_map_create(observable_timer_create(loop, 300), timer_3_observable_callback);
    loop_run(loop);
    loop_close(loop);

    g_assert(timer_1);
    g_assert(timer_2);
    g_assert(timer_3);
}

int main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  log_init();

  g_test_add_func ("/observers/test_timer_observer", test_timer_observer);

  int run_result = g_test_run();
  log_deinit();

  return run_result;
}
