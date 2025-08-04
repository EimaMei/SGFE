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
	#define RGFW_NO_INFO - do not define the RGFW_info struct (without RGFW_IMPLEMENTATION)

	#define RGFW_ALLOC x  - choose the default allocation function (defaults to standard malloc)
	#define RGFW_FREE x  - choose the default deallocation function (defaults to standard free)

	#define RGFW_EXPORT - use when building RGFW
	#define RGFW_IMPORT - use when linking with RGFW (not as a single-header)

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


#if defined(__cplusplus)
	extern "C" {
#endif

	/* makes sure the header file part is only defined once by default */
#ifndef RGFW_HEADER

#define RGFW_HEADER

#include <stddef.h>
#ifndef RGFW_INT_DEFINED
	#include <stdint.h>

	typedef uint8_t     u8;
	typedef int8_t      i8;
	typedef uint16_t   u16;
	typedef int16_t    i16;
	typedef uint32_t   u32;
	typedef int32_t    i32;
	typedef uint64_t   u64;
	typedef int64_t    i64;

	#define RGFW_INT_DEFINED
#endif

#ifndef RGFW_BOOL_DEFINED
	typedef u8 RGFW_bool;
#endif

#if !defined(RGFW_SSIZE_T_DEFINED) && !defined(_SYS_TYPES_H) && !defined(_SSIZE_T_DECLARED)
	typedef ptrdiff_t ssize_t;
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
	RGFW_screenPrimary,
	#ifdef RGFW_3DS
	RGFW_screenTop = RGFW_screenPrimary,
	RGFW_screenBottom,
	#endif

	RGFW_screenCount
};

typedef RGFW_ENUM(u8, RGFW_eventType) {
	/*! event codes */
	RGFW_eventNone = 0, /*!< no event has been sent */

	RGFW_quit, /*!< the user clicked the quit button */
	RGFW_deviceSleep, /* The device has entered sleep mode. */
	RGFW_deviceWakeup, /* The device has exited out of sleep mode. */

	RGFW_windowRefresh, /* The window content needs to be refreshed */
	RGFW_windowVideoModeChanged, /* Video mode for the window has been changed (usually by 'RGFW_windowChangeVideoMode'). */
	RGFW_windowFocusOut, /* User has exited the window and entered a new environment (eg. "Home Menu".) */
	RGFW_windowFocusIn, /* 	User has entered the window from an external environment (usually the "Home Menu"). */


	RGFW_controllerConnected, /*!< a controller was connected */
	RGFW_controllerDisconnected, /*!< a controller was disconnected */

	RGFW_buttonPressed, /*!< a controller button was pressed */
	RGFW_buttonReleased, /*!< a controller button was released */

	RGFW_axisMove, /*!< an axis of a controller was moved */

	RGFW_pointerMove, /* The pointer of the controller has moved. */
	RGFW_pointerPressed, /* The pointer buttons of the controller have been pressed. */

	RGFW_motionMove, /* The motion sensors of the controller have been moved. */
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
	/* Sets the video resolution to 400x240 with stereoscopy disabled. Works on
	 * all models. */
	RGFW_videoMode2D,
	/* Sets the video resolution to 400x240 with stereoscopy enabled. Works on 
	 * all non-2DS consoles. While the visible video resolution is 400x240, the
	 * actual buffer size is 800x240.
	 *
	 * Left image is from 0x0 to 400x240, right image is from 400x240 to 800x240. */
	RGFW_videoMode3D,
	/* Sets the video resolution to 800x240 with steroescopy disabled. Only works 
	 * on 3DS consoles and New 2DS XL. */
	RGFW_videoModeWide,
	/* TODO(EimaMei): Add an error if you select this on 2DS consoles.*/
	#endif

	RGFW_videoModeCount
};

/* TODO(EimaMei): document */
/* TODO(EimaMei): document how if native mode is turned on RGFW_pixelFormatRGBA8 might return false. */
typedef RGFW_ENUM(ssize_t, RGFW_pixelFormat) {
	/* Each channel bit is set to 8-bits. This option is always supported for
	 * non-native buffers, but might not be supported for native ones. By default
	 * 'RGFW_window_initBuffer' picks this as the pixel format. */
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

/*! Optional arguments for making a windows */
typedef RGFW_ENUM(u32, RGFW_windowFlags) {
	RGFW_windowFlagsNone      = 0,

	RGFW_windowFreeOnClose    = RGFW_BIT(15),
	RGFW_windowBuffer         = RGFW_BIT(16),
	RGFW_windowOpenGL         = RGFW_BIT(17),
	RGFW_windowEGL            = RGFW_BIT(18),

	#ifdef RGFW_3DS
	RGFW_windowTopScreen      = RGFW_BIT(29), /* TODO(EimaMei): New enum. */
	RGFW_windowBottomScreen   = RGFW_BIT(30), /* TODO(EimaMei): New enum. */
	RGFW_windowDualScreen     = RGFW_windowTopScreen | RGFW_windowBottomScreen, /* TODO(EimaMei): New enum. */
	#endif
};

/* TODO */
typedef struct RGFW_window RGFW_window;
/* TODO */
typedef struct RGFW_window_src RGFW_window_src;

/* TODO */
typedef struct RGFW_context_buffer RGFW_context_buffer;

/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*RGFW_deviceSleepFunc)(RGFW_window* win, RGFW_bool is_sleeping);

/*! RGFW_quit, the window that was closed */
typedef void (*RGFW_quitFunc)(RGFW_window* win);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*RGFW_windowRefreshFunc)(RGFW_window* win);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*RGFW_windowVideoModeFunc)(RGFW_window* win, RGFW_videoMode mode);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*RGFW_windowFocusFunc)(RGFW_window* win, RGFW_bool is_focused);

/*! RGFW_controllerConnected / RGFW_controllerDisconnected, the window that got the event, the controller in question, if the controller was connected (else it was disconnected) */
typedef void (*RGFW_controllerFunc)(RGFW_window* win, RGFW_controller* controller, RGFW_bool is_connected);

/*! RGFW_buttonPressed, the window that got the event, the button that was pressed, the scroll value, if it was a press (else it's a release) */
typedef void (*RGFW_buttonFunc)(RGFW_window* win, RGFW_controller* controller, RGFW_button button, RGFW_bool pressed);
/*! RGFW_axisMove, the window that got the event, the controller in question, the axis values and the axis count */
typedef void (*RGFW_axisFunc)(RGFW_window* win, RGFW_controller* controller, RGFW_axisType axis);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*RGFW_pointerFunc)(RGFW_window* win, RGFW_controller* controller, RGFW_pointerType pointer);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*RGFW_motionFunc)(RGFW_window* win, RGFW_controller* controller, RGFW_motionType motion);


typedef struct RGFW_callbacks {
	RGFW_deviceSleepFunc sleep;

	RGFW_quitFunc quit;
	RGFW_windowRefreshFunc refresh;
	RGFW_windowVideoModeFunc video_mode;
	RGFW_windowFocusFunc focus;

	RGFW_controllerFunc controller;

	RGFW_buttonFunc button;
	RGFW_axisFunc axis;
	RGFW_pointerFunc pointer;
	RGFW_motionFunc motion;
} RGFW_callbacks;


#if !defined(RGFW_NO_WINDOW_SRC) || defined(RGFW_IMPLEMENTATION)

#include <3ds.h>

#ifdef RGFW_OPENGL
#define RIP_BACKEND RIP_BACKEND_KYGX
#include <GLES/gl2.h>
#endif

struct RGFW_context_buffer {
	RGFW_screen screen;
	RGFW_videoMode mode;
	RGFW_pixelFormat format;
	u8* buffers[2];

	ssize_t current;
	u32 size;

	RGFW_bool is_buffer_allocated;
	RGFW_bool is_double_buffered;
	RGFW_bool is_native;

	#ifndef RGFW_BUFFER_NO_CONVERSION
	u8* buffers_native[2];
	#endif
};

#ifdef RGFW_OPENGL
struct RGFW_context_OpenGL {
	RGFW_screen screen;
	RGFW_videoMode mode;
	RGFW_pixelFormat format;
	GLASSCtx ctx;

	GLuint framebuffer;
	GLuint renderbuffer;
};
#endif

