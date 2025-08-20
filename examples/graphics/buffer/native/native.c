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
#include "renderer_native.h"


#ifndef SCALE_OBJECTS
	#ifdef SGFE_WII
	#define SCALE_OBJECTS 2
	#else
	#define SCALE_OBJECTS 1
	#endif
#endif


int main(void) {
	#ifndef SGFE_3DS
	SGFE_videoMode mode = SGFE_videoGetOptimalMode();
	#else
	/* This example supports basic stereoscopic 3D for the Nintendo 3DS. */
	SGFE_videoMode mode = SGFE_videoMode3D;
	#endif

	/* If you create a window buffer and __'SGFE_BUFFER_NO_CONVERSION' IS NOT defined__,
	 * SGFE by default will create framebuffers that _are not_ native and use the
	 * 'SGFE_bufferFormatRGBA8' format.
	 *
	 * If different parameters are desired, you have to use a combination of
	 * 'SGFE_windowMakeContextless()' (or 'SGFE_windowMake()' without 'SGFE_windowFlagBuffer')
	 *  and 'SGFE_windowCreateContextBuffer()'. */
	SGFE_window* win = SGFE_windowMake(mode, SGFE_windowFlagBuffer);
	if (win == NULL) { return 1; }

	CPU_Surface s = surface_make(SGFE_windowGetContextBuffer(win), CPU_colorMake(255, 255, 255, 0));
	surface_set_scale(&s, SCALE_OBJECTS);

	#ifdef SGFE_WII
	/* NOTE(EimaMei): The Wii outputs its native video in a 4:3 aspect ratio (usually
	 * 640x480 on NTSC or 640x576 on PAL) even when 16:9 mode is selected in the
	 * settings. This means that games have to manually scale down the graphics
	 * if widescreen is turned on, which is what we do here.
	 *
	 * This function can be turned on other systems that have "wide screens" but
	 * aren't in the 16:9 as pect ratio. Results may wary on how it'll look. */
	surface_set_widescreen(&s, SGFE_platformIsWidescreen());
	#endif

	isize width, height;
	SGFE_bufferGetResolution(s.ctx, &width, &height);

	isize x = 100, y = 100;
	CPU_Image img = CPU_imageMake(&s, img_lonic_data, img_lonic_width, img_lonic_height);

	surface_clear_buffers(&s);

	while (!SGFE_windowShouldClose(win)) {
		const SGFE_windowState* state = SGFE_windowPollEvents(win);

		SGFE_controller* p1 = SGFE_controllerGet(state->controllers, 0);
		if (p1) {
			if (SGFE_isDown(p1, BUTTON_START)) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				continue;
			}

			if (SGFE_isHeld(p1, BUTTON_LEFT) && (isize)((float)x * s.scale_x) > 0) {
				x -= 1;
			}
			else if (SGFE_isHeld(p1, BUTTON_RIGHT) && (isize)((float)x * s.scale_x) + img.w < width) {
				x += 1;
			}

			if (SGFE_isHeld(p1, BUTTON_UP) && (isize)((float)y * s.scale_y) > 0) {
				y -= 1;
			}
			else if (SGFE_isHeld(p1, BUTTON_DOWN) && (isize)((float)y * s.scale_y) + img.h < height) {
				y += 1;
			}
		}

		surface_rect(&s, CPU_rectMake(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, x, y, img);

		SGFE_windowSwapBuffers(win);
		surface_clear_dirty_rects(&s);
	}

	SGFE_FREE(img.data);
	SGFE_windowClose(win);
}
