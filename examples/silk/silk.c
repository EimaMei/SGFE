#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>

#if RGFW_3DS
	#define SILK_PIXELBUFFER_WIDTH  400
	#define SILK_PIXELBUFFER_HEIGHT 240
#endif

#define SILK_IMPLEMENTATION
#include "silk.h"


int main(void) {
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowBuffer);

	while (!RGFW_window_shouldClose(win)) {
		const RGFW_event* event;
		while (RGFW_window_checkEvent(win, &event)) {
			if (event->type == RGFW_buttonPressed && event->button == BUTTON_START) {
				RGFW_window_setShouldClose(win, RGFW_TRUE);
				break;
			}
		}

		pixel* buffer = (pixel*)(void*)win->buffer;
		silkClearPixelBufferColor(buffer, 0x11AA0033);

		i32 buf_width = win->bufferSize.w,
			buf_height = win->bufferSize.h;

		silkDrawRect(
			buffer,
			(vec2i) { SILK_PIXELBUFFER_WIDTH, SILK_PIXELBUFFER_HEIGHT },
			SILK_PIXELBUFFER_WIDTH,
			(vec2i) { (i32)((f32)buf_width * 0.3f), (i32)((f32)buf_height * 0.2f) },
			(vec2i) { (i32)((f32)buf_width * 0.4f), (i32)((f32)buf_height * 0.4f) },
			0xff0000ff
		);

		const char* text = "Hello, RGFW!";
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

		RGFW_window_swapBuffers(win);
	}

	RGFW_window_close(win);
}
