#define PORTABLEGL_IMPLEMENTATION
#define PGL_MAX_VERTICES 5000
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


int main(void) {
	SGFE_window* win = SGFE_windowMake(SGFE_videoGetOptimalMode(), SGFE_windowFlagBuffer);
	if (win == NULL) { return 1; }

	SGFE_contextBuffer* ctx = SGFE_windowGetContextBuffer(win);
	u8* buffer = SGFE_bufferGetFramebuffer(ctx);

	isize width, height;
	SGFE_bufferGetResolution(ctx, &width, &height);

	glContext context;
	SGFE_bool res = init_glContext(&context, (u32**)&buffer, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if (res == SGFE_FALSE) { return 1; }

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
		SGFE_windowPollEvents(win);

		SGFE_controller* p1 = SGFE_windowGetController(win, 0);
		if (SGFE_isDown(p1, BUTTON_START)) {
			SGFE_windowSetShouldClose(win, SGFE_TRUE);
			continue;
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SGFE_windowSwapBuffers(win);
		pglSetBackBuffer(SGFE_bufferGetFramebuffer(ctx));
	}

	free_glContext(&context);

	SGFE_windowClose(win);
}
