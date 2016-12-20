/**
 * @file observable_map.h
 * @date 19 Dec 2016
 * @brief Observable data mapper
 */

#pragma once

#include "../observable.h"

/**
 * @brief observable_map_create Create map Observable
 * @param observable Observable to map
 * @param callback Callback, which maps data
 * @return New Observable if cussesfully, NULL otherwise
 */
Observable *observable_map_create(Observable *observable, observable_cb callback);
