/*
SGFE.h - v0.0.0 - a general-purpose framework library for porting desktop
applications to consoles.
===========================================================================
	- YOU MUST DEFINE 'SI_IMPLEMENTATION' in EXACTLY _one_ C file that includes
	this header, BEFORE the include like this:

		#define SGFE_IMPLEMENTATION
		#include "sgfe.h"

	- All other files should just include the library without the #define macro.

	- Main target for the library is C99 and above. Using C89 or C++ is untested
	and may lead to certain issues.

	- NOTE: Disabling certain features may or may not cause compiler errors for
	certain spots that are dependent on them. Use this feature at your own risk.

===========================================================================
MACROS
	- For any of the macros to work, you must _always_ define it before including
	the library. Example:
	```c
		#define SGFE_IMPLEMENTATION
		TODO(EimaMei): add macro example here.
		#include "sili.h"
	```

	TODO(EimaMei): add macros here.

===========================================================================
CREDITS
	- Colleague Riley Mabb's RGFW (https://github.com/ColleagueRiley/RGFW) - RGFW
	is used as the base foundation for this library as a lot of the general API
	design choices and functionality are taken from it. Riley has also helped out
	when discussing certain features that would eventually get merged into both
	RGFW proper and SGFE. 

LICENSE
	- This software is licensed under the zlib license (see the LICENSE at the
	bottom of the file).

*/


/*
	#define SGFE_IMPLEMENTATION - (required) makes it so the source code is included
	#define SGFE_DEBUG - (optional) makes it so SGFE prints debug messages and errors when they're found
	#define SGFE_NO_INFO - do not define the SGFE_info struct (without SGFE_IMPLEMENTATION)

	#define SGFE_ALLOC x  - choose the default allocation function (defaults to standard malloc)
	#define SGFE_FREE x  - choose the default deallocation function (defaults to standard free)

	#define SGFE_EXPORT - use when building SGFE
	#define SGFE_IMPORT - use when linking with SGFE (not as a single-header)

	#define SGFE_bool x - choose what type to use for bool, by default u32 is used
*/


/*
	=== ORIGINAL SGFE ===
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

#ifndef SGFE_INCLUDE_SGFE_H
#define SGFE_INCLUDE_SGFE_H

#if defined(__cplusplus)
extern "C" {
#endif


#ifndef SGFE_UNUSED
	#define SGFE_UNUSED(x) (void)(x)
#endif

#ifndef SGFE_ALLOC
	#include <stdlib.h>
	#define SGFE_ALLOC(size) malloc(size)
	#define SGFE_FREE(ptr) free(ptr)
#endif

#ifndef SGFE_ASSERT
	#define SGFE_ASSERT(condition) SGFE_windowAssert(SGFE__ROOT_WIN, condition, #condition, __FILE__, __LINE__)
#endif

#if !defined(SGFE_MEMCPY) || !defined(SGFE_STRNCMP) || !defined(SGFE_MEMSET)
	#include <string.h>
#endif

#ifndef SGFE_MEMSET
	#define SGFE_MEMSET(ptr, value, num) memset(ptr, value, num)
#endif

#ifndef SGFE_MEMCPY
	#define SGFE_MEMCPY(dist, src, len) memcpy(dist, src, len)
#endif

#ifndef SGFE_COUNTOF
	#define SGFE_COUNTOF(array) (isize)(sizeof(array) / sizeof((array)[0]))
#endif

#ifndef SGFE_BIT
	#define SGFE_BIT(x) (1 << (x))
#endif

#if !defined(SGFE_PRINTF) && ( defined(SGFE_DEBUG) || defined(SGFE_WAYLAND) )
	/* required when using SGFE_DEBUG */
	#include <stdio.h>
	#define SGFE_PRINTF(fmt, ...) printf(fmt, __VA_ARGS__)
#endif

#if defined(SGFE_EXPORT) || defined(SGFE_IMPORT)
	#if defined(SGFE_EXPORT)
		#define SGFE_DEF __attribute__((visibility("default")))
	#endif
	#ifndef SGFE_DEF
		#define SGFE_DEF
	#endif
#endif

#ifndef SGFE_DEF
	#define SGFE_DEF inline
#endif

#ifndef SGFE_ENUM
	#define SGFE_ENUM(type, name) type name; enum
#endif



/* Constants */

#ifndef SGFE_STANDARD_GRAVITY
/* TODO */
#define SGFE_STANDARD_GRAVITY 9.80665f
#endif

#ifndef SGFE_PI
/* TODO */
#define SGFE_PI 3.141592653589793238462643383279502884f
#endif



#ifndef SGFE_CUSTOM_BACKEND

#if defined(__3DS__) || defined(SGFE_3DS)
	#ifndef SGFE_3DS
	#define SGFE_3DS 1
	#endif

	#ifndef SGFE_MAX_CONTROLLERS
	#define SGFE_MAX_CONTROLLERS 1
	#endif

	#ifndef SGFE_HAS_OPENGL
	#define SGFE_HAS_OPENGL 1
	#endif

#elif defined(GEKKO) || defined(SGFE_WII)
	#ifndef SGFE_WII
	#define SGFE_WII 1
	#endif

	#ifndef SGFE_MAX_CONTROLLERS
	#define SGFE_MAX_CONTROLLERS 4
	#endif

	#define SGFE__BACKEND_FREE_WINDOW_IN_CLOSE

	#ifdef SGFE_IMPLEMENTATION
	#include <gctypes.h>
	typedef s8   i8;
	typedef s16 i16;
	typedef s32 i32;
	typedef s64 i64;
	#define SGFE_DEFINE_TYPE_STDINT
	#endif

#else
	#error "Unknown platform. Currently supported targets: SGFE_3DS, SGFE_WII."

#endif

#endif /* SGFE_CUSTOM_BACKEND */

#ifndef SGFE_HAS_OPENGL
	#undef RGFW_OPENGL
#endif

#ifndef SGFE_DEFINE_TYPE_STDINT
	#include <stdint.h>

	typedef int8_t      i8;
	typedef int16_t    i16;
	typedef int32_t    i32;
	typedef int64_t    i64;

	typedef uint8_t     u8;
	typedef uint16_t   u16;
	typedef uint32_t   u32;
	typedef uint64_t   u64;
#endif

#ifndef SGFE_DEFINE_TYPE_SIZE
	#include <stddef.h>
	typedef size_t     usize;
	typedef ptrdiff_t  isize;
#endif

#ifndef SGFE_DEFINE_TYPE_BOOL
	typedef u8 SGFE_bool;
#endif

#define SGFE_BOOL(x) (SGFE_bool)((x) != 0)
#define SGFE_TRUE (SGFE_bool)1
#define SGFE_FALSE (SGFE_bool)0




#ifndef SGFE_CUSTOM_BACKEND

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_systemModel) {
	SGFE_systemModelNone,

	#if SGFE_3DS
	SGFE_systemModel3DS,
	SGFE_systemModel3DSXL,
	SGFE_systemModelN3DS,
	SGFE_systemModel2DS,
	SGFE_systemModelN3DSXL,
	SGFE_systemModelN2DSXL,
	#endif

	SGFE_systemModelUnknown,
	SGFE_systemModelCount,
};


/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_screen) {
	SGFE_screenPrimary,

	#ifdef SGFE_3DS
	SGFE_screenTop = SGFE_screenPrimary,
	SGFE_screenBottom,
	#endif

	SGFE_screenCount
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_videoMode) {
	#ifdef SGFE_3DS
	/* Sets the video resolution to 400x240 with stereoscopy disabled. Works on
	 * all models. */
	SGFE_videoMode2D,
	/* Sets the video resolution to 400x240 with stereoscopy enabled. Works on
	 * all non-2DS consoles. While the visible video resolution is 400x240, the
	 * actual buffer size is 800x240.
	 *
	 * Left image is from 0x0 to 400x240, right image is from 400x240 to 800x240. */
	SGFE_videoMode3D,
	/* Sets the video resolution to 800x240 with steroescopy disabled. Only works
	 * on 3DS consoles and New 2DS XL. */
	SGFE_videoModeWide,

	#elif SGFE_WII
	/* 320x240 NTSC 60 hz Progressive */
	SGFE_videoModeNTSC_240i,
	/* 720x480 NTSC 60 hz Interlaced */
	SGFE_videoModeNTSC_480i,
	/* 720x480 NTSC 60 hz Progressive */
	SGFE_videoModeNTSC_480p,

	/* 320x264 PAL 50 hz Interlaced */
	SGFE_videoModePAL_264i,
	/* 720x576 PAL 50 hz Interlaced */
	SGFE_videoModePAL_576i,
	/* 720x576 PAL 50 hz Progressive */
	SGFE_videoModePAL_576p,

	/* 320x240 PAL-M 60 hz Progressive */
	SGFE_videoModeMPAL_240i,
	/* 720x480 PAL-M 60 hz Interlaced */
	SGFE_videoModeMPAL_480i,
	/* 720x480 PAL-M 60 hz Progressive */
	SGFE_videoModeMPAL_480p,

	/* 320x240 PAL 60 hz Interlaced */
	SGFE_videoModePAL60hz_240i,
	/* 720x480 PAL 60 hz Interlaced */
	SGFE_videoModePAL60hz_480i,
	/* 720x480 PAL 60 hz Progressive */
	SGFE_videoModePAL60hz_480p,

	#endif

	SGFE_videoModeCount
};

/* TODO(EimaMei): document */
/* TODO(EimaMei): document how if native mode is turned on SGFE_pixelFormatRGBA8 might return false. */
typedef SGFE_ENUM(isize, SGFE_pixelFormat) {
	/* Each channel bit is set to 8-bits. This option is always supported for
	 * non-native buffers, but might not be supported for native ones. By default
	 * 'SGFE_window_initBuffer' picks this as the pixel format. */
	SGFE_pixelFormatRGBA8,

	#ifdef SGFE_3DS
	SGFE_pixelFormatBGR8,
	SGFE_pixelFormatRGB565,
	SGFE_pixelFormatRGB5_A1,
	SGFE_pixelFormatRGBA4,

	#elif SGFE_WII
	/* TODO */
	SGFE_pixelFormatYUV,

	#endif

	SGFE_pixelFormatCount,
};


/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_controllerType) {
	/* TODO(EimaMei): document */
	SGFE_controllerTypeStandard = 0,

	#ifdef SGFE_WII
	/* TODO(EimaMei): document */
	SGFE_controllerTypeWiiRemote = SGFE_controllerTypeStandard,
	/* TODO(EimaMei): document */
	SGFE_controllerTypeNunchuk,
	/* TODO(EimaMei): document */
	SGFE_controllerTypeUnknown,
	#endif

	SGFE_controllerTypeCount,
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_buttonType) {
	SGFE_buttonInvalid = -1,

	#ifdef SGFE_3DS
	SGFE_button_A = 0,
	SGFE_button_B,
	SGFE_button_Select,
	SGFE_button_Start,
	SGFE_button_DpadRight,
	SGFE_button_DpadLeft,
	SGFE_button_DpadUp,
	SGFE_button_DpadDown,
	SGFE_button_R,
	SGFE_button_L,
	SGFE_button_X,
	SGFE_button_Y,
	SGFE_button_ZL,
	SGFE_button_ZR,
	SGFE_button_CstickRight,
	SGFE_button_CstickLeft,
	SGFE_button_CstickUp,
	SGFE_button_CstickDown,

	SGFE_buttonCount,

	#elif SGFE_WII
	SGFE_button_2 = 0,
	SGFE_button_1,
	SGFE_button_B,
	SGFE_button_A,
	SGFE_button_Minus,
	SGFE_button_Home,
	SGFE_button_Left,
	SGFE_button_Right,
	SGFE_button_Down,
	SGFE_button_Up,
	SGFE_button_Plus,

	SGFE_buttonCount_WiiRemote,


	SGFE_button_Z = SGFE_button_Plus + 1,
	SGFE_button_C,
	SGFE_buttonCount_Nunchuk,
	#endif
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(u32, SGFE_button) {
	#ifdef SGFE_3DS
	SGFE_A            = SGFE_BIT(SGFE_button_A),
	SGFE_B            = SGFE_BIT(SGFE_button_B),
	SGFE_Select       = SGFE_BIT(SGFE_button_Select),
	SGFE_Start        = SGFE_BIT(SGFE_button_Start),
	SGFE_DpadRight    = SGFE_BIT(SGFE_button_DpadRight),
	SGFE_DpadLeft     = SGFE_BIT(SGFE_button_DpadLeft),
	SGFE_DpadUp       = SGFE_BIT(SGFE_button_DpadUp),
	SGFE_DpadDown     = SGFE_BIT(SGFE_button_DpadDown),
	SGFE_R            = SGFE_BIT(SGFE_button_R),
	SGFE_L            = SGFE_BIT(SGFE_button_L),
	SGFE_X            = SGFE_BIT(SGFE_button_X),
	SGFE_Y            = SGFE_BIT(SGFE_button_Y),
	SGFE_ZL           = SGFE_BIT(SGFE_button_ZL),
	SGFE_ZR           = SGFE_BIT(SGFE_button_ZR),
	SGFE_CstickRight  = SGFE_BIT(SGFE_button_CstickRight),
	SGFE_CstickLeft   = SGFE_BIT(SGFE_button_CstickLeft),
	SGFE_CstickUp     = SGFE_BIT(SGFE_button_CstickUp),
	SGFE_CstickDown   = SGFE_BIT(SGFE_button_CstickDown),

	SGFE_buttonMask_Face = SGFE_A | SGFE_B | SGFE_X | SGFE_Y | SGFE_Select | SGFE_Start,
	SGFE_buttonMask_Dpad = SGFE_DpadRight | SGFE_DpadLeft | SGFE_DpadUp | SGFE_DpadDown,
	SGFE_buttonMask_Shoulder = SGFE_R | SGFE_L,
	SGFE_buttonMask_ZL_ZR = SGFE_ZL | SGFE_ZR,
	SGFE_buttonMask_Cstick = SGFE_CstickRight | SGFE_CstickLeft | SGFE_CstickUp | SGFE_CstickDown,

	SGFE_buttonMask_All = SGFE_buttonMask_Face     |
						  SGFE_buttonMask_Dpad     |
						  SGFE_buttonMask_Shoulder |
						  SGFE_buttonMask_ZL_ZR    |
						  SGFE_buttonMask_Cstick

	#elif SGFE_WII
	SGFE_2      = SGFE_BIT(SGFE_button_2),
	SGFE_1      = SGFE_BIT(SGFE_button_1),
	SGFE_B      = SGFE_BIT(SGFE_button_B),
	SGFE_A      = SGFE_BIT(SGFE_button_A),
	SGFE_Minus  = SGFE_BIT(SGFE_button_Minus),
	SGFE_Home   = SGFE_BIT(SGFE_button_Home),
	SGFE_Left   = SGFE_BIT(SGFE_button_Left),
	SGFE_Right  = SGFE_BIT(SGFE_button_Right),
	SGFE_Down   = SGFE_BIT(SGFE_button_Down),
	SGFE_Up     = SGFE_BIT(SGFE_button_Up),
	SGFE_Plus   = SGFE_BIT(SGFE_button_Plus),
	SGFE_Z      = SGFE_BIT(SGFE_button_Z),
	SGFE_C      = SGFE_BIT(SGFE_button_C),

	SGFE_buttonMask_WiiRemote = SGFE_2 | SGFE_1 | SGFE_B | SGFE_A | SGFE_Minus |
						SGFE_Home | SGFE_Left | SGFE_Right | SGFE_Down | SGFE_Up |
						SGFE_Plus,
	SGFE_buttonMask_Nunchuk = SGFE_buttonMask_WiiRemote | SGFE_buttonMask_WiiRemote

	#endif
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_axisType) {
	#ifdef SGFE_3DS
	SGFE_axisLeftX,
	SGFE_axisLeftY,

	/* TODO(EimaMei): As of July 2nd 2025 Circle Pad Pro support isn't going to
	 * be added until this PR gets merged: https://github.com/devkitPro/libctru/pull/568. */
	/* SGFE_axisTypeRightX,
	SGFE_axisTypeRightY, */

	#elif SGFE_WII
	SGFE_axisLeftX,
	SGFE_axisLeftY,

	#endif

	SGFE_axisTypeCount,
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_pointerType) {
	#ifdef SGFE_3DS
	SGFE_pointerTouchscreen,

	#elif SGFE_WII
	SGFE_pointerInfrared,

	#endif

	SGFE_pointerTypeCount,
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_motionType) {
	#if SGFE_3DS
	SGFE_motionAccelerometer,
	SGFE_motionGyroscope,

	#elif SGFE_WII
	SGFE_motionAccelerometer,
	SGFE_motionGyroscope,
	SGFE_motionNunchukAccelerometer,

	#endif

	SGFE_motionTypeCount,
};

#endif /* SGFE_CUSTOM_BACKEND */

typedef struct SGFE_axis {
	float value;
	float deadzone;
} SGFE_axis;

typedef SGFE_ENUM(u8, SGFE_buttonState) {
	SGFE_buttonStateSupported = SGFE_BIT(0),
	SGFE_buttonStateCurrent   = SGFE_BIT(1),
	SGFE_buttonStatePrevious  = SGFE_BIT(2),
} SGFE_keyState;

/* TODO(EimaMei): new structure. */
typedef struct SGFE_controller {
	/* TODO */
	isize port;
	/* Denotes what type of controller it is. */
	SGFE_controllerType type;
	/* Denotes if the controller is connected. */
	SGFE_bool connected;

	/* TODO */
	SGFE_button buttons_held, buttons_down, buttons_up;

	/* Current axes states. */
	SGFE_axis axes[SGFE_axisTypeCount];
	/* Current pointer states. */
	isize pointers[SGFE_pointerTypeCount][2];
	/* Current motion states. */
	float motions[SGFE_motionTypeCount][3];

	/* Boolean states of enabled pointers. */
	SGFE_bool enabled_pointers[SGFE_pointerTypeCount];
	/* Boolean states of enabled motions. */
	SGFE_bool enabled_motions[SGFE_motionTypeCount];
} SGFE_controller;



/* TODO */
typedef SGFE_ENUM(isize, SGFE_contextType) {
	SGFE_contextTypeNone,
	SGFE_contextTypeBuffer,
	SGFE_contextTypeOpenGL,
	SGFE_contextTypeNative
};


/* TODO */
typedef struct SGFE_context SGFE_context;

/* TODO */
SGFE_DEF SGFE_contextType SGFE_contextGetType(const SGFE_context* ctx);
/* TODO */
SGFE_DEF void* SGFE_contextGetData(SGFE_context* ctx);


/* TODO */
typedef struct SGFE_contextBuffer SGFE_contextBuffer;

/* TODO */
SGFE_DEF SGFE_contextBuffer* SGFE_contextGetBuffer(SGFE_context* ctx);


#ifdef SGFE_OPENGL
/**
 * OpenGL Context type
 *
 * The structure is used as an opaque data type to an OpenGL context.
 *
 * \sa SGFE_createContext_OpenGL
 */
typedef struct SGFE_contextOpenGL SGFE_contextOpenGL;

/* TODO */
SGFE_DEF SGFE_contextOpenGL* SGFE_contextGetOpenGL(SGFE_context* ctx);
#endif

typedef SGFE_ENUM(isize, SGFE_eventType) {
	/*! event codes */
	SGFE_eventNone = 0, /*!< no event has been sent */

	SGFE_eventQuit, /*!< the user clicked the quit button */
	SGFE_eventDeviceSleep, /* The device has entered sleep mode. */
	SGFE_eventDeviceWakeup, /* The device has exited out of sleep mode. */

	SGFE_windowRefresh, /* The window content needs to be refreshed */
	SGFE_eventVideoModeChanged, /* Video mode for the window has been changed (usually by 'SGFE_windowChangeVideoMode'). */
	SGFE_eventFocusOut, /* User has exited the window and entered a new environment (eg. "Home Menu".) */
	SGFE_eventFocusIn, /* 	User has entered the window from an external environment (usually the "Home Menu"). */


	SGFE_eventControllerConnected, /*!< a controller was connected */
	SGFE_eventControllerDisconnected, /*!< a controller was disconnected */

	SGFE_eventButtonDown, /*!< a controller button was pressed */
	SGFE_eventButtonUp, /*!< a controller button was released */

	SGFE_eventAxis, /*!< an axis of a controller was moved */
	SGFE_eventPointer,
	SGFE_eventMotion, /* The motion sensors of the controller have been moved. */
};

typedef struct SGFE_event_controller {
	SGFE_eventType type;
	SGFE_controller* controller;
} SGFE_event_controller;

typedef struct SGFE_event_button {
	SGFE_eventType type;
	SGFE_controller* controller;

	SGFE_button buttons;
	SGFE_button buttons_down;
	u32 raw_buttons;
	u32 raw_buttons_down;
} SGFE_event_button;

typedef struct SGFE_event_axis {
	SGFE_controller* controller;
	SGFE_axisType which;
	float value;
	float deadzone;
} SGFE_event_axis;

typedef struct SGFE_event_pointer {
	SGFE_controller* controller;
	SGFE_pointerType which;
	i32 x, y;
} SGFE_event_pointer;

typedef struct SGFE_event_motion {
	SGFE_controller* controller;
	SGFE_motionType which;
	float x, y, z;
} SGFE_event_motion;

typedef struct SGFE_event_user {
	isize code;
	void* data1;
	void* data2;
} SGFE_event_user;

typedef struct SGFE_event {
	SGFE_eventType type;

	SGFE_event_controller controller;
	SGFE_event_button button;
	SGFE_event_axis axis;
	SGFE_event_pointer pointer;
	SGFE_event_motion motion;
	SGFE_event_user user;
} SGFE_event;



/* TODO */
typedef struct SGFE_window SGFE_window;
/* TODO */
typedef struct SGFE_windowSource SGFE_windowSource;

/*! Optional arguments for making a windows */
typedef SGFE_ENUM(u32, SGFE_windowFlags) {
	SGFE_windowFlagsNone      = 0,

	SGFE_windowFreeOnClose    = SGFE_BIT(15),
	SGFE_windowBuffer         = SGFE_BIT(16),
	SGFE_windowOpenGL         = SGFE_BIT(17),
	SGFE_windowEGL            = SGFE_BIT(18),

	#ifdef SGFE_3DS
	SGFE_windowTopScreen      = SGFE_BIT(29), /* TODO(EimaMei): New enum. */
	SGFE_windowBottomScreen   = SGFE_BIT(30), /* TODO(EimaMei): New enum. */
	SGFE_windowDualScreen     = SGFE_windowTopScreen | SGFE_windowBottomScreen, /* TODO(EimaMei): New enum. */
	#endif
};

/* TODO(EimaMei): Move window create functions here. */

/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_deviceSleepFunc)(SGFE_window* win, SGFE_bool is_sleeping);

