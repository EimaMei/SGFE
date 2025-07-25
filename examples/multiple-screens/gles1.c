
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>
#include "../gles/gles1_vshader.h"

static
GLuint load_shader(GLenum shader_type, GLenum binary_format, const u8* binary_source, ssize_t binary_size) {
	GLuint shader = glCreateShader(shader_type);
	glShaderBinary(1, &shader, binary_format, binary_source, binary_size);

	return shader;
}

typedef struct {
	float pos[2];
	float color[4];
} vertex;

static
void scene_setup(vertex* allocated_vertices) {
	/* OpenGL setup. */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint vertex_shader = load_shader(
		GL_VERTEX_SHADER, gles1_vshader_format, gles1_vshader, sizeof(gles1_vshader)
	);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);

	glLinkProgram(shader_program);
	{
		GLint success;
		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			char info_log[512];
			glGetProgramInfoLog(shader_program, 512, NULL, info_log);
			printf("Program Link Error: %s\n", info_log);
		}
	}
	glUseProgram(shader_program);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)allocated_vertices->pos);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)&allocated_vertices->color);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	#ifdef RGFW_3DS
	/* NOTE(EimaMei): The Nintendo 3DS internally uses a "portrait" resolution
	 * for its screens, meaning bottom left is (-1.0, 1.0) while top right is
	 * (1.0, -1.0). You can either handle this issue on your own by having a matrix
	 * that's always rotated by 90 degrees clockwise or let RGFW do that for you
	 * by inputting a mat4 uniform variable and calling the "fixScreen" function. */
	RGFW_platform_OpenGL_rotateScreen(shader_program, "RGFW_PROJECTION");
	#endif
}

int main(void) {
	RGFW_setGLHint(RGFW_glMajor, 1);
	RGFW_setGLHint(RGFW_glMinor, 0);
	RGFW_setGLHint(RGFW_glProfile, RGFW_glES);

	/* NOTE(EimaMei): On some embedded platforms video graphics must be allocated
	 * from a specific memory address, otherwise you'll just only see a blank screen.
	 * We copy over our vertices to a valid buffer for this. */
	vertex vertices[] = {
		{{-1,  1},  {1, 0, 0, 1}},
		{{ 1,  1},  {0, 1, 0, 1}},
		{{ 1, -1},  {0, 0, 1, 1}},
	};
	vertex* allocated_vertices = RGFW_ALLOC_SYS(sizeof(vertices));
	memcpy(allocated_vertices, vertices, sizeof(vertices));

#ifdef RGFW_3DS
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowOpenGL | RGFW_windowDualScreen);

	RGFW_window_makeCurrent(win, RGFW_screenTop);
	scene_setup(allocated_vertices);

	RGFW_window_makeCurrent(win, RGFW_screenBottom);
	scene_setup(allocated_vertices);
#else
	#error "This platform does not support multiple screens."
#endif

	while (RGFW_window_checkEvents(win, 0)) {
		if (RGFW_isPressed(RGFW_controllerGet(win, 0), BUTTON_START)) {
			RGFW_window_setShouldClose(win, RGFW_TRUE);
			continue;
		}

#ifdef RGFW_3DS
		RGFW_window_makeCurrent(win, RGFW_screenTop);
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RGFW_COUNTOF(vertices));
		}

		RGFW_window_makeCurrent(win, RGFW_screenBottom); {
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RGFW_COUNTOF(vertices));
		}
#endif

		RGFW_window_swapBuffers_OpenGL(win);
	}

	RGFW_FREE_SYS(allocated_vertices);
	RGFW_window_close(win);
}
