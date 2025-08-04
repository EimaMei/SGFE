
#define RGFW_OPENGL
#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>
#include "../gles/gles1_vshader.h"

static
GLuint load_shader(GLenum shader_type, const u8* binary_source, ssize_t binary_size) {
	#ifdef RGFW_3DS
	#ifndef GL_SHADER_BINARY_PICA
	#define GL_SHADER_BINARY_PICA 0x6000
	#endif

	GLenum binary_format = GL_SHADER_BINARY_PICA;
	#endif

	GLuint shader = glCreateShader(shader_type);
	glShaderBinary(1, &shader, binary_format, binary_source, binary_size);

	return shader;
}

typedef struct {
	float pos[2];
	float color[4];
} vertex;

static const vertex vertices[] = {
	{{-0.5f, -0.5f},  {1, 0, 0, 1}},
	{{ 0.5f, -0.5f},  {0, 1, 0, 1}},
	{{ 0.0f,  0.5f},  {0, 0, 1, 1}},
};

static
void scene_setup(u32* vbo) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint vertex_shader = load_shader(
		GL_VERTEX_SHADER, gles1_vshader, sizeof(gles1_vshader)
	);

	u32 shader_program = glCreateProgram();
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
	#ifdef RGFW_3DS
	/* NOTE(EimaMei): The Nintendo 3DS internally uses a "portrait" resolution
	 * for its screens, meaning bottom left is (-1.0, 1.0) while top right is
	 * (1.0, -1.0). You can either handle this issue on your own by having a matrix
	 * that's always rotated by 90 degrees clockwise or let RGFW do that for you
	 * by inputting a mat4 uniform variable and calling the "fixScreen" function. */
	RGFW_platform_OpenGL_rotateScreen(shader_program, "RGFW_PROJECTION");
	#endif
	glDeleteProgram(shader_program);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}

int main(void) {
	RGFW_setHint_OpenGL(RGFW_glMajor, 1);
	RGFW_setHint_OpenGL(RGFW_glMinor, 0);
	RGFW_setHint_OpenGL(RGFW_glProfile, RGFW_glProfile_ES);

#ifdef RGFW_3DS
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowOpenGL | RGFW_windowDualScreen);
	RGFW_context_OpenGL *top = RGFW_window_getContextEx_OpenGL(win, RGFW_screenTop),
						*bottom = RGFW_window_getContextEx_OpenGL(win, RGFW_screenBottom);

	u32 vbo[2];
	RGFW_window_makeCurrent_OpenGL(win, top);
	scene_setup(&vbo[0]);

	RGFW_window_makeCurrent_OpenGL(win, bottom);
	scene_setup(&vbo[1]);
#else
	#error "This platform does not support multiple screens."
#endif

	while (RGFW_window_checkEvents(win, 0)) {
		if (RGFW_isPressed(RGFW_controllerGet(win, 0), BUTTON_START)) {
			RGFW_window_setShouldClose(win, RGFW_TRUE);
			continue;
		}

#ifdef RGFW_3DS
		RGFW_window_makeCurrent_OpenGL(win, top); {
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RGFW_COUNTOF(vertices));
		}

		RGFW_window_makeCurrent_OpenGL(win, bottom); {
			glClear(GL_COLOR_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, RGFW_COUNTOF(vertices));
		}
#endif

		RGFW_window_swapBuffers_OpenGL(win);
	}

	RGFW_window_makeCurrent_OpenGL(win, top);
	glDeleteBuffers(1, &vbo[0]);

	RGFW_window_makeCurrent_OpenGL(win, bottom);
	glDeleteBuffers(1, &vbo[1]);

	RGFW_window_close(win);
}
