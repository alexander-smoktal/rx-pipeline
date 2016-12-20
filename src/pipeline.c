#include "pipeline.h"

#include "atom.h"

/**
 * @brief Pipeline manager. Can be used to store and search
 *        pipelines for given constraints
 */
struct PipelineManager {
    khash_t(ptr_hash_map) *pipelines; /**< Active pipelines */
};

PipelineManager *pipemanager_create() {
    PipelineManager *result = xmalloc(sizeof(PipelineManager));

    result->pipelines = kh_init(ptr_hash_map);

    return result;
}

void pipemanager_add_pipeline(PipelineManager *manager, Observable *pipe, int id) {
    int khash_res;

    unsigned int key_iter = kh_put(ptr_hash_map, manager->pipelines, id, &khash_res);
    kh_val(manager->pipelines, key_iter) = manager;
}

void pipemanager_make_pipeline(PipelineManager *manager,
                               Observable *source,
                               unsigned int pipecount, ...) {
    va_list arglist;
    Observable *pipe = source;

    va_start(arglist, pipecount);

    for (unsigned int i = 0; i < pipecount; ++i) {
        pipe = observable_map_create(pipe, va_arg(arglist, observable_cb));
    }

    pipemanager_add_pipeline(manager, pipe, POINTER_TO_INT(pipe));
}

void pipemanager_destroy(PipelineManager *manager) {
    Observable *current_pipe = NULL;
    kh_foreach_value(manager->pipelines, current_pipe, {
                        observable_destroy(current_pipe);
                     });
}

