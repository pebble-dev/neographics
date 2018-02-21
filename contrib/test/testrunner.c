/*
 * The first draft of the test runner, currently without any unit tests
 */
#include <graphics.h>

#include <stb_image.h>
#include <stb_image_write.h>

#define _MACRO_STR_VALUE(arg) #arg
#define MACRO_STR_VALUE(arg) _MACRO_STR_VALUE(arg)
#define OUTPUT_FILENAME ("output_" MACRO_STR_VALUE(PBL_TYPE) ".png")

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

// Stubs to not anger the linker anymore
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
	uint8_t* framebuffer = (uint8_t*)malloc(__SCREEN_FRAMEBUFFER_ROW_BYTE_AMOUNT * __SCREEN_HEIGHT);
	if (framebuffer == NULL) {
		fprintf(stderr, "Could not allocate framebuffer\n");
		return 1;
	}
	n_GContext* ctx = n_graphics_context_from_buffer(framebuffer);
	if (ctx == NULL) {
		fprintf(stderr, "Could not create context\n");
		return 1;
	}

	// create a test output
	uint16_t w = __SCREEN_WIDTH, h = __SCREEN_HEIGHT;
	n_graphics_context_set_fill_color(ctx, n_GColorBlack);
	n_graphics_fill_rect(ctx, n_GRect(0, 0, w, h), 0, n_GCornerNone);
	n_graphics_context_set_fill_color(ctx, n_GColorRed);
	n_graphics_fill_rect(ctx, n_GRect(w / 4, h / 4, w / 2, h / 2), 0, n_GCornerNone);
	saveFramebufferToPNG(ctx, OUTPUT_FILENAME);

	n_graphics_context_destroy(ctx);
	free(framebuffer);
	return 0;
}
