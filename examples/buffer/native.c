#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION

/* Specifying 'RGFW_BUFFER_NO_CONVERSION' disables Embedded functionality to convert
 * standard buffers into a format that the deivce can understand, meaning you can 
 * _only_ use native buffers instead of being able to use either. Calling functions 
 * like 'RGFW_window_initBufferFormat' will silently fail and not display, meaning 
 * everything must be done via 'RGFW_window_initBufferNative' or 'RGFW_window_initBufferScreen'
 * with the 'is_native' option enabled.
 *
 * What formats are expected entirely depends on the system. Some systems don't
 * support RGBA8, some might have a completely different screen resolution format
 * (eg. 3DS). For any platform-specific details consult the documentation provided
 * in the RGFW_embedded.h header file. */
#define RGFW_BUFFER_NO_CONVERSION

#include <RGFW_embedded.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
	#ifndef RGFW_3DS
	RGFW_videoMode mode = RGFW_videoModeOptimal();
	#else
	/* cpu_renderer.h implements basic stereoscopic rendering for the 3DS. */
	RGFW_videoMode mode = RGFW_videoMode3D;
	#endif

	RGFW_window* win = RGFW_createWindowContextless(RGFW_windowFlagsNone);
	RGFW_bool res = RGFW_window_createContext_buffer(
		win, mode, RGFW_pixelFormatOptimal(), RGFW_TRUE
	);
	if (res == RGFW_FALSE) { return 1; }

	CPU_Surface s = surface_make(RGFW_window_getContext_buffer(win), CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

	RGFW_area win_res = RGFW_context_bufferGetResolution(s.ctx);
	#ifdef RGFW_3DS
	/* NOTE(EimaMei): Native buffers have their resolution flipped as internally 
	 * the 3DS framebuffers are treated as "portraits" that are rotated by 90 degrees
	 * clockwise. */
	i32 old_width = win_res.w;
	win_res.w = win_res.h;
	win_res.h = old_width;
	#endif
	/* NOTE(EimaMei): For stereoscopic images we have to divide the width in half
	 * since we don't want Lonic (from the left eye image) moving to the right one
	 * since that'll create a distorted view. */
	if (RGFW_context_bufferIsStereoscopic(win->buffer)) {
		win_res.w /= 2;
	}
	RGFW_rect r = RGFW_RECT(100, 100, img_lonic_width, img_lonic_height);

	while (!RGFW_window_shouldClose(win)) {
		const RGFW_event* event;
		while (RGFW_window_checkEvent(win, &event)) {
			if (event->type == RGFW_buttonPressed && event->button == BUTTON_START) {
				RGFW_window_setShouldClose(win, RGFW_TRUE);
				break;
			}
		}

		RGFW_controller* p1 = RGFW_controllerGet(win, 0);

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
		surface_clear_dirty_rects(&s);
	}

	RGFW_window_close(win);
}
