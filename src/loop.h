#ifndef _LOOP_H_
#define _LOOP_H_

#include "atom.h"

typedef struct Loop Loop;
typedef struct Stream Stream;

typedef void (*src_read_cb)(ssize_t size, char *buf);

Loop *loop_create(void);
int loop_run(Loop *loop);
Stream *loop_tty_create(Loop *loop, src_read_cb callback);
Stream *loop_file_create(Loop *loop, const char *path, src_read_cb callback);
void loop_stream_chutdown(Stream *stream);

#endif // _LOOP_H_
