#include <stdio.h>
#include <stdlib.h>
#include <math.h>

size_t counter = 0;

#define SGFE_WII_NO_WAIT_FOR_CONNECTION
#define SGFE_MAX_KEYBOARDS 1
#define SGFE_IMPLEMENTATION
#include <SGFE-wii.h>
#define SGFE_WII 1
#include <resources/controls.h>

SGFE_bool shutup = 0;

static
void callback_error(SGFE_debugContext ctx) {
	printf(
		"%s:%zi:%s: %s %s: %s (%s)\n",
		ctx.filename, ctx.line, ctx.function,
		SGFE_debugSourceName(ctx.source), SGFE_debugTypeName(ctx.type),
		SGFE_debugCodeGetDesc(ctx.source, ctx.type, ctx.code),
		SGFE_debugCodeGetName(ctx.source, ctx.type, ctx.code)
	);
}

static
void callback_sleep(SGFE_window* win, SGFE_bool is_sleeping) {
	printf("Device %s\n", is_sleeping ? "sleeps" : "wakes up");
}

static
void callback_focus(SGFE_window* win, SGFE_bool is_focused) {
	printf("Device %s\n", is_focused ? "is focused" : "isn't focused");
}

static
void callback_quit(SGFE_window* win) {
	printf("window quit\n");
}

static
void callback_controller(SGFE_window* win, SGFE_controller* controller, SGFE_bool connected) {
	printf("'%s' has been %s\n", SGFE_controllerGetName(controller->type), connected ? "connected" : "disconnected");
}

static
void callback_battery(SGFE_window* win, SGFE_controller* controller, SGFE_powerState state, isize battery_procent) {
	printf(
		"Batery status for '%s' has changed (battery: %zi%%, state: %zi)\n",
		SGFE_controllerGetName(controller->type),
		battery_procent, state
	);
}

static
void callback_button(SGFE_window* win, SGFE_controller* controller, SGFE_buttonType button,
		SGFE_bool is_repeated, SGFE_bool is_pressed) {
	printf(
		"button %s: %s (repeated: %i)\n", 
		is_pressed ? "pressed" : "released", SGFE_controllerGetNameButton(controller->type, button),
		is_repeated
	);

	if (SGFE_isHeld(controller, BUTTON_START)) {
		SGFE_windowSetShouldClose(win, SGFE_TRUE);
	}
}

static
void callback_axis(SGFE_window* win, SGFE_controller* controller, const SGFE_axis* axis) {
	if (fabsf(axis->value) < axis->deadzone) {
		return;
	}

	printf(
		"%s: value (%f); deadzone (%f)\n",
		SGFE_controllerGetNameAxis(controller->type, axis->type),
		axis->value, axis->deadzone
	);
}

static
void callback_pointer(SGFE_window* win, SGFE_controller* controller, const SGFE_pointer* pointer) {
	printf(
		"%s: %zix%zi\n",
		SGFE_controllerGetNamePointer(controller->type, pointer->type),
		pointer->x, pointer->y
	);
}

static
void callback_motion(SGFE_window* win, SGFE_controller* controller, const SGFE_motion* motion) {
	printf(
		"%s: %fx%fx%f\n",
		SGFE_controllerGetNameMotion(controller->type, motion->type),
		motion->x, motion->y, motion->z
	);
}

static
void callback_keyboard(SGFE_window* win, SGFE_keyboard* keyboard, SGFE_bool connected) {
	printf("Keyboard has been %s\n", connected ? "connected" : "disconnected");
}

static
void callback_key(SGFE_window* win, SGFE_keyboard* keyboard, SGFE_key key,
		SGFE_bool is_repeated, SGFE_bool is_pressed) {
	printf(
		"button %s: %i (repeated: %i)\n", 
		is_pressed ? "pressed" : "released", key,
		is_repeated
	);

	if (SGFE_isKeyDown(keyboard, SGFE_keyEsc)) {
		SGFE_windowSetShouldClose(win, SGFE_TRUE);
	}
}

int main(void) {
	SGFE_setDebugCallback(callback_error, NULL);

	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagTerminal);

	SGFE_windowSetDeviceSleepCallback(win, callback_sleep);
	SGFE_windowSetQuitCallback(win, callback_quit);
	SGFE_windowSetFocusCallback(win, callback_focus);
	SGFE_windowSetControllerCallback(win, callback_controller);
	SGFE_windowSetControllerBatteryCallback(win, callback_battery);
	SGFE_windowSetButtonCallback(win, callback_button);
	SGFE_windowSetAxisCallback(win, callback_axis);
	SGFE_windowSetPointerCallback(win, callback_pointer);
	SGFE_windowSetMotionCallback(win, callback_motion);

	SGFE_windowSetKeyboardCallback(win, callback_keyboard);
	SGFE_windowSetKeyCallback(win, callback_key);

	while (!SGFE_windowShouldClose(win)) {
		SGFE_windowPollEvents(win);
		SGFE_windowSwapBuffers(win);
	}
	SGFE_windowClose(win);
}
