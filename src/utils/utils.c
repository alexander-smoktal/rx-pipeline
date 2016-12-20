#include "utils.h"

void *xmalloc(unsigned size) {
    void *p = malloc(size);
    if (p == NULL) {
        log_error("Failed to allocate memory. Exiting.");
        exit(EXIT_FAILURE);
    }
    return p;
}
