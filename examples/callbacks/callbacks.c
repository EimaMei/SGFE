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

#define SGFE_ALLOC(size) myAlloc(size, __LINE__, __FILE__)
#define SGFE_FREE(size) myFree(size, __LINE__, __FILE__)

#define SGFE_DEBUG
#define SGFE_IMPLEMENTATION
#include <SGFE.h>
#include <resources/controls.h>


static
void callback_error(SGFE_debugType type, SGFE_error error, SGFE_debugContext ctx, const char* msg) {
	if (type != SGFE_debugTypeError || error == SGFE_errorNone) {
		return;
	}

	printf("SGFE ERROR: %s\n", msg);
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
void callback_button(SGFE_window* win, SGFE_controller* controller, SGFE_button buttons, SGFE_bool pressed) {
	printf("key %s:", pressed ? "pressed" : "released");

	SGFE_buttonType button;
	SGFE_button copy = buttons;
	while (SGFE_iterateButtonMask(&copy, &button)) {
		printf(" %s", SGFE_controllerGetNameButton(controller, button));
	}
	printf("\n");

	SGFE_windowSetShouldClose(win, SGFE_BOOL(buttons & BUTTON_START));
}

static
void callback_axis(SGFE_window* win, SGFE_controller* controller, SGFE_axisType which) {
	SGFE_axis* axis = &controller->axes[which];
	if (fabsf(axis->value) < axis->deadzone) {
		return;
	}

	printf(
		"%s: value (%f); deadzone (%f)\n",
		SGFE_controllerGetNameAxis(controller, which),
		axis->value, axis->deadzone
	);
}

static
void callback_pointer(SGFE_window* win, SGFE_controller* controller, SGFE_pointerType which) {
	isize x = controller->pointers[which][0],
			y = controller->pointers[which][1];

	printf(
		"%s: %ix%i\n",
		SGFE_controllerGetNamePointer(controller, which),
		x, y
	);
}

static
void callback_motion(SGFE_window* win, SGFE_controller* controller, SGFE_motionType which) {
	float x = controller->motions[which][0],
		  y = controller->motions[which][1],
		  z = controller->motions[which][2];

	printf(
		"%s: %fx%fx%f\n",
		SGFE_controllerGetNameMotion(controller, which),
		x, y, z
	);
}


int main(void) {
	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagsNone);
	SGFE_windowInitTerminalOutput(win);

	SGFE_setDebugCallback(callback_error);
	SGFE_setDeviceSleepCallback(win, callback_sleep);
	SGFE_setWindowQuitCallback(win, callback_quit);
	SGFE_setWindowFocusCallback(win, callback_focus);
	SGFE_setControllerCallback(win, callback_controller);
	SGFE_setButtonCallback(win, callback_button);
	SGFE_setAxisCallback(win, callback_axis);
	SGFE_setPointerCallback(win, callback_pointer);
	SGFE_setMotionCallback(win, callback_motion);

	while (!SGFE_windowShouldClose(win)) {
		SGFE_windowPollEvents(win);
		SGFE_windowSwapBuffers(win);
	}
	SGFE_windowClose(win);
}
