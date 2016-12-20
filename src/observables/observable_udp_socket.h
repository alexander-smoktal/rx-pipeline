/**
 * @file observable_udp_socket.h
 * @data 16 Dec 2016
 * @brief udp socket observable
 */

#pragma once

#include "../loop.h"
#include "../observable.h"

/**
 * @brief observable_udp_socket_create Create observable udp socket
 * @param loop Loop, which runs timer
 * @param port Port on which to listen to
 * @param callback which to call when the data arrives
 * @return New Observable if successfully, NULL otherwise
 */
Observable *observable_udp_socket_create(Loop *loop, unsigned short port, observable_cb callback);
