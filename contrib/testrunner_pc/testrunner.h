#pragma once
#include "../test/test.h"
#include "graphics.h"

#define SCREEN_FRAMEBUFFER_SIZE __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT * __SCREEN_HEIGHT
#define ERROR_MESSAGE_BUFFER_SIZE 512

extern const n_Test tests[];

void saveFramebufferToPNG(struct n_GContext* ctx, const char* filename);

typedef enum {
    NGFX_CONCOLOR_NORMAL = 0,
    NGFX_CONCOLOR_RED = 1,
    NGFX_CONCOLOR_GREEN = 2
} ConsoleColor;
void setConsoleColor(ConsoleColor foreground); // for every further write

// testrunner contest
typedef struct {
    uint32_t count;
    uint32_t capacity;
    uint32_t* ids;
    const char** names;
} RessourceMapping;

typedef struct {
    const uint8_t* framebuffer;
    const n_GContext* context;
    n_GPoint error_coords;
    RessourceMapping res_mapping;
    char message_buffer[ERROR_MESSAGE_BUFFER_SIZE];
    char message_buffer2[ERROR_MESSAGE_BUFFER_SIZE]; // to be used by ngfxtest_msg_xyz functions
} TestRunnerContext;

void resetRessourceMapping(TestRunnerContext* context);
const char* getRessourceNameById(TestRunnerContext* context, uint32_t ressource_id);
