#include "testrunner.h"

#include <graphics.h>
#include <stb_image.h>
#include <stb_image_write.h>

void saveFramebufferToPNG(n_GContext* ctx, const char* filename) {
    static uint8_t color_framebuffer[__SCREEN_WIDTH * __SCREEN_HEIGHT * 4]; // ugly, but hey we are on PC now
    uint8_t* outColorPtr = color_framebuffer;

#ifdef PBL_BW
    uint8_t* fbColorPtr, *fbLinePtr = ctx->fbuf;
    for (uint32_t y = 0; y < __SCREEN_HEIGHT; y++) {
        fbColorPtr = fbLinePtr;
        for (uint32_t x = 0; x < __SCREEN_WIDTH / 8; x++) {
            for (int i = 0; i < 8; i++) {
                uint8_t val = (*fbColorPtr & (1 << i)) ? 255 : 0;
                outColorPtr[0] = outColorPtr[1] = outColorPtr[2] = val;
                outColorPtr[3] = 255;
                outColorPtr += 4;
            }
            fbColorPtr += 1;
        }
        fbLinePtr += __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT;
    }
#else
    uint32_t byteCount = __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT * __SCREEN_HEIGHT;
    n_GColor* fbColorPtr = (n_GColor*)ctx->fbuf;
    while (--byteCount) {
        outColorPtr[0] = fbColorPtr->r * 85;
        outColorPtr[1] = fbColorPtr->g * 85;
        outColorPtr[2] = fbColorPtr->b * 85;
        outColorPtr[3] = fbColorPtr->a * 85;
        fbColorPtr += 1;
        outColorPtr += 4;
    }
#endif

    stbi_write_png(filename, __SCREEN_WIDTH, __SCREEN_HEIGHT, 4, color_framebuffer, __SCREEN_WIDTH * 4);
}

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

void setConsoleColor(ConsoleColor foreground) {
#ifdef WIN32
    static HANDLE console = NULL;
    if (console == NULL) {
        console = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    if (console != NULL) {
        // We have a console handle, use that
        WORD attributes;
        switch (foreground) {
            case NGFX_CONCOLOR_RED:
                attributes = FOREGROUND_RED;
                break;
            case NGFX_CONCOLOR_GREEN:
                attributes = FOREGROUND_GREEN;
                break;
            case NGFX_CONCOLOR_WHITE:
            default:
                attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
                break;
        }
        SetConsoleTextAttribute(console, attributes);

        return;
    }
#endif
    // if not try ANSI sequences
    char colorSeq;
    switch (foreground) {
        case NGFX_CONCOLOR_RED:
            colorSeq = 31;
            break;
        case NGFX_CONCOLOR_GREEN:
            colorSeq = 32;
            break;
        case NGFX_CONCOLOR_WHITE:
        default:
            colorSeq = 37;
            break;
    }

    putchar(27); // ESC
    putchar(colorSeq);
}
