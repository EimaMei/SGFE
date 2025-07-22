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
	#define RGFW_EGL - (optional) use EGL for loading an OpenGL context (instead of the system's OpenGL api)
	#define RGFW_NO_API - (optional) don't use any rendering API (no OpenGL, no vulkan, no directX)

	#define RGFW_ALLOC x  - choose the default allocation function (defaults to standard malloc)
	#define RGFW_FREE x  - choose the default deallocation function (defaults to standard free)

	#define RGFW_EXPORT - use when building RGFW
	#define RGFW_IMPORT - use when linking with RGFW (not as a single-header)

	#define RGFW_USE_INT - force the use c-types rather than stdint.h (for systems that might not have stdint.h (msvc))
	#define RGFW_bool x - choose what type to use for bool, by default u32 is used
*/


/*
	=== ORIGINAL RGFW ===
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

#ifndef RGFW_UNUSED
	#define RGFW_UNUSED(x) (void)(x)
#endif

#ifndef RGFW_ALLOC
	#include <stdlib.h>
	#define RGFW_ALLOC(size) malloc(size)
	#define RGFW_FREE(ptr) free(ptr)
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

#ifndef RGFW_COUNTOF
	#define RGFW_COUNTOF(array) (ssize_t)(sizeof(array) / sizeof((array)[0]))
#endif

#ifndef RGFW_BIT
	#define RGFW_BIT(x) (1 << (x))
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

#ifdef __3DS__
	#ifndef RGFW_3DS
	#define RGFW_3DS 1
	#endif
	#ifndef RGFW_MAX_CONTROLLER_DEVICES
	#define RGFW_MAX_CONTROLLER_DEVICES 1
	#endif

	#include <3ds.h>

	#ifdef RGFW_OPENGL
	#define RIP_BACKEND RIP_BACKEND_KYGX
	#include <GLES/gl2.h>
	#endif

	/* TODO(EimaMei): comment. */
	#ifndef RGFW_ALLOC_SYS
	#define RGFW_ALLOC_SYS(size) linearAlloc((size))
	#define RGFW_FREE_SYS(ptr) linearFree(ptr)
	#endif
#endif

/*
	regular RGFW stuff
*/

typedef RGFW_ENUM(ssize_t, RGFW_screen) {
	#ifdef RGFW_3DS
	RGFW_screenTop,
	RGFW_screenBottom,
	#else
	RGFW_screenPrimary,
	#endif

	RGFW_screenCount
};

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

/*! basic 3D vector type in float-point decimals, used for motion usually. */
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
	/* Denotes if the controller is connected. */
	RGFW_bool connected;
	/* Denotes what type of controller it is. */
	RGFW_controllerType type;

	/* Current button states. */
	RGFW_buttonState buttons[RGFW_controllerButtonCount];
	/* Current axes states. */
    RGFW_axis axes[RGFW_axisTypeCount];
	/* Current pointer states. */
	RGFW_point pointers[RGFW_pointerTypeCount];
	/* Current motion states. */
	RGFW_point3D motions[RGFW_motionTypeCount];

	/* Range of valid buttons for the controller. */
	RGFW_button button_start, button_end;
	/* Boolean states of enabled pointers. */
	RGFW_bool enabled_pointers[RGFW_pointerTypeCount];
	/* Boolean states of enabled motions. */
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
} RGFW_event;


/* TODO(EimaMei): document */
typedef RGFW_ENUM(i32, RGFW_videoMode) {
#ifdef RGFW_3DS
	RGFW_videoMode2D,
	RGFW_videoMode3D,
	RGFW_videoModeWide, /* NOTE(EimaMei): Doesn't work on regular 2DS consoles. TODO(EimaMei): Add an error if you select this on 2DS consoles.*/
#endif

	RGFW_videoModeCount
};

/* TODO(EimaMei): document */
/* TODO(EimaMei): document how if native mode is turned on RGFW_pixelFormatRGBA8 might return false. */
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
	RGFW_systemModelUnknown,
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
	RGFW_bool has_checked_events, lcd_is_on;
	accelVector accel;
	angularRate gyro;

	u8* buffers[RGFW_screenCount][2];
	RGFW_pixelFormat format[RGFW_screenCount];
	ssize_t current_buffer[RGFW_screenCount];
	u32 buffer_size[RGFW_screenCount];

	RGFW_bool is_buffer_allocated[RGFW_screenCount];
	RGFW_bool is_double_buffered[RGFW_screenCount];
	RGFW_bool is_native[RGFW_screenCount];

	#ifndef RGFW_BUFFER_NO_CONVERSION
		u8* buffers_native[RGFW_screenCount][2];
	#endif

	#ifdef RGFW_OPENGL
		RGFW_bool kygx_initialized;
		GLASSCtx ctx[RGFW_screenCount];
		GLuint gl_framebuffer[RGFW_screenCount];
		GLuint gl_renderbuffer[RGFW_screenCount];
	#endif
} RGFW_window_src;

#endif

/*! Optional arguments for making a windows */
typedef RGFW_ENUM(u32, RGFW_windowFlags) {
	RGFW_windowNoInitAPI      = RGFW_BIT(0), /* do NOT init an API (including the software rendering buffer) (mostly for bindings. you can also use `#define RGFW_NO_API`) */
	RGFW_windowOpenGL         = RGFW_BIT(1), /* TODO(EimaMei): New enum. */
	RGFW_windowBuffer         = RGFW_BIT(2), /* TODO(EimaMei): New enum. */
	RGFW_windowConsoleInit    = RGFW_BIT(3), /* TODO(EimaMei): New enum. */
	RGFW_windowFreeOnClose    = RGFW_BIT(15), /*!< free (RGFW_window_close) the RGFW_window struct when the window is closed (by the end user) */

	#ifdef RGFW_3DS
	RGFW_windowTopScreen      = RGFW_BIT(16), /* TODO(EimaMei): New enum. */
	RGFW_windowBottomScreen   = RGFW_BIT(17), /* TODO(EimaMei): New enum. */
	RGFW_windowDualScreen     = RGFW_windowTopScreen | RGFW_windowBottomScreen, /* TODO(EimaMei): New enum. */
	#endif

	RGFW_windowInitAllAPIs    = RGFW_windowOpenGL | RGFW_windowBuffer
};

