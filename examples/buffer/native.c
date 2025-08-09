#define SGFE_DEBUG
#define SGFE_IMPLEMENTATION

/* Specifying 'SGFE_BUFFER_NO_CONVERSION' disables Embedded functionality to convert
 * standard buffers into a format that the deivce can understand, meaning you can 
 * _only_ use native buffers instead of being able to use either. Calling functions 
 * like 'SGFE_window_initBufferFormat' will silently fail and not display, meaning 
 * everything must be done via 'SGFE_window_initBufferNative' or 'SGFE_window_initBufferScreen'
 * with the 'is_native' option enabled.
 *
 * What formats are expected entirely depends on the system. Some systems don't
 * support RGBA8, some might have a completely different screen resolution format
 * (eg. 3DS). For any platform-specific details consult the documentation provided
 * in the SGFE.h header file. */
#define SGFE_BUFFER_NO_CONVERSION

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
	#ifndef SGFE_3DS
	SGFE_videoMode mode = SGFE_videoModeOptimal();
	#else
	/* cpu_renderer.h implements basic stereoscopic rendering for the 3DS. */
	SGFE_videoMode mode = SGFE_videoMode3D;
	#endif

	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagsNone);
	SGFE_bool res = SGFE_windowCreateContextBuffer(
		win, mode, SGFE_pixelFormatOptimal(), SGFE_TRUE
	);
	if (res == SGFE_FALSE) { return 1; }

	CPU_Surface s = surface_make(SGFE_windowGetContextBuffer(win), CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

	SGFE_area win_res = SGFE_bufferGetResolution(s.ctx);
	#ifdef SGFE_3DS
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
	if (SGFE_bufferIsStereoscopic(win->buffer)) {
		win_res.w /= 2;
	}
	SGFE_rect r = SGFE_RECT(100, 100, img_lonic_width, img_lonic_height);

	while (!SGFE_windowShouldClose(win)) {
		const SGFE_event* event;
		while (SGFE_windowCheckEvent(win, &event)) {
			if (event->type == SGFE_buttonPressed && event->button == BUTTON_START) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				break;
			}
		}

		SGFE_controller* p1 = SGFE_controllerGet(win, 0);

		if (SGFE_isPressed(p1, BUTTON_LEFT) && r.x > 0) {
			r.x -= 1;
		}
		else if (SGFE_isPressed(p1, BUTTON_RIGHT) && r.x + r.w < win_res.w) {
			r.x += 1;
		}

		if (SGFE_isPressed(p1, BUTTON_UP) && r.y > 0) {
			r.y -= 1;
		}
		else if (SGFE_isPressed(p1, BUTTON_DOWN) && r.y + r.h < win_res.h) {
			r.y += 1;
		}

		surface_rect(&s, SGFE_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, r, img_lonic_data);

		SGFE_windowSwapBuffers(win);
		surface_clear_dirty_rects(&s);
	}

	SGFE_windowClose(win);
}
