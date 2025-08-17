#define SGFE_IMPLEMENTATION

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


#ifndef SCALE_OBJECTS
	#ifdef SGFE_WII
	#define SCALE_OBJECTS 2
	#else
	#define SCALESCALE_OBJECTS 1
	#endif
#endif


int main(void) {
	/* If you create a window buffer and __'SGFE_BUFFER_NO_CONVERSION' IS NOT defined__,
	 * SGFE by default will create framebuffers that _are not_ native and use the
	 * 'SGFE_bufferFormatRGBA8' format.
	 *
	 * If different parameters are desired, you have to use a combination of
	 * 'SGFE_windowMakeContextless()' (or 'SGFE_windowMake()' without 'SGFE_windowFlagBuffer')
	 *  and 'SGFE_windowCreateContextBuffer()'. */
	SGFE_window* win = SGFE_windowMake(SGFE_videoGetOptimalMode(), SGFE_windowFlagBuffer);
	if (win == NULL) { return 1; }

	CPU_Surface s = surface_make(SGFE_windowGetContextBuffer(win), CPU_colorMake(255, 255, 255, 0));
	surface_clear_buffers(&s);

	isize width, height;
	SGFE_bufferGetResolution(s.ctx, &width, &height);

	CPU_Rect r = CPU_rectMake(100, 100, img_lonic_width * SCALE_OBJECTS, img_lonic_height * SCALE_OBJECTS);

	u8* lonic_but_BIGGER = SGFE_ALLOC(r.w * r.h * 4);
	for (isize y = 0; y < r.h; y += 1) {
		for (isize x = 0; x < r.w; x += 1) {
			uint8_t* p = &img_lonic_data[4 * (y * img_lonic_width + x)];

			for (isize i = 0; i < 4; i++) {
				lonic_but_BIGGER[4 * ((SCALE_OBJECTS * y    ) * r.w + (SCALE_OBJECTS * x    )) + i] = p[i];
				lonic_but_BIGGER[4 * ((SCALE_OBJECTS * y    ) * r.w + (SCALE_OBJECTS * x + 1)) + i] = p[i];
				lonic_but_BIGGER[4 * ((SCALE_OBJECTS * y + 1) * r.w + (SCALE_OBJECTS * x    )) + i] = p[i];
				lonic_but_BIGGER[4 * ((SCALE_OBJECTS * y + 1) * r.w + (SCALE_OBJECTS * x + 1)) + i] = p[i];
			}
		}
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

		surface_rect(
			&s,
			CPU_rectMake(15, 15, 64 * SCALE_OBJECTS, 64 * SCALE_OBJECTS), 
			CPU_colorMake(0, 255, 0, 255)
		);
		surface_bitmap(&s, r, lonic_but_BIGGER);

		SGFE_windowSwapBuffers(win);
		surface_clear_dirty_rects(&s);
	}

	SGFE_FREE(lonic_but_BIGGER);
	SGFE_windowClose(win);
}