typedef struct RGFW_window {
	RGFW_screen current;
	RGFW_window_src src; /*!< src window data */

	u8* buffer; /*!< buffer for software rendering */
	RGFW_area bufferSize;
	RGFW_videoMode mode; /* TODO(EimaMei): document */

	/* struct RGFW_info */
	RGFW_event events[32];
	ssize_t eventLen;

	ssize_t controllerCount; /*!< the actual amount of controllers */
	RGFW_controller controllers[RGFW_MAX_CONTROLLER_DEVICES];

	RGFW_bool stopCheckEvents_bool;
	u32 _flags; /*!< windows flags (for RGFW to check) */

	void* userPtr;
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


RGFWDEF RGFW_bool RGFW_window_initBuffer(RGFW_window* win);
RGFWDEF RGFW_bool RGFW_window_initBufferFormat(RGFW_window* win, RGFW_pixelFormat format);
RGFWDEF RGFW_bool RGFW_window_initBufferNative(RGFW_window* win, RGFW_pixelFormat format);
RGFWDEF RGFW_bool RGFW_window_initBufferScreen(RGFW_window* win, RGFW_screen screen,
	u8* buffers[2], RGFW_pixelFormat format, RGFW_bool is_native);

/*!< free buffers used for software rendering within the window */
RGFWDEF void RGFW_window_freeBuffer(RGFW_window* win);
RGFWDEF void RGFW_window_freeBufferScreen(RGFW_window* win, RGFW_screen screen);

/* supports OpenGL and software rendering */
RGFWDEF void RGFW_window_swapBuffers_buffer(RGFW_window* win); /*!< swap the rendering buffer */
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_window_makeCurrent_buffer(RGFW_window* win, RGFW_screen screen);


/* TODO(EimaMei): New function. */
RGFWDEF RGFW_bool RGFW_window_bufferSetFormat(RGFW_window* win, RGFW_pixelFormat format);
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_window_bufferSetNative(RGFW_window* win, RGFW_bool is_native);
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_window_bufferSetDoubleBuffering(RGFW_window* win, RGFW_bool enable);
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_window_bufferFlipBuffers(RGFW_window* win);

/* TODO(EimaMei): New function. */
RGFWDEF RGFW_pixelFormat RGFW_window_bufferGetFormat(RGFW_window* win);
/* TODO(EimaMei): New function. */
RGFWDEF RGFW_bool RGFW_window_bufferIsNative(RGFW_window* win);
/* TODO(EimaMei): New function. */
RGFWDEF RGFW_bool RGFW_window_bufferIsDoubleBuffered(RGFW_window* win);
/* TODO(EimaMei): New function. */
RGFWDEF ssize_t RGFW_window_bufferGetCurrent(RGFW_window* win);



/*! set the window flags (will undo flags if they don't match the old ones) */
RGFWDEF void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags flags);

/* TODO(EimaMei): New function. */
RGFWDEF RGFW_area RGFW_windowGetSize(RGFW_window* win);


/*!
	this function checks an *individual* event (and updates window structure attributes)
	this means, using this function without a while loop may cause event lag

	ex.

	while (RGFW_window_checkEvent(win) != NULL) [this keeps checking events until it reaches the last one]

	this function is optional if you choose to use event callbacks,
	although you still need some way to tell RGFW to process events eg. `RGFW_window_checkEvents`
*/

RGFWDEF RGFW_bool RGFW_window_checkEvent(RGFW_window* win, const RGFW_event** ev_out);

/*!
	for RGFW_window_eventWait and RGFW_window_checkEvents
	waitMS -> Allows the function to keep checking for events even after `RGFW_window_checkEvent == NULL`
			  if waitMS == 0, the loop will not wait for events
			  if waitMS > 0, the loop will wait that many miliseconds after there are no more events until it returns
			  if waitMS == -1 or waitMS == the max size of an unsigned 32-bit int, the loop will not return until it gets another event
*/
typedef RGFW_ENUM(i32, RGFW_eventWait) {
	RGFW_eventWaitNone = 0,
	RGFW_eventWaitNext = -1
};

/*! sleep until RGFW gets an event or the timer ends (defined by OS) */
RGFWDEF void RGFW_window_eventWait(RGFW_window* win, i32 waitMS);

/*!
	check all the events until there are none left.
	Return true if the window should close, otherwise false.
*/
RGFWDEF RGFW_bool RGFW_window_checkEvents(RGFW_window* win, RGFW_eventWait waitMS);

/*!
	tell RGFW_window_eventWait to stop waiting (to be ran from another thread)
*/
RGFWDEF void RGFW_stopCheckEvents(void);

/*! window managment functions */
RGFWDEF void RGFW_window_close(RGFW_window* win); /*!< close the window and free leftover data */

/*
	makes it so `RGFW_window_shouldClose` returns true or overrides a window close
	by modifying window flags
*/
RGFWDEF void RGFW_window_setShouldClose(RGFW_window* win, RGFW_bool shouldClose);

/*! if the window should close (RGFW_close was sent or escape was pressed) */
RGFWDEF RGFW_bool RGFW_window_shouldClose(RGFW_window* win);

/* TODO(EimaMei): new function. */
RGFWDEF RGFW_bool RGFW_window_consoleInit(RGFW_window* win);


/** @} */

/** @} */

/** * @defgroup Input
* @{ */

RGFWDEF RGFW_bool RGFW_isPressed(RGFW_controller* controller, RGFW_button button); /*!< if key is pressed (key code)*/

RGFWDEF RGFW_bool RGFW_wasPressed(RGFW_controller* controller, RGFW_button button); /*!< if key was pressed (checks previous state only) (key code) */

RGFWDEF RGFW_bool RGFW_isHeld(RGFW_controller* controller, RGFW_button button); /*!< if key is held (key code) */
RGFWDEF RGFW_bool RGFW_isReleased(RGFW_controller* controller, RGFW_button button); /*!< if key is released (key code) */


/* Returns the amount of currently connected controllers.*/
RGFWDEF ssize_t RGFW_controllerGetCount(RGFW_window* win);

/* Returns the controller strucutr associated with the port. */
RGFWDEF RGFW_controller* RGFW_controllerGet(RGFW_window* win, ssize_t port);
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
	RGFW_errorNone = 0,
	RGFW_errorOutOfMemory,
	RGFW_errorSystem,
	RGFW_errorOpenGLContext,
	RGFW_errorEventQueue,
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
/** @} */


/** * @defgroup graphics_API
* @{ */

/*!< make the screen the current drawing context */
RGFWDEF void RGFW_window_makeCurrent(RGFW_window* win, RGFW_screen screen);

/*! get the screen used for the current drawing context */
RGFWDEF RGFW_screen RGFW_getCurrent(RGFW_window* win);

/* supports OpenGL and software rendering */
RGFWDEF void RGFW_window_swapBuffers(RGFW_window* win); /*!< swap the rendering buffer */
RGFWDEF void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval);

typedef void (*RGFW_proc)(void); /* function pointer equivalent of void* */

/*! native API functions */
#if defined(RGFW_OPENGL) || defined(RGFW_EGL)
/*!< create an OpenGL context for the RGFW window, run by createWindow by default (unless the RGFW_windowNoInitAPI is included) */
RGFWDEF RGFW_bool RGFW_window_initOpenGL(RGFW_window* win);
RGFWDEF RGFW_bool RGFW_window_initOpenGLScreen(RGFW_window* win, RGFW_screen screen);
/*!< called by `RGFW_window_close` by default (unless the RGFW_windowNoInitAPI is set) */
RGFWDEF void RGFW_window_freeOpenGL(RGFW_window* win);

