/*
*
*	RGFW 1.7.5-dev

* Copyright (C) 2022-25 Riley Mabb (@ColleagueRiley)
*
* libpng license
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.

* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/

/*
	(MAKE SURE RGFW_IMPLEMENTATION is in exactly one header or you use -D RGFW_IMPLEMENTATION)
	#define RGFW_IMPLEMENTATION - makes it so source code is included with header
*/

/*
	#define RGFW_IMPLEMENTATION - (required) makes it so the source code is included
	#define RGFW_DEBUG - (optional) makes it so RGFW prints debug messages and errors when they're found
	#define RGFW_BUFFER - (optional) draw directly to (RGFW) window pixel buffer that is drawn to screen (the buffer is in the RGBA format)
	#define RGFW_EGL - (optional) use EGL for loading an OpenGL context (instead of the system's OpenGL api)
	#define RGFW_DIRECTX - (optional) include integration directX functions (windows only)
	#define RGFW_VULKAN - (optional) include helpful vulkan integration functions and macros
	#define RGFW_WEBGPU - (optional) use webGPU for rendering (Web ONLY)
	#define RGFW_NO_API - (optional) don't use any rendering API (no OpenGL, no vulkan, no directX)

	#define RGFW_LINK_EGL (optional) (windows only) if EGL is being used, if EGL functions should be defined dymanically (using GetProcAddress)
	#define RGFW_X11 (optional) (unix only) if X11 should be used. This option is turned on by default by unix systems except for MacOS
	#define RGFW_WAYLAND (optional) (unix only) use Wayland. (This can be used with X11)
	#define RGFW_NO_X11 (optional) (unix only) don't fallback to X11 when using Wayland
	#define RGFW_NO_LOAD_WGL (optional) (windows only) if WGL should be loaded dynamically during runtime
	#define RGFW_NO_X11_CURSOR (optional) (unix only) don't use XCursor
	#define RGFW_NO_X11_CURSOR_PRELOAD (optional) (unix only) use XCursor, but don't link it in code, (you'll have to link it with -lXcursor)
	#define RGFW_NO_X11_EXT_PRELOAD (optional) (unix only) use Xext, but don't link it in code, (you'll have to link it with -lXext)
    #define RGFW_NO_LOAD_WINMM (optional) (windows only) use winmm (timeBeginPeriod), but don't link it in code, (you'll have to link it with -lwinmm)
	#define RGFW_NO_WINMM (optional) (windows only) don't use winmm
	#define RGFW_NO_IOKIT (optional) (macOS) don't use IOKit
	#define RGFW_NO_UNIX_CLOCK (optional) (unix) don't link unix clock functions
	#define RGFW_NO_DWM (windows only) - do not use or link dwmapi
	#define RGFW_USE_XDL (optional) (X11) if XDL (XLib Dynamic Loader) should be used to load X11 dynamically during runtime (must include XDL.h along with RGFW)
	#define RGFW_COCOA_GRAPHICS_SWITCHING - (optional) (cocoa) use automatic graphics switching (allow the system to choose to use GPU or iGPU)
	#define RGFW_NO_DPI - do not calculate DPI (no XRM nor libShcore included)
	#define RGFW_BUFFER_BGR - use the BGR format for bufffers instead of RGB, saves processing time
    #define RGFW_ADVANCED_SMOOTH_RESIZE - use advanced methods for smooth resizing (may result in a spike in memory usage or worse performance) (eg. WM_TIMER and XSyncValue)
    #define RGFW_NO_INFO - do not define the RGFW_info struct (without RGFW_IMPLEMENTATION)

	#define RGFW_ALLOC x  - choose the default allocation function (defaults to standard malloc)
	#define RGFW_FREE x  - choose the default deallocation function (defaults to standard free)

	#define RGFW_EXPORT - use when building RGFW
	#define RGFW_IMPORT - use when linking with RGFW (not as a single-header)

	#define RGFW_USE_INT - force the use c-types rather than stdint.h (for systems that might not have stdint.h (msvc))
	#define RGFW_bool x - choose what type to use for bool, by default u32 is used
*/

/*
Example to get you started :

linux : gcc main.c -lX11 -lXrandr -lGL
windows : gcc main.c -lopengl32 -lgdi32
macos : gcc main.c -framework Cocoa -framework CoreVideo -framework OpenGL -framework IOKit

#define RGFW_IMPLEMENTATION
#include "RGFW.h"

u8 icon[4 * 3 * 3] = {0xFF, 0x00, 0x00, 0xFF,    0xFF, 0x00, 0x00, 0xFF,     0xFF, 0x00, 0x00, 0xFF,   0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,     0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xFF};

int main() {
	RGFW_window* win = RGFW_createWindow("name", RGFW_RECT(100, 100, 500, 500), (u64)0);

	RGFW_window_setIcon(win, icon, RGFW_AREA(3, 3), 4);

	while (RGFW_window_shouldClose(win) == RGFW_FALSE) {
		while (RGFW_window_checkEvent(win)) {
		    if (win->event.type == RGFW_quit || RGFW_isPressed(win, RGFW_escape))
			    break;
        }

		RGFW_window_swapBuffers(win);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	RGFW_window_close(win);
}

	compiling :

	if you wish to compile the library all you have to do is create a new file with this in it

	rgfw.c
	#define RGFW_IMPLEMENTATION
	#include "RGFW.h"

	You may also want to add
	`#define RGFW_EXPORT` when compiling and
	`#define RGFW_IMPORT`when linking RGFW on it's own:
	this reduces inline functions and prevents bloat in the object file

	then you can use gcc (or whatever compile you wish to use) to compile the library into object file

	ex. gcc -c RGFW.c -fPIC

	after you compile the library into an object file, you can also turn the object file into an static or shared library

	(commands ar and gcc can be replaced with whatever equivalent your system uses)

	static : ar rcs RGFW.a RGFW.o
	shared :
		windows:
			gcc -shared RGFW.o -lopengl32 -lgdi32 -o RGFW.dll
		linux:
			gcc -shared RGFW.o -lX11 -lGL -lXrandr -o RGFW.so
		macos:
			gcc -shared RGFW.o -framework CoreVideo -framework Cocoa -framework OpenGL -framework IOKit
*/



/*
	Credits :
		EimaMei/Sacode : Much of the code for creating windows using winapi, Wrote the Silicon library, helped with MacOS Support, siliapp.h -> referencing

		stb - This project is heavily inspired by the stb single header files

		GLFW:
			certain parts of winapi and X11 are very poorly documented,
			GLFW's source code was referenced and used throughout the project.

		contributors : (feel free to put yourself here if you contribute)
		krisvers (@krisvers) -> code review
		EimaMei (@SaCode) -> code review
		Nycticebus (@Code-Nycticebus) -> bug fixes
		Rob Rohan (@robrohan) -> X11 bugs and missing features, MacOS/Cocoa fixing memory issues/bugs
		AICDG (@THISISAGOODNAME) -> vulkan support (example)
		@Easymode -> support, testing/debugging, bug fixes and reviews
		Joshua Rowe (omnisci3nce) - bug fix, review (macOS)
		@lesleyrs -> bug fix, review (OpenGL)
        Nick Porcino (@meshula) - testing, organization, review (MacOS, examples)
        @therealmarrakesh -> documentation
        @DarekParodia -> code review (X11) (C++)
        @NishiOwO -> fix BSD support, fix OSMesa example
        @BaynariKattu -> code review and documentation
        Miguel Pinto (@konopimi) -> code review, fix vulkan example
        @m-doescode -> code review (wayland)
        Robert Gonzalez (@uni-dos) -> code review (wayland)
        @TheLastVoyager -> code review
        @yehoravramenko -> code review (winapi)
        @halocupcake -> code review (OpenGL)
        @GideonSerf -> documentation
        Alexandre Almeida (@M374LX) -> code review (keycodes)
        Vũ Xuân Trường (@wanwanvxt) -> code review (winapi)
        Lucas (@lightspeedlucas) -> code review (msvc++)
        Jeffery Myers (@JeffM2501) -> code review (msvc)
        Zeni (@zenitsuyo) -> documentation
        TheYahton (@TheYahton) -> documentation
        nonexistant_object (@DiarrheaMcgee
*/

#if _MSC_VER
	#pragma comment(lib, "gdi32")
	#pragma comment(lib, "shell32")
	#pragma comment(lib, "User32")
    #pragma warning( push )
	#pragma warning( disable : 4996 4191 4127)
    #if _MSC_VER < 600
        #define RGFW_C89
    #endif
#else
    #if defined(__STDC__) && !defined(__STDC_VERSION__)
        #define RGFW_C89
    #endif
#endif

#ifndef RGFW_UNUSED
	#define RGFW_UNUSED(x) (void)(x)
#endif

#ifndef RGFW_ALLOC
	#include <stdlib.h>
	#define RGFW_ALLOC malloc
	#define RGFW_FREE free
#endif

#ifndef RGFW_ASSERT
	#include <assert.h>
	#define RGFW_ASSERT assert
#endif

#if !defined(RGFW_MEMCPY) || !defined(RGFW_STRNCMP) || !defined(RGFW_MEMSET)
	#include <string.h>
#endif

#ifndef RGFW_MEMSET
	#define RGFW_MEMSET(ptr, value, num) memset(ptr, value, num)
#endif

#ifndef RGFW_MEMCPY
	#define RGFW_MEMCPY(dist, src, len) memcpy(dist, src, len)
#endif

#if !defined(RGFW_PRINTF) && ( defined(RGFW_DEBUG) || defined(RGFW_WAYLAND) )
    /* required when using RGFW_DEBUG */
    #include <stdio.h>
    #define RGFW_PRINTF printf
#endif

#if defined(RGFW_EXPORT) || defined(RGFW_IMPORT)
	#if defined(RGFW_EXPORT)
		#define RGFWDEF __attribute__((visibility("default")))
	#endif
    #ifndef RGFWDEF
        #define RGFWDEF
    #endif
#endif

#ifndef RGFWDEF
	#ifdef RGFW_C89
		#define RGFWDEF __inline
	#else
		#define RGFWDEF inline
	#endif
#endif

#ifndef RGFW_ENUM
	#define RGFW_ENUM(type, name) type name; enum
#endif


