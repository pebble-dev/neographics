#define _CRT_SECURE_NO_WARNINGS // Let me be POSIX
#include "testrunner.h"

#define TEST_RESSOURCE_DIR "test/ressources/"
#define MAX_PATH_LEN 256

void resetRessourceMapping() {
    runner_context.res_mapping.count = 0;
}

const char* getRessourceNameById(uint32_t ressource_id) {
    TestRunnerContext* context = &runner_context;
    uint32_t* id_ptr = context->res_mapping.ids;
    uint32_t i;

    for (i = 0; i < context->res_mapping.count; i++) {
        if (*id_ptr == ressource_id) {
            return context->res_mapping.names[i];
        }
    }
    
    return NULL;
}

static void prv_ensureResMappingCapacity() {
    static const uint32_t BLOCK_SIZE = 16;
    TestRunnerContext* context = &runner_context;
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

bool int_ngfxtest_map_ressource(const char* ressource_name, uint32_t ressource_id) {
    TestRunnerContext* context = &runner_context;

    // Try to open file
    char filename[MAX_PATH_LEN];
    snprintf(filename, MAX_PATH_LEN, "%s%s", TEST_RESSOURCE_DIR, ressource_name);
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return false;
    }
    fclose(fp);

    // Search old entry
    uint32_t i;
    for (i = 0; i < context->res_mapping.count; i++) {
        if (context->res_mapping.ids[i] == ressource_id) {
            context->res_mapping.names[i] = ressource_name;
            return true;
        }
    }

    // Append new entry
    prv_ensureResMappingCapacity();
    context->res_mapping.ids[i] = ressource_id;
    context->res_mapping.names[i] = ressource_name;
    context->res_mapping.count++;
    return true;
}

ResHandle resource_get_handle(uint32_t id) {
    const char* name = getRessourceNameById(id);
    return (ResHandle)name;
}

size_t resource_size(ResHandle handle) {
    if (handle == NULL) {
        return 0;
    }

    char path[MAX_PATH_LEN];
    snprintf(path, MAX_PATH_LEN, "%s%s", TEST_RESSOURCE_DIR, (const char*)handle);
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    size_t size = (size_t)ftell(fp);
    fclose(fp);

    return size;
}

size_t resource_load(ResHandle handle, uint8_t * buffer, size_t max_length) {
    if (handle == NULL || buffer == NULL || max_length == 0) {
        return 0;
    }

    char path[MAX_PATH_LEN];
    snprintf(path, MAX_PATH_LEN, "%s%s", TEST_RESSOURCE_DIR, (const char*)handle);
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        return 0;
    }
    size_t size_read = fread(buffer, 1, max_length, fp);
    fclose(fp);

    return size_read;
}
