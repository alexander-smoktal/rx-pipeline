#ifndef _LOOP_H_
#define _LOOP_H_

#include "atom.h"

/**
 * @brief The Loop struct. Loop abstraction above Libuv loop.
 */
struct Loop {
    uv_loop_t *loop;      // Internal Libuv loop
};

typedef struct Loop Loop;

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

#endif // _LOOP_H_
