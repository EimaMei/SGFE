#define SGFE_DEBUG
#define SGFE_IMPLEMENTATION
#include <SGFE.h>
#include <resources/controls.h>
#include <math.h>


/* Prints information about a newly connected controller. */
void controller_printInfo(SGFE_controller* controller);


int main(void) {
	/* In SGFE, the "window" is the structure in which the SGFE library stores everything
	 * inside. It is essentially the global state of the application.*/
	SGFE_window* win = SGFE_createWindowContextless(SGFE_windowFlagsNone);
	/* On consoles you usually don't have easy (or any) access to a real terminal.
	 * To get around this SGFE enables you to turn the entire window into a terminal
	 * output screen.
	 *
	 * Any function that writes to stdout/stderr (e.g., printf) will have its output
	 * directly blitted into the screen. */
	SGFE_windowInitTerminalOutput(win);
	/* On consoles it's best to only poll the events and not loop through all of
	 * them (since "event" ordering does not matter on consoles). However, since
	 * this style of event checking is popular on desktop, you can still enable
	 * this functionality by setting the "queue events" flag to true.
	 * 
	 * For porting RGFW apps it's also important to look through SGFE's events and
	 * see which events differs and which ones are similiar to RGFW. */
	SGFE_windowSetQueueEvents(win, SGFE_TRUE);
	/* TODO(EimaMei): remove this. */
	SGFE_bool motion_enabled = SGFE_FALSE;

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		controller_printInfo(SGFE_window_controllerGet(win, i));
	}

	while (!SGFE_window_shouldClose(win)) {
		const SGFE_event* event = NULL;
		while (SGFE_window_checkEvent(win, &event)) {
			switch (event->type) {
				case SGFE_eventQuit: {
					SGFE_window_setShouldClose(win, SGFE_TRUE);
				} break;

				case SGFE_eventControllerConnected: {
					controller_printInfo(event->controller.controller);
				} break;

				case SGFE_eventButtonDown: {
					SGFE_buttonType button;
					SGFE_button total_mask = event->button.buttons;

					while (SGFE_iterateButtonMask(&total_mask, &button)) {
						printf(
							"pressed %s (repeat: %i)\n",
							SGFE_controllerGetNameButton(event->button.controller, button),
							(event->button.buttons_down & SGFE_BIT(button)) == 0
						);
					}
				} break;

				case SGFE_eventButtonUp: {
					SGFE_buttonType button;
					SGFE_button total_mask = event->button.buttons;

					while (SGFE_iterateButtonMask(&total_mask, &button)) {
						printf(
							"released %s\n",
							SGFE_controllerGetNameButton(event->button.controller, button)
						);
					}
				} break;

				case SGFE_eventAxis: {
					SGFE_event_axis axis = event->axis;
					if (fabsf(axis.value) < axis.deadzone) {
						continue;
					}

					printf(
						"%s: value (%f); deadzone (%f)\n",
						SGFE_controllerGetNameAxis(axis.controller, axis.which),
						axis.value, axis.deadzone
					);
				} break;

				case SGFE_eventPointer: {
					SGFE_event_pointer pointer = event->pointer;
					printf(
						"%s: %ix%i\n",
						SGFE_controllerGetNamePointer(pointer.controller, pointer.which),
						pointer.x, pointer.y
					);
				} break;

				case SGFE_eventMotion: {
					/* NOTE(EimaMei): We don't want the screen to be flooded
					 * with a bunch of events about motion sensors, so a check
					 * is added to print them only when needed. */
					if (!motion_enabled) { break; }
					SGFE_event_motion motion = event->motion;

					printf(
						"%s: %fx%fx%f\n",
						SGFE_controllerGetNameMotion(motion.controller, motion.which),
						motion.x, motion.y, motion.z
					);
				} break;
			}
		}

		const SGFE_controller* controller = SGFE_window_controllerGet(win, 0);
		if (controller->connected && SGFE_isHeld(controller, BUTTON_BACK)) {
			if (SGFE_isDown(controller, BUTTON_PRIMARY)) {
				motion_enabled ^= SGFE_TRUE;
				printf(
					"Motion %s!\n",
					motion_enabled ? "enabled" : "disabled"
				);
			}
			else if (SGFE_isDown(controller, BUTTON_START)) {
				SGFE_window_setShouldClose(win, SGFE_TRUE);
				continue;
			}
		}

		SGFE_windowSwapBuffers(win);
	}
	SGFE_windowClose(win);
}

void controller_printInfo(SGFE_controller* controller) {
	if (!controller->connected) { return ; }
	printf("\n\n");
	isize start, end;

	printf("Controller #%zi: %s\n", controller->port, SGFE_controllerGetName(controller));

	SGFE_controller_getRangeAxis(controller, &start, &end);
	for (SGFE_axisType j = start; j < end; j += 1) {
		printf("\tAxis #%02zi: %s\n", j, SGFE_controllerGetNameAxis(controller, j));
	}

	if (start != -1) { printf("\n"); }

	for (SGFE_buttonType j = 0; j < SGFE_controller_getButtonCount(controller); j += 1) {
		printf("\tButton #%02zi: %s\n", j, SGFE_controllerGetNameButton(controller, j));
	}

	/* NOTE(EimaMei): More often than not motion capabilities are disabled
	 * by default as they chew up a lot of resources and you usually don't
	 * need them. However, for a basic example it doesn't matter.
	 *
	 * You can see which motion sensors are enabled by checking
	 * '.enabled_motions[]' and seeing if they equal to true. */
	SGFE_controller_getRangeMotion(controller, &start, &end);
	for (SGFE_motionType j = start; j < end; j += 1) {
		SGFE_controller_enableMotion(controller, j, SGFE_TRUE);
		printf("\tMotion device #%02zi: %s\n", j, SGFE_controllerGetNameMotion(controller, j));
	}

	/* NOTE(EimaMei): Usually most pointer capabilities are enabled by default
	 * unlike motion sensors, however there aren't any guarantees that all
	 * pointer functionality will be enabled by default.
	 *
	 * You can see which pointer functionalities are enabled by checking
	 * '.enabled_pointers[]' and seeing if they equal to true. */
	SGFE_controller_getRangePointer(controller, &start, &end);
	for (SGFE_pointerType j = start; j < end; j += 1) {
		SGFE_controller_enablePointer(controller, j, SGFE_TRUE);
		printf("\tPointer device #%02zi: %s\n", j, SGFE_controllerGetNameMotion(controller, j));
	}
}