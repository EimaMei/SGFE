#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG
#define RGFW_BUFFER_NATIVE
#define RGFW_BUFFER

#include <RGFW_embedded.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


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

	/* NOTE(EimaMei): 'RGFW_windowGetSize(win)' may actually differ to 'win->bufferSize' 
	 * in certain situations where the viewport for the window is actually smaller 
	 * than the rendered buffer. 
	 *
	 * Platforms where it differs:
	 * - 3DS (when using RGFW_videoMode3D) - internally the buffer is 800x240, 
	 * however the viewport is still set to 400x240 as the second half of the 
	 * resolution is used for the 3D effect. */
	RGFW_area win_res = RGFW_windowGetSize(win);

	// todo: swap function and flush function.
	CPU_Surface s = surface_make(win, CPU_colorMake(255, 255, 255, 255));
	surface_clearBuffers(&s);

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
		else if (RGFW_isPressed(p1, BUTTON_RIGHT) && r.x + r.w < win_res.w) {
			r.x += 1;
		}

		if (RGFW_isPressed(p1, BUTTON_UP) && r.y > 0) {
			r.y -= 1;
		}
		else if (RGFW_isPressed(p1, BUTTON_DOWN) && r.y + r.h < win_res.h) {
			r.y += 1;
		}

		surface_rect(&s, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, r, img_lonic_data);

		surface_flush(&s);
	}

	RGFW_window_close(win);
}

#if 1

CPU_Color CPU_colorRGBA(u8 r, u8 g, u8 b, u8 a) {
    CPU_Color res = 0;
    res |= (u32)r <<  8;
    res |= (u32)g << 16;
    res |= (u32)b << 24;
    res |= (u32)a <<  0;

    return res;
}

void surface_bitmap_platform(CPU_Surface* surface, RGFW_rect r, u8* bitmap) {
	RGFW_window* win = surface->win;

	float slider = RGFW_systemGet3DSlider();
	RGFW_bool skip = (slider == 0.0f);

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

			ssize_t opixel = 4 * ((j - r.y) * r.w + (i - r.x));
			win->buffer[pixel_left + 0] = bitmap[opixel + 3];
			win->buffer[pixel_left + 1] = bitmap[opixel + 2];
			win->buffer[pixel_left + 2] = bitmap[opixel + 1];
			win->buffer[pixel_left + 3] = bitmap[opixel + 0];

			if (skip) {
				continue;
			}

			pixel_left = 4 * ((i + 400 - (i32)(slider * 5)) * 240 + (240 - j));
			win->buffer[pixel_left + 0] = bitmap[opixel + 3];
			win->buffer[pixel_left + 1] = bitmap[opixel + 2];
			win->buffer[pixel_left + 2] = bitmap[opixel + 1];
			win->buffer[pixel_left + 3] = bitmap[opixel + 0];
		}
	}
}

void surface_rect_platform(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color) {
	RGFW_window* win = surface->win;

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t src = 4 * (i * 240 + (240 - j));
			RGFW_MEMCPY(&win->buffer[src], &clear_color, 4);
		}
	}

	float slider = RGFW_systemGet3DSlider();

	if (slider == 0.0f) {
		return;
	}

	r.x += 400 - (i32)(slider * 5);
	width = r.x + r.w;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t src = 4 * (i * 240 + (240 - j));
			RGFW_MEMCPY(&win->buffer[src], &clear_color, 4);
		}
	}
}

#endif