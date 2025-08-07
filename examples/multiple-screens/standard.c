#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG

#include <RGFW_embedded.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
#ifdef RGFW_3DS
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowBuffer | RGFW_windowDualScreen);

	CPU_Surface top = surface_make(
		RGFW_windowGetContextExBuffer(win, RGFW_screenTop),
		CPU_colorMake(255, 255, 255, 255)
	);
	CPU_Surface bottom = surface_make(
		RGFW_windowGetContextExBuffer(win, RGFW_screenBottom),
		CPU_colorMake(43, 184, 0, 255)
	);

	surface_clear_buffers(&bottom);
	surface_clear_buffers(&top);
#else
    #error "This platform does not support multiple screens."
#endif

	RGFW_rect r = RGFW_RECT(100, 100, img_lonic_width, img_lonic_height);

	RGFW_area win_res = RGFW_bufferGetResolution(win->buffer);
	/* NOTE(EimaMei): For stereoscopic images we have to divide the width in half
	 * since we don't want Lonic (from the left eye image) moving to the right one
	 * since that'll create a distorted view. */
	if (RGFW_bufferIsStereoscopic(win->buffer)) {
		win_res.w /= 2;
	}

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

		#ifdef RGFW_3DS
		RGFW_windowMakeCurrentBuffer(win, top.ctx); {
			surface_clear_dirty_rects(&top);
			surface_rect(&top, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
			surface_bitmap(&top, r, img_lonic_data);
		}

		RGFW_windowMakeCurrentBuffer(win, bottom.ctx); {
			surface_clear_dirty_rects(&bottom);
			surface_rect(&bottom, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(15, 129, 216, 255));
		}
		#endif

		RGFW_windowSwapBuffers(win);
	}

	RGFW_window_close(win);
}
