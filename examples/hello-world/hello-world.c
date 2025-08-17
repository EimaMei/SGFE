#define SGFE_IMPLEMENTATION 1
#include <SGFE.h>
#include <resources/controls.h>


int main(void) {
	/* In SGFE, the "window" is the structure in which the library stores everything
	 * inside. It is essentially the global state of the application.*/
	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagNone);
	if (win == NULL) { return 1; }

	/* On consoles you usually don't have easy (or any) access to a real terminal.
	 * To get around this SGFE give you the option of turning the entire screen
	 * into a terminal output screen.
	 *
	 * Any function that writes to stdout/stderr (e.g., printf) will have its output
	 * directly blitted into the screen.
	 *
	 * You can also create a window with a terminal output by specifying
	 * 'SGFE_windowFlagTerminal' when creating a window. */
	SGFE_bool output_initialized = SGFE_windowInitTerminalOutput(win);
	if (!output_initialized) { return 1; }

    printf("Hello, world! The system hasn't crashed, thankfully.\n");

	/* The function returns true whenever SGFE gets a "quit" request from either
	 * the system or the user itself by calling 'SGFE_windowSetShouldClose()'. */
	while (!SGFE_windowShouldClose(win)) {
		/* 'SGFE_windowPollEvents' receives all of the events for the system for
		 * that frame.
		 * 
		 * SGFE has 3 ways of checking event information.
		 * 1. Event queue (equivalent to standard SDL) - by enabling the event queue
		 * you're able to iterate through all of the events via a loop and handle
		 * each one inside it. This is the most "desktop-like" design for SGFE.
		 * For an example of this, check out: examples/event_queue.
		 *
		 * 2. Callbacks (equivalent to GLFW) - you can set callback functions that
		 * immediately get called once the specific event is fully processed. The
		 * benefit of this system is that this is the fastest way to receive events
		 * as you don't have to go through an entire queue or wait until polling
		 * is finished. Downside to this is that callbacks might not scale well
		 * (especially for games). For an example of this, check out: examples/callbacks.
		 *
		 * 3. State - each frame 'SGFE_windowPollEvents()' updates the 'state'
		 * structure that resides within the window and returns the pointer to it.
		 * The window state structure contains all of the information about which
		 * events fired and their respective data. This is the simplest and most
		 * console-friendly system out of the 3 since you're not wasting any additional
		 * processing while also having major flexibility with how you'd interpret
		 * the data. For an example of this, check out: examples/window-state. */
        const SGFE_windowState* state = SGFE_windowPollEvents(win);

		/* A very small sample of how you could use the window state structure.
		 * Identical to calling 'SGFE_windowShouldClose()'. */
		if (state->should_quit) {
			printf("Exiting this world! Ciao!\n");
		}

		/* This functions waits for the VBlank and then swaps the buffers of the 
		 * current contexts (if there are any). Currently this function handles
		 * swapping buffer functionality for buffer rendering and OpenGL (if it's
		 * supported on the platform). If no context is initialized, this function
		 * only waits for the VBlank. */
		SGFE_windowSwapBuffers(win);
	}

	/* Closes the window and any system services that were opened. It also handles
	 * any additional exit states unique to the hardware (e.g. the Wii's POWER and
	 * RESET buttons). */
	SGFE_windowClose(win);
}
