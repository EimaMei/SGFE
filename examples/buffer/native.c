#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG
#define RGFW_BUFFER_NATIVE
#define RGFW_BUFFER

#include <RGFW_embedded.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


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
			if (win->event->type == RGFW_quit || RGFW_isPressed(win->event->controller, BUTTON_START)) {
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

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

			ssize_t opixel = 4 * ((j - r.y) * r.w + (i - r.x));
			win->buffer[pixel_left + 0] = bitmap[opixel + 3];
			win->buffer[pixel_left + 1] = bitmap[opixel + 2];
			win->buffer[pixel_left + 2] = bitmap[opixel + 1];
			win->buffer[pixel_left + 3] = bitmap[opixel + 0];
		}
	}

	ssize_t cur_buf = surface->win->src.current_buffer;
	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[cur_buf][surface->dirty_rect_count[cur_buf] - 1];
	if (dirty_rect->slider_offset == 0) { return; }


	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t pixel_left = 4 * (i * 240 + (240 - j));

			ssize_t opixel = 4 * ((j - r.y) * r.w + (i - r.x));
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

	ssize_t cur_buf = surface->win->src.current_buffer;
	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[cur_buf][surface->dirty_rect_count[cur_buf] - 1];
	if (dirty_rect->slider_offset == 0) { return; }

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t src = 4 * (i * 240 + (240 - j));
			RGFW_MEMCPY(&win->buffer[src], &clear_color, 4);
		}
	}
}

void surface_add_dirty_rect_platform(CPU_Surface* surface, RGFW_rect r) {
	ssize_t cur_buf = surface->win->src.current_buffer;

	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[cur_buf][surface->dirty_rect_count[cur_buf]];
	dirty_rect->r = r;
	dirty_rect->slider_offset = (i32)(RGFW_platformGet3DSlider() * 5);
}

void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect) {
	RGFW_window* win = surface->win;

	RGFW_rect r = dirty_rect->r;
	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t src = 4 * (i * 240 + (240 - j));
			RGFW_MEMCPY(&win->buffer[src], &surface->clear_color, 4);
		}
	}

	if (dirty_rect->slider_offset == 0) {
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			ssize_t src = 4 * (i * 240 + (240 - j));
			RGFW_MEMCPY(&win->buffer[src], &surface->clear_color, 4);
		}
	}
}

#endif