/*! OpenGL init hints */
typedef RGFW_ENUM(i32, RGFW_glHints)  {
	RGFW_glStencil = 0,  /*!< set stencil buffer bit size (0 by default) */
	RGFW_glSamples, /*!< set number of sample buffers (0 by default) */
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

RGFWDEF void RGFW_window_makeCurrent_OpenGL(RGFW_window* win, RGFW_screen screen); /*!< to be called by RGFW_window_makeCurrent */
RGFWDEF void RGFW_window_swapBuffers_OpenGL(RGFW_window* win); /*!< swap opengl buffer (only) called by RGFW_window_swapInterval  */

RGFWDEF void* RGFW_getCurrent_OpenGL(void); /*!< get the current context (OpenGL backend (3DS))*/

#endif


/** @} */

/** * @defgroup Supporting
* @{ */


/* TODO(EimaMei): Explain the differences between Desktop and Embedded. */
/*! standard event queue, used for injecting events and returning source API callback events like any other queue check */
/* these are all used internally by RGFW */
RGFW_event* RGFW_eventQueuePush(RGFW_window* win);
const RGFW_event* RGFW_eventQueuePop(RGFW_window* win);

/* for C++ / C89 */
#define RGFW_eventQueuePushEx(eventInit) { RGFW_event e; eventInit; RGFW_eventQueuePush(e); }

/*!
	key codes and mouse icon enums
*/
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


#ifdef RGFW_3DS
/* TODO(EimaMei): Move these somewhere more appropriate. */
/* TODO(EimaMei): Explain 'RGFW_platform' functions. */

/* TODO(EimaMei): new function. | can return 0 or maybe not. */
RGFWDEF RGFW_systemModel RGFW_platformGetModel(void);
/* TODO(EimaMei): new function. */
RGFWDEF float RGFW_platformGet3DSlider(void);

#ifdef RGFW_OPENGL
/* TODO(EimaMei): new function */
RGFWDEF RGFW_bool RGFW_platform_OpenGL_rotateScreen(GLuint shader_program, const char* mat4_uniform_name);
#endif

#endif

/* TODO(EimaMei): New function. */
RGFWDEF RGFW_area RGFW_screenGetResolution(RGFW_window* win, RGFW_screen screen);


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


#ifdef RGFW_IMPLEMENTATION


/*
RGFW_IMPLEMENTATION starts with generic RGFW defines

This is the start of keycode data
*/



/*
	the c++ compiler doesn't support setting up an array like,
	we'll have to do it during runtime using a function & this messy setup
*/



/* var - VARIABLE | mask - UINT | set - RGFW_bool
 * Sets/unsets the mask for the variable. */
#define RGFW_MASK_SET(var, mask, set) do { \
	if (set) (var) |=  (mask); \
	else     (var) &= ~(mask); \
} while (0)

RGFWDEF void RGFW_resetKeyPrev(RGFW_window* win);


void RGFW_resetKeyPrev(RGFW_window* win) {
	/* TODO(EimaMei): This feels inneficient to do each frame. Must be resolved. */
	ssize_t i, j;
	for (i = 0; i < RGFW_MAX_CONTROLLER_DEVICES; i += 1) {
		RGFW_controller* controller = &win->controllers[i];
		if (!controller->connected) {
			continue;
		}

		for (j = controller->button_start; j < controller->button_end; j += 1) {
			RGFW_MASK_SET(win->controllers[i].buttons[j], RGFW_buttonStatePrevious, RGFW_FALSE);
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
		case RGFW_infoWindow: RGFW_PRINTF(" videoMode = %s\n", RGFW_videoModeStr(ctx.win->mode)); break;
		default: RGFW_PRINTF("\n");
	}
	#endif
}

RGFW_bool RGFW_window_checkEvents(RGFW_window* win, i32 waitMS) {
	RGFW_window_eventWait(win, waitMS);

	const RGFW_event* ev;
	while (RGFW_window_checkEvent(win, &ev)) {}

	return !RGFW_window_shouldClose(win);
}


/*
no more event call back defines
*/

/* TODO(EimaMei): remove these. */
#define RGFW_EVENT_QUIT 		RGFW_BIT(25) /* the window close button was pressed */
#define RGFW_WINDOW_ALLOC 		RGFW_BIT(28) /* if window was allocated by RGFW */
#define RGFW_INTERNAL_FLAGS (RGFW_EVENT_QUIT | RGFW_WINDOW_ALLOC)


RGFW_window* RGFW_createWindow(RGFW_videoMode mode, RGFW_windowFlags flags) {
	RGFW_window* win = (RGFW_window*)RGFW_ALLOC(sizeof(RGFW_window));
	if (win == NULL) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOutOfMemory, RGFW_DEBUG_CTX(NULL, 0), "Failed to allocate a window.");
		return NULL;
	}

    return RGFW_createWindowPtr(mode, flags | RGFW_WINDOW_ALLOC, win);
}

RGFWDEF RGFW_bool RGFW_create_window_platform(RGFW_window* win);

RGFW_window* RGFW_createWindowPtr(RGFW_videoMode mode, RGFW_windowFlags flags,
		RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	win->current = 0;
	win->buffer = NULL;
	win->bufferSize = RGFW_AREA(0, 0);
	win->mode = mode;
	win->eventLen = 0;
	win->stopCheckEvents_bool = RGFW_FALSE;
	win->_flags = 0;

	win->controllerCount = 0;
	RGFW_MEMSET(win->controllers, 0, sizeof(win->controllers));

	RGFW_bool res = RGFW_create_window_platform(win);
	if (res == false) { return NULL; }
	RGFW_window_setFlags(win, win->_flags | flags);

	if (win->_flags & RGFW_windowNoInitAPI) {
		return win;
	}

	switch (win->_flags & RGFW_windowInitAllAPIs) {
		case RGFW_windowBuffer: RGFW_window_initBuffer(win); break;
		#ifdef RGFW_OPENGL
		case RGFW_windowOpenGL: RGFW_window_initOpenGL(win); break;
		case RGFW_windowBuffer | RGFW_windowOpenGL:
			RGFW_window_initOpenGL(win);
			RGFW_window_initBuffer(win);
			break;
		#endif

		/* NOTE(EimaMei): No API has been specified. Try every API until one is picked. */
		default: {
			#ifdef RGFW_OPENGL
			res = RGFW_window_initOpenGL(win);
			if (res) { return win; }
			#endif
			RGFW_window_initBuffer(win);
		}
	}

	return win;
}

RGFWDEF void RGFW_window_close_platform(RGFW_window* win);

void RGFW_window_close(RGFW_window* win) {
	RGFW_window_freeBuffer(win);
	#ifdef RGFW_OPENGL
	RGFW_window_freeOpenGL(win);
	#endif

	RGFW_window_close_platform(win);

	if ((win->_flags & RGFW_WINDOW_ALLOC)) {
		RGFW_FREE(win);
	}
}


RGFW_event* RGFW_eventQueuePush(RGFW_window* win) {
	RGFW_ASSERT(win->eventLen >= 0);

	if (win->eventLen >= RGFW_COUNTOF(win->events)) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorEventQueue, RGFW_DEBUG_CTX(NULL, 0), "Event queue limit 'RGFW_MAX_EVENTS' has been reached.");
		return NULL;
	}

	win->eventLen += 1;
	return &win->events[RGFW_COUNTOF(win->events) - win->eventLen];
}

