#ifndef _LOOP_H_
#define _LOOP_H_

#include "atom.h"

typedef struct Loop Loop;
typedef struct Stream Stream;

typedef void (*src_read_cb)(Stream *stream, ssize_t size, char *buf);

/**
 * @brief loop_create. Create new Loop object.
 * @return Newly created Loop
 */
Loop *loop_create(void);

/**
 * @brief loop_run. Run given loop.
 * @param loop Loop.
 * @return If succesfully ran.
 */
bool loop_run(Loop *loop);

/**
 * @brief loop_close. Close given loop. Close all handles and destroy the loop.
 * @param loop Loop.
 */
void loop_close(Loop *loop);

/**
 * @brief loop_tty_create. Create TTY console source, which being pulled by the loop.
 * @param loop Loop, which pulls newly created source.
 * @param callback Callback, whihc will be called once data recieved.
 * @param ctxt Context, which user will be able retrieve with 'stream_get_context'
 *        inside the user callback.
 * @return Newly created stream, if succeeded, or NULL otherwise.
 */
Stream *loop_tty_create(Loop *loop, src_read_cb callback, void *ctxt);

/**
* @brief loop_file_create. Create file source, which being pulled by the loop.
* @param loop Loop, which pulls newly created source.
* @param path Filesystem path to the file.
* @param callback Callback, whihc will be called once data recieved.
* @param ctxt Context, which user will be able retrieve with 'stream_get_context'
*        inside the user callback.
* @return Newly created stream, if succeeded, or NULL otherwise.
*/
Stream *loop_file_create(Loop *loop, const char *path, src_read_cb callback, void *ctxt);

/**
 * @brief stream_shutdown. Shutdown the stream a free resources.
 * @param stream Stream
 * @return If succeeded.
 */
bool stream_shutdown(Stream *stream);

/**
 * @brief stream_get_context. Get user context for a stream.
 * @param stream Stream.
 * @return User context.
 */
void *stream_get_context(Stream *stream);

#endif // _LOOP_H_