#if defined(__cplusplus) && !defined(__EMSCRIPTEN__)
	extern "C" {
#endif

	/* makes sure the header file part is only defined once by default */
#ifndef RGFW_HEADER

#define RGFW_HEADER

#include <stddef.h>
#ifndef RGFW_INT_DEFINED
	#ifdef RGFW_USE_INT /* optional for any system that might not have stdint.h */
		typedef unsigned char       u8;
		typedef signed char         i8;
		typedef unsigned short     u16;
		typedef signed short 	   i16;
		typedef unsigned long int  u32;
		typedef signed long int    i32;
		typedef unsigned long long u64;
		typedef signed long long   i64;
	#else /* use stdint standard types instead of c "standard" types */
		#include <stdint.h>

		typedef uint8_t     u8;
		typedef int8_t      i8;
		typedef uint16_t   u16;
		typedef int16_t    i16;
		typedef uint32_t   u32;
		typedef int32_t    i32;
		typedef uint64_t   u64;
		typedef int64_t    i64;
	#endif

	#define RGFW_INT_DEFINED
#endif

#ifndef RGFW_BOOL_DEFINED
    #define RGFW_BOOL_DEFINED
    typedef u8 RGFW_bool;
#endif

#define RGFW_BOOL(x) (RGFW_bool)((x) ? RGFW_TRUE : RGFW_FALSE) /* force an value to be 0 or 1 */
#define RGFW_TRUE (RGFW_bool)1
#define RGFW_FALSE (RGFW_bool)0

/* these OS macros look better & are standardized */
/* plus it helps with cross-compiling */

#ifdef __3DS__
	#ifndef RGFW_3DS
	#define RGFW_3DS 1
	#endif

	#define RGFW_MAX_CONTROLLER_DEVICES 1

	/* TODO(EimaMei): Circle Pad Pro support would require this to be 4, but as
	 * of June 29th 2025 I'm not planning to add support for it anytime soon,
	 * unless this PR gets merged: https://github.com/devkitPro/libctru/pull/568. */

	#define RGFW_HAS_KEYBOARD_SUPPORT  0
	#define RGFW_HAS_MOUSE_SUPPORT     0

	#include <3ds.h>

	#ifdef RGFW_OPENGL
	#define RIP_BACKEND RIP_BACKEND_KYGX
	#include <GLES/gl2.h>
	#endif

	/* TODO(EimaMei): comment. */
	#ifndef RGFW_ALLOC_SYS
	#define RGFW_ALLOC_SYS(size) linearAlloc((size_t)(size))
	#define RGFW_FREE_SYS(ptr) linearFree(ptr)
	#endif
#endif

/*
	regular RGFW stuff
*/

#define RGFW_key u8

typedef RGFW_ENUM(u8, RGFW_eventType) {
	/*! event codes */
	RGFW_eventNone = 0, /*!< no event has been sent */

	RGFW_controllerConnected, /*!< a controller was connected */
	RGFW_controllerDisconnected, /*!< a controller was disconnected */
	RGFW_buttonPressed, /*!< a controller button was pressed */
	RGFW_buttonReleased, /*!< a controller button was released */
	RGFW_controllerAxisMove, /*!< an axis of a controller was moved */

	RGFW_pointerPressed, /* TODO(EimaMei): Do research on this. */
	RGFW_pointerMove, /* TODO(EimaMei): NEW ENUM* */

	RGFW_motionMove, /* TODO(EimaMei): NEW ENUM* */
	/*! controller event note
		RGFW_event.controller holds which controller was altered, if any
		RGFW_event.button holds which controller button was pressed

		RGFW_event.axis holds the data of all the axises
		RGFW_event.axisesCount says how many axises there are
	*/
	RGFW_windowResized, /*!< the window was resized (by the user), [on WASM this means the browser was resized] */
	RGFW_windowRefresh, /* The window content needs to be refreshed */

	/* attribs change event note
		The event data is sent straight to the window structure
		with win->r.x, win->r.y, win->r.w and win->r.h
	*/
	RGFW_quit, /*!< the user clicked the quit button */
	RGFW_windowMaximized, /*!< the window was maximized */
	RGFW_windowMinimized, /*!< the window was minimized */
	RGFW_windowRestored, /*!< the window was restored */
	RGFW_scaleUpdated /*!< content scale factor changed */
};

#ifndef RGFW_MAX_PATH
#define RGFW_MAX_PATH 260 /* max length of a path (for dnd) */
#endif
#ifndef RGFW_MAX_DROPS
#define RGFW_MAX_DROPS 260 /* max items you can drop at once */
#endif

#define RGFW_BIT(x) (1 << x)

/* for RGFW_event.lockstate */
typedef RGFW_ENUM(u8, RGFW_keymod) {
	RGFW_modCapsLock = RGFW_BIT(0),
	RGFW_modNumLock  = RGFW_BIT(1),
	RGFW_modControl  = RGFW_BIT(2),
	RGFW_modAlt = RGFW_BIT(3),
	RGFW_modShift  = RGFW_BIT(4),
	RGFW_modSuper = RGFW_BIT(5),
	RGFW_modScrollLock = RGFW_BIT(6)
};

/* NOTE(EimaMei): The following input enums are implementation-defined. The first
 * value will always be equal to zero, while the following types' values are
 * incremented linearly. */

typedef RGFW_ENUM(i32, RGFW_controllerType) {
	/* NOTE(EimaMei): Value '0' indicates the 'Default' controller used for
	 * the system. Must be present on all implementations. */
	RGFW_controllerTypeStandard = 0,

	RGFW_controllerTypeCount,
};

typedef RGFW_ENUM(i32, RGFW_axisType) {
#ifdef RGFW_3DS
	RGFW_axisTypeLeftX,
	RGFW_axisTypeLeftY,
	/* TODO(EimaMei): As of July 2nd 2025 Circle Pad Pro support isn't going to
	 * be added until this PR gets merged: https://github.com/devkitPro/libctru/pull/568. */
	/* RGFW_axisTypeRightX,
	RGFW_axisTypeRightY, */
#endif

	RGFW_axisTypeCount,
};

typedef RGFW_ENUM(i32, RGFW_pointerType) {
#ifdef RGFW_3DS
	RGFW_pointerTouchscreen,
#endif

	RGFW_pointerTypeCount,
};

typedef RGFW_ENUM(i32, RGFW_motionType) {
#ifdef RGFW_3DS
	RGFW_motionAccelerometer,
	RGFW_motionGyroscope,
#endif

	RGFW_motionTypeCount,
};

typedef RGFW_ENUM(i32, RGFW_button) {
	RGFW_buttonInvalid = -1,

	/* NOTE(EimaMei): The first controller type's (aka the default controller type's)
	 * buttons take precedence in the naming scheme, meaning they can be named as
	 * 'RGFW_<button name>' instead of 'RGFW_<controller type><Button name>'. */
#ifdef RGFW_3DS
	RGFW_A = 0,
	RGFW_B,
	RGFW_Select,
	RGFW_Start,
	RGFW_DpadRight,
	RGFW_DpadLeft,
	RGFW_DpadUp,
	RGFW_DpadDown,
	RGFW_R,
	RGFW_L,
	RGFW_X,
	RGFW_Y,
	RGFW_ZL,
	RGFW_ZR,
	RGFW_CstickRight,
	RGFW_CstickLeft,
	RGFW_CstickUp,
	RGFW_CstickDown,

#endif

	RGFW_controllerButtonCount
};

/*! basic vector type, if there's not already a point/vector type of choice */
#ifndef RGFW_point
	typedef struct RGFW_point { i32 x, y; } RGFW_point;
#endif

/* TODO(EimaMei): New structure. */
#ifndef RGFW_point3D
	typedef struct RGFW_point3D { float x, y, z; } RGFW_point3D;
#endif

/*! basic rect type, if there's not already a rect type of choice */
#ifndef RGFW_rect
	typedef struct RGFW_rect { i32 x, y, w, h; } RGFW_rect;
#endif

/*! basic area type, if there's not already a area type of choice */
#ifndef RGFW_area
	typedef struct RGFW_area { i32 w, h; } RGFW_area;
#endif

#if defined(__cplusplus) && !defined(__APPLE__)
#define RGFW_POINT(x, y) {(i32)x, (i32)y}
#define RGFW_RECT(x, y, w, h) {(i32)x, (i32)y, (i32)w, (i32)h}
#define RGFW_AREA(w, h) {(u32)w, (u32)h}
#else
#define RGFW_POINT(x, y) (RGFW_point){(i32)(x), (i32)(y)}
#define RGFW_RECT(x, y, w, h) (RGFW_rect){(i32)(x), (i32)(y), (i32)(w), (i32)(h)}
#define RGFW_AREA(w, h) (RGFW_area){w, h}
#endif

typedef struct RGFW_axis {
	float value;
	float deadzone;
} RGFW_axis;

typedef RGFW_ENUM(u8, RGFW_buttonState) {
	RGFW_buttonStateSupported = RGFW_BIT(0),
	RGFW_buttonStateCurrent   = RGFW_BIT(1),
	RGFW_buttonStatePrevious  = RGFW_BIT(2),
} RGFW_keyState;

/* TODO(EimaMei): new structure. */
typedef struct RGFW_controller {
	RGFW_bool connected;
	RGFW_controllerType type;

	/* Input */
	RGFW_buttonState buttons[RGFW_controllerButtonCount];
    RGFW_axis axes[RGFW_axisTypeCount];
	RGFW_point pointers[RGFW_pointerTypeCount];
	RGFW_point3D motions[RGFW_motionTypeCount];

	RGFW_button button_start, button_end;
	RGFW_bool enabled_pointers[RGFW_pointerTypeCount];
	RGFW_bool enabled_motions[RGFW_motionTypeCount];
} RGFW_controller;

/*! Event structure for checking/getting events */
typedef struct RGFW_event {
	RGFW_eventType type; /*!< which event has been sent?*/
	RGFW_controller* controller;

	RGFW_axisType axis;
	RGFW_pointerType pointer;
	RGFW_motionType motion;
	RGFW_button button;

	void* _win; /*!< the window this event applies too (for event queue events) */
} RGFW_event;


/* TODO(EimaMei): Add RGFW_NATIVE. Add an option to disable all of the non optimal modes. */
/* TODO(EimaMei): document */
typedef RGFW_ENUM(i32, RGFW_videoMode) {
#ifdef RGFW_3DS
	RGFW_videoMode2D,
	RGFW_videoMode3D,
	RGFW_videoModeWide, /* NOTE(EimaMei): Doesn't work on regular 2DS consoles. TODO(EimaMei): Add an error if you select this on 2DS consoles.*/
	RGFW_videoModeBottomScreen,
#endif

	RGFW_videoModeCount
};

/* TODO(EimaMei): document */
typedef RGFW_ENUM(i32, RGFW_pixelFormat) {
	RGFW_pixelFormatRGBA8,

#ifdef RGFW_3DS
	RGFW_pixelFormatBGR8,
	RGFW_pixelFormatRGB565,
	RGFW_pixelFormatRGB5_A1,
	RGFW_pixelFormatRGBA4,
#endif

	RGFW_pixelFormatCount,
};


/* TODO(EimaMei): document */
typedef RGFW_ENUM(i32, RGFW_systemModel) {
#if RGFW_3DS
	RGFW_systemModel3DS,
	RGFW_systemModel3DSXL,
	RGFW_systemModelN3DS,
	RGFW_systemModel2DS,
	RGFW_systemModelN3DSXL,
	RGFW_systemModelN2DSXL,
#endif

	RGFW_systemModelCount,
};

/*! source data for the window (used by the APIs) */
#if defined(RGFW_3DS)

typedef struct RGFW_window_src {
	RGFW_bool has_checked_events;
	accelVector accel;
	angularRate gyro;

	#if defined(RGFW_OPENGL)
		GLASSCtx ctx;
		GLuint framebuffer;
		GLuint renderbuffer;
	#endif

	#if defined(RGFW_BUFFER)
		u8* buffers[2];
		#ifndef RGFW_BUFFER_NATIVE
		u8* buffers_native[2];
		#endif
		ssize_t current_buffer;
		ssize_t buffer_size;
	#endif
} RGFW_window_src;

#endif

/*! Optional arguments for making a windows */
typedef RGFW_ENUM(u32, RGFW_windowFlags) {
	RGFW_windowNoInitAPI      = RGFW_BIT(0), /* do NOT init an API (including the software rendering buffer) (mostly for bindings. you can also use `#define RGFW_NO_API`) */
	RGFW_windowConsoleInit    = RGFW_BIT(1), /* TODO(EimaMei): New enum. */
	RGFW_windowFreeOnClose    = RGFW_BIT(15), /*!< free (RGFW_window_close) the RGFW_window struct when the window is closed (by the end user) */
};

typedef struct RGFW_window {
	RGFW_window_src src; /*!< src window data */

#if defined(RGFW_BUFFER)
	u8* buffer; /*!< buffer for software rendering */
	RGFW_area bufferSize;
#endif
	void* userPtr; /* ptr for usr data */

	RGFW_event* event; /*!< current event */

	RGFW_videoMode mode; /* TODO(EimaMei): document */
	RGFW_pixelFormat format; /* TODO(EimaMei): document */

    /*! which key RGFW_window_shouldClose checks. Settting this to RGFW_keyNULL disables the feature. */
   // RGFW_key exitKey;

	u32 _flags; /*!< windows flags (for RGFW to check) */
} RGFW_window; /*!< window structure for managing the window */


/** * @defgroup Window_management
* @{ */


RGFWDEF RGFW_window* RGFW_createWindow(
	RGFW_videoMode mode, /* TODO(EimaMei): document */
	RGFW_windowFlags flags /* extra arguments ((u32)0 means no flags used)*/
); /*!< function to create a window and struct */

RGFWDEF RGFW_window* RGFW_createWindowPtr(
	RGFW_videoMode mode, /* TODO(EimaMei): document */
	RGFW_windowFlags flags, /* extra arguments (NULL / (u32)0 means no flags used) */
	RGFW_window* win /* ptr to the window struct you want to use */
); /*!< function to create a window (without allocating a window struct) */

RGFWDEF void RGFW_window_initBuffer(RGFW_window* win);
RGFWDEF void RGFW_window_initBufferSize(RGFW_window* win, RGFW_videoMode mode, RGFW_pixelFormat format);
RGFWDEF void RGFW_window_initBufferPtr(RGFW_window* win, u8* buffer, RGFW_videoMode mode, RGFW_pixelFormat format);

/*! set the window flags (will undo flags if they don't match the old ones) */
RGFWDEF void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags);