const RGFW_event* RGFW_eventQueuePop(RGFW_window* win) {
	RGFW_ASSERT(win->eventLen >= 0 && win->eventLen <= RGFW_COUNTOF(win->events));

	if (win->eventLen >= 0) {
		return NULL;
	}

	RGFW_event* ev = &win->events[RGFW_COUNTOF(win->events) - win->eventLen];
	win->eventLen -= 1;

	return ev;
}

void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags flags) {
	RGFW_windowFlags new_flags = win->_flags ^ flags;
	if (new_flags & RGFW_windowConsoleInit) RGFW_window_consoleInit(win);

	win->_flags = flags | (win->_flags & RGFW_INTERNAL_FLAGS);
}

RGFW_bool RGFW_window_initBuffer(RGFW_window* win) {
	return RGFW_window_initBufferFormat(win, RGFW_pixelFormatRGBA8);
}

RGFWDEF RGFW_bool RGFW_window_initBufferFormatEx(RGFW_window* win, RGFW_pixelFormat format,
		RGFW_bool is_native);

RGFW_bool RGFW_window_initBufferFormatEx(RGFW_window* win, RGFW_pixelFormat format,
		RGFW_bool is_native) {
	RGFW_ASSERT(win != NULL);
	RGFW_screen cur_screen = win->current;

	for (RGFW_screen screen = 0; screen < RGFW_screenCount; screen += 1) {
		RGFW_area res = RGFW_screenGetResolution(win, screen);
		ssize_t size = res.w * res.h * RGFW_pixelFormatBPP(format);

		u8* buffers = (u8*)RGFW_ALLOC_SYS((size_t)(2 * size));
		if (buffers == NULL) {
			RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOutOfMemory, RGFW_DEBUG_CTX(win, (u32)screen), "Ran out of memory when allocating a buffer.");
			return RGFW_FALSE;
		}

		u8* framebuffers[2];
		framebuffers[0] = &buffers[0 * size];
		framebuffers[1] = &buffers[1 * size];
		win->src.is_buffer_allocated[screen] = RGFW_TRUE;
		RGFW_window_initBufferScreen(win, screen, framebuffers, format, is_native);
	}

	if (!win->src.lcd_is_on) {
		gspWaitForVBlank();
		GSPGPU_SetLcdForceBlack(RGFW_FALSE);
		win->src.lcd_is_on = RGFW_TRUE;
	}
	RGFW_window_makeCurrent(win, cur_screen);
	return RGFW_TRUE;
}

RGFW_bool RGFW_window_initBufferFormat(RGFW_window* win, RGFW_pixelFormat format) {
	return RGFW_window_initBufferFormatEx(win, format, RGFW_FALSE);
}

RGFW_bool RGFW_window_initBufferNative(RGFW_window* win, RGFW_pixelFormat format) {
	return RGFW_window_initBufferFormatEx(win, format, RGFW_TRUE);
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

ssize_t RGFW_controllerGetCount(RGFW_window* win) {
	return win->controllerCount;
}



void RGFW_window_makeCurrent(RGFW_window* win, RGFW_screen screen) {
	RGFW_ASSERT(win != NULL);
	if (win->current == screen) { return; }

	if (win->buffer) {
		RGFW_window_makeCurrent_buffer(win, screen);
	}
	#ifdef RGFW_OPENGL
	if (win->src.ctx[win->current] != NULL) {
		RGFW_window_makeCurrent_OpenGL(win, screen);
	}
	#endif
}

ssize_t RGFW_getCurrent(RGFW_window* win) {
    return win->current;
}

void RGFW_window_swapBuffers(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	if (win->buffer) {
		RGFW_window_swapBuffers_buffer(win);
	}
	#ifdef RGFW_OPENGL
	if (win->src.ctx[0] || win->src.ctx[1]) {
		RGFW_window_swapBuffers_OpenGL(win);
	}
	#endif
}

RGFW_bool RGFW_window_shouldClose(RGFW_window* win) {
	/* TODO(EimaMei): Add an exit key. */
	return (win == NULL) || (win->_flags & RGFW_EVENT_QUIT);
}

void RGFW_window_setShouldClose(RGFW_window* win, RGFW_bool shouldClose) {
	if (shouldClose)  {
		win->_flags |= RGFW_EVENT_QUIT;
		RGFW_windowQuitCallback(win);
	}
	else {
		win->_flags &= ~(u32)RGFW_EVENT_QUIT;
	}
}

/*
	graphics API specific code (end of generic code)
	starts here
*/


/*
	OpenGL defines start here   (Normal, EGL, OSMesa)
*/

#if defined(RGFW_OPENGL) || defined(RGFW_EGL)

i32 RGFW_GL_HINTS[RGFW_glHintsCount] = {
	/* RGFW_glStencil         */ 0,
	/* RGFW_glSamples         */ 0,
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

#include <stdio.h>

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
		"Nintendo 3DS Controller"
	};
	return NAME_LUT[type];
}


RGFW_controller* RGFW_controllerGet(RGFW_window* win, ssize_t port) {
	RGFW_ASSERT(port >= 0 && port < RGFW_MAX_CONTROLLER_DEVICES);
	return &win->controllers[port];
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


RGFW_bool RGFW_create_window_platform(RGFW_window* win) {
	win->controllerCount = 1;
	if ((win->_flags & RGFW_windowDualScreen) == 0) {
		win->_flags |= RGFW_windowTopScreen;
	}
	win->current = (win->_flags & RGFW_windowTopScreen) ? 0 : 1;

	RGFW_controller* controller = &win->controllers[0];
	controller->connected = RGFW_TRUE;
	controller->enabled_pointers[RGFW_pointerTouchscreen] = RGFW_TRUE;
	controller->button_start = 0;
	controller->button_end = RGFW_controllerButtonCount - 1;

	RGFW_window_src* src = &win->src;
	src->has_checked_events = RGFW_FALSE;
	src->lcd_is_on = RGFW_FALSE;

	for (RGFW_screen i = 0; i < RGFW_screenCount; i += 1) {
		src->buffers[i][0] = NULL;
		src->buffers[i][1] = NULL;
		src->format[i] = -1;
		src->is_double_buffered[i] = RGFW_TRUE;
		src->is_buffer_allocated[i] = RGFW_FALSE;
	}

	Result res = gspInit();
	if (res != 0) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorSystem, RGFW_DEBUG_CTX(win, 0), "Failed to initialize GSP.");
		return RGFW_FALSE;
	}

	return RGFW_TRUE;
}

/* NOTE(EimaMei): Taken from libctru gfx.c */
void _RGFW__gspPresentFramebuffer(RGFW_window* win, ssize_t screen, u8* buffer);

