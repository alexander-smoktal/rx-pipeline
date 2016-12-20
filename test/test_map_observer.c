#include "atom.h"
#include "utils.h"

static int sum = 0;
static int mul = 1;

static Buffer sum_callback(Observable *observable, Buffer data) {
    sum += GPOINTER_TO_INT(data.data);

    return buffer_no_data();
}

static Buffer mul_callback(Observable *observable, Buffer data) {
    mul *= GPOINTER_TO_INT(data.data);

    return buffer_no_data();
}

static void test_map_observer() {
    Observable *source = observable_proxy_create();
    Observable *sum_map = observable_map_create(source, sum_callback);
    Observable *mul_map = observable_map_create(source, mul_callback);

    g_assert_cmpint(sum, ==, 0);
    g_assert_cmpint(mul, ==, 1);

    observable_proxy_push(source, buffer_create(GINT_TO_POINTER(42), sizeof(int)));
    g_assert_cmpint(sum, ==, 42);
    g_assert_cmpint(mul, ==, 42);

    observable_proxy_push(source, buffer_create(GINT_TO_POINTER(21), sizeof(int)));
    g_assert_cmpint(sum, ==, 63);
    g_assert_cmpint(mul, ==, 882);

    observable_proxy_push(source, buffer_create(GINT_TO_POINTER(-121), sizeof(int)));
    g_assert_cmpint(sum, ==, -58);
    g_assert_cmpint(mul, ==, -106722);

    observable_proxy_push(source, buffer_create(GINT_TO_POINTER(1), sizeof(int)));
    g_assert_cmpint(sum, ==, -57);
    g_assert_cmpint(mul, ==, -106722);

    observable_destroy(sum_map);
    observable_destroy(mul_map);
}

int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);
    log_init();

    g_test_add_func("/observers/test_map_observer", test_map_observer);

    int run_result = g_test_run();
    log_deinit();

    return run_result;
}
