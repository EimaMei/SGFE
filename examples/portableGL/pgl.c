#define PORTABLEGL_IMPLEMENTATION
#include "portablegl.h"

#define RGFW_IMPLEMENTATION
#include <RGFW_embedded.h>
#include <resources/controls.h>


typedef struct My_Uniforms
{
	vec4 v_color;
} My_Uniforms;

static
void identity_vs(float* vs_output, vec4* vertex_attribs, Shader_Builtins* builtins, void* uniforms) {
	builtins->gl_Position = vertex_attribs[0];
}

static
void uniform_color_fs(float* fs_input, Shader_Builtins* builtins, void* uniforms) {
	builtins->gl_FragColor = ((My_Uniforms*)uniforms)->v_color;
}

int main() {
	RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), RGFW_windowBuffer);

	glContext context;
	init_glContext(&context, (u32**)&win->buffer, win->bufferSize.w, win->bufferSize.h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

	float points[] = { -0.5, -0.5, 0,
						0.5, -0.5, 0,
						0,    0.5, 0 };


	My_Uniforms the_uniforms;

	GLuint myshader = pglCreateProgram(identity_vs, uniform_color_fs, 0, NULL, GL_FALSE);
	glUseProgram(myshader);

	pglSetUniform(&the_uniforms);

	the_uniforms.v_color = (vec4){1.0f, 0.0f, 0.0f, 1.0f};

	GLuint triangle;
	glGenBuffers(1, &triangle);
	glBindBuffer(GL_ARRAY_BUFFER, triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*9, points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glClearColor(1, 1, 1, 1);

	while (!RGFW_window_shouldClose(win)) {
		const RGFW_event* event;
		while (RGFW_window_checkEvent(win, &event)) {
			if (event->type == RGFW_buttonPressed && event->button == BUTTON_START) {
				RGFW_window_setShouldClose(win, RGFW_TRUE);
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		RGFW_window_swapBuffers(win);
		context.back_buffer.buf = win->buffer;
		context.back_buffer.lastrow = &win->buffer[(win->bufferSize.h - 1) * win->bufferSize.w * (i32)sizeof(u32)];
	}

	free_glContext(&context);

	RGFW_window_close(win);
}
