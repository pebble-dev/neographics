/*
 * The local pc testrunner
 */
#include "testrunner.h"
#include <stdarg.h>
#include <graphics.h>

TestRunnerContext runner_context;

// Stubs to not anger the linker
GBitmap* graphics_capture_frame_buffer(n_GContext* ctx) {
    return NULL;
}

GBitmap* graphics_capture_frame_buffer_format(n_GContext* ctx, GBitmapFormat format) {
    return NULL;
}

bool graphics_release_frame_buffer(n_GContext* ctx, GBitmap* bitmap) {
    return false;
}

int main(int argc, char* argv[]) {
    uint8_t* framebuffer = (uint8_t*)malloc(SCREEN_FRAMEBUFFER_SIZE);
    if (framebuffer == NULL) {
        fprintf(stderr, "Could not allocate framebuffer\n");
        return 2;
    }
    n_GContext* ctx = n_graphics_context_from_buffer(framebuffer);
    if (ctx == NULL) {
        fprintf(stderr, "Could not create context\n");
        return 2;
    }
    memset(&runner_context, 0, sizeof(TestRunnerContext));
    runner_context.framebuffer = framebuffer;
    runner_context.context = ctx;

    // Run the tests
    uint32_t test_count = 0, test_succeeded = 0;
    const n_Test* current_test = tests;
    while (current_test->func != NULL) {
        memset(framebuffer, 0, SCREEN_FRAMEBUFFER_SIZE);
        bool should_fail = strcmp(current_test->module, "Test") == 0 &&
            strstr(current_test->name, "Fail") == current_test->name;

        test_count++;
        n_TestResult result = current_test->func();
        bool success = (should_fail && !result.success) || (!should_fail && result.success);
        if (success) {
            test_succeeded++;
            setConsoleColor(NGFX_CONCOLOR_GREEN);
            printf("[S] %s.%s\n", current_test->module, current_test->name);
        }
        else {
            setConsoleColor(NGFX_CONCOLOR_RED);
            printf("[F] %s.%s - %s:%d\n", current_test->module, current_test->name, result.file, result.line);
        }
        if (!success) {
            printf("    %s\n", result.message);
        }
        setConsoleColor(NGFX_CONCOLOR_NORMAL);

        current_test++;
    }

    setConsoleColor(NGFX_CONCOLOR_NORMAL);
    printf("\n%d / %d Tests succeeded\n", test_succeeded, test_count);

    n_graphics_context_destroy(ctx);
    free(framebuffer);
    return test_count != test_succeeded;
}

// error messages for the tests

n_GColor ngfxtest_get_pixel(n_GPoint point) {
#ifdef PBL_BW
    uint32_t byteIndex = point.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT + point.x / 8;
    uint32_t bitMask = 1 << (point.x % 8);
    return (runner_context.framebuffer[byteIndex] & bitMask) ? n_GColorWhite : n_GColorBlack;
#else
    uint32_t index = point.y * __SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT + point.x;
    return (n_GColor) { .argb = runner_context.framebuffer[index] };
#endif
}

bool int_ngfxtest_pixel_eq(n_GPoint point, n_GColor expected_color) {
    if (point.x < 0 || point.y < 0 || point.x >= __SCREEN_WIDTH || point.y >= __SCREEN_HEIGHT) {
        return false;
    }
    n_GColor actual = ngfxtest_get_pixel(point);
    return actual.argb == expected_color.argb;
}

bool int_ngfxtest_subscreen_eq(n_GRect rect, uint32_t expected_ressource_id) {
    return false;
}

const char* int_ngfxtest_format_msg(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(runner_context.message_buffer, ERROR_MESSAGE_BUFFER_SIZE, format, args);
    va_end(args);
    return runner_context.message_buffer;
}

const char* int_ngfxtest_msg_pixel(n_GPoint point, n_GColor expected) {
    if (point.x < 0 || point.y < 0 || point.x >= __SCREEN_WIDTH || point.y >= __SCREEN_HEIGHT) {
        snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
            "Pixel coord out of bounds: (GPoint){%d, %d}",
            point.x, point.y);
    }
    else {
        n_GColor actual = ngfxtest_get_pixel(point);
        snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
            "Pixel value at (GPoint){%d, %d} unexpected.\n    Actual: (GColor){%d, %d, %d, %d} \tExpected: (GColor){%d, %d, %d, %d}",
            point.x, point.y, actual.r, actual.g, actual.b, actual.a, expected.r, expected.g, expected.b, expected.a);
    }
    return runner_context.message_buffer2;
}

const char* int_ngfxtest_msg_subscreen(n_GRect rect, uint32_t expected_ressource_id) {
    return "Not implemented yet";
}
