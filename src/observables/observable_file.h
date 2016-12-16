#pragma once

#include "../loop.h"
#include "../observable.h"

Observable *observable_file_create(Loop *loop, const char *path, size_t buffer_size, observable_cb callback);