/*! SGFE_eventQuit, the window that was closed */
typedef void (*SGFE_quitFunc)(SGFE_window* win);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_windowRefreshFunc)(SGFE_window* win);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_windowVideoModeFunc)(SGFE_window* win, SGFE_videoMode mode);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_windowFocusFunc)(SGFE_window* win, SGFE_bool is_focused);

/*! SGFE_controllerConnected / SGFE_controllerDisconnected, the window that got the event, the controller in question, if the controller was connected (else it was disconnected) */
typedef void (*SGFE_controllerFunc)(SGFE_window* win, SGFE_controller* controller, SGFE_bool is_connected);

/*! SGFE_buttonPressed, the window that got the event, the button that was pressed, the scroll value, if it was a press (else it's a release) */
typedef void (*SGFE_buttonFunc)(SGFE_window* win, SGFE_controller* controller, SGFE_button mask, SGFE_bool down);
/*! SGFE_axisMove, the window that got the event, the controller in question, the axis values and the axis count */
typedef void (*SGFE_axisFunc)(SGFE_window* win, SGFE_controller* controller, SGFE_axisType axis);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_pointerFunc)(SGFE_window* win, SGFE_controller* controller, SGFE_pointerType pointer);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_motionFunc)(SGFE_window* win, SGFE_controller* controller, SGFE_motionType motion);


typedef struct SGFE_callbacks {
	SGFE_deviceSleepFunc sleep;

	SGFE_quitFunc quit;
	SGFE_windowRefreshFunc refresh;
	SGFE_windowVideoModeFunc video_mode;
	SGFE_windowFocusFunc focus;

	SGFE_controllerFunc controller;

	SGFE_buttonFunc button;
	SGFE_axisFunc axis;
	SGFE_pointerFunc pointer;
	SGFE_motionFunc motion;
} SGFE_callbacks;


#if (defined(SGFE_IMPLEMENTATION) || !defined(SGFE_NO_WINDOW_SRC)) && !defined(SGFE_CUSTOM_BACKEND)

#ifdef SGFE_3DS

#include <3ds.h>

#ifdef SGFE_OPENGL
#define RIP_BACKEND RIP_BACKEND_KYGX
#include <GLES/gl2.h>
#endif

struct SGFE_contextBuffer {
	SGFE_screen screen;
	SGFE_videoMode mode;
	SGFE_pixelFormat format;

	isize current;
	u8* buffers[2];

	SGFE_bool is_buffer_allocated;
	SGFE_bool is_double_buffered;
	SGFE_bool is_native;

	#ifndef SGFE_BUFFER_NO_CONVERSION
	u8* buffers_native[2];
	#endif

	u32 size;
};

#ifdef SGFE_OPENGL
struct SGFE_contextOpenGL {
	SGFE_screen screen;
	SGFE_videoMode mode;
	SGFE_pixelFormat format;
	GLASSCtx ctx;

	GLuint framebuffer;
	GLuint renderbuffer;
};
#endif

struct SGFE_context {
	SGFE_contextType type;
	union {
		SGFE_contextBuffer buffer;
		#ifdef SGFE_OPENGL
		SGFE_contextOpenGL gl;
		#endif
	} data;
};

struct SGFE_windowSource {
	SGFE_bool lcd_is_on;
	aptHookCookie apt_hook;
	SGFE_context ctx[SGFE_screenCount];
};

#elif SGFE_WII

#ifndef HW_RVL
#define HW_RVL 1
#endif
#include <gccore.h>
#include <wiiuse/wpad.h>


struct SGFE_contextBuffer {
	SGFE_videoMode mode;
	SGFE_screen screen;
	SGFE_pixelFormat format;

	isize current;
	u8* buffers[2];

	SGFE_bool is_buffer_allocated;
	SGFE_bool is_double_buffered;
	SGFE_bool is_native;

	#ifndef SGFE_BUFFER_NO_CONVERSION
	u8* buffers_native[2];
	#endif

	GXRModeObj* gx_video_mode;
};

#ifdef SGFE_OPENGL
struct SGFE_contextOpenGL {
};
#endif

struct SGFE_context {
	SGFE_contextType type;
	union {
		SGFE_contextBuffer buffer;
		#ifdef SGFE_OPENGL
		SGFE_contextOpenGL gl;
		#endif
	} data;
};

struct SGFE_windowSource {
	struct wiimote_t** wiimotes;
	SGFE_context ctx;
};

#endif

struct SGFE_window {
	/* TODO */
	SGFE_windowSource src;
	/* TODO */
	SGFE_context* current[SGFE_screenCount];

	/* struct SGFE_info */
	SGFE_event events[32];
	isize event_len;

	SGFE_controller controllers[SGFE_MAX_CONTROLLERS];

	SGFE_bool queue_events, polled_events;
	u32 _flags; /*!< windows flags (for SGFE to check) */

	SGFE_callbacks callbacks;
	void* userPtr;
}; /*!< window structure for managing the window */

#endif /* (defined(SGFE_IMPLEMENTATION) || !defined(SGFE_NO_WINDOW_SRC)) && !defined(SGFE_CUSTOM_BACKEND) */

/* TODO */
SGFE_DEF isize SGFE_sizeofWindow(void);
/* TODO */
SGFE_DEF isize SGFE_sizeofWindowSrc(void);


