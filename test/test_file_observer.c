#include "atom.h"

static void test_file_observer() {
    printf("Hello world\n");
}

int
main (int argc, char *argv[])
{
  g_test_init (&argc, &argv, NULL);

  g_test_add_func ("/observers/test_file_observer", test_file_observer);

  return g_test_run();
}