struct RGFW_window_src {
	RGFW_bool has_checked_events, lcd_is_on;
	accelVector accel;
	angularRate gyro;
	aptHookCookie apt_hook;

	RGFW_context_buffer buffer[RGFW_screenCount];
	#ifdef RGFW_OPENGL
	struct RGFW_context_OpenGL gl[RGFW_screenCount];
	#endif
};

struct RGFW_window {
	RGFW_window_src src; /*!< src window data */

	/* TODO */
	RGFW_context_buffer* buffer;
	#ifdef RGFW_OPENGL
	struct RGFW_context_OpenGL* gl;
	#endif

	/* struct RGFW_info */
	RGFW_event events[32];
	ssize_t eventLen;

	ssize_t controllerCount; /*!< the actual amount of controllers */
	RGFW_controller controllers[RGFW_MAX_CONTROLLER_DEVICES];

	RGFW_bool stopCheckEvents_bool;
	u32 _flags; /*!< windows flags (for RGFW to check) */

	RGFW_callbacks callbacks;
	void* userPtr;
}; /*!< window structure for managing the window */

#endif /* !defined(RGFW_NO_WINDOW_SRC) || defined(RGFW_IMPLEMENTATION) */

/* TODO */
RGFWDEF size_t RGFW_sizeofWindow(void);
/* TODO */
RGFWDEF size_t RGFW_sizeofWindowSrc(void);

/* TODO */
RGFWDEF RGFW_context_buffer* RGFW_window_getCurrent_buffer(RGFW_window* win);
/* TODO */
RGFWDEF RGFW_context_buffer* RGFW_window_getContext_buffer(RGFW_window* win);
/* TODO */
RGFWDEF RGFW_context_buffer* RGFW_window_getContextEx_buffer(RGFW_window* win, RGFW_screen screen);

#ifdef RGFW_OPENGL
/* TODO */
RGFWDEF struct RGFW_context_OpenGL* RGFW_window_getCurrent_OpenGL(RGFW_window* win);
/* TODO */
RGFWDEF struct RGFW_context_OpenGL* RGFW_window_getContext_OpenGL(RGFW_window* win);
/* TODO */
RGFWDEF struct RGFW_context_OpenGL* RGFW_window_getContextEx_OpenGL(RGFW_window* win, RGFW_screen screen);
#endif

RGFWDEF void* RGFW_window_getUserPtr(RGFW_window* win); /*!< gets the userPtr of the window | returns RGFW_window.userPtr */
RGFWDEF void RGFW_window_setUserPtr(RGFW_window* win, void* ptr); /*!< sets the userPtr of the window | writes to RGFW_window.userPtr */

RGFWDEF RGFW_window_src* RGFW_window_getSrc(RGFW_window* win); /*!< returns fat pointer of window, which is sourced from the window casted to the fast pointer */


RGFWDEF RGFW_window* RGFW_createWindow(
	RGFW_videoMode mode, /* TODO(EimaMei): document */
	RGFW_windowFlags flags /* extra arguments ((u32)0 means no flags used)*/
); /*!< function to create a window and struct */

RGFWDEF RGFW_window* RGFW_createWindowPtr(
	RGFW_videoMode mode, /* TODO(EimaMei): document */
	RGFW_windowFlags flags, /* extra arguments (NULL / (u32)0 means no flags used) */
	RGFW_window* win /* ptr to the window struct you want to use */
); /*!< function to create a window (without allocating a window struct) */

/* TODO */
RGFWDEF RGFW_window* RGFW_createWindowContextless(RGFW_windowFlags flags);
/* TODO */
RGFWDEF RGFW_window* RGFW_createWindowPtrContextless(RGFW_windowFlags flags, RGFW_window* win);


/* TODO */
RGFWDEF RGFW_bool RGFW_window_createContext_buffer(RGFW_window* win, RGFW_videoMode mode,
	RGFW_pixelFormat format, RGFW_bool is_native);

/* TODO */
RGFWDEF void RGFW_window_freeContext_buffer(RGFW_window* win);


/* supports OpenGL and software rendering */
RGFWDEF void RGFW_window_swapBuffers_buffer(RGFW_window* win); /*!< swap the rendering buffer */
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_window_makeCurrent_buffer(RGFW_window* win, RGFW_context_buffer* buffer);



/* TODO */
RGFWDEF RGFW_bool RGFW_createContext_buffer(RGFW_screen screen, RGFW_videoMode mode,
	RGFW_pixelFormat format, RGFW_bool is_native, RGFW_context_buffer* out_b);
/* TODO */
RGFWDEF RGFW_bool RGFW_createContextPtr_buffer(u8* buffers[2], RGFW_screen screen,
	RGFW_videoMode mode, RGFW_pixelFormat format, RGFW_bool is_native, RGFW_context_buffer* out_b);
/* TODO */
RGFWDEF void RGFW_context_free_buffer(RGFW_context_buffer* buffer);


/* TODO(EimaMei): New function. */
RGFWDEF RGFW_bool RGFW_context_bufferSetFormat(RGFW_context_buffer* b, RGFW_pixelFormat format);
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_context_bufferSetNative(RGFW_context_buffer* b, RGFW_bool is_native);
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_context_bufferSetDoubleBuffering(RGFW_context_buffer* b, RGFW_bool enable);
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_context_bufferFlipBuffers(RGFW_context_buffer* b);

/* TODO */
RGFWDEF RGFW_screen RGFW_context_bufferGetScreen(RGFW_context_buffer* b);
/* TODO(EimaMei): New function. */
RGFWDEF u8* RGFW_context_bufferGetBuffer(RGFW_context_buffer* b);
/* TODO */
RGFWDEF ssize_t RGFW_context_bufferGetBufferCount(RGFW_context_buffer* b);
/* TODO(EimaMei): New function. */
RGFWDEF ssize_t RGFW_context_bufferGetCurrent(RGFW_context_buffer* b);
/* TODO */
RGFWDEF RGFW_area RGFW_context_bufferGetResolution(RGFW_context_buffer* b);

/* TODO(EimaMei): New function. */
RGFWDEF RGFW_pixelFormat RGFW_context_bufferGetFormat(RGFW_context_buffer* b);
/* TODO(EimaMei): New function. */
RGFWDEF RGFW_bool RGFW_context_bufferIsNative(RGFW_context_buffer* b);
/* TODO(EimaMei): New function. */
RGFWDEF RGFW_bool RGFW_context_bufferIsDoubleBuffered(RGFW_context_buffer* b);
/* TODO */
RGFWDEF RGFW_bool RGFW_context_bufferIsStereoscopic(RGFW_context_buffer* b);



/*! set the window flags (will undo flags if they don't match the old ones) */
RGFWDEF void RGFW_window_setFlags(RGFW_window* win, RGFW_windowFlags flags);


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
	RGFW_debugTypeError,
	RGFW_debugTypeWarning,
	RGFW_debugTypeInfo
};

typedef RGFW_ENUM(u8, RGFW_error) {
	RGFW_errorNone = 0,
	RGFW_errorOutOfMemory,
	RGFW_errorSystem,
	RGFW_errorOpenGLContext,
	RGFW_errorEventQueue,
	RGFW_infoWindow, /* TODO(EimaMei): move these. */
	RGFW_infoBuffer,
	RGFW_infoGlobal,
	RGFW_infoOpenGL,
	GFW_warningOpenGL,
};

typedef struct RGFW_debugContext { RGFW_window* win; u32 srcError; } RGFW_debugContext;

#if defined(__cplusplus) && !defined(__APPLE__)
#define RGFW_DEBUG_CTX(win, err) {win, err}
#else
#define RGFW_DEBUG_CTX(win, err) (RGFW_debugContext){win, err}
#endif

typedef void (* RGFW_debugFunc)(RGFW_debugType type, RGFW_error err, RGFW_debugContext ctx, const char* msg);
RGFWDEF void RGFW_sendDebugInfo(RGFW_debugType type, RGFW_error err, RGFW_debugContext ctx, const char* msg);
/** @} */


/*! TODO(EimaMei): new function. */
RGFWDEF RGFW_deviceSleepFunc RGFW_setDeviceSleepCallback(RGFW_window* win, RGFW_deviceSleepFunc func);