/* TODO */
SGFE_DEF SGFE_context* SGFE_windowGetCurrent(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_context* SGFE_windowGetCurrentEx(SGFE_window* win, SGFE_screen screen);

/* TODO */
SGFE_DEF SGFE_context* SGFE_windowGetContext(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_context* SGFE_windowGetContextEx(SGFE_window* win, SGFE_screen screen);

/* TODO */
SGFE_DEF SGFE_contextBuffer* SGFE_windowGetCurrentBuffer(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_contextBuffer* SGFE_windowGetCurrentExBuffer(SGFE_window* win, SGFE_screen screen);

/* TODO */
SGFE_DEF SGFE_contextBuffer* SGFE_windowGetContextBuffer(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_contextBuffer* SGFE_windowGetContextExBuffer(SGFE_window* win, SGFE_screen screen);

#ifdef SGFE_OPENGL
/* TODO */
SGFE_DEF SGFE_contextOpenGL* SGFE_windowGetCurrentOpenGL(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_contextOpenGL* SGFE_windowGetCurrentExOpenGL(SGFE_window* win, SGFE_screen screen);

/* TODO */
SGFE_DEF SGFE_contextOpenGL* SGFE_windowGetContextOpenGL(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_contextOpenGL* SGFE_windowGetContextExOpenGL(SGFE_window* win, SGFE_screen screen);
#endif


SGFE_DEF void* SGFE_window_getUserPtr(SGFE_window* win);
SGFE_DEF void SGFE_window_setUserPtr(SGFE_window* win, void* ptr);

SGFE_DEF SGFE_bool SGFE_windowIsQueuingEvents(SGFE_window* win);
SGFE_DEF void SGFE_windowSetQueueEvents(SGFE_window* win, SGFE_bool is_queuing_events);

SGFE_DEF SGFE_windowSource* SGFE_window_getSrc(SGFE_window* win);


/* TODO */
SGFE_DEF SGFE_window* SGFE_windowMake(SGFE_videoMode mode, SGFE_windowFlags flags);
SGFE_DEF SGFE_window* SGFE_windowMakePtr(
	SGFE_videoMode mode, /* TODO(EimaMei): document */
	SGFE_windowFlags flags, /* extra arguments (NULL / (u32)0 means no flags used) */
	SGFE_window* win /* ptr to the window struct you want to use */
); /*!< function to create a window (without allocating a window struct) */

/* TODO */
SGFE_DEF SGFE_window* SGFE_windowMakeContextless(SGFE_windowFlags flags);
/* TODO */
SGFE_DEF SGFE_window* SGFE_windowMakePtrContextless(SGFE_windowFlags flags, SGFE_window* win);


/*! window managment functions */
SGFE_DEF void SGFE_windowClose(SGFE_window* win); /*!< close the window and free leftover data */
/* TODO */
SGFE_DEF void SGFE_windowFreeContext(SGFE_window* win);


/*! set the window flags (will undo flags if they don't match the old ones) */
SGFE_DEF void SGFE_windowSetFlags(SGFE_window* win, SGFE_windowFlags flags);


/* TODO */
SGFE_DEF void SGFE_windowPollEvents(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_bool SGFE_windowCheckEvent(SGFE_window* win, const SGFE_event** out_event);

/* TODO */
SGFE_DEF SGFE_bool SGFE_windowEventPush(SGFE_window* win, const SGFE_event* event);
/* TODO */
SGFE_DEF const SGFE_event* SGFE_windowEventPop(SGFE_window* win);


/* TODO */
SGFE_DEF void SGFE_windowSwapBuffers(SGFE_window* win);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_windowMakeCurrent(SGFE_window* win, SGFE_context* ctx);


/* Returns the controller strucutr associated with the port. */
SGFE_DEF SGFE_controller* SGFE_windowGetController(SGFE_window* win, isize port);


/* TODO */
SGFE_DEF void SGFE_windowSetShouldClose(SGFE_window* win, SGFE_bool shouldClose);
/* TODO */
SGFE_DEF SGFE_bool SGFE_windowShouldClose(SGFE_window* win);


/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_bool SGFE_windowInitTerminalOutput(SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowAssert(SGFE_window* win, SGFE_bool is_asserted, const char* condition_str,
	const char* file, isize line);



SGFE_DEF SGFE_bool SGFE_isHeld(const SGFE_controller* controller, SGFE_button mask);
SGFE_DEF SGFE_bool SGFE_isDown(const SGFE_controller* controller, SGFE_button mask);
SGFE_DEF SGFE_bool SGFE_isUp(const SGFE_controller* controller, SGFE_button mask);


/* TODO */
SGFE_DEF SGFE_buttonType SGFE_buttonGetType(SGFE_button mask);
/* TODO */
SGFE_DEF SGFE_bool SGFE_iterateButtonMask(SGFE_button* buttons_mask, SGFE_buttonType* out_button);

/* TODO */
SGFE_DEF SGFE_button SGFE_buttonFromAPI(SGFE_controllerType type, u32 mask);
/* TODO */
SGFE_DEF u32 SGFE_buttonToAPI(SGFE_controllerType type, SGFE_button button);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF SGFE_button SGFE_buttonGetMask(SGFE_controllerType type);

/* TODO(EimaMei): */
SGFE_DEF void SGFE_controllerGetRangeButton(const SGFE_controller* controller, SGFE_buttonType* out_first, SGFE_buttonType* out_last);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF void SGFE_controllerGetRangeAxis(const SGFE_controller* controller, SGFE_axisType* out_first, SGFE_axisType* out_last);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF void SGFE_controllerGetRangePointer(const SGFE_controller* controller, SGFE_pointerType* out_first, SGFE_pointerType* out_last);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF void SGFE_controllerGetRangeMotion(const SGFE_controller* controller, SGFE_motionType* out_first, SGFE_motionType* out_last);


/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetName(const SGFE_controller* controller);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNameButton(const SGFE_controller* controller,
	SGFE_buttonType button);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNameAxis(const SGFE_controller* controller,
	SGFE_axisType axis);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNamePointer(const SGFE_controller* controller,
	SGFE_pointerType type);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNameMotion(const SGFE_controller* controller,
	SGFE_motionType type);


/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF SGFE_bool SGFE_controllerEnablePointer(SGFE_controller* controller,
	SGFE_pointerType pointer, SGFE_bool enable);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF SGFE_bool SGFE_controllerEnableMotion(SGFE_controller* controller,
	SGFE_motionType motion, SGFE_bool enable);



/** * @defgroup error handling
* @{ */
typedef SGFE_ENUM(u8, SGFE_debugType) {
	SGFE_debugTypeError,
	SGFE_debugTypeWarning,
	SGFE_debugTypeInfo
};

typedef SGFE_ENUM(u8, SGFE_error) {
	SGFE_errorNone = 0,
	SGFE_errorOutOfMemory,
	SGFE_errorSystem,
	SGFE_errorOpenGLContext,
	SGFE_errorEventQueue,
	SGFE_infoWindow, /* TODO(EimaMei): move these. */
	SGFE_infoBuffer,
	SGFE_infoGlobal,
	SGFE_infoOpenGL,
	GFW_warningOpenGL,
};

typedef struct SGFE_debugContext { SGFE_window* win; u32 srcError; } SGFE_debugContext;

#if defined(__cplusplus) && !defined(__APPLE__)
#define SGFE_DEBUG_CTX(win, err) {win, err}
#else
#define SGFE_DEBUG_CTX(win, err) (SGFE_debugContext){win, err}
#endif

typedef void (* SGFE_debugFunc)(SGFE_debugType type, SGFE_error err, SGFE_debugContext ctx, const char* msg);
SGFE_DEF void SGFE_sendDebugInfo(SGFE_debugType type, SGFE_error err, SGFE_debugContext ctx, const char* msg);
/** @} */


/*! TODO(EimaMei): new function. */
SGFE_DEF SGFE_deviceSleepFunc SGFE_setDeviceSleepCallback(SGFE_window* win, SGFE_deviceSleepFunc func);

/*! set callback for a window quit event. Returns previous callback function (if it was set)  */
SGFE_DEF SGFE_quitFunc SGFE_setWindowQuitCallback(SGFE_window* win, SGFE_quitFunc func);
/*! TODO(EimaMei): new function. */
SGFE_DEF SGFE_windowRefreshFunc SGFE_setWindowRefreshCallback(SGFE_window* win, SGFE_windowRefreshFunc func);
/*! TODO(EimaMei): new function. */
SGFE_DEF SGFE_windowVideoModeFunc SGFE_setWindowVideoModeCallback(SGFE_window* win, SGFE_windowVideoModeFunc func);
/*! TODO(EimaMei): new function. */
SGFE_DEF SGFE_windowFocusFunc SGFE_setWindowFocusCallback(SGFE_window* win, SGFE_windowFocusFunc func);

/*! set callback for when a controller is connected or disconnected. Returns the previous callback function (if it was set) */
SGFE_DEF SGFE_controllerFunc SGFE_setControllerCallback(SGFE_window* win, SGFE_controllerFunc func);

/*! set callback for a controller button (press / release) event. Returns previous callback function (if it was set)  */
SGFE_DEF SGFE_buttonFunc SGFE_setButtonCallback(SGFE_window* win, SGFE_buttonFunc func);
/*! set callback for a controller axis move event. Returns previous callback function (if it was set)  */
SGFE_DEF SGFE_axisFunc SGFE_setAxisCallback(SGFE_window* win, SGFE_axisFunc func);
/* TODO(EimaMei): NEW FUNCTION. */
SGFE_DEF SGFE_pointerFunc SGFE_setPointerCallback(SGFE_window* win, SGFE_pointerFunc func);
/* TODO(EimaMei): NEW FUNCTION. */
SGFE_DEF SGFE_motionFunc SGFE_setMotionCallback(SGFE_window* win, SGFE_motionFunc func);

/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_debugFunc SGFE_setDebugCallback(SGFE_debugFunc func);



/* */
SGFE_DEF SGFE_bool SGFE_bufferMakeWithDefaultSettings(SGFE_contextBuffer* out_buffer,
	SGFE_videoMode mode, SGFE_pixelFormat format, SGFE_bool allocate_buffers);

/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* out_buffer);
/* TODO */
SGFE_DEF void SGFE_bufferFreeContext(SGFE_contextBuffer* buffer);

/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* out_buffer);
/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferFreeFramebuffers(SGFE_contextBuffer* out_buffer);
/* TODO */
SGFE_DEF u8* SGFE_bufferConvertFramebufferToNative(SGFE_contextBuffer* b);


/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_bool SGFE_bufferSetFormat(SGFE_contextBuffer* b, SGFE_pixelFormat format);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_bufferSetNative(SGFE_contextBuffer* b, SGFE_bool is_native);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_bufferSetDoubleBuffered(SGFE_contextBuffer* b, SGFE_bool enable);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_bufferFlipFramebuffers(SGFE_contextBuffer* b);

/* TODO(EimaMei): New function. */
SGFE_DEF u8* SGFE_bufferGetFramebuffer(SGFE_contextBuffer* b);

/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_pixelFormat SGFE_contextGetFormat(const SGFE_contextBuffer* b);
/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_bool SGFE_bufferIsNative(const SGFE_contextBuffer* b);
/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_bool SGFE_bufferIsDoubleBuffered(const SGFE_contextBuffer* b);
/* TODO(EimaMei): New function. */
SGFE_DEF isize SGFE_contextBufferGetCurrent(const SGFE_contextBuffer* b);

/* TODO */
SGFE_DEF SGFE_screen SGFE_bufferGetScreen(SGFE_contextBuffer* b);
/* TODO */
SGFE_DEF isize SGFE_bufferGetFramebufferCount(SGFE_contextBuffer* b);
/* TODO */
SGFE_DEF void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height);
/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferIsStereoscopic(SGFE_contextBuffer* b);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowCreateContextBuffer(SGFE_window* win, SGFE_videoMode mode,
	SGFE_pixelFormat format, SGFE_bool is_native);



#if defined(SGFE_OPENGL)

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
 * OpenGL context initializiation integer values
 *
 * OpenGL hints are used to set desired parameters for when creating a new OpenGL
 * context. These are treated as 'hints' since the OpenGL driver is only obligated
 * to fufill the "minimum" requirements for some parameters (e.g., specifying a
 * 16-bit color buffer could yield a 24 or even 32 bit one). Other parameters may
 * require specific values to function (e.g., SGFE_glProfile).
 *
 * Settings hints must be done before calling SGFE_createContext_OpenGL() or
 * SGFE_windowMake().
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
 * \sa SGFE_setHint_OpenGL
 * \sa SGFE_getHint_OpenGL
 */
typedef SGFE_ENUM(i32, SGFE_glHint)  {
	/* Type of OpenGL API context (SGFE_glProfile_Core by default). */
	SGFE_glProfile,
	/* OpenGL context major version (1 by default). */
	SGFE_glMajor,
	/* OpenGL context minor version (0 by default). */
	SGFE_glMinor,

	/* Minimum number of bits for the red channel of the color buffer (8 by default). */
	SGFE_glRed,
	/* Minimum number of bits for the green channel of the color buffer (8 by default). */
	SGFE_glGreen,
	/* Minimum number of bits for the blue channel of the color buffer (8 by default). */
	SGFE_glBlue,
	/* Minimum number of bits for the alpha channel of the color buffer (8 by default). */
	SGFE_glAlpha,

	/* Boolean if double buffering is used (true by defult). */
	SGFE_glDoubleBuffer,
	/* Minimum number of bits for the depth buffer (24 by default). */
	SGFE_glDepth,
	/* Minimum number of bits in the stencil buffer (0 by default). */
	SGFE_glStencil,


	/* Minimum number of bits for the red channel of the accumulation buffer (0 by default). */
	SGFE_glAccumRed,
	/* Minimum number of bits for the green channel of the accumulation buffer (0 by default). */
	SGFE_glAccumGreen,
	/* Minimum number of bits for the blue channel of the accumulation buffer (0 by default). */
	SGFE_glAccumBlue,
	/* Minimum number of bits for the alpha channel of the accumulation buffer (0 by default). */
	SGFE_glAccumAlpha,

	/* Number of samples used for multiplesample anti-alisaing (0 by default). */
	SGFE_glSamples,
	/* Number of auxiliary buffers. (0 by default). Deprecated OpenGL feature, do not use for new code. */
	SGFE_glAuxBuffers,

	/* Boolean if the renderering is done on the CPU (false by defult). */
	SGFE_glSoftwareRenderer,
	/* Boolean if the context is stereosopic 3D (false by default). */
	SGFE_glStereo,
	/* Boolean if sRGB framebuffer is used (false by default). */
	SGFE_glSRGB,
	/* Boolean if the context is robust (memory-safe) (false by default). For more information: https://registry.khronos.org/OpenGL/extensions/EXT/EXT_robustness.txt. */
	SGFE_glRobustness,
	/* Boolean if the context has additional debugging features (false by default). */
	SGFE_glDebug,
	/* Boolean if OpenGL errors have undefined behavior (false by default). Fore more information: https://registry.khronos.org/OpenGL/extensions/KHR/KHR_no_error.txt. */
	SGFE_glNoError,
	/* Boolean if the OpenGL driver should flush the graphics when changing contexts (false by default). */
	SGFE_glFlushOnContextChange,
	/* Boolean if OpenGL context sharing is enabled (false by default). */
	SGFE_glShareWithCurrentContext,

	/* Number of OpenGL hints that SGFE supports. */
	SGFE_glHintCount,
};

/**
 * OpenGL context hint values
 *
 * Certain SGFE_glHint require a specific value instead of a regular number or
 * boolean. Any specific value follows the naming scheme of `SGFE_<glHint>_<value>`.
 *
 * \sa SGFE_glHint
 */
typedef SGFE_ENUM(i32, SGFE_glValue)  {
	/* Nothing is done when a context is changed. */
	SGFE_glReleaseBehavior_None = 0,
	/* OpenGL flushes the previous context when changing contexts. */
	SGFE_glReleaseBehavior_Flush,

	/* OpenGL Core (OpenGL 3.2-4.6). */
	SGFE_glProfile_Core = 0,
	/* OpenGL Compatibility (OpenGL 1.0-3.1). */
	SGFE_glProfile_Compatibility,
	/* OpenGL ES. */
	SGFE_glProfile_ES
};

/**
 * Sets an OpenGL hint before creating the context.
 *
 * \param hint An OpenGL hint. If the hint is negative or surpasses SGFE_glHintCount,
 * the function panics.
 * \param value The desired value for the hint. No checking is done for the parameter.
 *
 * \sa SGFE_getHint_OpenGL
 */
SGFE_DEF void SGFE_setHint_OpenGL(SGFE_glHint hint, i32 value);

/**
 * Returns the value of an OpenGL hint from the current context.
 *
 * \param hint An OpenGL hint. If the hint is negative or surpasses SGFE_glHintCount,
 * the function panics.
 *
 * \returns The value associated with the OpenGL hint.
 *
 * \sa SGFE_setHint_OpenGL
 */
SGFE_DEF i32 SGFE_getHint_OpenGL(SGFE_glHint hint);

/**
 * Returns the currently binded OpenGL context.
 *
 * This function differs from SGFE_getCurrent_OpenGL()
 *
 * \returns The currently binded OpenGL context.
 */
SGFE_DEF void* SGFE_getCurrentContext_OpenGL(void);

/* === SGFE_contextOpenGL === */

/* TODO */
SGFE_DEF SGFE_bool SGFE_createContext_OpenGL(SGFE_screen screen, SGFE_videoMode mode, SGFE_contextOpenGL* out_gl);
/* TODO */
SGFE_DEF void SGFE_context_free_OpenGL(SGFE_contextOpenGL* gl);

/* TODO */
SGFE_DEF void SGFE_context_swapInterval_OpenGL(SGFE_contextOpenGL* gl, i32 swap_interval);

/* === */

/*!< create an OpenGL context for the SGFE window, run by createWindow by default (unless the SGFE_windowNoInitAPI is included) */
SGFE_DEF SGFE_bool SGFE_window_createContext_OpenGL(SGFE_window* win, SGFE_videoMode mode);
/* TODO */
SGFE_DEF void SGFE_window_freeContext_OpenGL(SGFE_window* win);

/* TODO */
SGFE_DEF void SGFE_window_swapInterval_OpenGL(SGFE_window* win, i32 swap_interval);
/* TODO */
SGFE_DEF void SGFE_window_swapBuffers_OpenGL(SGFE_window* win); /*!< swap the rendering buffer */
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_window_makeCurrent_OpenGL(SGFE_window* win, SGFE_contextOpenGL* out_gl);

#endif



/* Returns a video mode that's considered to be the most optimal for the system
 * by the library. Usually this returns a mode that has a standard resolution,
 * refresh rate and technique for drawing scanlines that works across all models. */
SGFE_DEF SGFE_videoMode SGFE_videoModeOptimal(void);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_videoModeResolution(SGFE_videoMode mode,
	isize* out_width, isize* out_height);

/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_videoMode SGFE_pixelFormatOptimal(void);
/* TODO(EimaMei): New function. */
SGFE_DEF isize SGFE_pixelFormatBytesPerPixel(SGFE_pixelFormat format);


/* TODO(EimaMei): New function. */
SGFE_DEF const char* SGFE_videoModeStr(SGFE_videoMode mode);
/* TODO(EimaMei): New function. */
SGFE_DEF const char* SGFE_pixelFormatStr(SGFE_pixelFormat format);



/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_systemModel SGFE_platformGetModel(void);

#ifdef SGFE_3DS

/* TODO */
SGFE_button SGFE_platformButtonFromAPI(u32 mask);
/* TODO */
u32 SGFE_platformButtonToAPI(SGFE_button button);


/* TODO(EimaMei): new function. */
SGFE_DEF float SGFE_platformGet3DSlider(void);


#ifdef SGFE_OPENGL
/* TODO(EimaMei): new function */
SGFE_DEF SGFE_bool SGFE_platformRotateScreenOpenGL(GLuint shader_program, const char* mat4_uniform_name);
#endif

#endif




#ifdef SGFE_IMPLEMENTATION

const SGFE_button SGFE_BUTTON_MASK_BITS_LUT[SGFE_controllerTypeCount];

const isize SGFE_BUTTON_RANGE_LUT[SGFE_controllerTypeCount][2];
const isize SGFE_AXIS_RANGE_LUT[SGFE_controllerTypeCount][2];
const isize SGFE_POINTER_RANGE_LUT[SGFE_controllerTypeCount][2];
const isize SGFE_MOTION_RANGE_LUT[SGFE_controllerTypeCount][2];

const char* SGFE_CONTROLLER_NAME_LUT[SGFE_controllerTypeCount];
const char* SGFE_AXIS_NAME_LUT[SGFE_axisTypeCount];
const char* SGFE_POINTER_NAME_LUT[SGFE_pointerTypeCount];
const char* SGFE_MOTION_NAME_LUT[SGFE_motionTypeCount];

const isize SGFE_VIDEO_RESOLUTION_LUT[SGFE_videoModeCount][2];
const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_pixelFormatCount];

const char* SGFE_VIDEO_MODE_NAME_LUT[SGFE_videoModeCount];
const char* SGFE_PIXEL_FORMAT_NAME_LUT[SGFE_pixelFormatCount];

/* NOTE(EimaMei): Do not use this! Only used for assertions. */
SGFE_window* SGFE__ROOT_WIN;


SGFE_DEF SGFE_bool SGFE_windowMake_platform(SGFE_window* win);
SGFE_DEF void SGFE_windowClose_platform(SGFE_window* win);

SGFE_DEF const char* SGFE_controllerGetNameButton_platform(const SGFE_controller* controller,
	SGFE_buttonType button);

SGFE_DEF SGFE_bool SGFE_controllerEnablePointer_platform(SGFE_controller* controller, SGFE_pointerType pointer,
		SGFE_bool enable);
SGFE_DEF SGFE_bool SGFE_controllerEnableMotion_platform(SGFE_controller* controller, SGFE_pointerType pointer,
		SGFE_bool enable);

SGFE_DEF u8* SGFE__fetchSwapBuffer(SGFE_contextBuffer* b);


/* var - VARIABLE | mask - UINT | set - SGFE_bool
 * Sets/unsets the mask for the variable. */
#define SGFE_MASK_SET(var, mask, set) do { \
	if (set) (var) |=  (mask); \
	else     (var) &= ~(mask); \
} while (0)

u8* SGFE__fetchSwapBuffer(SGFE_contextBuffer* b) {
	#ifndef SGFE_BUFFER_NO_CONVERSION
	return SGFE_bufferConvertFramebufferToNative(b);
	#else
	return SGFE_bufferGetFramebuffer(b);
	#endif
}


SGFE_contextType SGFE_contextGetType(const SGFE_context* ctx) {
	SGFE_ASSERT(ctx != NULL);
	return ctx->type;
}

void* SGFE_contextGetData(SGFE_context* ctx) {
	SGFE_ASSERT(ctx != NULL);
	return &ctx->data;
}


SGFE_contextBuffer* SGFE_contextGetBuffer(SGFE_context* ctx) {
	SGFE_ASSERT(ctx != NULL);
	SGFE_ASSERT(ctx->type == SGFE_contextTypeBuffer);
	return &ctx->data.buffer;
}

#ifdef SGFE_OPENGL
SGFE_contextOpenGL* SGFE_contextGetOpenGL(SGFE_context* ctx) {
	SGFE_ASSERT(ctx != NULL);
	SGFE_ASSERT(ctx->type == SGFE_contextTypeBuffer);
	return &ctx->data.gl;
}
#endif



#define SGFE_CALLBACK_DEFINE(return_type, name, member_name) \
	return_type name(SGFE_window* win, return_type func) { \
		SGFE_ASSERT(win != NULL); \
		\
		return_type previous_func = win->callbacks.member_name; \
		win->callbacks.member_name = func; \
		return previous_func; \
	}

SGFE_CALLBACK_DEFINE(SGFE_deviceSleepFunc, SGFE_setDeviceSleepCallback, sleep)

SGFE_CALLBACK_DEFINE(SGFE_quitFunc, SGFE_setWindowQuitCallback, quit)
SGFE_CALLBACK_DEFINE(SGFE_windowRefreshFunc, SGFE_setWindowRefreshCallback, refresh)
SGFE_CALLBACK_DEFINE(SGFE_windowVideoModeFunc, SGFE_setWindowVideoModeCallback, video_mode)
SGFE_CALLBACK_DEFINE(SGFE_windowFocusFunc, SGFE_setWindowFocusCallback, focus)

SGFE_CALLBACK_DEFINE(SGFE_controllerFunc, SGFE_setControllerCallback, controller)

SGFE_CALLBACK_DEFINE(SGFE_buttonFunc, SGFE_setButtonCallback, button)
SGFE_CALLBACK_DEFINE(SGFE_axisFunc, SGFE_setAxisCallback, axis)
SGFE_CALLBACK_DEFINE(SGFE_pointerFunc, SGFE_setPointerCallback, pointer)
SGFE_CALLBACK_DEFINE(SGFE_motionFunc, SGFE_setMotionCallback, motion)

SGFE_debugFunc SGFE_debugFuncSrc;
SGFE_debugFunc SGFE_setDebugCallback(SGFE_debugFunc func) {
	SGFE_debugFunc previous = SGFE_debugFuncSrc;
	SGFE_debugFuncSrc = func;
	return previous;
}

#define SGFE_windowDeviceSleepCallback(win, is_sleeping) \
	if (win->callbacks.sleep) { \
		win->callbacks.sleep(win, is_sleeping); \
	} do {} while(0)

#define SGFE_windowQuitCallback(win) \
	if (win->callbacks.quit) { \
		win->callbacks.quit(win); \
	} do {} while(0)

#define SGFE_windowRefreshCallback(win) \
	if (win->callbacks.refresh) { \
		win->callbacks.refresh(win); \
	} do {} while(0)

#define SGFE_windowVideoModeCallback(win, video_mode) \
	if (win->callbacks.video_mode) { \
		win->callbacks.video_mode(win, video_mode); \
	} do {} while(0)

#define SGFE_windowFocusCallback(win, is_focused) \
	if (win->callbacks.focus) { \
		win->callbacks.focus(win, is_focused); \
	} do {} while(0)

#define SGFE_windowControllerCallback(win, controller_s, is_connected) \
	if (win->callbacks.controller) { \
		win->callbacks.controller(win, controller_s, is_connected); \
	} do {} while(0)

#define SGFE_windowButtonCallback(win, controller, button_s, pressed) \
	do { \
	if (win->callbacks.button) { \
		win->callbacks.button(win, controller, button_s, pressed); \
	} \
	} while(0)

#define SGFE_windowAxisCallback(win, controller, type) \
	if (win->callbacks.axis) { \
		win->callbacks.axis(win, controller, type); \
	} do {} while(0)

#define SGFE_windowPointerCallback(win, controller, type) \
	if (win->callbacks.pointer) { \
		win->callbacks.pointer(win, controller, type); \
	} do {} while(0)

#define SGFE_windowMotionCallback(win, controller, type) \
	if (win->callbacks.motion) { \
		win->callbacks.motion(win, controller, type); \
	} do {} while(0)

void SGFE_sendDebugInfo(SGFE_debugType type, SGFE_error err, SGFE_debugContext ctx, const char* msg) {
	#if defined(SGFE_3DS) && defined(SGFE_OPENGL)
	if (SGFE_getHint_OpenGL(SGFE_glNoError)) { return ; }
	#endif

	if (SGFE_debugFuncSrc) {
		SGFE_debugFuncSrc(type, err, ctx, msg);
		return;
	}

	#ifdef SGFE_DEBUG
	switch (type) {
		case SGFE_debugTypeInfo: SGFE_PRINTF("SGFE INFO (%i %i): %s\n", type, err, msg); break;
		case SGFE_debugTypeError: SGFE_PRINTF("SGFE ERROR (%i %i): %s\n", type, err, msg); break;
		case SGFE_debugTypeWarning: SGFE_PRINTF("SGFE WARNING (%i %i): %s\n", type, err, msg); break;
		default: break;
	}
	#endif
}


isize SGFE_sizeofWindow(void) { return sizeof(SGFE_window); }
isize SGFE_sizeofWindowSrc(void) { return sizeof(SGFE_windowSource); }


SGFE_context* SGFE_windowGetCurrent(SGFE_window* win) {
	#ifndef SGFE_3DS
	return  win->current[SGFE_screenPrimary];
	#else
	return win->current[!(win->_flags & SGFE_windowTopScreen)];
	#endif
}

SGFE_context* SGFE_windowGetCurrentEx(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT(win != NULL);
	return win->current[screen];
}

SGFE_context* SGFE_windowGetContext(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	#ifndef SGFE_3DS
	return &win->src.ctx;
	#else
	return SGFE_windowGetContextEx(win, !(win->_flags & SGFE_windowTopScreen));
	#endif
}

SGFE_context* SGFE_windowGetContextEx(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(screen >= 0 && screen < SGFE_screenCount);

	#ifndef SGFE_3DS
	return &win->src.ctx;
	#else
	return &win->src.ctx[screen];
	#endif
}


SGFE_contextBuffer* SGFE_windowGetCurrentBuffer(SGFE_window* win) {
	return SGFE_contextGetBuffer(SGFE_windowGetCurrent(win));
}

SGFE_contextBuffer* SGFE_windowGetCurrentExBuffer(SGFE_window* win, SGFE_screen screen) {
	return SGFE_contextGetBuffer(SGFE_windowGetCurrentEx(win, screen));
}


SGFE_contextBuffer* SGFE_windowGetContextBuffer(SGFE_window* win) {
	return SGFE_contextGetBuffer(SGFE_windowGetContext(win));
}

SGFE_contextBuffer* SGFE_windowGetContextExBuffer(SGFE_window* win, SGFE_screen screen) {
	return SGFE_contextGetBuffer(SGFE_windowGetContextEx(win, screen));
}


#ifdef SGFE_OPENGL
SGFE_contextOpenGL* SGFE_windowGetCurrentOpenGL(SGFE_window* win) {
	return SGFE_contextGetOpenGL(SGFE_windowGetCurrent(win));
}

SGFE_contextOpenGL* SGFE_windowGetCurrentExOpenGL(SGFE_window* win) {
	return SGFE_contextGetOpenGL(SGFE_windowGetCurrentEx(win));
}

SGFE_contextOpenGL* SGFE_windowGetContextBuffer(SGFE_window* win) {
	return SGFE_contextGetOpenGL(SGFE_windowGetContext(win));
}

SGFE_contextOpenGL* SGFE_windowGetContextExOpenGL(SGFE_window* win, SGFE_screen screen) {
	return SGFE_contextGetOpenGL(SGFE_windowGetContextEx(win, screen));
}
#endif


void* SGFE_window_getUserPtr(SGFE_window* win) { SGFE_ASSERT(win != NULL); return win->userPtr; }
void SGFE_window_setUserPtr(SGFE_window* win, void* ptr) { SGFE_ASSERT(win != NULL); win->userPtr = ptr; }

SGFE_bool SGFE_windowIsQueuingEvents(SGFE_window* win) { SGFE_ASSERT(win != NULL); return win->queue_events; }
void SGFE_windowSetQueueEvents(SGFE_window* win, SGFE_bool is_queuing_events) { SGFE_ASSERT(win != NULL); win->queue_events = is_queuing_events; }

SGFE_windowSource* SGFE_window_getSrc(SGFE_window* win) { SGFE_ASSERT(win != NULL); return &win->src; }

/* TODO(EimaMei): remove these. */
#define SGFE_EVENT_QUIT 		SGFE_BIT(25) /* the window close button was pressed */
#define SGFE_WINDOW_ALLOC 		SGFE_BIT(28) /* if window was allocated by SGFE */
#define SGFE_INTERNAL_FLAGS (SGFE_EVENT_QUIT | SGFE_WINDOW_ALLOC)

SGFE_window* SGFE_windowMake(SGFE_videoMode mode, SGFE_windowFlags flags) {
	SGFE_window* win = (SGFE_window*)SGFE_ALLOC(sizeof(SGFE_window));
	if (win == NULL) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOutOfMemory, SGFE_DEBUG_CTX(NULL, 0), "Failed to allocate a window.");
		return NULL;
	}

	return SGFE_windowMakePtr(mode, flags | SGFE_WINDOW_ALLOC, win);
}

SGFE_window* SGFE_windowMakePtr(SGFE_videoMode mode, SGFE_windowFlags flags,
		SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	win->event_len = 0;
	win->_flags = flags;
	win->queue_events = SGFE_FALSE;
	win->polled_events = SGFE_FALSE;
	SGFE_MEMSET(win->current, 0, sizeof(win->current));
	SGFE_MEMSET(win->controllers, 0, sizeof(win->controllers));
	SGFE_MEMSET(&win->callbacks, 0, sizeof(win->callbacks));

	SGFE_bool res = SGFE_windowMake_platform(win);
	if (res == SGFE_FALSE) { return NULL; }
	SGFE__ROOT_WIN = win;
	SGFE_windowSetFlags(win, win->_flags);

	switch (win->_flags & (SGFE_windowBuffer | SGFE_windowOpenGL)) {
		case 0: break;
		case SGFE_windowBuffer: {
			res = SGFE_windowCreateContextBuffer(win, mode, SGFE_pixelFormatRGBA8, SGFE_FALSE);
		} break;

		#ifdef SGFE_OPENGL
		case SGFE_windowOpenGL: {
			res = SGFE_window_createContext_OpenGL(win, mode);
		} break;
		#endif

		default: SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorSystem, SGFE_DEBUG_CTX(win, 0), "Multiple graphical APIs were specified. Creating nothing.");
	}

	if (!res) {
		SGFE_windowClose(win);
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorSystem, SGFE_DEBUG_CTX(win, 0), "Failed to create a graphical context.");
		return NULL;
	}

	return win;
}