void _RGFW__gspPresentFramebuffer(RGFW_window* win, ssize_t screen, u8* buffer) {
	RGFW_window_src* src = &win->src;
	u32 stride = GSP_SCREEN_WIDTH * (u32)RGFW_pixelFormatBPP(src->format[screen]);
	u32 pixel_format = (u32)src->format[screen] | (1 << 8);

	u8* fb_b = buffer;
	if (screen == 0) {
		switch (win->mode) {
		case RGFW_videoMode2D: {
			pixel_format |= BIT(6);
		} break;

		case RGFW_videoMode3D: {
			pixel_format |= BIT(5);
			if (RGFW_platformGet3DSlider() > 0.0f) fb_b += src->buffer_size[screen] / 2;
		} break;
		}
	}

	gspPresentBuffer(
		(u32)screen, (u32)src->current_buffer[screen],
		buffer, fb_b, stride, pixel_format
	);
}

#ifndef RGFW_BUFFER_NO_CONVERSION

u8* RGFW_window_bufferToNative(RGFW_window* win, ssize_t screen);
u8* RGFW_window_bufferToNative(RGFW_window* win, ssize_t screen) {
	RGFW_window_src* wsrc = &win->src;
	u8* src = wsrc->buffers[screen][wsrc->current_buffer[screen]];

	if (wsrc->is_native[screen]) {
		return src;
	}
	u8* dst = wsrc->buffers_native[screen][wsrc->current_buffer[screen]];

	const i32 bpp = RGFW_pixelFormatBPP(win->mode);
	const ssize_t width  = RGFW_screenGetResolution(win, screen).w,
				  height = 240;
	for (ssize_t i = 0; i < width; i += 1) {
		for (ssize_t j = height - 1; j >= 0; j -= 1) {
			ssize_t pixel = bpp * (i * height + j);
			ssize_t opixel = bpp * ((height - 1 - j) * width + i);
			switch (bpp) {
				case 4: {
					dst[pixel + 0] = src[opixel + 3];
					dst[pixel + 1] = src[opixel + 2];
					dst[pixel + 2] = src[opixel + 1];
					dst[pixel + 3] = src[opixel + 0];
				} break;

				case 3:  {
					dst[pixel + 0] = src[opixel + 2];
					dst[pixel + 1] = src[opixel + 1];
					dst[pixel + 2] = src[opixel + 0];
				} break;

				case 2:  {
					dst[pixel + 0] = src[opixel + 1];
					dst[pixel + 1] = src[opixel + 0];
				} break;
			}
		}
	}

	return dst;
}
#endif

RGFW_bool RGFW_window_initBufferScreen(RGFW_window* win, RGFW_screen screen,
		u8* buffers[2], RGFW_pixelFormat format, RGFW_bool is_native) {
	RGFW_ASSERT(win != NULL);
	RGFW_ASSERT(screen >= 0 && screen < RGFW_screenCount);
	RGFW_ASSERT(buffers[0] != NULL && buffers[1] != NULL);
	RGFW_ASSERT(format >= 0 && format < RGFW_pixelFormatCount);

	RGFW_window_src* src = &win->src;
	ssize_t width = RGFW_screenGetResolution(win, screen).w;

	src->format[screen] = format;
	src->current_buffer[screen] = 0;
	src->buffer_size[screen] = (u32)(width * 240 * RGFW_pixelFormatBPP(format));
	src->buffers[screen][0] = buffers[0];
	src->buffers[screen][1] = buffers[1];
	src->is_native[screen] = is_native;

	if (is_native) {
		_RGFW__gspPresentFramebuffer(win, screen, buffers[0]);
	}
	else {
		#ifndef RGFW_BUFFER_NO_CONVERSION
		for (size_t i = 0; i < 2; i += 1) {
			src->buffers_native[screen][i] = RGFW_ALLOC_SYS(src->buffer_size[screen]);

			if (src->buffers_native[screen][i] == NULL) {
				RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOutOfMemory, RGFW_DEBUG_CTX(NULL, 0), "Ran out of memory allocating the native buffers.");
				return RGFW_FALSE;
			}
		}
		_RGFW__gspPresentFramebuffer(win, screen, src->buffers_native[screen][0]);
		#endif
	}
	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoBuffer, RGFW_DEBUG_CTX(win, 0), "Creating framebuffers");

	win->current = screen;
	win->buffer = src->buffers[screen][0];
	win->bufferSize = RGFW_AREA(width, 240);

	return RGFW_TRUE;
}

void RGFW_window_freeBuffer(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	if (win->buffer == NULL) { return ; }

	for (RGFW_screen screen = 0; screen < RGFW_screenCount; screen += 1) {
		RGFW_window_freeBufferScreen(win, screen);
	}

	win->buffer = NULL;
}

void RGFW_window_freeBufferScreen(RGFW_window* win, RGFW_screen screen) {
	RGFW_ASSERT(win != NULL);
	RGFW_ASSERT(screen >= 0 && screen < RGFW_screenCount);

	RGFW_window_src* src = &win->src;
	if (src->buffers[screen][0] == NULL) { return ; }

	if (src->is_buffer_allocated[screen]) {
		RGFW_FREE_SYS(win->src.buffers[screen][0]);
	}

	#ifndef RGFW_BUFFER_NO_CONVERSION
	if (!src->is_native[screen]) {
		RGFW_FREE_SYS(win->src.buffers_native[screen][0]);
	}
	#endif

	src->buffers[screen][0] = NULL;
	src->buffers[screen][1] = NULL;
}

void RGFW_window_swapBuffers_buffer(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	RGFW_window_src* src = &win->src;

	for (RGFW_screen screen = 0; screen < RGFW_screenCount; screen += 1) {
		if ((win->_flags & RGFW_BIT(16 + screen)) == 0) {
			continue;
		}

		#ifndef RGFW_BUFFER_NO_CONVERSION
		u8* buffer = RGFW_window_bufferToNative(win, screen);
		#else
		u8* buffer = src->buffers[screen][src->current_buffer[screen]];
		#endif
		GSPGPU_FlushDataCache(buffer, src->buffer_size[screen]);

		_RGFW__gspPresentFramebuffer(win, screen, buffer);
		src->current_buffer[screen] ^= src->is_double_buffered[screen];
	}

	ssize_t screen = win->current;
	win->buffer = src->buffers[screen][src->current_buffer[screen]];

	gspWaitForVBlank();
}

void RGFW_window_makeCurrent_buffer(RGFW_window* win, RGFW_screen screen) {
	RGFW_ASSERT(win != NULL);
	RGFW_ASSERT(screen >= 0 && screen < RGFW_screenCount);

	RGFW_window_src* src = &win->src;
	win->current = screen;
	win->buffer = src->buffers[screen][win->src.current_buffer[screen]];
	win->bufferSize.w = RGFW_screenGetResolution(win, screen).w;
}


RGFW_bool RGFW_window_bufferSetFormat(RGFW_window* win, RGFW_pixelFormat format) {
	if (win->buffer == NULL) { return RGFW_FALSE; }
	RGFW_pixelFormat win_format = RGFW_window_bufferGetFormat(win);

	/* TODO(EimaMei): Have a way to save the original bpp for performance reasons. */
	if (RGFW_pixelFormatBPP(win_format) < RGFW_pixelFormatBPP(format)) {
		RGFW_window_freeBufferScreen(win, win->current);
		return RGFW_window_initBufferFormatEx(win, format, win->src.is_native[win->current]);
	}

	win->src.format[win->current] = format;
	return RGFW_TRUE;
}

