#include <uv.h>

#include "atom.h"

int main(int argc, char *argv[])
{
    init_log();

    log_debug("Hello world");

    deinit_log();
    return 0;
}