SGFE_window* SGFE_windowMakeContextless(SGFE_windowFlags flags) {
	SGFE_ASSERT((flags & (SGFE_windowOpenGL | SGFE_windowBuffer)) == 0);
	return SGFE_windowMake(0, flags);
}

SGFE_window* SGFE_windowMakePtrContextless(SGFE_windowFlags flags, SGFE_window* win) {
	return SGFE_windowMakePtr(0, flags, win);
}


void SGFE_windowClose(SGFE_window* win) {
	SGFE_windowFreeContext(win);
	SGFE_windowClose_platform(win);

	#ifndef SGFE__BACKEND_FREE_WINDOW_IN_CLOSE
	if ((win->_flags & SGFE_WINDOW_ALLOC)) {
		SGFE_FREE(win);
	}
	#endif
}

void SGFE_windowFreeContext(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	for (SGFE_screen screen = 0; screen < SGFE_screenCount; screen += 1) {
		SGFE_context* ctx = SGFE_windowGetContextEx(win, screen);
		if (ctx == NULL) { continue; }

		switch (ctx->type) {
			case SGFE_contextTypeBuffer: {
				SGFE_bufferFreeContext(SGFE_contextGetBuffer(ctx));
			} break;

			#ifdef RGFW_OPENGL
			case SGFE_contextTypeOpenGL: {
				SGFE_bufferFreeContext(SGFE_contextGetOpenGL(ctx));
			} break;
			#endif
		}
	}
}


SGFE_bool SGFE_windowCheckEvent(SGFE_window* win, const SGFE_event** out_event) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(out_event != NULL);

	if (win->event_len == 0 && win->polled_events == SGFE_FALSE) {
		SGFE_windowPollEvents(win);
	}

	const SGFE_event* event = SGFE_windowEventPop(win);
	*out_event = event;
	win->polled_events = (event != NULL);

	return (event != NULL);
}

SGFE_bool SGFE_windowEventPush(SGFE_window* win, const SGFE_event* event) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(win->event_len >= 0);

	if (win->event_len >= SGFE_COUNTOF(win->events)) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorEventQueue, SGFE_DEBUG_CTX(NULL, 0), "Event queue limit 'SGFE_MAX_EVENTS' has been reached.");
		return SGFE_FALSE;
	}

	win->event_len += 1;
	win->events[SGFE_COUNTOF(win->events) - win->event_len] = *event;
	return SGFE_TRUE;
}

const SGFE_event* SGFE_windowEventPop(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(win->event_len >= 0 && win->event_len <= SGFE_COUNTOF(win->events));

	if (win->event_len <= 0) {
		return NULL;
	}
	SGFE_ASSERT(win->queue_events == SGFE_TRUE);

	SGFE_event* ev = &win->events[SGFE_COUNTOF(win->events) - win->event_len];
	win->event_len -= 1;

	return ev;
}


SGFE_bool SGFE_windowShouldClose(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	return SGFE_BOOL(win->_flags & SGFE_EVENT_QUIT);
}

void SGFE_windowSetShouldClose(SGFE_window* win, SGFE_bool shouldClose) {
	SGFE_ASSERT(win != NULL);

	if (shouldClose)  {
		win->_flags |= SGFE_EVENT_QUIT;
		SGFE_windowQuitCallback(win);
	}
	else {
		win->_flags &= ~(u32)SGFE_EVENT_QUIT;
	}
}


void SGFE_windowAssert(SGFE_window* win, SGFE_bool is_asserted,
		const char* condition_str, const char* file, isize line) {
	if (is_asserted) { return; }
	if (win == NULL) { win = SGFE__ROOT_WIN; }

	if (win == NULL) {
		win = SGFE_windowMakeContextless(SGFE_windowFlagsNone);
		if (win == NULL) {
			/* NOTE(EimaMei): In case that 'stderr' actually leads to somewhere. */
			fprintf(stderr, "%s:%zi: %s\n", file, line, condition_str);
			exit(1);
		}
	}

	SGFE_windowSetQueueEvents(win, SGFE_FALSE);
	SGFE_windowInitTerminalOutput(win);
	fprintf(stderr, "\033[91m%s:%zi\033[0m: %s\n\nPress any button to exit.\n", file, line, condition_str);

	while (!SGFE_windowShouldClose(win)) {
		SGFE_windowPollEvents(win);

		for (ssize_t i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
			SGFE_controller* controller = SGFE_windowGetController(win, i);
			if (controller->connected && controller->buttons_down != 0) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				break;
			}
		}

		SGFE_windowSwapBuffers(win);
	}

	SGFE_windowClose(win);
	exit(1);
}


SGFE_bool SGFE_windowCreateContextBuffer(SGFE_window* win, SGFE_videoMode mode,
		SGFE_pixelFormat format, SGFE_bool is_native) {
	SGFE_ASSERT(win != NULL);
	SGFE_windowFreeContext(win);

	for (SGFE_screen screen = 0; screen < SGFE_screenCount; screen += 1) {
		SGFE_context* ctx = SGFE_windowGetContextEx(win, screen);
		ctx->type = SGFE_contextTypeBuffer;

		SGFE_contextBuffer* b = &ctx->data.buffer;
		SGFE_bufferMakeWithDefaultSettings(b, mode, format, SGFE_FALSE);
		b->screen = screen;
		b->is_native = is_native;

		SGFE_bool res = SGFE_bufferAllocFramebuffers(b);
		if (res == SGFE_FALSE) { return SGFE_FALSE; }

		res = SGFE_bufferCreateContext(b);
		if (res == SGFE_FALSE) { return SGFE_FALSE; }

		SGFE_windowMakeCurrent(win, ctx);
	}

	#ifdef SGFE_3DS
	if (!win->src.lcd_is_on) {
		gspWaitForVBlank();
		GSPGPU_SetLcdForceBlack(SGFE_FALSE);
		win->src.lcd_is_on = SGFE_TRUE;
	}
	#endif

	win->_flags |= SGFE_windowBuffer;
	return SGFE_TRUE;
}


SGFE_bool SGFE_bufferSetFormat(SGFE_contextBuffer* b, SGFE_pixelFormat format) {
	if (b == NULL) { return SGFE_FALSE; }

	/* TODO(EimaMei): Have a way to save the original bpp for performance reasons. */
	if (SGFE_pixelFormatBytesPerPixel(b->format) < SGFE_pixelFormatBytesPerPixel(format)) {
		SGFE_bufferFreeContext(b);
		b->format = format;
		return SGFE_bufferCreateContext(b);
	}
	else {
		b->format = format;
	}

	return SGFE_TRUE;
}

void SGFE_bufferSetNative(SGFE_contextBuffer* b, SGFE_bool is_native) {
	SGFE_ASSERT(b != NULL);
	b->is_native = SGFE_BOOL(is_native);
}

void SGFE_bufferSetDoubleBuffered(SGFE_contextBuffer* b, SGFE_bool is_double_buffered) {
	SGFE_ASSERT(b != NULL);
	b->is_double_buffered = SGFE_BOOL(is_double_buffered);
}

void SGFE_bufferFlipFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	b->current ^= 1;
}


u8* SGFE_bufferGetFramebuffer(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->buffers[b->current];
}


SGFE_pixelFormat SGFE_contextGetFormat(const SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->format;
}

SGFE_bool SGFE_bufferIsNative(const SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->is_native;
}

SGFE_bool SGFE_bufferIsDoubleBuffered(const SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->is_double_buffered;
}

isize SGFE_contextBufferGetCurrent(const SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->current;
}


SGFE_screen SGFE_bufferGetScreen(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#ifndef SGFE_3DS
	return SGFE_screenPrimary;
	#else
	return b->screen;
	#endif
}

isize SGFE_bufferGetFramebufferCount(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->is_double_buffered ? 2 : 1;
}

SGFE_bool SGFE_bufferIsStereoscopic(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#ifndef SGFE_3DS
	return SGFE_FALSE;
	#else
	return b->screen == SGFE_screenTop && b->mode == SGFE_videoMode3D;
	#endif
}




SGFE_controller* SGFE_windowGetController(SGFE_window* win, isize port) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(port >= 0 && port < SGFE_MAX_CONTROLLERS);
	return &win->controllers[port];
}


SGFE_bool SGFE_isHeld(const SGFE_controller* controller, SGFE_button mask) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT((mask & ~SGFE_buttonGetMask(controller->type)) == 0);
	return (controller->buttons_held & mask) == mask;
}

SGFE_bool SGFE_isDown(const SGFE_controller* controller, SGFE_button mask) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT((mask & ~SGFE_buttonGetMask(controller->type)) == 0);
	return (controller->buttons_down & mask) == mask;
}

SGFE_bool SGFE_isUp(const SGFE_controller* controller, SGFE_button mask) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT((mask & ~SGFE_buttonGetMask(controller->type)) == 0);
	return (controller->buttons_up & mask) == mask;
}


SGFE_buttonType SGFE_buttonGetType(SGFE_button mask) {
	isize count = 0;
	while ((mask & 1) == 0) {
		count += 1;
		mask >>= 1;
	}
	return count;
}

