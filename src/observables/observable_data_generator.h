/**
 * @file observable_proxy.h
 * @date 20 Dec 2016
 * @brief Data generator source observable
 */

#pragma once

#include "../observable.h"

/**
 * @brief Function, which generates Observable data
 */
typedef Buffer(*generator_fn)(Observable *observable);

/**
 * @brief observable_generator_create Create new Generator
 * @param func Function, which generates data
 * @return New Observable if successfully, NULL otherwise
 */
Observable *observable_generator_create(generator_fn func);

/**
 * @brief observable_generator_run Run data Generator
 * @param observable Generator
 */
void observable_generator_run(Observable *observable);
