#pragma once
#include "../test/test.h"
#include "graphics.h"

#define _MACRO_TO_STRING(m) #m
#define MACRO_TO_STRING(m) _MACRO_TO_STRING(m)
#define PBL_TYPE_STR MACRO_TO_STRING(PBL_TYPE)

#define SCREEN_FRAMEBUFFER_SIZE __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT * __SCREEN_HEIGHT
#define ERROR_MESSAGE_BUFFER_SIZE 512

extern const n_Test tests[];

// Utils
void saveFramebufferToPNG(struct n_GContext *ctx, const char *filename);

typedef enum {
    NGFX_CONCOLOR_NORMAL = 0,
    NGFX_CONCOLOR_RED = 1,
    NGFX_CONCOLOR_GREEN = 2
} ConsoleColor;
void setConsoleColor(ConsoleColor foreground); // for every further write

// testrunner context
typedef struct {
    uint32_t count;
    uint32_t capacity;
    uint32_t *ids;
    const char** names;
} ResourceMapping;

typedef struct {
    const uint8_t *framebuffer;
    const n_GContext *context;
    ResourceMapping res_mapping;
    char message_buffer[ERROR_MESSAGE_BUFFER_SIZE];
    char message_buffer2[ERROR_MESSAGE_BUFFER_SIZE]; // to be used by int_ngfxtest_msg_xyz functions
} TestRunnerContext;
extern TestRunnerContext runner_context;

// Resources
typedef struct { // to be replaced with n_GBitmap
    uint32_t width;
    uint32_t height;
    n_GColor pixels[];
} ResImage;

void resetResourceMapping();
const char *getResourceNameById(uint32_t resource_id);
ResImage *loadImageByName(const char *name);
ResImage *loadImageById(uint32_t resource_id);
