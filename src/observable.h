#ifndef _OBSERVABLE_H_
#define _OBSERVABLE_H_

#include "atom.h"

struct Observable;
typedef struct Observable Observable;
typedef struct Buffer Buffer;

typedef Buffer (*observable_cb)(Observable *observable, Buffer data);
typedef bool (*observable_destroy_cb)(Observable *observable);

#include "obs_timer.h"
#include "obs_join.h"

struct Observable {
    GHashTable *subscribers;
    Observable *source;
    observable_cb callback;
    observable_destroy_cb destroy_cb;
    void *data;
};

Observable *observable_create(void);
Observable *observable_subscribe(Observable *observable, observable_cb);
bool observable_destroy(Observable *observable);

#endif // _OBSERVABLE_H_