/*! set callback for a window quit event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_quitFunc RGFW_setWindowQuitCallback(RGFW_window* win, RGFW_quitFunc func);
/*! TODO(EimaMei): new function. */
RGFWDEF RGFW_windowRefreshFunc RGFW_setWindowRefreshCallback(RGFW_window* win, RGFW_windowRefreshFunc func);
/*! TODO(EimaMei): new function. */
RGFWDEF RGFW_windowVideoModeFunc RGFW_setWindowVideoModeCallback(RGFW_window* win, RGFW_windowVideoModeFunc func);
/*! TODO(EimaMei): new function. */
RGFWDEF RGFW_windowFocusFunc RGFW_setWindowFocusCallback(RGFW_window* win, RGFW_windowFocusFunc func);

/*! set callback for when a controller is connected or disconnected. Returns the previous callback function (if it was set) */
RGFWDEF RGFW_controllerFunc RGFW_setControllerCallback(RGFW_window* win, RGFW_controllerFunc func);

/*! set callback for a controller button (press / release) event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_buttonFunc RGFW_setButtonCallback(RGFW_window* win, RGFW_buttonFunc func);
/*! set callback for a controller axis move event. Returns previous callback function (if it was set)  */
RGFWDEF RGFW_axisFunc RGFW_setAxisCallback(RGFW_window* win, RGFW_axisFunc func);
/* TODO(EimaMei): NEW FUNCTION. */
RGFWDEF RGFW_pointerFunc RGFW_setPointerCallback(RGFW_window* win, RGFW_pointerFunc func);
/* TODO(EimaMei): NEW FUNCTION. */
RGFWDEF RGFW_motionFunc RGFW_setMotionCallback(RGFW_window* win, RGFW_motionFunc func);

/* TODO(EimaMei): new function. */
RGFWDEF RGFW_debugFunc RGFW_setDebugCallback(RGFW_debugFunc func);



#if defined(RGFW_OPENGL)

/**
 * # OpenGL
 *
 *
 * ## Description
 * This section covers OpenGL context creation, management and cleanup to enable
 * and help with OpenGL window rendering.
 *
 * ## Support
 * Current OpenGL support for all platforms:
 * - Nintendo 3DS - OpenGL ES 1.1* (see remarks).
 *
 * ## Remarks
 * TODO: Multiple screens 
 * ### Nintendo 3DS
 * TODO: GLASS, libctru fork.
*/

/**
 * OpenGL Context type
 *
 * The structure is used as an opaque data type to an OpenGL context. 
 *
 * \sa RGFW_createContext_OpenGL
 */
typedef struct RGFW_context_OpenGL RGFW_context_OpenGL;

/**
 * OpenGL context initializiation integer values 
 *
 * OpenGL hints are used to set desired parameters for when creating a new OpenGL
 * context. These are treated as 'hints' since the OpenGL driver is only obligated
 * to fufill the "minimum" requirements for some parameters (e.g., specifying a
 * 16-bit color buffer could yield a 24 or even 32 bit one). Other parameters may
 * require specific values to function (e.g., RGFW_glProfile).
 *
 * Settings hints must be done before calling RGFW_createContext_OpenGL() or
 * RGFW_createWindow().
 *
 * Most warnings and errors are implementation-defined, however some basic ones
 * are guaranteed. A warning is emitted when the specified hint value is not
 * supported but its minimum value is (e.g., user specified a 16-bit color buffer -
 * the context can only supply a 24-bit one, resulting in a warning). An error is
 * emitted when the minimum hint value cannot be provided.
 *
 * When a specific hint value is not supported but its minimum value is, the hint
 * value is changed to its minimum after the context has been created.
 * 
 * \sa RGFW_setHint_OpenGL
 * \sa RGFW_getHint_OpenGL
 */
typedef RGFW_ENUM(i32, RGFW_glHint)  {
	/* Type of OpenGL API context (RGFW_glProfile_Core by default). */
	RGFW_glProfile,
	/* OpenGL context major version (1 by default). */
	RGFW_glMajor,
	/* OpenGL context minor version (0 by default). */
	RGFW_glMinor,

	/* Minimum number of bits for the red channel of the color buffer (8 by default). */
	RGFW_glRed,
	/* Minimum number of bits for the green channel of the color buffer (8 by default). */
	RGFW_glGreen,
	/* Minimum number of bits for the blue channel of the color buffer (8 by default). */
	RGFW_glBlue,
	/* Minimum number of bits for the alpha channel of the color buffer (8 by default). */
	RGFW_glAlpha,

	/* Boolean if double buffering is used (true by defult). */
	RGFW_glDoubleBuffer,
	/* Minimum number of bits for the depth buffer (24 by default). */
	RGFW_glDepth,
	/* Minimum number of bits in the stencil buffer (0 by default). */
	RGFW_glStencil,


	/* Minimum number of bits for the red channel of the accumulation buffer (0 by default). */
	RGFW_glAccumRed,
	/* Minimum number of bits for the green channel of the accumulation buffer (0 by default). */
	RGFW_glAccumGreen,
	/* Minimum number of bits for the blue channel of the accumulation buffer (0 by default). */
	RGFW_glAccumBlue,
	/* Minimum number of bits for the alpha channel of the accumulation buffer (0 by default). */
	RGFW_glAccumAlpha,

	/* Number of samples used for multiplesample anti-alisaing (0 by default). */
	RGFW_glSamples,
	/* Number of auxiliary buffers. (0 by default). Deprecated OpenGL feature, do not use for new code. */
	RGFW_glAuxBuffers,

	/* Boolean if the renderering is done on the CPU (false by defult). */
	RGFW_glSoftwareRenderer,
	/* Boolean if the context is stereosopic 3D (false by default). */
	RGFW_glStereo,
	/* Boolean if sRGB framebuffer is used (false by default). */
	RGFW_glSRGB,
	/* Boolean if the context is robust (memory-safe) (false by default). For more information: https://registry.khronos.org/OpenGL/extensions/EXT/EXT_robustness.txt. */
	RGFW_glRobustness,
	/* Boolean if the context has additional debugging features (false by default). */
	RGFW_glDebug,
	/* Boolean if OpenGL errors have undefined behavior (false by default). Fore more information: https://registry.khronos.org/OpenGL/extensions/KHR/KHR_no_error.txt. */
	RGFW_glNoError,
	/* Boolean if the OpenGL driver should flush the graphics when changing contexts (false by default). */
	RGFW_glFlushOnContextChange,
	/* Boolean if OpenGL context sharing is enabled (false by default). */
	RGFW_glShareWithCurrentContext,

	/* Number of OpenGL hints that RGFW supports. */
	RGFW_glHintCount,
};

/**
 * OpenGL context hint values 
 *
 * Certain RGFW_glHint require a specific value instead of a regular number or
 * boolean. Any specific value follows the naming scheme of `RGFW_<glHint>_<value>`.
 * 
 * \sa RGFW_glHint
 */
typedef RGFW_ENUM(i32, RGFW_glValue)  {
	/* Nothing is done when a context is changed. */
	RGFW_glReleaseBehavior_None = 0,
	/* OpenGL flushes the previous context when changing contexts. */
	RGFW_glReleaseBehavior_Flush,

	/* OpenGL Core (OpenGL 3.2-4.6). */
	RGFW_glProfile_Core = 0,
	/* OpenGL Compatibility (OpenGL 1.0-3.1). */
	RGFW_glProfile_Compatibility,
	/* OpenGL ES. */
	RGFW_glProfile_ES
};

/**
 * Sets an OpenGL hint before creating the context.
 *
 * \param hint An OpenGL hint. If the hint is negative or surpasses RGFW_glHintCount,
 * the function panics.
 * \param value The desired value for the hint. No checking is done for the parameter.
 *
 * \sa RGFW_getHint_OpenGL
 */
RGFWDEF void RGFW_setHint_OpenGL(RGFW_glHint hint, i32 value);

/**
 * Returns the value of an OpenGL hint from the current context.
 *
 * \param hint An OpenGL hint. If the hint is negative or surpasses RGFW_glHintCount,
 * the function panics.
 *
 * \returns The value associated with the OpenGL hint.
 *
 * \sa RGFW_setHint_OpenGL
 */
