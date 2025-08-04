#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG

#include <RGFW_embedded.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowBuffer);
	if (win == NULL) { return 1; }

	CPU_Surface s = surface_make(RGFW_window_getContext_buffer(win), CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

	RGFW_area win_res = RGFW_context_bufferGetResolution(s.ctx);
	RGFW_rect r = RGFW_RECT(100, 100, img_lonic_width, img_lonic_height);

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

		surface_rect(&s, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, r, img_lonic_data);

		RGFW_window_swapBuffers_buffer(win);
		surface_clear_dirty_rects(&s);
	}

	RGFW_window_close(win);
}
