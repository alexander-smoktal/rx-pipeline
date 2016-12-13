#pragma once

#include "observable.h"

typedef struct PipelineManager PipelineManager;

PipelineManager *pipemanager_create();
void pipemanager_add_pipeline(PipelineManager *manager, Observable *pipe);
void pipemanager_make_pipeline(PipelineManager *manager,
                               Observable *source,
                               unsigned int pipecount, ...);
void pipemanager_destroy(PipelineManager *manager);
