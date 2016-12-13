#pragma once

#include "../loop.h"
#include "../observable.h"

Observable *observable_timer_create(Loop *loop, uint64_t msec, observable_cb callback);
