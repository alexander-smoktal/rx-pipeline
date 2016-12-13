#pragma once

#include "common.h"

struct Observable;
typedef struct Observable Observable;

typedef void *(*observable_cb)(Observable *observable, void *data);
typedef void (*observable_destroy_cb)(Observable *observable);

struct Observable {
    GHashTable *subscribers;
    observable_cb callback;
    observable_destroy_cb destroy_cb;
    void *data;
};

void observable_init(Observable *observable);
void observable_deinit(Observable *observable);
void observable_broadcast(Observable *observable, void *data);
void observable_subscribe(Observable *listener, Observable *subscriber);
void observable_unsubscribe_from(Observable *listener, Observable *subscriber);
bool observable_destroy(Observable *observable);
