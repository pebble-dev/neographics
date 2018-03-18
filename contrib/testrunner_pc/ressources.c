#include "testrunner.h"

void resetRessourceMapping(TestRunnerContext* context) {
    context->res_mapping.count = 0;
}

const char* getRessourceNameById(TestRunnerContext* context, uint32_t ressource_id) {
    uint32_t* id_ptr = context->res_mapping.ids;
    uint32_t i;

    for (i = 0; i < context->res_mapping.count; i++) {
        if (*id_ptr == ressource_id) {
            return context->res_mapping.names[i];
        }
    }
    
    return NULL;
}

static void _ensureResMappingCapacity(TestRunnerContext* context) {
    static const uint32_t BLOCK_SIZE = 16;
    if (context->res_mapping.count == context->res_mapping.capacity) {
        uint32_t new_capacity = context->res_mapping.capacity + BLOCK_SIZE;
        
        uint32_t* new_ids = (uint32_t*)realloc(context->res_mapping.ids, sizeof(uint32_t) * new_capacity);
        if (new_ids == NULL) {
            fputs("Could not allocate ressource mapping ids", stderr);
            exit(2);
        }
        const char** new_names = (const char**)realloc(context->res_mapping.names, sizeof(const char**) * new_capacity);
        if (new_names == NULL) {
            fputs("Could not allocate ressource mapping names", stderr);
            exit(2);
        }

        context->res_mapping.ids = new_ids;
        context->res_mapping.names = new_names;
        context->res_mapping.capacity = new_capacity;
    }
}

void _ngfxtest_map_ressource(void* raw_context, const char* ressource_name, uint32_t ressource_id) {
    TestRunnerContext* context = (TestRunnerContext*)raw_context;

    // Search old entry
    uint32_t i;
    for (i = 0; i < context->res_mapping.count; i++) {
        if (context->res_mapping.ids[i] == ressource_id) {
            context->res_mapping.names[i] = ressource_name;
            return;
        }
    }

    // Append new entry
    _ensureResMappingCapacity(context);
    context->res_mapping.ids[i] = ressource_id;
    context->res_mapping.names[i] = ressource_name;
    context->res_mapping.count++;
}
