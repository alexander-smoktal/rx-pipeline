#include "atom.h"
#include "utils.h"

static int current_result = 0;

static void *join_callback(Observable *left, Observable *right, void *data) {
    if (left) {
        current_result += GPOINTER_TO_INT(data);
    } else {
        current_result -= GPOINTER_TO_INT(data);
    }

    return NULL;
}

static void test_join_observer() {
    Observable *left = observable_proxy_create();
    Observable *right = observable_proxy_create();
    Observable *join = observable_join(left, right, join_callback);

    g_assert_cmpint(current_result, ==, 0);

    observable_proxy_push(left, GINT_TO_POINTER(42));
    g_assert_cmpint(current_result, ==, 42);

    observable_proxy_push(right, GINT_TO_POINTER(21));
    g_assert_cmpint(current_result, ==, 21);

    observable_proxy_push(right, GINT_TO_POINTER(121));
    g_assert_cmpint(current_result, ==, -100);

    observable_proxy_push(left, GINT_TO_POINTER(1100));
    g_assert_cmpint(current_result, ==, 1000);

    observable_destroy(join);
}

int main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  log_init();

  g_test_add_func ("/observers/test_join_observer", test_join_observer);

  int run_result = g_test_run();
  log_deinit();

  return run_result;
}