RGFWDEF RGFW_area RGFW_windowGetSize(RGFW_window* win);


/*!
	this function checks an *individual* event (and updates window structure attributes)
	this means, using this function without a while loop may cause event lag

	ex.

	while (RGFW_window_checkEvent(win) != NULL) [this keeps checking events until it reaches the last one]

	this function is optional if you choose to use event callbacks,
	although you still need some way to tell RGFW to process events eg. `RGFW_window_checkEvents`
*/

RGFWDEF RGFW_event* RGFW_window_checkEvent(RGFW_window* win); /*!< check current event (returns a pointer to win->event or NULL if there is no event)*/

/*!
	for RGFW_window_eventWait and RGFW_window_checkEvents
	waitMS -> Allows the function to keep checking for events even after `RGFW_window_checkEvent == NULL`
			  if waitMS == 0, the loop will not wait for events
			  if waitMS > 0, the loop will wait that many miliseconds after there are no more events until it returns
			  if waitMS == -1 or waitMS == the max size of an unsigned 32-bit int, the loop will not return until it gets another event
*/
typedef RGFW_ENUM(i32, RGFW_eventWait) {
	RGFW_eventNoWait = 0,
	RGFW_eventWaitNext = -1
};

/*! sleep until RGFW gets an event or the timer ends (defined by OS) */
RGFWDEF void RGFW_window_eventWait(RGFW_window* win, i32 waitMS);

/*!
	check all the events until there are none left.
	This should only be used if you're using callbacks only
*/
RGFWDEF void RGFW_window_checkEvents(RGFW_window* win, i32 waitMS);

/*!
	tell RGFW_window_eventWait to stop waiting (to be ran from another thread)
*/
RGFWDEF void RGFW_stopCheckEvents(void);

/*! window managment functions */
RGFWDEF void RGFW_window_close(RGFW_window* win); /*!< close the window and free leftover data */

/*
	Locks cursor at the center of the window
	win->event.point becomes raw mouse movement data

	this is useful for a 3D camera
*/
RGFWDEF void RGFW_window_mouseHold(RGFW_window* win, RGFW_area area);
/*! if the mouse is held by RGFW */
RGFWDEF RGFW_bool RGFW_window_mouseHeld(RGFW_window* win);
/*! stop holding the mouse and let it move freely */
RGFWDEF void RGFW_window_mouseUnhold(RGFW_window* win);

/*
	makes it so `RGFW_window_shouldClose` returns true or overrides a window close
	by modifying window flags
*/
RGFWDEF void RGFW_window_setShouldClose(RGFW_window* win, RGFW_bool shouldClose);

/*! if the window should close (RGFW_close was sent or escape was pressed) */
RGFWDEF RGFW_bool RGFW_window_shouldClose(RGFW_window* win);

/* TODO(EimaMei): new function. */
RGFWDEF void RGFW_window_consoleInit(RGFW_window* win);

/** @} */

/** @} */

/** * @defgroup Input
* @{ */

RGFWDEF RGFW_bool RGFW_isPressed(RGFW_controller* controller, RGFW_button button); /*!< if key is pressed (key code)*/

RGFWDEF RGFW_bool RGFW_wasPressed(RGFW_controller* controller, RGFW_button button); /*!< if key was pressed (checks previous state only) (key code) */

RGFWDEF RGFW_bool RGFW_isHeld(RGFW_controller* controller, RGFW_button button); /*!< if key is held (key code) */
RGFWDEF RGFW_bool RGFW_isReleased(RGFW_controller* controller, RGFW_button button); /*!< if key is released (key code) */


/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF ssize_t RGFW_controllerGetCount(void);

/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF RGFW_controller* RGFW_controllerGet(ssize_t port);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF void RGFW_controllerPointerEnable(RGFW_controller* controller,
		RGFW_pointerType pointer, RGFW_bool enable);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF void RGFW_controllerMotionEnable(RGFW_controller* controller,
		RGFW_motionType motion, RGFW_bool enable);


/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_controllerName(RGFW_controllerType type);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_buttonName(RGFW_button button);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_axisName(RGFW_axisType axis);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_pointerName(RGFW_pointerType type);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_motionName(RGFW_motionType type);


/** * @defgroup error handling
* @{ */
typedef RGFW_ENUM(u8, RGFW_debugType) {
	RGFW_typeError = 0, RGFW_typeWarning, RGFW_typeInfo
};

typedef RGFW_ENUM(u8, RGFW_errorCode) {
	RGFW_noError = 0, /*!< no error */
	RGFW_errOutOfMemory,
	RGFW_errOpenGLContext, RGFW_errEGLContext, /*!< error with the OpenGL context */
	RGFW_errWayland, RGFW_errX11,
	RGFW_errIOKit,
	RGFW_errClipboard,
	RGFW_errFailedFuncLoad,
	RGFW_errBuffer,
	RGFW_errEventQueue,
	RGFW_infoWindow, RGFW_infoBuffer, RGFW_infoGlobal, RGFW_infoOpenGL,
	RGFW_warningWayland, RGFW_warningOpenGL,
};

typedef struct RGFW_debugContext { RGFW_window* win; u32 srcError; } RGFW_debugContext;

#if defined(__cplusplus) && !defined(__APPLE__)
#define RGFW_DEBUG_CTX(win, err) {win, err}
#else
#define RGFW_DEBUG_CTX(win, err) (RGFW_debugContext){win, err}
#endif

typedef void (* RGFW_debugfunc)(RGFW_debugType type, RGFW_errorCode err, RGFW_debugContext ctx, const char* msg);
RGFWDEF RGFW_debugfunc RGFW_setDebugCallback(RGFW_debugfunc func);
RGFWDEF void RGFW_sendDebugInfo(RGFW_debugType type, RGFW_errorCode err, RGFW_debugContext ctx, const char* msg);
/** @} */

/**


	event callbacks.
	These are completely optional, so you can use the normal
	RGFW_checkEvent() method if you prefer that

* @defgroup Callbacks
* @{
*/

/*! RGFW_windowResized, the window and its new rect value  */
typedef void (* RGFW_windowResizedfunc)(RGFW_window* win, RGFW_rect r);
/*! RGFW_quit, the window that was closed */
typedef void (* RGFW_windowQuitfunc)(RGFW_window* win);
/*! RGFW_mouseEnter / RGFW_mouseLeave, the window that changed, the point of the mouse (enter only) and if the mouse has entered */
typedef void (* RGFW_mouseNotifyfunc)(RGFW_window* win, RGFW_point point, RGFW_bool status);
/*! RGFW_buttonPressed, the window that got the event, the button that was pressed, the scroll value, if it was a press (else it's a release) */
typedef void (* RGFW_buttonfunc)(RGFW_window* win, RGFW_controller* controller, RGFW_button button, RGFW_bool pressed);
/*! RGFW_controllerAxisMove, the window that got the event, the controller in question, the axis values and the axis count */
typedef void (* RGFW_controllerAxisfunc)(RGFW_window* win, RGFW_controller* controller, RGFW_axisType axis);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (* RGFW_pointerMovefunc)(RGFW_window* win, RGFW_controller* controller, RGFW_pointerType pointer);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (* RGFW_motionMovefunc)(RGFW_window* win, RGFW_controller* controller, RGFW_motionType motion);
/*! RGFW_controllerConnected / RGFW_controllerDisconnected, the window that got the event, the controller in question, if the controller was connected (else it was disconnected) */
typedef void (* RGFW_controllerfunc)(RGFW_window* win, RGFW_controller* controller, RGFW_bool connected);

/*! set callback for a window resize event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_windowResizedfunc RGFW_setWindowResizedCallback(RGFW_windowResizedfunc func);
/*! set callback for a window quit event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_windowQuitfunc RGFW_setWindowQuitCallback(RGFW_windowQuitfunc func);
/*! set callback for a mouse notify event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_mouseNotifyfunc RGFW_setMouseNotifyCallback(RGFW_mouseNotifyfunc func);
/*! set callback for a controller button (press / release) event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_buttonfunc RGFW_setButtonCallback(RGFW_buttonfunc func);
/*! set callback for a controller axis move event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_controllerAxisfunc RGFW_setControllerAxisCallback(RGFW_controllerAxisfunc func);
/* TODO(EimaMei): NEW FUNCTION. */
RGFWDEF RGFW_pointerMovefunc RGFW_setPointerMoveCallback(RGFW_pointerMovefunc func);
/* TODO(EimaMei): NEW FUNCTION. */
RGFWDEF RGFW_motionMovefunc RGFW_setMotionMoveCallback(RGFW_motionMovefunc func);
/*! set callback for when a controller is connected or disconnected. Returns the previous callback function (if it was set) */
RGFWDEF RGFW_controllerfunc RGFW_setControllerCallback(RGFW_controllerfunc func);
/*! set call back for when window is maximized. Returns the previous callback function (if it was set) */
RGFWDEF RGFW_windowResizedfunc RGFW_setWindowMaximizedCallback(RGFW_windowResizedfunc func);
/*! set call back for when window is minimized. Returns the previous callback function (if it was set) */
RGFWDEF RGFW_windowResizedfunc RGFW_setWindowMinimizedCallback(RGFW_windowResizedfunc func);
/*! set call back for when window is restored. Returns the previous callback function (if it was set) */
RGFWDEF RGFW_windowResizedfunc RGFW_setWindowRestoredCallback(RGFW_windowResizedfunc func);
/** @} */


/** * @defgroup graphics_API
* @{ */

/*!< make the window the current OpenGL drawing context

	NOTE:
 	if you want to switch the graphics context's thread,
	you have to run RGFW_window_makeCurrent(NULL); on the old thread
	then RGFW_window_makeCurrent(valid_window) on the new thread
*/
RGFWDEF void RGFW_window_makeCurrent(RGFW_window* win);

/*! get current RGFW window graphics context */
RGFWDEF RGFW_window* RGFW_getCurrent(void);

/* supports OpenGL and software rendering */
RGFWDEF void RGFW_window_swapBuffers(RGFW_window* win); /*!< swap the rendering buffer */
RGFWDEF void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval);
/*!< render the software rendering buffer (this is called by RGFW_window_swapInterval)  */
RGFWDEF void RGFW_window_swapBuffers_software(RGFW_window* win);

typedef void (*RGFW_proc)(void); /* function pointer equivalent of void* */

/*! native API functions */
#if defined(RGFW_OPENGL) || defined(RGFW_EGL)
/*!< create an OpenGL context for the RGFW window, run by createWindow by default (unless the RGFW_windowNoInitAPI is included) */
RGFWDEF bool RGFW_window_initOpenGL(RGFW_window* win);
/*!< called by `RGFW_window_close` by default (unless the RGFW_windowNoInitAPI is set) */
RGFWDEF void RGFW_window_freeOpenGL(RGFW_window* win);

