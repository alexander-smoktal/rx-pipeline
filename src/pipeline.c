#include "pipeline.h"

#include "atom.h"

struct PipelineManager {
    GHashTable *pipelines;
};

static void observable_destroy_gwrapper(gpointer data) {
    observable_destroy((Observable *) data);
}

PipelineManager *pipemanager_create() {
    PipelineManager *result = malloc(sizeof(PipelineManager));
    result ->pipelines = g_hash_table_new_full(g_direct_hash, g_direct_equal,
                         NULL,
                         observable_destroy_gwrapper);
    return result;
}

void pipemanager_add_pipeline(PipelineManager *manager, Observable *pipe) {
    g_hash_table_insert(manager->pipelines, pipe, pipe);
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

    g_hash_table_insert(manager->pipelines, pipe, pipe);
}

void pipemanager_destroy(PipelineManager *manager) {
    g_hash_table_destroy(manager->pipelines);
}

