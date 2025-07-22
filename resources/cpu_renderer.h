#include <RGFW_embedded.h>

#ifdef RGFW_3DS

typedef struct {
	RGFW_rect r;
	i32 slider_offset;
}  CPU_DirtyRect;

static inline RGFW_rect* CPU_DirtyRectGetRect(CPU_DirtyRect* r) { return &r->r; }

#else 

typedef RGFW_rect CPU_DirtyRect;
static inline RGFW_rect* CPU_DirtyRectGetRect(CPU_DirtyRect* r) { return &r; }

#endif

#ifndef CPU_colorMake
#define CPU_colorMake(r, g, b, a) CPU_colorRGBA(r, g, b, a)
#endif

typedef struct CPU_Color {
	u8 r, g, b, a; 
} CPU_Color;

static inline CPU_Color CPU_colorRGBA(u8 r, u8 g, u8 b, u8 a) {
	CPU_Color res;
	res.r = r;
	res.g = g;
	res.b = b;
	res.a = a;

	return res;
}

static inline CPU_Color CPU_colorRGB(u8 r, u8 g, u8 b) { return CPU_colorRGBA(r, g, b, 255); }

typedef struct CPU_Surface {
	RGFW_window* win;
	CPU_Color clear_color;

	ssize_t dirty_rect_count[2];
	CPU_DirtyRect dirty_rects[2][16];
} CPU_Surface;


/* */
CPU_Surface surface_make(RGFW_window* win, CPU_Color clear_color);

/* */
void surface_clear_dirty_rects(CPU_Surface* surface);
/* */
void surface_clear_buffers(CPU_Surface* surface);


/* */
void surface_rect(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color);
/* */
void surface_bitmap(CPU_Surface* surface, RGFW_rect r, u8* bitmap);

/* */
void surface_add_dirty_rect(CPU_Surface* surface, RGFW_rect r);
/* */
CPU_DirtyRect* surface_get_last_dirty_rect(CPU_Surface* surface);


/* */
void surface_rect_platform(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color);
/* */
void surface_bitmap_platform(CPU_Surface* surface, RGFW_rect r, u8* bitmap);

/* */
void surface_add_dirty_rect_platform(CPU_Surface* surface, RGFW_rect r);
/* */
void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect);




CPU_Surface surface_make(RGFW_window* win, CPU_Color clear_color) {
	CPU_Surface surface;
	surface.win = win;
	surface.clear_color = clear_color;
	surface.dirty_rect_count[0] = 0;
	surface.dirty_rect_count[1] = 0;
	return surface;
}


void surface_clear_buffers(CPU_Surface* surface) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(RGFW_window_bufferGetFormat(win));

	for (ssize_t i = 0; i < 2; i += 1) {
		for (ssize_t y = 0; y < win->bufferSize.h; y += 1) {
			for (ssize_t x = 0; x < win->bufferSize.w; x += 1) {
				ssize_t index = bpp * (y * win->bufferSize.w + x);
				RGFW_MEMCPY(&win->buffer[index], &surface->clear_color, (size_t)bpp);
			}
		}
		RGFW_window_swapBuffers_buffer(win);
	}
}


void surface_add_dirty_rect(CPU_Surface* surface, RGFW_rect r) {
	ssize_t current = RGFW_window_bufferGetCurrent(surface->win);
	ssize_t* count = &surface->dirty_rect_count[current];
	if (*count >= RGFW_COUNTOF(surface->dirty_rects)) {
		surface_clear_dirty_rects(surface);
	}

	if (RGFW_window_bufferIsNative(surface->win)) {
		surface_add_dirty_rect_platform(surface, r);
	}
	else {
		RGFW_rect* out_r = CPU_DirtyRectGetRect(&surface->dirty_rects[current][*count]);
		*out_r = r;
	}

	*count += 1;
}

CPU_DirtyRect* surface_get_last_dirty_rect(CPU_Surface* surface) {
	ssize_t current = RGFW_window_bufferGetCurrent(surface->win);
	return &surface->dirty_rects[current][surface->dirty_rect_count[current] - 1];
}

void surface_rect(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color) {
	surface_add_dirty_rect(surface, r);

	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(RGFW_window_bufferGetFormat(win));

	if (RGFW_window_bufferIsNative(win)) {
		surface_rect_platform(surface, r, clear_color);
		return ;
	}

	for (ssize_t y = r.y; y < r.y + r.h; y += 1) {
		for (ssize_t x = r.x; x < r.x + r.w; x += 1) {
			ssize_t index = bpp * (y * win->bufferSize.w + x);

			RGFW_MEMCPY(&win->buffer[index], &clear_color, (size_t)bpp);
		}
	}
}