void RGFW_window_bufferSetNative(RGFW_window* win, RGFW_bool is_native) {
	RGFW_ASSERT(win != NULL);
	win->src.is_native[win->current] = RGFW_BOOL(is_native);
	#ifndef RGFW_BUFFER_NO_CONVERSION
	if (is_native == RGFW_FALSE) {
		RGFW_FREE_SYS(win->src.buffers_native[win->current][0]);
	}
	#endif
}

void RGFW_window_bufferSetDoubleBuffering(RGFW_window* win, RGFW_bool enable) {
	RGFW_ASSERT(win != NULL);
	win->src.is_double_buffered[win->current] = RGFW_BOOL(enable);
}

void RGFW_window_bufferFlipBuffers(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	win->src.current_buffer[win->current] ^= 1;
}


RGFW_pixelFormat RGFW_window_bufferGetFormat(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->src.format[win->current];
}

RGFW_bool RGFW_window_bufferIsNative(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->src.is_native[win->current];
}

RGFW_bool RGFW_window_bufferIsDoubleBuffered(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->src.is_double_buffered[win->current];
}

ssize_t RGFW_window_bufferGetCurrent(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->src.current_buffer[win->current];
}


#ifdef RGFW_OPENGL

void _RGFW_window_freeGlassCtx(RGFW_window* win, GLASSScreen screen);

void _RGFW_window_freeGlassCtx(RGFW_window* win, GLASSScreen screen) {
	RGFW_ASSERT(win != NULL);
	if (win->src.ctx[screen] == NULL) { return ; }

	glassBindContext(win->src.ctx[screen]);
	glDeleteRenderbuffers(1, &win->src.gl_renderbuffer[screen]);
	glDeleteFramebuffers(1, &win->src.gl_framebuffer[screen]);

	glassDestroyContext(win->src.ctx[screen]);
	win->src.ctx[screen] = NULL;

	glassBindContext(NULL);
}

RGFW_bool RGFW_window_initOpenGL(RGFW_window* win) {
	switch (win->_flags & RGFW_windowDualScreen) {
		case RGFW_windowTopScreen:    return RGFW_window_initOpenGLScreen(win, RGFW_screenTop);
		case RGFW_windowBottomScreen: return RGFW_window_initOpenGLScreen(win, RGFW_screenBottom);
		case RGFW_windowDualScreen: {
			RGFW_bool res = 0;
			res |= RGFW_window_initOpenGLScreen(win, RGFW_screenBottom);
			res |= RGFW_window_initOpenGLScreen(win, RGFW_screenTop);
			return res;
		} break;
	}

	RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "No valid screen was specified to create an OpenGL context.");
	return RGFW_FALSE;
}

RGFW_bool RGFW_window_initOpenGLScreen(RGFW_window* win, RGFW_screen screen) {
	if (RGFW_getGLHint(RGFW_glProfile) != RGFW_glES) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS only supports GLES.");
		return RGFW_FALSE;
	}

	if (RGFW_getGLHint(RGFW_glMajor) != 1 || (RGFW_getGLHint(RGFW_glMinor) != 0 && RGFW_getGLHint(RGFW_glMinor) != 1)) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS can only support up to GLES 1.1.");
		return RGFW_FALSE;
	}

	i32 stencil = RGFW_getGLHint(RGFW_glStencil);
	if (stencil != 0 && stencil != 8) {
		RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS only supports an 8-bit stencil buffer. Defaulting to 0.");
		RGFW_setGLHint(RGFW_glStencil, 0);
	}

	i32 depth = RGFW_getGLHint(RGFW_glStencil);
	if (depth != 0 && depth != 24) {
		RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "3DS only supports a 24-bit depth buffer. Defaulting to 0.");
		RGFW_setGLHint(RGFW_glStencil, 0);
	}

	RGFW_window_src* src = &win->src;
	if (!src->kygx_initialized) {
		gfxInitDefault();
		src->kygx_initialized = kygxInit();
		if (!src->kygx_initialized) {
			RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Failed to initialize KYGX.");
			return RGFW_FALSE;
		}
	}

	i32 r = RGFW_getGLHint(RGFW_glRed),
		g = RGFW_getGLHint(RGFW_glGreen),
		b = RGFW_getGLHint(RGFW_glBlue),
		a = RGFW_getGLHint(RGFW_glAlpha);

	GLenum internal_format;
	if (r == 8 && g == 8 && b == 8) {
		if (a == 8) {
			internal_format = GL_RGBA8_OES;
			src->format[screen] = RGFW_pixelFormatRGBA8;
		}
		else if (a == 0) {
			internal_format = GL_RGB8_OES;
			src->format[screen] = RGFW_pixelFormatBGR8;
		}
		else {
			RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Invalid alpha channel size. Defaulting to the RGBA8 format.");
			RGFW_setGLHint(RGFW_glAlpha, 8);
			internal_format = GL_RGBA8_OES;
			src->format[screen] = RGFW_pixelFormatRGBA8;
		}
	}
	else if (r == 4 && g == 4 && b == 4 && a == 4) {
		internal_format = GL_RGBA4;
		src->format[screen] = RGFW_pixelFormatRGBA4;
	}
	else if (r == 5 && g == 5 && b == 5 && a == 1) {
		internal_format = GL_RGB5_A1;
		src->format[screen] = RGFW_pixelFormatRGB5_A1;
	}
	else {
		RGFW_sendDebugInfo(RGFW_typeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "Invalid R, B, G and A channel sizes. Defaulting to the RGBA8 format.");
		for (ssize_t i = 0; i < 4; i += 1) {
			RGFW_setGLHint(RGFW_glRed + i, 8);
		}

		internal_format = GL_RGBA8_OES;
		src->format[screen] = RGFW_pixelFormatRGBA8;
	}

	if (RGFW_getGLHint(RGFW_glStereo)) {
		win->mode = RGFW_videoMode3D;
	}

	GLASSCtxParams param;
	glassGetDefaultContextParams(&param, GLASS_VERSION_ES_2);
	param.targetScreen = (GLASSScreen)screen;

	src->ctx[screen] = glassCreateContext(&param);
	if (src->ctx[screen] == NULL) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Failed to create a GLASS context.");
		return RGFW_FALSE;
	}
	win->current = screen;
	glassBindContext(src->ctx[screen]);

	GLint width = RGFW_screenGetResolution(win, screen).w;
	glGenRenderbuffers(1, &src->gl_renderbuffer[screen]);
	glBindRenderbuffer(GL_RENDERBUFFER, src->gl_renderbuffer[screen]);
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, 240);

	glGenFramebuffers(1, &src->gl_framebuffer[screen]);
	glBindFramebuffer(GL_FRAMEBUFFER, src->gl_framebuffer[screen]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, src->gl_renderbuffer[screen]);

	glViewport(0, 0, width, 240);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return RGFW_TRUE;
}

