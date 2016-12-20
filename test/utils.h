#pragma once

#include <glib.h>

#include "atom.h"

Observable *observable_pipe_create();
void observable_pipe_push(Observable *pipe, void *data);
