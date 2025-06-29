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
	#define RGFW_EGL - (optional) use EGL for loading an OpenGL context (instead of the system's opengl api)
	#define RGFW_OPENGL_ES1 - (optional) use EGL to load and use Opengl ES (version 1) for backend rendering (instead of the system's opengl api)
									This version doesn't work for desktops (I'm pretty sure)
	#define RGFW_OPENGL_ES2 - (optional) use OpenGL ES (version 2)
	#define RGFW_OPENGL_ES3 - (optional) use OpenGL ES (version 3)
	#define RGFW_DIRECTX - (optional) include integration directX functions (windows only)
	#define RGFW_VULKAN - (optional) include helpful vulkan integration functions and macros
	#define RGFW_WEBGPU - (optional) use webGPU for rendering (Web ONLY)
	#define RGFW_NO_API - (optional) don't use any rendering API (no opengl, no vulkan, no directX)

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
	#define RGFW_USERPTR x - choose the default userptr sent to the malloc call, (NULL by default)

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
        @halocupcake -> code review (opengl)
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

#ifndef RGFW_USERPTR
	#define RGFW_USERPTR NULL
#endif

#ifndef RGFW_UNUSED
	#define RGFW_UNUSED(x) (void)(x)
#endif

#ifndef RGFW_ROUND
	#define RGFW_ROUND(x) (i32)((x) >= 0 ? (x) + 0.5f : (x) - 0.5f)
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

#if !defined(RGFW_MEMCPY) || !defined(RGFW_STRNCMP) || !defined(RGFW_STRNCPY) || !defined(RGFW_MEMSET)
	#include <string.h>
#endif

#ifndef RGFW_MEMSET
	#define RGFW_MEMSET(ptr, value, num) memset(ptr, value, num)
#endif

#ifndef RGFW_MEMCPY
	#define RGFW_MEMCPY(dist, src, len) memcpy(dist, src, len)
#endif

#ifndef RGFW_STRNCMP
	#define RGFW_STRNCMP(s1, s2, max) strncmp(s1, s2, max)
#endif

#ifndef RGFW_STRNCPY
	#define RGFW_STRNCPY(dist, src, len) strncpy(dist, src, len)
#endif

#ifndef RGFW_STRSTR
	#define RGFW_STRSTR(str, substr) strstr(str, substr)
#endif

#ifndef RGFW_STRTOL
	/* required for X11 XDnD and X11 Monitor DPI */
	#include <stdlib.h>
	#define RGFW_STRTOL(str, endptr, base) strtol(str, endptr, base)
	#define RGFW_ATOF(num) atof(num)
#endif

#if !defined(RGFW_PRINTF) && ( defined(RGFW_DEBUG) || defined(RGFW_WAYLAND) )
    /* required when using RGFW_DEBUG */
    #include <stdio.h>
    #define RGFW_PRINTF printf
#endif

#ifdef RGFW_WIN95 /* for windows 95 testing (not that it really works) */
	#define RGFW_NO_MONITOR
	#define RGFW_NO_PASSTHROUGH
#endif

#if defined(RGFW_EXPORT) ||  defined(RGFW_IMPORT)
	#if defined(_WIN32)
		#if defined(__TINYC__) && (defined(RGFW_EXPORT) ||  defined(RGFW_IMPORT))
			#define __declspec(x) __attribute__((x))
		#endif

		#if defined(RGFW_EXPORT)
			#define RGFWDEF __declspec(dllexport)
		#else
			#define RGFWDEF __declspec(dllimport)
		#endif
	#else
		#if defined(RGFW_EXPORT)
			#define RGFWDEF __attribute__((visibility("default")))
		#endif
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
	#define RGFW_MAX_BUTTON_COUNT RGFW_controllerButtonCount

	/* TODO(EimaMei): Circle Pad Pro support would require this to be 4, but as
	 * of June 29th 2025 I'm not planning to add support for it anytime soon,
	 * unless this PR gets merged: https://github.com/devkitPro/libctru/pull/568. */
	#ifndef RGFW_MAX_AXIS_COUNT
	#define RGFW_MAX_AXIS_COUNT 2
	#endif
	#define RGFW_MAX_MOTION_SENSORS 2


	#define RGFW_HAS_POINTER_SUPPORT   1
	#define RGFW_HAS_MOTION_SUPPORT    1
	#define RGFW_HAS_KEYBOARD_SUPPORT  0
	#define RGFW_HAS_MOUSE_SUPPORT     0

	#include <3ds.h>
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
	typedef struct RGFW_point3D { i32 x, y, z; } RGFW_point3D;
#endif

/*! basic rect type, if there's not already a rect type of choice */
#ifndef RGFW_rect
	typedef struct RGFW_rect { i32 x, y, w, h; } RGFW_rect;
#endif

/*! basic area type, if there's not already a area type of choice */
#ifndef RGFW_area
	typedef struct RGFW_area { u32 w, h; } RGFW_area;
#endif

#if defined(__cplusplus) && !defined(__APPLE__)
#define RGFW_POINT(x, y) {(i32)x, (i32)y}
#define RGFW_RECT(x, y, w, h) {(i32)x, (i32)y, (i32)w, (i32)h}
#define RGFW_AREA(w, h) {(u32)w, (u32)h}
#else
#define RGFW_POINT(x, y) (RGFW_point){(i32)(x), (i32)(y)}
#define RGFW_RECT(x, y, w, h) (RGFW_rect){(i32)(x), (i32)(y), (i32)(w), (i32)(h)}
#define RGFW_AREA(w, h) (RGFW_area){(u32)(w), (u32)(h)}
#endif

#ifndef RGFW_NO_MONITOR
	/* monitor mode data | can be changed by the user (with functions)*/
	typedef struct RGFW_monitorMode {
		RGFW_area area; /*!< monitor workarea size */
		u32 refreshRate; /*!< monitor refresh rate */
		u8 red, blue, green;
	} RGFW_monitorMode;

	/*! structure for monitor data */
	typedef struct RGFW_monitor {
		i32 x, y; /*!< x - y of the monitor workarea */
		char name[128]; /*!< monitor name */
		float scaleX, scaleY; /*!< monitor content scale */
		float pixelRatio; /*!< pixel ratio for monitor (1.0 for regular, 2.0 for hiDPI)  */
		float physW, physH; /*!< monitor physical size in inches */

		RGFW_monitorMode mode;
	} RGFW_monitor;

	/*! get an array of all the monitors (max 6) */
	RGFWDEF RGFW_monitor* RGFW_getMonitors(size_t* len);
	/*! get the primary monitor */
	RGFWDEF RGFW_monitor RGFW_getPrimaryMonitor(void);

	typedef RGFW_ENUM(u8, RGFW_modeRequest) {
		RGFW_monitorScale = RGFW_BIT(0), /*!< scale the monitor size */
		RGFW_monitorRefresh = RGFW_BIT(1), /*!< change the refresh rate */
		RGFW_monitorRGB = RGFW_BIT(2), /*!< change the monitor RGB bits size */
		RGFW_monitorAll = RGFW_monitorScale | RGFW_monitorRefresh | RGFW_monitorRGB
	};

	/*! request a specific mode */
	RGFWDEF RGFW_bool RGFW_monitor_requestMode(RGFW_monitor mon, RGFW_monitorMode mode, RGFW_modeRequest request);
	/*! check if 2 monitor modes are the same */
	RGFWDEF RGFW_bool RGFW_monitorModeCompare(RGFW_monitorMode mon, RGFW_monitorMode mon2, RGFW_modeRequest request);
#endif

/* RGFW mouse loading */
typedef void RGFW_mouse;

/*!< loads mouse icon from bitmap (similar to RGFW_window_setIcon). Icon NOT resized by default */
RGFWDEF RGFW_mouse* RGFW_loadMouse(u8* icon, RGFW_area a, i32 channels);
/*!< frees RGFW_mouse data */
RGFWDEF void RGFW_freeMouse(RGFW_mouse* mouse);


typedef struct RGFW_axis {
	i16 value;
	i16 deadzone;
} RGFW_axis;


/* NOTE: some parts of the data can represent different things based on the event (read comments in RGFW_event struct) */
/*! Event structure for checking/getting events */
typedef struct RGFW_event {
	RGFW_eventType type; /*!< which event has been sent?*/
	RGFW_point point; /*!< mouse x, y of event (or drop point) */

	/*! which controller this event applies to (if applicable to any) */
	ssize_t input_index;

	ssize_t whichAxis; /* which axis was effected */
	RGFW_axis axis;

	RGFW_bool repeat; /*!< key press event repeated (the key is being held) */
	RGFW_button button; /* !< which mouse (or controller) button was pressed */

	void* _win; /*!< the window this event applies too (for event queue events) */
} RGFW_event;

/*! source data for the window (used by the APIs) */
#if defined(RGFW_3DS)

typedef struct RGFW_window_src {

} RGFW_window_src;

#endif

/*! Optional arguments for making a windows */
typedef RGFW_ENUM(u32, RGFW_windowFlags) {
	RGFW_windowNoInitAPI = RGFW_BIT(0), /* do NOT init an API (including the software rendering buffer) (mostly for bindings. you can also use `#define RGFW_NO_API`) */
	RGFW_windowNoBorder = RGFW_BIT(1), /*!< the window doesn't have a border */
	RGFW_windowNoResize = RGFW_BIT(2), /*!< the window cannot be resized by the user */
	RGFW_windowAllowDND = RGFW_BIT(3), /*!< the window supports drag and drop */
	RGFW_windowHideMouse = RGFW_BIT(4), /*! the window should hide the mouse (can be toggled later on using `RGFW_window_mouseShow`) */
	RGFW_windowFullscreen = RGFW_BIT(5), /*!< the window is fullscreen by default */
	RGFW_windowTransparent = RGFW_BIT(6), /*!< the window is transparent (only properly works on X11 and MacOS, although it's meant for for windows) */
	RGFW_windowCenter = RGFW_BIT(7), /*! center the window on the screen */
	RGFW_windowOpenglSoftware = RGFW_BIT(8), /*! use OpenGL software rendering */
	RGFW_windowCocoaCHDirToRes = RGFW_BIT(9), /*! (cocoa only), change directory to resource folder */
	RGFW_windowScaleToMonitor = RGFW_BIT(10), /*! scale the window to the screen */
	RGFW_windowHide = RGFW_BIT(11), /*! the window is hidden */
	RGFW_windowMaximize = RGFW_BIT(12),
	RGFW_windowCenterCursor = RGFW_BIT(13),
	RGFW_windowFloating = RGFW_BIT(14), /*!< create a floating window */
	RGFW_windowFreeOnClose = RGFW_BIT(15), /*!< free (RGFW_window_close) the RGFW_window struct when the window is closed (by the end user) */
	RGFW_windowFocusOnShow = RGFW_BIT(16), /*!< focus the window when it's shown */
	RGFW_windowMinimize = RGFW_BIT(17), /*!< focus the window when it's shown */
	RGFW_windowFocus = RGFW_BIT(18), /*!< if the window is in focus */
	RGFW_windowedFullscreen = RGFW_windowNoBorder | RGFW_windowMaximize
};