SGFE_bool SGFE_iterateButtonMask(SGFE_button* buttons_mask, SGFE_buttonType* out_button) {
	SGFE_ASSERT(buttons_mask != NULL);
	SGFE_ASSERT(out_button != NULL);
	if (*buttons_mask == 0) { return SGFE_FALSE; }

	SGFE_buttonType type = SGFE_buttonGetType(*buttons_mask);
	*buttons_mask &= ~SGFE_BIT(type);
	*out_button = type;

	return SGFE_TRUE;
}


SGFE_button SGFE_buttonGetMask(SGFE_controllerType type) {
	SGFE_ASSERT(type >= 0 && type < SGFE_controllerTypeCount);
	return SGFE_BUTTON_MASK_BITS_LUT[type];
}


void SGFE_controllerGetRangeButton(const SGFE_controller* controller,
		SGFE_buttonType* out_first,SGFE_buttonType* out_last) {
	SGFE_ASSERT(controller != NULL);
	if (out_first) { *out_first = SGFE_BUTTON_RANGE_LUT[controller->type][0]; }
	if (out_last)  { *out_last  = SGFE_BUTTON_RANGE_LUT[controller->type][1]; }
}

void SGFE_controllerGetRangeAxis(const SGFE_controller* controller,
		SGFE_axisType* out_first, SGFE_axisType* out_last) {
	SGFE_ASSERT(controller != NULL);
	if (out_first) { *out_first = SGFE_AXIS_RANGE_LUT[controller->type][0]; }
	if (out_last)  { *out_last  = SGFE_AXIS_RANGE_LUT[controller->type][1]; }
}

void SGFE_controllerGetRangePointer(const SGFE_controller* controller,
		SGFE_pointerType* out_first, SGFE_pointerType* out_last) {
	SGFE_ASSERT(controller != NULL);
	if (out_first) { *out_first = SGFE_POINTER_RANGE_LUT[controller->type][0]; }
	if (out_last)  { *out_last  = SGFE_POINTER_RANGE_LUT[controller->type][1]; }
}

void SGFE_controllerGetRangeMotion(const SGFE_controller* controller,
		SGFE_motionType* out_first, SGFE_motionType* out_last) {
	SGFE_ASSERT(controller != NULL);
	if (out_first) { *out_first = SGFE_MOTION_RANGE_LUT[controller->type][0]; }
	if (out_last)  { *out_last  = SGFE_MOTION_RANGE_LUT[controller->type][1]; }
}


const char* SGFE_controllerGetName(const SGFE_controller* controller) {
	SGFE_ASSERT(controller != NULL);
	return SGFE_CONTROLLER_NAME_LUT[controller->type];
}

const char* SGFE_controllerGetNameButton(const SGFE_controller* controller,
		SGFE_buttonType type) {
	SGFE_ASSERT(controller != NULL);
	#ifdef SGFE_DEBUG
	SGFE_buttonType start, end;
	SGFE_controllerGetRangeButton(controller, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(type >= start && type <= end);
	#endif

	return SGFE_controllerGetNameButton_platform(controller, type);
}

const char* SGFE_controllerGetNameAxis(const SGFE_controller* controller,
		SGFE_axisType type) {
	SGFE_ASSERT(controller != NULL);
	#ifdef SGFE_DEBUG
	SGFE_axisType start, end;
	SGFE_controllerGetRangeAxis(controller, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(type >= start && type <= end);
	#endif

	return SGFE_AXIS_NAME_LUT[type];
}

const char* SGFE_controllerGetNamePointer(const SGFE_controller* controller,
		SGFE_pointerType type) {
	SGFE_ASSERT(controller != NULL);
	#ifdef SGFE_DEBUG
	SGFE_pointerType start, end;
	SGFE_controllerGetRangeAxis(controller, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(type >= start && type <= end);
	#endif

	return SGFE_POINTER_NAME_LUT[type];
}

const char* SGFE_controllerGetNameMotion(const SGFE_controller* controller,
		SGFE_motionType type) {
	SGFE_ASSERT(controller != NULL);
	#ifdef SGFE_DEBUG
	SGFE_motionType start, end;
	SGFE_controllerGetRangeAxis(controller, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(type >= start && type <= end);
	#endif

	return SGFE_MOTION_NAME_LUT[type];
}


SGFE_bool SGFE_controllerEnablePointer(SGFE_controller* controller, SGFE_motionType pointer,
		SGFE_bool enable) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT(pointer >= 0 && pointer < SGFE_pointerTypeCount);

	#ifdef SGFE_DEBUG
	isize start, end;
	SGFE_controllerGetRangePointer(controller, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(pointer >= start && pointer <= end);
	#endif

	SGFE_bool res = SGFE_controllerEnablePointer_platform(controller, pointer, SGFE_BOOL(enable));
	if (res) {
		controller->enabled_pointers[pointer] = SGFE_BOOL(enable);
	}

	return res;
}

SGFE_bool SGFE_controllerEnableMotion(SGFE_controller* controller, SGFE_motionType motion,
		SGFE_bool enable) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT(motion >= 0 && motion < SGFE_motionTypeCount);

	#ifdef SGFE_DEBUG
	isize start, end;
	SGFE_controllerGetRangeMotion(controller, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(motion >= start && motion <= end);
	#endif

	SGFE_bool res = SGFE_controllerEnableMotion_platform(controller, motion, SGFE_BOOL(enable));
	if (res) {
		controller->enabled_motions[motion] = SGFE_BOOL(enable);
	}

	return res;
}



void SGFE_videoModeResolution(SGFE_videoMode mode, isize* out_width, isize* out_height) {
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	if (out_width)  {  *out_width = SGFE_VIDEO_RESOLUTION_LUT[mode][0]; }
	if (out_height) { *out_height = SGFE_VIDEO_RESOLUTION_LUT[mode][1]; }
}

isize SGFE_pixelFormatBytesPerPixel(SGFE_pixelFormat format) {
	SGFE_ASSERT(format >= 0 && format < SGFE_pixelFormatCount);
	return SGFE_FORMAT_BYTES_PER_PIXEL_LUT[format];
}


const char* SGFE_pixelFormatStr(SGFE_pixelFormat format) {
	SGFE_ASSERT(format >= 0 && format < SGFE_pixelFormatCount);
	return SGFE_PIXEL_FORMAT_NAME_LUT[format];
}

const char* SGFE_videoModeStr(SGFE_videoMode mode) {
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	return SGFE_VIDEO_MODE_NAME_LUT[mode];
}


#if defined(SGFE_OPENGL)

i32 SGFE_GL_HINTS[SGFE_glHintCount] = {
	/* SGFE_glProfile                 */ SGFE_glProfile_Core,
	/* SGFE_glMajor                   */ 1,
	/* SGFE_glMinor                   */ 0,

	/* SGFE_glRed                     */ 8,
	/* SGFE_glGreen                   */ 8,
	/* SGFE_glBlue                    */ 8,
	/* SGFE_glAlpha                   */ 8,

	/* SGFE_glDoubleBuffer            */ SGFE_TRUE,
	/* SGFE_glDepth                   */ 24,
	/* SGFE_glStencil                 */ 0,

	/* SGFE_glAccumRed                */ 0,
	/* SGFE_glAccumGreen              */ 0,
	/* SGFE_glAccumBlue               */ 0,
	/* SGFE_glAccumAlpha              */ 0,

	/* SGFE_glSamples                 */ 0,
	/* SGFE_glAuxBuffers              */ 0,

	/* SGFE_glSoftwareRenderer        */ SGFE_FALSE,
	/* SGFE_glStereo                  */ SGFE_FALSE,
	/* SGFE_glSRGB                    */ SGFE_FALSE,
	/* SGFE_glRobustness              */ SGFE_FALSE,
	/* SGFE_glDebug                   */ SGFE_FALSE,
	/* SGFE_glNoError                 */ SGFE_FALSE,
	/* SGFE_glFlushOnContextChange    */ SGFE_FALSE,
	/* SGFE_glShareWithCurrentContext */ SGFE_FALSE
};

void SGFE_setHint_OpenGL(SGFE_glHint hint, i32 value) {
	SGFE_ASSERT(hint >= 0 && hint < SGFE_glHintCount);
	SGFE_GL_HINTS[hint] = value;
}

i32 SGFE_getHint_OpenGL(SGFE_glHint hint) {
	SGFE_ASSERT(hint >= 0 && hint < SGFE_glHintCount);
	return SGFE_GL_HINTS[hint];
}

#endif


#ifdef SGFE_3DS

const SGFE_button SGFE_BUTTON_MASK_BITS_LUT[SGFE_controllerTypeCount] = {
	SGFE_buttonMask_All
};

const isize SGFE_BUTTON_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{SGFE_button_A, SGFE_button_CstickDown}
};
const isize SGFE_AXIS_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{SGFE_axisLeftX, SGFE_axisLeftY},
};
const isize SGFE_POINTER_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{SGFE_pointerTouchscreen, SGFE_pointerTouchscreen},
};
const isize SGFE_MOTION_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{SGFE_motionAccelerometer, SGFE_motionGyroscope}
};

const char* SGFE_CONTROLLER_NAME_LUT[SGFE_controllerTypeCount] = {
	"Nintendo 3DS"
};
const char* SGFE_BUTTON_NAMES_3DS_LUT[SGFE_buttonCount] = {
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
const char* SGFE_AXIS_NAME_LUT[SGFE_axisTypeCount] = {
	"Circle-Pad Left X", "Circle-Pad Left Y"
};
const char* SGFE_POINTER_NAME_LUT[SGFE_pointerTypeCount] = {
	"Touchscreen"
};
const char* SGFE_MOTION_NAME_LUT[SGFE_motionTypeCount] = {
	"Accelerometer", "Gyroscope"
};

const isize SGFE_VIDEO_RESOLUTION_LUT[SGFE_videoModeCount][2] = {
	{400, 240}, {800, 240}, {800, 240},
};

const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_pixelFormatCount] = {
	4, 3, 2, 2, 2
};



void SGFE__aptHookCallback(APT_HookType hook, void* param);
void _SGFE__gspPresentFramebuffer(SGFE_contextBuffer* b, u8* buffer);

void SGFE__3dsControllerCstick(SGFE_window* win, SGFE_controller* controller, 
		SGFE_axisType which, i16 value);

/* NOTE(EimaMei): sys/iosupport.h stuff alongside some initialization stuff
 * from consoleInit. The reason why we define everything is so that we
 * wouldn't have to include them.  */
extern isize con_write(struct _reent *r,void *fd,const char *ptr, size_t len);
struct _SGFE_devoptab_t {
	const char *name;
	size_t structSize; void *open_r; void *close_r;
	isize (*write_r)(struct _reent *r,
	void *fd, const char *ptr, size_t len); void *read_r; void *seek_r;  void *fstat_r;
	void *stat_r; void *link_r; void *unlink_r; void *chdir_r; void *rename_r;
	void *mkdir_r; size_t dirStateSize; void *diropen_r; void *dirreset_r;
	void *dirnext_r; void *dirclose_r; void *statvfs_r; void *ftruncate_r;
	void *fsync_r; void *deviceData; void *chmod_r; void *fchmod_r; void *rmdir_r;
	void *lstat_r; void *utimes_r; void *fpathconf_r; void *pathconf_r; void *symlink_r;
	void *readlink_r;
};
extern const struct _SGFE_devoptab_t* devoptab_list[];

extern PrintConsole* currentConsole;
extern PrintConsole defaultConsole;


void SGFE__aptHookCallback(APT_HookType hook, void* param) {
	SGFE_window* win = param;

	static const SGFE_eventType APT_HOOK_LUT[APTHOOK_COUNT] = {
		SGFE_eventFocusOut,
		SGFE_eventFocusIn,
		SGFE_eventDeviceSleep,
		SGFE_eventDeviceWakeup,
		SGFE_eventQuit
	};

	/* TODO(EimaMei): maybe add a way to disbale "focus out" by not allowing to press home. */
	SGFE_event event;
	event.type = APT_HOOK_LUT[hook];

	SGFE_bool res = SGFE_windowEventPush(win, &event);
	if (res == SGFE_FALSE) { return; }

	if (hook <= APTHOOK_ONRESTORE) {
		SGFE_windowFocusCallback(win, event.type == SGFE_eventFocusIn);
	}
	else if (hook <= APTHOOK_ONWAKEUP) {
		SGFE_windowDeviceSleepCallback(win, event.type == SGFE_eventDeviceSleep);
	}
	else {
		SGFE_windowQuitCallback(win);
	}
}

/* NOTE(EimaMei): Taken from libctru gfx.c */
void _SGFE__gspPresentFramebuffer(SGFE_contextBuffer* b, u8* buffer) {
	u32 stride = GSP_SCREEN_WIDTH * (u32)SGFE_pixelFormatBytesPerPixel(b->format);
	u32 pixel_format = (u32)b->format | (1 << 8);

	u8* fb_b = buffer;
	if (b->screen == 0) {
		switch (b->mode) {
		case SGFE_videoMode2D: {
			pixel_format |= BIT(6);
		} break;

		case SGFE_videoMode3D: {
			pixel_format |= BIT(5);
			if (SGFE_platformGet3DSlider() > 0.0f) fb_b += b->size / 2;
		} break;
		}
	}

	gspPresentBuffer((u32)b->screen, (u32)b->current, buffer, fb_b, stride, pixel_format);
}

void SGFE__3dsControllerCstick(SGFE_window* win, SGFE_controller* controller, 
		SGFE_axisType which, i16 value) {
	SGFE_axis* axis = &controller->axes[which];
	axis->value = (float)value / 156.0f; 

	SGFE_windowAxisCallback(win, controller, which);
	if (win->queue_events) {
		SGFE_event event;
		event.type = SGFE_eventAxis;
		event.axis.controller = controller;
		event.axis.which = which;
		event.axis.value = axis->value;
		event.axis.deadzone = axis->deadzone;
		SGFE_windowEventPush(win, &event);
	}
}




SGFE_bool SGFE_windowMake_platform(SGFE_window* win) {
	if ((win->_flags & SGFE_windowDualScreen) == 0) {
		win->_flags |= SGFE_windowTopScreen;
	}

	SGFE_controller* controller = &win->controllers[0];
	controller->port = 0;
	controller->type = SGFE_controllerTypeStandard;
	controller->connected = SGFE_TRUE;
	controller->enabled_pointers[SGFE_pointerTouchscreen] = SGFE_TRUE;
	
	for (SGFE_axisType which = 0; which < SGFE_axisTypeCount; which += 1) {
		/* NOTE(EimaMei): I picked '40' as the deadzone based on how the CPAD bits
		 * are set if the value is larger than 40. (http://3dbrew.org/wiki/HID_Shared_Memory). */
		controller->axes[which].deadzone = (40.0f / 156.0f);
	}

	SGFE_windowSource* src = &win->src;
	SGFE_MEMSET(src->ctx, 0, sizeof(src->ctx));
	src->lcd_is_on = SGFE_FALSE;

	aptHook(&src->apt_hook, SGFE__aptHookCallback, win);

	Result res = gspInit();
	if (res != 0) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorSystem, SGFE_DEBUG_CTX(win, 0), "Failed to initialize GSP.");
		return SGFE_FALSE;
	}

	#ifdef SGFE_OPENGL
	gfxInitDefault();
	SGFE_bool kygx_init = kygxInit();
	if (!kygx_init) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Failed to initialize KYGX.");
		return SGFE_FALSE;
	}
	#endif

	return SGFE_TRUE;
}


void SGFE_windowClose_platform(SGFE_window* win) {
	if (gspHasGpuRight()) {
		gspWaitForVBlank();
		GSPGPU_SetLcdForceBlack(SGFE_TRUE);
	}
	gspExit();

	#ifdef SGFE_OPENGL
	kygxExit();
	gfxExit();
	#endif
	SGFE_UNUSED(win);
}


void SGFE_windowSetFlags(SGFE_window* win, SGFE_windowFlags flags) {
	SGFE_ASSERT(win != NULL);
	win->_flags = flags | (win->_flags & SGFE_INTERNAL_FLAGS);
}


