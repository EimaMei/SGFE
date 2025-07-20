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

#define RGFW_BUFFER
#define RGFW_BUFFER_NATIVE
#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>


static
void errorfunc(RGFW_debugType type, RGFW_errorCode err, RGFW_debugContext ctx, const char* msg) {
	if (type != RGFW_typeError || err == RGFW_noError) return; /* disregard non-errors */
	/* only care about errors for this window
		If there were two windows and the error uses the root window it will also be ignored,
			this may ignore important errors
	*/
	printf("RGFW ERROR: %s\n", msg);
}

static
void windowresizefunc(RGFW_window* win, RGFW_rect r) {
	printf("window resized %i %i\n", r.w, r.h);
}

static
void windowquitfunc(RGFW_window* win) {
	printf("window quit\n");
}

static
void buttonfunc(RGFW_window* win, RGFW_controller* controller, RGFW_button button, RGFW_bool pressed) {
	printf(
		"key %s: %i (%s)\n",
		pressed ? "pressed" : "released",
		button, RGFW_buttonName(button)
	);

	if (button == BUTTON_START) {
		RGFW_window_setShouldClose(win, RGFW_TRUE);
	}
}

static
void axisfunc(RGFW_window* win, RGFW_controller* controller, RGFW_axisType axis_type) {
	RGFW_axis* axis = &controller->axes[axis_type];
	printf(
		"%s: value (%f); deadzone (%f)\n",
		RGFW_axisName(axis_type),
		axis->value, axis->deadzone
	);
}

static
void pointerfunc(RGFW_window* win, RGFW_controller* controller, RGFW_pointerType pointer_type) {
	RGFW_point point = controller->pointers[pointer_type];
	printf(
		"%s: %ix%i\n",
		RGFW_pointerName(pointer_type),
		point.x, point.y
	);
}

static
void motionfunc(RGFW_window* win, RGFW_controller* controller, RGFW_motionType motion_type) {
	RGFW_point3D vector = controller->motions[motion_type];
	printf(
		"%s: %fx%fx%f\n",
		RGFW_motionName(motion_type),
		vector.x, vector.y, vector.z
	);
}

static
void controllerfunc(RGFW_window* win, RGFW_controller* controller, RGFW_bool connected) {
	printf("'%s' has been %s\n", RGFW_controllerName(controller->type), connected ? "connected" : "disconnected");
}

int main(void) {
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowConsoleInit);

	RGFW_setDebugCallback(errorfunc);
	//RGFW_setWindowResizedCallback(windowresizefunc);
	RGFW_setWindowQuitCallback(windowquitfunc);
	RGFW_setControllerAxisCallback(axisfunc);
	RGFW_setPointerMoveCallback(pointerfunc);
	RGFW_setMotionMoveCallback(motionfunc);
	RGFW_setButtonCallback(buttonfunc);
	RGFW_setControllerCallback(controllerfunc);

	while (RGFW_window_checkEvents(win, RGFW_eventWaitNext)) {}
	RGFW_window_close(win);
}
