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
	printf("'%s' has been %s\n", RGFW_controllerName(controller->type), connected ? "connected" : "disconnected");
}

static
void callback_button(RGFW_window* win, RGFW_controller* controller, RGFW_button button, RGFW_bool pressed) {
	printf(
		"key %s: %i (%s)\n",
		pressed ? "pressed" : "released",
		button, RGFW_buttonName(button)
	);

	switch (button) {
		case BUTTON_START: RGFW_window_setShouldClose(win, RGFW_TRUE); break;
		case BUTTON_BACK: {
			RGFW_videoMode new_mode = win->mode + 1;
			if (new_mode >= RGFW_videoModeCount) { new_mode = 0; } /* Wrap back around. */
		} break;
	}
}

static
void callback_axis(RGFW_window* win, RGFW_controller* controller, RGFW_axisType axis_type) {
	RGFW_axis* axis = &controller->axes[axis_type];
	printf(
		"%s: value (%f); deadzone (%f)\n",
		RGFW_axisName(axis_type),
		axis->value, axis->deadzone
	);
}

static
void callback_pointer(RGFW_window* win, RGFW_controller* controller, RGFW_pointerType pointer_type) {
	RGFW_point point = controller->pointers[pointer_type];
	printf(
		"%s: %ix%i\n",
		RGFW_pointerName(pointer_type),
		point.x, point.y
	);
}

static
void callback_motion(RGFW_window* win, RGFW_controller* controller, RGFW_motionType motion_type) {
	RGFW_point3D vector = controller->motions[motion_type];
	printf(
		"%s: %fx%fx%f\n",
		RGFW_motionName(motion_type),
		vector.x, vector.y, vector.z
	);
}


int main(void) {
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowConsoleInit);

	RGFW_setDebugCallback(callback_error);
	RGFW_setDeviceSleepCallback(win, callback_sleep);
	RGFW_setWindowQuitCallback(win, callback_quit);
	RGFW_setWindowFocusCallback(win, callback_focus);
	RGFW_setControllerCallback(win, callback_controller);
	RGFW_setButtonCallback(win, callback_button);
	RGFW_setAxisCallback(win, callback_axis);
	RGFW_setPointerCallback(win, callback_pointer);
	RGFW_setMotionCallback(win, callback_motion);

	while (RGFW_window_checkEvents(win, RGFW_eventWaitNext)) {}
	RGFW_window_close(win);
}