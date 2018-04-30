/*
 * The local pc testrunner
 */
#include "testrunner.h"
#include <stdarg.h>
#include <stb.h>

TestRunnerContext runner_context;

// Stubs to not anger the linker
GBitmap *graphics_capture_frame_buffer(n_GContext *ctx) {
    return NULL;
}

GBitmap *graphics_capture_frame_buffer_format(n_GContext *ctx, GBitmapFormat format) {
    return NULL;
}

bool graphics_release_frame_buffer(n_GContext *ctx, GBitmap *bitmap) {
    return false;
}

void print_help() {
    static const char* text =
        "usage: <test_neographics> {<option> [<argument>]}\n"
        "options:\n"
        "  -h          Shows this help screen\n"
        "  -m <string> Only run modules containing a string\n"
        "  -t <string> Only run tests containing a string\n"
        "  -a <path>   Save actual images to a path\n";
    fputs(text, stderr);
}

int main(int argc, char *argv[]) {
    // Parse arguments
    char* arg_include_module = "";
    char* arg_include_test = "";
    char* arg_actual_image_path = NULL;

    char** opts = stb_getopt_param(&argc, argv, "mta");
    if (opts == NULL) {
        fputs("Missing argument", stderr);
        print_help();
        return 1;
    }
    char** cur_opt = opts;
    while (*cur_opt != NULL) {
        char option = (*cur_opt)[0];
        char* argument = *cur_opt + 1;
        switch (option) {
            case('h'):
                print_help();
                return 0;
            case('m'):
                arg_include_module = argument;
                break;
            case('t'):
                arg_include_test = argument;
                break;
            case('a'):
                arg_actual_image_path = argument;
                break;
            default:
                fprintf(stderr, "Unknown option: %c\n", option);
                return 1;
        }
        cur_opt++;
    }

    // Run the tests
    if (!initTestRunnerContext(&runner_context))
        return 2;

    uint32_t test_count = 0, test_succeeded = 0;
    const n_Test *current_test = tests;
    while (current_test->func != NULL) {
        if (stb_stristr(current_test->module, arg_include_module) == NULL ||
            stb_stristr(current_test->name, arg_include_test) == NULL)
            continue;

        test_count++;
        bool should_fail = strcmp(current_test->module, "Test") == 0 &&
            strstr(current_test->name, "Fail") == current_test->name;

        resetTestRunnerContext(&runner_context, current_test->module, current_test->name);
        runner_context.actual_image_path = should_fail ? NULL : arg_actual_image_path;

        n_TestResult result = current_test->func(runner_context.framebuffer, runner_context.context);
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

    freeTestRunnerContext(&runner_context);
    stb_getopt_free(opts);
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
    return (n_GColor) { .argb = runner_context.framebuffer[index] | 0b11000000 }; // the screen is always opaque
#endif
}

n_GColor ngfxtest_convert_color_to_system(n_GColor color) {
#ifdef PBL_BW
    float luma = color.r * 0.2126f + color.g * 0.7152f + color.b * 0.11f;
    return (luma > 3 / 2) ? n_GColorWhite : n_GColorBlack;
#else
    return color;
#endif
}

bool int_ngfxtest_pixel_eq(n_GPoint point, n_GColor expected_color) {
    if (point.x < 0 || point.y < 0 || point.x >= __SCREEN_WIDTH || point.y >= __SCREEN_HEIGHT) {
        return false;
    }
    n_GColor actual = ngfxtest_get_pixel(point);
    actual.a = 3;
    return actual.argb == ngfxtest_convert_color_to_system(expected_color).argb;
}

bool int_ngfxtest_subscreen_eq(n_GRect rect, uint32_t expected_resource_id) {
    // Check bounds
    if (rect.origin.x < 0 || rect.origin.y < 0 || rect.size.w <= 0 || rect.size.h <= 0 ||
        rect.origin.x + rect.size.w > __SCREEN_WIDTH ||
        rect.origin.y + rect.size.h > __SCREEN_HEIGHT) {
        snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
            "Subscreen rect out of bounds: (GRect){%d, %d, %d, %d}",
            rect.origin.x, rect.origin.y, rect.size.w, rect.size.h);
        return false;
    }

    // Load image
    const char *res_name = getResourceNameById(expected_resource_id);
    if (res_name == NULL) {
        snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
            "Unmapped expected resource id: %d", expected_resource_id);
        return false;
    }
    n_GBitmap *expected_img = loadImageByName(res_name);
    if (expected_img == NULL) {
        snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
            "Could not load expected resource image: %s", res_name);
        return false;
    }
    if (expected_img->bounds.size.w != rect.size.w || expected_img->bounds.size.h != rect.size.h) {
        snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
            "Wrong expected resource image size.\n    Requested: (GSize){%d, %d} \tResource: (GSize){%d, %d}",
            rect.size.w, rect.size.h, expected_img->bounds.size.w, expected_img->bounds.size.h);
        free(expected_img);
        return false;
    }

    // Compare
    int16_t x, y;
    for (y = 0; y < rect.size.h; y++) {
        for (x = 0; x < rect.size.w; x++) {
            n_GPoint point = n_GPoint(rect.origin.x + x, rect.origin.y + y);
            n_GColor actual = ngfxtest_get_pixel(point);
            n_GColor expected = (n_GColor)expected_img->addr[y * expected_img->bounds.size.w + x];
            expected = ngfxtest_convert_color_to_system(expected);
            if (actual.argb != expected.argb) {
                snprintf(runner_context.message_buffer2, ERROR_MESSAGE_BUFFER_SIZE,
                    "Screen pixel value at (GPoint){%d, %d} unexpected.\n    Actual: (GColor){%d, %d, %d, %d} \tExpected: (GColor){%d, %d, %d, %d}",
                    point.x, point.y, actual.r, actual.g, actual.b, actual.a, expected.r, expected.g, expected.b, expected.a);
                free(expected_img);
                saveAsActualImage(&runner_context);
                return false;
            }
        }
    }

    free(expected_img);
    return true;
}

const char *int_ngfxtest_format_msg(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsnprintf(runner_context.message_buffer, ERROR_MESSAGE_BUFFER_SIZE, format, args);
    va_end(args);
    return runner_context.message_buffer;
}

const char *int_ngfxtest_msg_pixel(n_GPoint point, n_GColor expected) {
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

const char *int_ngfxtest_msg_subscreen(n_GRect rect, uint32_t expected_resource_id) {
    return runner_context.message_buffer2;
}