typedef struct RGFW_window {
	RGFW_window_src src; /*!< src window data */

#if defined(RGFW_BUFFER)
	u8* buffer; /*!< buffer for non-GPU systems (OSMesa, basic software rendering) */
	/* when rendering using RGFW_BUFFER, the buffer is in the RGBA format */
	RGFW_area bufferSize;
#endif
	void* userPtr; /* ptr for usr data */

	RGFW_event event; /*!< current event */

	RGFW_area r; /*!< the w and h of the struct */

    /*! which key RGFW_window_shouldClose checks. Settting this to RGFW_keyNULL disables the feature. */
    RGFW_key exitKey;
	RGFW_point _lastMousePoint; /*!< last cusor point (for raw mouse data) */

	u32 _flags; /*!< windows flags (for RGFW to check) */
	RGFW_area _oldRect; /*!< area before fullscreen */
} RGFW_window; /*!< window structure for managing the window */

/*! scale monitor to window size */
RGFWDEF RGFW_bool RGFW_monitor_scaleToWindow(RGFW_monitor mon, RGFW_window* win);

/** * @defgroup Window_management
* @{ */


/*!
 * the class name for X11 and WinAPI. apps with the same class will be grouped by the WM
 * by default the class name will == the root window's name
*/
RGFWDEF void RGFW_setXInstName(const char* name); /*!< X11 instance name (window name will by used by default) */

/*! (cocoa only) change directory to resource folder */
RGFWDEF void RGFW_moveToMacOSResourceDir(void);

/* NOTE: (windows) if the executable has an icon resource named RGFW_ICON, it will be set as the initial icon for the window */

RGFWDEF RGFW_window* RGFW_createWindow(
	RGFW_area rect, /* rect of window */
	RGFW_windowFlags flags /* extra arguments ((u32)0 means no flags used)*/
); /*!< function to create a window and struct */

RGFWDEF RGFW_window* RGFW_createWindowPtr(
	RGFW_area area, /* area of window */
	RGFW_windowFlags flags, /* extra arguments (NULL / (u32)0 means no flags used) */
	RGFW_window* win /* ptr to the window struct you want to use */
); /*!< function to create a window (without allocating a window struct) */

RGFWDEF void RGFW_window_initBuffer(RGFW_window* win);
RGFWDEF void RGFW_window_initBufferSize(RGFW_window* win, RGFW_area area);
RGFWDEF void RGFW_window_initBufferPtr(RGFW_window* win, u8* buffer, RGFW_area area);

/*! set the window flags (will undo flags if they don't match the old ones) */
RGFWDEF void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags);

/*! get the size of the screen to an area struct */
RGFWDEF RGFW_area RGFW_getScreenSize(void);


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

/*! move a window to a given point */
RGFWDEF void RGFW_window_move(RGFW_window* win,
	RGFW_point v /*!< new pos */
);

#ifndef RGFW_NO_MONITOR
	/*! move window to a specific monitor */
	RGFWDEF void RGFW_window_moveToMonitor(RGFW_window* win, RGFW_monitor m /* monitor */);
#endif

/*! resize window to a current size/area */
RGFWDEF void RGFW_window_resize(RGFW_window* win, /*!< source window */
	RGFW_area a /*!< new size */
);

/*! set window aspect ratio */
RGFWDEF void RGFW_window_setAspectRatio(RGFW_window* win, RGFW_area a);
/*! set the minimum dimensions of a window */
RGFWDEF void RGFW_window_setMinSize(RGFW_window* win, RGFW_area a);
/*! set the maximum dimensions of a window */
RGFWDEF void RGFW_window_setMaxSize(RGFW_window* win, RGFW_area a);

RGFWDEF void RGFW_window_focus(RGFW_window* win); /*!< sets the focus to this window */
RGFWDEF RGFW_bool RGFW_window_isInFocus(RGFW_window* win); /*!< checks the focus to this window */
RGFWDEF void RGFW_window_raise(RGFW_window* win); /*!< raise the window (to the top) */
RGFWDEF void RGFW_window_maximize(RGFW_window* win); /*!< maximize the window */
RGFWDEF void RGFW_window_setFullscreen(RGFW_window* win, RGFW_bool fullscreen); /*!< turn fullscreen on / off for a window */
RGFWDEF void RGFW_window_center(RGFW_window* win); /*!< center the window */
RGFWDEF void RGFW_window_minimize(RGFW_window* win); /*!< minimize the window (in taskbar (per OS))*/
RGFWDEF void RGFW_window_restore(RGFW_window* win); /*!< restore the window from minimized (per OS)*/
RGFWDEF void RGFW_window_setFloating(RGFW_window* win, RGFW_bool floating); /*!< make the window a floating window */
RGFWDEF void RGFW_window_setOpacity(RGFW_window* win, u8 opacity); /*!< sets the opacity of a window */

RGFWDEF RGFW_bool RGFW_window_opengl_isSoftware(RGFW_window* win);

/*! if the window should have a border or not (borderless) based on bool value of `border` */
RGFWDEF void RGFW_window_setBorder(RGFW_window* win, RGFW_bool border);
RGFWDEF RGFW_bool RGFW_window_borderless(RGFW_window* win);

/*! turn on / off dnd (RGFW_windowAllowDND stil must be passed to the window)*/
RGFWDEF void RGFW_window_setDND(RGFW_window* win, RGFW_bool allow);
/*! check if DND is allowed */
RGFWDEF RGFW_bool RGFW_window_allowsDND(RGFW_window* win);


#ifndef RGFW_NO_PASSTHROUGH
	/*! turn on / off mouse passthrough */
	RGFWDEF void RGFW_window_setMousePassthrough(RGFW_window* win, RGFW_bool passthrough);
#endif

/*! rename window to a given string */
RGFWDEF void RGFW_window_setName(RGFW_window* win,
	const char* name
);

RGFWDEF RGFW_bool RGFW_window_setIcon(RGFW_window* win, /*!< source window */
	u8* icon /*!< icon bitmap */,
	RGFW_area a /*!< width and height of the bitmap */,
	i32 channels /*!< how many channels the bitmap has (rgb : 3, rgba : 4) */
); /*!< image MAY be resized by default, set both the taskbar and window icon */

typedef RGFW_ENUM(u8, RGFW_icon) {
	RGFW_iconTaskbar = RGFW_BIT(0),
	RGFW_iconWindow = RGFW_BIT(1),
	RGFW_iconBoth = RGFW_iconTaskbar | RGFW_iconWindow
};
RGFWDEF RGFW_bool RGFW_window_setIconEx(RGFW_window* win, u8* icon, RGFW_area a, i32 channels, u8 type);

/*!< sets mouse to RGFW_mouse icon (loaded from a bitmap struct) */
RGFWDEF void RGFW_window_setMouse(RGFW_window* win, RGFW_mouse* mouse);

/*!< sets the mouse to a standard API cursor (based on RGFW_MOUSE, as seen at the end of the RGFW_HEADER part of this file) */
RGFWDEF	RGFW_bool RGFW_window_setMouseStandard(RGFW_window* win, u8 mouse);

RGFWDEF RGFW_bool RGFW_window_setMouseDefault(RGFW_window* win); /*!< sets the mouse to the default mouse icon */
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

/*! hide the window */
RGFWDEF void RGFW_window_hide(RGFW_window* win);
/*! show the window */
RGFWDEF void RGFW_window_show(RGFW_window* win);

/*
	makes it so `RGFW_window_shouldClose` returns true or overrides a window close
	by modifying window flags
*/
RGFWDEF void RGFW_window_setShouldClose(RGFW_window* win, RGFW_bool shouldClose);

/*! where the mouse is on the screen */
RGFWDEF RGFW_point RGFW_getGlobalMousePoint(void);

/*! where the mouse is on the window */
RGFWDEF RGFW_point RGFW_window_getMousePoint(RGFW_window* win);

/*! show the mouse or hide the mouse */
RGFWDEF void RGFW_window_showMouse(RGFW_window* win, RGFW_bool show);
/*! if the mouse is hidden */
RGFWDEF RGFW_bool RGFW_window_mouseHidden(RGFW_window* win);
/*! move the mouse to a given point */
RGFWDEF void RGFW_window_moveMouse(RGFW_window* win, RGFW_point v);

/*! if the window should close (RGFW_close was sent or escape was pressed) */
RGFWDEF RGFW_bool RGFW_window_shouldClose(RGFW_window* win);
/*! if the window is fullscreen */
RGFWDEF RGFW_bool RGFW_window_isFullscreen(RGFW_window* win);
/*! if the window is hidden */
RGFWDEF RGFW_bool RGFW_window_isHidden(RGFW_window* win);
/*! if the window is minimized */
RGFWDEF RGFW_bool RGFW_window_isMinimized(RGFW_window* win);
/*! if the window is maximized */
RGFWDEF RGFW_bool RGFW_window_isMaximized(RGFW_window* win);
/*! if the window is floating */
RGFWDEF RGFW_bool RGFW_window_isFloating(RGFW_window* win);
/** @} */

/** * @defgroup Monitor
* @{ */

#ifndef RGFW_NO_MONITOR
/*
	scale the window to the monitor.
	This is run by default if the user uses the arg `RGFW_scaleToMonitor` during window creation
*/
RGFWDEF void RGFW_window_scaleToMonitor(RGFW_window* win);
/*! get the struct of the window's monitor  */
RGFWDEF RGFW_monitor RGFW_window_getMonitor(RGFW_window* win);
#endif

/** @} */

/** * @defgroup Input
* @{ */

RGFWDEF RGFW_bool RGFW_isPressed(u8 controller, RGFW_button button); /*!< if key is pressed (key code)*/

RGFWDEF RGFW_bool RGFW_wasPressed(u8 controller, RGFW_button button); /*!< if key was pressed (checks previous state only) (key code) */

RGFWDEF RGFW_bool RGFW_isHeld(u8 controller, RGFW_button button); /*!< if key is held (key code) */
RGFWDEF RGFW_bool RGFW_isReleased(u8 controller, RGFW_button button); /*!< if key is released (key code) */


/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF ssize_t RGFW_controllerGetCount(void);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_controllerName(RGFW_controllerType type);

