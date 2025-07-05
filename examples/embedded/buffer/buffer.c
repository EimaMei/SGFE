#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER
//#define RGFW_OPENGL
#include <stdio.h>
#define RGFW_DEBUG
#include <RGFW_embedded.h>
#include "../resources/lonic_bin.h"

RGFW_area screenSize;

/* fill buffer with a color, clearing anything that was on it */
void clear(RGFW_window* win, u8 color[4]) {
    for (ssize_t y = 0; y < screenSize.h; y += 1) {
		for (ssize_t x = 0; x < screenSize.w; x += 1) {
            ssize_t index = 4 * (y * screenSize.w + x);
			RGFW_MEMCPY(&win->buffer[index], color, 4);
        }
    }
}


#define si_swap(a, b) do { \
	__typeof__(a) tmp = a; \
	a = b; \
	b = tmp; \
} while (0)

void drawBitmap(RGFW_window* win, u8* bitmap, RGFW_rect r) {
#ifndef RGFW_BUFFER_NATIVE
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

#else
	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

			ssize_t opixel = 4 * ((height - 1 - j) * width + i);
			win->buffer[pixel_left + 0] = bitmap[opixel + 3];
			win->buffer[pixel_left + 1] = bitmap[opixel + 2];
			win->buffer[pixel_left + 2] = bitmap[opixel + 1];
			win->buffer[pixel_left + 3] = bitmap[opixel + 0];
            //(&win->buffer[pixel_left], bitmap, 4);
			//si_swap(win->buffer[pixel_left + 0], win->buffer[pixel_left + 3]);
			//si_swap(win->buffer[pixel_left + 1], win->buffer[pixel_left + 2]);

			///ssize_t pixel_right = pixel_left + win->src.framebuffer_size / 2 - x_3d;
            ///RGFW_MEMCPY(&win->buffer[pixel_right], bitmap, 4);
        }
    }
#endif

}


void drawRect(RGFW_window* win, RGFW_rect r, u8 color[4]) {
#ifndef RGFW_BUFFER_NATIVE

	for (ssize_t y = r.y; y < r.y + r.h; y += 1) {
		for (ssize_t x = r.x; x < r.x + r.w; x += 1) {
			ssize_t index = 4 * (y * win->bufferSize.w + x);

			RGFW_MEMCPY(&win->buffer[index], color, 4);

			#ifdef RGFW_3DS
			float slider = osGet3DSliderState();
			i32 x_3d = 4 * (400 - (i32)(slider * 5));
            RGFW_MEMCPY(&win->buffer[index + x_3d], color, 4);
			#endif
        }
    }

#else
	si_swap(color[0], color[3]);
	si_swap(color[1], color[2]);

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

            RGFW_MEMCPY(&win->buffer[pixel_left], color, 4);

			ssize_t pixel_right = pixel_left + win->src.framebuffer_size / 2 - x_3d;
            RGFW_MEMCPY(&win->buffer[pixel_right], color, 4);
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

/* TODO(EimaMei): Split this into 3 examples: simple, advance, fully native. */
int main(void) {
    RGFW_window* win = RGFW_createWindow(RGFW_videoMode3D, 0);
    screenSize = win->bufferSize;

    i8 running = 1;
	RGFW_rect r = RGFW_RECT(100, 100, img_lonic_width, img_lonic_height);

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
		else if (RGFW_isPressed(p1, BUTTON_RIGHT) && r.x + r.w < screenSize.w) {
			r.x += 1;
		}

		if (RGFW_isPressed(p1, BUTTON_UP) && r.y > 0) {
			r.y -= 1;
		}
		else if (RGFW_isPressed(p1, BUTTON_DOWN) && r.y + r.h < screenSize.h) {
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
