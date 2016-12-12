#include "buffer.h"

static Buffer s_EOF = {
    .data = NULL,
    .size = 0xDEADBEEF
};

Buffer END() {
    return s_EOF;
}

Buffer *buffer_create(size_t size) {
    Buffer *result = malloc(sizeof(Buffer));
    result->data  = malloc(sizeof(char) * size);
    result->size = size;

    return result;
}

char *buffer_data(Buffer *buffer) {
    return buffer->data;
}

size_t buffer_size(Buffer *buffer) {
    return buffer->size;
}

void buffer_destroy(Buffer *buffer) {
    free(buffer->data);
    free(buffer);
}
