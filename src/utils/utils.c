#include "utils.h"

void *xmalloc(unsigned sz) {
    void *p = malloc(sz);
    if (p == NULL) {
        log_error("Failed to allocate memory. Exiting.");
        exit(EXIT_FAILURE);
    }
    return p;
}
