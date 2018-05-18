#pragma once
#include "../test/test.h"
#include "graphics.h"
#include "gbitmap/gbitmap.h"

#define _MACRO_TO_STRING(m) #m
#define MACRO_TO_STRING(m) _MACRO_TO_STRING(m)
#define PBL_TYPE_STR MACRO_TO_STRING(PBL_TYPE)

#define SCREEN_FRAMEBUFFER_SIZE __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT * __SCREEN_HEIGHT
#define ERROR_MESSAGE_BUFFER_SIZE 512
#define MAX_LOADED_IMAGES 32

extern const n_Test tests[];

// Utils
bool saveFramebufferToPNG(struct n_GContext *ctx, const char *filename);

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
    uint8_t *framebuffer;
    n_GContext *context;
    ResourceMapping res_mapping;
    n_GBitmap *images[MAX_LOADED_IMAGES];
    const char *actual_image_path;
    const char *current_test_module;
    const char *current_test_name;
    char message_buffer[ERROR_MESSAGE_BUFFER_SIZE];
    char message_buffer2[ERROR_MESSAGE_BUFFER_SIZE]; // to be used by int_ngfxtest_msg_xyz functions
} TestRunnerContext;
extern TestRunnerContext runner_context;

bool initTestRunnerContext(TestRunnerContext *runner_context);
void resetTestRunnerContext(TestRunnerContext *runner_context, const char *module, const char *name);
void freeTestRunnerContext(TestRunnerContext *runner_context);
void saveAsActualImage(TestRunnerContext *runner_context);

// Resources
const char *getResourceNameById(uint32_t resource_id);
n_GBitmap *loadImageByName(const char *name);
n_GBitmap *loadImageById(uint32_t resource_id);
n_GBitmap *convert8BitImage(n_GBitmap *source, n_GBitmapFormat format);
