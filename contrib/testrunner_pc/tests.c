#include "testrunner.h"

// Test function implementations
#undef NGFX_TEST
#undef NGFX_BIG_TEST
#define NGFX_TEST(module,name,code) NGFX_BIG_TEST(module, name, code)
#define NGFX_BIG_TEST(module,name,code) \
    n_TestResult test_##module##_##name (void* _ngfxtest_context) { \
        { code; } \
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
#define NGFX_BIG_TEST(a_module,a_name,code) \
    (n_Test) { \
        .module = #a_module, \
        .name = #a_name, \
        .func = test_##a_module##_##a_name \
    },

const n_Test tests[] = {
#include "../test/tests.h"
    (n_Test) { .func = NULL }
};
