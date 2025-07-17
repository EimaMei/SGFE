#define RGFW_BUFFER
#include <RGFW_embedded.h>


#ifndef RGFW_BUFFER_NATIVE
    typedef struct CPU_Color { u8 r, g, b, a; } CPU_Color;
    typedef RGFW_rect CPU_DirtyRect;
#elif defined(RGFW_3DS)
    typedef u32 CPU_Color;
    typedef struct {
        RGFW_rect r;
        i32 slider_offset;
    }  CPU_DirtyRect;
#endif

#ifndef CPU_colorMake
#define CPU_colorMake(r, g, b, a) CPU_colorRGBA(r, g, b, a)
#endif


typedef struct CPU_Surface {
    RGFW_window* win;
    CPU_Color clear_color;

    ssize_t dirty_rect_count[2];
    CPU_DirtyRect dirty_rects[2][16];
} CPU_Surface;


/* */
CPU_Surface surface_make(RGFW_window* win, CPU_Color clear_color);

/* */
void surface_flush(CPU_Surface* surface);
/* */
void surface_clearBuffers(CPU_Surface* surface);


/* */
void surface_rect(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color);
/* */
void surface_bitmap(CPU_Surface* surface, RGFW_rect r, u8* bitmap);

/* */
void surface_add_dirty_rect(CPU_Surface* surface, RGFW_rect r);


#ifdef RGFW_BUFFER_NATIVE
/* */
void surface_rect_platform(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color);
/* */
void surface_bitmap_platform(CPU_Surface* surface, RGFW_rect r, u8* bitmap);

/* */
void surface_add_dirty_rect_platform(CPU_Surface* surface, RGFW_rect r);
/* */
void surface_clear_dirty_rect_platform(CPU_Surface* surface, CPU_DirtyRect* dirty_rect);
#endif

/* */
CPU_Color CPU_colorRGBA(u8 r, u8 g, u8 b, u8 a);
/* */
static inline CPU_Color CPU_colorRGB(u8 r, u8 g, u8 b) { return CPU_colorRGBA(r, g, b, 255); }



CPU_Surface surface_make(RGFW_window* win, CPU_Color clear_color) {
    CPU_Surface surface;
    surface.win = win;
    surface.clear_color = clear_color;
    surface.dirty_rect_count[0] = 0;
    surface.dirty_rect_count[1] = 0;
    return surface;
}


void surface_clearBuffers(CPU_Surface* surface) {
    RGFW_window* win = surface->win;
    i32 bpp = RGFW_pixelFormatBPP(win->format);

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
    ssize_t cur_buf = surface->win->src.current_buffer;
    ssize_t* count = &surface->dirty_rect_count[cur_buf];
    if (*count >= RGFW_COUNTOF(surface->dirty_rects)) {
        surface_flush(surface);
    }

#ifndef RGFW_BUFFER_NATIVE
    surface->dirty_rects[cur_buf][*count] = r;
#else
    surface_add_dirty_rect_platform(surface, r);
#endif

    *count += 1;
}

void surface_rect(CPU_Surface* surface, RGFW_rect r, CPU_Color clear_color) {
    surface_add_dirty_rect(surface, r);

#ifndef RGFW_BUFFER_NATIVE
    RGFW_window* win = surface->win;
    i32 bpp = RGFW_pixelFormatBPP(win->format);

	for (ssize_t y = r.y; y < r.y + r.h; y += 1) {
		for (ssize_t x = r.x; x < r.x + r.w; x += 1) {
			ssize_t index = bpp * (y * win->bufferSize.w + x);

			RGFW_MEMCPY(&win->buffer[index], &clear_color, (size_t)bpp);
		}
	}
#else
	surface_rect_platform(surface, r, clear_color);
#endif
}

void surface_bitmap(CPU_Surface* surface, RGFW_rect r, u8* bitmap) {
    surface_add_dirty_rect(surface, r);

#ifndef RGFW_BUFFER_NATIVE
    RGFW_window* win = surface->win;

	for (ssize_t y = 0; y < r.h; y += 1) {
		for (ssize_t x = 0; x < r.w; x += 1) {
			ssize_t dst_index = 4 * ((r.y + y) * win->bufferSize.w + (r.x + x));
			ssize_t src_index = 4 * (y * r.w + x);

			RGFW_MEMCPY(&win->buffer[dst_index], &bitmap[src_index], 4);
		}
	}
#else
	surface_bitmap_platform(surface, r, bitmap);
#endif
}

void surface_flush(CPU_Surface* surface) {
    RGFW_window* win = surface->win;
    RGFW_window_swapBuffers_buffer(win);

    ssize_t cur_buf = win->src.current_buffer;
    for (ssize_t i = 0; i < surface->dirty_rect_count[cur_buf]; i += 1) {
        #ifndef RGFW_BUFFER_NATIVE
            RGFW_rect r = surface->dirty_rects[cur_buf][i];
            i32 bpp = RGFW_pixelFormatBPP(win->format);

            for (ssize_t y = r.y; y < r.y + r.h; y += 1) {
                for (ssize_t x = r.x; x < r.x + r.w; x += 1) {
                    ssize_t index = bpp * (y * win->bufferSize.w + x);

                    RGFW_MEMCPY(&win->buffer[index], &surface->clear_color, (size_t)bpp);
                }
            }
        #else
            surface_clear_dirty_rect_platform(surface, &surface->dirty_rects[cur_buf][i]);
        #endif
    }

    surface->dirty_rect_count[cur_buf] = 0;
}

#ifndef RGFW_BUFFER_NATIVE

CPU_Color CPU_colorRGBA(u8 r, u8 g, u8 b, u8 a) {
    CPU_Color res;
    res.r = r;
    res.g = g;
    res.b = b;
    res.a = a;

    return res;
}

#endif
