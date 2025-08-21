#define SGFE_IMPLEMENTATION

#include <SGFE.h>
#include <resources/controls.h>
#include <resources/lonic_bin.h>
#include "../buffer/standard/renderer_standard.h"


int main(void) {
#ifdef SGFE_3DS
	SGFE_window* win = SGFE_windowMake(SGFE_videoGetOptimalMode(), SGFE_windowFlagBuffer | SGFE_windowFlagDualScreen);

	CPU_Surface primary = surface_make(
		SGFE_windowGetContextExBuffer(win, SGFE_screenTop),
		CPU_colorMake(255, 255, 255, 255)
	);
	CPU_Surface bottom = surface_make(
		SGFE_windowGetContextExBuffer(win, SGFE_screenBottom),
		CPU_colorMake(43, 184, 0, 255)
	);

	surface_clear_buffers(&bottom);
	surface_clear_buffers(&primary);

#else
	#error "This platform does not support multiple scre
#endif

	isize width, height;
	SGFE_bufferGetResolution(primary.ctx, &width, &height);

	isize x = 100, y = 100;
	CPU_Image img = CPU_imageMake(&primary, img_lonic_data, img_lonic_width, img_lonic_height);

	surface_clear_buffers(&primary);

	while (!SGFE_windowShouldClose(win)) {
		const SGFE_windowState* state = SGFE_windowPollEvents(win);

		SGFE_controller* p1 = SGFE_controllerGet(state->controllers, 0);
		if (p1) {
			if (SGFE_isDown(p1, BUTTON_START)) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				continue;
			}

			if (SGFE_isHeld(p1, BUTTON_LEFT) && (isize)((float)x * primary.scale_x) > 0) {
				x -= 1;
			}
			else if (SGFE_isHeld(p1, BUTTON_RIGHT) && (isize)((float)x * primary.scale_x) + img.w < width) {
				x += 1;
			}

			if (SGFE_isHeld(p1, BUTTON_UP) && (isize)((float)y * primary.scale_y) > 0) {
				y -= 1;
			}
			else if (SGFE_isHeld(p1, BUTTON_DOWN) && (isize)((float)y * primary.scale_y) + img.h < height) {
				y += 1;
			}
		}

		#ifdef SGFE_3DS
		surface_rect(&primary, CPU_rectMake(15, 15, 64, 64), CPU_colorMake(0, 255, 0, 255));
		surface_bitmap(&primary, x, y, img);

		surface_rect(&bottom, CPU_rectMake(15, 15, 64, 64), CPU_colorMake(15, 129, 216, 255));
		#endif

		SGFE_windowSwapBuffers(win);

		#ifdef SGFE_3DS
		surface_clear_dirty_rects(&primary);
		surface_clear_dirty_rects(&bottom);
		#endif
	}

	SGFE_windowClose(win);
}