void surface_bitmap(CPU_Surface* surface, RGFW_rect r, u8* bitmap) {
	surface_add_dirty_rect(surface, r);

	RGFW_window* win = surface->win;
	if (RGFW_window_bufferIsNative(win)) {
		surface_bitmap_platform(surface, r, bitmap);
		return ;
	}

	for (ssize_t y = 0; y < r.h; y += 1) {
		for (ssize_t x = 0; x < r.w; x += 1) {
			ssize_t dst_index = 4 * ((r.y + y) * win->bufferSize.w + (r.x + x));
			ssize_t src_index = 4 * (y * r.w + x);

			RGFW_MEMCPY(&win->buffer[dst_index], &bitmap[src_index], 4);
		}
	}
}

void surface_clear_dirty_rects(CPU_Surface* surface) {
	RGFW_window* win = surface->win;

	ssize_t current = RGFW_window_bufferGetCurrent(win);
	RGFW_bool is_native = RGFW_window_bufferIsNative(win);

	for (ssize_t i = 0; i < surface->dirty_rect_count[current]; i += 1) {
		CPU_DirtyRect* dirty_rect = &surface->dirty_rects[current][i];
		if (is_native) {
			surface_clear_dirty_rect_platform(surface, dirty_rect);
			continue;
		}
	
		RGFW_rect r = *CPU_DirtyRectGetRect(dirty_rect);
		i32 bpp = RGFW_pixelFormatBPP(RGFW_window_bufferGetFormat(win));

		for (ssize_t y = r.y; y < r.y + r.h; y += 1) {
			for (ssize_t x = r.x; x < r.x + r.w; x += 1) {
				ssize_t index = bpp * (y * win->bufferSize.w + x);

				RGFW_MEMCPY(&win->buffer[index], &surface->clear_color, (size_t)bpp);
			}
		}
	}

	surface->dirty_rect_count[current] = 0;
}


#ifdef RGFW_3DS

static inline CPU_Color CPU_colorToNative(CPU_Color clr) {
	CPU_Color res;
	res.r = clr.b;
	res.g = clr.g;
	res.b = clr.r;
	res.a = 0xFF;

	return res;
}

void surface_bitmap_platform(CPU_Surface* surface, RGFW_rect r, u8* bitmap) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(RGFW_window_bufferGetFormat(win));

	ssize_t width = r.x + r.w, height = r.y + r.h;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			u8* rgba = &bitmap[4 * ((j - r.y) * r.w + (i - r.x))];
			CPU_Color color = CPU_colorRGBA(rgba[2], rgba[1], rgba[0], 0xFF);

			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}

	CPU_DirtyRect* dirty_rect = surface_get_last_dirty_rect(surface);
	if (RGFW_getCurrent(win) == RGFW_screenBottom || dirty_rect->slider_offset == 0) { 
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (ssize_t i = r.x; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= r.y; j -= 1) {
			u8* rgba = &bitmap[4 * ((j - r.y) * r.w + (i - r.x))];
			CPU_Color color = CPU_colorRGBA(rgba[2], rgba[1], rgba[0], 0xFF);

			RGFW_MEMCPY(
				&win->buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}
}

void surface_rect_platform(CPU_Surface* surface, RGFW_rect r, CPU_Color color) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(RGFW_window_bufferGetFormat(win));
	color = CPU_colorToNative(color);

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

	CPU_DirtyRect* dirty_rect = surface_get_last_dirty_rect(surface);
	if (RGFW_getCurrent(win) == RGFW_screenBottom || dirty_rect->slider_offset == 0) { 
		return;
	}

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
	ssize_t current = RGFW_window_bufferGetCurrent(surface->win);

	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[current][surface->dirty_rect_count[current]];
	dirty_rect->r = r;
	dirty_rect->slider_offset = (i32)(RGFW_platformGet3DSlider() * 5);
}

void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect) {
	RGFW_window* win = surface->win;
	i32 bpp = RGFW_pixelFormatBPP(RGFW_window_bufferGetFormat(win));
	CPU_Color color = CPU_colorToNative(surface->clear_color);

	RGFW_rect r = dirty_rect->r;
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

	if (RGFW_getCurrent(win) == RGFW_screenBottom || dirty_rect->slider_offset == 0) { 
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w, height = r.y + r.h;
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

#endif
