#define SGFE_IMPLEMENTATION
#include <SGFE.h>
#include <resources/controls.h>

#if SGFE_3DS
	#define SILK_PIXELBUFFER_WIDTH  400
	#define SILK_PIXELBUFFER_HEIGHT 240
#endif

#define SILK_IMPLEMENTATION
#include "silk.h"


int main(void) {
	SGFE_window* win = SGFE_windowMake(SGFE_videoModeOptimal(), SGFE_windowBuffer);

	while (!SGFE_windowShouldClose(win)) {
		const SGFE_event* event;
		while (SGFE_windowCheckEvent(win, &event)) {
			if (event->type == SGFE_buttonPressed && event->button == BUTTON_START) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				break;
			}
		}

		SGFE_contextBuffer* ctx = SGFE_windowGetContextBuffer(win);
		pixel* buffer = (pixel*)(void*)SGFE_bufferGetFramebuffer(ctx);
		silkClearPixelBufferColor(buffer, 0x11AA0033);

		SGFE_area res = SGFE_bufferGetResolution(ctx);
		i32 buf_width = res.w,
			buf_height = res.h;

		silkDrawRect(
			buffer,
			(vec2i) { SILK_PIXELBUFFER_WIDTH, SILK_PIXELBUFFER_HEIGHT },
			SILK_PIXELBUFFER_WIDTH,
			(vec2i) { (i32)((f32)buf_width * 0.3f), (i32)((f32)buf_height * 0.2f) },
			(vec2i) { (i32)((f32)buf_width * 0.4f), (i32)((f32)buf_height * 0.4f) },
			0xff0000ff
		);

		const char* text = "Hello, SGFE!";
		const size_t text_size = 8;
		const i32 text_spacing = 1;

		silkDrawTextDefault(
			buffer,
			(vec2i) { SILK_PIXELBUFFER_WIDTH, SILK_PIXELBUFFER_HEIGHT },
			SILK_PIXELBUFFER_WIDTH,
			text,
			(vec2i) {
				buf_width / 2 - silkMeasureText(text, text_size, text_spacing).x / 2,
				buf_height / 2 - silkMeasureText(text, text_size, text_spacing).y / 2 + buf_height / 4
			},
			text_size,
			text_spacing,
			0xff000000
		);

		SGFE_windowSwapBuffers(win);
	}

	SGFE_windowClose(win);
}
