#define SGFE_OPENGL
#define SGFE_IMPLEMENTATION
#include <SGFE.h>
#include <resources/controls.h>
#include "gles1_vshader.h"

static
GLuint load_shader(GLenum shader_type, const u8* binary_source, isize binary_size) {
	#ifdef SGFE_3DS
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

int main(void) {
	SGFE_setHint_OpenGL(SGFE_glMajor, 1);
	SGFE_setHint_OpenGL(SGFE_glMinor, 0);
	SGFE_setHint_OpenGL(SGFE_glProfile, SGFE_glProfile_ES);
	SGFE_window* win = SGFE_windowMake(SGFE_videoModeOptimal(), SGFE_windowOpenGL);

	/* === OpenGL init === */
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
	#ifdef SGFE_3DS
	/* NOTE(EimaMei): The Nintendo 3DS internally uses a "portrait" resolution
	 * for its screens, meaning bottom left is (-1.0, 1.0) while top right is
	 * (1.0, -1.0). You can either handle this issue on your own by having a matrix
	 * that's always rotated by 90 degrees clockwise or let SGFE do that for you
	 * by inputting a mat4 uniform variable and calling the "fixScreen" function. */
	SGFE_platformRotateScreenOpenGL(shader_program, "SGFE_PROJECTION");
	#endif
	glDeleteProgram(shader_program);

	u32 vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, color));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	while (SGFE_windowCheckEvents(win, 0)) {
		if (SGFE_isHeld(SGFE_controllerGet(win, 0), BUTTON_START)) {
			SGFE_windowSetShouldClose(win, SGFE_TRUE);
			continue;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, SGFE_COUNTOF(vertices));

		SGFE_window_swapBuffers_OpenGL(win);
	}

	glDeleteBuffers(1, &vbo);
	SGFE_windowClose(win);
}
