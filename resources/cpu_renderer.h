#include <SGFE.h>


typedef struct { isize x, y, w, h; } CPU_Rect;

static inline CPU_Rect CPU_rectMake(isize x, isize y, isize w, isize h) {
	CPU_Rect res;
	res.x = x;
	res.y = y;
	res.w = w;
	res.h = h;

	return res;
}

#ifdef SGFE_3DS

typedef struct {
	CPU_Rect r;
	i32 slider_offset;
}  CPU_DirtyRect;

static inline CPU_Rect* CPU_DirtyRectGetRect(CPU_DirtyRect* r) { return &r->r; }

#else 

typedef CPU_Rect CPU_DirtyRect;
static inline CPU_Rect* CPU_DirtyRectGetRect(CPU_DirtyRect* r) { return &r; }

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
	SGFE_contextBuffer* ctx;
	CPU_Color clear_color;
	isize width, height;

	isize dirty_rect_count[2];
	CPU_DirtyRect dirty_rects[2][16];
} CPU_Surface;


/* */
CPU_Surface surface_make(SGFE_contextBuffer* b, CPU_Color clear_color);

/* */
void surface_clear_dirty_rects(CPU_Surface* surface);
/* */
void surface_clear_buffers(CPU_Surface* surface);


/* */
void surface_rect(CPU_Surface* surface, CPU_Rect r, CPU_Color clear_color);
/* */
void surface_bitmap(CPU_Surface* surface, CPU_Rect r, u8* bitmap);

/* */
void surface_add_dirty_rect(CPU_Surface* surface, CPU_Rect r);
/* */
CPU_DirtyRect* surface_get_last_dirty_rect(CPU_Surface* surface);


/* */
void surface_rect_platform(CPU_Surface* surface, CPU_Rect r, CPU_Color clear_color);
/* */
void surface_bitmap_platform(CPU_Surface* surface, CPU_Rect r, u8* bitmap);

/* */
void surface_add_dirty_rect_platform(CPU_Surface* surface, CPU_Rect r);
/* */
void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect);




CPU_Surface surface_make(SGFE_contextBuffer* ctx, CPU_Color clear_color) {
	CPU_Surface surface;
	surface.ctx = ctx;
	surface.clear_color = clear_color;
	surface.dirty_rect_count[0] = 0;
	surface.dirty_rect_count[1] = 0;
	SGFE_bufferGetResolution(ctx, &surface.width, &surface.height);

	return surface;
}


void surface_clear_buffers(CPU_Surface* surface) {
	isize bpp = SGFE_bufferFormatGetBytesPerPixel(SGFE_bufferGetFormat(surface->ctx));

	for (isize i = 0; i < SGFE_bufferGetFramebufferCount(surface->ctx); i += 1) {
		u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);

		for (isize y = 0; y < surface->height; y += 1) {
			for (isize x = 0; x < surface->width; x += 1) {
				SGFE_MEMCPY(
					&buffer[bpp * (y * surface->width + x)],
					&surface->clear_color, 
					(size_t)bpp
				);
			}
		}

		SGFE_bufferFlipFramebuffers(surface->ctx);
	}
}


void surface_add_dirty_rect(CPU_Surface* surface, CPU_Rect r) {
	isize current = SGFE_contextBufferGetCurrent(surface->ctx);

	isize* count = &surface->dirty_rect_count[current];
	if (*count >= SGFE_COUNTOF(surface->dirty_rects)) {
		surface_clear_dirty_rects(surface);
	}

	if (SGFE_bufferIsNative(surface->ctx)) {
		surface_add_dirty_rect_platform(surface, r);
	}
	else {
		CPU_Rect* out_r = CPU_DirtyRectGetRect(&surface->dirty_rects[current][*count]);
		*out_r = r;
	}

	*count += 1;
}

CPU_DirtyRect* surface_get_last_dirty_rect(CPU_Surface* surface) {
	isize current = SGFE_contextBufferGetCurrent(surface->ctx);
	return &surface->dirty_rects[current][surface->dirty_rect_count[current] - 1];
}

void surface_rect(CPU_Surface* surface, CPU_Rect r, CPU_Color clear_color) {
	surface_add_dirty_rect(surface, r);

	if (SGFE_bufferIsNative(surface->ctx)) {
		surface_rect_platform(surface, r, clear_color);
		return ;
	}

	u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
	for (isize y = r.y; y < r.y + r.h; y += 1) {
		for (isize x = r.x; x < r.x + r.w; x += 1) {
			SGFE_MEMCPY(
				&buffer[4 * (y * surface->width + x)],
				&clear_color,
				4
			);
		}
	}
}

void surface_bitmap(CPU_Surface* surface, CPU_Rect r, u8* bitmap) {
	surface_add_dirty_rect(surface, r);

	if (SGFE_bufferIsNative(surface->ctx)) {
		surface_bitmap_platform(surface, r, bitmap);
		return ;
	}

	u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
	for (isize y = 0; y < r.h; y += 1) {
		for (isize x = 0; x < r.w; x += 1) {
			SGFE_MEMCPY(
				&buffer[4 * ((r.y + y) * surface->width + (r.x + x))],
				&bitmap[4 * (y * r.w + x)],
				4
			);
		}
	}
}

