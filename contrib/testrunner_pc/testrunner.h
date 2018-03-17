#pragma once
#include "../test/test.h"

extern const n_Test tests[];

void saveFramebufferToPNG(struct n_GContext* ctx, const char* filename);

typedef enum {
    NGFX_CONCOLOR_WHITE = 0,
    NGFX_CONCOLOR_RED = 1,
    NGFX_CONCOLOR_GREEN = 2
} ConsoleColor;
void setConsoleColor(ConsoleColor foreground); // for every further write
