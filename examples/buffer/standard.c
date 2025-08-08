#define SGFE_IMPLEMENTATION
#define SGFE_DEBUG

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
	SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowBuffer);
	if (win == NULL) { return 1; }

	CPU_Surface s = surface_make(SGFE_windowGetContextBuffer(win), CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

	SGFE_area win_res = SGFE_bufferGetResolution(s.ctx);
	SGFE_rect r = SGFE_RECT(100, 100, img_lonic_width, img_lonic_height);

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

		surface_rect(&s, SGFE_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, r, img_lonic_data);

		SGFE_windowSwapBuffers(win);
		surface_clear_dirty_rects(&s);
	}

	SGFE_windowClose(win);
}
