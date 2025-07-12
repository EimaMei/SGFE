#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG

#define RGFW_BUFFER_NATIVE
#define RGFW_BUFFER

#include <RGFW_embedded.h>
#include "../resources/lonic_bin.h"


/* fill buffer with a color, clearing anything that was on it */
void clear(RGFW_window* win, u8 color[4]) {
	for (ssize_t y = 0; y < win->bufferSize.h; y += 1) {
		for (ssize_t x = 0; x < win->bufferSize.w; x += 1) {
			ssize_t index = 4 * (y * win->bufferSize.w + x);
			RGFW_MEMCPY(&win->buffer[index], color, 4);
		}
	}
}

void drawBitmap(RGFW_window* win, u8* bitmap, RGFW_rect r) {
#ifdef RGFW_3DS
	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

			ssize_t opixel = 4 * ((j - r.y) * r.w + (i - r.x));
			win->buffer[pixel_left + 0] = bitmap[opixel + 3];
			win->buffer[pixel_left + 1] = bitmap[opixel + 2];
			win->buffer[pixel_left + 2] = bitmap[opixel + 1];
			win->buffer[pixel_left + 3] = bitmap[opixel + 0];

			pixel_left += win->src.front_buffer_size / 2 - (i32)(RGFW_systemGet3DSlider() * 5);
			win->buffer[pixel_left + 0] = bitmap[opixel + 3];
			win->buffer[pixel_left + 1] = bitmap[opixel + 2];
			win->buffer[pixel_left + 2] = bitmap[opixel + 1];
			win->buffer[pixel_left + 3] = bitmap[opixel + 0];
		}
	}
#endif

}


void drawRect(RGFW_window* win, RGFW_rect r, u8 color[4]) {
#ifdef RGFW_3DS
	#define RGFW_swap(a, b) do { \
		u8 tmp = a; \
		a = b; \
		b = tmp; \
	} while (0)

	RGFW_swap(color[0], color[3]);
	RGFW_swap(color[1], color[2]);

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

			RGFW_MEMCPY(&win->buffer[pixel_left], color, 4);

			#ifdef RGFW_3DS
			float slider = RGFW_systemGet3DSlider();
			i32 x_3d = (i32)(slider * 5);
			ssize_t pixel_right = pixel_left + win->src.front_buffer_size / 2 - x_3d;
			RGFW_MEMCPY(&win->buffer[pixel_right], color, 4);
			#endif
		}
	}
#endif
}


#if RGFW_3DS
	#define BUTTON_QUIT   RGFW_Start
	#define BUTTON_LEFT   RGFW_DpadLeft
	#define BUTTON_RIGHT  RGFW_DpadRight
	#define BUTTON_UP     RGFW_DpadUp
	#define BUTTON_DOWN   RGFW_DpadDown
#endif


int main(void) {
	#ifndef RGFW_3DS
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), 0);
	#else
	RGFW_window* win = RGFW_createWindow(RGFW_videoMode3D, 0);
	#endif
	RGFW_rect r = RGFW_RECT(100, 100, img_lonic_width, img_lonic_height);

	RGFW_bool running = RGFW_TRUE;

	while (running) {
		while (RGFW_window_checkEvent(win)) {
			if (win->event->type == RGFW_quit || RGFW_isPressed(RGFW_controllerGet(0), BUTTON_QUIT)) {
				running = 0;
				break;
			}
		}

		RGFW_controller* p1 = RGFW_controllerGet(0);

		if (RGFW_isPressed(p1, BUTTON_LEFT) && r.x > 0) {
			r.x -= 1;
		}
		else if (RGFW_isPressed(p1, BUTTON_RIGHT) && r.x + r.w < win->bufferSize.w) {
			r.x += 1;
		}

		if (RGFW_isPressed(p1, BUTTON_UP) && r.y > 0) {
			r.y -= 1;
		}
		else if (RGFW_isPressed(p1, BUTTON_DOWN) && r.y + r.h < win->bufferSize.h) {
			r.y += 1;
		}

		u8 color[4] = {255,255,255,255};
		u8 color2[4] = {0, 255, 0, 255};

		clear(win, color);
		drawRect(win, RGFW_RECT(15, 15, 64, 64), color2);
		drawBitmap(win, img_lonic_data, r);

		// RGFW_window_swapBuffers could work here too, but I want to ensure only the CPU buffer is being swapped
		RGFW_window_swapBuffers_software(win);
	}

	RGFW_window_close(win);
}
