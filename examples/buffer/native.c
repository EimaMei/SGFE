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
	SGFE_videoMode mode = SGFE_videoGetOptimalMode();
	#else
	/* cpu_renderer.h implements basic stereoscopic rendering for the 3DS. */
	SGFE_videoMode mode = SGFE_videoMode3D;
	#endif

	/* If you create a window buffer and __'SGFE_BUFFER_NO_CONVERSION' IS defined__,
	 * SGFE by default will create framebuffers that _are_ native and use the
	 * 'SGFE_bufferFormatOptimal()' format.
	 *
	 * If different parameters are desired, you have to use a combination of
	 * 'SGFE_windowMakeContextless()' (or 'SGFE_windowMake()' without 'SGFE_windowFlagBuffer')
	 *  and 'SGFE_windowCreateContextBuffer()'. */
	SGFE_window* win = SGFE_windowMake(mode, SGFE_windowFlagBuffer);
	if (win == NULL) { return 1; }

	CPU_Surface s = surface_make(SGFE_windowGetContextBuffer(win), CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

	CPU_Rect r = CPU_rectMake(100, 100, img_lonic_width, img_lonic_height);

	/* NOTE(EimaMei): On some systems the native framebuffer resolutions could follow
	 * a format that's different to what is standard on desktop.
	 *
	 * For example, on the 3DS framebuffers are treated as "portraits" that are
	 * rotated by 90 degrees clockwise, meaning SGFE_bufferGetResolution() would
	 * return 200x240 under native mode.
	 *
	 * SGFE_videoGetResolution() is guaranteed to always return the resolution
	 * in the standardized format (WIDTHxHEIGHT). */
	isize width, height;
	SGFE_videoGetResolution(SGFE_bufferGetVideoMode(s.ctx), &width, &height);

	/* NOTE(EimaMei): For stereoscopic images we have to divide the width in half
	 * since we don't want Lonic (from the left eye image) moving into the right one,
	 * otherwise a distorted view will form. */
	if (SGFE_bufferIsStereoscopic(s.ctx)) {
		width /= 2;
	}

	while (!SGFE_windowShouldClose(win)) {
		SGFE_windowPollEvents(win);

		SGFE_controller* p1 = SGFE_windowGetController(win, 0);
		if (SGFE_isDown(p1, BUTTON_START)) {
			SGFE_windowSetShouldClose(win, SGFE_TRUE);
			continue;
		}

		if (SGFE_isHeld(p1, BUTTON_LEFT) && r.x > 0) {
			r.x -= 1;
		}
		else if (SGFE_isHeld(p1, BUTTON_RIGHT) && r.x + r.w < width) {
			r.x += 1;
		}

		if (SGFE_isHeld(p1, BUTTON_UP) && r.y > 0) {
			r.y -= 1;
		}
		else if (SGFE_isHeld(p1, BUTTON_DOWN) && r.y + r.h < height) {
			r.y += 1;
		}

		surface_rect(&s, CPU_rectMake(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, r, img_lonic_data);

		SGFE_windowSwapBuffers(win);
		surface_clear_dirty_rects(&s);
	}

	SGFE_windowClose(win);
}