/*! OpenGL init hints */
typedef RGFW_ENUM(i32, RGFW_glHints)  {
	RGFW_glStencil = 0,  /*!< set stencil buffer bit size (8 by default) */
	RGFW_glSamples, /*!< set number of sample buffers (4 by default) */
	RGFW_glStereo, /*!< hint the context to use stereoscopic frame buffers for 3D (false by default) */
	RGFW_glAuxBuffers, /*!< number of aux buffers (0 by default) */
	RGFW_glDoubleBuffer, /*!< request double buffering (true by default) */
	RGFW_glRed, RGFW_glGreen, RGFW_glBlue, RGFW_glAlpha, /*!< set color bit sizes (all 8 by default) */
	RGFW_glDepth, /*!< set depth buffer bit size (24 by default) */
	RGFW_glAccumRed, RGFW_glAccumGreen, RGFW_glAccumBlue,RGFW_glAccumAlpha, /*!< set accumulated RGBA bit sizes (all 0 by default) */
	RGFW_glSRGB, /*!< request sRGA format (false by default) */
	RGFW_glRobustness, /*!< request a "robust" (as in memory-safe) context (false by default). For more information check the overview section: https://registry.khronos.org/OpenGL/extensions/EXT/EXT_robustness.txt */
	RGFW_glDebug, /*!< request OpenGL debugging (false by default). */
	RGFW_glNoError, /*!< request no OpenGL errors (false by default). This causes OpenGL errors to be undefined behavior. For more information check the overview section: https://registry.khronos.org/OpenGL/extensions/KHR/KHR_no_error.txt */
	RGFW_glReleaseBehavior, /*!< hint how the OpenGL driver should behave when changing contexts (RGFW_glReleaseNone by default). For more information check the overview section: https://registry.khronos.org/OpenGL/extensions/KHR/KHR_context_flush_control.txt */
	RGFW_glProfile, /*!< set OpenGL API profile (RGFW_glCore by default) */
	RGFW_glMajor, RGFW_glMinor,  /*!< set the OpenGL API profile version (by default RGFW_glMajor is 1, RGFW_glMinor is 0) */

	RGFW_glHintsCount,
};

typedef RGFW_ENUM(i32, RGFW_glValue)  {
	RGFW_releaseFlush = 0, RGFW_glReleaseNone, /* RGFW_glReleaseBehavior options */
	RGFW_glCore = 0, RGFW_glCompatibility, RGFW_glES /*!< RGFW_glProfile options */
};

RGFWDEF void RGFW_setGLHint(RGFW_glHints hint, i32 value);
RGFWDEF i32 RGFW_getGLHint(RGFW_glHints hint);

RGFWDEF void RGFW_window_makeCurrent_OpenGL(RGFW_window* win); /*!< to be called by RGFW_window_makeCurrent */
RGFWDEF void RGFW_window_swapBuffers_OpenGL(RGFW_window* win); /*!< swap opengl buffer (only) called by RGFW_window_swapInterval  */

RGFWDEF void* RGFW_getCurrent_OpenGL(void); /*!< get the current context (OpenGL backend (3DS))*/

#endif


/** @} */

/** * @defgroup Supporting
* @{ */

#define RGFW_MAX_EVENTS 32


/*!< change which window is the root window */
RGFWDEF void RGFW_setRootWindow(RGFW_window* win);
RGFWDEF RGFW_window* RGFW_getRootWindow(void);

/*! standard event queue, used for injecting events and returning source API callback events like any other queue check */
/* these are all used internally by RGFW */
void RGFW_eventQueuePush(RGFW_event event);
RGFW_event* RGFW_eventQueuePop(RGFW_window* win);

/* for C++ / C89 */
#define RGFW_eventQueuePushEx(eventInit) { RGFW_event e; eventInit; RGFW_eventQueuePush(e); }

/*!
	key codes and mouse icon enums
*/
#undef RGFW_key
typedef RGFW_ENUM(u8, RGFW_key) {
	RGFW_keyNULL = 0,
	RGFW_keyEscape = '\033',
	RGFW_keyBacktick = '`',
	RGFW_key0 = '0',
	RGFW_key1 = '1',
	RGFW_key2 = '2',
	RGFW_key3 = '3',
	RGFW_key4 = '4',
	RGFW_key5 = '5',
	RGFW_key6 = '6',
	RGFW_key7 = '7',
	RGFW_key8 = '8',
	RGFW_key9 = '9',

	RGFW_keyMinus = '-',
	RGFW_keyEquals = '=',
	RGFW_keyBackSpace = '\b',
	RGFW_keyTab = '\t',
	RGFW_keySpace = ' ',

	RGFW_keyA = 'a',
	RGFW_keyB = 'b',
	RGFW_keyC = 'c',
	RGFW_keyD = 'd',
	RGFW_keyE = 'e',
	RGFW_keyF = 'f',
	RGFW_keyG = 'g',
	RGFW_keyH = 'h',
	RGFW_keyI = 'i',
	RGFW_keyJ = 'j',
	RGFW_keyK = 'k',
	RGFW_keyL = 'l',
	RGFW_keyM = 'm',
	RGFW_keyN = 'n',
	RGFW_keyO = 'o',
	RGFW_keyP = 'p',
	RGFW_keyQ = 'q',
	RGFW_keyR = 'r',
	RGFW_keyS = 's',
	RGFW_keyT = 't',
	RGFW_keyU = 'u',
	RGFW_keyV = 'v',
	RGFW_keyW = 'w',
	RGFW_keyX = 'x',
	RGFW_keyY = 'y',
	RGFW_keyZ = 'z',

	RGFW_keyPeriod = '.',
	RGFW_keyComma = ',',
	RGFW_keySlash = '/',
	RGFW_keyBracket = '[',
    RGFW_keyCloseBracket = ']',
    RGFW_keySemicolon = ';',
	RGFW_keyApostrophe = '\'',
	RGFW_keyBackSlash = '\\',
	RGFW_keyReturn = '\n',
	RGFW_keyEnter = RGFW_keyReturn,

	RGFW_keyDelete = '\177', /* 127 */

	RGFW_keyF1,
	RGFW_keyF2,
	RGFW_keyF3,
	RGFW_keyF4,
	RGFW_keyF5,
	RGFW_keyF6,
	RGFW_keyF7,
	RGFW_keyF8,
	RGFW_keyF9,
	RGFW_keyF10,
	RGFW_keyF11,
	RGFW_keyF12,

	RGFW_keyCapsLock,
	RGFW_keyShiftL,
	RGFW_keyControlL,
	RGFW_keyAltL,
	RGFW_keySuperL,
	RGFW_keyShiftR,
	RGFW_keyControlR,
	RGFW_keyAltR,
	RGFW_keySuperR,
	RGFW_keyUp,
	RGFW_keyDown,
	RGFW_keyLeft,
	RGFW_keyRight,
	RGFW_keyInsert,
	RGFW_keyEnd,
	RGFW_keyHome,
	RGFW_keyPageUp,
	RGFW_keyPageDown,

	RGFW_keynumLock,
	RGFW_keyKP_Slash,
	RGFW_keymultiply,
	RGFW_keyKP_Minus,
	RGFW_keyKP_1,
	RGFW_keyKP_2,
	RGFW_keyKP_3,
	RGFW_keyKP_4,
	RGFW_keyKP_5,
	RGFW_keyKP_6,
	RGFW_keyKP_7,
	RGFW_keyKP_8,
	RGFW_keyKP_9,
	RGFW_keyKP_0,
	RGFW_keyKP_Period,
	RGFW_keyKP_Return,
	RGFW_keyscrollLock,
    RGFW_keyprintScreen,
    RGFW_keypause,
    RGFW_keyLast = 256 /* padding for alignment ~(175 by default) */
 };


/*! converts api keycode to the RGFW unmapped/physical key */
RGFWDEF RGFW_button RGFW_apiKeyToRGFW(u32 keycode);
/*! converts RGFW keycode to the unmapped/physical api key */
RGFWDEF u32 RGFW_rgfwToApiKey(RGFW_button keycode);


/* TODO(EimaMei): Move these somewhere more appropriate. */
/* TODO(EimaMei): Explain 'si_system' functions. */

/* TODO(EimaMei): new function. | can return 0 or maybe not. */
RGFWDEF RGFW_systemModel RGFW_systemGetModel(void);
/* TODO(EimaMei): new function. */
RGFWDEF float RGFW_systemGet3DSlider(void);

#ifdef RGFW_OPENGL
/* TODO(EimaMei): new function */
bool RGFW_system_OpenGL_fixScreen(GLuint shader_program, const char* mat4_uniform_name);
#endif

/* Returns a video mode that's considered to be the most optimal for the system
 * by the library. Usually this returns a mode that has a standard resolution,
 * refresh rate and technique for drawing scanlines that works across all models. */
RGFWDEF RGFW_videoMode RGFW_videoModeOptimal(void);
/* Returns a video mode that's considered to be the most high-quality for the
 * system. This usually entails a mode with the highest resolution, refresh rate
 * and/or using non-interlaced scanning. Note that return of this can differ per
 * specific model. */
RGFWDEF RGFW_videoMode RGFW_videoModeHighest(void);
/* TODO(EimaMei): New function. */
RGFWDEF RGFW_area RGFW_videoModeResolution(RGFW_videoMode mode);
/* TODO(EimaMei): New function. */
RGFWDEF const char* RGFW_videoModeStr(RGFW_videoMode mode);

/* TODO(EimaMei): New function. */
RGFWDEF RGFW_videoMode RGFW_pixelFormatOptimal(void);
/* TODO(EimaMei): New function. */
RGFWDEF i32 RGFW_pixelFormatBPP(RGFW_pixelFormat mode);
/* TODO(EimaMei): New function. */
RGFWDEF const char* RGFW_pixelFormatStr(RGFW_pixelFormat mode);


/*! optional init/deinit function */
RGFWDEF i32 RGFW_init(void); /*!< is called by default when the first window is created by default */
RGFWDEF void RGFW_deinit(void); /*!< is called by default when the last open window is closed */

RGFWDEF void* RGFW_init_heap(void); /*!< inits RGFW on the heap instead of in a global var */
RGFWDEF void RGFW_deinit_heap(void); /*!< deinits the heap instance */


#if !defined(RGFW_NO_INFO) || defined(RGFW_IMPLEMENTATION)
typedef struct RGFW_info {
    RGFW_window* root;
    RGFW_window* current;
    ssize_t windowCount;
    ssize_t eventLen;
	RGFW_systemModel model;

    RGFW_event events[RGFW_MAX_EVENTS];

	/* TODO(EimaMei): Decide what to do with keycodes. */
	#if RGFW_HAS_KEYBOARD_SUPPORT
    u32 apiKeycodes[RGFW_keyLast];
	/* u8 keycodes[RGFW_OS_BASED_VALUE(256, 512, 128, 256)]; */
	#endif

	ssize_t controller_count; /*!< the actual amount of controllers */
	RGFW_controller controllers[RGFW_MAX_CONTROLLER_DEVICES];

	RGFW_bool stopCheckEvents_bool;

	#ifdef RGFW_3DS
	#ifdef RGFW_OPENGL
	RGFW_bool kygx_initialized;
	#endif
	#endif

} RGFW_info;

RGFWDEF i32 RGFW_init_ptr(RGFW_info* info); /*!< init RGFW, storing the data at the pointer */
RGFWDEF void RGFW_deinit_ptr(RGFW_info* info); /*!< deinits RGFW instance at pointer */

RGFWDEF void RGFW_setInfo(RGFW_info* info);
RGFWDEF RGFW_info* RGFW_getInfo(void);
#endif

#ifdef RGFW_IMPLEMENTATION
RGFW_info* _RGFW = NULL;
void RGFW_setInfo(RGFW_info* info) { _RGFW = info; }
RGFW_info* RGFW_getInfo(void) { return _RGFW; }


/*
RGFW_IMPLEMENTATION starts with generic RGFW defines

This is the start of keycode data
*/