void SGFE_windowPollEvents(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	/* TODO(EimaMei): devkitPro actually gives you access to 'hidSharedMem' and
	 * 'hidMemHandle', allowing you to manage input yourself. Might have to take
	 * a better look at it. */
	if (!aptMainLoop()) {
		SGFE_windowSetShouldClose(win, SGFE_TRUE);
		SGFE_windowQuitCallback(win);

		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventQuit;
			SGFE_windowEventPush(win, &event);
		}
		return;
	}

	hidScanInput();
	u32 held = hidKeysHeld(),
		down = hidKeysDown(),
		up = hidKeysUp();

	SGFE_controller* controller = SGFE_windowGetController(win, 0);
	controller->buttons_held = SGFE_platformButtonFromAPI(held);
	controller->buttons_down = SGFE_platformButtonFromAPI(down);
	controller->buttons_up   = SGFE_platformButtonFromAPI(up);

	if (controller->buttons_held != 0) {
		SGFE_windowButtonCallback(win, controller, controller->buttons_held, SGFE_TRUE);

		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventButtonDown;
			event.button.controller = controller;
			event.button.buttons = controller->buttons_held;
			event.button.buttons_down = controller->buttons_down;
			event.button.raw_buttons = held;
			event.button.raw_buttons_down = down;
			SGFE_windowEventPush(win, &event);
		}
	}

	if (controller->buttons_up != 0) {
		SGFE_windowButtonCallback(win, controller, controller->buttons_up, SGFE_FALSE);
		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventButtonUp;
			event.button.controller = controller;
			event.button.buttons = controller->buttons_up;
			event.button.buttons_down = 0;
			event.button.raw_buttons = up;
			event.button.raw_buttons_down = 0;
			SGFE_windowEventPush(win, &event);
		}
	}

	circlePosition cpad;
	hidCircleRead(&cpad);

	if (held & (KEY_CPAD_LEFT | KEY_CPAD_RIGHT)) {
		SGFE__3dsControllerCstick(win, controller, SGFE_axisLeftX, cpad.dx);
	}

	if (held & (KEY_CPAD_UP | KEY_CPAD_DOWN)) {
		SGFE__3dsControllerCstick(win, controller, SGFE_axisLeftY, cpad.dy);
	}

	if (controller->enabled_motions[SGFE_motionAccelerometer]) {
		accelVector accel;
		hidAccelRead(&accel);

		float* xyz = (float*)&controller->motions[SGFE_motionAccelerometer];
		xyz[0] = (float)accel.x / 512.0f * SGFE_STANDARD_GRAVITY;
		xyz[1] = (float)accel.y / 512.0f * SGFE_STANDARD_GRAVITY;
		xyz[2] = (float)accel.z / 512.0f * SGFE_STANDARD_GRAVITY;
			
		SGFE_windowPointerCallback(win, controller, SGFE_motionAccelerometer);
		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventMotion;
			event.motion.controller = controller;
			event.motion.which = SGFE_motionAccelerometer;
			event.motion.x = xyz[0];
			event.motion.y = xyz[1];
			event.motion.z = xyz[2];
			SGFE_windowEventPush(win, &event);
		}
	}

	if (controller->enabled_motions[SGFE_motionGyroscope]) {
		angularRate gyro;
		hidGyroRead(&gyro);

		float coeff = 1.0f;
		HIDUSER_GetGyroscopeRawToDpsCoefficient(&coeff);

		float* xyz = (float*)&controller->motions[SGFE_motionGyroscope];
		xyz[0] = (float)gyro.x * coeff * SGFE_PI / 180.0f;
		xyz[1] = (float)gyro.y * coeff * SGFE_PI / 180.0f;
		xyz[2] = (float)gyro.z * coeff * SGFE_PI / 180.0f;
			
		SGFE_windowPointerCallback(win, controller, SGFE_motionGyroscope);
		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventMotion;
			event.motion.controller = controller;
			event.motion.which = SGFE_motionGyroscope;
			event.motion.x = xyz[0];
			event.motion.y = xyz[1];
			event.motion.z = xyz[2];
			SGFE_windowEventPush(win, &event);
		}
	}

	if (controller->enabled_pointers[SGFE_pointerTouchscreen] && (held & KEY_TOUCH)) {
		touchPosition touch;
		hidTouchRead(&touch);

		isize* xy = controller->pointers[SGFE_pointerTouchscreen];
		xy[0] = touch.px;
		xy[1] = touch.py;

		SGFE_windowPointerCallback(win, controller, SGFE_pointerTouchscreen);
		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventPointer;
			event.pointer.controller = controller;
			event.pointer.which = SGFE_pointerTouchscreen;
			event.pointer.x = xy[0];
			event.pointer.y = xy[1];
			SGFE_windowEventPush(win, &event);
		}
	}
}


void SGFE_windowSwapBuffers(SGFE_window* win) {
	SGFE_context* ctx = SGFE_windowGetCurrent(win);
	SGFE_ASSERT(ctx != NULL);

	switch (ctx->type) {
		case SGFE_contextTypeBuffer: {
			SGFE_ASSERT(win->current[0] && win->current[1]);
			SGFE_ASSERT(win->current[0]->type == win->current[1]->type);

			for (SGFE_screen screen = 0; screen < SGFE_screenCount; screen += 1) {
				ctx = SGFE_windowGetCurrentEx(win, screen);
				SGFE_contextBuffer* b = SGFE_windowGetContextExBuffer(win, screen);
				u8* buffer = SGFE__fetchSwapBuffer(b);
				GSPGPU_FlushDataCache(buffer, b->size);

				_SGFE__gspPresentFramebuffer(b, buffer);
				b->current ^= b->is_double_buffered;
			}
			gspWaitForVBlank();
		} break;
	}
}

void SGFE_windowMakeCurrent(SGFE_window* win, SGFE_context* ctx) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(ctx != NULL);

	switch (SGFE_contextGetType(ctx)) {
		case SGFE_contextTypeBuffer: {
			win->current[ctx->data.buffer.screen] = ctx;
		} break;
	}
}


SGFE_bool SGFE_windowInitTerminalOutput(SGFE_window* win) {
	/* TODO(EimaMei): Remove this entire function and replace it with a helper library
	 * and function. */
	/* NOTE(EimaMei): Taken from libctru console.c */
	static SGFE_bool console_initialized = SGFE_FALSE;
	SGFE_ASSERT(win != NULL);

	SGFE_context* ctx = SGFE_windowGetContext(win);
	if (ctx->type == SGFE_contextTypeNone) {
		SGFE_bool res = SGFE_windowCreateContextBuffer(win, SGFE_videoModeOptimal(), SGFE_pixelFormatRGB565, SGFE_TRUE);

		if (res == SGFE_FALSE) {
			return SGFE_FALSE;
		}
	}

	SGFE_contextBuffer* b = &ctx->data.buffer;
	SGFE_bufferSetNative(b, SGFE_TRUE);
	SGFE_bufferSetFormat(b, SGFE_pixelFormatRGB565);
	SGFE_bufferSetDoubleBuffered(b, SGFE_FALSE);

	if (!console_initialized) {
		/* NOTE(EimaMei): Taken from libctru console.c */
		static struct _SGFE_devoptab_t dotab_stdout;
		dotab_stdout.name = "con";
		dotab_stdout.write_r = con_write;

		devoptab_list[1] = &dotab_stdout;
		devoptab_list[2] = &dotab_stdout;

		setvbuf(stdout, NULL, _IONBF, 0);
		setvbuf(stderr, NULL, _IONBF, 0);

		console_initialized = SGFE_TRUE;
	}
	*currentConsole = defaultConsole;
	currentConsole->consoleInitialised = SGFE_TRUE;
	currentConsole->frameBuffer = (u16*)(void*)SGFE_bufferGetFramebuffer(b);

	/* NOTE(EimaMei): Taken from libctru console.c */
	if (b->screen == SGFE_screenTop) {
		SGFE_bool is_wide = (b->mode == SGFE_videoModeWide);
		currentConsole->consoleWidth = is_wide ? 100 : 50;
		currentConsole->windowWidth = is_wide ? 100 : 50;
	}

	/* TODO(EimaMei): Should be kept in the library. */
	/*if ((win->_flags & SGFE_windowDualScreen) == SGFE_windowDualScreen) {
		win->current ^= 1;
	}*/

	consoleClear();
	SGFE_windowSwapBuffers(win);
	return SGFE_TRUE;
}



SGFE_button SGFE_buttonFromAPI(SGFE_controllerType type, u32 mask) {
	SGFE_ASSERT(type > 0 && type <= SGFE_controllerTypeCount);
	return SGFE_platformButtonFromAPI(mask);
}

u32 SGFE_buttonToAPI(SGFE_controllerType type, SGFE_button button) {
	SGFE_ASSERT((button & ~SGFE_buttonGetMask(type)) == 0);
	return SGFE_platformButtonToAPI(button);
}


const char* SGFE_controllerGetNameButton_platform(const SGFE_controller* controller,
		SGFE_buttonType button) {
	return SGFE_BUTTON_NAMES_3DS_LUT[button];
	SGFE_UNUSED(controller);
}


SGFE_bool SGFE_controllerEnablePointer_platform(SGFE_controller* controller,
		SGFE_pointerType pointer, SGFE_bool enable) {
	return SGFE_TRUE;
	SGFE_UNUSED(controller); SGFE_UNUSED(pointer); SGFE_UNUSED(enable);
}

SGFE_bool SGFE_controllerEnableMotion_platform(SGFE_controller* controller,
		SGFE_motionType motion, SGFE_bool enable) {
	Result res;
	switch (motion) {
		case SGFE_motionAccelerometer: {
			if (enable) res = HIDUSER_EnableAccelerometer();
			else        res = HIDUSER_DisableAccelerometer();
		} break;

		case SGFE_motionGyroscope: {
			if (enable) res = HIDUSER_EnableGyroscope();
			else        res = HIDUSER_DisableGyroscope();
		} break;
	}

	return (res == 0);
	SGFE_UNUSED(controller); 
}



SGFE_bool SGFE_bufferMakeWithDefaultSettings(SGFE_contextBuffer* out_buffer,
		SGFE_videoMode mode, SGFE_pixelFormat format, SGFE_bool allocate_buffers) {
	SGFE_ASSERT(out_buffer != NULL);
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	SGFE_ASSERT(format >= 0 && format < SGFE_pixelFormatCount);

	SGFE_contextBuffer* b = out_buffer;
	b->screen = SGFE_screenPrimary;
	b->mode = mode;
	b->format = format;

	b->current = 0;

	b->is_buffer_allocated = SGFE_FALSE;
	b->is_double_buffered = SGFE_FALSE;
	b->is_native = SGFE_FALSE;

	SGFE_bool res;
	if (allocate_buffers) {
		res = SGFE_bufferAllocFramebuffers(out_buffer);
	}
	else {
		res = SGFE_TRUE;
		SGFE_MEMSET(b->buffers, 0, sizeof(b->buffers));
		#ifndef SGFE_BUFFER_NO_CONVERSION
		SGFE_MEMSET(b->buffers_native, 0, sizeof(b->buffers_native));
		#endif
	}

	b->size = 0;
	return res;
}


SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);

	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);
	b->size = (u32)(width * height * SGFE_pixelFormatBytesPerPixel(b->format));
	b->current = 0;

	#ifndef SGFE_BUFFER_NO_CONVERSION
	_SGFE__gspPresentFramebuffer(b, (b->is_native) ? b->buffers[0] : b->buffers_native[0]);
	#else
	_SGFE__gspPresentFramebuffer(b, b->buffers[0]);
	#endif

	SGFE_sendDebugInfo(SGFE_debugTypeInfo, SGFE_infoBuffer, SGFE_DEBUG_CTX(0, 0), "Creating framebuffers");
	return SGFE_TRUE;
}

void SGFE_bufferFreeContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	SGFE_bufferFreeFramebuffers(b);
}


SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	SGFE_bufferFreeFramebuffers(b);

	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);

	isize size = width * height * SGFE_pixelFormatBytesPerPixel(b->format);

	if (b->is_native) {
		u8* buffers = linearAlloc((size_t)(2 * size));
		if (buffers == NULL) { return SGFE_FALSE; }

		b->buffers[0] = &buffers[0 * size];
		b->buffers[1] = &buffers[1 * size];
	}
	else {
		u8* buffers = SGFE_ALLOC((size_t)(2 * size));
		if (buffers == NULL) { return SGFE_FALSE; }

		#ifndef SGFE_BUFFER_NO_CONVERSION
		u8* native_buffers = linearAlloc((size_t)(2 * size));
		if (native_buffers == NULL) { return SGFE_FALSE; }
		#endif

		b->buffers[0] = &buffers[0 * size];
		b->buffers[1] = &buffers[1 * size];

		#ifndef SGFE_BUFFER_NO_CONVERSION
		b->buffers_native[0] = &native_buffers[0 * size];
		b->buffers_native[1] = &native_buffers[1 * size];
		#endif
	}

	b->is_buffer_allocated = SGFE_TRUE;
	return SGFE_TRUE;
}

SGFE_bool SGFE_bufferFreeFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	if (b->buffers[0] == NULL || !b->is_buffer_allocated) { return SGFE_FALSE; }

	if (b->is_native) {
		linearFree(b->buffers[0]);
		b->buffers[0] = NULL;
		b->buffers[1] = NULL;
	}
	else {
		SGFE_FREE(b->buffers[0]);
		b->buffers[0] = NULL;
		b->buffers[1] = NULL;

		#ifndef SGFE_BUFFER_NO_CONVERSION
		linearFree(b->buffers_native[0]);
		b->buffers_native[0] = NULL;
		b->buffers_native[1] = NULL;
		#endif
	}

	b->is_buffer_allocated = SGFE_FALSE;
	return SGFE_TRUE;
}

u8* SGFE_bufferConvertFramebufferToNative(SGFE_contextBuffer* b) {
	u8* src = SGFE_bufferGetFramebuffer(b);

	if (b->is_native) {
		return src;
	}
	u8* dst = b->buffers_native[b->current];

	isize bpp = SGFE_pixelFormatBytesPerPixel(b->mode);
	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);

	for (isize i = 0; i < width; i += 1) {
		for (isize j = height - 1; j >= 0; j -= 1) {
			isize pixel = bpp * (i * height + j);
			isize opixel = bpp * ((height - 1 - j) * width + i);
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


void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height) {
	SGFE_ASSERT(b != NULL);

	if (SGFE_bufferIsNative(b)) {
		if (b->screen == SGFE_screenTop) {
			SGFE_videoModeResolution(b->mode, out_height, out_width);
		}
		else {
			if (out_width)  {  *out_width = 240; }
			if (out_height) { *out_height = 320; }
		}
	}
	else {
		if (b->screen == SGFE_screenTop) {
			SGFE_videoModeResolution(b->mode, out_width, out_height);
		}
		else {
			if (out_width)  {  *out_width = 320; }
			if (out_height) { *out_height = 240; }
		}
	}
}



SGFE_videoMode SGFE_videoModeOptimal(void) {
	return SGFE_videoMode2D;
}

SGFE_videoMode SGFE_pixelFormatOptimal(void) {
	return SGFE_pixelFormatBGR8;
}







#ifdef SGFE_OPENGL

SGFE_bool SGFE_createContext_OpenGL(SGFE_screen screen, SGFE_videoMode mode, SGFE_contextOpenGL* out_gl) {
	SGFE_ASSERT(screen >= 0 && screen <= SGFE_screenCount);
	SGFE_ASSERT(out_gl != NULL);

	if (SGFE_getHint_OpenGL(SGFE_glProfile) != SGFE_glProfile_ES) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "3DS only supports GLES.");
		return SGFE_FALSE;
	}

	if (SGFE_getHint_OpenGL(SGFE_glMajor) != 1 || (SGFE_getHint_OpenGL(SGFE_glMinor) != 0 && SGFE_getHint_OpenGL(SGFE_glMinor) != 1)) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "3DS can only support up to GLES 1.1.");
		return SGFE_FALSE;
	}

	i32 stencil = SGFE_getHint_OpenGL(SGFE_glStencil);
	if (stencil > 8) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported stencil buffer size.");
		return SGFE_FALSE;
	}
	else if (stencil != 0 && stencil != 8) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported stencil buffer size. Defaulting to 8.");
		SGFE_setHint_OpenGL(SGFE_glStencil, 8);
	}

	i32 depth = SGFE_getHint_OpenGL(SGFE_glStencil);
	if (depth > 24) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported depth buffer size.");
		return SGFE_FALSE;
	}
	else if (stencil != 0 && stencil != 24) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported depth buffer size. Defaulting to 24.");
		SGFE_setHint_OpenGL(SGFE_glStencil, 24);
	}

	i32 r = SGFE_getHint_OpenGL(SGFE_glRed),
		g = SGFE_getHint_OpenGL(SGFE_glGreen),
		b = SGFE_getHint_OpenGL(SGFE_glBlue),
		a = SGFE_getHint_OpenGL(SGFE_glAlpha);

	GLenum internal_format;
	if (a == 0) {
		if (r <= 5 && g <= 6 && b <= 5) {
			internal_format = GL_RGB565;
			out_gl->format = SGFE_pixelFormatRGB565;

			if (r != 5 || g != 6 || b != 5) {
				SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGB565.");
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGB8_OES;
			out_gl->format = SGFE_pixelFormatBGR8;

			if (r != 8 || g != 8 || b != 8) {
				SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGB8.");
			}
		}
		else {
			SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return SGFE_FALSE;
		}
	}
	else if (a == 1) {
		if (r <= 5 && g <= 5 && b <= 5) {
			internal_format = GL_RGB5_A1;
			out_gl->format = SGFE_pixelFormatRGB5_A1;

			if (r != 5 || g != 5 || b != 5) {
				SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGB5_A1.");
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA8_OES;
			out_gl->format = SGFE_pixelFormatRGBA8;
			SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA8.");
		}
		else {
			SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return SGFE_FALSE;
		}
	}
	else if (a <= 4) {
		if (r <= 4 && g <= 4 && b <= 4) {
			internal_format = GL_RGBA4;
			out_gl->format = SGFE_pixelFormatRGBA4;

			if (r != 4 || g != 4 || b != 4 || a != 4) {
				SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA4.");
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA4;
			out_gl->format = SGFE_pixelFormatRGBA4;

			SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA8.");
		}
		else {
			SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return SGFE_FALSE;
		}
	}
	else if (a <= 8) {
		if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA8_OES;
			out_gl->format = SGFE_pixelFormatRGBA8;

			if (r != 8 || g != 8 || b != 8 || a != 8) {
				SGFE_sendDebugInfo(SGFE_debugTypeWarning, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format. Defaulting to RGBA8.");
			}
		}
		else {
			SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
			return SGFE_FALSE;
		}
	}
	else {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Unsupported color buffer format.");
		return SGFE_FALSE;
	}

	out_gl->mode = (SGFE_getHint_OpenGL(SGFE_glStereo) && screen == SGFE_screenTop)
		? SGFE_videoMode3D
		: mode;

	GLASSCtxParams param;
	glassGetDefaultContextParams(&param, GLASS_VERSION_ES_2);
	param.targetScreen = (GLASSScreen)screen;

	out_gl->ctx = glassCreateContext(&param);
	if (out_gl->ctx == NULL) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(NULL, 0), "Failed to create a GLASS context.");
		return SGFE_FALSE;
	}
	glassBindContext(out_gl->ctx);

	GLint width = (screen == SGFE_screenTop)
		? SGFE_videoModeResolution(out_gl->mode).h
		: 320;
	glGenRenderbuffers(1, &out_gl->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, out_gl->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, 240);

	glGenFramebuffers(1, &out_gl->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, out_gl->framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, out_gl->renderbuffer);

	glViewport(0, 0, width, 240);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return SGFE_TRUE;
}

