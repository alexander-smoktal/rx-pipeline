#ifndef _OBS_JOIN_H_
#define _OBS_JOIN_H_

#include "atom.h"

typedef Buffer (*observable_join_cb)(Observable *left, Observable *right, Buffer data);

Observable *observable_join(Observable *left, Observable *right, observable_join_cb callback);

#endif // _OBS_JOIN_H_
