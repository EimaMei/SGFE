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

#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>


static
void callback_error(RGFW_debugType type, RGFW_error error, RGFW_debugContext ctx, const char* msg) {
	if (type != RGFW_debugTypeError || error == RGFW_errorNone) {
		return;
	}

	printf("RGFW ERROR: %s\n", msg);
}

static
void callback_sleep(RGFW_window* win, RGFW_bool is_sleeping) {
	printf("Device %s\n", is_sleeping ? "sleeps" : "wakes up");
}

static
void callback_focus(RGFW_window* win, RGFW_bool is_focused) {
	printf("Device %s\n", is_focused ? "is focused" : "isn't focused");
}

static
void callback_quit(RGFW_window* win) {
	printf("window quit\n");
}

static
void callback_videoMode(RGFW_window* win, RGFW_videoMode video_mode) {
	printf("TODO\n");
}

static
void callback_controller(RGFW_window* win, RGFW_controller* controller, RGFW_bool connected) {
	printf("'%s' has been %s\n", RGFW_controllerGetName(controller), connected ? "connected" : "disconnected");
}

static
void callback_button(RGFW_window* win, RGFW_controller* controller, RGFW_button buttons, RGFW_bool pressed) {
	printf("key %s:", pressed ? "pressed" : "released");

	RGFW_buttonType button;
	RGFW_button copy = buttons;
	while (RGFW_iterateButtonMask(&copy, &button)) {
		printf(" %s", RGFW_controllerGetNameButton(controller, button));
	}
	printf("\n");

	RGFW_window_setShouldClose(win, RGFW_BOOL(buttons & BUTTON_START));
}

static
void callback_axis(RGFW_window* win, RGFW_controller* controller, RGFW_axisType which) {
	RGFW_axis* axis = &controller->axes[which];
	if (fabsf(axis->value) < axis->deadzone) {
		return;
	}

	printf(
		"%s: value (%f); deadzone (%f)\n",
		RGFW_controllerGetNameAxis(controller, which),
		axis->value, axis->deadzone
	);
}

static
void callback_pointer(RGFW_window* win, RGFW_controller* controller, RGFW_pointerType which) {
	ssize_t x = controller->pointers[which][0],
			y = controller->pointers[which][1];

	printf(
		"%s: %ix%i\n",
		RGFW_controllerGetNamePointer(controller, which),
		x, y
	);
}

static
void callback_motion(RGFW_window* win, RGFW_controller* controller, RGFW_motionType which) {
	float x = controller->motions[which][0],
		  y = controller->motions[which][1],
		  z = controller->motions[which][2];

	printf(
		"%s: %fx%fx%f\n",
		RGFW_controllerGetNameMotion(controller, which),
		x, y, z
	);
}


int main(void) {
	RGFW_window* win = RGFW_createWindowContextless(RGFW_windowFlagsNone);
	RGFW_windowInitConsole(win);

	RGFW_setDebugCallback(callback_error);
	RGFW_setDeviceSleepCallback(win, callback_sleep);
	RGFW_setWindowQuitCallback(win, callback_quit);
	RGFW_setWindowFocusCallback(win, callback_focus);
	RGFW_setControllerCallback(win, callback_controller);
	RGFW_setButtonCallback(win, callback_button);
	RGFW_setAxisCallback(win, callback_axis);
	RGFW_setPointerCallback(win, callback_pointer);
	RGFW_setMotionCallback(win, callback_motion);

	while (!RGFW_window_shouldClose(win)) {
		RGFW_window_pollEvents(win);
		RGFW_windowSwapBuffers(win);
	}
	RGFW_window_close(win);
}