/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_buttonName(RGFW_button button);
/* TODO(EimaMei): NEW FUNCTION */
RGFWDEF const char* RGFW_axisName(i32 axis);

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
	RGFW_errOpenglContext, RGFW_errEGLContext, /*!< error with the OpenGL context */
	RGFW_errWayland, RGFW_errX11,
	RGFW_errDirectXContext,
	RGFW_errIOKit,
	RGFW_errClipboard,
	RGFW_errFailedFuncLoad,
	RGFW_errBuffer,
	RGFW_infoMonitor, RGFW_infoWindow, RGFW_infoBuffer, RGFW_infoGlobal, RGFW_infoOpenGL,
	RGFW_warningWayland, RGFW_warningOpenGL
};

typedef struct RGFW_debugContext { RGFW_window* win; RGFW_monitor* monitor; u32 srcError; } RGFW_debugContext;

#if defined(__cplusplus) && !defined(__APPLE__)
#define RGFW_DEBUG_CTX(win, err) {win, NULL, err}
#define RGFW_DEBUG_CTX_MON(monitor) {_RGFW->root, &monitor, 0}
#else
#define RGFW_DEBUG_CTX(win, err) (RGFW_debugContext){win, NULL, err}
#define RGFW_DEBUG_CTX_MON(monitor) (RGFW_debugContext){_RGFW->root, &monitor, 0}
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
typedef void (* RGFW_buttonfunc)(RGFW_window* win, ssize_t controller, RGFW_button button, RGFW_bool pressed);
/*! RGFW_controllerAxisMove, the window that got the event, the controller in question, the axis values and the axis count */
typedef void (* RGFW_controllerAxisfunc)(RGFW_window* win, ssize_t controller, ssize_t whichAxis);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (* RGFW_pointerMovefunc)(RGFW_window* win, ssize_t pointer, RGFW_point point);
/*! RGFW_controllerConnected / RGFW_controllerDisconnected, the window that got the event, the controller in question, if the controller was connected (else it was disconnected) */
typedef void (* RGFW_controllerfunc)(RGFW_window* win, u16 controller, RGFW_bool connected);

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

/*!< make the window the current opengl drawing context

	NOTE:
 	if you want to switch the graphics context's thread,
	you have to run RGFW_window_makeCurrent(NULL); on the old thread
	then RGFW_window_makeCurrent(valid_window) on the new thread
*/
RGFWDEF void RGFW_window_makeCurrent(RGFW_window* win);

/*! get current RGFW window graphics context */
RGFWDEF RGFW_window* RGFW_getCurrent(void);

/* supports openGL, directX, OSMesa, EGL and software rendering */
RGFWDEF void RGFW_window_swapBuffers(RGFW_window* win); /*!< swap the rendering buffer */
RGFWDEF void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval);
/*!< render the software rendering buffer (this is called by RGFW_window_swapInterval)  */
RGFWDEF void RGFW_window_swapBuffers_software(RGFW_window* win);

typedef void (*RGFW_proc)(void); /* function pointer equivalent of void* */

/*! native API functions */
#if defined(RGFW_OPENGL) || defined(RGFW_EGL)
/*!< create an opengl context for the RGFW window, run by createWindow by default (unless the RGFW_windowNoInitAPI is included) */
RGFWDEF void RGFW_window_initOpenGL(RGFW_window* win);
/*!< called by `RGFW_window_close` by default (unless the RGFW_windowNoInitAPI is set) */
RGFWDEF void RGFW_window_freeOpenGL(RGFW_window* win);

/*! OpenGL init hints */
typedef RGFW_ENUM(u8, RGFW_glHints)  {
	RGFW_glStencil = 0,  /*!< set stencil buffer bit size (8 by default) */
	RGFW_glSamples, /*!< set number of sampiling buffers (4 by default) */
	RGFW_glStereo, /*!< use GL_STEREO (GL_FALSE by default) */
	RGFW_glAuxBuffers, /*!< number of aux buffers (0 by default) */
	RGFW_glDoubleBuffer, /*!< request double buffering */
	RGFW_glRed, RGFW_glGreen, RGFW_glBlue, RGFW_glAlpha, /*!< set RGBA bit sizes */
	RGFW_glDepth,
	RGFW_glAccumRed, RGFW_glAccumGreen, RGFW_glAccumBlue,RGFW_glAccumAlpha, /*!< set accumulated RGBA bit sizes */
	RGFW_glSRGB, /*!< request sRGA */
	RGFW_glRobustness, /*!< request a robust context */
	RGFW_glDebug, /*!< request opengl debugging */
	RGFW_glNoError, /*!< request no opengl errors */
	RGFW_glReleaseBehavior,
	RGFW_glProfile,
	RGFW_glMajor, RGFW_glMinor,
	RGFW_glFinalHint = 32, /*!< the final hint (not for setting) */
	RGFW_releaseFlush = 0,  RGFW_glReleaseNone, /* RGFW_glReleaseBehavior options */
	RGFW_glCore = 0,  RGFW_glCompatibility /*!< RGFW_glProfile options */
};
RGFWDEF void RGFW_setGLHint(RGFW_glHints hint, i32 value);
RGFWDEF RGFW_bool RGFW_extensionSupported(const char* extension, size_t len);	/*!< check if whether the specified API extension is supported by the current OpenGL or OpenGL ES context */
RGFWDEF RGFW_proc RGFW_getProcAddress(const char* procname); /*!< get native opengl proc address */
RGFWDEF void RGFW_window_makeCurrent_OpenGL(RGFW_window* win); /*!< to be called by RGFW_window_makeCurrent */
RGFWDEF void RGFW_window_swapBuffers_OpenGL(RGFW_window* win); /*!< swap opengl buffer (only) called by RGFW_window_swapInterval  */
void* RGFW_getCurrent_OpenGL(void); /*!< get the current context (OpenGL backend (GLX) (WGL) (EGL) (cocoa) (webgl))*/

RGFWDEF RGFW_bool RGFW_extensionSupportedPlatform(const char* extension, size_t len);	/*!< check if whether the specified platform-specific API extension is supported by the current OpenGL or OpenGL ES context */
#endif
#ifdef RGFW_VULKAN
	#if defined(RGFW_WAYLAND) && defined(RGFW_X11)
    	#define VK_USE_PLATFORM_WAYLAND_KHR
		#define VK_USE_PLATFORM_XLIB_KHR
        #define RGFW_VK_SURFACE ((RGFW_usingWayland()) ? ("VK_KHR_wayland_surface") : ("VK_KHR_xlib_surface"))
    #elif defined(RGFW_WAYLAND)
		#define VK_USE_PLATFORM_WAYLAND_KHR
		#define VK_USE_PLATFORM_XLIB_KHR
        #define RGFW_VK_SURFACE "VK_KHR_wayland_surface"
    #elif defined(RGFW_X11)
		#define VK_USE_PLATFORM_XLIB_KHR
		#define RGFW_VK_SURFACE "VK_KHR_xlib_surface"
	#elif defined(RGFW_WINDOWS)
		#define VK_USE_PLATFORM_WIN32_KHR
		#define OEMRESOURCE
		#define RGFW_VK_SURFACE "VK_KHR_win32_surface"
	#elif defined(RGFW_MACOS) && !defined(RGFW_MACOS_X11)
		#define VK_USE_PLATFORM_MACOS_MVK
		#define RGFW_VK_SURFACE "VK_MVK_macos_surface"
	#else
		#define RGFW_VK_SURFACE NULL
	#endif

/* if you don't want to use the above macros */
RGFWDEF const char** RGFW_getVKRequiredInstanceExtensions(size_t* count); /*!< gets (static) extension array (and size (which will be 2)) */

#include <vulkan/vulkan.h>

RGFWDEF VkResult RGFW_window_createVKSurface(RGFW_window* win, VkInstance instance, VkSurfaceKHR* surface);
RGFWDEF RGFW_bool RGFW_getVKPresentationSupport(VkInstance instance, VkPhysicalDevice physicalDevice, u32 queueFamilyIndex);
#endif
#ifdef RGFW_DIRECTX
#ifndef RGFW_WINDOWS
	#undef RGFW_DIRECTX
#else
	#define OEMRESOURCE
	#include <dxgi.h>

	#ifndef __cplusplus
		#define __uuidof(T) IID_##T
	#endif
RGFWDEF int RGFW_window_createDXSwapChain(RGFW_window* win, IDXGIFactory* pFactory, IUnknown* pDevice, IDXGISwapChain** swapchain);
#endif
#endif

/** @} */

/** * @defgroup Supporting
* @{ */

RGFWDEF double RGFW_getTime(void); /*!< get time in seconds since RGFW_setTime, which ran when the first window is open  */
RGFWDEF u64 RGFW_getTimeNS(void); /*!< get time in nanoseconds RGFW_setTime, which ran when the first window is open */
RGFWDEF void RGFW_sleep(u64 milisecond); /*!< sleep for a set time */
RGFWDEF void RGFW_setTime(double time); /*!< set timer in seconds */
RGFWDEF u64 RGFW_getTimerValue(void); /*!< get API timer value */
RGFWDEF u64 RGFW_getTimerFreq(void); /*!< get API time freq */

#define RGFW_MAX_EVENTS 32

/*< updates fps / sets fps to cap (must by ran manually by the user at the end of a frame), returns current fps */
RGFWDEF u32 RGFW_checkFPS(double startTime, u32 frameCount, u32 fpsCap);

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


typedef RGFW_ENUM(u8, RGFW_mouseIcons) {
	RGFW_mouseNormal = 0,
	RGFW_mouseArrow,
	RGFW_mouseIbeam,
	RGFW_mouseCrosshair,
	RGFW_mousePointingHand,
	RGFW_mouseResizeEW,
	RGFW_mouseResizeNS,
	RGFW_mouseResizeNWSE,
	RGFW_mouseResizeNESW,
	RGFW_mouseResizeAll,
	RGFW_mouseNotAllowed,
    RGFW_mouseIconFinal = 16 /* padding for alignment */
};
/** @} */

#endif /* RGFW_HEADER */
#if defined(RGFW_X11) || defined(RGFW_WAYLAND)
	#define RGFW_OS_BASED_VALUE(l, w, m, h) l
#elif defined(RGFW_WINDOWS)
	#define RGFW_OS_BASED_VALUE(l, w, m, h) w
#elif defined(RGFW_MACOS)
	#define RGFW_OS_BASED_VALUE(l, w, m, h) m
#elif defined(RGFW_WASM)
	#define RGFW_OS_BASED_VALUE(l, w, m, h) h
#endif

