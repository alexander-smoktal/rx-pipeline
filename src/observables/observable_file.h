#pragma once

#include "../loop.h"
#include "../observable.h"

/**
 * @brief observable_file_create Create file connector.
 * @param loop Loop, which runs observable.
 * @param path Path to file.
 * @param buffer_size Chunk size for file reads.
 * @param callback Callback to process file data.
 * @return New Observable if succesfully, NULL otherwise.
 */
Observable *observable_file_create(Loop *loop, const char *path, size_t buffer_size, observable_cb callback);
