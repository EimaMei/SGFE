#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG
#define RGFW_BUFFER

#include <RGFW_embedded.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
#ifdef RGFW_3DS
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowDualScreen);

	CPU_Surface top = surface_make(win, CPU_colorMake(255, 255, 255, 255)),
				bottom = surface_make(win, CPU_colorMake(43, 184, 0, 255));

	RGFW_window_makeCurrent(RGFW_SCREEN_BOTTOM);
	surface_clearBuffers(&bottom);

	RGFW_window_makeCurrent(RGFW_SCREEN_TOP);
	surface_clearBuffers(&top);
#else
    #error "This platform does not support multiple screens."
#endif

	RGFW_rect r = RGFW_RECT(100, 100, img_lonic_width, img_lonic_height);

	/* NOTE(EimaMei): 'RGFW_windowGetSize(win)' may actually differ to 'win->bufferSize'
	 * in certain situations where the viewport for the window is actually smaller
	 * than the rendered buffer.
	 *
	 * Platforms where it differs:
	 * - 3DS (when using RGFW_videoMode3D) - internally the buffer is 800x240,
	 * however the viewport is still set to 400x240 as the second half of the
	 * resolution is used for the 3D effect. */
	RGFW_area win_res = RGFW_windowGetSize(win);

	while (!RGFW_window_shouldClose(win)) {
		while (RGFW_TRUE) {
			const RGFW_event* event = RGFW_window_checkEvent(win);
			if (event == NULL) { break; }

			if (event->type == RGFW_buttonPressed && event->button == BUTTON_START) {
				RGFW_window_setShouldClose(win, RGFW_TRUE);
				break;
			}
		}

		RGFW_controller* p1 = RGFW_controllerGet(0);

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
		RGFW_window_makeCurrent(RGFW_SCREEN_TOP); {
			surface_clearDirtyRects(&top);
			surface_rect(&top, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
			surface_bitmap(&top, r, img_lonic_data);
		}

		RGFW_window_makeCurrent(RGFW_SCREEN_BOTTOM); {
			surface_clearDirtyRects(&bottom);
			surface_rect(&bottom, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(15, 129, 216, 255));
		}
		#endif

		RGFW_window_swapBuffers_buffer(win);
	}

	RGFW_window_close(win);
}
