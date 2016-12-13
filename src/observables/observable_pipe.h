#pragma once

#include "../observable.h"

Observable *observable_pipe_create(Observable *observable, observable_cb callback);