RGFWDEF i32 RGFW_getHint_OpenGL(RGFW_glHint hint);

/**
 * Returns the currently binded OpenGL context.
 *
 * This function differs from RGFW_getCurrent_OpenGL()
 *
 * \returns The currently binded OpenGL context.
 */
RGFWDEF void* RGFW_getCurrentContext_OpenGL(void);

/* === RGFW_context_OpenGL === */

/* TODO */
RGFWDEF RGFW_bool RGFW_createContext_OpenGL(RGFW_screen screen, RGFW_videoMode mode, RGFW_context_OpenGL* out_gl);
/* TODO */
RGFWDEF void RGFW_context_free_OpenGL(RGFW_context_OpenGL* gl);

/* TODO */
RGFWDEF void RGFW_context_swapInterval_OpenGL(RGFW_context_OpenGL* gl, i32 swap_interval);

/* === */

/*!< create an OpenGL context for the RGFW window, run by createWindow by default (unless the RGFW_windowNoInitAPI is included) */
RGFWDEF RGFW_bool RGFW_window_createContext_OpenGL(RGFW_window* win, RGFW_videoMode mode);
/* TODO */
RGFWDEF void RGFW_window_freeContext_OpenGL(RGFW_window* win);

/* TODO */
RGFWDEF void RGFW_window_swapInterval_OpenGL(RGFW_window* win, i32 swap_interval);
/* TODO */
RGFWDEF void RGFW_window_swapBuffers_OpenGL(RGFW_window* win); /*!< swap the rendering buffer */
/* TODO(EimaMei): New function. */
RGFWDEF void RGFW_window_makeCurrent_OpenGL(RGFW_window* win, RGFW_context_OpenGL* out_gl);

#endif


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


#define RGFW_CALLBACK_DEFINE(return_type, name, member_name) \
	return_type name(RGFW_window* win, return_type func) { \
		RGFW_ASSERT(win != NULL); \
		\
		return_type previous_func = win->callbacks.member_name; \
		win->callbacks.member_name = func; \
		return previous_func; \
	}

RGFW_CALLBACK_DEFINE(RGFW_deviceSleepFunc, RGFW_setDeviceSleepCallback, sleep)

RGFW_CALLBACK_DEFINE(RGFW_quitFunc, RGFW_setWindowQuitCallback, quit)
RGFW_CALLBACK_DEFINE(RGFW_windowRefreshFunc, RGFW_setWindowRefreshCallback, refresh)
RGFW_CALLBACK_DEFINE(RGFW_windowVideoModeFunc, RGFW_setWindowVideoModeCallback, video_mode)
RGFW_CALLBACK_DEFINE(RGFW_windowFocusFunc, RGFW_setWindowFocusCallback, focus)

RGFW_CALLBACK_DEFINE(RGFW_controllerFunc, RGFW_setControllerCallback, controller)

RGFW_CALLBACK_DEFINE(RGFW_buttonFunc, RGFW_setButtonCallback, button)
RGFW_CALLBACK_DEFINE(RGFW_axisFunc, RGFW_setAxisCallback, axis)
RGFW_CALLBACK_DEFINE(RGFW_pointerFunc, RGFW_setPointerCallback, pointer)
RGFW_CALLBACK_DEFINE(RGFW_motionFunc, RGFW_setMotionCallback, motion)

RGFW_debugFunc RGFW_debugFuncSrc;
RGFW_debugFunc RGFW_setDebugCallback(RGFW_debugFunc func) {
	RGFW_debugFunc previous = RGFW_debugFuncSrc;
	RGFW_debugFuncSrc = func;
	return previous;
}

#define RGFW_windowDeviceSleepCallback(win, is_sleeping) \
	if (win->callbacks.sleep) { \
		win->callbacks.sleep(win, is_sleeping); \
	} do {} while(0)

#define RGFW_windowQuitCallback(win) \
	if (win->callbacks.quit) { \
		win->callbacks.quit(win); \
	} do {} while(0)

#define RGFW_windowRefreshCallback(win) \
	if (win->callbacks.refresh) { \
		win->callbacks.refresh(win); \
	} do {} while(0)

#define RGFW_windowVideoModeCallback(win, video_mode) \
	if (win->callbacks.video_mode) { \
		win->callbacks.video_mode(win, video_mode); \
	} do {} while(0)

#define RGFW_windowFocusCallback(win, is_focused) \
	if (win->callbacks.focus) { \
		win->callbacks.focus(win, is_focused); \
	} do {} while(0)

#define RGFW_windowControllerCallback(win, controller_s, is_connected) \
	if (win->callbacks.controller) { \
		win->callbacks.controller(win, controller_s, is_connected); \
	} do {} while(0)

#define RGFW_windowButtonCallback(win, controller, button_s, pressed) \
	do { \
	if (win->callbacks.button) { \
		win->callbacks.button(win, controller, button_s, pressed); \
	} \
	} while(0)

#define RGFW_windowAxisCallback(win, controller, type) \
	if (win->callbacks.axis) { \
		win->callbacks.axis(win, controller, type); \
	} do {} while(0)

#define RGFW_windowPointerCallback(win, controller, type) \
	if (win->callbacks.pointer) { \
		win->callbacks.pointer(win, controller, type); \
	} do {} while(0)

#define RGFW_windowMotionCallback(win, controller, type) \
	if (win->callbacks.motion) { \
		win->callbacks.motion(win, controller, type); \
	} do {} while(0)

#ifdef RGFW_DEBUG
#include <stdio.h>
#endif

void RGFW_sendDebugInfo(RGFW_debugType type, RGFW_error err, RGFW_debugContext ctx, const char* msg) {
	#if defined(RGFW_3DS) && defined(RGFW_OPENGL)
	if (RGFW_getHint_OpenGL(RGFW_glNoError)) { return ; }
	#endif

	if (RGFW_debugFuncSrc) {
		RGFW_debugFuncSrc(type, err, ctx, msg);
		return;
	}

	#ifdef RGFW_DEBUG
	switch (type) {
		case RGFW_debugTypeInfo: RGFW_PRINTF("RGFW INFO (%i %i): %s", type, err, msg); break;
		case RGFW_debugTypeError: RGFW_PRINTF("RGFW ERROR (%i %i): %s", type, err, msg); break;
		case RGFW_debugTypeWarning: RGFW_PRINTF("RGFW WARNING (%i %i): %s", type, err, msg); break;
		default: break;
	}

	switch (err) {
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


size_t RGFW_sizeofWindow(void) { return sizeof(RGFW_window); }
size_t RGFW_sizeofWindowSrc(void) { return sizeof(RGFW_window_src); }


RGFW_context_buffer* RGFW_window_getCurrent_buffer(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->buffer;
}

RGFW_context_buffer* RGFW_window_getContext_buffer(RGFW_window* win) {
	#ifndef RGFW_3DS
	RGFW_ASSERT(win != NULL);
	return &win->src.buffer;
	#else
	return RGFW_window_getContextEx_buffer(win, RGFW_screenPrimary);
	#endif
}

RGFW_context_buffer* RGFW_window_getContextEx_buffer(RGFW_window* win, RGFW_screen screen) {
	RGFW_ASSERT(win != NULL);
	RGFW_ASSERT(screen >= 0 && screen < RGFW_screenCount);

	#ifndef RGFW_3DS
	return &win->src.buffer;
	#else
	return &win->src.buffer[screen];
	#endif
}

#ifdef RGFW_OPENGL

RGFW_context_OpenGL* RGFW_window_getCurrent_OpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	return win->gl;
}

RGFW_context_OpenGL* RGFW_window_getContext_OpenGL(RGFW_window* win) {
	#ifndef RGFW_3DS
	RGFW_ASSERT(win != NULL);
	return &win->src.gl;
	#else
	return RGFW_window_getContextEx_OpenGL(win, RGFW_screenPrimary);
	#endif
}

RGFW_context_OpenGL* RGFW_window_getContextEx_OpenGL(RGFW_window* win, RGFW_screen screen) {
	RGFW_ASSERT(win != NULL);
	RGFW_ASSERT(screen >= 0 && screen < RGFW_screenCount);

	#ifndef RGFW_3DS
	return &win->src.gl;
	#else
	return &win->src.gl[screen];
	#endif
}

#endif

void* RGFW_window_getUserPtr(RGFW_window* win) { RGFW_ASSERT(win != NULL); return win->userPtr; }
void RGFW_window_setUserPtr(RGFW_window* win, void* ptr) { RGFW_ASSERT(win != NULL); win->userPtr = ptr; }

RGFW_window_src* RGFW_window_getSrc(RGFW_window* win) { RGFW_ASSERT(win != NULL); return &win->src; }

/* TODO(EimaMei): remove these. */
#define RGFW_EVENT_QUIT 		RGFW_BIT(25) /* the window close button was pressed */
#define RGFW_WINDOW_ALLOC 		RGFW_BIT(28) /* if window was allocated by RGFW */
#define RGFW_INTERNAL_FLAGS (RGFW_EVENT_QUIT | RGFW_WINDOW_ALLOC)

RGFW_window* RGFW_createWindow(RGFW_videoMode mode, RGFW_windowFlags flags) {
	RGFW_window* win = (RGFW_window*)RGFW_ALLOC(sizeof(RGFW_window));
	if (win == NULL) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOutOfMemory, RGFW_DEBUG_CTX(NULL, 0), "Failed to allocate a window.");
		return NULL;
	}

	return RGFW_createWindowPtr(mode, flags | RGFW_WINDOW_ALLOC, win);
}