void surface_clear_dirty_rects(CPU_Surface* surface) {
	isize current = SGFE_contextBufferGetCurrent(surface->ctx);

	for (isize i = 0; i < surface->dirty_rect_count[current]; i += 1) {
		CPU_DirtyRect* dirty_rect = &surface->dirty_rects[current][i];
		if (SGFE_bufferIsNative(surface->ctx)) {
			surface_clear_dirty_rect_platform(surface, dirty_rect);
			continue;
		}

		u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
		CPU_Rect r = *CPU_DirtyRectGetRect(dirty_rect);

		for (isize y = r.y; y < r.y + r.h; y += 1) {
			for (isize x = r.x; x < r.x + r.w; x += 1) {
				SGFE_MEMCPY(
					&buffer[4 * (y * surface->width + x)],
					&surface->clear_color,
					4
				);
			}
		}
	}

	surface->dirty_rect_count[current] = 0;
}


#ifdef SGFE_3DS

static inline CPU_Color CPU_colorToNative(CPU_Color clr) {
	CPU_Color res;
	res.r = clr.b;
	res.g = clr.g;
	res.b = clr.r;
	res.a = 0xFF;

	return res;
}

void surface_bitmap_platform(CPU_Surface* surface, CPU_Rect r, u8* bitmap) {
	u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
	i32 bpp = SGFE_bufferFormatGetBytesPerPixel(SGFE_bufferGetFormat(surface->ctx));

	isize width = r.x + r.w, height = r.y + r.h;
	for (isize i = r.x; i < width; i += 1) {
		for (isize j = height - 1; j >= r.y; j -= 1) {
			u8* rgba = &bitmap[4 * ((j - r.y) * r.w + (i - r.x))];
			CPU_Color color = CPU_colorRGBA(rgba[2], rgba[1], rgba[0], 0xFF);

			SGFE_MEMCPY(
				&buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}

	CPU_DirtyRect* dirty_rect = surface_get_last_dirty_rect(surface);
	if (dirty_rect->slider_offset == 0 || surface->ctx->screen == SGFE_screenBottom) {
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (isize i = r.x; i < width; i += 1) {
		for (isize j = height - 1; j >= r.y; j -= 1) {
			u8* rgba = &bitmap[4 * ((j - r.y) * r.w + (i - r.x))];
			CPU_Color color = CPU_colorRGBA(rgba[2], rgba[1], rgba[0], 0xFF);

			SGFE_MEMCPY(
				&buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}
}

void surface_rect_platform(CPU_Surface* surface, CPU_Rect r, CPU_Color color) {
	u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
	i32 bpp = SGFE_bufferFormatGetBytesPerPixel(SGFE_bufferGetFormat(surface->ctx));
	color = CPU_colorToNative(color);

	isize width = r.x + r.w, height = r.y + r.h;
	for (isize i = r.x; i < width; i += 1) {
		for (isize j = height - 1; j >= r.y; j -= 1) {
			SGFE_MEMCPY(
				&buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}

	CPU_DirtyRect* dirty_rect = surface_get_last_dirty_rect(surface);
	if (dirty_rect->slider_offset == 0 || surface->ctx->screen == SGFE_screenBottom) {
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w;
	for (isize i = r.x; i < width; i += 1) {
		for (isize j = height - 1; j >= r.y; j -= 1) {
			SGFE_MEMCPY(
				&buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}
}

void surface_add_dirty_rect_platform(CPU_Surface* surface, CPU_Rect r) {
	isize current = SGFE_contextBufferGetCurrent(surface->ctx);

	CPU_DirtyRect* dirty_rect = &surface->dirty_rects[current][surface->dirty_rect_count[current]];
	dirty_rect->r = r;
	dirty_rect->slider_offset = (i32)(SGFE_platformGet3DSlider() * 5);
}

void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect) {
	u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
	i32 bpp = SGFE_bufferFormatGetBytesPerPixel(SGFE_bufferGetFormat(surface->ctx));
	CPU_Color color = CPU_colorToNative(surface->clear_color);

	CPU_Rect r = dirty_rect->r;
	isize width = r.x + r.w, height = r.y + r.h;
	for (isize i = r.x; i < width; i += 1) {
		for (isize j = height - 1; j >= r.y; j -= 1) {
			SGFE_MEMCPY(
				&buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}

	if (dirty_rect->slider_offset == 0 || surface->ctx->screen == SGFE_screenBottom) {
		return;
	}

	r.x += 400 - dirty_rect->slider_offset;
	width = r.x + r.w, height = r.y + r.h;
	for (isize i = r.x; i < width; i += 1) {
		for (isize j = height - 1; j >= r.y; j -= 1) {
			SGFE_MEMCPY(
				&buffer[bpp * (i * 240 + (240 - j))],
				&color,
				(size_t)bpp
			);
		}
	}
}

#endif
