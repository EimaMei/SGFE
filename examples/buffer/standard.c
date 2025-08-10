#define SGFE_IMPLEMENTATION
#define SGFE_DEBUG

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
	/* If you create a window buffer and __'SGFE_BUFFER_NO_CONVERSION' IS NOT defined__,
	 * SGFE by default will create framebuffers that _are not_ native and use the
	 * 'SGFE_pixelFormatRGBA8' format.
	 * 
	 * If different parameters are desired, you have to use a combination of
	 * 'SGFE_windowMakeContextless()' (or 'SGFE_windowMake()' without 'SGFE_windowBuffer')
	 *  and 'SGFE_windowCreateContextBuffer()'. */
	SGFE_window* win = SGFE_windowMake(SGFE_videoModeOptimal(), SGFE_windowBuffer);
	if (win == NULL) { return 1; }

	CPU_Surface s = surface_make(SGFE_windowGetContextBuffer(win), CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

	CPU_Rect r = CPU_rectMake(100, 100, img_lonic_width, img_lonic_height);

	isize width, height;
	SGFE_bufferGetResolution(s.ctx, &width, &height);

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