RGFW_window* RGFW_createWindowContextless(RGFW_windowFlags flags) {
	RGFW_ASSERT((flags & (RGFW_windowOpenGL | RGFW_windowBuffer)) == 0);
	return RGFW_createWindow(0, flags);
}

RGFW_window* RGFW_createWindowPtrContextless(RGFW_windowFlags flags, RGFW_window* win) {
	return RGFW_createWindowPtr(0, flags, win);
}

RGFWDEF RGFW_bool RGFW_create_window_platform(RGFW_window* win);

RGFW_window* RGFW_createWindowPtr(RGFW_videoMode mode, RGFW_windowFlags flags,
		RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	win->eventLen = 0;
	win->stopCheckEvents_bool = RGFW_FALSE;
	win->_flags = flags;

	win->buffer = NULL;
	#ifdef RGFW_OPENGL
	win->gl = NULL;
	#endif

	win->controllerCount = 0;
	RGFW_MEMSET(win->controllers, 0, sizeof(win->controllers));
	RGFW_MEMSET(&win->callbacks, 0, sizeof(win->callbacks));

	RGFW_bool res = RGFW_create_window_platform(win);
	if (res == false) { return NULL; }	
	RGFW_window_setFlags(win, win->_flags);

	switch (win->_flags & (RGFW_windowBuffer | RGFW_windowOpenGL)) {
		case 0: break;
		case RGFW_windowBuffer: {
			res = RGFW_window_createContext_buffer(win, mode, RGFW_pixelFormatRGBA8, RGFW_FALSE); 
		} break;

		#ifdef RGFW_OPENGL
		case RGFW_windowOpenGL: {
			res = RGFW_window_createContext_OpenGL(win, mode);
		} break;
		#endif

		default: RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorSystem, RGFW_DEBUG_CTX(win, 0), "Multiple graphical APIs were specified. Creating nothing.");
	}

	if (!res) {
		RGFW_window_close(win);
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorSystem, RGFW_DEBUG_CTX(win, 0), "Failed to create a graphical context.");
		return NULL;
	}

	return win;
}

RGFWDEF void RGFW_window_close_platform(RGFW_window* win);

void RGFW_window_close(RGFW_window* win) {
	RGFW_window_freeContext_buffer(win);
	#ifdef RGFW_OPENGL
	RGFW_window_freeContext_OpenGL(win);
	#endif

	RGFW_window_close_platform(win);

	if ((win->_flags & RGFW_WINDOW_ALLOC)) {
		RGFW_FREE(win);
	}
}


RGFW_event* RGFW_eventQueuePush(RGFW_window* win) {
	RGFW_ASSERT(win->eventLen >= 0);

	if (win->eventLen >= RGFW_COUNTOF(win->events)) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorEventQueue, RGFW_DEBUG_CTX(NULL, 0), "Event queue limit 'RGFW_MAX_EVENTS' has been reached.");
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
	win->_flags = flags | (win->_flags & RGFW_INTERNAL_FLAGS);
}

RGFW_bool RGFW_window_createContext_buffer(RGFW_window* win, RGFW_videoMode mode,
		RGFW_pixelFormat format, RGFW_bool is_native) {
	RGFW_ASSERT(win != NULL);
	RGFW_window_freeContext_buffer(win);

	#if RGFW_screenCount == 1
		#error "TODO"
		RGFW_bool res = ...;
		RGFW_window_makeCurrent_buffer(win, win->src.buffer);
	#else
		for (RGFW_screen screen = 0; screen < RGFW_screenCount; screen += 1) {		
			RGFW_bool res = RGFW_createContext_buffer(screen, mode, format, is_native, &win->src.buffer[screen]);
			if (res == RGFW_FALSE) { return RGFW_FALSE; }
		}

		#ifdef RGFW_3DS
		RGFW_window_makeCurrent_buffer(win, &win->src.buffer[!(win->_flags & RGFW_windowTopScreen)]);
		#endif
	#endif

	if (!win->src.lcd_is_on) {
		gspWaitForVBlank();
		GSPGPU_SetLcdForceBlack(RGFW_FALSE);
		win->src.lcd_is_on = RGFW_TRUE;
	}

	win->_flags |= RGFW_windowBuffer;
	return RGFW_TRUE;
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

#if defined(RGFW_OPENGL)

i32 RGFW_GL_HINTS[RGFW_glHintCount] = {
	/* RGFW_glProfile                 */ RGFW_glProfile_Core,
	/* RGFW_glMajor                   */ 1,
	/* RGFW_glMinor                   */ 0,

	/* RGFW_glRed                     */ 8,
	/* RGFW_glGreen                   */ 8,
	/* RGFW_glBlue                    */ 8,
	/* RGFW_glAlpha                   */ 8,

	/* RGFW_glDoubleBuffer            */ RGFW_TRUE,
	/* RGFW_glDepth                   */ 24,
	/* RGFW_glStencil                 */ 0,

	/* RGFW_glAccumRed                */ 0,
	/* RGFW_glAccumGreen              */ 0,
	/* RGFW_glAccumBlue               */ 0,
	/* RGFW_glAccumAlpha              */ 0,

	/* RGFW_glSamples                 */ 0,
	/* RGFW_glAuxBuffers              */ 0,

	/* RGFW_glSoftwareRenderer        */ RGFW_FALSE,
	/* RGFW_glStereo                  */ RGFW_FALSE,
	/* RGFW_glSRGB                    */ RGFW_FALSE,
	/* RGFW_glRobustness              */ RGFW_FALSE,
	/* RGFW_glDebug                   */ RGFW_FALSE,
	/* RGFW_glNoError                 */ RGFW_FALSE,
	/* RGFW_glFlushOnContextChange    */ RGFW_FALSE,
	/* RGFW_glShareWithCurrentContext */ RGFW_FALSE
};

void RGFW_setHint_OpenGL(RGFW_glHint hint, i32 value) {
	RGFW_ASSERT(hint >= 0 && hint < RGFW_glHintCount);
	RGFW_GL_HINTS[hint] = value;
}

i32 RGFW_getHint_OpenGL(RGFW_glHint hint) {
	RGFW_ASSERT(hint >= 0 && hint < RGFW_glHintCount);
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

void RGFW__aptHookCallback(APT_HookType hook, void* param);
void RGFW__aptHookCallback(APT_HookType hook, void* param) {
	RGFW_window* win = param;

	static const RGFW_eventType APT_HOOK_LUT[APTHOOK_COUNT] = {
		RGFW_windowFocusOut,
		RGFW_windowFocusIn,
		RGFW_deviceSleep,
		RGFW_deviceWakeup,
		RGFW_quit
	};

	// todo> maybe add a way to "Add focus" by not allowing to press home.
	RGFW_event* event = RGFW_eventQueuePush(win);
	if (event == NULL) { return; }
	event->type = APT_HOOK_LUT[hook];

	if (hook <= APTHOOK_ONRESTORE) {
		RGFW_windowFocusCallback(win, event->type == RGFW_windowFocusIn);
	}
	else if (hook <= APTHOOK_ONWAKEUP) {
		RGFW_windowDeviceSleepCallback(win, event->type == RGFW_deviceSleep);
	}
	else {
		RGFW_windowQuitCallback(win);
	}
}

RGFW_bool RGFW_create_window_platform(RGFW_window* win) {
	win->controllerCount = 1;
	if ((win->_flags & RGFW_windowDualScreen) == 0) {
		win->_flags |= RGFW_windowTopScreen;
	}

	RGFW_controller* controller = &win->controllers[0];
	controller->connected = RGFW_TRUE;
	controller->enabled_pointers[RGFW_pointerTouchscreen] = RGFW_TRUE;
	controller->button_start = 0;
	controller->button_end = RGFW_controllerButtonCount;

	RGFW_window_src* src = &win->src;
	src->has_checked_events = RGFW_FALSE;
	src->lcd_is_on = RGFW_FALSE;

	for (RGFW_screen i = 0; i < RGFW_screenCount; i += 1) {
		RGFW_context_buffer* b = &src->buffer[i];
		b->buffers[0] = NULL;
		b->buffers[1] = NULL;
		b->format = -1;
		b->is_double_buffered = RGFW_TRUE;
		b->is_buffer_allocated = RGFW_FALSE;

		#ifdef RGFW_OPENGL
		RGFW_context_OpenGL* gl = &src->gl[i];
		gl->ctx = NULL;
		gl->framebuffer = gl->renderbuffer = 0;
		#endif
	}

	aptHook(&src->apt_hook, RGFW__aptHookCallback, win);

	Result res = gspInit();
	if (res != 0) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorSystem, RGFW_DEBUG_CTX(win, 0), "Failed to initialize GSP.");
		return RGFW_FALSE;
	}

	#ifdef RGFW_OPENGL
	gfxInitDefault();
	RGFW_bool kygx_init = kygxInit();
	if (!kygx_init) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Failed to initialize KYGX.");
		return RGFW_FALSE;
	}
	#endif

	return RGFW_TRUE;
}

