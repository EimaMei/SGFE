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
	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagNone);
	/* On consoles you usually don't have easy (or any) access to a real terminal.
	 * To get around this SGFE enables you to turn the entire screen into a terminal
	 * output screen.
	 *
	 * Any function that writes to stdout/stderr (e.g., printf) will have its output
	 * directly blitted into the screen.
	 * 
	 * You can also create a window with a terminal output by specifying
	 * 'SGFE_windowFlagTerminal' when creating a window. */
	SGFE_windowInitTerminalOutput(win);

	/* On consoles it's best to only poll the events and not loop through all of
	 * them (since "event" ordering does not matter on consoles). However, since
	 * this style of event checking is popular on desktop, you can still enable
	 * this functionality by setting the "queue events" flag to true.
	 *
	 * For porting RGFW apps it's also important to look through SGFE's events and
	 * see which events differs and which ones are similiar to RGFW.
	 *
	 * For this example we also disable certain events right off the bat so that
	 * the screen wouldn't get flooded with events that we may not want to see
	 * right now. */
	SGFE_windowSetQueueEvents(win, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventPointer, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMotion, SGFE_FALSE);

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		controller_printInfo(SGFE_windowGetController(win, i));
	}

	while (!SGFE_windowShouldClose(win)) {
		const SGFE_event* event = NULL;
		while (SGFE_windowCheckEvent(win, &event)) {
			switch (event->type) {
				case SGFE_eventControllerConnected: {
					controller_printInfo(event->controller.controller);
				} break;

				case SGFE_eventButtonDown: {
					printf(
						"pressed %s (repeat: %i)\n",
						SGFE_controllerGetNameButton(event->button.controller, event->button.button),
						event->button.repeat
					);
				} break;

				case SGFE_eventButtonUp: {
					printf(
						"released %s\n",
						SGFE_controllerGetNameButton(event->button.controller, event->button.button)
					);
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
					if (!SGFE_windowGetEventEnabled(win, SGFE_eventMotion)) { break; }
					SGFE_event_motion motion = event->motion;

					printf(
						"%s: %fx%fx%f\n",
						SGFE_controllerGetNameMotion(motion.controller, motion.which),
						motion.x, motion.y, motion.z
					);
				} break;
			}
		}

		const SGFE_controller* controller = SGFE_windowGetController(win, 0);
		if (controller->connected && SGFE_isHeld(controller, BUTTON_BACK)) {
			if (SGFE_isDown(controller, BUTTON_PRIMARY)) {
				SGFE_bool state = !SGFE_windowGetEventEnabled(win, SGFE_eventPointer);
				SGFE_windowSetEventEnabled(win, SGFE_eventPointer, state);
				printf(
					"Pointer %s!\n",
					state ? "enabled" : "disabled"
				);
			}
			else if (SGFE_isDown(controller, BUTTON_SECONDARY)) {
				SGFE_bool state = !SGFE_windowGetEventEnabled(win, SGFE_eventMotion);
				SGFE_windowSetEventEnabled(win, SGFE_eventMotion, state);
				printf(
					"Motion %s!\n",
					state ? "enabled" : "disabled"
				);
			}
			else if (SGFE_isDown(controller, BUTTON_START)) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
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
	isize first, last;

	printf("Controller #%zi: %s\n", controller->port, SGFE_controllerGetName(controller));

	SGFE_controllerGetRangeAxis(controller, &first, &last);
	for (SGFE_axisType j = first; j <= last; j += 1) {
		printf("\tAxis #%02zi: %s\n", j, SGFE_controllerGetNameAxis(controller, j));
	}

	SGFE_controllerGetRangeButton(controller, &first, &last);
	for (SGFE_buttonType j = first; j <= last; j += 1) {
		printf("\tButton #%02zi: %s\n", j, SGFE_controllerGetNameButton(controller, j));
	}

	/* NOTE(EimaMei): More often than not motion capabilities are disabled
	 * by default as they chew up a lot of resources and you usually don't
	 * need them. However, for a basic example it doesn't matter.
	 *
	 * You can see which motion sensors are enabled by checking
	 * '.enabled_motions[]' and seeing if they equal to true. */
	SGFE_controllerGetRangeMotion(controller, &first, &last);
	for (SGFE_motionType j = first; j <= last; j += 1) {
		SGFE_controllerEnableMotion(controller, j, SGFE_TRUE);
		printf("\tMotion device #%02zi: %s\n", j, SGFE_controllerGetNameMotion(controller, j));
	}

	/* NOTE(EimaMei): Usually most pointer capabilities are enabled by default
	 * unlike motion sensors, however there aren't any guarantees that all
	 * pointer functionality will be enabled by default.
	 *
	 * You can see which pointer functionalities are enabled by checking
	 * '.enabled_pointers[]' and seeing if they equal to true. */
	SGFE_controllerGetRangePointer(controller, &first, &last);
	for (SGFE_pointerType j = first; j <= last; j += 1) {
		SGFE_controllerEnablePointer(controller, j, SGFE_TRUE);
		printf("\tPointer device #%02zi: %s\n", j, SGFE_controllerGetNamePointer(controller, j));
	}
}
