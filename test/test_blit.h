/*
 * So turns out graphics_draw_bitmap_in_rect is rather complicated...
 * Here are the dimensions we have to test against:
 *  - platform (Aplite/Basalt/Chalk)
 *  - pixelformat (of the bitmap)
 *  - screen coverage (fully/partial/none)
 *  - bitmap size vs screen size (<= or >)
 *  - tiling (one (sub-)tile, multiple tiles)
 *  - compositing mode (on Aplite all, on Basalt/Chalk Assign and Set)
 * with a combination count of 1080 only some are actually tested :)
 *
 * the test structure is always the same though:
 *  - manually draw b/w checkerboard pattern on the screen
 *  - set unique test parameters
 *  - draw the test image
 *  - compare the whole screen against an expected output
 * 
 * to make it extra hard always take a sub-bitmap
 */

#include "test.h"
#include "gbitmap/gbitmap.h"

#define SMILEY_SUB_X 20
#define SMILEY_SUB_Y 20
#define SMILEY_WIDTH 67
#define SMILEY_HEIGHT 123
#define BLENDING_WIDTH 32
#define BLENDING_HEIGHT 32
#define BIG_SMILEY_WIDTH 250
#define BIG_SMILEY_HEIGHT 228

#define DRAW_CHECKERBOARD do { \
        n_graphics_context_set_fill_color(context, n_GColorBlack); \
        n_graphics_fill_rect(context, n_GRect(0, 0, __SCREEN_WIDTH, __SCREEN_HEIGHT), 0, n_GCornerNone); \
        n_graphics_context_set_fill_color(context, n_GColorWhite); \
        const int size = 6; \
        n_GRect p = n_GRect(0, 0, size, size); \
        for (; p.origin.y < __SCREEN_HEIGHT; p.origin.y += size) { \
            p.origin.x = (p.origin.y / size) % 2 ? 0 : size; \
            for (; p.origin.x < __SCREEN_WIDTH; p.origin.x += size * 2) { \
                n_graphics_fill_rect(context, p, 0, n_GCornerNone); \
            } \
        } \
    } while(0)

NGFX_BIG_TEST(Blit, Assign1Bit,
    ngfxtest_map_resource("blit_assign1bit.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

#ifndef PBL_BW // bw currently only allows 1Bit bitmaps
NGFX_BIG_TEST(Blit, Assign8Bit,
    ngfxtest_map_resource("blit_assign8bit.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat8Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, Assign1BitPalette,
    ngfxtest_map_resource("blit_assign1bitpalette.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1BitPalette);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, Assign2BitPalette,
    ngfxtest_map_resource("blit_assign2bitpalette.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat2BitPalette);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, Assign4BitPalette,
    ngfxtest_map_resource("blit_assign4bitpalette.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat4BitPalette);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)
#endif // !PBL_BW

NGFX_BIG_TEST(Blit, Tiling1Bit,
    ngfxtest_map_resource("blit_tiling1bit.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(
        -SMILEY_WIDTH/2, -SMILEY_HEIGHT/2, 3 * SMILEY_WIDTH, 3 * SMILEY_HEIGHT
    ));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

#ifndef PBL_BW // bw currently only allows 1bit bitmaps
NGFX_BIG_TEST(Blit, Tiling8Bit,
    ngfxtest_map_resource("blit_tiling8bit.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat8Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(
        -SMILEY_WIDTH/2, -SMILEY_HEIGHT/2, 3 * SMILEY_WIDTH, 3 * SMILEY_HEIGHT
    ));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, Tiling4BitPalette,
    ngfxtest_map_resource("blit_tiling4bitpalette.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat4BitPalette);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(
        -SMILEY_WIDTH/2, -SMILEY_HEIGHT/2, 3 * SMILEY_WIDTH, 3 * SMILEY_HEIGHT
    ));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, Blending8Bit,
    ngfxtest_map_resource("blit_blending8bit.png", 1);
    ngfxtest_map_resource("blit_blending.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(blending, 2, n_GBitmapFormat8Bit);
    n_graphics_context_set_compositing_mode(context, n_GCompOpSet);
    n_graphics_draw_bitmap_in_rect(context, blending, n_GRect(
        16, 16, BLENDING_WIDTH, BLENDING_HEIGHT
    ));

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, Blending4BitPalette,
    ngfxtest_map_resource("blit_blending4bitpalette.png", 1);
    ngfxtest_map_resource("blit_blending.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(blending, 2, n_GBitmapFormat4BitPalette);
    n_graphics_context_set_compositing_mode(context, n_GCompOpSet);
    n_graphics_draw_bitmap_in_rect(context, blending, n_GRect(
        16, 16, BLENDING_WIDTH, BLENDING_HEIGHT
    ));

    NGFX_ASSERT_SCREEN(1);
)
#endif // !PBL_BW

NGFX_BIG_TEST(Blit, PartiallyClipped,
    ngfxtest_map_resource("blit_partiallyclipped.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image(fullsmiley, 2);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(-32, -32, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, FullyClipped,
    ngfxtest_map_resource("blit_fullyclipped.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image(fullsmiley, 2);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(
        -2 * SMILEY_WIDTH, -2 * SMILEY_HEIGHT, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, BiggerThanScreen,
    ngfxtest_map_resource("blit_biggerthanscreen.png", 1);
    ngfxtest_map_resource("blit_bigsmiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image(bigsmiley, 2);
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssign);
    n_graphics_draw_bitmap_in_rect(context, bigsmiley, n_GRect(
        -30, -30, BIG_SMILEY_WIDTH, BIG_SMILEY_HEIGHT
    ));

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, CompOpSet,
    ngfxtest_map_resource("blit_compopset.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image(fullsmiley, 2);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpSet);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, CompOpInverted,
    ngfxtest_map_resource("blit_compopinverted.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAssignInverted);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, CompOpOr,
    ngfxtest_map_resource("blit_compopor.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpOr);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, CompOpAnd,
    ngfxtest_map_resource("blit_compopand.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpAnd);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)

NGFX_BIG_TEST(Blit, CompOpClear,
    ngfxtest_map_resource("blit_compopclear.png", 1);
    ngfxtest_map_resource("blit_smiley.png", 2);
    DRAW_CHECKERBOARD;

    ngfxtest_load_image_ex(fullsmiley, 2, n_GBitmapFormat1Bit);
    n_GBitmap *smiley = n_gbitmap_create_as_sub_bitmap(fullsmiley, n_GRect(
        SMILEY_SUB_X, SMILEY_SUB_Y, SMILEY_WIDTH, SMILEY_HEIGHT
    ));
    n_graphics_context_set_compositing_mode(context, n_GCompOpClear);
    n_graphics_draw_bitmap_in_rect(context, smiley, n_GRect(16, 16, SMILEY_WIDTH, SMILEY_HEIGHT));
    n_gbitmap_destroy(smiley);

    NGFX_ASSERT_SCREEN(1);
)
