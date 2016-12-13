#pragma once

#include "../observable.h"

Observable *observable_map_create(Observable *observable, observable_cb callback);
