#define RGFW_IMPLEMENTATION
#define RGFW_BUFFER
#include <RGFW_embedded.h>

#if RGFW_3DS
	#define SILK_PIXELBUFFER_WIDTH  400
    #define SILK_PIXELBUFFER_HEIGHT 240

    #define BUTTON_QUIT   RGFW_Start
#endif

#define SILK_IMPLEMENTATION
#include "silk.h"

typedef int8_t      i8;
typedef uint16_t   u16;
typedef int16_t    i16;
typedef uint64_t   u64;
typedef int64_t    i64;


int main(void) {
    RGFW_window* win = RGFW_createWindow(RGFW_videoModeOptimal(), 0);

    RGFW_bool running = RGFW_TRUE;
    while (running) {
        while (RGFW_window_checkEvent(win)) {
            if (win->event->type == RGFW_quit || RGFW_isPressed(win->event->controller, BUTTON_QUIT)) {
                running = 0;
                break;
            }   
        } 
        
        silkClearPixelBufferColor((pixel*)win->buffer, 0x11AA0033);

        silkDrawRect(
            (pixel*)win->buffer, 
            (vec2i) { SILK_PIXELBUFFER_WIDTH, SILK_PIXELBUFFER_HEIGHT },
            SILK_PIXELBUFFER_WIDTH,
            (vec2i) { 150, 100}, 
            (vec2i) { 200, 200 },
            0xff0000ff
        );

        const char* text = "Hello, RGFW!";
        const size_t text_size = 8;
        const i32 text_spacing = 1;

        silkDrawTextDefault(
            (pixel*)win->buffer, 
            (vec2i) { SILK_PIXELBUFFER_WIDTH, SILK_PIXELBUFFER_HEIGHT },
            SILK_PIXELBUFFER_WIDTH,
            text, 
            (vec2i) { 
                win->bufferSize.w / 2 - silkMeasureText(text, text_size, text_spacing).x / 2, 
                win->bufferSize.h / 2 - silkMeasureText(text, text_size, text_spacing).y / 2 + win->bufferSize.h / 4 
            }, 
            text_size,
            text_spacing,
            0xff000000
        );
        

        RGFW_window_swapBuffers(win);
    }

    RGFW_window_close(win);
}