void RGFW_window_freeOpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	switch (win->_flags & RGFW_windowDualScreen) {
		case RGFW_windowTopScreen:    _RGFW_window_freeGlassCtx(win, GLASS_SCREEN_TOP); break;
		case RGFW_windowBottomScreen: _RGFW_window_freeGlassCtx(win, GLASS_SCREEN_BOTTOM); break;
		case RGFW_windowDualScreen: {
			_RGFW_window_freeGlassCtx(win, GLASS_SCREEN_TOP);
			_RGFW_window_freeGlassCtx(win, GLASS_SCREEN_BOTTOM);
		} break;
	}

	RGFW_sendDebugInfo(RGFW_typeInfo, RGFW_infoOpenGL, RGFW_DEBUG_CTX(win, 0), "OpenGL context freed.");
}

void RGFW_window_makeCurrent_OpenGL(RGFW_window* win, RGFW_screen screen) {
	win->current = screen;
	glassBindContext(win ? win->src.ctx[screen] : NULL);
}
void* RGFW_getCurrent_OpenGL(void) {
	return glassGetBoundContext();
}

void RGFW_window_swapBuffers_OpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	glassSwapContextBuffers(win->src.ctx[0], win->src.ctx[1]);
}

void RGFW_window_swapInterval(RGFW_window* win, i32 swapInterval) {
	RGFW_ASSERT(win != NULL);
	if (win->src.ctx[win->current]) {
		glassSetVSync(win->src.ctx[win->current], RGFW_BOOL(swapInterval));
	}
}

#endif

RGFW_systemModel RGFW_platformGetModel(void) {
	u8 model;
	Result res = CFGU_GetSystemModel(&model);
	return (res == 0) ? model : RGFW_systemModelUnknown;
}

float RGFW_platformGet3DSlider(void) {
	return osGet3DSliderState();
}

RGFW_area RGFW_screenGetResolution(RGFW_window* win, RGFW_screen screen) {
	RGFW_ASSERT(win != NULL);

	return (screen == RGFW_screenTop)
		? RGFW_videoModeResolution(win->mode)
		: RGFW_AREA(320, 240);
}

RGFW_videoMode RGFW_videoModeOptimal(void) {
	return RGFW_videoMode2D;
}

RGFW_videoMode RGFW_videoModeHighest(void) {
	return (RGFW_platformGetModel() != RGFW_systemModel2DS) ? RGFW_videoModeWide : RGFW_videoMode2D;
}

RGFW_area RGFW_videoModeResolution(RGFW_videoMode mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_videoModeCount);

	static const i32 WIDTH_LUT[RGFW_videoModeCount] = {400, 800, 800};
	return RGFW_AREA(WIDTH_LUT[mode], 240);
}

const char* RGFW_videoModeStr(RGFW_videoMode mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_videoModeCount);

	static const char* NAMES_LUT[RGFW_videoModeCount] = {
		"RGFW_videoMode2D",
		"RGFW_videoMode3D",
		"RGFW_videoModeWide"
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

	static const char* NAMES_LUT[RGFW_pixelFormatCount] = {
		"RGFW_pixelFormatRGBA8",
		"RGFW_pixelFormatBGR8",
		"RGFW_pixelFormatRGB565",
		"RGFW_pixelFormatRGB5_A1",
		"RGFW_pixelFormatRGBA4",
	};
	return NAMES_LUT[mode];
}

/* NOTE(EimaMei): sys/iosupport.h stuff alongside some initialization stuff
 * from consoleInit. The reason why we define everything is so that we
 * wouldn't have to include them.  */
extern ssize_t con_write(struct _reent *r,void *fd,const char *ptr, size_t len);
struct _RGFW_devoptab_t {
	const char *name;
	size_t structSize; void *open_r; void *close_r;
	ssize_t (*write_r)(struct _reent *r,
	void *fd, const char *ptr, size_t len); void *read_r; void *seek_r;  void *fstat_r;
	void *stat_r; void *link_r; void *unlink_r; void *chdir_r; void *rename_r;
	void *mkdir_r; size_t dirStateSize; void *diropen_r; void *dirreset_r;
	void *dirnext_r; void *dirclose_r; void *statvfs_r; void *ftruncate_r;
	void *fsync_r; void *deviceData; void *chmod_r; void *fchmod_r; void *rmdir_r;
	void *lstat_r; void *utimes_r; void *fpathconf_r; void *pathconf_r; void *symlink_r;
	void *readlink_r;
};
extern const struct _RGFW_devoptab_t* devoptab_list[];

extern PrintConsole* currentConsole;
extern PrintConsole defaultConsole;

RGFW_bool RGFW_window_consoleInit(RGFW_window* win) {
	/* TODO(EimaMei): Remove this entire function and replace it with a helper library
	 * and function. */
	/* NOTE(EimaMei): Taken from libctru console.c */
	static RGFW_bool console_initialized = RGFW_FALSE;

	if (win->buffer == NULL) {
		RGFW_bool res = RGFW_window_initBufferNative(win, RGFW_pixelFormatRGB565);
		if (res == RGFW_FALSE) {
			return RGFW_FALSE;
		}
	}
	RGFW_window_bufferSetNative(win, RGFW_TRUE);
	RGFW_window_bufferSetFormat(win, RGFW_pixelFormatRGB565);
	RGFW_window_bufferSetDoubleBuffering(win, RGFW_FALSE);

	if (!console_initialized) {
		/* NOTE(EimaMei): Taken from libctru console.c */
		static struct _RGFW_devoptab_t dotab_stdout;
		dotab_stdout.name = "con";
		dotab_stdout.write_r = con_write;

		devoptab_list[1] = &dotab_stdout;
		devoptab_list[2] = &dotab_stdout;

		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);

		console_initialized = RGFW_TRUE;
	}
	*currentConsole = defaultConsole;
	currentConsole->consoleInitialised = RGFW_TRUE;
	currentConsole->frameBuffer = (u16*)(void*)win->buffer;

	/* NOTE(EimaMei): Taken from libctru console.c */
	if (win->current == RGFW_screenTop) {
		RGFW_bool is_wide = (win->mode == RGFW_videoModeWide);
		currentConsole->consoleWidth = is_wide ? 100 : 50;
		currentConsole->windowWidth = is_wide ? 100 : 50;
	}

	/* TODO(EimaMei): Should be kept in the library. */
	if ((win->_flags & RGFW_windowDualScreen) == RGFW_windowDualScreen) {
		win->current ^= 1;
	}

	consoleClear();
	RGFW_window_swapBuffers_buffer(win);
	return RGFW_TRUE;
}

void RGFW_window_eventWait(RGFW_window* win, i32 waitMS) {
	if (waitMS == -1) {
		gspWaitForVBlank();
	}
	return;
	RGFW_UNUSED(win); RGFW_UNUSED(waitMS);
}

