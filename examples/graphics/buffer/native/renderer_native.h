#include <SGFE.h>


typedef struct {
	isize x, y, w, h;
} CPU_Rect;

typedef struct {
	u8* data;
	isize w, h;
} CPU_Image;

typedef struct CPU_Color {
	u8 r, g, b, a; 
} CPU_Color;


typedef struct CPU_Surface {
	SGFE_contextBuffer* ctx;
	u32 clear_color;
	isize width, height;

	isize scale_multiplier;
	float scale_x, scale_y;

	isize dirty_rect_count[2];
	CPU_Rect dirty_rects[2][16];
} CPU_Surface;


/* TODO */
CPU_Surface surface_make(SGFE_contextBuffer* b, CPU_Color clear_color);

/* TODO */
CPU_Rect CPU_rectMake(isize x, isize y, isize w, isize h);
/* TODO */
CPU_Color CPU_colorMake(u8 r, u8 g, u8 b, u8 a);
/* TODO */
CPU_Image CPU_imageMake(const CPU_Surface* s, const u8* data, isize width, isize height);


/* TODO */
void surface_set_widescreen(CPU_Surface* s, SGFE_bool is_wide);


/* */
void surface_clear_dirty_rects(CPU_Surface* surface);
/* */
void surface_clear_buffers(CPU_Surface* surface);


/* */
void surface_rect(CPU_Surface* surface, CPU_Rect r, CPU_Color color);
/* */
void surface_bitmap(CPU_Surface* surface, isize x, isize y, CPU_Image img);

/* */
void surface_add_dirty_rect(CPU_Surface* surface, CPU_Rect r);





CPU_Surface surface_make(SGFE_contextBuffer* ctx, CPU_Color clear_color) {
	CPU_Surface surface;
	surface.ctx = ctx;
	surface.scale_x = 1.0f;
	surface.scale_y = 1.0f;
	surface.scale_multiplier = 1.0f;
	surface.clear_color = SGFE_platformRGB8ToYCbCr_singular(clear_color.r, clear_color.g, clear_color.b);
	surface.dirty_rect_count[0] = 0;
	surface.dirty_rect_count[1] = 0;
	SGFE_bufferGetResolution(ctx, &surface.width, &surface.height);

	return surface;
}

CPU_Rect CPU_rectMake(isize x, isize y, isize w, isize h) {
	CPU_Rect res;
	res.x = x;
	res.y = y;
	res.w = w;
	res.h = h;

	return res;
}

CPU_Color CPU_colorMake(u8 r, u8 g, u8 b, u8 a) {
	CPU_Color res;
	res.r = r;
	res.g = g;
	res.b = b;
	res.a = a;

	return res;
}

CPU_Image CPU_imageMake(const CPU_Surface* s, const u8* data, isize width, isize height) {
	CPU_Image res;
	res.w = (isize)((float)width  * s->scale_x);
	res.h = (isize)((float)height * s->scale_y);
	res.data = SGFE_ALLOC(4 * res.w * res.h);

	for (isize y = 0; y < res.h; y += 1) {
		isize scaled_y = (isize)((float)y / s->scale_y);

		for (isize x = 0; x < res.w; x += 1) {
			float scaled_x = (float)x / s->scale_x;

			SGFE_MEMCPY(
				&res.data[4 * (y * res.w + x)],
				&data[4 * (scaled_y * width + (isize)scaled_x)],
				4
			);
		}
	}
	return res;
}


void surface_set_widescreen(CPU_Surface* s, SGFE_bool is_wide) {
	if (!is_wide) {
		s->scale_x = s->scale_y = (float)s->scale_multiplier;
		return ;
	}

	isize multiply = 1;
	while (multiply * 256 < s->width) { multiply += 1; }

	s->scale_x = (float)s->width  / (256.0f * (float)multiply) * (float)s->scale_multiplier;
	s->scale_y = (float)s->height / (144.0f * (float)multiply) * (float)s->scale_multiplier;
}

void surface_clear_buffers(CPU_Surface* surface) {
	for (isize i = 0; i < SGFE_bufferGetFramebufferCount(surface->ctx); i += 1) {
		u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);

		for (isize y = 0; y < surface->height; y += 1) {
			for (isize x = 0; x < surface->width; x += 2) {
				SGFE_MEMCPY(
					&buffer[2 * (y * surface->width + x)],
					&surface->clear_color,
					4
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

	surface->dirty_rects[current][*count] = r;

	*count += 1;
}

void surface_rect(CPU_Surface* surface, CPU_Rect r, CPU_Color color) {
	r.x = (isize)((float)r.x * surface->scale_x);
	r.y = (isize)((float)r.y * surface->scale_y);
	r.w = (isize)((float)r.w * surface->scale_x);
	r.h = (isize)((float)r.h * surface->scale_y);

	surface_add_dirty_rect(surface, r);

	u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
	u32 YcbCr = SGFE_platformRGB8ToYCbCr_singular(color.r, color.g, color.b);

	for (isize y = r.y; y < r.y + r.h; y += 1) {
		for (isize x = r.x; x < r.x + r.w; x += 2) {
			SGFE_MEMCPY(
				&buffer[2 * (y * surface->width + x)],
				&YcbCr,
				4
			);
		}
	}
}

void surface_bitmap(CPU_Surface* surface, isize x, isize y, CPU_Image img) {
	CPU_Rect r = CPU_rectMake(x, y, img.w, img.h);
	r.x = (isize)((float)r.x * surface->scale_x);
	r.y = (isize)((float)r.y * surface->scale_y);

	surface_add_dirty_rect(surface, r);

	u32* buffer = (void*)SGFE_bufferGetFramebuffer(surface->ctx);
	buffer += (r.x + r.y * surface->width) / 2;

	for (y = 0; y < r.h; y += 1) {
		for (x = 0; x < r.w; x += 2) {
			u8* src = &img.data[4 * (y * r.w + x)];
			u32 YcbCr = SGFE_platformRGB8ToYCbCr(src[0], src[1], src[2], src[4], src[5], src[6]);

			buffer[x / 2] = YcbCr;
		}
		buffer += surface->width / 2;
	}
}

void surface_clear_dirty_rects(CPU_Surface* surface) {
	isize current = SGFE_contextBufferGetCurrent(surface->ctx);

	for (isize i = 0; i < surface->dirty_rect_count[current]; i += 1) {
		u8* buffer = SGFE_bufferGetFramebuffer(surface->ctx);
		CPU_Rect r = surface->dirty_rects[current][i];

		for (isize y = r.y; y < r.y + r.h; y += 1) {
			for (isize x = r.x; x < r.x + r.w; x += 2) {
				SGFE_MEMCPY(
					&buffer[2 * (y * surface->width + x)],
					&surface->clear_color,
					4
				);
			}
		}
	}

	surface->dirty_rect_count[current] = 0;
}
