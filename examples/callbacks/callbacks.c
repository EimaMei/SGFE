#include <stdio.h>
#include <stdlib.h>
#include <math.h>

size_t counter = 0;

#define SGFE_DEBUG
#define SGFE_IMPLEMENTATION
#include <SGFE.h>
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
void callback_videoMode(SGFE_window* win, SGFE_videoMode video_mode) {
	printf("TODO\n");
}

static
void callback_controller(SGFE_window* win, SGFE_controller* controller, SGFE_bool connected) {
	printf("'%s' has been %s\n", SGFE_controllerGetName(controller), connected ? "connected" : "disconnected");
}

static
void callback_button(SGFE_window* win, SGFE_controller* controller, SGFE_buttonType button, SGFE_bool pressed) {
	printf("key %s: %s\n", pressed ? "pressed" : "released", SGFE_controllerGetNameButton(controller, button));

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
		SGFE_controllerGetNameAxis(controller, axis->type),
		axis->value, axis->deadzone
	);
}

static
void callback_pointer(SGFE_window* win, SGFE_controller* controller, const SGFE_pointer* pointer) {
	printf(
		"%s: %zix%zi\n",
		SGFE_controllerGetNamePointer(controller, pointer->type),
		pointer->x, pointer->y
	);
}

static
void callback_motion(SGFE_window* win, SGFE_controller* controller, const SGFE_motion* motion) {
	printf(
		"%s: %fx%fx%f\n",
		SGFE_controllerGetNameMotion(controller, motion->type),
		motion->x, motion->y, motion->z
	);
}


int main(void) {
	SGFE_setDebugCallback(callback_error, NULL);

	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagTerminal);

	SGFE_windowSetDeviceSleepCallback(win, callback_sleep);
	SGFE_windowSetQuitCallback(win, callback_quit);
	SGFE_windowSetFocusCallback(win, callback_focus);
	SGFE_windowSetControllerCallback(win, callback_controller);
	SGFE_windowSetButtonCallback(win, callback_button);
	SGFE_windowSetAxisCallback(win, callback_axis);
	SGFE_windowSetPointerCallback(win, callback_pointer);
	SGFE_windowSetMotionCallback(win, callback_motion);

	while (!SGFE_windowShouldClose(win)) {
		SGFE_windowPollEvents(win);
		SGFE_windowSwapBuffers(win);
	}
	SGFE_windowClose(win);
}