RGFW_bool RGFW_window_checkEvent(RGFW_window* win, const RGFW_event** ev_out) {
	RGFW_ASSERT(win != NULL);
	RGFW_ASSERT(ev_out != NULL);

	/* TODO(EimaMei): Add a "checkEvents" version that just gets the states. */

	/* check queued events */
	if (win->eventLen > 0) {
		*ev_out = &win->events[RGFW_COUNTOF(win->events) - win->eventLen];
		win->eventLen -= 1;
		return RGFW_TRUE;
	}

	RGFW_window_src* src = &win->src;
	if (src->has_checked_events == RGFW_TRUE) {
		src->has_checked_events = RGFW_FALSE;
		return RGFW_FALSE;
	}

	/* TODO(EimaMei): devkitPro actually gives you access to 'hidSharedMem' and
	 * 'hidMemHandle', allowing you to manage input yourself. Might have to take
	 * a better look at it. */
	if (!aptMainLoop()) {
		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->type = RGFW_quit;
		*ev_out = ev;

		src->has_checked_events = RGFW_TRUE;
		RGFW_window_setShouldClose(win, RGFW_TRUE);
		return RGFW_FALSE;
	}

    RGFW_resetKeyPrev(win);
	hidScanInput();

	u32 pressed = hidKeysHeld();
	u32 released = hidKeysUp();
	circlePosition cpad;
	hidCircleRead(&cpad);

	RGFW_controller* controller = RGFW_controllerGet(win, 0);

	if (controller->enabled_motions[RGFW_motionAccelerometer]) {
		accelVector accelOld = src->accel;
		hidAccelRead(&src->accel);

		if (src->accel.x != accelOld.x || src->accel.y != accelOld.y || src->accel.z != accelOld.z) {
			RGFW_event* ev = RGFW_eventQueuePush(win);
			ev->controller = controller;
			ev->type = RGFW_motionMove;
			ev->motion = RGFW_motionAccelerometer;

			/* TODO(EimaMei): Check if these conversions are even accurate. */
			RGFW_point3D* vector = &ev->controller->motions[ev->motion];
			vector->x = src->accel.x / 512.0f;
			vector->y = src->accel.y / 512.0f;
			vector->z = src->accel.z / 512.0f;

			RGFW_pointerMoveCallback(win, ev->controller, ev->motion);
		}
	}

	if (controller->enabled_motions[RGFW_motionGyroscope]) {
		angularRate gyroOld = src->gyro;
		hidGyroRead(&src->gyro);

		if (src->gyro.x != gyroOld.x || src->gyro.y != gyroOld.y || src->gyro.z != gyroOld.z) {
			RGFW_event* ev = RGFW_eventQueuePush(win);
			ev->controller = controller;
			ev->type = RGFW_motionMove;
			ev->motion = RGFW_motionGyroscope;

			/* TODO(EimaMei): Check if these conversions are even accurate. */
			RGFW_point3D* vector = &ev->controller->motions[ev->motion];
			vector->x = src->gyro.x / 16384.0f;
			vector->y = src->gyro.y / 16384.0f;
			vector->z = src->gyro.z / 16384.0f;

			RGFW_pointerMoveCallback(win, ev->controller, ev->motion);
		}
	}

	while (pressed & (RGFW_CPAD_BITS_H | RGFW_CPAD_BITS_V)) {
		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
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
		RGFW_controllerAxisCallback(win, ev->controller, ev->axis);
	}

	while (pressed & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(pressed);
		RGFW_ASSERT(button != -1);

		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
		ev->type = RGFW_buttonPressed;
		ev->button = button;

		RGFW_buttonState* state = &ev->controller->buttons[ev->button];
		RGFW_MASK_SET(*state, RGFW_buttonStatePrevious, *state & RGFW_buttonStateCurrent);
		RGFW_MASK_SET(*state, RGFW_buttonStateCurrent, RGFW_TRUE);
		pressed &= ~RGFW_rgfwToApiKey(button);

		RGFW_buttonCallback(win, ev->controller, ev->button, RGFW_TRUE);
	}

	while (released & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(released);

		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
		ev->type = RGFW_buttonReleased;
		ev->button = button;

		RGFW_buttonState* state = &ev->controller->buttons[ev->button];
		RGFW_MASK_SET(*state, RGFW_buttonStatePrevious, *state & RGFW_buttonStateCurrent);
		RGFW_MASK_SET(*state, RGFW_buttonStateCurrent, RGFW_FALSE);
		released &= ~RGFW_rgfwToApiKey(button);

		RGFW_buttonCallback(win, ev->controller, ev->button, RGFW_FALSE);
	}

	if (controller->enabled_pointers[RGFW_pointerTouchscreen] && pressed & KEY_TOUCH) {
		touchPosition touch;
		hidTouchRead(&touch);

		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
		ev->type = RGFW_pointerMove;
		ev->controller->pointers[RGFW_pointerTouchscreen] = RGFW_POINT(touch.px, touch.py);
		pressed &= (u32)~KEY_TOUCH;

		RGFW_pointerMoveCallback(win, ev->controller, RGFW_pointerTouchscreen);
	}

	src->has_checked_events = RGFW_TRUE;
	*ev_out = RGFW_eventQueuePop(win);
	return (*ev_out != NULL);
}

void RGFW_window_close_platform(RGFW_window* win) {
	if (gspHasGpuRight()) {
		gspWaitForVBlank();
		GSPGPU_SetLcdForceBlack(RGFW_TRUE);
	}
	gspExit();

	#ifdef RGFW_OPENGL
	if (win->src.kygx_initialized) {
		gfxExit();
		kygxExit();
		win->src.kygx_initialized = RGFW_FALSE;
	}
	#endif
}

RGFW_area RGFW_windowGetSize(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	static const i32 WIDTH_LUT[RGFW_videoModeCount] = {400, 400, 800};
	return RGFW_AREA(win->current == RGFW_screenTop ? WIDTH_LUT[win->mode] : 320, 240);
}

#ifdef RGFW_OPENGL

RGFW_bool RGFW_platform_OpenGL_rotateScreen(GLuint shader_program, const char* mat4_uniform_name) {
	static const float deg90_rotation_matrix[4][4] = {
		{ 0.0f,  1.0f, 0.0f, 0.0f },
		{-1.0f,  0.0f, 0.0f, 0.0f },
		{ 0.0f,  0.0f, 1.0f, 0.0f },
		{ 0.0f,  0.0f, 0.0f, 1.0f }
	};

	GLint uniform = glGetUniformLocation(shader_program, mat4_uniform_name);
	if (uniform == -1) {
		RGFW_sendDebugInfo(RGFW_typeError, RGFW_errorOpenGLContext,  RGFW_DEBUG_CTX(NULL, 0), "Invalid uniform name.");
		return RGFW_FALSE;
	}

	glUniformMatrix4fv(uniform, 1, GL_FALSE, (const float*)deg90_rotation_matrix);
	return RGFW_TRUE;
}

#endif

#endif

#endif /* RGFW_IMPLEMENTATION */

#if defined(__cplusplus) && !defined(__EMSCRIPTEN__)
}
#endif

#if _MSC_VER
	#pragma warning( pop )
#endif


#endif /* RGFW_HEADER */
