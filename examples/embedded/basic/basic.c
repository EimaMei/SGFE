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
	#define CMD_BUTTON    RGFW_Select
	#define QUIT_BUTTON   RGFW_Start
	#define MOTION_BUTTON RGFW_Y
	#define FPS_BUTTON RGFW_X
#endif



int main(void) {
	gfxInit(GSP_RGBA8_OES, GSP_RGB565_OES, false);
	consoleInit(GFX_BOTTOM, NULL);

	RGFW_window *win = RGFW_createWindow(RGFW_AREA(0, 0), 0);


	RGFW_bool motion_enabled = false;
	u32 frames = 0;
	u32 fps = 0;
	const double startTime = RGFW_getTime();

	ssize_t i, j;
	for (i = 0; i < RGFW_controllerGetCount(); i += 1) {
		printf("\n\n");
		RGFW_controller* controller = RGFW_controllerGet(i);

		/* NOTE(EimaMei): More often than not motion capabilities are disabled
		 * by default as they chew up a lot of resources and you usually don't
		 * need them. However, for a basic example it doesn't matter.
		 *
		 * You can see which motion sensors are enabled by checking
		 * '.enabled_motions[]' and seeing if they equal to true. */
		for (j = 0; j < RGFW_motionTypeCount; j += 1) {
			RGFW_controllerMotionEnable(controller, j, true);
		}
		/* NOTE(EimaMei): Usually most pointer capabilities are enabled by default
		 * unlike motion sensors, however there aren't any guarantees that all
		 * pointer functionality will be enabled by default.
		 *
		 * You can see which pointer functionalities are enabled by checking
		 * '.enabled_pointers[]' and seeing if they equal to true. */
		for (j = 0; j < RGFW_pointerTypeCount; j += 1) {
			RGFW_controllerPointerEnable(controller, j, true);
		}

		/* TODO(EimaMei): dealing with supported buttons, axes. */
		printf("Controller #%i: %s\n", i, RGFW_controllerName(controller->type));
		for (j = 0; j < RGFW_axisTypeCount; j += 1) {
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
				case RGFW_buttonPressed:
					printf("pressed %s\n", RGFW_buttonName(event->button));
					break;
				case RGFW_buttonReleased:
					printf("released %s\n", RGFW_buttonName(event->button));
					break;
				case RGFW_controllerAxisMove: {
					RGFW_axis* axis = &event->controller->axes[event->axis];
					printf(
						"%s: value (%f); deadzone (%f)\n",
						RGFW_axisName(event->axis),
						axis->value, axis->deadzone
					);
				} break;

				case RGFW_pointerMove: {
					RGFW_point point = event->controller->pointers[event->pointer];
					printf(
						"%s: %ix%i\n",
						RGFW_pointerName(event->pointer),
						point.x, point.y
					);
				} break;

				case RGFW_motionMove: {
					/* NOTE(EimaMei): We don't want the screen to be flooded
					 * with a bunch of event about motion sensors, so a check
					 * is added to print them only when needed. */
					if (!motion_enabled) { break; }
					RGFW_point3D vector = event->controller->motions[event->motion];

					printf(
						"%s: %fx%fx%f\n",
						RGFW_motionName(event->motion),
						vector.x, vector.y, vector.z
					);
				} break;
			}
		}

		RGFW_controller* controller = RGFW_controllerGet(0);
		if (controller->connected) {
			if (RGFW_isPressed(controller, CMD_BUTTON)) {
				if (RGFW_isHeld(controller, MOTION_BUTTON)) {
					motion_enabled ^= true;
					printf("Motion %s!\n", motion_enabled ? "enabled" : "disabled");

					while (RGFW_isPressed(controller, CMD_BUTTON)) {
						RGFW_window_checkEvents(win, RGFW_eventWaitNext);
					}
				}
				else if (RGFW_isHeld(controller, FPS_BUTTON))
					printf("fps: %d\n", fps);
				else if (RGFW_isHeld(controller, QUIT_BUTTON)) {
					RGFW_window_setShouldClose(win, RGFW_TRUE);
					continue;
				}
			}
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

		//fps = RGFW_checkFPS(startTime, frames, 60);
		frames++;
	}

	//RGFW_freeMouse(mouse);
	RGFW_window_close(win);

    if (counter > 0) printf("MEMORY LEAK!\n");
    return 0;
}

