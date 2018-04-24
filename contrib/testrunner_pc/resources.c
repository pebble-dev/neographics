#define _CRT_SECURE_NO_WARNINGS // Let me be POSIX
#include "testrunner.h"
#include <stb_image.h>

#define TEST_RESOURCE_DIR "test/resources/"
#define MAX_PATH_LEN 256

void resetResourceMapping() {
    int i;
    runner_context.res_mapping.count = 0;

    for (i = 0; i < MAX_LOADED_IMAGES; i++) {
        if (runner_context.images[i] != NULL) {
            free(runner_context.images[i]);
            runner_context.images[i] = NULL;
        }
    }
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

const n_GBitmap *int_ngfxtest_load_image(uint32_t resource_id, n_GBitmapFormat format) {
    n_GBitmap* result = loadImageById(resource_id);
    if (result == NULL)
        return NULL;

    if (format != n_GBitmapFormat8Bit) {
        n_GBitmap* converted = convert8BitImage(result, format);
        free(result);
        if (converted == NULL)
            return NULL;
        else
            result = converted;
    }

    int i = 0;
    for (; i < MAX_LOADED_IMAGES; i++) {
        if (runner_context.images[i] == NULL) {
            runner_context.images[i] = result;
            return result;
        }
    }

    free(result);
    return NULL;
}

n_GBitmap *loadImageById(uint32_t resource_id) {
    const char *name = getResourceNameById(resource_id);
    return name == NULL ? NULL : loadImageByName(name);
}

n_GBitmap *loadImageByName(const char *name) {
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

    n_GBitmap *res = (n_GBitmap*)malloc(sizeof(n_GBitmap) + w * h * sizeof(n_GColor));
    if (res == NULL) {
        free(pixels);
        return NULL;
    }
    res->addr = ((uint8_t*)res) + sizeof(n_GBitmap);
    res->bounds = n_GRect(0, 0, w, h);
    res->format = n_GBitmapFormat8Bit;
    res->raw_bitmap_size = res->bounds.size;
    res->palette = 0;
    res->palette_size = 0;
    res->free_palette_on_destroy = false;
    res->free_data_on_destroy = false;
    res->row_size_bytes = w;

    n_GColor *resPixelPtr = (n_GColor*)res->addr;
    unsigned char *imgPixelPtr = pixels;
    uint32_t i;
    for (i = 0; i < w*h; i++) {
        *resPixelPtr = n_GColorFromRGBA(imgPixelPtr[0], imgPixelPtr[1], imgPixelPtr[2], imgPixelPtr[3]);
        resPixelPtr++;
        imgPixelPtr += 4;
    }

    free(pixels);
    return res;
}

float color_distance(n_GColor a, n_GColor b) {
    int dr = (int)a.r - b.r;
    int dg = (int)a.g - b.g;
    int db = (int)a.b - b.b;
    int da = (int)a.a - b.a;
    return dr * dr + dg * dg + db * db + da * da;
}

n_GBitmap *convert8BitImage(n_GBitmap* source, n_GBitmapFormat format) {
    // this uses a fixed palette pretty close to the ANSI colors (but better suited for ngfx tests)
    // 1Bit is just 1BitPalette without the palette
    static const uint8_t source_palette[16] = {
        0b11000000, 0b11111111, 0b11110000, 0b11001100,
        0b11100100, 0b11000010, 0b11100010, 0b11001010,
        0b00000000, 0b11010101, 0b11110101, 0b11011101, 
        0b11111101, 0b11010111, 0b11110111, 0b11011111
    };

    uint32_t bits_per_pixel;
    switch (format) {
        case n_GBitmapFormat4BitPalette: bits_per_pixel = 4; break;
        case n_GBitmapFormat2BitPalette: bits_per_pixel = 2; break;
        case n_GBitmapFormat1BitPalette:
        case n_GBitmapFormat1Bit: bits_per_pixel = 1; break;
        default: return NULL;
    }

    uint32_t w = source->raw_bitmap_size.w;
    uint32_t h = source->raw_bitmap_size.h;
    uint32_t palette_size = 1 << bits_per_pixel;
    uint32_t pitch = (w * bits_per_pixel + 7) / 8;
    n_GBitmap *res = (n_GBitmap*)malloc(sizeof(n_GBitmap) + pitch * h + palette_size * sizeof(n_GColor));
    if (res == NULL) {
        free(res);
        return NULL;
    }
    res->addr = ((uint8_t*)res) + sizeof(n_GBitmap);
    res->palette = res->addr + pitch * h;
    res->bounds = source->bounds;
    res->format = format;
    res->free_data_on_destroy = false;
    res->free_palette_on_destroy = false;
    res->raw_bitmap_size = source->raw_bitmap_size;
    res->row_size_bytes = pitch;
    res->palette_size = palette_size;

    memset(res->addr, 0, pitch * h);
    memcpy(res->palette, source_palette, palette_size * sizeof(n_GColor));

    for (uint32_t y = 0; y < h; y++) {
        for (uint32_t x = 0; x < w; x++) {
            n_GColor src_pixel = {
                .argb = source->addr[y * source->row_size_bytes + x]
            };

            uint32_t nearest = 0;
            float nearest_distance = color_distance(src_pixel, (n_GColor) { .argb = source_palette[0] });
            for (uint32_t i = 1; i < palette_size; i++) {
                float distance = color_distance(src_pixel, (n_GColor) { .argb = source_palette[i] });
                if (distance < nearest_distance) {
                    nearest_distance = distance;
                    nearest = i;
                }
            }

            uint32_t byte_index = y * pitch + x / (8 / bits_per_pixel);
            uint32_t mask = (1 << bits_per_pixel) - 1;
            uint32_t shift = (x % (8 / bits_per_pixel)) * bits_per_pixel;
            res->addr[byte_index] |= (nearest & mask) << shift;
        }
    }

    return res;
}