/*! optional init/deinit function */
RGFWDEF i32 RGFW_init(void); /*!< is called by default when the first window is created by default */
RGFWDEF void RGFW_deinit(void); /*!< is called by default when the last open window is closed */

RGFWDEF void* RGFW_init_heap(void); /*!< inits RGFW on the heap instead of in a global var */
RGFWDEF void RGFW_deinit_heap(void); /*!< deinits the heap instance */

typedef struct {
	RGFW_bool current  : 1;
	RGFW_bool prev  : 1;
} RGFW_keyState;

#if !defined(RGFW_NO_INFO) || defined(RGFW_IMPLEMENTATION)
typedef struct RGFW_info {
    RGFW_window* root;
    RGFW_window* current;
    i32 windowCount;
    i32 eventLen;
    i32 eventIndex;

    RGFW_mouse* hiddenMouse;
    RGFW_event events[RGFW_MAX_EVENTS];

	/* TODO(EimaMei): Decide what to do with keycodes. */
	#if RGFW_HAS_KEYBOARD_SUPPORT
    u32 apiKeycodes[RGFW_keyLast];
	/* u8 keycodes[RGFW_OS_BASED_VALUE(256, 512, 128, 256)]; */
	#endif

    /* controller data */
	ssize_t controllerCount; /*!< the actual amount of controllers */
	RGFW_controllerType controllerType[RGFW_MAX_CONTROLLER_DEVICES]; /*!< if a key is currently pressed or not (per controller) */
	RGFW_keyState controllerButtons[RGFW_MAX_CONTROLLER_DEVICES][RGFW_MAX_BUTTON_COUNT]; /*!< if a key is currently pressed or not (per controller) */
    RGFW_axis controllerAxes[RGFW_MAX_CONTROLLER_DEVICES][RGFW_MAX_AXIS_COUNT]; /*!< if a key is currently pressed or not (per controller) */

    RGFW_bool stopCheckEvents_bool;
    u64 timerOffset;

#if RGFW_3DS
	RGFW_bool check;
	u32 pressed, released;
	circlePosition cpad;
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


void RGFW_setTime(double time) {
    //_RGFW->timerOffset = RGFW_getTimerValue() - (u64)(time * (double)RGFW_getTimerFreq());
}

double RGFW_getTime(void) {
	//return (double) ((double)(RGFW_getTimerValue() - _RGFW->timerOffset) / (double)RGFW_getTimerFreq());
}

u64 RGFW_getTimeNS(void) {
	//return (u64)(((double)((RGFW_getTimerValue() - _RGFW->timerOffset)) * 1e9) / (double)RGFW_getTimerFreq());
}

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

RGFWDEF void RGFW_resetKeyPrev(void);


void RGFW_resetKeyPrev(void) {
	/* TODO(EimaMei): This feels inneficient to do each frame. Must be resolved. */
	size_t i, j;
	for (i = 0; i < RGFW_MAX_CONTROLLER_DEVICES; i += 1) {
		for (j = 0; j < RGFW_MAX_BUTTON_COUNT; j += 1) {
			_RGFW->controllerButtons[i][j].prev = RGFW_FALSE;
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
#define RGFW_controllerAxisCallback(w, controller, whichAxis) if (RGFW_controllerAxisCallbackSrc) RGFW_controllerAxisCallbackSrc(w, controller, whichAxis)

RGFW_CALLBACK_DEFINE(pointerMove, PointerMove)
#define RGFW_pointerMoveCallback(w, pointer, point) if (RGFW_pointerMoveCallbackSrc) RGFW_pointerMoveCallbackSrc(w, pointer, point)

RGFW_CALLBACK_DEFINE(controller, Controller)
#define RGFW_controllerCallback(w, controller, connected) if (RGFW_controllerCallbackSrc) RGFW_controllerCallbackSrc(w, controller, connected)

RGFW_CALLBACK_DEFINE(debug, Debug)
#define RGFW_debugCallback(type, err, ctx, msg) if (RGFW_debugCallbackSrc) RGFW_debugCallbackSrc(type, err, ctx, msg)
#undef RGFW_CALLBACK_DEFINE

#ifdef RGFW_DEBUG
#include <stdio.h>
#endif

void RGFW_sendDebugInfo(RGFW_debugType type, RGFW_errorCode err, RGFW_debugContext ctx, const char* msg) {
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
		case RGFW_errBuffer: case RGFW_infoBuffer: RGFW_PRINTF(" buffer size: %i %i\n", ctx.win->bufferSize.w, ctx.win->bufferSize.h); break;
		#endif
		case RGFW_infoMonitor: RGFW_PRINTF(": scale (%s):\n   rect: {%i, %i, %i, %i}\n   physical size:%f %f\n   scale: %f %f\n   pixelRatio: %f\n   refreshRate: %i\n   depth: %i\n", ctx.monitor->name, ctx.monitor->x, ctx.monitor->y, ctx.monitor->mode.area.w, ctx.monitor->mode.area.h, ctx.monitor->physW, ctx.monitor->physH, ctx.monitor->scaleX, ctx.monitor->scaleY, ctx.monitor->pixelRatio, ctx.monitor->mode.refreshRate, ctx.monitor->mode.red + ctx.monitor->mode.green + ctx.monitor->mode.blue); break;
		case RGFW_infoWindow: RGFW_PRINTF("  with rect of {%i, %i} \n", ctx.win->r.w, ctx.win->r.h); break;
		case RGFW_errDirectXContext: RGFW_PRINTF(" srcError %i\n", ctx.srcError); break;
		default: RGFW_PRINTF("\n");
	}
	#endif
}

void RGFW_window_checkEvents(RGFW_window* win, i32 waitMS) {
	RGFW_window_eventWait(win, waitMS);

	while (RGFW_window_checkEvent(win) != NULL && RGFW_window_shouldClose(win) == 0) {
		if (win->event.type == RGFW_quit) return;
	}

	#ifdef RGFW_WASM /* WASM needs to run the sleep function for asyncify */
		RGFW_sleep(0);
	#endif
}

void RGFW_window_checkMode(RGFW_window* win);
void RGFW_window_checkMode(RGFW_window* win) {
	/* TODO(EimaMei): Figure out this entire function. */
	if (RGFW_window_isMinimized(win)) {
		win->_flags |= RGFW_windowMinimize;
		/* TODO(EimaMei): Figure out. */
		// RGFW_windowMinimizedCallback(win, win->r);
	} else if (RGFW_window_isMaximized(win)) {
		win->_flags |= RGFW_windowMaximize;
		RGFW_eventQueuePushEx(e.type = RGFW_windowMaximized; e._win = win);
		/* TODO(EimaMei): Figure out. */
		// RGFW_windowMaximizedCallback(win, win->r);
	} else if (((win->_flags & RGFW_windowMinimize) && !RGFW_window_isMaximized(win)) ||
				(win->_flags & RGFW_windowMaximize && !RGFW_window_isMaximized(win))) {
		win->_flags &= ~(u32)RGFW_windowMinimize;
		if (RGFW_window_isMaximized(win) == RGFW_FALSE) win->_flags &= ~(u32)RGFW_windowMaximize;
		RGFW_eventQueuePushEx(e.type = RGFW_windowRestored; e._win = win);
		/* TODO(EimaMei): Figure out. */
		// RGFW_windowRestoredCallback(win, win->r);
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
#define RGFW_HOLD_MOUSE			RGFW_BIT(26) /*!< hold the moues still */
#define RGFW_MOUSE_LEFT 		RGFW_BIT(27) /* if mouse left the window */
#define RGFW_WINDOW_ALLOC 		RGFW_BIT(28) /* if window was allocated by RGFW */
#define RGFW_BUFFER_ALLOC 		RGFW_BIT(29) /* if window.buffer was allocated by RGFW */
#define RGFW_WINDOW_INIT 		RGFW_BIT(30) /* if window.buffer was allocated by RGFW */
#define RGFW_INTERNAL_FLAGS (RGFW_EVENT_QUIT | RGFW_HOLD_MOUSE |  RGFW_MOUSE_LEFT | RGFW_WINDOW_ALLOC | RGFW_BUFFER_ALLOC | RGFW_windowFocus)

RGFW_window* RGFW_createWindow(RGFW_area area, RGFW_windowFlags flags) {
	RGFW_window* win = (RGFW_window*)RGFW_ALLOC(sizeof(RGFW_window));
	RGFW_ASSERT(win != NULL);
    win->_flags = RGFW_WINDOW_ALLOC;
    return RGFW_createWindowPtr(area, flags, win);
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
    _RGFW->eventIndex = 0;
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
	if (_RGFW->eventLen >= RGFW_MAX_EVENTS) return;
	_RGFW->events[_RGFW->eventLen] = event;
    _RGFW->eventLen++;
}

RGFW_event* RGFW_eventQueuePop(RGFW_window* win) {
	RGFW_event* ev;
    if (_RGFW->eventLen == 0) return NULL;

	ev = (RGFW_event*)&_RGFW->events[_RGFW->eventIndex];

    _RGFW->eventLen--;
    if (_RGFW->eventLen >= 0 && _RGFW->eventIndex < _RGFW->eventLen) {
		_RGFW->eventIndex++;
    } else if (_RGFW->eventLen == 0) {
        _RGFW->eventIndex = 0;
    }

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

	if (win->event.type == 0 && _RGFW->eventLen == 0) {
		RGFW_resetKeyPrev();
	}

	if (win->event.type == RGFW_quit && win->_flags & RGFW_windowFreeOnClose) {
        static RGFW_event event;
        event = win->event;
        RGFW_window_close(win);
	    return &event;
    }

	/* TODO(EimaMei): I think this is correct? */
	/* if (win->event.type != RGFW_DNDInit) win->event.type = 0; */
	win->event.type = 0;

	/* check queued events */
	ev = RGFW_eventQueuePop(win);
	if (ev != NULL) {
		if (ev->type == RGFW_quit) RGFW_window_setShouldClose(win, RGFW_TRUE);
		win->event = *ev;
    }
	else return NULL;

	return &win->event;
}


RGFWDEF void RGFW_window_basic_init(RGFW_window* win, RGFW_area area, RGFW_windowFlags flags);
void RGFW_setRootWindow(RGFW_window* win) { _RGFW->root = win; }
RGFW_window* RGFW_getRootWindow(void) { return _RGFW->root; }

/* do a basic initialization for RGFW_window, this is to standard it for each OS */
void RGFW_window_basic_init(RGFW_window* win, RGFW_area area, RGFW_windowFlags flags) {
	RGFW_UNUSED(flags);
    if (_RGFW == NULL) RGFW_init();
    _RGFW->windowCount++;

    /* rect based the requested flags */
	if (_RGFW->root == NULL) {
		RGFW_setRootWindow(win);
		RGFW_setTime(0);
	}

	if (!(win->_flags & RGFW_WINDOW_ALLOC)) win->_flags = 0;

	/* set and init the new window's data */
	win->r = area;
	/* TODO(EimaMei): Add an input exit button? Should this be kept for keyboard
	 * supported system? */
#if RGFW_HAS_KEYBOARD_SUPPORT
	win->exitKey = RGFW_escape;
#endif

	win->_flags = 0 | (win->_flags & RGFW_WINDOW_ALLOC);
    win->_flags |= flags;
	win->_lastMousePoint.x = 0;
	win->_lastMousePoint.y = 0;
}

void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags flags) {
	RGFW_windowFlags cmpFlags = win->_flags;
	if (win->_flags & RGFW_WINDOW_INIT) cmpFlags = 0;

	#ifndef RGFW_NO_MONITOR
	if (flags & RGFW_windowScaleToMonitor)			RGFW_window_scaleToMonitor(win);
	#endif

	if (flags & RGFW_windowCenter)					RGFW_window_center(win);
	if (flags & RGFW_windowCenterCursor)
		RGFW_window_moveMouse(win, RGFW_POINT(win->r.w / 2, win->r.h / 2));
	if (flags & RGFW_windowNoBorder)				RGFW_window_setBorder(win, 0);
	else RGFW_window_setBorder(win, 1);
	if (flags & RGFW_windowFullscreen)				RGFW_window_setFullscreen(win, RGFW_TRUE);
	else if (cmpFlags & RGFW_windowFullscreen) 	RGFW_window_setFullscreen(win, 0);
	if (flags & RGFW_windowMaximize)				RGFW_window_maximize(win);
	else if (cmpFlags & RGFW_windowMaximize) 	RGFW_window_restore(win);
	if (flags & RGFW_windowMinimize)				RGFW_window_minimize(win);
	else if (cmpFlags & RGFW_windowMinimize) 	RGFW_window_restore(win);
	if (flags & RGFW_windowHideMouse)				RGFW_window_showMouse(win, 0);
	else if (cmpFlags & RGFW_windowHideMouse)  	RGFW_window_showMouse(win, 1);
	if (flags & RGFW_windowHide)				RGFW_window_hide(win);
	else if (cmpFlags & RGFW_windowHide)  		RGFW_window_show(win);
	if (flags & RGFW_windowCocoaCHDirToRes)			RGFW_moveToMacOSResourceDir();
	if (flags & RGFW_windowFloating)				RGFW_window_setFloating(win, 1);
	else if (cmpFlags & RGFW_windowFloating)		RGFW_window_setFloating(win, 0);
	if (flags & RGFW_windowFocus)					RGFW_window_focus(win);

	if (flags & RGFW_windowNoResize) {
	    RGFW_window_setMaxSize(win, RGFW_AREA(win->r.w, win->r.h));
	    RGFW_window_setMinSize(win, RGFW_AREA(win->r.w, win->r.h));
	} else if (cmpFlags & RGFW_windowNoResize) {
		RGFW_window_setMaxSize(win, RGFW_AREA(0, 0));
		RGFW_window_setMinSize(win, RGFW_AREA(0, 0));
	}

	win->_flags = flags | (win->_flags & RGFW_INTERNAL_FLAGS);
}

RGFW_bool RGFW_window_opengl_isSoftware(RGFW_window* win) {
    return RGFW_BOOL(win->_flags |= RGFW_windowOpenglSoftware);
}

RGFW_bool RGFW_window_isInFocus(RGFW_window* win) {
#ifdef RGFW_WASM
    return RGFW_TRUE;
#else
    return RGFW_BOOL(win->_flags & RGFW_windowFocus);
#endif
}

void RGFW_window_initBuffer(RGFW_window* win) {
    RGFW_area area = RGFW_getScreenSize();
    if ((win->_flags & RGFW_windowNoResize))
        area = RGFW_AREA(win->r.w, win->r.h);

    RGFW_window_initBufferSize(win, area);
}

void RGFW_window_initBufferSize(RGFW_window* win, RGFW_area area) {
#if defined(RGFW_BUFFER)
    win->_flags |= RGFW_BUFFER_ALLOC;
	#ifndef RGFW_WINDOWS
        u8* buffer = (u8*)RGFW_ALLOC(area.w * area.h * 4);
        RGFW_ASSERT(buffer != NULL);

        RGFW_window_initBufferPtr(win, buffer, area);
    #else /* windows's bitmap allocs memory for us */
	RGFW_window_initBufferPtr(win, (u8*)NULL, area);
	#endif
#else
    RGFW_UNUSED(win); RGFW_UNUSED(area);
#endif
}

RGFW_bool RGFW_isPressed(u8 controller, RGFW_button button) {
	RGFW_ASSERT(controller < RGFW_MAX_CONTROLLER_DEVICES);
	RGFW_ASSERT(button >= 0 && button < RGFW_MAX_BUTTON_COUNT);
	return _RGFW->controllerButtons[controller][button].current;
}

RGFW_bool RGFW_wasPressed(u8 controller, RGFW_button button) {
	RGFW_ASSERT(controller < RGFW_MAX_CONTROLLER_DEVICES);
	RGFW_ASSERT(button >= 0 && button < RGFW_MAX_BUTTON_COUNT);
	return _RGFW->controllerButtons[controller][button].prev;
}

RGFW_bool RGFW_isHeld(u8 controller, RGFW_button button) {
	return RGFW_isPressed(controller, button) && RGFW_wasPressed(controller, button);
}

RGFW_bool RGFW_isReleased(u8 controller, RGFW_button button) {
	return !RGFW_isPressed(controller, button) && RGFW_wasPressed(controller, button);
}

ssize_t RGFW_controllerGetCount(void) {
	return _RGFW->controllerCount;
}



RGFW_point RGFW_window_getMousePoint(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->_lastMousePoint;
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

RGFWDEF void RGFW_setBit(u32* data, u32 bit, RGFW_bool value);
void RGFW_setBit(u32* data, u32 bit, RGFW_bool value) {
	if (value)
		*data |= bit;
	else if (!value && (*(data) & bit))
		*data ^= bit;
}

void RGFW_window_center(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	RGFW_area screenR = RGFW_getScreenSize();
	RGFW_window_move(win, RGFW_POINT((i32)(screenR.w - (u32)win->r.w) / 2, (screenR.h - (u32)win->r.h) / 2));
}

RGFW_bool RGFW_monitor_scaleToWindow(RGFW_monitor mon, RGFW_window* win) {
	RGFW_monitorMode mode;
    RGFW_ASSERT(win != NULL);

	mode.area.w = (u32)win->r.w;
	mode.area.h = (u32)win->r.h;
	return RGFW_monitor_requestMode(mon, mode, RGFW_monitorScale);
}

void RGFW_splitBPP(u32 bpp, RGFW_monitorMode* mode);
void RGFW_splitBPP(u32 bpp, RGFW_monitorMode* mode) {
    if (bpp == 32) bpp = 24;
    mode->red = mode->green = mode->blue = (u8)(bpp / 3);

    u32 delta = bpp - (mode->red * 3); /* handle leftovers */
    if (delta >= 1) mode->green = mode->green + 1;
    if (delta == 2) mode->red = mode->red + 1;
}

RGFW_bool RGFW_monitorModeCompare(RGFW_monitorMode mon, RGFW_monitorMode mon2, RGFW_modeRequest request) {
	return (((mon.area.w == mon2.area.w && mon.area.h == mon2.area.h) || !(request & RGFW_monitorScale)) &&
			((mon.refreshRate == mon2.refreshRate) || !(request & RGFW_monitorRefresh)) &&
			((mon.red == mon2.red && mon.green == mon2.green && mon.blue == mon2.blue) || !(request & RGFW_monitorRGB)));
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

#ifndef RGFW_NO_MONITOR
void RGFW_window_scaleToMonitor(RGFW_window* win) {
	RGFW_monitor monitor = RGFW_window_getMonitor(win);
	if (monitor.scaleX == 0 && monitor.scaleY == 0)
		return;

	RGFW_window_resize(win, RGFW_AREA((u32)(monitor.scaleX * (float)win->r.w), (u32)(monitor.scaleY * (float)win->r.h)));
}

void RGFW_window_moveToMonitor(RGFW_window* win, RGFW_monitor m) {
	RGFW_window_move(win, RGFW_POINT(m.x, m.y));
}
#endif

RGFW_bool RGFW_window_setIcon(RGFW_window* win, u8* icon, RGFW_area a, i32 channels) {
	return RGFW_window_setIconEx(win, icon, a, channels, RGFW_iconBoth);
}

RGFWDEF void RGFW_captureCursor(RGFW_window* win, RGFW_area);
RGFWDEF void RGFW_releaseCursor(RGFW_window* win);


RGFW_bool RGFW_window_mouseHeld(RGFW_window* win) { return RGFW_BOOL(win->_flags & RGFW_HOLD_MOUSE); }

void RGFW_window_mouseHold(RGFW_window* win, RGFW_area area) {
	if (!area.w && !area.h)
		area = RGFW_AREA(win->r.w / 2, win->r.h / 2);

	win->_flags |= RGFW_HOLD_MOUSE;
    RGFW_captureCursor(win, win->r);
	RGFW_window_moveMouse(win, RGFW_POINT(win->r.w / 2, win->r.h / 2));
}

void RGFW_window_mouseUnhold(RGFW_window* win) {
	win->_flags &= ~(u32)RGFW_HOLD_MOUSE;
	RGFW_releaseCursor(win);
}

u32 RGFW_checkFPS(double startTime, u32 frameCount, u32 fpsCap) {
	double deltaTime = RGFW_getTime() - startTime;
	if (deltaTime == 0) return 0;

	double fps = (frameCount / deltaTime); /* the numer of frames over the time it took for them to render */
	if (fpsCap && fps > fpsCap) {
		double frameTime = (double)frameCount / (double)fpsCap; /* how long it should take to finish the frames */
		double sleepTime = frameTime - deltaTime; /* subtract how long it should have taken with how long it did take */

		if (sleepTime > 0) RGFW_sleep((u32)(sleepTime * 1000));
	}

	return (u32) fps;
}

#if defined(RGFW_BUFFER)
void RGFW_RGB_to_BGR(RGFW_window* win, u8* data) {
	#if !defined(RGFW_BUFFER_BGR)
	u32 x, y;
	for (y = 0; y < (u32)win->r.h; y++) {
		for (x = 0; x < (u32)win->r.w; x++) {
			u32 index = (y * 4 * win->bufferSize.w) + x * 4;

			u8 red = data[index];
			data[index] = win->buffer[index + 2];
			data[index + 2] = red;
		}
	}
    #else
	RGFW_UNUSED(win); RGFW_UNUSED(data);
	#endif
}
#endif

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

RGFWDEF void RGFW_window_showMouseFlags(RGFW_window* win, RGFW_bool show);
void RGFW_window_showMouseFlags(RGFW_window* win, RGFW_bool show) {
	if (show && (win->_flags & RGFW_windowHideMouse))
		win->_flags ^= RGFW_windowHideMouse;
	else if (!show && !(win->_flags & RGFW_windowHideMouse))
		win->_flags |= RGFW_windowHideMouse;
}

RGFW_bool RGFW_window_mouseHidden(RGFW_window* win) {
	return (RGFW_bool)RGFW_BOOL(win->_flags & RGFW_windowHideMouse);
}

RGFW_bool RGFW_window_borderless(RGFW_window* win) {
	return (RGFW_bool)RGFW_BOOL(win->_flags & RGFW_windowNoBorder);
}

RGFW_bool RGFW_window_isFullscreen(RGFW_window* win){ return RGFW_BOOL(win->_flags & RGFW_windowFullscreen); }
RGFW_bool RGFW_window_allowsDND(RGFW_window* win) { return RGFW_BOOL(win->_flags & RGFW_windowAllowDND); }

/*
	graphics API specific code (end of generic code)
	starts here
*/


/*
	OpenGL defines start here   (Normal, EGL, OSMesa)
*/

#if defined(RGFW_OPENGL) || defined(RGFW_EGL)


/* EGL, normal OpenGL only */
#ifndef RGFW_EGL
i32 RGFW_GL_HINTS[RGFW_glFinalHint] = {8,
#else
i32 RGFW_GL_HINTS[RGFW_glFinalHint] = {0,
#endif
	0, 0, 0, 1, 8, 8, 8, 8, 24, 0, 0, 0, 0, 0, 0, 0, 0, RGFW_glReleaseNone, RGFW_glCore, 0, 0};

void RGFW_setGLHint(RGFW_glHints hint, i32 value) {
	if (hint < RGFW_glFinalHint && hint) RGFW_GL_HINTS[hint] = value;
}

RGFW_bool RGFW_extensionSupportedStr(const char* extensions, const char* ext, size_t len) {
    const char *start = extensions;
    const char *where;
    const char* terminator;

    if (extensions == NULL || ext == NULL)
        return RGFW_FALSE;

    where = RGFW_STRSTR(extensions, ext);
    while (where) {
        terminator = where + len;
        if ((where == start || *(where - 1) == ' ') &&
            (*terminator == ' ' || *terminator == '\0')) {
            return RGFW_TRUE;
        }
        where = RGFW_STRSTR(terminator, ext);
    }

    return RGFW_FALSE;
}

RGFW_bool RGFW_extensionSupported(const char* extension, size_t len) {
    #ifdef GL_NUM_EXTENSIONS
    if (RGFW_GL_HINTS[RGFW_glMajor] >= 3) {
        i32 i;
        GLint count = 0;

        RGFW_proc RGFW_glGetStringi = RGFW_getProcAddress("glGetStringi");
        RGFW_proc RGFW_glGetIntegerv = RGFW_getProcAddress("RGFW_glGetIntegerv");
        if (RGFW_glGetIntegerv)
            ((void(*)(GLenum, GLint*))RGFW_glGetIntegerv)(GL_NUM_EXTENSIONS, &count);

        for (i = 0; RGFW_glGetStringi && i < count;  i++) {
            const char* en = ((const char* (*)(u32, u32))RGFW_glGetStringi)(GL_EXTENSIONS, (u32)i);
            if (en && RGFW_STRNCMP(en, extension, len) == 0)
                return RGFW_TRUE;
        }
    } else
#endif
    {
        RGFW_proc RGFW_glGetString = RGFW_getProcAddress("glGetString");

        if (RGFW_glGetString) {
            const char* extensions = ((const char*(*)(u32))RGFW_glGetString)(GL_EXTENSIONS);
            if ((extensions != NULL) && RGFW_extensionSupportedStr(extensions, extension, len))
                return RGFW_TRUE;
        }
    }

    return RGFW_extensionSupportedPlatform(extension, len);
}

/* OPENGL normal only (no EGL / OSMesa) */
#if defined(RGFW_OPENGL) && !defined(RGFW_EGL) && !defined(RGFW_CUSTOM_BACKEND) && !defined(RGFW_WASM)

/*  The window'ing api needs to know how to render the data we (or opengl) give it
	MacOS and Windows do this using a structure called a "pixel format"
	X11 calls it a "Visual"
	This function returns the attributes for the format we want */
i32* RGFW_initFormatAttribs(void);
i32* RGFW_initFormatAttribs(void) {
	static i32 attribs[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

	size_t index = (sizeof(attribs) / sizeof(attribs[0])) - 27;

	#define RGFW_GL_ADD_ATTRIB(attrib, attVal) \
		if (attVal) { \
			attribs[index] = attrib;\
			attribs[index + 1] = attVal;\
			index += 2;\
		}

        RGFW_GL_ADD_ATTRIB(RGFW_GL_DOUBLEBUFFER, 1);

		RGFW_GL_ADD_ATTRIB(RGFW_GL_ALPHA_SIZE, RGFW_GL_HINTS[RGFW_glAlpha]);
		RGFW_GL_ADD_ATTRIB(RGFW_GL_DEPTH_SIZE, RGFW_GL_HINTS[RGFW_glDepth]);
        RGFW_GL_ADD_ATTRIB(RGFW_GL_STENCIL_SIZE, RGFW_GL_HINTS[RGFW_glStencil]);
		RGFW_GL_ADD_ATTRIB(RGFW_GL_STEREO, RGFW_GL_HINTS[RGFW_glStereo]);
		RGFW_GL_ADD_ATTRIB(RGFW_GL_AUX_BUFFERS, RGFW_GL_HINTS[RGFW_glAuxBuffers]);

		i32 accumSize = (i32)(RGFW_GL_HINTS[RGFW_glAccumRed] + RGFW_GL_HINTS[RGFW_glAccumGreen] +  RGFW_GL_HINTS[RGFW_glAccumBlue] + RGFW_GL_HINTS[RGFW_glAccumAlpha]) / 4;
		RGFW_GL_ADD_ATTRIB(14, accumSize);

	RGFW_GL_ADD_ATTRIB(0, 0);

	return attribs;
}

/* EGL only (no OSMesa nor normal OPENGL) */
#elif defined(RGFW_EGL)

#include <EGL/egl.h>

#if defined(RGFW_LINK_EGL)
	typedef EGLBoolean(EGLAPIENTRY* PFN_eglInitialize)(EGLDisplay, EGLint*, EGLint*);

	PFNEGLINITIALIZEPROC eglInitializeSource;
	PFNEGLGETCONFIGSPROC eglGetConfigsSource;
	PFNEGLCHOOSECONFIcontrollerROC eglChooseConfigSource;
	PFNEGLCREATEWINDOWSURFACEPROC eglCreateWindowSurfaceSource;
	PFNEGLCREATECONTEXTPROC eglCreateContextSource;
	PFNEGLMAKECURRENTPROC eglMakeCurrentSource;
	PFNEGLGETDISPLAYPROC eglGetDisplaySource;
	PFNEGLSWAPBUFFERSPROC eglSwapBuffersSource;
	PFNEGLSWAPINTERVALPROC eglSwapIntervalSource;
	PFNEGLBINDAPIPROC eglBindAPISource;
	PFNEGLDESTROYCONTEXTPROC eglDestroyContextSource;
	PFNEGLTERMINATEPROC eglTerminateSource;
	PFNEGLDESTROYSURFACEPROC eglDestroySurfaceSource;

	#define eglInitialize eglInitializeSource
	#define eglGetConfigs eglGetConfigsSource
	#define eglChooseConfig eglChooseConfigSource
	#define eglCreateWindowSurface eglCreateWindowSurfaceSource
	#define eglCreateContext eglCreateContextSource
	#define eglMakeCurrent eglMakeCurrentSource
	#define eglGetDisplay eglGetDisplaySource
	#define eglSwapBuffers eglSwapBuffersSource
	#define eglSwapInterval eglSwapIntervalSource
	#define eglBindAPI eglBindAPISource
	#define eglDestroyContext eglDestroyContextSource
	#define eglTerminate eglTerminateSource
	#define eglDestroySurface eglDestroySurfaceSource;
#endif


#define EGL_SURFACE_MAJOR_VERSION_KHR 0x3098
#define EGL_SURFACE_MINOR_VERSION_KHR 0x30fb

#ifndef RGFW_GL_ADD_ATTRIB
#define RGFW_GL_ADD_ATTRIB(attrib, attVal) \
	if (attVal) { \
		attribs[index] = attrib;\
		attribs[index + 1] = attVal;\
		index += 2;\
	}
#endif


void RGFW_window_initOpenGL(RGFW_window* win) {
	EGLint major, minor;

	eglInitialize(win->src.EGL_display, &major, &minor);

	#ifndef EGL_OPENGL_ES1_BIT
	#define EGL_OPENGL_ES1_BIT 0x1
	#endif

	EGLint egl_config[24] = {
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,
		#ifdef RGFW_OPENGL_ES1
		EGL_OPENGL_ES1_BIT,
		#elif defined(RGFW_OPENGL_ES3)
		EGL_OPENGL_ES3_BIT,
		#elif defined(RGFW_OPENGL_ES2)
		EGL_OPENGL_ES2_BIT,
		#else
		EGL_OPENGL_BIT,
		#endif
		EGL_NONE, EGL_NONE
	};

	{
		size_t index = 7;
		EGLint* attribs = egl_config;

		RGFW_GL_ADD_ATTRIB(EGL_RED_SIZE, RGFW_GL_HINTS[RGFW_glRed]);
		RGFW_GL_ADD_ATTRIB(EGL_GREEN_SIZE, RGFW_GL_HINTS[RGFW_glBlue]);
		RGFW_GL_ADD_ATTRIB(EGL_BLUE_SIZE, RGFW_GL_HINTS[RGFW_glGreen]);
		RGFW_GL_ADD_ATTRIB(EGL_ALPHA_SIZE, RGFW_GL_HINTS[RGFW_glAlpha]);
		RGFW_GL_ADD_ATTRIB(EGL_DEPTH_SIZE, RGFW_GL_HINTS[RGFW_glDepth]);

		if (RGFW_GL_HINTS[RGFW_glSRGB])
			RGFW_GL_ADD_ATTRIB(0x3089, RGFW_GL_HINTS[RGFW_glSRGB]);

		RGFW_GL_ADD_ATTRIB(EGL_NONE, EGL_NONE);
	}

	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(win->src.EGL_display, egl_config, &config, 1, &numConfigs);

	#if defined(RGFW_MACOS)
		void* layer = RGFW_cocoaGetLayer();

		RGFW_window_cocoaSetLayer(win, layer);

		win->src.EGL_surface = eglCreateWindowSurface(win->src.EGL_display, config, (EGLNativeWindowType) layer, NULL);
	#elif defined(RGFW_WINDOWS)
		win->src.EGL_surface = eglCreateWindowSurface(win->src.EGL_display, config, (EGLNativeWindowType) win->src.window, NULL);
	#elif defined(RGFW_WAYLAND)
		if (_RGFW->useWaylandBool)
			win->src.EGL_surface = eglCreateWindowSurface(win->src.EGL_display, config, (EGLNativeWindowType) win->src.eglWindow, NULL);
		else
    #endif
    #ifdef RGFW_X11
            win->src.EGL_surface = eglCreateWindowSurface(win->src.EGL_display, config, (EGLNativeWindowType) win->src.window, NULL);
    #else
    {}
    #endif
    #if !defined(RGFW_X11) && !defined(RGFW_WAYLAND) && !defined(RGFW_MACOS)
		win->src.EGL_surface = eglCreateWindowSurface(win->src.EGL_display, config, (EGLNativeWindowType) win->src.window, NULL);
	#endif

	EGLint attribs[12];
	size_t index = 0;

#ifdef RGFW_OPENGL_ES1
    RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_CLIENT_VERSION, 1);
#elif defined(RGFW_OPENGL_ES2)
    RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_CLIENT_VERSION, 2);
#elif defined(RGFW_OPENGL_ES3)
    RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_CLIENT_VERSION, 3);
#endif

    RGFW_GL_ADD_ATTRIB(EGL_STENCIL_SIZE, RGFW_GL_HINTS[RGFW_glStencil]);
	RGFW_GL_ADD_ATTRIB(EGL_SAMPLES, RGFW_GL_HINTS[RGFW_glSamples]);

    if (RGFW_GL_HINTS[RGFW_glDoubleBuffer] == 0)
		RGFW_GL_ADD_ATTRIB(EGL_RENDER_BUFFER, EGL_SINGLE_BUFFER);

	if (RGFW_GL_HINTS[RGFW_glMajor]) {
		RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_MAJOR_VERSION, RGFW_GL_HINTS[RGFW_glMajor]);
		RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_MINOR_VERSION, RGFW_GL_HINTS[RGFW_glMinor]);

		if (RGFW_GL_HINTS[RGFW_glProfile] == RGFW_glCore) {
			RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT);
		}
		else {
			RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT);
		}
	}

	RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_OPENGL_ROBUST_ACCESS, RGFW_GL_HINTS[RGFW_glRobustness]);
	RGFW_GL_ADD_ATTRIB(EGL_CONTEXT_OPENGL_DEBUG, RGFW_GL_HINTS[RGFW_glDebug]);
	if (RGFW_GL_HINTS[RGFW_glReleaseBehavior] == RGFW_releaseFlush) {
		RGFW_GL_ADD_ATTRIB(0x2097, 0x2098);
	} else {
		RGFW_GL_ADD_ATTRIB(0x2096, 0x0000);
	}

	RGFW_GL_ADD_ATTRIB(EGL_NONE, EGL_NONE);

	#if defined(RGFW_OPENGL_ES1) || defined(RGFW_OPENGL_ES2) || defined(RGFW_OPENGL_ES3)
	eglBindAPI(EGL_OPENGL_ES_API);
	#else
	eglBindAPI(EGL_OPENGL_API);
	#endif

	win->src.EGL_context = eglCreateContext(win->src.EGL_display, config, EGL_NO_CONTEXT, attribs);

	if (win->src.EGL_context == NULL) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errEGLContext, RGFW_DEBUG_CTX(win, 0), "failed to create an EGL opengl context");
		return;
	}

	eglMakeCurrent(win->src.EGL_display, win->src.EGL_surface, win->src.EGL_surface, win->src.EGL_context);
	eglSwapBuffers(win->src.EGL_display, win->src.EGL_surface);
	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoOpenGL, RGFW_DEBUG_CTX(win, 0), "EGL opengl context initalized");
}

