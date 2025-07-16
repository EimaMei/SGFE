#define RGFW_IMPLEMENTATION
#define RGFW_DEBUG

// #define RGFW_BUFFER_NATIVE
#define RGFW_BUFFER

#include <RGFW_embedded.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


#if RGFW_3DS
	#define BUTTON_QUIT   RGFW_Start
	#define BUTTON_LEFT   RGFW_DpadLeft
	#define BUTTON_RIGHT  RGFW_DpadRight
	#define BUTTON_UP     RGFW_DpadUp
	#define BUTTON_DOWN   RGFW_DpadDown
#endif


int main(void) {
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), 0);
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

	// todo: swap function and flush function.
	CPU_Surface s = surface_make(win, CPU_colorMake(255, 255, 255, 255));
	surface_clearBuffers(&s);

	RGFW_bool running = RGFW_TRUE;
	while (running) {
		while (RGFW_window_checkEvent(win)) {
			if (win->event->type == RGFW_quit || RGFW_isPressed(RGFW_controllerGet(0), BUTTON_QUIT)) {
				running = 0;
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

		surface_rect(&s, RGFW_RECT(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&s, r, img_lonic_data);

		surface_flush(&s);
	}

	RGFW_window_close(win);
}