/*
	the c++ compiler doesn't support setting up an array like,
	we'll have to do it during runtime using a function & this messy setup
*/


#if RGFW_HAS_KEYBOARD_SUPPORT
RGFW_keyState RGFW_keyboard[RGFW_keyLast];
#endif

/* var - VARIABLE | mask - UINT | set - bool
 * Sets/unsets the mask for the variable. */
#define RGFW_MASK_SET(var, mask, set) do { \
	if (set) (var) |=  (mask); \
	else     (var) &= ~(mask); \
} while (0)

#define RGFW_COUNTOF(array) (ssize_t)(sizeof(array) / sizeof((array)[0]))

RGFWDEF void RGFW_resetKeyPrev(void);


void RGFW_resetKeyPrev(void) {
	/* TODO(EimaMei): This feels inneficient to do each frame. Must be resolved. */
	ssize_t i, j;
	for (i = 0; i < RGFW_MAX_CONTROLLER_DEVICES; i += 1) {
		RGFW_controller* controller = &_RGFW->controllers[i];
		if (!controller->connected) {
			continue;
		}

		for (j = controller->button_start; j < controller->button_end; j += 1) {
			RGFW_MASK_SET(_RGFW->controllers[i].buttons[j], RGFW_buttonStatePrevious, false);
		}
	}
}

/*
	this is the end of keycode data
*/

/*
	event callback defines start here
*/
#define RGFW_CALLBACK_DEFINE(x, x2) \
RGFW_##x##func RGFW_##x##CallbackSrc = NULL; \
RGFW_##x##func RGFW_set##x2##Callback(RGFW_##x##func func) { \
    RGFW_##x##func prev = RGFW_##x##CallbackSrc; \
    RGFW_##x##CallbackSrc = func; \
    return prev; \
}

RGFW_CALLBACK_DEFINE(windowResized, WindowResized)
#define RGFW_windowResizedCallback(w, r) if (RGFW_windowResizedCallbackSrc) RGFW_windowResizedCallbackSrc(w, r)

RGFW_CALLBACK_DEFINE(windowQuit, WindowQuit)
#define RGFW_windowQuitCallback(w) if (RGFW_windowQuitCallbackSrc) RGFW_windowQuitCallbackSrc(w)

RGFW_CALLBACK_DEFINE(mouseNotify, MouseNotify)
#define RGFW_mouseNotifyCallback(w, p, status) if (RGFW_mouseNotifyCallbackSrc) RGFW_mouseNotifyCallbackSrc(w, p, status)

RGFW_CALLBACK_DEFINE(button, Button)
#define RGFW_buttonCallback(w, controller, button, press) if (RGFW_buttonCallbackSrc) RGFW_buttonCallbackSrc(w, controller, button, press)

RGFW_CALLBACK_DEFINE(controllerAxis, ControllerAxis)
#define RGFW_controllerAxisCallback(w, controller, axis) if (RGFW_controllerAxisCallbackSrc) RGFW_controllerAxisCallbackSrc(w, controller, axis)

RGFW_CALLBACK_DEFINE(pointerMove, PointerMove)
#define RGFW_pointerMoveCallback(w, controller, pointer) if (RGFW_pointerMoveCallbackSrc) RGFW_pointerMoveCallbackSrc(w, controller, pointer)

RGFW_CALLBACK_DEFINE(motionMove, MotionMove)
#define RGFW_motionMoveCallback(w, controller, motion) if (RGFW_pointerMoveCallbackSrc) RGFW_motionMoveCallbackSrc(w, controller, motion)

RGFW_CALLBACK_DEFINE(controller, Controller)
#define RGFW_controllerCallback(w, controller, connected) if (RGFW_controllerCallbackSrc) RGFW_controllerCallbackSrc(w, controller, connected)

RGFW_CALLBACK_DEFINE(debug, Debug)
#define RGFW_debugCallback(type, err, ctx, msg) if (RGFW_debugCallbackSrc) RGFW_debugCallbackSrc(type, err, ctx, msg)
#undef RGFW_CALLBACK_DEFINE

#ifdef RGFW_DEBUG
#include <stdio.h>
#endif

void RGFW_sendDebugInfo(RGFW_debugType type, RGFW_errorCode err, RGFW_debugContext ctx, const char* msg) {
	#ifdef RGFW_OPENGL
	if (RGFW_getGLHint(RGFW_glNoError)) { return ;}
	#endif
	RGFW_debugCallback(type, err, ctx, msg);

    #ifdef RGFW_DEBUG
	switch (type) {
		case RGFW_typeInfo: RGFW_PRINTF("RGFW INFO (%i %i): %s", type, err, msg); break;
		case RGFW_typeError: RGFW_PRINTF("RGFW DEBUG (%i %i): %s", type, err, msg); break;
		case RGFW_typeWarning: RGFW_PRINTF("RGFW WARNING (%i %i): %s", type, err, msg); break;
		default: break;
	}

	switch (err) {
		#ifdef RGFW_BUFFER
		case RGFW_errBuffer: case RGFW_infoBuffer: RGFW_PRINTF(" videoMode = %s, pixelFormat = %s\n", RGFW_videoModeStr(ctx.win->mode), RGFW_pixelFormatStr(ctx.win->format)); break;
		#endif
		case RGFW_infoWindow: RGFW_PRINTF(" videoMode = %s, pixelFormat = %s\n", RGFW_videoModeStr(ctx.win->mode), RGFW_pixelFormatStr(ctx.win->format)); break;
		default: RGFW_PRINTF("\n");
	}
	#endif
}

void RGFW_window_checkEvents(RGFW_window* win, i32 waitMS) {
	RGFW_window_eventWait(win, waitMS);

	while (RGFW_window_checkEvent(win) != NULL && RGFW_window_shouldClose(win) == 0) {
		if (win->event->type == RGFW_quit) return;
	}
}


/*
no more event call back defines
*/

#define SET_ATTRIB(a, v) { \
    RGFW_ASSERT(((size_t) index + 1) < sizeof(attribs) / sizeof(attribs[0])); \
    attribs[index++] = a; \
    attribs[index++] = v; \
}

/* RGFW_BIT(24) */
#define RGFW_EVENT_QUIT 		RGFW_BIT(25) /* the window close button was pressed */
#define RGFW_MOUSE_LEFT 		RGFW_BIT(27) /* if mouse left the window */
#define RGFW_WINDOW_ALLOC 		RGFW_BIT(28) /* if window was allocated by RGFW */
#define RGFW_BUFFER_ALLOC 		RGFW_BIT(29) /* if window.buffer was allocated by RGFW */
#define RGFW_WINDOW_INIT 		RGFW_BIT(30) /* if window.buffer was allocated by RGFW */
#define RGFW_INTERNAL_FLAGS (RGFW_EVENT_QUIT | RGFW_MOUSE_LEFT | RGFW_WINDOW_ALLOC | RGFW_BUFFER_ALLOC)


RGFW_window* RGFW_createWindow(RGFW_videoMode mode, RGFW_windowFlags flags) {
	RGFW_window* win = (RGFW_window*)RGFW_ALLOC(sizeof(RGFW_window));
	RGFW_ASSERT(win != NULL);
    win->_flags = RGFW_WINDOW_ALLOC;
    return RGFW_createWindowPtr(mode, flags, win);
}

#ifndef RGFW_FORCE_INIT
RGFW_info _rgfwGlobal;
#endif

i32 RGFW_init(void) { return RGFW_init_ptr(&_rgfwGlobal); }
void RGFW_deinit(void) { RGFW_deinit_ptr(&_rgfwGlobal); }

void* RGFW_init_heap(void) {
    RGFW_info* info = (RGFW_info*)RGFW_ALLOC(sizeof(RGFW_info));
    RGFW_init_ptr(info);
    return (void*)info;
}

void RGFW_deinit_heap(void) {
    RGFW_deinit_ptr(_RGFW);
    RGFW_FREE(_RGFW);
    _RGFW = NULL;
}

i32 RGFW_initPlatform(void);
void RGFW_deinitPlatform(void);

i32 RGFW_init_ptr(RGFW_info* info) {
    if (info == _RGFW || info == NULL) return 1;

    RGFW_setInfo(info);

    _RGFW->root = NULL;
    _RGFW->current = NULL;
    _RGFW->windowCount = -1;
    _RGFW->eventLen = 0;
    _RGFW->windowCount = 0;

    RGFW_MEMSET(_RGFW, 0, sizeof(RGFW_info));

#if RGFW_HAS_KEYBOARD_SUPPORT
    RGFW_init_keys();
#endif
    i32 out = RGFW_initPlatform();
    RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoGlobal, RGFW_DEBUG_CTX(NULL, 0), "global context initialized");
    return out;
}

void RGFW_deinit_ptr(RGFW_info* info) {
    if (info == NULL) return;

    RGFW_setInfo(info);
    RGFW_deinitPlatform();

    _RGFW->root = NULL;
    _RGFW->windowCount = 0;
    RGFW_setInfo(NULL);
	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoGlobal, RGFW_DEBUG_CTX(NULL, 0), "global context deinitialized");
}


void RGFW_eventQueuePush(RGFW_event event) {
	RGFW_ASSERT(_RGFW->eventLen >= 0);

	if (_RGFW->eventLen >= RGFW_MAX_EVENTS) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errEventQueue, RGFW_DEBUG_CTX(NULL, 0), "Event queue limit 'RGFW_MAX_EVENTS' has been reached.");
		return;
	}

	_RGFW->eventLen += 1;
	_RGFW->events[RGFW_MAX_EVENTS - _RGFW->eventLen] = event;
}

RGFW_event* RGFW_eventQueuePop(RGFW_window* win) {
	RGFW_ASSERT(_RGFW->eventLen >= 0 && _RGFW->eventLen <= RGFW_MAX_EVENTS);

    if (_RGFW->eventLen == 0) {
		return NULL;
	}

	RGFW_event* ev = &_RGFW->events[RGFW_MAX_EVENTS - _RGFW->eventLen];
	_RGFW->eventLen -= 1;

	if (ev->_win != win && ev->_win != NULL) {
        RGFW_eventQueuePush(*ev);
        return NULL;
	}

	return ev;
}

RGFW_event* RGFW_window_checkEventCore(RGFW_window* win);
RGFW_event* RGFW_window_checkEventCore(RGFW_window* win) {
	RGFW_event* ev;
    RGFW_ASSERT(win != NULL);

    if (win->event->type == 0 && _RGFW->eventLen == 0) {
        RGFW_resetKeyPrev();
    }

	/* check queued events */
	ev = RGFW_eventQueuePop(win);
	if (ev != NULL) {
		if (ev->type == RGFW_quit) {
			RGFW_window_setShouldClose(win, RGFW_TRUE);
			if (win->_flags & RGFW_windowFreeOnClose) {
				RGFW_window_close(win);
			}
		}
		win->event = ev;
    }
	else return NULL;

	return win->event;
}


RGFWDEF void RGFW_window_basic_init(RGFW_window* win, RGFW_videoMode mode, RGFW_windowFlags flags);
void RGFW_setRootWindow(RGFW_window* win) { _RGFW->root = win; }
RGFW_window* RGFW_getRootWindow(void) { return _RGFW->root; }

/* do a basic initialization for RGFW_window, this is to standard it for each OS */
void RGFW_window_basic_init(RGFW_window* win, RGFW_videoMode mode, RGFW_windowFlags flags) {
	RGFW_UNUSED(flags);
    if (_RGFW == NULL) RGFW_init();
    _RGFW->windowCount++;

    /* rect based the requested flags */
	if (_RGFW->root == NULL) {
		RGFW_setRootWindow(win);
	}

	if (!(win->_flags & RGFW_WINDOW_ALLOC)) win->_flags = 0;

	/* set and init the new window's data */
	win->mode = mode;
	win->format = -1;
	/* TODO(EimaMei): Add an input exit button? Should this be kept for keyboard
	 * supported system? */
#if RGFW_HAS_KEYBOARD_SUPPORT
	win->exitKey = RGFW_escape;
#endif

	win->_flags = (win->_flags & RGFW_WINDOW_ALLOC) | flags;
	win->event = NULL;

	if ((flags & RGFW_windowNoInitAPI) == 0) {
		#ifdef RGFW_OPENGL
		RGFW_window_initOpenGL(win);
		#endif
        RGFW_window_initBuffer(win);
    }

	RGFW_window_setFlags(win, win->_flags);
}