/* NOTE(EimaMei): Taken from libctru gfx.c */
void _RGFW__gspPresentFramebuffer(RGFW_context_buffer* b, u8* buffer);

void _RGFW__gspPresentFramebuffer(RGFW_context_buffer* b, u8* buffer) {
	u32 stride = GSP_SCREEN_WIDTH * (u32)RGFW_pixelFormatBPP(b->format);
	u32 pixel_format = (u32)b->format | (1 << 8);

	u8* fb_b = buffer;
	if (b->screen == 0) {
		switch (b->mode) {
		case RGFW_videoMode2D: {
			pixel_format |= BIT(6);
		} break;

		case RGFW_videoMode3D: {
			pixel_format |= BIT(5);
			if (RGFW_platformGet3DSlider() > 0.0f) fb_b += b->size / 2;
		} break;
		}
	}

	gspPresentBuffer((u32)b->screen, (u32)b->current, buffer, fb_b, stride, pixel_format);
}

#ifndef RGFW_BUFFER_NO_CONVERSION

u8* RGFW_window_bufferToNative(RGFW_context_buffer* b);
u8* RGFW_window_bufferToNative(RGFW_context_buffer* b) {
	u8* src = RGFW_context_bufferGetBuffer(b);

	if (b->is_native) {
		return src;
	}
	u8* dst = b->buffers_native[b->current];

	const i32 bpp = RGFW_pixelFormatBPP(b->mode);
	const ssize_t width  = RGFW_context_bufferGetResolution(b).w,
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

RGFW_bool RGFW_createContext_buffer(RGFW_screen screen, RGFW_videoMode mode,
		RGFW_pixelFormat format, RGFW_bool is_native, RGFW_context_buffer* out_b) {
	RGFW_area area = (screen == RGFW_screenTop)
		? RGFW_videoModeResolution(mode)
		: RGFW_AREA(320, 240);
	ssize_t size = area.w * area.h * RGFW_pixelFormatBPP(format);

	u8* buffers = (u8*)RGFW_ALLOC_SYS((size_t)(2 * size));
	if (buffers == NULL) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOutOfMemory, RGFW_DEBUG_CTX(NULL, (u32)screen), "Ran out of memory when allocating a buffer.");
		return RGFW_FALSE;
	}

	u8* buffers_list[2];
	buffers_list[0] = &buffers[0 * size];
	buffers_list[1] = &buffers[1 * size];
	out_b->is_buffer_allocated = RGFW_TRUE;

	return RGFW_createContextPtr_buffer(buffers_list, screen, mode, format, is_native, out_b);
}

RGFW_bool RGFW_createContextPtr_buffer(u8* buffers[2], RGFW_screen screen, RGFW_videoMode mode,
		RGFW_pixelFormat format, RGFW_bool is_native, RGFW_context_buffer* out_b) {
	RGFW_ASSERT(out_b != NULL);
	RGFW_ASSERT(buffers[0] != NULL);
	RGFW_ASSERT(screen >= 0 && screen < RGFW_screenCount);
	RGFW_ASSERT(mode >= 0 && mode < RGFW_videoModeCount);
	RGFW_ASSERT(format >= 0 && format < RGFW_pixelFormatCount);

	RGFW_area area = (screen == RGFW_screenTop)
		? RGFW_videoModeResolution(mode)
		: RGFW_AREA(240, 320);
	RGFW_context_buffer* b = out_b;
	b->screen = screen;
	b->mode = mode;
	b->format = format;
	b->current = 0;
	b->size = (u32)(area.w * area.h * RGFW_pixelFormatBPP(format));
	b->buffers[0] = buffers[0];
	b->buffers[1] = buffers[1];
	b->is_native = is_native;

	#ifndef RGFW_BUFFER_NO_CONVERSION
	if (!is_native) {
		for (size_t i = 0; i < 2; i += 1) {
			b->buffers_native[i] = RGFW_ALLOC_SYS(b->size);

			if (b->buffers_native[i] == NULL) {
				RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOutOfMemory, RGFW_DEBUG_CTX(NULL, 0), "Ran out of memory allocating the native buffers.");
				return RGFW_FALSE;
			}
		}
	}
	_RGFW__gspPresentFramebuffer(b, b->buffers_native[0]);
	#else
	_RGFW__gspPresentFramebuffer(b, b->buffers[0]);
	#endif

	RGFW_sendDebugInfo(RGFW_debugTypeInfo, RGFW_infoBuffer, RGFW_DEBUG_CTX(0, 0), "Creating framebuffers");

	return RGFW_TRUE;
}

void RGFW_window_freeContext_buffer(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	if (win->_flags & RGFW_windowBuffer) {
		for (RGFW_screen screen = 0; screen < RGFW_screenCount; screen += 1) {
			RGFW_context_buffer* b = &win->src.buffer[screen];
			if (win->buffer == b) {
				RGFW_window_makeCurrent_buffer(win, NULL);
			}
			RGFW_context_free_buffer(b);
		}
		win->_flags &= ~(RGFW_windowFlags)RGFW_windowBuffer;
	}

	win->buffer = NULL;
}

void RGFW_context_free_buffer(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);

	if (b->is_buffer_allocated) {
		RGFW_FREE_SYS(b->buffers[0]);
	}

	#ifndef RGFW_BUFFER_NO_CONVERSION
	if (!b->is_native) {
		RGFW_FREE_SYS(b->buffers_native[0]);
	}
	#endif

	b->buffers[0] = NULL;
	b->buffers[1] = NULL;
}

void RGFW_window_swapBuffers_buffer(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	for (RGFW_screen screen = 0; screen < RGFW_screenCount; screen += 1) {
		RGFW_context_buffer* b = &win->src.buffer[screen];
		#ifndef RGFW_BUFFER_NO_CONVERSION
		u8* buffer = RGFW_window_bufferToNative(b);
		#else
		u8* buffer = b->buffers[b->current];
		#endif
		GSPGPU_FlushDataCache(buffer, b->size);

		_RGFW__gspPresentFramebuffer(b, buffer);
		b->current ^= b->is_double_buffered;
	}

	gspWaitForVBlank();
}

