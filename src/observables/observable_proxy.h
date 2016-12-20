/**
 * @file observable_proxy.h
 * @date 16 Dec 2016
 * @brief Utility Proxy source observable
 */

#pragma once

#include "../observable.h"

/** @addtogroup Observable_Proxy
 *  @{
 */

/**
 * @brief observable_proxy_create Create proxy source
 * @return New observable if successfully, NULL otherwise
 */
Observable *observable_proxy_create();

/**
 * @brief observable_proxy_push Push data to proxy observable
 * @param proxy Proxy observable
 * @param data Data to push
 */
void observable_proxy_push(Observable *proxy, Buffer data);

/** @}*/
