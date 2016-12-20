/**
 * @file observable_join.h
 * @date 19 Dec 2016
 * @brief Observables join
 */

#pragma once

#include "../observable.h"

/**
 * @brief Callback to join observables data.
 *        Incoming data may be data buffer, or 'buffer_end_of_data()'.
 *
 *        If you want to indicate end of observable data, return 'buffer_end_of_data()'.
 *        If you don't have data to broadcast, return 'buffer_no_data()'.
 */
typedef Buffer (*observable_join_cb)(Observable *left, Observable *right, Buffer data);

/**
 * @brief observable_join Create joined Observable. Inside the callback data source
 *        Observable will point to an object. Another observable will be NULL
 * @param left Left Observable to join
 * @param right Right Observable to join
 * @param callback Callback to join data
 * @return New Observable if successfully, NULL otherwise.
 */
Observable *observable_join(Observable *left, Observable *right, observable_join_cb callback);