void RGFW_window_makeCurrent_buffer(RGFW_window* win, RGFW_context_buffer* buffer) {
	win->buffer = buffer;
}

RGFW_bool RGFW_context_bufferSetFormat(RGFW_context_buffer* b, RGFW_pixelFormat format) {
	if (b == NULL) { return RGFW_FALSE; }

	/* TODO(EimaMei): Have a way to save the original bpp for performance reasons. */
	if (RGFW_pixelFormatBPP(b->format) < RGFW_pixelFormatBPP(format)) {
		RGFW_context_free_buffer(b);
		return RGFW_createContext_buffer(b->screen, b->mode, format, b->is_native, b);
	}
	else {
		b->format = format;
	}

	return RGFW_TRUE;
}

void RGFW_context_bufferSetNative(RGFW_context_buffer* b, RGFW_bool is_native) {
	RGFW_ASSERT(b != NULL);
	b->is_native = RGFW_BOOL(is_native);

	#ifndef RGFW_BUFFER_NO_CONVERSION
	if (is_native == RGFW_FALSE) {
		RGFW_FREE_SYS(b->buffers_native[0]);
	}
	#endif
}

void RGFW_context_bufferSetDoubleBuffering(RGFW_context_buffer* b, RGFW_bool is_double_buffered) {
	RGFW_ASSERT(b != NULL);
	b->is_double_buffered = RGFW_BOOL(is_double_buffered);
}

void RGFW_context_bufferFlipBuffers(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	b->current ^= 1;
}

RGFW_screen RGFW_context_bufferGetScreen(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	#ifndef RGFW_3DS
	return RGFW_screenPrimary;
	#else
	return b->screen;
	#endif
}

u8* RGFW_context_bufferGetBuffer(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	return b->buffers[b->current];
}

ssize_t RGFW_context_bufferGetBufferCount(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	return b->is_double_buffered ? 2 : 1;	
}

RGFW_pixelFormat RGFW_context_bufferGetFormat(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	return b->format;
}

ssize_t RGFW_context_bufferGetCurrent(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	return b->current;
}

RGFW_area RGFW_context_bufferGetResolution(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);

	if (RGFW_context_bufferIsNative(b)) {
		return (b->screen == RGFW_screenTop)
			? RGFW_videoModeResolution(b->mode)
			: RGFW_AREA(240, 320);
	}
	else {
		RGFW_area area = RGFW_videoModeResolution(b->mode);
		return (b->screen == RGFW_screenTop)
			? RGFW_AREA(area.h, area.w)
			: RGFW_AREA(320, 240);
	}
}

RGFW_bool RGFW_context_bufferIsNative(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	return b->is_native;
}

RGFW_bool RGFW_context_bufferIsDoubleBuffered(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	return b->is_double_buffered;
}

RGFW_bool RGFW_context_bufferIsStereoscopic(RGFW_context_buffer* b) {
	RGFW_ASSERT(b != NULL);
	#ifndef RGFW_3DS
	return RGFW_FALSE;
	#else
	return b->screen == RGFW_screenTop && b->mode == RGFW_videoMode3D;
	#endif
}


#ifdef RGFW_OPENGL

RGFW_bool RGFW_createContext_OpenGL(RGFW_screen screen, RGFW_videoMode mode, RGFW_context_OpenGL* out_gl) {
	RGFW_ASSERT(screen >= 0 && screen <= RGFW_screenCount);
	RGFW_ASSERT(out_gl != NULL);

	if (RGFW_getHint_OpenGL(RGFW_glProfile) != RGFW_glProfile_ES) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "3DS only supports GLES.");
		return RGFW_FALSE;
	}

	if (RGFW_getHint_OpenGL(RGFW_glMajor) != 1 || (RGFW_getHint_OpenGL(RGFW_glMinor) != 0 && RGFW_getHint_OpenGL(RGFW_glMinor) != 1)) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "3DS can only support up to GLES 1.1.");
		return RGFW_FALSE;
	}

	i32 stencil = RGFW_getHint_OpenGL(RGFW_glStencil);
	if (stencil > 8) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported stencil buffer size.");
		return RGFW_FALSE;
	}
	else if (stencil != 0 && stencil != 8) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported stencil buffer size. Defaulting to 8.");
		RGFW_setHint_OpenGL(RGFW_glStencil, 8);
	}

	i32 depth = RGFW_getHint_OpenGL(RGFW_glStencil);
	if (depth > 24) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported depth buffer size.");
		return RGFW_FALSE;
	}
	else if (stencil != 0 && stencil != 24) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported depth buffer size. Defaulting to 24.");
		RGFW_setHint_OpenGL(RGFW_glStencil, 24);
	}

	i32 r = RGFW_getHint_OpenGL(RGFW_glRed),
		g = RGFW_getHint_OpenGL(RGFW_glGreen),
		b = RGFW_getHint_OpenGL(RGFW_glBlue),
		a = RGFW_getHint_OpenGL(RGFW_glAlpha);

	GLenum internal_format;
	if (a == 0) {
		if (r <= 5 && g <= 6 && b <= 5) {
			internal_format = GL_RGB565;
			out_gl->format = RGFW_pixelFormatRGB565;

			if (r != 5 || g != 6 || b != 5) {
				RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGB565.");
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGB8_OES;
			out_gl->format = RGFW_pixelFormatBGR8;

			if (r != 8 || g != 8 || b != 8) {
				RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGB8.");
			}
		}
		else {
			RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return RGFW_FALSE;
		}
	}
	else if (a == 1) {
		if (r <= 5 && g <= 5 && b <= 5) {
			internal_format = GL_RGB5_A1;
			out_gl->format = RGFW_pixelFormatRGB5_A1;

			if (r != 5 || g != 5 || b != 5) {
				RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGB5_A1.");
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA8_OES;
			out_gl->format = RGFW_pixelFormatRGBA8;
			RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA8.");
		}
		else {
			RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return RGFW_FALSE;
		}
	}
	else if (a <= 4) {
		if (r <= 4 && g <= 4 && b <= 4) {
			internal_format = GL_RGBA4;
			out_gl->format = RGFW_pixelFormatRGBA4;

			if (r != 4 || g != 4 || b != 4 || a != 4) {
				RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA4.");
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA4;
			out_gl->format = RGFW_pixelFormatRGBA4;

			RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA8.");
		}
		else {
			RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return RGFW_FALSE;
		}
	}
	else if (a <= 8) {
		if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA8_OES;
			out_gl->format = RGFW_pixelFormatRGBA8;

			if (r != 8 || g != 8 || b != 8 || a != 8) {
				RGFW_sendDebugInfo(RGFW_debugTypeWarning, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA8.");
			}
		}
		else {
			RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return RGFW_FALSE;
		}
	}
	else {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
		return RGFW_FALSE;
	}

	out_gl->mode = (RGFW_getHint_OpenGL(RGFW_glStereo) && screen == RGFW_screenTop)
		? RGFW_videoMode3D
		: mode;

	GLASSCtxParams param;
	glassGetDefaultContextParams(&param, GLASS_VERSION_ES_2);
	param.targetScreen = (GLASSScreen)screen;

	out_gl->ctx = glassCreateContext(&param);
	if (out_gl->ctx == NULL) {
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(NULL, 0), "Failed to create a GLASS context.");
		return RGFW_FALSE;
	}
	glassBindContext(out_gl->ctx);

	GLint width = (screen == RGFW_screenTop)
		? RGFW_videoModeResolution(out_gl->mode).h
		: 320;
	glGenRenderbuffers(1, &out_gl->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, out_gl->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, 240);

	glGenFramebuffers(1, &out_gl->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, out_gl->framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, out_gl->renderbuffer);

	glViewport(0, 0, width, 240);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return RGFW_TRUE;
}

void RGFW_context_free_OpenGL(RGFW_context_OpenGL* out_gl) {
	RGFW_ASSERT(out_gl != NULL);
	if (out_gl->ctx == NULL) { return; }

	glassBindContext(out_gl->ctx);
	glDeleteRenderbuffers(1, &out_gl->renderbuffer);
	glDeleteFramebuffers(1, &out_gl->framebuffer);

	glassDestroyContext(out_gl->ctx);
	glassBindContext(NULL);
	out_gl->ctx = NULL;
}

void RGFW_context_swapInterval_OpenGL(RGFW_context_OpenGL* gl, i32 swap_interval) {
	RGFW_ASSERT(gl != NULL);
	if (gl->ctx) {
		glassSetVSync(gl->ctx, RGFW_BOOL(swap_interval));
	}
}


RGFW_bool RGFW_window_createContext_OpenGL(RGFW_window* win, RGFW_videoMode mode) {
	RGFW_ASSERT(win != NULL);

	RGFW_bool res;
	switch (win->_flags & RGFW_windowDualScreen) {
		case RGFW_windowTopScreen:    res = RGFW_createContext_OpenGL(RGFW_screenTop, mode, &win->src.gl[RGFW_screenTop]); break;
		case RGFW_windowBottomScreen: res = RGFW_createContext_OpenGL(RGFW_screenBottom, mode, &win->src.gl[RGFW_screenBottom]); break;
		case RGFW_windowDualScreen: {
			res = RGFW_createContext_OpenGL(RGFW_screenBottom, mode, &win->src.gl[RGFW_screenBottom]);
			res = RGFW_createContext_OpenGL(RGFW_screenTop, mode, &win->src.gl[RGFW_screenTop]);
		} break;

		default: {
			RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext, RGFW_DEBUG_CTX(win, 0), "No screen was specified to create an OpenGL context.");
			return RGFW_FALSE;
		}
	}

	if (!res) {
		return RGFW_FALSE;
	}
	RGFW_window_makeCurrent_OpenGL(win, &win->src.gl[!(win->_flags & RGFW_windowTopScreen)]);

	win->_flags |= RGFW_windowOpenGL;
	return res;
}

