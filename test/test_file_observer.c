#include "atom.h"
#include "utils.h"

#include <unistd.h>
#include <string.h>

static const int data[2] = {42, 0xDEADBEEF};
static int read_result[2] = {0};
static int *read_pointer = read_result;

void *file_observable_callback(Observable *observable, void *data) {
    if (data != end_of_data()) {
        *(read_pointer++) = *((int *) data);
    }

    return NULL;
}

static void test_file_observer() {
    // Create file to test and fill with some data
    char tmp_template[30];
    strncpy(tmp_template, "atom_observable_file_XXXXXX", sizeof(tmp_template));

    int fd = mkstemp(tmp_template);
    g_assert(fd != 0);

    g_assert(write(fd, data, sizeof(data)));
    close(fd);

    // Create loop read
    Loop *loop = loop_create();
    (void) observable_file_create(loop, tmp_template, sizeof(int), file_observable_callback);
    loop_run(loop);
    loop_close(loop);

    g_assert_cmpint(read_result[0], ==, data[0]);
    g_assert_cmpint(read_result[1], ==, data[1]);
}

int main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);
  log_init();

  g_test_add_func ("/observers/test_file_observer", test_file_observer);

  int run_result = g_test_run();
  log_deinit();

  return run_result;
}
