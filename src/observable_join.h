#pragma once

#include "observable.h"

typedef void *(*observable_join_cb)(Observable *left, Observable *right, void *data);

Observable *observable_join(Observable *left, Observable *right, observable_join_cb callback);
