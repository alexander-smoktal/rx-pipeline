#pragma once

#include "../log.h"

#include "klib/khash.h"
#include "klib/kvec.h"

#define CHECK_NULL_RETURN(val, ret) do { \
    if (val == NULL) {                   \
        log_error("Error: %s value must be non-NULL. %s:%d", "##val", __FILE__, __LINE__);\
        return ret;                      \
    }                                    \
} while (false);

// Glib conversion macros
#define POINTER_TO_INT(p) ((int)  (long) (p))
#define POINTER_TO_UINT(p) ((unsigned int) (unsigned long) (p))
#define INT_TO_POINTER(i) ((void *) (long) (i))
#define UINT_TO_POINTER(u) ((void *) (unsigned long) (u))

// KLib instances
// Pointer hash map. Must be used everywhere, to decrease memory footprint
KHASH_MAP_INIT_INT(ptr_hash_map, void *)
