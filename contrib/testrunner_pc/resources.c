#define _CRT_SECURE_NO_WARNINGS // Let me be POSIX
#include "testrunner.h"
#include <stb_image.h>

#define TEST_RESOURCE_DIR "test/resources/"
#define MAX_PATH_LEN 256

void resetResourceMapping() {
    runner_context.res_mapping.count = 0;
}

const char *getResourceNameById(uint32_t resource_id) {
    TestRunnerContext *context = &runner_context;
    uint32_t *id_ptr = context->res_mapping.ids;
    uint32_t i;

    for (i = 0; i < context->res_mapping.count; i++) {
        if (*id_ptr == resource_id) {
            return context->res_mapping.names[i];
        }
    }
    
    return NULL;
}

static FILE *openResourceByName(const char *name) {
    char path[MAX_PATH_LEN];
    snprintf(path, MAX_PATH_LEN, "%s/%s/%s", TEST_RESOURCE_DIR, PBL_TYPE_STR, name);
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        snprintf(path, MAX_PATH_LEN, "%s/%s", TEST_RESOURCE_DIR, name);
        fp = fopen(path, "rb");
        if (fp == NULL)
            return NULL;
    }
    return fp;
}

static void prv_ensureResMappingCapacity() {
    static const uint32_t BLOCK_SIZE = 16;
    TestRunnerContext *context = &runner_context;
    if (context->res_mapping.count == context->res_mapping.capacity) {
        uint32_t new_capacity = context->res_mapping.capacity + BLOCK_SIZE;
        
        uint32_t *new_ids = (uint32_t*)realloc(context->res_mapping.ids, sizeof(uint32_t) * new_capacity);
        if (new_ids == NULL) {
            fputs("Could not allocate resource mapping ids", stderr);
            exit(2);
        }
        const char** new_names = (const char**)realloc(context->res_mapping.names, sizeof(const char**) * new_capacity);
        if (new_names == NULL) {
            fputs("Could not allocate resource mapping names", stderr);
            exit(2);
        }

        context->res_mapping.ids = new_ids;
        context->res_mapping.names = new_names;
        context->res_mapping.capacity = new_capacity;
    }
}

bool int_ngfxtest_map_resource(const char *resource_name, uint32_t resource_id) {
    TestRunnerContext *context = &runner_context;

    // Try to open file
    FILE *fp = openResourceByName(resource_name);
    if (fp == NULL) {
        return false;
    }
    fclose(fp);

    // Search old entry
    uint32_t i;
    for (i = 0; i < context->res_mapping.count; i++) {
        if (context->res_mapping.ids[i] == resource_id) {
            context->res_mapping.names[i] = resource_name;
            return true;
        }
    }

    // Append new entry
    prv_ensureResMappingCapacity();
    context->res_mapping.ids[i] = resource_id;
    context->res_mapping.names[i] = resource_name;
    context->res_mapping.count++;
    return true;
}

ResHandle resource_get_handle(uint32_t id) {
    const char *name = getResourceNameById(id);
    return (ResHandle)name;
}

size_t resource_size(ResHandle handle) {
    if (handle == NULL) {
        return 0;
    }

    FILE *fp = openResourceByName((const char*)handle);
    if (fp == NULL) {
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    size_t size = (size_t)ftell(fp);
    fclose(fp);

    return size;
}

size_t resource_load(ResHandle handle, uint8_t *buffer, size_t max_length) {
    if (handle == NULL || buffer == NULL || max_length == 0) {
        return 0;
    }

    FILE *fp = openResourceByName((const char*)handle);
    if (fp == NULL) {
        return 0;
    }
    size_t size_read = fread(buffer, 1, max_length, fp);
    fclose(fp);

    return size_read;
}

ResImage *loadImageById(uint32_t resource_id) {
    const char *name = getResourceNameById(resource_id);
    return name == NULL ? NULL : loadImageByName(name);
}

ResImage *loadImageByName(const char *name) {
    if (name == NULL) {
        return NULL;
    }

    FILE *fp = openResourceByName(name);
    int w, h, comp;
    unsigned char *pixels = stbi_load_from_file(fp, &w, &h, &comp, 4);
    fclose(fp);
    if (pixels == NULL) {
        return NULL;
    }

    ResImage *res = (ResImage*)malloc(sizeof(ResImage) + w * h * sizeof(n_GColor));
    if (res == NULL) {
        free(pixels);
        return NULL;
    }
    res->width = w;
    res->height = h;

    n_GColor *resPixelPtr = res->pixels;
    unsigned char *imgPixelPtr = pixels;
    uint32_t i;
    for (i = 0; i < w*h; i++) {
#ifdef PBL_BW
        // According to pebble SDK's pebble_image_routines.py
        float luma = imgPixelPtr[0] * 0.2126f + imgPixelPtr[1] * 0.7152f + imgPixelPtr[2] * 0.11f;
        *resPixelPtr = (luma > 255 / 2) ? n_GColorWhite : n_GColorBlack;
#else
        *resPixelPtr = n_GColorFromRGBA(imgPixelPtr[0], imgPixelPtr[1], imgPixelPtr[2], imgPixelPtr[3]);
#endif
        resPixelPtr++;
        imgPixelPtr += 4;
    }

    free(pixels);
    return res;
}