void SGFE_context_free_OpenGL(SGFE_contextOpenGL* out_gl) {
	SGFE_ASSERT(out_gl != NULL);
	if (out_gl->ctx == NULL) { return; }

	glassBindContext(out_gl->ctx);
	glDeleteRenderbuffers(1, &out_gl->renderbuffer);
	glDeleteFramebuffers(1, &out_gl->framebuffer);

	glassDestroyContext(out_gl->ctx);
	glassBindContext(NULL);
	out_gl->ctx = NULL;
}

void SGFE_context_swapInterval_OpenGL(SGFE_contextOpenGL* gl, i32 swap_interval) {
	SGFE_ASSERT(gl != NULL);
	if (gl->ctx) {
		glassSetVSync(gl->ctx, SGFE_BOOL(swap_interval));
	}
}


SGFE_bool SGFE_window_createContext_OpenGL(SGFE_window* win, SGFE_videoMode mode) {
	SGFE_ASSERT(win != NULL);

	SGFE_bool res;
	switch (win->_flags & SGFE_windowDualScreen) {
		case SGFE_windowTopScreen:    res = SGFE_createContext_OpenGL(SGFE_screenTop, mode, &win->src.gl[SGFE_screenTop]); break;
		case SGFE_windowBottomScreen: res = SGFE_createContext_OpenGL(SGFE_screenBottom, mode, &win->src.gl[SGFE_screenBottom]); break;
		case SGFE_windowDualScreen: {
			res = SGFE_createContext_OpenGL(SGFE_screenBottom, mode, &win->src.gl[SGFE_screenBottom]);
			res = SGFE_createContext_OpenGL(SGFE_screenTop, mode, &win->src.gl[SGFE_screenTop]);
		} break;

		default: {
			SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext, SGFE_DEBUG_CTX(win, 0), "No screen was specified to create an OpenGL context.");
			return SGFE_FALSE;
		}
	}

	if (!res) {
		return SGFE_FALSE;
	}
	SGFE_window_makeCurrent_OpenGL(win, &win->src.gl[!(win->_flags & SGFE_windowTopScreen)]);

	win->_flags |= SGFE_windowOpenGL;
	return res;
}

void SGFE_window_freeContext_OpenGL(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	if ((win->_flags & SGFE_windowOpenGL) == 0) {
		return;
	}

	if (win->gl == &win->src.gl[SGFE_screenTop] || win->gl == &win->src.gl[SGFE_screenBottom]) {
		SGFE_window_makeCurrent_OpenGL(win, NULL);
	}

	switch (win->_flags & SGFE_windowDualScreen) {
		case SGFE_windowTopScreen: SGFE_context_free_OpenGL(&win->src.gl[SGFE_screenTop]); break;
		case SGFE_windowBottomScreen: SGFE_context_free_OpenGL(&win->src.gl[SGFE_screenBottom]); break;
		case SGFE_windowDualScreen: {
			SGFE_context_free_OpenGL(&win->src.gl[SGFE_screenTop]);
			SGFE_context_free_OpenGL(&win->src.gl[SGFE_screenBottom]);
		} break;
	}

	win->_flags &= ~(SGFE_windowFlags)SGFE_windowOpenGL;
	SGFE_sendDebugInfo(SGFE_debugTypeInfo, SGFE_infoOpenGL, SGFE_DEBUG_CTX(win, 0), "OpenGL context freed.");
}

void SGFE_window_swapInterval_OpenGL(SGFE_window* win, i32 swap_interval) {
	SGFE_ASSERT(win != NULL);
	SGFE_context_swapInterval_OpenGL(win->gl, swap_interval);
}

void SGFE_window_swapBuffers_OpenGL(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	glassSwapContextBuffers(win->src.gl[SGFE_screenTop].ctx, win->src.gl[SGFE_screenBottom].ctx);
}

void SGFE_window_makeCurrent_OpenGL(SGFE_window* win, SGFE_contextOpenGL* out_gl) {
	SGFE_ASSERT(win != NULL);

	win->gl = out_gl;
	glassBindContext(win->gl ? win->gl->ctx : NULL);
}


void* SGFE_getCurrentContext_OpenGL(void) {
	return glassGetBoundContext();
}

#endif



SGFE_systemModel SGFE_platformGetModel(void) {
	u8 model;
	Result res = CFGU_GetSystemModel(&model);
	if (res != 0) { return SGFE_systemModelUnknown; }

	return (model <= SGFE_systemModelN2DSXL) ? (model - 1) : SGFE_systemModelUnknown;
}


SGFE_button SGFE_platformButtonFromAPI(u32 mask) {
	#define SGFE__BUTTONS_BITS (u32)(0x7FF)
	#define SGFE__CSTICK_BITS (u32)(KEY_CSTICK_RIGHT | KEY_CSTICK_LEFT | KEY_CSTICK_UP | KEY_CSTICK_DOWN)
	#define SGFE__ZL_ZR_BITS (u32)(KEY_ZL | KEY_ZR)

	return 
		((mask & SGFE__CSTICK_BITS) >> 11) |
		((mask & SGFE__ZL_ZR_BITS) >> 2) | 
		((mask & SGFE__BUTTONS_BITS));

	#define SGFE__BUTTONS_BITS (u32)(0x7FF)
	#define SGFE__CSTICK_BITS (u32)(KEY_CSTICK_RIGHT | KEY_CSTICK_LEFT | KEY_CSTICK_UP | KEY_CSTICK_DOWN)
	#define SGFE__ZL_ZR_BITS (u32)(KEY_ZL | KEY_ZR)
}

u32 SGFE_platformButtonToAPI(SGFE_button button) {
	return 
		((button & SGFE_buttonMask_Cstick) << 11) |
		((button & SGFE_buttonMask_ZL_ZR) << 2) | 
		(button & (SGFE_buttonMask_Dpad | SGFE_buttonMask_Face | SGFE_buttonMask_Shoulder));
}


float SGFE_platformGet3DSlider(void) {
	return OS_SharedConfig->slider_3d;
}


#ifdef SGFE_OPENGL
SGFE_bool SGFE_platformRotateScreenOpenGL(GLuint shader_program, const char* mat4_uniform_name) {
	static const float deg90_rotation_matrix[4][4] = {
		{ 0.0f,  1.0f, 0.0f, 0.0f },
		{-1.0f,  0.0f, 0.0f, 0.0f },
		{ 0.0f,  0.0f, 1.0f, 0.0f },
		{ 0.0f,  0.0f, 0.0f, 1.0f }
	};

	GLint uniform = glGetUniformLocation(shader_program, mat4_uniform_name);
	if (uniform == -1) {
		SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOpenGLContext,  SGFE_DEBUG_CTX(NULL, 0), "Invalid uniform name.");
		return SGFE_FALSE;
	}

	glUniformMatrix4fv(uniform, 1, GL_FALSE, (const float*)deg90_rotation_matrix);
	return SGFE_TRUE;
}
#endif



#endif /* SGFE_3DS */




#ifdef SGFE_WII

i32 SGFE__WII_EXIT_STATE;

extern void *memalign (size_t, size_t);
extern int usleep (__useconds_t __useconds);

SGFE_button SGFE_buttonFromAPI_WiiRemote(u32 mask);
SGFE_button SGFE_buttonFromAPI_Nunchuk(u32 mask);

void SGFE__callbackWiiReset(u32 irq, void* ctx);
void SGFE__callbackWiiPower(void);
void SGFE__callbackWiiRemotePower(i32 channel);

void SGFE__wiiControllerOnConnect(SGFE_controller* controller, isize port, SGFE_bool is_connected);
void SGFE__wiiControllerAxis(SGFE_window* win, SGFE_controller* controller, SGFE_axisType type, WPADData* data);
void SGFE__wiiControllerSensor(SGFE_window* win, SGFE_controller* controller, WPADData* data);
void SGFE__wiiControllerMotion_wiimote(SGFE_window* win, SGFE_controller* controller);
void SGFE__wiiControllerMotion_nunchuk(SGFE_window* win, SGFE_controller* controller);

void SGFE__wiiCheckGXMode(SGFE_contextBuffer* b);



SGFE_button SGFE_buttonFromAPI_WiiRemote(u32 mask) {
	return ((mask & (u32)~0x1F) >> 2) | (mask & 0x1F);
}

SGFE_button SGFE_buttonFromAPI_Nunchuk(u32 mask) {
	return ((mask & ~0xFFFFu) >> 5) | SGFE_buttonFromAPI_WiiRemote(mask & 0xFFFF);
}

void SGFE__callbackWiiReset(u32 irq, void* ctx) { SGFE__WII_EXIT_STATE = SYS_RETURNTOMENU; SGFE_UNUSED(irq); SGFE_UNUSED(ctx); }
void SGFE__callbackWiiPower(void) { SGFE__WII_EXIT_STATE = SYS_POWEROFF_STANDBY; }
void SGFE__callbackWiiRemotePower(i32 channel) { SGFE__WII_EXIT_STATE = SYS_POWEROFF_STANDBY; SGFE_UNUSED(channel); }

void SGFE__wiiControllerOnConnect(SGFE_controller* controller, isize port, SGFE_bool is_connected) {
	controller->port = port;
	controller->connected = is_connected;

	if (is_connected) {
		WPADData* data = WPAD_Data(port);
		isize connect_retries = 0;

		data->exp.type = 0xFF;
		while (data->exp.type == 0xFF && connect_retries < 1000) {
			WPAD_ReadPending(port, NULL);
			connect_retries += 1;
			usleep(1000);
		}

		switch (data->exp.type) {
			case WPAD_EXP_NONE: controller->type = SGFE_controllerTypeWiiRemote; break;
			case WPAD_EXP_NUNCHUK: {
				controller->type = SGFE_controllerTypeNunchuk;
				controller->axes[SGFE_axisLeftX].deadzone = 0.2f;
				controller->axes[SGFE_axisLeftY].deadzone = 0.2f;
			} break;
			default: controller->type = SGFE_controllerTypeUnknown;
		}
	}
	else {
		SGFE_MEMSET(controller->enabled_motions, 0, sizeof(controller->enabled_motions));
		SGFE_MEMSET(controller->enabled_pointers, 0, sizeof(controller->enabled_pointers));
	}
}

void SGFE__wiiControllerAxis(SGFE_window* win, SGFE_controller* controller, SGFE_axisType type, WPADData* data) {
	float pos;
	float min, max;

	switch (controller->type) {
		case SGFE_controllerTypeNunchuk: {
			struct joystick_t* js = &data->exp.nunchuk.js;
			if (type == SGFE_axisLeftX) {
				pos = js->pos.x;
				max = js->max.x;
				min = js->min.x;
			}
			else {
				pos = js->pos.y;
				max = js->max.y;
				min = js->min.y;
			}
		} break;
	}

	/* NOTE(EimaMei): Normalizing the axis to be from -1 to 1. */
	SGFE_axis* axis = &controller->axes[type];
	axis->value = ((pos - min) / (max - min)) * 2.0f - 1.0f;

	SGFE_windowAxisCallback(win, controller, type);
	if (win->queue_events) {
		SGFE_event event;
		event.type = SGFE_eventAxis;
		event.axis.controller = controller;
		event.axis.which = type;
		event.axis.value = axis->value;
		event.axis.deadzone = axis->deadzone;
		SGFE_windowEventPush(win, &event);
	}
}

void SGFE__wiiControllerSensor(SGFE_window* win, SGFE_controller* controller, WPADData* data) {
	/* NOTE(EimaMei): For some reason the authors of wiiuse decided not to include these in the header. Why? */
	enum {
		IR_STATE_DEAD = 0,
		IR_STATE_GOOD,
		IR_STATE_SINGLE,
		IR_STATE_LOST,
	};
	ir_t* ir = &data->ir;
	if (!controller->enabled_pointers[SGFE_pointerInfrared] || ir->state == IR_STATE_LOST || ir->state == IR_STATE_DEAD) {
		return ;
	}
	controller->pointers[SGFE_pointerInfrared][0] = (i32)ir->ax;
	controller->pointers[SGFE_pointerInfrared][1] = (i32)ir->ay;
	SGFE_windowPointerCallback(win, controller, SGFE_pointerInfrared);

	if (win->queue_events) {
		SGFE_event event;
		event.type = SGFE_eventPointer;
		event.pointer.controller = controller;
		event.pointer.which = SGFE_pointerInfrared;
		event.pointer.x = controller->pointers[SGFE_pointerInfrared][0];
		event.pointer.y = controller->pointers[SGFE_pointerInfrared][1];
		SGFE_windowEventPush(win, &event);
	}
}

void SGFE__wiiControllerMotion_wiimote(SGFE_window* win, SGFE_controller* controller) {
	if (!controller->enabled_motions[SGFE_motionAccelerometer]) {
		return;
	}

	struct wiimote_t* wiimote = win->src.wiimotes[controller->port];
	float* motion = (float*)&controller->motions[SGFE_motionAccelerometer];
	motion[0] = (float)(wiimote->accel.x - wiimote->accel_calib.cal_zero.x) / wiimote->accel_calib.cal_g.x * SGFE_STANDARD_GRAVITY;
	motion[1] = (float)(wiimote->accel.z - wiimote->accel_calib.cal_zero.z) / wiimote->accel_calib.cal_g.z * SGFE_STANDARD_GRAVITY;
	motion[2] = (float)(wiimote->accel.y - wiimote->accel_calib.cal_zero.y) / wiimote->accel_calib.cal_g.y * SGFE_STANDARD_GRAVITY;

	SGFE_windowMotionCallback(win, controller, SGFE_motionAccelerometer);
	if (win->queue_events) {
		SGFE_event event;
		event.type = SGFE_eventMotion;
		event.motion.controller = controller;
		event.motion.which = SGFE_motionAccelerometer;
		event.motion.x = motion[0];
		event.motion.y = motion[1];
		event.motion.z = motion[2];
		SGFE_windowEventPush(win, &event);
	}
}

void SGFE__wiiControllerMotion_nunchuk(SGFE_window* win, SGFE_controller* controller) {
	if (!controller->enabled_motions[SGFE_motionNunchukAccelerometer]) {
		return;
	}

	const nunchuk_t* nunchuk = &win->src.wiimotes[controller->port]->exp.nunchuk;
	float* motion = (float*)&controller->motions[SGFE_motionNunchukAccelerometer];
	motion[0] = (float)(nunchuk->accel.x - nunchuk->accel_calib.cal_zero.x) / nunchuk->accel_calib.cal_g.x * 9.80665f;
	motion[1] = (float)(nunchuk->accel.z - nunchuk->accel_calib.cal_zero.z) / nunchuk->accel_calib.cal_g.z * 9.80665f;
	motion[2] = (float)(nunchuk->accel.y - nunchuk->accel_calib.cal_zero.y) / nunchuk->accel_calib.cal_g.y * 9.80665f;

	SGFE_windowMotionCallback(win, controller, SGFE_motionNunchukAccelerometer);
	if (win->queue_events) {
		SGFE_event event;
		event.type = SGFE_eventMotion;
		event.motion.controller = controller;
		event.motion.which = SGFE_motionNunchukAccelerometer;
		event.motion.x = motion[0];
		event.motion.y = motion[1];
		event.motion.z = motion[2];
		SGFE_windowEventPush(win, &event);
	}
}

void SGFE__wiiCheckGXMode(SGFE_contextBuffer* b) {
	if (b->gx_video_mode != NULL) { return; }

	switch (b->mode) {
		case SGFE_videoModeNTSC_240i: b->gx_video_mode = &TVNtsc240Int;   break;
		case SGFE_videoModeNTSC_480i: b->gx_video_mode = &TVNtsc480IntDf; break;
		case SGFE_videoModeNTSC_480p: b->gx_video_mode = &TVNtsc480Prog;  break;

		case SGFE_videoModePAL_264i: b->gx_video_mode = &TVPal264Int;        break;
		case SGFE_videoModePAL_576i: b->gx_video_mode = &TVPal576IntDfScale; break;
		case SGFE_videoModePAL_576p: b->gx_video_mode = &TVPal576ProgScale;  break;

		case SGFE_videoModeMPAL_240i: b->gx_video_mode = &TVMpal240Int;    break;
		case SGFE_videoModeMPAL_480i: b->gx_video_mode = &TVMpal480IntDf;  break;
		case SGFE_videoModeMPAL_480p: b->gx_video_mode = &TVMpal480Prog;   break;

		case SGFE_videoModePAL60hz_240i: b->gx_video_mode = &TVEurgb60Hz240Int;   break;
		case SGFE_videoModePAL60hz_480i: b->gx_video_mode = &TVEurgb60Hz480IntDf; break;
		case SGFE_videoModePAL60hz_480p: b->gx_video_mode = &TVEurgb60Hz480Prog;  break;
	}
}




const SGFE_button SGFE_BUTTON_MASK_BITS_LUT[SGFE_controllerTypeCount] = {
	SGFE_buttonMask_WiiRemote,
	SGFE_buttonMask_Nunchuk,
	0
};

const isize SGFE_BUTTON_COUNT_LUT[SGFE_controllerTypeCount] = {
	SGFE_buttonCount_WiiRemote,
	SGFE_buttonCount_Nunchuk,
	0
};
const isize SGFE_AXIS_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{0, -1},
	{SGFE_axisLeftX, SGFE_axisLeftY},
	{0, -1}
};
const isize SGFE_POINTER_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{SGFE_pointerInfrared, SGFE_pointerInfrared},
	{SGFE_pointerInfrared, SGFE_pointerInfrared},
	{0, -1}
};
const isize SGFE_MOTION_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	{SGFE_motionAccelerometer, SGFE_motionAccelerometer},
	{SGFE_motionAccelerometer, SGFE_motionNunchukAccelerometer},
	{0, -1}
};

