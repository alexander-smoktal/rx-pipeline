/**
 * @file buffer.h
 * @date 20 Dec 2016
 * @brief Data buffer
 */

#pragma once

#include "utils/utils.h"

/** @addtogroup Buffer
 *  @{
 */

/**
 * @brief Buffer Observable data Buffer
 */
typedef struct {
    void *data;    /**< Data */
    size_t size;   /**< Data size in bytes */
} Buffer;

/**
 * @brief make_buffer Create data Buffer
 * @param data Buffer data
 * @param size Buffer size
 * @return New buffer
 */
static inline Buffer buffer_create(void *data, size_t size) {
    return (Buffer) {
        .data = data, .size = size
    };
}

/**
 * @brief buffer_has_data If buffer contains data, which should be broadcasted
 * @param buffer Buffer
 * @return If we have Buffer with data
 */
static inline bool buffer_has_data(Buffer buffer) {
    return buffer.data != NULL;
}

/**
 * @brief buffer_no_data Empty buffer. If returned from user
 *        callback, we don't propagate data to observables.
 * @return Empty buffer
 */
static inline Buffer buffer_no_data() {
    return buffer_create(NULL, 0);
}

/**
 * @brief buffer_end_of_data End of observable data
 * @return EOD Buffer
 */
static inline Buffer buffer_end_of_data() {
    return buffer_create(INT_TO_POINTER(0xE0D), -1);
}

/** @}*/
