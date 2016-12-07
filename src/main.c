#include "atom.h"

void rd_cb(Connector *connector, ssize_t size, char *buf) {
    if (size > 0 && buf[0] == '%') {
        connector_shutdown(connector);
    } else {
        buf[size] = '\0';
        log_debug("stdin: %s", buf);
    }
}

int main(int argc, char *argv[])
{
    log_init();

    Loop *loop = loop_create();
    (void) loop_tty_create(loop, rd_cb, NULL);
    (void) loop_run(loop);

    log_deinit();
    return 0;
}