void RGFW_window_freeOpenGL(RGFW_window* win) {
	if (win->src.EGL_display == NULL) return;

	eglDestroySurface(win->src.EGL_display, win->src.EGL_surface);
	eglDestroyContext(win->src.EGL_display, win->src.EGL_context);
	eglTerminate(win->src.EGL_display);
  win->src.EGL_display = NULL;
  #ifdef RGFW_WAYLAND
		wl_egl_window_destroy(win->src.eglWindow);
		RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoOpenGL, RGFW_DEBUG_CTX(win, 0), "EGL window context freed");
  #endif
	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoOpenGL, RGFW_DEBUG_CTX(win, 0), "EGL opengl context freed");
}

void RGFW_window_makeCurrent_OpenGL(RGFW_window* win) {
    if (win == NULL)
        eglMakeCurrent(_RGFW->root->src.EGL_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    else {
        eglMakeCurrent(win->src.EGL_display, win->src.EGL_surface, win->src.EGL_surface, win->src.EGL_context);
    }
}

void RGFW_window_swapBuffers_OpenGL(RGFW_window* win) { eglSwapBuffers(win->src.EGL_display, win->src.EGL_surface); }

void* RGFW_getCurrent_OpenGL(void) { return eglGetCurrentContext(); }

#if defined(RGFW_WINDOWS)
HMODULE RGFW_wgl_dll = NULL;
#endif

RGFW_proc RGFW_getProcAddress(const char* procname) {
	#if defined(RGFW_WINDOWS)
	RGFW_proc proc = (RGFW_proc) GetProcAddress(RGFW_wgl_dll, procname);

		if (proc)
			return proc;
	#endif

	return (RGFW_proc) eglGetProcAddress(procname);
}

RGFW_bool RGFW_extensionSupportedPlatform(const char* extension, size_t len) {
    const char* extensions = eglQueryString(_RGFW->root->src.EGL_display, EGL_EXTENSIONS);
    return extensions != NULL && RGFW_extensionSupportedStr(extensions, extension, len);
}

void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval) {
	RGFW_ASSERT(win != NULL);

	eglSwapInterval(win->src.EGL_display, swapInterval);

}

