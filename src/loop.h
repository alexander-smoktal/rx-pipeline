#ifndef _LOOP_H_
#define _LOOP_H_

#include "atom.h"

typedef struct Loop Loop;
typedef struct Connector Connector;

typedef void (*src_read_cb)(Connector *connector, ssize_t size, char *buf);

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
 * @param callback Callback, which will be called once data recieved.
 * @param ctxt Context, which user will be able retrieve with 'connector_get_context'
 *        inside the user callback.
 * @return Newly created connector, if succeeded, or NULL otherwise.
 */
Connector *loop_tty_create(Loop *loop, src_read_cb callback, void *ctxt);

/**
* @brief loop_file_create. Create file source, which being pulled by the loop.
* @param loop Loop, which pulls newly created source.
* @param path Filesystem path to the file.
* @param callback Callback, whihc will be called once data recieved.
* @param ctxt Context, which user will be able retrieve with 'connector_get_context'
*        inside the user callback.
* @return Newly created connector, if succeeded, or NULL otherwise.
*/
Connector *loop_file_create(Loop *loop, const char *path, src_read_cb callback, void *ctxt);

/**
 * @brief connector_shutdown. Shutdown a connector and free resources.
 * @param connector Connector.
 * @return If succeeded.
 */
bool connector_shutdown(Connector *connector);

/**
 * @brief connector_get_context. Get user context for a connector.
 * @param connector Connector.
 * @return User context.
 */
void *connector_get_context(Connector *connector);

#endif // _LOOP_H_