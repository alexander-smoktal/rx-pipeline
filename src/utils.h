#pragma once

#include "log.h"

#define CHECK_NULL_RETURN(val, ret) do { \
    if (val == NULL) {                   \
        log_error("Error: %s value must be non-NULL. %s:%d", "##val", __FILE__, __LINE__);\
        return ret;                      \
    }                                    \
} while (false);
