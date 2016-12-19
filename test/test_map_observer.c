#include "atom.h"
#include "utils.h"

static int sum = 0;
static int mul = 1;

static void *sum_callback(Observable *observable, void *data) {
    sum += GPOINTER_TO_INT(data);

    return NULL;
}

static void *mul_callback(Observable *observable, void *data) {
    mul *= GPOINTER_TO_INT(data);

    return NULL;
}

static void test_map_observer() {
    Observable *source = observable_pipe_create();
    Observable *sum_map = observable_map_create(source, sum_callback);
    Observable *mul_map = observable_map_create(source, mul_callback);

    g_assert_cmpint(sum, ==, 0);
    g_assert_cmpint(mul, ==, 1);

    observable_pipe_push(source, GINT_TO_POINTER(42));
    g_assert_cmpint(sum, ==, 42);
    g_assert_cmpint(mul, ==, 42);

    observable_pipe_push(source, GINT_TO_POINTER(21));
    g_assert_cmpint(sum, ==, 63);
    g_assert_cmpint(mul, ==, 882);

    observable_pipe_push(source, GINT_TO_POINTER(-121));
    g_assert_cmpint(sum, ==, -58);
    g_assert_cmpint(mul, ==, -106722);

    observable_pipe_push(source, GINT_TO_POINTER(1));
    g_assert_cmpint(sum, ==, -57);
    g_assert_cmpint(mul, ==, -106722);

    observable_destroy(sum_map);
    observable_destroy(mul_map);
}

int main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  log_init();

  g_test_add_func ("/observers/test_map_observer", test_map_observer);

  int run_result = g_test_run();
  log_deinit();

  return run_result;
}
