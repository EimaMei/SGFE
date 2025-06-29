#include <stdio.h>
#include <stdlib.h>

size_t counter = 0;

void* myAlloc(size_t size, unsigned int line, const char* file) {
    void* ptr = malloc(size);
    printf("%s:%i allocated %u bytes at %p\n",  file, line, (unsigned int)size, ptr);
    counter++;

    return ptr;
}

void myFree(void* ptr, unsigned int line, const char* file) {
    counter--;
    printf("%s:%i freed address %p\n", file, line, ptr);
    free(ptr);
}

#define RGFW_ALLOC(size) myAlloc(size, __LINE__, __FILE__)
#define RGFW_FREE(size) myFree(size, __LINE__, __FILE__)

#define RGFW_DEBUG
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>

static unsigned char icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};


#if RGFW_3DS
	#define FPS_BUTTON  RGFW_Select
	#define QUIT_BUTTON RGFW_Start
#endif

int main(void) {
	gfxInit(GSP_RGBA8_OES, GSP_RGB565_OES, false);
	consoleInit(GFX_BOTTOM, NULL);

	RGFW_window *win = RGFW_createWindow(RGFW_AREA(500, 500), RGFW_windowCenter | RGFW_windowAllowDND);
	//RGFW_window_makeCurrent(win);
	//RGFW_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);
	//RGFW_window_setMouseStandard(win, RGFW_mouseResizeNESW);
	//RGFW_mouse *mouse = RGFW_loadMouse(icon, RGFW_AREA(3, 3), 4);

	u32 frames = 0;
	u32 fps = 0;
	const double startTime = RGFW_getTime();

	ssize_t i, j;
	for (i = 0; i < RGFW_controllerGetCount(); i += 1) {
		printf("\n\n");
		RGFW_controllerType type = _RGFW->controllerType[i];
	//RGFW_keyState controllerButtons[RGFW_MAX_CONTROLLER_DEVICES][RGFW_MAX_BUTTON_COUNT]; /*!< if a key is currently pressed or not (per controller) */

		printf("Controller #%i: %s\n", i, RGFW_controllerName(type));
		for (j = 0; j < RGFW_MAX_AXIS_COUNT; j += 1) {
			printf("\tAxis #%02i: %s\n", j, RGFW_axisName(j));
		}

		printf("\n");

		/* TODO(EimaMei): Add a way to get the maximum amount of buttons for a controller type. */
		/* TODO(EimaMei): Add a way to get buttons from a specific controller type. */
		for (j = 0; j < RGFW_MAX_BUTTON_COUNT; j += 1) {
			printf("\tButton #%02i: %s\n", j, RGFW_buttonName(j));
		}

	}

	while (!RGFW_window_shouldClose(win)) {
		RGFW_event *event = NULL;
		while ((event = RGFW_window_checkEvent(win)) != NULL) {
			switch (event->type) {
				case RGFW_quit:
					RGFW_window_setShouldClose(win, RGFW_TRUE);
					break;
				case RGFW_windowResized:
					if (event->point.x != 0 && event->point.y != 0)
						printf("resize: %ix%i\n", event->point.x, event->point.y);
					break;
				case RGFW_buttonPressed:
					printf("pressed %s\n", RGFW_buttonName(event->button));
					break;
				case RGFW_buttonReleased:
					printf("released %s\n", RGFW_buttonName(event->button));
					break;
				case RGFW_controllerAxisMove: {
					printf(
						"%s: value (%i); deadzone (%i)\n",
						RGFW_axisName(event->whichAxis),
						event->axis.value, event->axis.deadzone
					);
				} break;

				case RGFW_pointerMove: {
					printf(
						"%s: %ix%i\n",
						RGFW_pointerName(event->input_index),
						event->point.x, event->point.y
					);
				} break;

				case RGFW_motionMove: {
					if (!RGFW_isPressed(0, RGFW_Y)) { break; }

					printf(
						"%s: %ix%i\n",
						RGFW_pointerName(event->input_index),
						event->point.x, event->point.y
					);
				} break;
			}
		}

		if (RGFW_isReleased(0, FPS_BUTTON))
			printf("fps: %d\n", fps);
		//else if (RGFW_isReleased(win, RGFW_w))
		//	RGFW_window_setMouseDefault(win);
		//else if (RGFW_isReleased(win, RGFW_e))
		//	RGFW_window_setMouse(win, mouse);
		//else if (RGFW_isReleased(win, RGFW_q))
		//	RGFW_window_showMouse(win, 0);
		//else if (RGFW_isReleased(win, RGFW_t))
		//	RGFW_window_showMouse(win, 1);
		//else if (RGFW_isReleased(win, RGFW_down))
		//		RGFW_writeClipboard("DOWN 刺猬", 12);
		//else if (RGFW_isReleased(win, RGFW_up))
		//	printf("pasted '%s'\n", RGFW_readClipboard(NULL));

		else if (RGFW_isPressed(0, FPS_BUTTON) && RGFW_isPressed(0, QUIT_BUTTON)) {
			RGFW_window_setShouldClose(win, RGFW_TRUE);
			continue;
		}

#if 0
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(-0.6f, -0.75f);
			glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.6f, -0.75f);
			glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.0f, 0.75f);
		glEnd();
#endif

		//RGFW_window_swapBuffers(win);

		fps = RGFW_checkFPS(startTime, frames, 60);
		frames++;
	}

	//RGFW_freeMouse(mouse);
	RGFW_window_close(win);

    if (counter > 0) printf("MEMORY LEAK!\n");
    return 0;
}

