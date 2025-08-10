#define SGFE_IMPLEMENTATION
#define SGFE_DEBUG

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
#ifdef SGFE_3DS
	SGFE_window* win = SGFE_windowMake(SGFE_videoModeOptimal(), SGFE_windowBuffer | SGFE_windowDualScreen);

	CPU_Surface top = surface_make(
		SGFE_windowGetContextExBuffer(win, SGFE_screenTop),
		CPU_colorMake(255, 255, 255, 255)
	);
	CPU_Surface bottom = surface_make(
		SGFE_windowGetContextExBuffer(win, SGFE_screenBottom),
		CPU_colorMake(43, 184, 0, 255)
	);

	surface_clear_buffers(&bottom);
	surface_clear_buffers(&top);
#else
    #error "This platform does not support multiple screens."
#endif

	CPU_Rect r = CPU_rectMake(100, 100, img_lonic_width, img_lonic_height);

	isize width, height;
	SGFE_bufferGetResolution(top.ctx, &width, &height);

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

		#ifdef SGFE_3DS
		surface_rect(&top, CPU_rectMake(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&top, r, img_lonic_data);

		surface_rect(&bottom, CPU_rectMake(15, 15, 64, 64), CPU_colorMake(15, 129, 216, 255));
		#endif

		SGFE_windowSwapBuffers(win);

		#ifdef SGFE_3DS
		surface_clear_dirty_rects(&top);
		surface_clear_dirty_rects(&bottom);
		#endif
	}

	SGFE_windowClose(win);
}
