#define SGFE_IMPLEMENTATION
#define SGFE_DEBUG

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
#ifdef SGFE_3DS
	SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowBuffer | SGFE_windowDualScreen);

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

	SGFE_rect r = SGFE_RECT(100, 100, img_lonic_width, img_lonic_height);

	SGFE_area win_res = SGFE_bufferGetResolution(win->buffer);
	/* NOTE(EimaMei): For stereoscopic images we have to divide the width in half
	 * since we don't want Lonic (from the left eye image) moving to the right one
	 * since that'll create a distorted view. */
	if (SGFE_bufferIsStereoscopic(win->buffer)) {
		win_res.w /= 2;
	}

	while (!SGFE_window_shouldClose(win)) {
		const SGFE_event* event;
		while (SGFE_window_checkEvent(win, &event)) {
			if (event->type == SGFE_buttonPressed && event->button == BUTTON_START) {
				SGFE_window_setShouldClose(win, SGFE_TRUE);
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

		#ifdef SGFE_3DS
		SGFE_windowMakeCurrentBuffer(win, top.ctx); {
			surface_clear_dirty_rects(&top);
			surface_rect(&top, SGFE_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
			surface_bitmap(&top, r, img_lonic_data);
		}

		SGFE_windowMakeCurrentBuffer(win, bottom.ctx); {
			surface_clear_dirty_rects(&bottom);
			surface_rect(&bottom, SGFE_RECT(15, 15, 64, 64), CPU_colorMake(15, 129, 216, 255));
		}
		#endif

		SGFE_windowSwapBuffers(win);
	}

	SGFE_windowClose(win);
}
