#include <stdio.h>
#include <stdlib.h>

size_t counter = 0;

void* myAlloc(size_t size, int line, const char* file);
void myFree(void* ptr, int line, const char* file);

void* myAlloc(size_t size, int line, const char* file) {
	void* ptr = malloc(size);
	printf("%s:%i allocated %u bytes at %p\n",  file, line, (unsigned int)size, ptr);
	counter++;

	return ptr;
}

void myFree(void* ptr, int line, const char* file) {
	counter--;
	printf("%s:%i freed address %p\n", file, line, ptr);
	free(ptr);
}

#define RGFW_ALLOC(size) myAlloc(size, __LINE__, __FILE__)
#define RGFW_FREE(size) myFree(size, __LINE__, __FILE__)

#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>


int main(void) {
	RGFW_window* win = RGFW_createWindowContextless(RGFW_windowFlagsNone);
	RGFW_window_consoleInit(win);
	RGFW_bool motion_enabled = false;

	ssize_t i, j;
	for (i = 0; i < RGFW_controllerGetCount(win); i += 1) {
		printf("\n\n");
		RGFW_controller* controller = RGFW_controllerGet(win, i);

		/* NOTE(EimaMei): More often than not motion capabilities are disabled
		 * by default as they chew up a lot of resources and you usually don't
		 * need them. However, for a basic example it doesn't matter.
		 *
		 * You can see which motion sensors are enabled by checking
		 * '.enabled_motions[]' and seeing if they equal to true. */
		for (j = 0; j < RGFW_motionTypeCount; j += 1) {
			RGFW_controllerMotionEnable(controller, j, true);
		}
		/* NOTE(EimaMei): Usually most pointer capabilities are enabled by default
		 * unlike motion sensors, however there aren't any guarantees that all
		 * pointer functionality will be enabled by default.
		 *
		 * You can see which pointer functionalities are enabled by checking
		 * '.enabled_pointers[]' and seeing if they equal to true. */
		for (j = 0; j < RGFW_pointerTypeCount; j += 1) {
			RGFW_controllerPointerEnable(controller, j, true);
		}

		printf("Controller #%i: %s\n", i, RGFW_controllerName(controller->type));
		for (j = 0; j < RGFW_axisTypeCount; j += 1) {
			printf("\tAxis #%02i: %s\n", j, RGFW_axisName(j));
		}

		printf("\n");

		for (j = controller->button_start; j < controller->button_end; j += 1) {
			printf("\tButton #%02i: %s\n", j, RGFW_buttonName(j));
		}
	}

	while (!RGFW_window_shouldClose(win)) {
		const RGFW_event* event = NULL;
		while (RGFW_window_checkEvent(win, &event)) {
			switch (event->type) {
				case RGFW_quit:
					RGFW_window_setShouldClose(win, RGFW_TRUE);
					break;
				case RGFW_buttonPressed:
					printf("pressed %s\n", RGFW_buttonName(event->button));
					break;
				case RGFW_buttonReleased:
					printf("released %s\n", RGFW_buttonName(event->button));
					break;

				case RGFW_axisMove: {
					RGFW_axis* axis = &event->controller->axes[event->axis];
					printf(
						"%s: value (%f); deadzone (%f)\n",
						RGFW_axisName(event->axis),
						axis->value, axis->deadzone
					);
				} break;

				case RGFW_pointerMove: {
					RGFW_point point = event->controller->pointers[event->pointer];
					printf(
						"%s: %ix%i\n",
						RGFW_pointerName(event->pointer),
						point.x, point.y
					);
				} break;

				case RGFW_motionMove: {
					/* NOTE(EimaMei): We don't want the screen to be flooded
					 * with a bunch of events about motion sensors, so a check
					 * is added to print them only when needed. */
					if (!motion_enabled) { break; }
					RGFW_point3D vector = event->controller->motions[event->motion];

					printf(
						"%s: %fx%fx%f\n",
						RGFW_motionName(event->motion),
						vector.x, vector.y, vector.z
					);
				} break;
			}
		}

		RGFW_controller* controller = RGFW_controllerGet(win, 0);
		if (controller->connected) {
			if (RGFW_isPressed(controller, BUTTON_BACK)) {
				if (RGFW_isHeld(controller, BUTTON_WEST)) {
					motion_enabled ^= true;
					printf(
						"Motion %s! Now waiting for the BUTTON_BACK button to be released...\n",
						motion_enabled ? "enabled" : "disabled"
					);

					while (RGFW_window_checkEvents(win, RGFW_eventWaitNext)) {
						if (RGFW_isReleased(controller, BUTTON_BACK)) {
							break;
						}
					}
				}
				else if (RGFW_isHeld(controller, BUTTON_START)) {
					RGFW_window_setShouldClose(win, RGFW_TRUE);
					continue;
				}
			}
		}

		RGFW_window_swapBuffers_buffer(win);
	}
	RGFW_window_close(win);

	if (counter > 0) printf("MEMORY LEAK!\n");
	return 0;
}