void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags flags) {
	if (flags & RGFW_windowConsoleInit) RGFW_window_consoleInit(win);

	win->_flags = flags | (win->_flags & RGFW_INTERNAL_FLAGS);
}

void RGFW_window_initBuffer(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	RGFW_window_initBufferSize(win, win->mode, RGFW_pixelFormatRGBA8);
}

void RGFW_window_initBufferSize(RGFW_window* win, RGFW_videoMode mode, RGFW_pixelFormat format) {
	RGFW_area res = RGFW_videoModeResolution(mode);

	u8* buffer = (u8*)RGFW_ALLOC_SYS(res.w * res.h * RGFW_pixelFormatBPP(format));
	if (buffer == NULL) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errOutOfMemory, RGFW_DEBUG_CTX(NULL, 0), "Ran out of memory when allocating framebuffers.");
		return ;
	}

	win->_flags |= RGFW_BUFFER_ALLOC;
	RGFW_window_initBufferPtr(win, buffer, mode, format);
}

RGFW_bool RGFW_isPressed(RGFW_controller* controller, RGFW_button button) {
	RGFW_ASSERT(controller != NULL);
	RGFW_ASSERT(button >= controller->button_start && button < controller->button_end);
	return (controller->buttons[button] & RGFW_buttonStateCurrent) != 0;
}

RGFW_bool RGFW_wasPressed(RGFW_controller* controller, RGFW_button button) {
	RGFW_ASSERT(controller != NULL);
	RGFW_ASSERT(button >= controller->button_start && button < controller->button_end);
	return (controller->buttons[button] & RGFW_buttonStatePrevious) != 0;
}

RGFW_bool RGFW_isHeld(RGFW_controller* controller, RGFW_button button) {
	return RGFW_isPressed(controller, button) && RGFW_wasPressed(controller, button);
}

RGFW_bool RGFW_isReleased(RGFW_controller* controller, RGFW_button button) {
	return !RGFW_isPressed(controller, button) && RGFW_wasPressed(controller, button);
}

ssize_t RGFW_controllerGetCount(void) {
	return _RGFW->controller_count;
}



void RGFW_window_makeCurrent(RGFW_window* win) {
    _RGFW->current = win;
#if defined(RGFW_OPENGL) || defined(RGFW_EGL)
    RGFW_window_makeCurrent_OpenGL(win);
#endif
}

RGFW_window* RGFW_getCurrent(void) {
    return _RGFW->current;
}

void RGFW_window_swapBuffers(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	RGFW_window_swapBuffers_software(win);
#if defined(RGFW_OPENGL) || defined(RGFW_EGL)
	RGFW_window_swapBuffers_OpenGL(win);
#endif
}

RGFW_bool RGFW_window_shouldClose(RGFW_window* win) {
	/* TODO(EimaMei): Add an exit key. */
	return (win == NULL || (win->_flags & RGFW_EVENT_QUIT)
		#if RGFW_HAS_KEYBOARD_SUPPORT
		/* TODO(EimaMei): I am not sure if this should even be here. Fix it later
		 * on anyway. */
		|| (win->exitKey && RGFW_isPressed(win, win->exitKey))
		#endif
		);
}

void RGFW_window_setShouldClose(RGFW_window* win, RGFW_bool shouldClose) {
	if (shouldClose)  {
		win->_flags |= RGFW_EVENT_QUIT;
		RGFW_windowQuitCallback(win);
	} else {
		win->_flags &= ~(u32)RGFW_EVENT_QUIT;
	}
}

/* TODO(EimaMei): Fix this. */
#if RGFW_HAS_KEYBOARD_SUPPORT

RGFWDEF void RGFW_updateKeyMod(RGFW_window* win, RGFW_keymod mod, RGFW_bool value);
void RGFW_updateKeyMod(RGFW_window* win, RGFW_keymod mod, RGFW_bool value) {
	if (value) win->event.keyMod |= mod;
	else win->event.keyMod &= ~mod;
}

RGFWDEF void RGFW_updateKeyModsPro(RGFW_window* win, RGFW_bool capital, RGFW_bool numlock, RGFW_bool control, RGFW_bool alt, RGFW_bool shift, RGFW_bool super, RGFW_bool scroll);
void RGFW_updateKeyModsPro(RGFW_window* win, RGFW_bool capital, RGFW_bool numlock, RGFW_bool control, RGFW_bool alt, RGFW_bool shift, RGFW_bool super, RGFW_bool scroll) {
	RGFW_updateKeyMod(win, RGFW_modCapsLock, capital);
	RGFW_updateKeyMod(win, RGFW_modNumLock, numlock);
	RGFW_updateKeyMod(win, RGFW_modControl, control);
	RGFW_updateKeyMod(win, RGFW_modAlt, alt);
	RGFW_updateKeyMod(win, RGFW_modShift, shift);
	RGFW_updateKeyMod(win, RGFW_modSuper, super);
	RGFW_updateKeyMod(win, RGFW_modScrollLock, scroll);
}

RGFWDEF void RGFW_updateKeyMods(RGFW_window* win, RGFW_bool capital, RGFW_bool numlock, RGFW_bool scroll);
void RGFW_updateKeyMods(RGFW_window* win, RGFW_bool capital, RGFW_bool numlock, RGFW_bool scroll) {
	RGFW_updateKeyModsPro(win, capital, numlock,
					RGFW_isPressed(win, RGFW_controlL) || RGFW_isPressed(win, RGFW_controlR),
					RGFW_isPressed(win, RGFW_altL) || RGFW_isPressed(win, RGFW_altR),
					RGFW_isPressed(win, RGFW_shiftL) || RGFW_isPressed(win, RGFW_shiftR),
					RGFW_isPressed(win, RGFW_superL) || RGFW_isPressed(win, RGFW_superR),
					scroll);
}
#endif

/*
	graphics API specific code (end of generic code)
	starts here
*/


/*
	OpenGL defines start here   (Normal, EGL, OSMesa)
*/

#if defined(RGFW_OPENGL) || defined(RGFW_EGL)

i32 RGFW_GL_HINTS[RGFW_glHintsCount] = {
	/* RGFW_glStencil         */ 8,
	/* RGFW_glSamples         */ 4,
	/* RGFW_glStereo          */ RGFW_FALSE,
	/* RGFW_glAuxBuffers      */ 0,
	/* RGFW_glDoubleBuffer    */ RGFW_TRUE,
	/* RGFW_glRed             */ 8,
	/* RGFW_glGreen           */ 8,
	/* RGFW_glBlue            */ 8,
	/* RGFW_glAlpha           */ 8,
	/* RGFW_glDepth           */ 24,
	/* RGFW_glAccumRed        */ 0,
	/* RGFW_glAccumGreen      */ 0,
	/* RGFW_glAccumBlue       */ 0,
	/* RGFW_glAccumAlpha      */ 0,
	/* RGFW_glSRGB            */ RGFW_FALSE,
	/* RGFW_glRobustness      */ RGFW_FALSE,
	/* RGFW_glDebug           */ RGFW_FALSE,
	/* RGFW_glNoError         */ RGFW_FALSE,
	/* RGFW_glReleaseBehavior */ RGFW_glReleaseNone,
	/* RGFW_glProfile         */ RGFW_glCore,
	/* RGFW_glMajor           */ 1,
	/* RGFW_glMinor           */ 0,
};

void RGFW_setGLHint(RGFW_glHints hint, i32 value) {
	RGFW_ASSERT(hint >= 0 && hint < RGFW_glHintsCount);
	RGFW_GL_HINTS[hint] = value;
}

i32 RGFW_getGLHint(RGFW_glHints hint) {
	RGFW_ASSERT(hint >= 0 && hint < RGFW_glHintsCount);
	return RGFW_GL_HINTS[hint];
}

#endif

/*
This is where OS specific stuff starts
*/



/*


Start of 3DS


*/

#ifdef RGFW_3DS

#define RGFW_ACCEPTED_CTRU_INPUTS \
	(KEY_A | KEY_B | KEY_SELECT | KEY_START | KEY_DRIGHT | KEY_DLEFT | KEY_DUP | KEY_DDOWN | \
	KEY_R | KEY_L | KEY_X | KEY_Y | KEY_ZL | KEY_ZR | \
	KEY_CSTICK_RIGHT | KEY_CSTICK_LEFT | KEY_CSTICK_UP | KEY_CSTICK_DOWN)

#define RGFW_CPAD_BITS_H (KEY_CPAD_LEFT | KEY_CPAD_RIGHT)
#define RGFW_CPAD_BITS_V (KEY_CPAD_UP   | KEY_CPAD_DOWN )

#ifndef RGFW_IMPL_COUNT_TRAILING_ZEROES

i32 RGFW_countTrailingZeros(u32 x);
i32 RGFW_countTrailingZeros(u32 x) {
	i32 count = 0;
	while ((x & 1) == 0) {
		count += 1;
		x >>= 1;
	}
	return count;
}
#endif


const char* RGFW_controllerName(RGFW_controllerType type) {
	RGFW_ASSERT(type >= 0 && type < RGFW_controllerTypeCount);

	static const char* NAME_LUT[RGFW_controllerTypeCount] = {
		"Standard Nintendo 3DS Controller Layout"
	};
	return NAME_LUT[type];
}


RGFW_controller* RGFW_controllerGet(ssize_t port) {
	RGFW_ASSERT(port >= 0 && port < RGFW_MAX_CONTROLLER_DEVICES);
	return &_RGFW->controllers[port];
}


void RGFW_controllerPointerEnable(RGFW_controller* controller,
		RGFW_pointerType pointer, RGFW_bool enable) {
	RGFW_ASSERT(controller != NULL);
	RGFW_ASSERT(pointer >= 0 && pointer < RGFW_pointerTypeCount);

	controller->enabled_pointers[pointer] = enable;
}

void RGFW_controllerMotionEnable(RGFW_controller* controller, RGFW_motionType motion,
		RGFW_bool enable) {
	RGFW_ASSERT(controller != NULL);
	RGFW_ASSERT(motion >= 0 && motion < RGFW_motionTypeCount);

	if (controller->enabled_motions[motion] == enable) {
		return ;
	}

	switch (motion) {
		case RGFW_motionAccelerometer: {
			if (enable) HIDUSER_EnableAccelerometer();
			else        HIDUSER_DisableAccelerometer();
		} break;

		case RGFW_motionGyroscope: {
			if (enable) HIDUSER_EnableGyroscope();
			else        HIDUSER_DisableGyroscope();
		} break;
	}

	controller->enabled_motions[motion] = enable;
}

const char* RGFW_buttonName(RGFW_button button) {
	RGFW_ASSERT(button >= 0 && button < RGFW_controllerButtonCount);

	static const char* NAME_LUT[RGFW_controllerButtonCount] = {
		"A",
		"B",
		"Select",
		"Start",
		"D-Pad Right",
		"D-Pad Left",
		"D-Pad Up",
		"D-PAD Down",
		"R",
		"L",
		"X",
		"Y",
		"ZL",
		"ZR",
		"C-Stick Right",
		"C-Stick Left",
		"C-Stick Up",
		"C-Stick Down"
	};
	return NAME_LUT[button];
}