#endif /* RGFW_EGL */

/*
	end of RGFW_EGL defines
*/
#endif /* end of RGFW_GL (OpenGL, EGL, OSMesa )*/

/*
	RGFW_VULKAN defines
*/
#ifdef RGFW_VULKAN
#ifdef RGFW_MACOS
#include <objc/message.h>
#endif

const char** RGFW_getVKRequiredInstanceExtensions(size_t* count) {
    static const char* arr[2] = {VK_KHR_SURFACE_EXTENSION_NAME};
    arr[1] = RGFW_VK_SURFACE;
    if (count != NULL) *count = 2;

    return (const char**)arr;
}

VkResult RGFW_window_createVKSurface(RGFW_window* win, VkInstance instance, VkSurfaceKHR* surface) {
    RGFW_ASSERT(win != NULL); RGFW_ASSERT(instance);
	RGFW_ASSERT(surface != NULL);

    *surface = VK_NULL_HANDLE;

#ifdef RGFW_X11
    RGFW_GOTO_WAYLAND(0);
    VkXlibSurfaceCreateInfoKHR x11 = { VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR, 0, 0, (Display*) win->src.display, (Window) win->src.window };
    return vkCreateXlibSurfaceKHR(instance, &x11, NULL, surface);
#endif
#if defined(RGFW_WAYLAND)
RGFW_WAYLAND_LABEL
    VkWaylandSurfaceCreateInfoKHR wayland = { VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR, 0, 0, (struct wl_display*) win->src.wl_display, (struct wl_surface*) win->src.surface };
    return vkCreateWaylandSurfaceKHR(instance, &wayland, NULL, surface);
#elif defined(RGFW_WINDOWS)
    VkWin32SurfaceCreateInfoKHR win32 = { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, 0, 0, GetModuleHandle(NULL), (HWND)win->src.window };

    return vkCreateWin32SurfaceKHR(instance, &win32, NULL, surface);
#elif defined(RGFW_MACOS) && !defined(RGFW_MACOS_X11)
    void* contentView = ((void* (*)(id, SEL))objc_msgSend)((id)win->src.window, sel_getUid("contentView"));
    VkMacOSSurfaceCreateFlagsMVK macos = { VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK, 0, 0, win->src.display, (void*)contentView };

    return vkCreateMacOSSurfaceMVK(instance, &macos, NULL, surface);
#endif
}


