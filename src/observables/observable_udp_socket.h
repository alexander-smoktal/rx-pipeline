#pragma once

#include "../loop.h"
#include "../observable.h"

Observable *observable_udp_socket_create(Loop *loop, unsigned short port, observable_cb callback);
