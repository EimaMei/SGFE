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
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowNoInitAPI);
	#else
	RGFW_window* win = RGFW_createWindow(RGFW_videoMode3D, RGFW_windowNoInitAPI);
	#endif
	RGFW_window_initBufferSize(win, win->mode, RGFW_pixelFormatOptimal());

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

	CPU_Surface s = surface_make(win, CPU_colorMake(255, 255, 255, 255));
	surface_clearBuffers(&s);

	while (!RGFW_window_shouldClose(win)) {
		while (RGFW_TRUE) {
			const RGFW_event* event = RGFW_window_checkEvent(win);
			if (event == NULL) { break; }

			if (event->type == RGFW_buttonPressed && event->button == BUTTON_START) {
				RGFW_window_setShouldClose(win, RGFW_TRUE);
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

		RGFW_window_swapBuffers_buffer(win);
		surface_clearDirtyRects(&s);
	}

	RGFW_window_close(win);
}

#if 1

CPU_Color CPU_colorRGBA(u8 r, u8 g, u8 b, u8 a) {
	CPU_Color res = 0;
	res |= (u32)a << 24;
	res |= (u32)r << 16;
	res |= (u32)g <<  8;
	res |= (u32)b <<  0;

	return res;
}

void surface_bitmap_platform(CPU_Surface* surface, RGFW_rect r, u8* bitmap) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(win->format);

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			u8* rgba = &bitmap[4 * ((j - r.y) * r.w + (i - r.x))];
			CPU_Color color = CPU_colorRGBA(rgba[0], rgba[1], rgba[2], rgba[3]);

			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&color, (size_t)bpp
			);
		}
	}

	ssize_t cur_buf = surface->win->src.current_buffer[_RGFW->current];
	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[cur_buf][surface->dirty_rect_count[cur_buf] - 1];
	if (_RGFW->current == RGFW_SCREEN_BOTTOM || dirty_rect->slider_offset == 0) { return; }


	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			u8* rgba = &bitmap[4 * ((j - r.y) * r.w + (i - r.x))];
			CPU_Color color = CPU_colorRGBA(rgba[0], rgba[1], rgba[2], rgba[3]);

			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&color, (size_t)bpp
			);
		}
	}
}

void surface_rect_platform(CPU_Surface* surface, RGFW_rect r, CPU_Color color) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(win->format);

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}

	ssize_t cur_buf = surface->win->src.current_buffer[_RGFW->current];
	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[cur_buf][surface->dirty_rect_count[cur_buf] - 1];
	if (_RGFW->current == 1 || dirty_rect->slider_offset == 0) { return; }

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}
}

void surface_add_dirty_rect_platform(CPU_Surface* surface, RGFW_rect r) {
	ssize_t cur_buf = surface->win->src.current_buffer[_RGFW->current];

	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[cur_buf][surface->dirty_rect_count[cur_buf]];
	dirty_rect->r = r;
	dirty_rect->slider_offset = (i32)(RGFW_platformGet3DSlider() * 5);
}

void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(win->format);

	RGFW_rect r = dirty_rect->r;
	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&surface->clear_color,
				(size_t)bpp
			);
		}
	}

	if (_RGFW->current == 1 || dirty_rect->slider_offset == 0) {
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&surface->clear_color,
				(size_t)bpp
			);
		}
	}
}

#endif
