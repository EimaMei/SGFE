#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

#define RGFW_ASSERT(condition) damn_ex(condition, #condition, __FILE__, __LINE__)

void damn_ex(int condition, const char* msg, const char* file, int line) {
	if (!condition) {
		printf("%s:%i: %s\n", file, line, msg);
		exit(1);
	}
}
#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>


static
void add_new_controller(RGFW_controller* controller) {
	if (!controller->connected) { return ; }
	printf("\n\n");
	ssize_t start, end;

	printf("Controller #%zi: %s\n", controller->port, RGFW_controllerGetName(controller));

	RGFW_controller_getRangeAxis(controller, &start, &end);
	for (RGFW_axisType j = start; j < end; j += 1) {
		printf("\tAxis #%02zi: %s\n", j, RGFW_controllerGetNameAxis(controller, j));
	}

	if (start != -1) { printf("\n"); }

	for (RGFW_buttonType j = 0; j < RGFW_controller_getButtonCount(controller); j += 1) {
		printf("\tButton #%02zi: %s\n", j, RGFW_controllerGetNameButton(controller, j));
	}

	/* NOTE(EimaMei): More often than not motion capabilities are disabled
	 * by default as they chew up a lot of resources and you usually don't
	 * need them. However, for a basic example it doesn't matter.
	 *
	 * You can see which motion sensors are enabled by checking
	 * '.enabled_motions[]' and seeing if they equal to true. */
	RGFW_controller_getRangeMotion(controller, &start, &end);
	for (RGFW_motionType j = start; j < end; j += 1) {
		RGFW_controller_enableMotion(controller, j, RGFW_TRUE);
		printf("\tMotion device #%02zi: %s\n", j, RGFW_controllerGetNameMotion(controller, j));
	}

	/* NOTE(EimaMei): Usually most pointer capabilities are enabled by default
	 * unlike motion sensors, however there aren't any guarantees that all
	 * pointer functionality will be enabled by default.
	 *
	 * You can see which pointer functionalities are enabled by checking
	 * '.enabled_pointers[]' and seeing if they equal to true. */
	RGFW_controller_getRangePointer(controller, &start, &end);
	for (RGFW_pointerType j = start; j < end; j += 1) {
		RGFW_controller_enablePointer(controller, j, RGFW_TRUE);
		printf("\tPointer device #%02zi: %s\n", j, RGFW_controllerGetNameMotion(controller, j));
	}
}


int main(void) {
	RGFW_window* win = RGFW_createWindowContextless(RGFW_windowFlagsNone);
	RGFW_windowInitConsole(win);
	RGFW_window_setQueueEvents(win, RGFW_TRUE);
	RGFW_bool motion_enabled = RGFW_FALSE;

	for (ssize_t i = 0; i < RGFW_MAX_CONTROLLERS; i += 1) {
		add_new_controller(RGFW_window_controllerGet(win, i));
	}

	while (!RGFW_window_shouldClose(win)) {
		const RGFW_event* event = NULL;
		while (RGFW_window_checkEvent(win, &event)) {
			switch (event->type) {
				case RGFW_eventQuit: {
					RGFW_window_setShouldClose(win, RGFW_TRUE);
				} break;

				case RGFW_eventControllerConnected: {
					add_new_controller(event->controller.controller);
				} break;

				case RGFW_eventButtonDown: {
					RGFW_buttonType button;
					RGFW_button total_mask = event->button.buttons;

					while (RGFW_iterateButtonMask(&total_mask, &button)) {
						printf(
							"pressed %s (repeat: %i)\n",
							RGFW_controllerGetNameButton(event->button.controller, button),
							(event->button.buttons_down & RGFW_BIT(button)) == 0
						);
					}
				} break;

				case RGFW_eventButtonUp: {
					RGFW_buttonType button;
					RGFW_button total_mask = event->button.buttons;

					while (RGFW_iterateButtonMask(&total_mask, &button)) {
						printf(
							"released %s\n",
							RGFW_controllerGetNameButton(event->button.controller, button)
						);
					}
				} break;

				case RGFW_eventAxis: {
					RGFW_event_axis axis = event->axis;
					if (fabsf(axis.value) < axis.deadzone) {
						continue;
					}

					printf(
						"%s: value (%f); deadzone (%f)\n",
						RGFW_controllerGetNameAxis(axis.controller, axis.which),
						axis.value, axis.deadzone
					);
				} break;

				case RGFW_eventPointer: {
					RGFW_event_pointer pointer = event->pointer;
					printf(
						"%s: %ix%i\n",
						RGFW_controllerGetNamePointer(pointer.controller, pointer.which),
						pointer.x, pointer.y
					);
				} break;

				case RGFW_eventMotion: {
					/* NOTE(EimaMei): We don't want the screen to be flooded
					 * with a bunch of events about motion sensors, so a check
					 * is added to print them only when needed. */
					if (!motion_enabled) { break; }
					RGFW_event_motion motion = event->motion;

					printf(
						"%s: %fx%fx%f\n",
						RGFW_controllerGetNameMotion(motion.controller, motion.which),
						motion.x, motion.y, motion.z
					);
				} break;
			}
		}

		const RGFW_controller* controller = RGFW_window_controllerGet(win, 0);
		if (controller->connected && RGFW_isHeld(controller, BUTTON_BACK)) {
			if (RGFW_isDown(controller, BUTTON_PRIMARY)) {
				motion_enabled ^= RGFW_TRUE;
				printf(
					"Motion %s!\n",
					motion_enabled ? "enabled" : "disabled"
				);
			}
			else if (RGFW_isDown(controller, BUTTON_START)) {
				RGFW_window_setShouldClose(win, RGFW_TRUE);
				continue;
			}
		}

		RGFW_windowSwapBuffers(win);
	}
	RGFW_window_close(win);

	if (counter > 0) printf("MEMORY LEAK!\n");
	return 0;
}

