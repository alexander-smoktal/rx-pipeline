#ifndef _UTILS_H_
#define _UTILS_H_

#include "atom.h"

#define CHECK_NULL_RETURN(val, ret) do { \
    if (val == NULL) {                   \
        log_error("Error: %s value must be non-NULL. %s:%d", "##val", __FILE__, __LINE__);\
        return ret;                        \
    }                                      \
} while (false);

#endif // _UTILS_H_
