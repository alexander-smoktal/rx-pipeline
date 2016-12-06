#include "atom.h"

void rd_cb(ssize_t size, char *buf) {
    buf[size] = '\0';
    log_debug("stdin: %s", buf);
}

int main(int argc, char *argv[])
{
    log_init();

    Loop *loop = loop_create();
    loop_tty_create(loop, rd_cb);
    (void) loop_run(loop);

    log_deinit();
    return 0;
}