const char* RGFW_axisName(RGFW_axisType axis) {
	RGFW_ASSERT(axis >= 0 && axis < RGFW_axisTypeCount);

	static const char* NAME_LUT[RGFW_axisTypeCount] = {
		"Circle-Pad X Axis", "Circle-Pad Y Axis",
		//"Circle-Pad Pro X"
	};
	return NAME_LUT[axis];
}


const char* RGFW_pointerName(RGFW_pointerType type) {
	RGFW_ASSERT(type >= 0 && type < RGFW_pointerTypeCount);

	static const char* NAME_LUT[RGFW_pointerTypeCount] = {
		"Touchscreen"
	};
	return NAME_LUT[type];
}

const char* RGFW_motionName(RGFW_motionType type) {
	RGFW_ASSERT(type >= 0 && type < RGFW_motionTypeCount);

	static const char* NAME_LUT[RGFW_motionTypeCount] = {
		"Accelerometer",
		"Gyroscope"
	};
	return NAME_LUT[type];
}


RGFW_button RGFW_apiKeyToRGFW(u32 keycode) {
	/* TODO(EimaMei): Figure out if this functions should work with keyboards
	 * or not. */
	RGFW_ASSERT(RGFW_HAS_KEYBOARD_SUPPORT == 0);

	keycode &= RGFW_ACCEPTED_CTRU_INPUTS;
	if (keycode == 0) { return RGFW_buttonInvalid; }

	RGFW_button button = RGFW_countTrailingZeros(keycode);
	switch (button) {
		case 14: return RGFW_ZL;
		case 15: return RGFW_ZR;
	}
	if (button >= 24) { button -= 10; }

	RGFW_ASSERT(button >= 0 && button < RGFW_controllerButtonCount);

	return button;
}

u32 RGFW_rgfwToApiKey(RGFW_button button) {
	/* TODO(EimaMei): Figure out if this functions should work with keyboards
	 * or not. */
	RGFW_ASSERT(RGFW_HAS_KEYBOARD_SUPPORT == 0);
	RGFW_ASSERT(button >= 0 && button < RGFW_controllerButtonCount);

	static const u32 CTRU_RGFW_KEY_LUT[RGFW_controllerButtonCount] = {
		KEY_A,
		KEY_B,
		KEY_SELECT,
		KEY_START,
		KEY_DRIGHT,
		KEY_DLEFT,
		KEY_DUP,
		KEY_DDOWN,
		KEY_R,
		KEY_L,
		KEY_X,
		KEY_Y,
		KEY_ZL,
		KEY_ZR,
		KEY_CSTICK_RIGHT,
		KEY_CSTICK_LEFT,
		KEY_CSTICK_UP,
		KEY_CSTICK_DOWN
	};

	return CTRU_RGFW_KEY_LUT[button];
}


#ifdef RGFW_BUFFER
void _RGFW__gfxPresentFramebuffer(RGFW_window* win, bool has_stereo);
void _RGFW__gfxPresentFramebuffer(RGFW_window* win, bool has_stereo) {
	u32 stride = GSP_SCREEN_WIDTH * (u32)RGFW_pixelFormatBPP(win->format);
	u32 pixel_format = (u32)win->format | (1 << 8);

#ifndef RGFW_BUFFER_NATIVE
	u8* fb_a = win->src.buffers_native[win->src.current_buffer];
#else
	u8* fb_a = win->buffer;
#endif
	u8* fb_b = fb_a;

	switch (win->mode) {
		case RGFW_videoMode2D: {
			pixel_format |= BIT(6);
		} break;

		case RGFW_videoMode3D: {
			pixel_format |= BIT(5);
			if (has_stereo) fb_b += win->src.buffer_size / 2;
		} break;
	}

	gspPresentBuffer(
		win->mode == RGFW_videoModeBottomScreen, (u32)win->src.current_buffer,
		fb_a, fb_b, stride, pixel_format
	);
}

#ifndef RGFW_BUFFER_NATIVE
/* TODO(EimaMei): Add support for other 3 formats. */
void RGFW_window_bufferToNative(RGFW_window* win);
void RGFW_window_bufferToNative(RGFW_window* win) {
	u8* dst = win->src.buffers_native[win->src.current_buffer];
	u8* src = win->buffer;

	const ssize_t width  = win->bufferSize.w,
				  height = 240;
	for (ssize_t i = 0; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= 0; j -= 1) {
			ssize_t pixel = 4 * (i * height + j);

			ssize_t opixel = 4 * ((height - 1 - j) * width + i);
			dst[pixel + 0] = src[opixel + 3];
			dst[pixel + 1] = src[opixel + 2];
			dst[pixel + 2] = src[opixel + 1];
			dst[pixel + 3] = src[opixel + 0];
		}
	}
}
#endif

#endif

void RGFW_window_initBufferPtr(RGFW_window* win, u8* buffer, RGFW_videoMode mode,
		RGFW_pixelFormat format) {
#if defined(RGFW_BUFFER)
	win->buffer = buffer;
	win->bufferSize = RGFW_videoModeResolution(mode);
	win->mode = mode;
	win->format = format;

	win->src.current_buffer = 0;
	win->src.buffer_size = win->bufferSize.w * win->bufferSize.h * RGFW_pixelFormatBPP(format);
	#ifdef RGFW_BUFFER_NATIVE
	win->src.buffers[0] = buffer;
	win->src.buffers[1] = RGFW_ALLOC_SYS(win->src.buffer_size);
	#else
	win->src.buffers[0] = buffer;
	win->src.buffers[1] = RGFW_ALLOC((size_t)win->src.buffer_size);

	win->src.buffers_native[0] = RGFW_ALLOC_SYS(win->src.buffer_size);
	win->src.buffers_native[1] = RGFW_ALLOC_SYS(win->src.buffer_size);
	#endif

	_RGFW__gfxPresentFramebuffer(win, false);
	gspWaitForVBlank();
	GSPGPU_SetLcdForceBlack(RGFW_FALSE);

	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoBuffer, RGFW_DEBUG_CTX(win, 0), "Creating a 4 channel buffer");

#else
	RGFW_UNUSED(win); RGFW_UNUSED(buffer); RGFW_UNUSED(mode); RGFW_UNUSED(format);
#endif
}

#ifdef RGFW_OPENGL

bool RGFW_window_initOpenGL(RGFW_window* win) {
	if (RGFW_getGLHint(RGFW_glProfile) != RGFW_glES) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS only supports GLES.");
		return false;
	}

	if (RGFW_getGLHint(RGFW_glMajor) != 1 || (RGFW_getGLHint(RGFW_glMinor) != 0 && RGFW_getGLHint(RGFW_glMinor) != 1)) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS can only support up to GLES 1.1.");
		return false;
	}

	if (RGFW_getGLHint(RGFW_glStencil) != 8) {
		RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS only supports an 8-bit stencil buffer. Defaulting to 8 bits.");
		RGFW_setGLHint(RGFW_glStencil, 8);
	}

	if (RGFW_getGLHint(RGFW_glDepth) != 24) {
		RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS only supports a 24-bit depth buffer. Defaulting to 24 bits.");
		RGFW_setGLHint(RGFW_glStencil, 24);
	}

	/* TODO(EimaMei): Initialize kygx in the global context as opengl. */
	if (!_RGFW->kygx_initialized) {
		_RGFW->kygx_initialized = kygxInit();
		if (!_RGFW->kygx_initialized) {
			RGFW_sendDebugInfo(RGFW_typeError, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Failed to initialize KYGX.");
			return false;
		}
	}

	GLASSCtxParams params;
    glassGetDefaultContextParams(&params, GLASS_VERSION_2_0);
	params.targetScreen = (win->mode == RGFW_videoModeBottomScreen) ? GLASS_SCREEN_BOTTOM : GLASS_SCREEN_TOP;

	win->src.ctx = glassCreateContext(&params);
	if (win->src.ctx == NULL) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Failed to create a GLASS context.");
		return false;
	}
	glassBindContext(win->src.ctx);


	i32 r = RGFW_getGLHint(RGFW_glRed),
		g = RGFW_getGLHint(RGFW_glGreen),
		b = RGFW_getGLHint(RGFW_glBlue),
		a = RGFW_getGLHint(RGFW_glAlpha);

	GLenum internal_format;
	if (r == 8 && g == 8 && b == 8) {
		if (a == 8) {
			internal_format = GL_RGBA8_OES;
			win->format = RGFW_pixelFormatRGBA8;
		}
		else if (a == 0) {
			internal_format = GL_RGB8_OES;
			win->format = RGFW_pixelFormatBGR8;
		}
		else {
			RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Invalid alpha channel size. Defaulting to the RGBA8 format.");
			RGFW_setGLHint(RGFW_glAlpha, 8);
			internal_format = GL_RGBA8_OES;
			win->format = RGFW_pixelFormatRGBA8;
		}
	}
	else if (r == 4 && g == 4 && b == 4 && a == 4) {
		internal_format = GL_RGBA4;
		win->format = RGFW_pixelFormatRGBA4;
	}
	else if (r == 5 && g == 5 && b == 5 && a == 1) {
		internal_format = GL_RGB5_A1;
		win->format = RGFW_pixelFormatRGB5_A1;
	}
	else {
		RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Invalid R, B, G and A channel sizes. Defaulting to the RGBA8 format.");
		for (ssize_t i = 0; i < 4; i += 1) {
			RGFW_setGLHint(RGFW_glRed + i, 8);
		}

		internal_format = GL_RGBA8_OES;
		win->format = RGFW_pixelFormatRGBA8;
	}

	if (RGFW_getGLHint(RGFW_glStereo)) {
		if (win->mode == RGFW_videoModeBottomScreen) {
			RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Cannot enable 3D for the bottom screen.");
		}
		else {
			win->mode = RGFW_videoMode3D;
		}
	}

	GLint width = RGFW_videoModeResolution(win->mode).w;
	glGenRenderbuffers(1, &win->src.renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, win->src.renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, 240);

	glGenFramebuffers(1, &win->src.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, win->src.framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, win->src.renderbuffer);

	glViewport(0, 0, width, 240);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

void RGFW_window_freeOpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	if (win->src.ctx == NULL) { return ; }

	glDeleteRenderbuffers(1, &win->src.renderbuffer);
	glDeleteFramebuffers(1, &win->src.framebuffer);

	glassDestroyContext(win->src.ctx);
	win->src.ctx = NULL;
}

#endif

RGFW_systemModel RGFW_systemGetModel(void) {
	RGFW_init();
	return _RGFW->model;
}

float RGFW_systemGet3DSlider(void) {
	return osGet3DSliderState();
}

RGFW_videoMode RGFW_videoModeOptimal(void) {
	return RGFW_videoMode2D;
}

RGFW_videoMode RGFW_videoModeHighest(void) {
	return (RGFW_systemGetModel() != RGFW_systemModel2DS) ? RGFW_videoModeWide : RGFW_videoMode2D;
}

RGFW_area RGFW_videoModeResolution(RGFW_videoMode mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_videoModeCount);

	static const i32 WIDTH_LUT[RGFW_videoModeCount] = {400, 800, 800, 320};
	return RGFW_AREA(WIDTH_LUT[mode], 240);
}

const char* RGFW_videoModeStr(RGFW_videoMode mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_videoModeCount);

	static const char* NAMES_LUT[RGFW_videoModeCount] = {
		"RGFW_videoMode2D",
		"RGFW_videoMode3D",
		"RGFW_videoModeWide",
		"RGFW_videoModeBottomScreen",
	};
	return NAMES_LUT[mode];
}


RGFW_videoMode RGFW_pixelFormatOptimal(void) {
	return RGFW_pixelFormatBGR8;
}

i32 RGFW_pixelFormatBPP(RGFW_pixelFormat mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_pixelFormatCount);

	static const i32 BPP_LUT[RGFW_pixelFormatCount] = {4, 3, 2, 2, 2};
	return BPP_LUT[mode];
}

const char* RGFW_pixelFormatStr(RGFW_pixelFormat mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_pixelFormatCount);

