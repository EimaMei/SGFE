#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG

// #define RGFW_BUFFER_NATIVE
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
	for (ssize_t y = 0; y < r.h; y += 1) {
		for (ssize_t x = 0; x < r.w; x += 1) {
			ssize_t dst_index = 4 * ((r.y + y) * win->bufferSize.w + (r.x + x));
			ssize_t src_index = 4 * (y * r.w + x);

			RGFW_MEMCPY(&win->buffer[dst_index], &bitmap[src_index], 4);

			#ifdef RGFW_3DS
			float slider = RGFW_systemGet3DSlider();
			i32 x_3d = 4 * (400 - (i32)(slider * 5));

			RGFW_MEMCPY(&win->buffer[dst_index + x_3d], &bitmap[src_index + x_3d], 4);
			#endif
		}
	}
}


void drawRect(RGFW_window* win, RGFW_rect r, u8 color[4]) {
	for (ssize_t y = r.y; y < r.y + r.h; y += 1) {
		for (ssize_t x = r.x; x < r.x + r.w; x += 1) {
			ssize_t index = 4 * (y * win->bufferSize.w + x);

			RGFW_MEMCPY(&win->buffer[index], color, 4);

			#ifdef RGFW_3DS
			float slider = RGFW_systemGet3DSlider();
			i32 x_3d = 4 * (400 - (i32)(slider * 5));
			RGFW_MEMCPY(&win->buffer[index + x_3d], color, 4);
			#endif
		}
	}
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
