#ifndef _OBS_BUFFER_H_
#define _OBS_BUFFER_H_

#include "atom.h"

struct Buffer {
    char *data;
    size_t size;
};

typedef struct Buffer Buffer;

Buffer *buffer_create(size_t size);
char *buffer_data(Buffer *buffer);
size_t buffer_size(Buffer *buffer);
void buffer_destroy(Buffer *buffer);

Buffer END(void);

#endif // _OBS_BUFFER_H_