void RGFW_window_freeContext_OpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);

	if ((win->_flags & RGFW_windowOpenGL) == 0) {
		return;
	}
	
	if (win->gl == &win->src.gl[RGFW_screenTop] || win->gl == &win->src.gl[RGFW_screenBottom]) {
		RGFW_window_makeCurrent_OpenGL(win, NULL);
	}

	switch (win->_flags & RGFW_windowDualScreen) {
		case RGFW_windowTopScreen: RGFW_context_free_OpenGL(&win->src.gl[RGFW_screenTop]); break;
		case RGFW_windowBottomScreen: RGFW_context_free_OpenGL(&win->src.gl[RGFW_screenBottom]); break;
		case RGFW_windowDualScreen: {
			RGFW_context_free_OpenGL(&win->src.gl[RGFW_screenTop]); 
			RGFW_context_free_OpenGL(&win->src.gl[RGFW_screenBottom]);
		} break;
	}

	win->_flags &= ~(RGFW_windowFlags)RGFW_windowOpenGL;
	RGFW_sendDebugInfo(RGFW_debugTypeInfo, RGFW_infoOpenGL, RGFW_DEBUG_CTX(win, 0), "OpenGL context freed.");
}

void RGFW_window_swapInterval_OpenGL(RGFW_window* win, i32 swap_interval) {
	RGFW_ASSERT(win != NULL);
	RGFW_context_swapInterval_OpenGL(win->gl, swap_interval);
}

void RGFW_window_swapBuffers_OpenGL(RGFW_window* win) {
	RGFW_ASSERT(win != NULL);
	glassSwapContextBuffers(win->src.gl[RGFW_screenTop].ctx, win->src.gl[RGFW_screenBottom].ctx);
}

void RGFW_window_makeCurrent_OpenGL(RGFW_window* win, RGFW_context_OpenGL* out_gl) {
	RGFW_ASSERT(win != NULL);

	win->gl = out_gl;
	glassBindContext(win->gl ? win->gl->ctx : NULL);
}


void* RGFW_getCurrentContext_OpenGL(void) {
	return glassGetBoundContext();
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

RGFW_videoMode RGFW_videoModeOptimal(void) {
	return RGFW_videoMode2D;
}

RGFW_videoMode RGFW_videoModeHighest(void) {
	return (RGFW_platformGetModel() != RGFW_systemModel2DS) ? RGFW_videoModeWide : RGFW_videoMode2D;
}

RGFW_area RGFW_videoModeResolution(RGFW_videoMode mode) {
	RGFW_ASSERT(mode >= 0 && mode <= RGFW_videoModeCount);

	static const i32 WIDTH_LUT[RGFW_videoModeCount] = {400, 800, 800};
	return RGFW_AREA(240, WIDTH_LUT[mode]);
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
		RGFW_bool res = RGFW_window_createContext_buffer(win, RGFW_videoModeOptimal(), RGFW_pixelFormatRGB565, RGFW_TRUE);

		if (res == RGFW_FALSE) {
			return RGFW_FALSE;
		}
	}
	RGFW_context_bufferSetNative(win->buffer, RGFW_TRUE);
	RGFW_context_bufferSetFormat(win->buffer, RGFW_pixelFormatRGB565);
	RGFW_context_bufferSetDoubleBuffering(win->buffer, RGFW_FALSE);

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
	currentConsole->frameBuffer = (u16*)(void*)win->buffer->buffers[0];

	/* NOTE(EimaMei): Taken from libctru console.c */
	if (win->buffer->screen == RGFW_screenTop) {
		RGFW_bool is_wide = (win->buffer->mode == RGFW_videoModeWide);
		currentConsole->consoleWidth = is_wide ? 100 : 50;
		currentConsole->windowWidth = is_wide ? 100 : 50;
	}

	/* TODO(EimaMei): Should be kept in the library. */
	/*if ((win->_flags & RGFW_windowDualScreen) == RGFW_windowDualScreen) {
		win->current ^= 1;
	}*/

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

			RGFW_windowPointerCallback(win, ev->controller, ev->motion);
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

			RGFW_windowPointerCallback(win, ev->controller, ev->motion);
		}
	}

	while (pressed & (RGFW_CPAD_BITS_H | RGFW_CPAD_BITS_V)) {
		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
		ev->type = RGFW_axisMove;

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
		RGFW_windowAxisCallback(win, ev->controller, ev->axis);
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

		RGFW_windowButtonCallback(win, ev->controller, ev->button, RGFW_TRUE);
	}

	while (released & RGFW_ACCEPTED_CTRU_INPUTS) {
		RGFW_button button = RGFW_apiKeyToRGFW(released);
		RGFW_ASSERT(button != -1);

		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
		ev->type = RGFW_buttonReleased;
		ev->button = button;

		RGFW_buttonState* state = &ev->controller->buttons[ev->button];
		RGFW_MASK_SET(*state, RGFW_buttonStatePrevious, RGFW_TRUE);
		RGFW_MASK_SET(*state, RGFW_buttonStateCurrent, RGFW_FALSE);
		released &= ~RGFW_rgfwToApiKey(button);

		RGFW_windowButtonCallback(win, ev->controller, ev->button, RGFW_FALSE);
	}

	if (controller->enabled_pointers[RGFW_pointerTouchscreen] && pressed & KEY_TOUCH) {
		touchPosition touch;
		hidTouchRead(&touch);

		RGFW_event* ev = RGFW_eventQueuePush(win);
		ev->controller = controller;
		ev->type = RGFW_pointerMove;
		ev->controller->pointers[RGFW_pointerTouchscreen] = RGFW_POINT(touch.px, touch.py);
		pressed &= (u32)~KEY_TOUCH;

		RGFW_windowPointerCallback(win, ev->controller, RGFW_pointerTouchscreen);
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
	kygxExit();
	gfxExit();
	#endif
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
		RGFW_sendDebugInfo(RGFW_debugTypeError, RGFW_errorOpenGLContext,  RGFW_DEBUG_CTX(NULL, 0), "Invalid uniform name.");
		return RGFW_FALSE;
	}

	glUniformMatrix4fv(uniform, 1, GL_FALSE, (const float*)deg90_rotation_matrix);
	return RGFW_TRUE;
}

#endif

#endif

#endif /* RGFW_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#if _MSC_VER
	#pragma warning( pop )
#endif


#endif /* RGFW_HEADER */
