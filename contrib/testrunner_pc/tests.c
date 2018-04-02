#include "testrunner.h"

// Test function implementations
#undef NGFX_TEST
#undef NGFX_BIG_TEST
#define NGFX_TEST(module,name,...) NGFX_BIG_TEST(module, name, __VA_ARGS__)
#define NGFX_BIG_TEST(module,name,...) \
    n_TestResult test_##module##_##name (uint8_t *const framebuffer, n_GContext *const context) { \
        { __VA_ARGS__; } \
        return (n_TestResult) { \
            .success = true, \
            .file = __FILE__, \
            .line = __LINE__, \
            .message = "Succeeded", \
        }; \
    }

#include "../test/tests.h"

// Test metadata
#undef NGFX_BIG_TEST
#define NGFX_BIG_TEST(a_module,a_name,...) \
    (n_Test) { \
        .module = #a_module, \
        .name = #a_name, \
        .func = test_##a_module##_##a_name \
    },

const n_Test tests[] = {
#include "../test/tests.h"
    (n_Test) { .func = NULL }
};