#ifdef RGFW_3DS
	static const char* NAMES_LUT[RGFW_pixelFormatCount] = {
		"RGFW_pixelFormatRGBA8",
		"RGFW_pixelFormatBGR8",
		"RGFW_pixelFormatRGB565",
		"RGFW_pixelFormatRGB5_A1",
		"RGFW_pixelFormatRGBA4",
	};
	return NAMES_LUT[mode];
#endif
}


i32 RGFW_initPlatform(void) {
	u8 model;
	Result res = CFGU_GetSystemModel(&model);
	_RGFW->model = (res == 0) ? model : 0;

	_RGFW->controller_count = 1;

	RGFW_controller* controller = &_RGFW->controllers[0];
	controller->connected = true;
	controller->enabled_pointers[RGFW_pointerTouchscreen] = true;
	controller->button_start = 0;
	controller->button_end = RGFW_controllerButtonCount - 1;

	gfxInitDefault();

    return 0;
}

void RGFW_deinitPlatform(void) {
	gfxExit();

	#ifdef RGFW_OPENGL
	if (_RGFW->kygx_initialized) {
		kygxExit();
		_RGFW->kygx_initialized = false;
	}
	#endif
}

RGFW_window* RGFW_createWindowPtr(RGFW_videoMode mode, RGFW_windowFlags flags,
		RGFW_window* win) {
	RGFW_window_basic_init(win, mode, flags);
	return win;
}

void RGFW_window_consoleInit(RGFW_window* win) {
	RGFW_init();
	consoleInit(win->mode != RGFW_videoModeBottomScreen ? GFX_TOP : GFX_BOTTOM, NULL);
}

u32 hidCheckSectionUpdateTime(vu32 *sharedmem_section, u32 id);

void RGFW_window_eventWait(RGFW_window* win, i32 waitMS) {
	/* TODO(EimaMei): This doesn't actually sleep? Also it's unclear if setting
	 * nanosecond to -1 actually means that hid is going to wait for any event
	 * forever. */
	hidWaitForAnyEvent(RGFW_TRUE, 0, (waitMS == -1) ? -1 : waitMS * 1000000);
}

RGFW_event* RGFW_window_checkEvent(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	RGFW_event* ev;

	/* TODO(EimaMei): Add a better "checkEvents". */
	/* check queued events */
	ev = RGFW_eventQueuePop(win);
	if (ev != NULL) {
		if (ev->type == RGFW_quit) {
			RGFW_window_setShouldClose(win, RGFW_TRUE);
			if (win->_flags & RGFW_windowFreeOnClose) {
				RGFW_window_close(win);
			}
		}
		win->event = ev;
		return win->event;
    }

	RGFW_window_src* src = &win->src;
	if (src->has_checked_events == true) {
		win->event = NULL;
		src->has_checked_events = false;
		return NULL;
	}

	ev = &_RGFW->events[0];
	ev->_win = win;

	/* TODO(EimaMei): devkitPro actually gives you access to 'hidSharedMem' and
	 * 'hidMemHandle', allowing you to manage input yourself. Might have to take
	 * a better look at it. */
	if (!aptMainLoop()) {
		ev->type = RGFW_quit;
		src->has_checked_events = true;
		return ev;
	}

    RGFW_resetKeyPrev();
	hidScanInput();

	u32 pressed = hidKeysHeld();
	u32 released = hidKeysUp();
	circlePosition cpad;
	hidCircleRead(&cpad);

	ev->controller = RGFW_controllerGet(0);

	if (ev->controller->enabled_motions[RGFW_motionAccelerometer]) {
		accelVector accelOld = src->accel;
		hidAccelRead(&src->accel);

		if (src->accel.x != accelOld.x || src->accel.y != accelOld.y || src->accel.z != accelOld.z) {
			ev->type = RGFW_motionMove;
			ev->motion = RGFW_motionAccelerometer;

			/* TODO(EimaMei): Check if these conversations are even accurate. */
			RGFW_point3D* vector = &ev->controller->motions[ev->motion];
			vector->x = src->accel.x / 512.0f;
			vector->y = src->accel.y / 512.0f;
			vector->z = src->accel.z / 512.0f;

			RGFW_eventQueuePush(*ev);
			RGFW_pointerMoveCallback(win, ev->controller, ev->motion);
		}
	}

	if (ev->controller->enabled_motions[RGFW_motionGyroscope]) {
		angularRate gyroOld = src->gyro;
		hidGyroRead(&src->gyro);

		if (src->gyro.x != gyroOld.x || src->gyro.y != gyroOld.y || src->gyro.z != gyroOld.z) {
			ev->type = RGFW_motionMove;
			ev->motion = RGFW_motionGyroscope;

			/* TODO(EimaMei): Check if these conversations are even accurate. */
			RGFW_point3D* vector = &ev->controller->motions[ev->motion];
			vector->x = src->gyro.x / 16384.0f;
			vector->y = src->gyro.y / 16384.0f;
			vector->z = src->gyro.z / 16384.0f;

			/* TODO(EimaMei): Callback here. */
			RGFW_eventQueuePush(*ev);
			RGFW_pointerMoveCallback(win, ev->controller, ev->motion);
		}
	}

	while (pressed & (RGFW_CPAD_BITS_H | RGFW_CPAD_BITS_V)) {
		ev->type = RGFW_controllerAxisMove;

		float value;
		u32 bits = pressed;

		if (bits & RGFW_CPAD_BITS_H) {
			bits &= RGFW_CPAD_BITS_H;
			value = (float)cpad.dx;
			ev->axis = RGFW_axisTypeLeftX;
		}
		else {
			bits &= RGFW_CPAD_BITS_V;
			value = (float)cpad.dy;
			ev->axis = RGFW_axisTypeLeftY;
		}

		RGFW_axis* axis = &ev->controller->axes[ev->axis];
		/* NOTE(EimaMei): C-Pad range is -156 to 156. */
		axis->value    = value / 156.0f;
		/* NOTE(EimaMei): I picked '40' as the deadzone based on how the CPAD bits
		 * are set if the value is equal or larger than 41. (http://3dbrew.org/wiki/HID_Shared_Memory). */
		axis->deadzone = (40.0f / 156.0f);

		pressed &= ~bits;
		RGFW_eventQueuePush(*ev);
		RGFW_controllerAxisCallback(win, ev->controller, ev->axis);
	}

	while (pressed & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(pressed);
		RGFW_ASSERT(button != -1);

		ev->type = RGFW_buttonPressed;
		ev->button = button;

		RGFW_buttonState* state = &ev->controller->buttons[ev->button];
		RGFW_MASK_SET(*state, RGFW_buttonStatePrevious, *state & RGFW_buttonStateCurrent);
		RGFW_MASK_SET(*state, RGFW_buttonStateCurrent, true);
		pressed &= ~RGFW_rgfwToApiKey(button);

		RGFW_eventQueuePush(*ev);
		RGFW_buttonCallback(win, ev->controller, ev->button, true);
	}

	while (released & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(released);

		ev->type = RGFW_buttonReleased;
		ev->button = button;

		RGFW_buttonState* state = &ev->controller->buttons[ev->button];
		RGFW_MASK_SET(*state, RGFW_buttonStatePrevious, *state & RGFW_buttonStateCurrent);
		RGFW_MASK_SET(*state, RGFW_buttonStateCurrent, false);
		released &= ~RGFW_rgfwToApiKey(button);

		RGFW_eventQueuePush(*ev);
		RGFW_buttonCallback(win, ev->controller, ev->button, false);
	}

	if (ev->controller->enabled_pointers[RGFW_pointerTouchscreen] && pressed & KEY_TOUCH) {
		touchPosition touch;
		hidTouchRead(&touch);

		ev->type       = RGFW_pointerMove;
		ev->controller->pointers[RGFW_pointerTouchscreen] = RGFW_POINT(touch.px, touch.py);
		pressed &= (u32)~KEY_TOUCH;

		RGFW_eventQueuePush(*ev);
		RGFW_pointerMoveCallback(win, ev->controller, RGFW_pointerTouchscreen);
	}

	src->has_checked_events = true;
	if (_RGFW->eventLen > 0) {
		win->event = RGFW_eventQueuePop(win);
		return win->event;
	}

	return NULL;
}

void RGFW_window_close(RGFW_window* win) {
	/* NOTE(EimaMei): Only do gfxExit if win is the last window. */
	// TODO(EimaMei: ("riley has to fix this.\n");

#ifdef RGFW_OPENGL
	RGFW_window_freeOpenGL(win);
#endif

#ifdef RGFW_BUFFER
	if (win->buffer != NULL) {
		win->buffer = NULL;

		if ((win->_flags & RGFW_BUFFER_ALLOC)) {
			RGFW_FREE_SYS(win->src.buffers[0]);
		}

		#ifdef RGFW_BUFFER_NATIVE
		RGFW_FREE_SYS(win->src.buffers[1]);

		#else
		RGFW_FREE(win->src.buffers[1]);

		RGFW_FREE_SYS(win->src.buffers_native[0]);
		RGFW_FREE_SYS(win->src.buffers_native[1]);
		#endif
	}
#endif

	_RGFW->windowCount -= 1;
	if (_RGFW->windowCount == 0) {
		RGFW_deinit();
	}

	if ((win->_flags & RGFW_WINDOW_ALLOC)) {
		RGFW_FREE(win);
	}
}

RGFW_area RGFW_windowGetSize(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	static const i32 WIDTH_LUT[RGFW_videoModeCount] = {400, 400, 800, 320};
	return RGFW_AREA(WIDTH_LUT[win->mode], 240);
}


#if defined(RGFW_OPENGL)

void RGFW_window_makeCurrent_OpenGL(RGFW_window* win) {
	glassBindContext(win ? win->src.ctx : NULL);
}

void* RGFW_getCurrent_OpenGL(void) {
	return glassGetBoundContext();
}

void RGFW_window_swapBuffers_OpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	glassSwapContextBuffers(win->src.ctx, NULL);
}

void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval) {
	RGFW_ASSERT(win != NULL);
	((GLASSCtxParams*)win->src.ctx)->vsync = RGFW_BOOL(swapInterval);
}

bool RGFW_system_OpenGL_fixScreen(GLuint shader_program, const char* mat4_uniform_name) {
    static const float deg90_rotation_matrix[4][4] = {
        { 0.0f,  1.0f, 0.0f, 0.0f },
        {-1.0f,  0.0f, 0.0f, 0.0f },
        { 0.0f,  0.0f, 1.0f, 0.0f },
        { 0.0f,  0.0f, 0.0f, 1.0f }
    };

	GLint uniform = glGetUniformLocation(shader_program, mat4_uniform_name);
	if (uniform == -1) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errOpenGLContext,  RGFW_DEBUG_CTX(NULL, 0), "Invalid uniform name.");
		return false;
	}

    glUniformMatrix4fv(uniform, 1, GL_FALSE, (const float*)deg90_rotation_matrix);
	return true;
}

#endif

void RGFW_window_swapBuffers_software(RGFW_window* win) {
#if defined(RGFW_BUFFER)

#ifndef RGFW_BUFFER_NATIVE
	RGFW_window_bufferToNative(win);
	u8* buffer = win->src.buffers_native[win->src.current_buffer];
#else
	u8* buffer = win->buffer;
#endif
	GSPGPU_FlushDataCache(buffer, (u32)win->src.buffer_size);

	_RGFW__gfxPresentFramebuffer(win, (RGFW_systemGet3DSlider() == 0.0f));
	gspWaitForVBlank();

	win->src.current_buffer ^= 1;
	win->buffer = win->src.buffers[win->src.current_buffer];
#else
	RGFW_UNUSED(win);
#endif
}

#endif

#endif /* RGFW_IMPLEMENTATION */

#if defined(__cplusplus) && !defined(__EMSCRIPTEN__)
}
#endif

#if _MSC_VER
	#pragma warning( pop )
#endif


#endif /* RGFW_HEADER */
