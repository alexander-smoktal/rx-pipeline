#ifndef _OBS_TIMER_H_
#define _OBS_TIMER_H_

#include "atom.h"

Observable *observable_timer_create(Loop *loop, uint64_t msec);
Observable *observable_timed_generator_create(Loop *loop, uint64_t msec, observable_cb callback);

#endif // _OBS_TIMER_H_
