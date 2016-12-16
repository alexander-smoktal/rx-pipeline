/**
 * @file observable_timer.h
 * @date 16 Dec 2016
 * @brief Timer observable
 */

#pragma once

#include "../loop.h"
#include "../observable.h"

/**
 * @brief observable_timer_create Create timer observable
 * @param loop Loop, which runs timer
 * @param msec Timeout in milliseconds
 * @return New Observable if successfully, NULL otherwise
 */
Observable *observable_timer_create(Loop *loop, uint64_t msec);
