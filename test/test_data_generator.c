#include "atom.h"
#include "utils.h"

static int sum = 2;

static Buffer data_generator(Observable *observable) {
    static int counter = 1;

    g_assert_cmpint(sum, ==, counter * 2);
    ++counter;

    if (counter >= 5) {
        observable_destroy(observable);
    }

    sum = counter * 2;

    return buffer_no_data();
}

static void test_data_generator() {
    Observable *generator = observable_generator_create(data_generator);

    observable_generator_run(generator);

    g_assert_cmpint(sum, ==, 10);
}

int main(int argc, char *argv[]) {
    g_test_init(&argc, &argv, NULL);
    log_init();

    g_test_add_func("/observers/test_data_generator", test_data_generator);

    int run_result = g_test_run();
    log_deinit();

    return run_result;
}
