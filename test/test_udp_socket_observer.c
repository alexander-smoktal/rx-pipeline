#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> // execve(2)
#include <glib.h>

#include "atom.h"

static int udp_socket_test_passed;

static void aux_exec_udp_send(void) {
    execl("/bin/bash", "bash", "-c", "echo -n \"how are you today?\" > /dev/udp/localhost/3000", NULL);
    exit(EXIT_FAILURE);
}

/* FIXME: If for some reason the udp_socket_handler isn't called,
 * then the test be waiting forever. */
static void *udp_socket_handler(Observable *observable, void *data) {
    if (strcmp((char *)data, "how are you today?") == 0)
        udp_socket_test_passed = 1;

    // quit receiving udp data and stop the loop in any case
    observable_destroy(observable);
    return NULL;
}

static void check_data_receive(void) {
    Loop *loop = loop_create();

    (void) observable_udp_socket_create(loop, 3000, udp_socket_handler);

    g_test_trap_subprocess("/observables/observable_udp_socket/subprocess/aux_udp_send", 0, 0);

    loop_run(loop);
    loop_close(loop);

    g_assert_true(udp_socket_test_passed);
}

int main(int argc, char **argv) {
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/observers/observable_udp_socket/check_data_receive", check_data_receive);
    g_test_add_func("/observables/observable_udp_socket/subprocess/aux_udp_send", aux_exec_udp_send);

    return g_test_run();
}