const char* SGFE_CONTROLLER_NAME_LUT[SGFE_controllerTypeCount] = {
	"Wii Remote", "Nunchuk", "Unknown"
};
const char* SGFE_BUTTON_NAMES_NUNCHUK_LUT[SGFE_buttonCount_Nunchuk] = {
	"2", "1", "B", "A", "Minus", "Home", "Left", "Right", "Down", "Up", "Plus",
	"Z", "C"
};
const char* SGFE_AXIS_NAME_LUT[SGFE_axisTypeCount] = {
	"Nunchuk X", "Nunchuk Y"
};
const char* SGFE_POINTER_NAME_LUT[SGFE_pointerTypeCount] = {
	"IR Sensor"
};
const char* SGFE_MOTION_NAME_LUT[SGFE_motionTypeCount] = {
	"Accelerometer", "Gyroscope", "Nunchuk accelerometer"
};

const isize SGFE_VIDEO_RESOLUTION_LUT[SGFE_videoModeCount][2] = {
	{320, 240}, {720, 480}, {720, 480},
	{320, 264}, {720, 528}, {720, 576},
	{320, 240}, {720, 480}, {720, 480},
	{320, 240}, {720, 480}, {720, 480},
};

const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_pixelFormatCount] = {
	4, 2
};

const char* SGFE_VIDEO_MODE_NAME_LUT[SGFE_videoModeCount] = {
	"SGFE_videoModeNTSC_240i",
	"SGFE_videoModeNTSC_480i",
	"SGFE_videoModeNTSC_480p",

	"SGFE_videoModePAL_264i",
	"SGFE_videoModePAL_528p",
	"SGFE_videoModePAL_576i",

	"SGFE_videoModeMPAL_240i",
	"SGFE_videoModeMPAL_480i",
	"SGFE_videoModeMPAL_480p",

	"SGFE_videoModePAL60hz_240i",
	"SGFE_videoModePAL60hz_480i",
	"SGFE_videoModePAL60hz_480p",
};

const char* SGFE_PIXEL_FORMAT_NAME_LUT[SGFE_pixelFormatCount] = {
	"SGFE_pixelFormatRGBA8", "SGFE_pixelFormatYUV"
};




SGFE_bool SGFE_windowMake_platform(SGFE_window* win) {
	VIDEO_Init();

	PAD_Init();
	WPAD_Init();

	#ifndef SGFE_WII_NO_WAIT_FOR_CONNECTION
	SGFE_bool wpad_is_active = SGFE_FALSE;
	#endif

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		WPADData* data = WPAD_Data(i);
		isize connect_retries = 0;

		#ifndef SGFE_WII_NO_WAIT_FOR_CONNECTION
			if (!wpad_is_active) { data->err = 1984; }

			i32 status;
			do {
				status = WPAD_Probe(i, NULL);
				connect_retries += 1;
				usleep(1000);
			} while (connect_retries < 5000 && (data->err == 1984 || status == WPAD_ERR_NOT_READY));

			wpad_is_active = SGFE_TRUE;
			connect_retries = 0;
		#else
			isize status = WPAD_Probe(i, NULL);
		#endif

		SGFE_controller* controller = &win->controllers[i];
		if (status != 0) {
			controller->connected = SGFE_FALSE;
			continue;
		}

		SGFE__wiiControllerOnConnect(controller, i, SGFE_TRUE);
	}

	/* TODO(EimaMei): document why this works. */
	win->src.wiimotes = wiiuse_init(-1, NULL);

	SYS_SetResetCallback(SGFE__callbackWiiReset);
	SYS_SetPowerCallback(SGFE__callbackWiiPower);
	WPAD_SetPowerButtonCallback(SGFE__callbackWiiRemotePower);

	return SGFE_TRUE;
}

void SGFE_windowClose_platform(SGFE_window* win) {
	WPAD_Shutdown();

	if ((win->_flags & SGFE_WINDOW_ALLOC)) {
		SGFE_FREE(win);
	}

	if (SGFE__WII_EXIT_STATE != 0) {
		SYS_ResetSystem(SGFE__WII_EXIT_STATE, 0, 0);
	}
}


void SGFE_windowPollEvents(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	if (win->polled_events) {
		win->event_len = 0;
	}

	if (SGFE__WII_EXIT_STATE != 0) {
		SGFE_windowQuitCallback(win);
		SGFE_windowSetShouldClose(win, SGFE_TRUE);

		if (win->queue_events) {
			SGFE_event event;
			event.type = SGFE_eventQuit;
			SGFE_windowEventPush(win, &event);
		}
		return;
	}

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		SGFE_controller* controller = &win->controllers[i];
		i32 res = WPAD_ReadPending(i, NULL);

		if (res < WPAD_ERR_NONE) {
			if (controller->connected) {
				SGFE__wiiControllerOnConnect(controller, i, SGFE_FALSE);
				SGFE_windowControllerCallback(win, controller, SGFE_FALSE);

				if (win->queue_events) {
					SGFE_event event;
					event.type = SGFE_eventControllerDisconnected;
					event.controller.controller = controller;
					SGFE_windowEventPush(win, &event);
				}
			}
			continue;
		}

		WPADData* state = WPAD_Data(i);
		SGFE_controllerType old_controller_type = controller->type;

		if (!controller->connected) {
			SGFE__wiiControllerOnConnect(controller, i, SGFE_TRUE);
			SGFE_windowControllerCallback(win, controller, SGFE_TRUE);

			if (win->queue_events) {
				SGFE_event event;
				event.type = SGFE_eventControllerConnected;
				event.controller.controller = controller;
				SGFE_windowEventPush(win, &event);
			}
		}
		else if (controller->type != state->exp.type) {
			SGFE__wiiControllerOnConnect(controller, i, SGFE_TRUE);
		}

		switch (controller->type) {
			case SGFE_controllerTypeWiiRemote: {
				controller->buttons_up   = SGFE_buttonFromAPI_WiiRemote(state->btns_u & 0xFFFF);
				if (old_controller_type == SGFE_controllerTypeNunchuk) {
					controller->buttons_up |= (controller->buttons_held & (SGFE_C | SGFE_Z));
				}

				controller->buttons_held = SGFE_buttonFromAPI_WiiRemote(state->btns_h & 0xFFFF);
				controller->buttons_down = SGFE_buttonFromAPI_WiiRemote(state->btns_d & 0xFFFF);

				SGFE__wiiControllerSensor(win, controller, state);
				SGFE__wiiControllerMotion_wiimote(win, controller);
			} break;

			case SGFE_controllerTypeNunchuk: {
				controller->buttons_held = SGFE_buttonFromAPI_Nunchuk(state->btns_h);
				controller->buttons_down = SGFE_buttonFromAPI_Nunchuk(state->btns_d),
				controller->buttons_up   = SGFE_buttonFromAPI_Nunchuk(state->btns_u);

				nunchuk_t* nunchuk = &state->exp.nunchuk;
				if (nunchuk->js.pos.x != nunchuk->js.center.x) {
					SGFE__wiiControllerAxis(win, controller, SGFE_axisLeftX, state);
				}
				if (nunchuk->js.pos.y != nunchuk->js.center.y) {
					SGFE__wiiControllerAxis(win, controller, SGFE_axisLeftY, state);
				}

				SGFE__wiiControllerSensor(win, controller, state);
				SGFE__wiiControllerMotion_wiimote(win, controller);
				SGFE__wiiControllerMotion_nunchuk(win, controller);
			} break;

			default: {
				continue;
			}
		}

		if (controller->buttons_held != 0) {
			SGFE_windowButtonCallback(win, controller, controller->buttons_held, SGFE_TRUE);

			if (win->queue_events) {
				SGFE_event event;
				event.type = SGFE_eventButtonDown;
				event.button.controller = controller;
				event.button.buttons = controller->buttons_held;
				event.button.buttons_down = controller->buttons_down;
				event.button.raw_buttons = state->btns_h;
				event.button.raw_buttons_down = state->btns_d;
				SGFE_windowEventPush(win, &event);
			}
		}

		if (controller->buttons_up != 0) {
			SGFE_windowButtonCallback(win, controller, controller->buttons_up, SGFE_FALSE);

			if (win->queue_events) {
				SGFE_event event;
				event.type = SGFE_eventButtonUp;
				event.button.controller = controller;
				event.button.buttons = controller->buttons_up;
				event.button.buttons_down = 0;
				event.button.raw_buttons = state->btns_u;
				event.button.raw_buttons_down = 0;
				SGFE_windowEventPush(win, &event);
			}
		}
	}
}


void SGFE_windowSwapBuffers(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	switch (SGFE_contextGetType(win->current)) {
		case SGFE_contextTypeBuffer: {
			SGFE_contextBuffer* b = SGFE_contextGetBuffer(win->current);
			VIDEO_WaitVSync();
		} break;
	}
}

void SGFE_windowMakeCurrent(SGFE_window* win, SGFE_context* ctx) {
	win->current = ctx;
}


SGFE_bool SGFE_windowInitTerminalOutput(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	if (win->current == NULL) {
		SGFE_bool res = SGFE_windowCreateContextBuffer(win, SGFE_videoModeOptimal(), SGFE_pixelFormatYUV, SGFE_TRUE);

		if (res == SGFE_FALSE) {
			return SGFE_FALSE;
		}
	}

	SGFE_contextBuffer* b = SGFE_windowGetCurrentBuffer(win);
	SGFE_bufferSetNative(b, SGFE_TRUE);
	SGFE_bufferSetFormat(b, SGFE_pixelFormatYUV);
	SGFE_bufferSetDoubleBuffered(b, SGFE_FALSE);

	console_init(
		SGFE_bufferGetFramebuffer(b),
		0, 0, b->gx_video_mode->fbWidth, b->gx_video_mode->xfbHeight,
		b->gx_video_mode->fbWidth * VI_DISPLAY_PIX_SZ
	);

	return SGFE_TRUE;
}



SGFE_button SGFE_controllerGetButtonMaskFromAPI(const SGFE_controller* controller,
		u32 mask) {
	#error "damn"
	SGFE_ASSERT(controller != NULL);
	switch (controller->type) {
		case SGFE_controllerTypeWiiRemote: return SGFE_buttonFromAPI_WiiRemote(mask);
		case SGFE_controllerTypeNunchuk: return SGFE_buttonFromAPI_Nunchuk(mask);
	}
	return 0;
}


const char* SGFE_controllerGetNameButton(const SGFE_controller* controller,
		SGFE_buttonType button) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT(button >= 0 && button < SGFE_controllerGetButtonCount(controller));

	switch (controller->type) {
		case SGFE_controllerTypeWiiRemote:
		case SGFE_controllerTypeNunchuk:
			return SGFE_BUTTON_NAMES_NUNCHUK_LUT[button];
	}

	return "N/A";
}


void SGFE_controllerEnablePointer(SGFE_controller* controller, SGFE_pointerType pointer,
		SGFE_bool enable) {
	switch (pointer) {
		case SGFE_pointerInfrared: {
			if (enable) {
				enable = (WPAD_SetDataFormat(controller->port, WPAD_FMT_BTNS_ACC_IR)) == 0;
			}
			else {
				WPAD_SetDataFormat(
					controller->port,
					(controller->enabled_motions[SGFE_motionAccelerometer])
						? WPAD_FMT_BTNS_ACC
						: WPAD_FMT_BTNS
				);
			}
		} break;

		/*case SGFE_motionGyroscope: {
			if (enable) HIDUSER_EnableGyroscope();
			else        HIDUSER_DisableGyroscope();
		} break;*/
	}
}

void SGFE_controllerEnableMotion(SGFE_controller* controller, SGFE_motionType motion,
		SGFE_bool enable) {
	switch (motion) {
		case SGFE_motionAccelerometer: {
			if (enable) {
				if (!controller->enabled_pointers[SGFE_pointerInfrared]) {
					enable = (WPAD_SetDataFormat(controller->port, WPAD_FMT_BTNS_ACC)) == 0;
				}
			}
			else {
				WPAD_SetDataFormat(
					controller->port,
					(controller->enabled_pointers[SGFE_pointerInfrared])
						? WPAD_FMT_BTNS_ACC_IR
						: WPAD_FMT_BTNS
				);
			}
		} break;

		/*case SGFE_motionGyroscope: {
			if (enable) HIDUSER_EnableGyroscope();
			else        HIDUSER_DisableGyroscope();
		} break;*/
	}
}



void SGFE_bufferMakeWithDefaultSettings(SGFE_contextBuffer* out_buffer,
		SGFE_videoMode mode, SGFE_pixelFormat format) {
	SGFE_ASSERT(out_buffer != NULL);
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	SGFE_ASSERT(format >= 0 && format < SGFE_pixelFormatCount);

	SGFE_contextBuffer* b = out_buffer;
	b->screen = SGFE_screenPrimary;
	b->mode = mode;
	b->format = format;

	b->current = 0;
	SGFE_MEMSET(b->buffers, 0, sizeof(b->buffers));

	b->is_buffer_allocated = SGFE_FALSE;
	b->is_double_buffered = SGFE_FALSE;
	b->is_native = SGFE_FALSE;

	#ifndef SGFE_BUFFER_NO_CONVERSION
	SGFE_MEMSET(b->buffers_native, 0, sizeof(b->buffers_native));
	#endif

	b->gx_video_mode = NULL;
}


SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);

	if (b->buffers[0] == NULL) {
		SGFE_bool res = SGFE_bufferAllocFramebuffers(b);
		if (!res) {
			SGFE_sendDebugInfo(SGFE_debugTypeError, SGFE_errorOutOfMemory, SGFE_DEBUG_CTX(NULL, 0), "Ran out of memory when allocating framebuffers.");
			return SGFE_FALSE;
		}
	}

	SGFE__wiiCheckGXMode(b);
	VIDEO_Configure(b->gx_video_mode);

	u8* fb = SGFE_bufferGetFramebuffer(b);
	VIDEO_SetNextFramebuffer(fb);
	VIDEO_ClearFrameBuffer(b->gx_video_mode, fb, COLOR_BLACK);
	VIDEO_SetBlack(SGFE_FALSE);
	VIDEO_Flush();

	VIDEO_WaitVSync();
	if(b->gx_video_mode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

	return SGFE_TRUE;
}

void SGFE_bufferFreeContext(SGFE_contextBuffer* b) {
	SGFE_bufferFreeFramebuffers(b);
	b->buffers[0] = NULL;
	b->buffers[1] = NULL;
}


SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);

	if (b->buffers[0] != NULL) { SGFE_bufferFreeFramebuffers(b); }
	SGFE__wiiCheckGXMode(b);

	for (size_t i = 0; i < 2; i += 1) {
		b->buffers[i] = memalign(VIDEO_GetFrameBufferSize(b->gx_video_mode), 32);
		if (b->buffers[i] == NULL) { return SGFE_FALSE; }

		b->buffers[i] = MEM_K0_TO_K1(b->buffers[i]);
	}
	b->is_buffer_allocated = SGFE_TRUE;

	return SGFE_TRUE;
}

SGFE_bool SGFE_bufferFreeFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);

	free(b->buffers[0]);
	free(b->buffers[1]);

	return SGFE_TRUE;
}


void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height) {
	SGFE_ASSERT(b != NULL);
	SGFE_videoModeResolution(b->mode, out_width, out_height);
}



void SGFE_windowFreeContext(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);

	SGFE_context* ctx = SGFE_windowGetContext(win);
	switch (ctx->type) {
		case SGFE_contextTypeBuffer: {
			SGFE_bufferFreeContext(SGFE_contextGetBuffer(ctx));
		} break;

		#ifdef RGFW_OPENGL
		case SGFE_contextTypeOpenGL: {
			SGFE_bufferFreeContext(SGFE_contextGetOpenGL(ctx));
		} break;
		#endif
	}
}



SGFE_videoMode SGFE_videoModeOptimal(void) {
	i32 tvmode = CONF_GetVideo();

	if (CONF_GetProgressiveScan() > 0 && VIDEO_HaveComponentCable()) {
		switch (tvmode) {
			case CONF_VIDEO_NTSC:
				return SGFE_videoModeNTSC_480p;

			case CONF_VIDEO_PAL:
				return (CONF_GetEuRGB60() > 0)
					? SGFE_videoModePAL60hz_480p
					: SGFE_videoModePAL_576p;

			case CONF_VIDEO_MPAL:
				return SGFE_videoModeMPAL_480p;

			default:
				return SGFE_videoModeNTSC_480p;
		}
	}
	else {
		switch (tvmode) {
			case CONF_VIDEO_NTSC:
				return SGFE_videoModeNTSC_480i;

			case CONF_VIDEO_PAL:
				return (CONF_GetEuRGB60() > 0)
					? SGFE_videoModePAL60hz_480i
					: SGFE_videoModePAL_576i;

			case CONF_VIDEO_MPAL:
				return SGFE_videoModePAL60hz_480i;

			default:
				return SGFE_videoModeNTSC_480i;
		}
	}
}

SGFE_videoMode SGFE_pixelFormatOptimal(void) { return SGFE_pixelFormatYUV; }

#endif

#endif /* SGFE_IMPLEMENTATION */

#if defined(__cplusplus)
}
#endif

#if _MSC_VER
	#pragma warning( pop )
#endif


#endif /* SGFE_INCLUDE_SGFE_H */

/*
SGFE's LICENSE:
------------------------------------------------------------------------------
Copyright (C) 2025 EimaMei

This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------------------------------

RGFW's LICENSE:
------------------------------------------------------------------------------
zlib License

Copyright (C) 2022-2025 Riley Mabb (@ColleagueRiley)

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
  
1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required. 
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
------------------------------------------------------------------------------
*/
