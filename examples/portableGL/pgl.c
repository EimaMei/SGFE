#define PORTABLEGL_IMPLEMENTATION
#include "portablegl.h"

#define SGFE_INT_DEFINED
#define SGFE_IMPLEMENTATION
#include <SGFE.h>
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

static 
void portableGL_ctx_init(SGFE_window* win, glContext* gl_out) {
	u8* buffer = SGFE_bufferGetFramebuffer(win->buffer);
	SGFE_area res = SGFE_bufferGetResolution(win->buffer);
	init_glContext(gl_out, (u32**)&buffer, res.w, res.h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
}

int main() {
	SGFE_window* win = SGFE_windowMake(SGFE_videoModeOptimal(), SGFE_windowBuffer);
	if (win == NULL) { return 1; }

	SGFE_contextBuffer* ctx = SGFE_windowGetContextBuffer(win);
	u8* buffer = SGFE_bufferGetFramebuffer(ctx);
	SGFE_area res = SGFE_bufferGetResolution(ctx);

	glContext context;
	init_glContext(&context, (u32**)&buffer, res.w, res.h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

	float points[] = { 
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	GLuint myshader = pglCreateProgram(identity_vs, uniform_color_fs, 0, NULL, GL_FALSE);
	glUseProgram(myshader);

	My_Uniforms the_uniforms;
	pglSetUniform(&the_uniforms);
	the_uniforms.v_color = (vec4){1.0f, 0.0f, 0.0f, 1.0f};

	GLuint triangle;
	glGenBuffers(1, &triangle);
	glBindBuffer(GL_ARRAY_BUFFER, triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glClearColor(1, 1, 1, 1);

	while (!SGFE_windowShouldClose(win)) {
		const SGFE_event* event;
		while (SGFE_windowCheckEvent(win, &event)) {
			if (event->type == SGFE_buttonPressed && event->button == BUTTON_START) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SGFE_windowSwapBuffers(win);
		pglSetBackBuffer(SGFE_bufferGetFramebuffer(ctx));
	}

	free_glContext(&context);

	SGFE_windowClose(win);
}
