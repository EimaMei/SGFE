#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION

/* Specifying 'RGFW_BUFFER_NO_CONVERSION' disables Embedded functionality to convert
 * standard buffers into a format that the deivce can understand, meaning you can 
 * _only_ use native buffers instead of being able to use either. Calling functions 
 * like 'RGFW_window_initBufferFormat' will silently fail and not display, meaning 
 * everything must be done via 'RGFW_window_initBufferNative' or 'RGFW_window_initBufferScreen'
 * with the 'is_native' option enabled.
 *
 * What formats are expected entirely depends on the system. Some systems don't
 * support RGBA8, some might have a completely different screen resolution format
 * (eg. 3DS). For any platform-specific details consult the documentation provided
 * in the RGFW_embedded.h header file. */
#define RGFW_BUFFER_NO_CONVERSION

#include <RGFW_embedded.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include <resources/cpu_renderer.h>


int main(void) {
	#ifndef RGFW_3DS
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowNoInitAPI);
	#else
	/* On the 3DS basic 3D rendering is implemented in the CPU renderer. */
	RGFW_window* win = RGFW_createWindow(RGFW_videoMode3D, RGFW_windowNoInitAPI);
	#endif
	RGFW_bool res = RGFW_window_initBufferNative(win, RGFW_pixelFormatOptimal());
	if (res == RGFW_FALSE) { return 1; }

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

	CPU_Surface s = surface_make(win, CPU_colorMake(255, 255, 255, 255));
	surface_clear_buffers(&s);

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