RGFW_bool RGFW_getVKPresentationSupport(VkInstance instance, VkPhysicalDevice physicalDevice, u32 queueFamilyIndex) {
    RGFW_ASSERT(instance);
	if (_RGFW == NULL) RGFW_init();
#ifdef RGFW_X11
    RGFW_GOTO_WAYLAND(0);
	Visual* visual = DefaultVisual(_RGFW->display, DefaultScreen(_RGFW->display));
    if (_RGFW->root)
        visual = _RGFW->root->src.visual.visual;

    RGFW_bool out = vkGetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, _RGFW->display, XVisualIDFromVisual(visual));
    return out;
#endif
#if defined(RGFW_WAYLAND)
RGFW_WAYLAND_LABEL
    RGFW_bool wlout = vkGetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, _RGFW->wl_display);
    return wlout;
#elif defined(RGFW_WINDOWS)
#elif defined(RGFW_MACOS) && !defined(RGFW_MACOS_X11)
    return RGFW_FALSE; /* TODO */
#endif
}
#endif /* end of RGFW_vulkan */

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

const char* RGFW_axisName(i32 axis) {
	RGFW_ASSERT(axis >= 0 && axis < RGFW_MAX_AXIS_COUNT);

	static const char* NAME_LUT[RGFW_MAX_AXIS_COUNT] = {
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

void RGFW_window_initBufferPtr(RGFW_window* win, u8* buffer, RGFW_area area) {

#if defined(RGFW_BUFFER)
	win->buffer = (u8*)buffer;
	win->bufferSize = area;

	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoBuffer, RGFW_DEBUG_CTX(win, 0), "createing a 4 channel buffer");

	RGFW_GOTO_WAYLAND(0);
    #ifdef RGFW_X11
		win->src.bitmap = XCreateImage(
			win->src.display, win->src.visual.visual, (u32)win->src.visual.depth,
			ZPixmap, 0, NULL, area.w, area.h, 32, 0
		);
	#endif
	#ifdef RGFW_WAYLAND
		RGFW_WAYLAND_LABEL {}
		u32 size = (u32)(area.w * area.h * 4);
		int fd = RGFW_wl_create_shm_file(size);
		if (fd < 0) {
			RGFW_sendDebugInfo(RGFW_typeError, RGFW_errBuffer, RGFW_DEBUG_CTX(win, (u32)fd),"Failed to create a buffer.");
			exit(1);
        }

		win->src.buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (win->src.buffer == MAP_FAILED) {
			RGFW_sendDebugInfo(RGFW_typeError, RGFW_errBuffer, RGFW_DEBUG_CTX(win, 0), "mmap failed!");
			close(fd);
			exit(1);
		}

		win->_flags |= RGFW_BUFFER_ALLOC;

		struct wl_shm_pool* pool = wl_shm_create_pool(win->src.shm, fd, (i32)size);
		win->src.wl_buffer = wl_shm_pool_create_buffer(pool, 0, win->r.w, win->r.h, win->r.w * 4,
			WL_SHM_FORMAT_ARGB8888);
		wl_shm_pool_destroy(pool);

		close(fd);

		wl_surface_attach(win->src.surface, win->src.wl_buffer, 0, 0);
		wl_surface_commit(win->src.surface);

		u8 color[] = {0x00, 0x00, 0x00, 0xFF};

		size_t i;
		for (i = 0; i < area.w * area.h * 4; i += 4) {
			RGFW_MEMCPY(&win->buffer[i], color, 4);
		}
		RGFW_MEMCPY(win->src.buffer, win->buffer, (size_t)(win->r.w * win->r.h * 4));
	#endif
#else
	RGFW_UNUSED(win); RGFW_UNUSED(buffer); RGFW_UNUSED(area);
#endif
}

void RGFW_window_setBorder(RGFW_window* win, RGFW_bool border) {
	RGFW_setBit(&win->_flags, RGFW_windowNoBorder, !border);
}

void RGFW_releaseCursor(RGFW_window* win) {
}

void RGFW_captureCursor(RGFW_window* win, RGFW_area r) {
}

#ifndef RGFW_EGL
void RGFW_window_initOpenGL(RGFW_window* win) {
}

void RGFW_window_freeOpenGL(RGFW_window* win) {
}
#endif


i32 RGFW_initPlatform(void) {
	_RGFW->controllerCount = 1;

    return 0;
}

RGFW_window* RGFW_createWindowPtr(RGFW_area area, RGFW_windowFlags flags, RGFW_window* win) {
	RGFW_window_basic_init(win, area, flags);

	return win;
}

RGFW_area RGFW_getScreenSize(void) {
	RGFW_init();
}

RGFW_point RGFW_getGlobalMousePoint(void) {
	RGFW_init();
}


RGFW_event* RGFW_window_checkEvent(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	if ((win->_flags & RGFW_windowFreeOnClose) && (win->_flags & RGFW_EVENT_QUIT)) {
		return NULL;
	}

	RGFW_event* ev = RGFW_window_checkEventCore(win);
	if (ev) {
		return ev;
	}
	ev = &win->event;

	/* TODO(EimaMei): devkitPro actually gives you access to 'hidSharedMem' and
	 * 'hidMemHandle', allowing you to manage input yourself. Might have to take
	 * a better look at it. */
	if (_RGFW->check == false) {
		hidScanInput();
		hidCircleRead(&_RGFW->cpad);
		_RGFW->pressed = hidKeysHeld();
		_RGFW->released = hidKeysUp();
		_RGFW->check = true;

		if (!aptMainLoop()) {
			ev->type = RGFW_quit;
			return ev;
		}
	}

	if (_RGFW->pressed & (RGFW_CPAD_BITS_H | RGFW_CPAD_BITS_V)) {
		ev->type = RGFW_controllerAxisMove;
		ev->input_index = 0;

		i32 value;
		u32 bits = _RGFW->pressed;

		if (bits & RGFW_CPAD_BITS_H) {
			bits &= RGFW_CPAD_BITS_H;
			value = _RGFW->cpad.dx;
			ev->whichAxis = 0;
		}
		else {
			bits &= RGFW_CPAD_BITS_V;
			value = _RGFW->cpad.dy;
			ev->whichAxis = 1;
		}

		/* NOTE(EimaMei): C-Pad is from -156 to 156. */
		ev->axis.value    = (i16)(value * INT16_MAX / 156);
		/* NOTE(EimaMei): I picked '40' as the deadzone based on how the CPAD bits
		 * are set if the value is equal or larger than 41. (http://3dbrew.org/wiki/HID_Shared_Memory). */
		ev->axis.deadzone = (40 * INT16_MAX / 156);
		_RGFW->controllerAxes[0][ev->whichAxis] = ev->axis;

		_RGFW->pressed &= ~bits;
		RGFW_controllerAxisCallback(win, 0, ev->whichAxis);
		return ev;
	}

	if (_RGFW->pressed & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(_RGFW->pressed);
		RGFW_ASSERT(button != -1);
		u32 ogButton = RGFW_rgfwToApiKey(button);

		ev->type = RGFW_buttonPressed;
		ev->input_index = 0;
		ev->button = button;

		RGFW_keyState* state = &_RGFW->controllerButtons[0][ev->button];
		ev->repeat = (state->prev == true);
		state->prev = state->current;
		state->current = true;

		_RGFW->pressed &= ~ogButton;
		RGFW_buttonCallback(win, 0, ev->button, true);
		return ev;
	}

	if (_RGFW->released & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(_RGFW->released);

		ev->type        = RGFW_buttonReleased;
		ev->input_index = 0;
		ev->button      = (u8)button;
		ev->repeat      = false;

		RGFW_keyState* state = &_RGFW->controllerButtons[0][ev->button];
		state->prev = state->current;
		state->current = false;

		_RGFW->released &= ~RGFW_rgfwToApiKey(button);
		RGFW_buttonCallback(win, 0, ev->button, false);
		return ev;
	}

	if (_RGFW->pressed & KEY_TOUCH) {
		touchPosition touch;
		hidTouchRead(&touch);

		ev->type        = RGFW_pointerMove;
		ev->input_index = 0;
		ev->point       = RGFW_POINT(touch.px, touch.py);

		_RGFW->pressed &= (u32)~KEY_TOUCH;
		RGFW_pointerMoveCallback(win, 0, ev->point);
		return ev;
	}

	_RGFW->check = false;
	return NULL;
}

void RGFW_window_move(RGFW_window* win, RGFW_point v) {
	RGFW_ASSERT(win != NULL);
	/* TODO(EimaMei): Remove this? */
}


void RGFW_window_resize(RGFW_window* win, RGFW_area a) {
	RGFW_ASSERT(win != NULL);
	win->r.w = (i32)a.w;
	win->r.h = (i32)a.h;
}

void RGFW_window_setAspectRatio(RGFW_window* win, RGFW_area a) {
	RGFW_ASSERT(win != NULL);

	if (a.w == 0 && a.h == 0)
		return;
}

void RGFW_window_setMinSize(RGFW_window* win, RGFW_area a) {
	RGFW_ASSERT(win != NULL);
}

void RGFW_window_setMaxSize(RGFW_window* win, RGFW_area a) {
	RGFW_ASSERT(win != NULL);
}


void RGFW_window_maximize(RGFW_window* win) {
	win->_oldRect = win->r;
}

void RGFW_window_focus(RGFW_window* win) {
	RGFW_ASSERT(win);
}

void RGFW_window_raise(RGFW_window* win) {
	RGFW_ASSERT(win);
}

void RGFW_window_setFullscreen(RGFW_window* win, RGFW_bool fullscreen) {
	RGFW_ASSERT(win != NULL);
    if (fullscreen) {
		win->_flags |= RGFW_windowFullscreen;
		win->_oldRect = win->r;
	}
	else win->_flags &= ~(u32)RGFW_windowFullscreen;
}

void RGFW_window_setFloating(RGFW_window* win, RGFW_bool floating) {
    RGFW_ASSERT(win != NULL);
}

void RGFW_window_setOpacity(RGFW_window* win, u8 opacity) {
	RGFW_ASSERT(win != NULL);
}

void RGFW_window_minimize(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	if (RGFW_window_isMaximized(win)) return;

	win->_oldRect = win->r;
}

void RGFW_window_restore(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	win->r = win->_oldRect;
	//RGFW_window_move(win, RGFW_POINT(win->r.x, win->r.y));
	RGFW_window_resize(win, RGFW_AREA(win->r.w, win->r.h));

    RGFW_window_show(win);
}

RGFW_bool RGFW_window_isFloating(RGFW_window* win) {
	return RGFW_FALSE;
}

void RGFW_window_setName(RGFW_window* win, const char* name) {
	RGFW_ASSERT(win != NULL);
}

#ifndef RGFW_NO_PASSTHROUGH
void RGFW_window_setMousePassthrough(RGFW_window* win, RGFW_bool passthrough) {
	RGFW_ASSERT(win != NULL);
}
#endif /* RGFW_NO_PASSTHROUGH */

RGFW_bool RGFW_window_setIconEx(RGFW_window* win, u8* icon, RGFW_area a, i32 channels, u8 type) {
	RGFW_ASSERT(win != NULL);
}

RGFW_mouse* RGFW_loadMouse(u8* icon, RGFW_area a, i32 channels) {
    RGFW_ASSERT(icon);
	RGFW_ASSERT(channels == 3 || channels == 4);

}

void RGFW_window_setMouse(RGFW_window* win, RGFW_mouse* mouse) {
}

void RGFW_freeMouse(RGFW_mouse* mouse) {
}

void RGFW_window_moveMouse(RGFW_window* win, RGFW_point p) {
}

RGFW_bool RGFW_window_setMouseDefault(RGFW_window* win) {
	return RGFW_window_setMouseStandard(win, RGFW_mouseArrow);
}

RGFW_bool RGFW_window_setMouseStandard(RGFW_window* win, u8 mouse) {
	RGFW_ASSERT(win != NULL);
}

void RGFW_window_hide(RGFW_window* win) {
}

void RGFW_window_show(RGFW_window* win) {
	win->_flags &= ~(u32)RGFW_windowHide;
	if (win->_flags & RGFW_windowFocusOnShow) RGFW_window_focus(win);
}

void RGFW_window_close(RGFW_window* win) {
	/* NOTE(EimaMei): Only do gfxExit if win is the last window. */
	printf("riley has to fix this.\n");
}

#endif

#endif /* RGFW_IMPLEMENTATION */

#if defined(__cplusplus) && !defined(__EMSCRIPTEN__)
}
#endif

#if _MSC_VER
	#pragma warning( pop )
#endif
