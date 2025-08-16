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

	#define SGFE_ALLOC x  - choose the default allocation function (defaults to standard malloc)
	#define SGFE_FREE x  - choose the default deallocation function (defaults to standard free)

	#define SGFE_EXPORT - use when building SGFE
	#define SGFE_IMPORT - use when linking with SGFE (not as a single-header)

	#define SGFE_bool x - choose what type to use for bool, by default u32 is used
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


#ifndef SGFE_ASSERT_MSG
	#ifndef NDEBUG
	#define SGFE_ASSERT_MSG(condition, msg) SGFE_windowAssert(SGFE__ROOT_WIN, condition, #condition, __FILE__, __LINE__, msg)
	#else
	#define SGFE_ASSERT_MSG(condition, msg)
	#endif
#endif

#ifndef SGFE_ASSERT
	#define SGFE_ASSERT(condition) SGFE_ASSERT_MSG(condition, "")
#endif

#ifndef SGFE_ASSERT_FMT
	#define SGFE_NO_CUSTOM_ASSERT_FMT
	#ifndef NDEBUG
	#define SGFE_ASSERT_FMT(condition, fmt, ...) SGFE_windowAssertFmt(SGFE__ROOT_WIN, condition, #condition, __FILE__, __LINE__, fmt, __VA_ARGS__)
	#else
	#define SGFE_ASSERT_FMT(condition, fmt ...)
	#endif
#endif

#ifndef SGFE_ASSERT_NOT_NULL
	#define SGFE_ASSERT_NOT_NULL(ptr) SGFE_ASSERT_FMT(ptr != NULL, "Pointer cannot be NULL. " #ptr " = %p;", ptr)
#endif

#ifndef SGFE_ASSERT_NOT_NEG
	#define SGFE_ASSERT_NOT_NEG(x) SGFE_ASSERT_FMT(x >= 0, "Integer cannot be negative. " #x " = %zi;", x)
#endif

#ifndef SGFE_ASSERT_BOOL
	#define SGFE_ASSERT_BOOL(x) SGFE_ASSERT_FMT(x == SGFE_FALSE || x == SGFE_TRUE, "Boolean must equal to SGFE_FALSE or SGFE_TRUE. " #x " = %zi;", x)
#endif


#ifndef SGFE_MEMSET
	#include <string.h>
	#define SGFE_MEMSET(ptr, value, num) memset(ptr, value, num)
#endif

#ifndef SGFE_MEMCPY
	#include <string.h>
	#define SGFE_MEMCPY(dist, src, len) memcpy(dist, src, len)
#endif

#ifndef SGFE_STRLEN
	#include <string.h>
	#define SGFE_STRLEN(str) (isize)strlen(str)
#endif

#ifndef SGFE_COUNTOF
	#define SGFE_COUNTOF(array) (isize)(sizeof(array) / sizeof((array)[0]))
#endif

#ifndef SGFE_BIT
	#define SGFE_BIT(x) (1 << (x))
#endif

#ifndef SGFE_PRINTF
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
/**
 * A constant to represent standard gravity
 *
 * This constant is used for calculating the values for accelerometer sensors.
 */
#define SGFE_STANDARD_GRAVITY 9.80665f
#endif

#ifndef SGFE_PI
/**
 * Constant Pi (float)
 *
 * This constant is used for calculating the values for gyroscopes.
 */
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

	#define SGFE_HAS_OPENGL              1
	#define SGFE_HAS_MULTIPLE_SCREENS    1
	#define SGFE_VBLANK_RATE            60

	#ifdef SGFE_IMPLEMENTATION
	#include <3ds/types.h>
	typedef s8   i8;
	typedef s16 i16;
	typedef s32 i32;
	typedef s64 i64;
	#define SGFE_DEFINE_TYPE_STDINT
	#endif

#elif defined(GEKKO) || defined(SGFE_WII)
	#ifndef SGFE_WII
	#define SGFE_WII 1
	#endif

	#ifndef SGFE_MAX_CONTROLLERS
	#define SGFE_MAX_CONTROLLERS 4
	#endif

	#define SGFE_HAS_OPENGL              0
	#define SGFE_HAS_MULTIPLE_SCREENS    0
	#define SGFE_VBLANK_RATE            60

	#ifdef SGFE_IMPLEMENTATION
	#include <gctypes.h>
	typedef s8   i8;
	typedef s16 i16;
	typedef s32 i32;
	typedef s64 i64;
	#define SGFE_DEFINE_TYPE_STDINT
	#define SGFE__BACKEND_FREE_WINDOW_IN_CLOSE
	#endif

#else
	#error "Unknown platform. Currently supported targets: SGFE_3DS, SGFE_WII."

#endif

#endif /* SGFE_CUSTOM_BACKEND */

#ifndef SGFE_HAS_OPENGL
	#undef SGFE_OPENGL
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

/**
 * SGFE screen type
 *
 * Regular systems tend to have only one screen where all graphical output goes to.
 * However, some consoles (primarily Nintendo handhelds) contain multiple screens
 * that need to be taken care of. The SGFE screen type is used to abstract this
 * in a way that would make the API convenient for all consoles regardless of
 * screen count.
 *
 * For systems that have one screen, SGFE can be used without paying much attention
 * to structures or function that mention anything about screens. Only nuisance
 * to note is if you do use said functions, the 'screen' must be 'SGFE_screenPrimary'.
 *
 * For systems that have multiple screens, concern is only raised when creating
 * and using graphical contexts (events are screen-agnostic). When creating buffer/OpenGL
 * contexts any valid screen can be specified to be used in their respective structures
 * before calling 'createContext'. Other notable use case is changing the current
 * context which can be done by calling 'SGFE_windowSetContextEx()'. Because OpenGL
 * only has _one_ current context, setting the context per screen is required for
 * rendering multiple screens at once.
 *
 *
 * \sa SGFE_windowGetContextTypeEx
 * \sa SGFE_windowGetContextEx
 * \sa SGFE_windowSetContextEx */
typedef SGFE_ENUM(isize, SGFE_screen) {
	/* The primary screen for the system. This value is always a constant regardless
	 * of the window state or currently set context. */
	SGFE_screenPrimary,

	#ifdef SGFE_3DS
	/* 3DS top screen. */
	SGFE_screenTop = SGFE_screenPrimary,
	/* 3DS bottom screen. */
	SGFE_screenBottom,
	#endif

	/* Amount of available screens for this backend. */
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
	/* Sets the video resolution to 800x240 with steroescopy disabled.
	 * NOTE: You cannot use wide mode for 2DS consoles (minus XL). Attempt at it
	 * will result in an assertion error. */
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

	/* Amount of video modes for this backend. */
	SGFE_videoModeCount
};

/* TODO(EimaMei): document */
/* TODO(EimaMei): document how if native mode is turned on SGFE_bufferFormatRGBA8 might return false. */
typedef SGFE_ENUM(isize, SGFE_bufferFormat) {
	/* Each channel bit is set to 8-bits. This option is always supported for
	 * non-native buffers, but might not be supported for native ones. By default
	 * 'SGFE_window_initBuffer' picks this as the pixel format. */
	SGFE_bufferFormatRGBA8,

	#ifdef SGFE_3DS
	/* BGR format where all channels are 8-bit. */
	SGFE_bufferFormatBGR8,
	/* RGB format where red and blue channels are 5-bit, green color channel - 6-bit. */
	SGFE_bufferFormatRGB565,
	/* RGBA format where the RGB channels are 5-bit, alpha channel - 1-bit. */
	SGFE_bufferFormatRGB5_A1,
	/* RGBA format where all channels are 4-bit. */
	SGFE_bufferFormatRGBA4,

	#elif SGFE_WII
	/* TODO */
	SGFE_bufferFormatYUV,

	#endif

	/* Amount of pixel formats for this backend. */
	SGFE_bufferFormatCount,
};


/**
 * SGFE controller type
 *
 * All consoles have at least one standard controller to communicate input for
 * obvious reasons. However many consoles also have multiple controller types that
 * differ from another dramatically.
 *
 * Every SGFE backend has the standard controller type defined under 'SGFE_controllerTypeStandard'
 * which equals to zero. After that a backend can have any amount of additional
 * types.
 *
 * You can access the controller's type by reading the 'SGFE_controller' structure's
 * member with the same name.
 *
 * \sa SGFE_controller */
typedef SGFE_ENUM(isize, SGFE_controllerType) {
	/* Standard controller type. The most common/default controller type. */
	SGFE_controllerTypeStandard = 0,

	#ifdef SGFE_WII
	/* Wii Remote (without any extensions). */
	SGFE_controllerTypeWiiRemote = SGFE_controllerTypeStandard,
	/* Wii Remote + Nunchuk. */
	SGFE_controllerTypeNunchuk,
	/* Either a Wii Remote with an unsupported extension, a GameCube controller or
	 * Wii (U) Classic controller. */
	SGFE_controllerTypeUnknown,
	#endif

	/* Amount of controller types for this backend. */
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
	SGFE_axisType type;
	float value;
	float deadzone;
} SGFE_axis;

typedef struct SGFE_pointer {
	SGFE_pointerType type;
	isize x, y;
} SGFE_pointer;

typedef struct SGFE_motion {
	SGFE_motionType type;
	float x, y, z;
} SGFE_motion;

/* TODO */
typedef SGFE_ENUM(isize, SGFE_powerState) {
	SGFE_powerStateUnknown,
	SGFE_powerStateNoBattery,
	SGFE_powerStateOnBattery,
	SGFE_powerStateCharging,
	SGFE_powerStateFullyCharged,
};


/* TODO(EimaMei): new structure. */
typedef struct SGFE_controller {
	/* Denotes what type of controller it is. */
	SGFE_controllerType type;
	/* TODO */
	isize array_index;

	/* TODO */
	SGFE_button buttons_held, buttons_down, buttons_up;

	/* Current axes states. */
	SGFE_axis axes[SGFE_axisTypeCount];
	/* Current pointer states. */
	SGFE_pointer pointers[SGFE_pointerTypeCount];
	/* Current motion states. */
	SGFE_motion motions[SGFE_motionTypeCount];

	/* Boolean states of enabled pointers. */
	SGFE_bool enabled_pointers[SGFE_pointerTypeCount];
	/* Boolean states of enabled motions. */
	SGFE_bool enabled_motions[SGFE_motionTypeCount];

	/* TODO */
	SGFE_powerState power_state;
	/* TODO */
	isize battery_procent;

	/* TODO */
	struct SGFE_controller* prev, *next;
} SGFE_controller;

/* TODO */
typedef struct SGFE_controllerList {
	isize count;
	SGFE_controller* first;
	SGFE_controller* last;
} SGFE_controllerList;



/* TODO */
typedef SGFE_ENUM(isize, SGFE_contextType) {
	SGFE_contextTypeNone,
	SGFE_contextTypeBuffer,
	SGFE_contextTypeGL,

	SGFE_contextTypeCount,
};

typedef SGFE_ENUM(isize, SGFE_eventType) {
	/* No event. */
	SGFE_eventNone = 0,


	/* Application is being shut down. */
	SGFE_eventQuit,
	SGFE_eventDeviceSleep, /* The device has entered sleep mode. */
	SGFE_eventDeviceWakeup, /* The device has exited out of sleep mode. */

	SGFE_eventFocusOut, /* User has exited the window and entered a new environment (eg. "Home Menu".) */
	SGFE_eventFocusIn, /* 	User has entered the window from an external environment (usually the "Home Menu"). */


	/* TODO */
	SGFE_eventControllerConnected,
	/* TODO */
	SGFE_eventControllerDisconnected,
	/* TODO */
	SGFE_eventControllerBattery,

	/* TODO */
	SGFE_eventButtonDown,
	/* TODO */
	SGFE_eventButtonUp,

	/* TODO */
	SGFE_eventAxis,
	/* TODO */
	SGFE_eventPointer,
	/* TODO */
	SGFE_eventMotion, /* The motion sensors of the controller have been moved. */


	/* TODO */
	SGFE_eventKeyboardConnected,
	/* TODO */
	SGFE_eventKeyboardDisconnected,

	/* TODO */
	SGFE_eventKeyboardDown,
	/* TODO */
	SGFE_eventKeyboardUp,

	/* TODO */
	SGFE_eventTextInput,


	/* TODO */
	SGFE_eventMouseConnected,
	/* TODO */
	SGFE_eventMouseDisconnected,

	/* TODO */
	SGFE_eventMouseMove,
	/* TODO */
	SGFE_eventMouseButtonDown,
	/* TODO */
	SGFE_eventMouseButtonUp,
	/* TODO */
	SGFE_eventMouseWheel,


	/* TODO */
	SGFE_eventCount
};


#ifndef SGFE_MAX_EVENTS
#define SGFE_MAX_EVENTS 32
#endif

typedef struct SGFE_event_common {
	SGFE_eventType type;
	u64 timestamp;
} SGFE_event_common;

typedef struct SGFE_event_controller {
	SGFE_eventType type;
	u64 timestamp;
	SGFE_controller* controller;
} SGFE_event_controller;

typedef struct SGFE_event_battery {
	SGFE_eventType type;
	u64 timestamp;
	SGFE_controller* controller;
	SGFE_powerState state;
	isize battery_procent;
} SGFE_event_battery;

typedef struct SGFE_event_button {
	SGFE_eventType type;
	u64 timestamp;
	SGFE_controller* controller;

	SGFE_buttonType button;
	SGFE_bool repeat;
	u32 raw_button;
} SGFE_event_button;

typedef struct SGFE_event_axis {
	SGFE_eventType type;
	u64 timestamp;
	SGFE_controller* controller;

	SGFE_axisType which;
	float value;
	float deadzone;
} SGFE_event_axis;

typedef struct SGFE_event_pointer {
	SGFE_eventType type;
	u64 timestamp;
	SGFE_controller* controller;

	SGFE_pointerType which;
	i32 x, y;
} SGFE_event_pointer;

typedef struct SGFE_event_motion {
	SGFE_eventType type;
	u64 timestamp;
	SGFE_controller* controller;

	SGFE_motionType which;
	float x, y, z;
} SGFE_event_motion;

typedef struct SGFE_event_text {
	SGFE_eventType type;
	u64 timestamp;

	u8* text;
	isize text_len;
} SGFE_event_text;

typedef struct SGFE_event_user {
	SGFE_eventType type;
	u64 timestamp;

	isize code;
	void* data1;
	void* data2;
} SGFE_event_user;

typedef union SGFE_event {
	SGFE_eventType type;
	SGFE_event_common common;
	SGFE_event_controller controller;
	SGFE_event_battery battery;
	SGFE_event_button button;
	SGFE_event_axis axis;
	SGFE_event_pointer pointer;
	SGFE_event_motion motion;
	SGFE_event_text text;
	SGFE_event_user user;
} SGFE_event;



/* TODO */
typedef struct SGFE_window SGFE_window;
/* TODO */
typedef struct SGFE_windowSource SGFE_windowSource;

/*! Optional arguments for making a windows */
typedef SGFE_ENUM(u32, SGFE_windowFlag) {
	/* No window flags. */
	SGFE_windowFlagNone         = 0,

	/* Disables the default behavior of enabling the screen. */
	SGFE_windowFlagDontShow     = SGFE_BIT(14),
	/* Turns the entire screen into a terminal output. */
	SGFE_windowFlagTerminal     = SGFE_BIT(15),
	/* Creates a buffer context alongside the window. If failed, all 'windowMake'
	 * functions return NULL. */
	SGFE_windowFlagBuffer       = SGFE_BIT(16),
	/* Creates an OpenGL context alongside the window. If failed, all 'windowMake'
	 * functions return NULL. */
	SGFE_windowFlagOpenGL       = SGFE_BIT(17),
	/* Creates an OpenGL context alongside the window. If failed, all 'windowMake'
	 * functions return NULL. [This option currently does nothing]. */
	SGFE_windowFlagEGL          = SGFE_BIT(18),

	#ifdef SGFE_3DS
	/* Enables rendering for the top 3DS screen. If none of the screen flags are
	 * pickes, this flag gets implicitly set during the creation of the window. */
	SGFE_windowFlagTopScreen    = SGFE_BIT(29),
	/* Enables rendering for the bottom 3DS screen. This also sets the bottom screen
	 * as the 'primary' screen for functions that input the window structure. */
	SGFE_windowFlagBottomScreen = SGFE_BIT(30),
	/* Enables rendering for both 3DS screen. The primary screen is the top screen. */
	SGFE_windowFlagDualScreen   = SGFE_windowFlagTopScreen | SGFE_windowFlagBottomScreen,
	#endif
};


typedef struct SGFE_windowState {
	/* TODO */
	SGFE_bool is_visible;

	/* TODO */
	SGFE_bool should_quit;
	/* TODO */
	SGFE_bool is_sleeping;
	/* TODO */
	SGFE_bool is_focused;
	/* TODO */
	SGFE_bool has_text_input;


	/* TODO */
	SGFE_controllerList controllers;

	/* TOOD */
	isize text_len;
	/* TODO */
	u8* text;
} SGFE_windowState;

#if defined(SGFE_IMPLEMENTATION) || !defined(SGFE_NO_WINDOW_SRC)

#ifdef SGFE_3DS

#include <3ds.h>
#include <malloc.h>

#ifdef SGFE_OPENGL
#include <GLES/gl2.h>
#endif


struct SGFE_windowSource {
	aptHookCookie apt_hook;

	SwkbdState* keyboard;
	char* swkbd_shared_mem;
	Handle swkbd_shared_mem_handle;
	SGFE_bool kb_null_terminated;
};

struct SGFE_contextBufferSource {
	u32 size;

	/* NOTE(EimaMei): for FPS. */
	isize frames_counter;
	SGFE_bool* run_gsp_loop;
};

#ifdef SGFE_OPENGL
struct SGFE_contextGL {
	GLASSCtx ctx;
	GLuint framebuffer;
	GLuint renderbuffer;
};
#endif

#elif SGFE_WII

#ifndef HW_RVL
#define HW_RVL 1
#endif
#include <gccore.h>
#include <wiiuse/wpad.h>


struct SGFE_windowSource {
	struct wiimote_t** wiimotes;
};

struct SGFE_contextBufferSource {
	GXRModeObj* gx_video_mode;
};

#endif

struct SGFE_contextBuffer {
	SGFE_screen screen;
	SGFE_videoMode mode;
	SGFE_bufferFormat format;

	isize current;
	u8* buffers[2];

	SGFE_bool is_buffer_allocated;
	SGFE_bool is_double_buffered;
	SGFE_bool is_native;

	isize frames;
	u8* buffers_native[2];
	struct SGFE_contextBufferSource src;
};

struct SGFE_window {
	/* TODO */
	SGFE_windowSource src;
	/* TODO */
	SGFE_windowState state;

	/* TODO */
	SGFE_contextType current_type[SGFE_screenCount];
	/* TODO */
	void* current[SGFE_screenCount];

	union {
		/* TODO */
		struct SGFE_contextBuffer buffer;
		#ifdef SGFE_OPENGL
		/* TODO */
		struct SGFE_contextGL gl;
		#endif
	} ctx[SGFE_screenCount];

	/* TODO */
	SGFE_controller controllers[SGFE_MAX_CONTROLLERS];
	/* TODO */
	SGFE_controllerList disconnected_controllers;

	/* TODO */
	SGFE_bool is_queueing_events, has_polled_events;
	/* TODO */
	isize event_len;
	/* TODO */
	SGFE_event events[SGFE_MAX_EVENTS];
	/* TODO */
	SGFE_bool enabled_events[SGFE_eventCount];

	/* TODO */
	SGFE_windowFlag flags;
	/* TODO */
	void* user_ptr;
	/* TODO */
	SGFE_button exit_buttons;

	/* TODO */
	SGFE_bool is_allocated;

	struct {
		void (*sleep)(void);

		void (*quit)(void);
		void (*focus)(void);

		void (*controller)(void);
		void (*controller_battery)(void);

		void (*button)(void);
		void (*axis)(void);
		void (*pointer)(void);
		void (*motion)(void);
		void (*debug)(void);

		void (*text_input)(void);
	} callbacks;
}; /*!< window structure for managing the window */

#endif /* defined(SGFE_IMPLEMENTATION) || !defined(SGFE_NO_WINDOW_SRC)) */

/* TODO */
SGFE_DEF isize SGFE_sizeofWindow(void);
/* TODO */
SGFE_DEF isize SGFE_sizeofWindowSrc(void);

SGFE_DEF void* SGFE_windowGetUserPtr(SGFE_window* win);
SGFE_DEF void SGFE_windowSetUserPtr(SGFE_window* win, void* ptr);
/* TODO */
SGFE_DEF SGFE_windowSource* SGFE_windowGetSource(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_screen SGFE_windowGetCurrentScreen(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_bool SGFE_windowIsScreenEnabled(SGFE_window* win, SGFE_screen screen);




/* TODO */
SGFE_DEF SGFE_window* SGFE_windowMake(SGFE_videoMode mode, SGFE_windowFlag flags);
SGFE_DEF SGFE_window* SGFE_windowMakePtr(
	SGFE_videoMode mode, /* TODO(EimaMei): document */
	SGFE_windowFlag flags, /* extra arguments (NULL / (u32)0 means no flags used) */
	SGFE_window* win /* ptr to the window struct you want to use */
); /*!< function to create a window (without allocating a window struct) */

/* TODO */
SGFE_DEF SGFE_window* SGFE_windowMakeContextless(SGFE_windowFlag flags);
/* TODO */
SGFE_DEF SGFE_window* SGFE_windowMakePtrContextless(SGFE_windowFlag flags, SGFE_window* win);


/*! window managment functions */
SGFE_DEF void SGFE_windowClose(SGFE_window* win); /*!< close the window and free leftover data */
/* TODO */
SGFE_DEF void SGFE_windowFreeContext(SGFE_window* win);


/* TODO */
SGFE_DEF SGFE_windowFlag SGFE_windowGetFlags(const SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowSetFlags(SGFE_window* win, SGFE_windowFlag flags);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowGetIsQueuingEvents(const SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowSetQueueEvents(SGFE_window* win, SGFE_bool is_queuing_events);

/* TODO */
SGFE_bool SGFE_windowGetEventEnabled(const SGFE_window* win, SGFE_eventType type);
/* TODO */
void SGFE_windowSetEventEnabled(SGFE_window* win, SGFE_eventType type, SGFE_bool is_enabled);
/* TODO */
void SGFE_windowSetEventEnabledDefault(SGFE_window* win);

/* TODO */
SGFE_DEF const SGFE_windowState* SGFE_windowPollEvents(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_bool SGFE_windowCheckEvent(SGFE_window* win, const SGFE_event** out_event);
/* TODO */
SGFE_DEF SGFE_bool SGFE_windowCheckQueuedEvent(SGFE_window* win, const SGFE_event** out_event);

/* TODO */
SGFE_DEF SGFE_bool SGFE_windowEventPush(SGFE_window* win, const SGFE_event* event);
/* TODO */
SGFE_DEF const SGFE_event* SGFE_windowEventPop(SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowEventQueueFlush(SGFE_window* win);


/* TODO */
SGFE_DEF void SGFE_windowSwapBuffers(SGFE_window* win);

/* TODO */
SGFE_DEF SGFE_contextType SGFE_windowGetContextType(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_contextType SGFE_windowGetContextTypeEx(SGFE_window* win, SGFE_screen screen);

/* TODO */
SGFE_DEF void* SGFE_windowGetContext(SGFE_window* win);
/* TODO */
SGFE_DEF void* SGFE_windowGetContextEx(SGFE_window* win, SGFE_screen screen);
/* TODO */
SGFE_DEF void SGFE_windowSetContext(SGFE_window* win, SGFE_contextType type, void* ctx);
/* TODO */
SGFE_DEF void SGFE_windowSetContextEx(SGFE_window* win, SGFE_contextType type, void* ctx, SGFE_screen screen);


/* Returns the controller strucutr associated with the port. */
SGFE_DEF SGFE_controller* SGFE_windowGetController(SGFE_window* win, isize port);
/* TODO */
SGFE_DEF SGFE_bool SGFE_windowIsControllerConnected(SGFE_window* win, const SGFE_controller* controller);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowShouldClose(SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowSetShouldClose(SGFE_window* win, SGFE_bool should_close);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowIsVisible(const SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowSetVisible(SGFE_window* win, SGFE_bool is_visible);


/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_bool SGFE_windowInitTerminalOutput(SGFE_window* win);
/* TODO */
SGFE_DEF void SGFE_windowAssert(SGFE_window* win, SGFE_bool is_asserted,
	const char* condition_str, const char* file, isize line, const char* message);
#ifdef SGFE_NO_CUSTOM_ASSERT_FMT
/* TODO */
SGFE_DEF void SGFE_windowAssertFmt(SGFE_window* win, SGFE_bool is_asserted,
	const char* condition_str, const char* file, isize line, const char* fmt, ...);
#endif



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
SGFE_DEF const SGFE_axis* SGFE_controllerGetAxis(const SGFE_controller* controller,
	SGFE_axisType which);
/* TODO(EimaMei): */
SGFE_DEF const SGFE_pointer* SGFE_controllerGetPointer(const SGFE_controller* controller,
	SGFE_pointerType which);
/* TODO(EimaMei): */
SGFE_DEF const SGFE_motion* SGFE_controllerGetMotion(const SGFE_controller* controller,
	SGFE_motionType which);

/* TODO(EimaMei): */
SGFE_DEF void SGFE_controllerGetRangeButton(SGFE_controllerType type, SGFE_buttonType* out_first, SGFE_buttonType* out_last);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF void SGFE_controllerGetRangeAxis(SGFE_controllerType type, SGFE_axisType* out_first, SGFE_axisType* out_last);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF void SGFE_controllerGetRangePointer(SGFE_controllerType type, SGFE_pointerType* out_first, SGFE_pointerType* out_last);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF void SGFE_controllerGetRangeMotion(SGFE_controllerType type, SGFE_motionType* out_first, SGFE_motionType* out_last);

/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetName(SGFE_controllerType type);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNameButton(SGFE_controllerType type,
	SGFE_buttonType button);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNameAxis(SGFE_controllerType type,
	SGFE_axisType axis);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNamePointer(SGFE_controllerType type,
	SGFE_pointerType pointer);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF const char* SGFE_controllerGetNameMotion(SGFE_controllerType type,
	SGFE_motionType motion);

/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF SGFE_bool SGFE_controllerEnablePointer(SGFE_controller* controller,
	SGFE_pointerType pointer, SGFE_bool enable);
/* TODO(EimaMei): NEW FUNCTION */
SGFE_DEF SGFE_bool SGFE_controllerEnableMotion(SGFE_controller* controller,
	SGFE_motionType motion, SGFE_bool enable);



/* TODO */
typedef struct SGFE_contextBuffer SGFE_contextBuffer;


/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_bufferFormat SGFE_bufferGetOptimalFormat(void);


/* */
SGFE_DEF SGFE_bool SGFE_bufferMakeWithDefaultSettings(SGFE_contextBuffer* out_buffer,
	SGFE_videoMode mode, SGFE_bufferFormat format, SGFE_bool allocate_buffers);
/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferSetPlatformSettings(SGFE_contextBuffer* b);


/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* out_buffer);
/* TODO */
SGFE_DEF void SGFE_bufferFreeContext(SGFE_contextBuffer* b);

/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* out_buffer);
/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferFreeFramebuffers(SGFE_contextBuffer* out_buffer);

/* TODO */
SGFE_DEF u8* SGFE_bufferConvertFramebufferToNative(SGFE_contextBuffer* b);


/* TODO */
SGFE_DEF isize SGFE_bufferGetSwapInterval(const SGFE_contextBuffer* b);
/* TODO */
SGFE_DEF isize SGFE_bufferGetFramesPerSecond(const SGFE_contextBuffer* b);

/* TODO */
SGFE_DEF void SGFE_bufferSetSwapInterval(SGFE_contextBuffer* b, isize swap_interval);
/* TODO */
SGFE_DEF void SGFE_bufferSetFramesPerSecond(SGFE_contextBuffer* b, isize fps);


/* TODO(EimaMei): New function. */
SGFE_DEF u8* SGFE_bufferGetFramebuffer(SGFE_contextBuffer* b);

/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_bufferFormat SGFE_bufferGetFormat(const SGFE_contextBuffer* b);
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
/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_videoMode SGFE_bufferGetVideoMode(const SGFE_contextBuffer* b);
/* TODO */
SGFE_DEF void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height);
/* TODO */
SGFE_DEF SGFE_bool SGFE_bufferIsStereoscopic(SGFE_contextBuffer* b);


/* TODO(EimaMei): New function. */
SGFE_DEF SGFE_bool SGFE_bufferSetFormat(SGFE_contextBuffer* b, SGFE_bufferFormat format);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_bufferSetNative(SGFE_contextBuffer* b, SGFE_bool is_native);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_bufferSetDoubleBuffered(SGFE_contextBuffer* b, SGFE_bool enable);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_bufferFlipFramebuffers(SGFE_contextBuffer* b);


/* TODO(EimaMei): New function. */
SGFE_DEF isize SGFE_bufferFormatGetBytesPerPixel(SGFE_bufferFormat format);
/* TODO(EimaMei): New function. */
SGFE_DEF const char* SGFE_bufferFormatGetStr(SGFE_bufferFormat format);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowCreateContextBuffer(SGFE_window* win, SGFE_videoMode mode,
	SGFE_bufferFormat format, SGFE_bool is_native);

/* TODO */
SGFE_DEF void SGFE_windowSwapBuffersBuffer(SGFE_window* win);

/* TODO */
SGFE_contextBuffer* SGFE_windowGetContextBuffer(SGFE_window* win);
/* TODO */
SGFE_contextBuffer* SGFE_windowGetContextExBuffer(SGFE_window* win, SGFE_screen screen);
/* TODO */
SGFE_DEF void SGFE_windowSetContextBuffer(SGFE_window* win, SGFE_contextBuffer* ctx);
/* TODO */
SGFE_DEF void SGFE_windowSetContextExBuffer(SGFE_window* win, SGFE_contextBuffer* ctx, SGFE_screen screen);



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
 * OpenGL Context type
 *
 * The structure is used as an opaque data type to an OpenGL context.
 *
 * \sa SGFE_glCreateContext
 */
typedef struct SGFE_contextGL SGFE_contextGL;

/* TODO */
typedef SGFE_ENUM(isize, SGFE_glProfile) {
 	/* OpenGL Core (OpenGL 3.2-4.6). */
	SGFE_glProfileCore = 0,
	/* OpenGL Compatibility (OpenGL 1.0-3.1). */
	SGFE_glProfileCompatibility,
	/* OpenGL ES. */
	SGFE_glProfileES,

	SGFE_glProfileCount
};

/**
 * OpenGL context initializiation values
 *
 * OpenGL hints are used to set desired parameters for when creating a new OpenGL
 * context. These are treated as 'hints' since the OpenGL driver is only obligated
 * to fufill the "minimum" requirements for some parameters (e.g., specifying a
 * 16-bit color buffer could yield a 24 or even 32 bit one). Other parameters may
 * require specific values to function (e.g., SGFE_glProfile).
 *
 * Settings hints must be done before calling SGFE_glCreateContext() or
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
 * To specify anti-aliasing you can pick either SSAA (by modifying the 'downscale'
 * hints) or MSAA (by modifying the 'samples' hint). It's important to note that
 * you musn't attempt using both methods as that is unintended behaviour.
 */
typedef struct SGFE_contextHintsGL  {
	/* Type of OpenGL API context (SGFE_glProfile_Core by default). */
	SGFE_glProfile profile;
	/* OpenGL context major version (1 by default). */
	isize major;
	/* OpenGL context minor version (0 by default). */
	isize minor;

	/* Minimum number of bits for the red channel of the color buffer (8 by default). */
	isize red;
	/* Minimum number of bits for the green channel of the color buffer (8 by default). */
	isize green;
	/* Minimum number of bits for the blue channel of the color buffer (8 by default). */
	isize blue;
	/* Minimum number of bits for the alpha channel of the color buffer (8 by default). */
	isize alpha;

	/* Minimum number of bits for the depth buffer (24 by default). */
	isize depth;
	/* Minimum number of bits in the stencil buffer (0 by default). */
	isize stencil;

	/* Minimum number of bits for the red channel of the accumulation buffer (0 by default). */
	isize accum_red;
	/* Minimum number of bits for the green channel of the accumulation buffer (0 by default). */
	isize accum_green;
	/* Minimum number of bits for the blue channel of the accumulation buffer (0 by default). */
	isize accum_blue;
	/* Minimum number of bits for the alpha channel of the accumulation buffer (0 by default). */
	isize accum_alpha;

	/* Number of samples used for MSAA (0 by default).
	 * NOTE: Refer to the last paragraph of the struct's description. */
	isize samples;
	/* Number of times to downscale the framebuffer's width for SSAA (0 by default).
	 * NOTE: Refer to the last paragraph of the struct's description. */
	isize downscale_width;
	/* Number of times to downscale the framebuffer's height for SSAA (0 by default).
	 * NOTE: Refer to the last paragraph of the struct's description. */
	isize downscale_height;

	/* Number of auxiliary buffers. (0 by default). Deprecated OpenGL feature, do not use for new code. */
	isize aux_buffers;

	/* Boolean if double buffering is used (true by defult). */
	SGFE_bool is_double_buffered;
	/* Boolean if VSync is enabled (true by default). */
	SGFE_bool is_vsync;
	/* Boolean if the renderering is done on the CPU (false by default). */
	SGFE_bool is_software_renderer;
	/* Boolean if the context is stereosopic 3D (false by default). */
	SGFE_bool is_stereo;
	/* Boolean if sRGB framebuffer is used (false by default). */
	SGFE_bool is_SRGB;
	/* Boolean if the context is robust (memory-safe) (false by default). For more information: https://registry.khronos.org/OpenGL/extensions/EXT/EXT_robustness.txt. */
	SGFE_bool is_robust;
	/* Boolean if the context has additional debugging features (false by default). */
	SGFE_bool is_debug;
	/* Boolean if OpenGL errors have undefined behavior (false by default). Fore more information: https://registry.khronos.org/OpenGL/extensions/KHR/KHR_no_error.txt. */
	SGFE_bool has_no_errors;
	/* Boolean if the OpenGL driver should flush the graphics when changing contexts (false by default). */
	SGFE_bool flush_on_ctx_change;

	/* Context that is shared with the newly created context (NULL by default). */
	SGFE_contextGL* shared_context;

	/* Screen that the context is being used for (SGFE_screenPrimary by default). */
	SGFE_screen screen;
} SGFE_contextHintsGL;



/**
 * Returns the currently bound OpenGL context.
 *
 * Note that this function returns the OpenGL context that's bound to the thread,
 * not to the window.
 *
 * \returns The currently bound OpenGL context.
 */
SGFE_DEF void* SGFE_glGetBoundContext(void);


/* TODO | enum | can return -1 */
SGFE_DEF isize SGFE_glGetVertexShaderType(void);
/* TODO | can return -1 */
SGFE_DEF isize SGFE_glGetFragmentShaderType(void);
/* TODO | can return -1 */
SGFE_DEF isize SGFE_glGetGeometryShaderType(void);


/* TODO */
SGFE_DEF void SGFE_glHintsMakeWithDefaultSettings(SGFE_contextHintsGL* hints);
/* TODO */
SGFE_DEF void SGFE_glHintsAssert(const SGFE_contextHintsGL* gl);

/* TODO */
SGFE_DEF SGFE_contextHintsGL* SGFE_glHintsGetGlobal(void);
/* TODO */
SGFE_DEF void SGFE_glHintsSetGlobal(const SGFE_contextHintsGL* hints);


/* TODO */
SGFE_DEF SGFE_bool SGFE_glCreateContext(SGFE_contextGL* gl, SGFE_videoMode mode, SGFE_contextHintsGL* hints);
/* TODO */
SGFE_DEF void SGFE_glFreeContext(SGFE_contextGL* gl);

/* TODO */
SGFE_DEF isize SGFE_glGetSwapInterval(const SGFE_contextGL* gl);
/* TODO */
SGFE_DEF void SGFE_glSetSwapInterval(SGFE_contextGL* gl, isize swap_interval);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowCreateContextGL(SGFE_window* win, SGFE_videoMode mode);

/* TODO */
SGFE_DEF void SGFE_windowSwapBuffersGL(SGFE_window* win);

/* TODO */
SGFE_DEF SGFE_contextGL* SGFE_windowGetContextGL(SGFE_window* win);
/* TODO */
SGFE_DEF SGFE_contextGL* SGFE_windowGetContextExGL(SGFE_window* win, SGFE_screen screen);
/* TODO */
SGFE_DEF void SGFE_windowSetContextGL(SGFE_window* win, SGFE_contextGL* ctx);
/* TODO */
SGFE_DEF void SGFE_windowSetContextExGL(SGFE_window* win, SGFE_contextGL* ctx, SGFE_screen screen);

#endif



typedef SGFE_ENUM(isize, SGFE_textInputType) {
	/* TODO */
	SGFE_textInputTypeStandard,
	/* TODO */
	SGFE_textInputTypeQWERTY,
	/* TODO */
	SGFE_textInputTypeNumpad,
	/* TODO */
	SGFE_textInputTypeWestern,
};

typedef SGFE_ENUM(isize, SGFE_textInputPassword) {
	/* TODO */
	SGFE_textInputPasswordNone,
	/* TODO */
	SGFE_textInputPasswordHide,
	/* TODO */
	SGFE_textInputPasswordHideDelay,
};

typedef SGFE_ENUM(u32, SGFE_textInputFlag) {
	/* TODO */
	SGFE_textInputFlagMultiline                = SGFE_BIT(0),

	/* TODO */
	SGFE_textInputFlagFixedLength              = SGFE_BIT(1),
	/* TODO */
	SGFE_textInputFlagNotEmpty                 = SGFE_BIT(2),
	/* TODO */
	SGFE_textInputFlagNotBlank                 = SGFE_BIT(3),
};

#ifndef SGFE_CUSTOM_BACKEND
typedef SGFE_ENUM(u32, SGFE_textInputPlatformFlag) {
	SGFE_textInputPlatformFlagNone = 0,

	#ifdef SGFE_3DS
	SGFE_textInputPlatformFlagParental        = SGFE_BIT(0),
	SGFE_textInputPlatformFlagDarkenTopScreen = SGFE_BIT(1),
	SGFE_textInputPlatformFlagPredictiveInput = SGFE_BIT(2),
	SGFE_textInputPlatformFlagMultiline       = SGFE_BIT(3),
	SGFE_textInputPlatformFlagFixedWidth      = SGFE_BIT(4),
	SGFE_textInputPlatformFlagAllowHome       = SGFE_BIT(5),
	SGFE_textInputPlatformFlagAllowReset      = SGFE_BIT(6),
	SGFE_textInputPlatformFlagAllowPower      = SGFE_BIT(7),
	SGFE_textInputPlatformFlagDefaultQwerty   = SGFE_BIT(9),
	#endif
};
#endif

typedef struct SGFE_textInputSettings {
	/* TODO */
	SGFE_textInputType type;
	/* TODO */
	SGFE_textInputPassword password_mode;

	/* TODO */
	SGFE_textInputFlag flags;
	/* TODO */
	SGFE_textInputPlatformFlag platform_flags;

	/* TODO */
	SGFE_bool null_terminated_strings;

	/* TODO */
	isize initial_text_len;
	/* TODO */
	const char* initial_text;

	/* TODO */
	isize hint_text_len;
	/* TODO */
	const char* hint_text;

	/* TODO | -1 | remove? */
	isize dialog_button_count;
} SGFE_textInputSettings;


/* TODO */
SGFE_DEF SGFE_bool SGFE_textInputSettingsMakeDefault(SGFE_textInputSettings* s,
	SGFE_bool null_terminated_strings);
/* TODO */
SGFE_DEF SGFE_bool SGFE_textInputSetPlatformFlags(SGFE_textInputSettings* s);


/* TODO */
SGFE_DEF SGFE_bool SGFE_windowTextInputBegin(SGFE_window* win, u8* buffer, isize buffer_len,
		SGFE_textInputSettings* s);
/* TODO */
SGFE_DEF void SGFE_windowTextInputEnd(SGFE_window* win);


/* TODO*/
SGFE_DEF SGFE_bool SGFE_windowTextInputIsActive(const SGFE_window* win);



/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_videoSignal) {
	SGFE_videoSignalUnknown,

	SGFE_videoSignalNTSC,
	SGFE_videoSignalPAL,
	SGFE_videoSignalMPAL,
	SGFE_videoSignalPAL60,
	/* TODO */
	SGFE_videoSignalDigital,
};


/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_videoSignal SGFE_videoGetSignal(void);


/* Returns a video mode that's considered to be the most optimal for the system
 * by the library. Usually this returns a mode that has a standard resolution,
 * refresh rate and technique for drawing scanlines that works across all models. */
SGFE_DEF SGFE_videoMode SGFE_videoGetOptimalMode(void);
/* TODO(EimaMei): New function. */
SGFE_DEF void SGFE_videoModeResolution(SGFE_videoMode mode,
	isize* out_width, isize* out_height);


/* TODO(EimaMei): New function. */
SGFE_DEF const char* SGFE_videoModeStr(SGFE_videoMode mode);



#ifndef SGFE_CUSTOM_BACKEND
/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_systemModel) {
	SGFE_systemModelUnknown,

	#if SGFE_3DS
	SGFE_systemModel3DS,
	SGFE_systemModel3DSXL,
	SGFE_systemModelN3DS,
	SGFE_systemModel2DS,
	SGFE_systemModelN3DSXL,
	SGFE_systemModelN2DSXL,
	#endif

	SGFE_systemModelCount,
};
#endif

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_systemRegion) {
	SGFE_systemRegionUnknown,

	SGFE_systemRegionUS,
	SGFE_systemRegionEurope,
	SGFE_systemRegionAustralia,

	SGFE_systemRegionJapan,
	SGFE_systemRegionKorea,
	SGFE_systemRegionTaiwan,
	SGFE_systemRegionChina,
	SGFE_systemRegionAsia,

	SGFE_systemRegionBrazil,
	SGFE_systemRegionLatinAmerica,

	SGFE_systemRegionSouthAfrica,
	SGFE_systemRegionAfrica,

	SGFE_systemRegionNotInTheList,
};

/* TODO(EimaMei): document */
typedef SGFE_ENUM(isize, SGFE_systemLanguage) {
	SGFE_systemLanguageUnknown,

	SGFE_systemLanguageEnglish,
	SGFE_systemLanguageFrench,
	SGFE_systemLanguageGerman,
	SGFE_systemLanguageItalian,
	SGFE_systemLanguageSpanish,
	SGFE_systemLanguagePortuguese,
	SGFE_systemLanguageDutch,
	SGFE_systemLanguageRussian,

	SGFE_systemLanguageJapanese,
	SGFE_systemLanguageKorean,
	SGFE_systemLanguageTaiwanese,
	SGFE_systemLanguageChinese,

	SGFE_systemLanguageNotInTheList,
};


/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_systemModel SGFE_systemGetModel(void);

/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_systemRegion SGFE_systemGetRegion(void);

/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_systemLanguage SGFE_systemGetLanguage(void);



/**
 * Converts system ticks into UNIX time with nanosecond precision.
 *
 * Time representation range:
 * min: 1677-09-21 00:12:44.145224192 UTC+0
 * max: 2262-04-11 23:47:16.854775807 UTC+0
 *
 * \returns The converted time
 */
SGFE_DEF i64 SGFE_platformGetTimeFromTicks(u64 ticks);

/**
 * Returns the amount of ticks/the current time-stamp counter.
 *
 * This function is equivalent to RDTSC on x86. It is is specifically useful for
 * durations as the returned time is monotonic.
 *
 * \returns The amount of ticks/The time-stamp counter.
 */
SGFE_DEF u64 SGFE_platformGetTicks(void);
/**
 * Returns the system's clock speed in Hz.
 *
 * \returns The system's clock speed in Hz.
 */
SGFE_DEF u64 SGFE_platformGetClockSpeed(void);


/* TODO(EimaMei): new function. */
SGFE_DEF void SGFE_platformWaitForVBlank(void);

/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_bool SGFE_platformInitTerminalOutput(SGFE_contextBuffer* b);

/* TODO */
SGFE_DEF SGFE_bool SGFE_platformHasSoftwareKeybord(void);


#ifdef SGFE_3DS

/* TODO */
SGFE_DEF SGFE_button SGFE_platformButtonFromAPI(u32 mask);
/* TODO */
SGFE_DEF u32 SGFE_platformButtonToAPI(SGFE_button button);


/* TODO(EimaMei): new function. */
SGFE_DEF float SGFE_platformGet3DSlider(void);


#ifdef SGFE_OPENGL
/* TODO(EimaMei): new function */
SGFE_DEF SGFE_bool SGFE_platformRotateScreenGL(GLuint shader_program, const char* mat4_uniform_name);
#endif

#elif SGFE_WII

/* TODO */
SGFE_DEF SGFE_button SGFE_platformButtonFromWiiRemote(u32 mask);
/* TODO */
SGFE_DEF u32 SGFE_platformButtonFromToRemote(SGFE_button button);


#endif



/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_deviceSleepProc)(SGFE_window* win, SGFE_bool is_sleeping);

/*! SGFE_eventQuit, the window that was closed */
typedef void (*SGFE_quitProc)(SGFE_window* win);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_focusProc)(SGFE_window* win, SGFE_bool is_focused);

/*! SGFE_controllerConnected / SGFE_controllerDisconnected, the window that got the event, the controller in question, if the controller was connected (else it was disconnected) */
typedef void (*SGFE_controllerProc)(SGFE_window* win, SGFE_controller* controller, SGFE_bool is_connected);
/* TODO */
typedef void (*SGFE_controllerBatteryProc)(SGFE_window* win, SGFE_controller* controller, SGFE_powerState state, isize battery_procent);

/*! SGFE_buttonPressed, the window that got the event, the button that was pressed, the scroll value, if it was a press (else it's a release) */
typedef void (*SGFE_buttonProc)(SGFE_window* win, SGFE_controller* controller, SGFE_buttonType button, SGFE_bool down);
/*! SGFE_axisMove, the window that got the event, the controller in question, the axis values and the axis count */
typedef void (*SGFE_axisProc)(SGFE_window* win, SGFE_controller* controller, const SGFE_axis* axis);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_pointerProc)(SGFE_window* win, SGFE_controller* controller, const SGFE_pointer* pointer);
/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_motionProc)(SGFE_window* win, SGFE_controller* controller, const SGFE_motion* motion);

/* TODO(EimaMei): NEW FUNCTION. */
typedef void (*SGFE_textInputProc)(SGFE_window* win, u8* text, isize len);


/*! TODO(EimaMei): new function. */
SGFE_DEF SGFE_deviceSleepProc SGFE_windowSetDeviceSleepCallback(SGFE_window* win, SGFE_deviceSleepProc func);

/*! set callback for a window quit event. Returns previous callback function (if it was set)  */
SGFE_DEF SGFE_quitProc SGFE_windowSetQuitCallback(SGFE_window* win, SGFE_quitProc func);
/*! TODO(EimaMei): new function. */
SGFE_DEF SGFE_focusProc SGFE_windowSetFocusCallback(SGFE_window* win, SGFE_focusProc func);

/*! set callback for when a controller is connected or disconnected. Returns the previous callback function (if it was set) */
SGFE_DEF SGFE_controllerProc SGFE_windowSetControllerCallback(SGFE_window* win, SGFE_controllerProc func);
/* TODO */
SGFE_DEF SGFE_controllerBatteryProc SGFE_windowSetControllerBatteryCallback(SGFE_window* win, SGFE_controllerBatteryProc func);

/*! set callback for a controller button (press / release) event. Returns previous callback function (if it was set)  */
SGFE_DEF SGFE_buttonProc SGFE_windowSetButtonCallback(SGFE_window* win, SGFE_buttonProc func);
/*! set callback for a controller axis move event. Returns previous callback function (if it was set)  */
SGFE_DEF SGFE_axisProc SGFE_windowSetAxisCallback(SGFE_window* win, SGFE_axisProc func);
/* TODO(EimaMei): NEW FUNCTION. */
SGFE_DEF SGFE_pointerProc SGFE_windowSetPointerCallback(SGFE_window* win, SGFE_pointerProc func);
/* TODO(EimaMei): NEW FUNCTION. */
SGFE_DEF SGFE_motionProc SGFE_windowSetMotionCallback(SGFE_window* win, SGFE_motionProc func);

/* TODO(EimaMei): NEW FUNCTION. */
SGFE_DEF SGFE_textInputProc SGFE_windowSetTextInputCallback(SGFE_window* win, SGFE_textInputProc func);



typedef SGFE_ENUM(isize, SGFE_debugSource) {
	/* TODO */
	SGFE_debugSourceAPI,
	/* TODO */
	SGFE_debugSourcePlatformAPI,
	/* TODO */
	SGFE_debugSourceSystem,
	/* TODO */
	SGFE_debugSourceGL,
	/* TODO */
	SGFE_debugSourceApp,

	SGFE_debugSourceCount
};

typedef SGFE_ENUM(isize, SGFE_debugType) {
	SGFE_debugTypeError,
	SGFE_debugTypeWarning,
	SGFE_debugTypeInfo,

	SGFE_debugTypeCount
};

typedef const struct SGFE_debugContext {
	/* From which source the debug message came from. */
	SGFE_debugSource source;
	/* Type/severity of the debug message. */
	SGFE_debugType type;
	/* Implementation defined debug message code. The code is defined by the aforementioned source
	 * and type. */
	isize code;

	/* Application-defined message length. 0 on every source except on 'SGFE_debugSourceApp'.
	 * if the user has specified one. */
	isize msg_len;
	/* Application-defined message length. Empty string except on 'SGFE_debugSourceApp'.
	 * if the user has specified one. */
	const u8* msg;

	/* The file's line count where the debug message was called. */
	isize line;
	/* The file where the debug message was called. */
	const char* filename;
	/* The function where the debug message was called. */
	const char* function;

	/* Implementation-defined 'context' parameter. This holds a pointer to an object
	 * that relates to the debug message. For 'SGFE_debugSource[Platform]API' this
	 * can be a window, an OpenGL context or buffer context — which one specifically
	 * depends on the 'code' value. */
	void* ctx;
	/* User-supplemented parameter data. This is set by 'SGFE_setDebugCallback'. */
	void* user_data;
} SGFE_debugContext;


/* TODO */
typedef void (*SGFE_debugProc)(SGFE_debugContext ctx);
/* TODO(EimaMei): new function. */
SGFE_DEF SGFE_debugProc SGFE_setDebugCallback(SGFE_debugProc func, void* user_param);


/* TODO | sends SGFE_debugSourceApp */
#define SGFE_debugSend(ctx, type, code) SGFE_debugSendMsg(ctx, type, code, "")
/* TODO | sends SGFE_debugSourceApp */
#define SGFE_debugSendMsg(ctx, type, code, msg) (SGFE_debugSendMsg)(ctx, type, code, msg, __FILE__, __LINE__, __func__)

/* TODO */
SGFE_DEF const char* SGFE_debugSourceName(SGFE_debugSource source);
/* TODO */
SGFE_DEF const char* SGFE_debugTypeName(SGFE_debugType type);

/* TODO */
SGFE_DEF const char* SGFE_debugCodeGetName(SGFE_debugSource source, SGFE_debugType type, isize code);
/* TODO */
SGFE_DEF const char* SGFE_debugCodeGetDesc(SGFE_debugSource source, SGFE_debugType type, isize code);


typedef SGFE_ENUM(isize, SGFE_error) {
	SGFE_errorOutOfMemory,

	SGFE_errorEventQueue,
	SGFE_errorMultipleAPIs,

	SGFE_errorCreateContextBuffer,
	SGFE_errorCreateContextGL,

	SGFE_errorGLContextProfile,
	SGFE_errorGLContextVersion,
	SGFE_errorGLContextStencil,
	SGFE_errorGLContextDepth,
	SGFE_errorGLContextColor,
	SGFE_errorGLContextSamples,
	SGFE_errorGLContextDownscale,

	SGFE_errorCount,
};

typedef SGFE_ENUM(isize, SGFE_warning) {
	SGFE_warningGLContextStencil,
	SGFE_warningGLContextDepth,
	SGFE_warningGLContextColor,

	SGFE_warningCount,
};

typedef SGFE_ENUM(isize, SGFE_info) {
	SGFE_infoCreateContextBuffer,
	SGFE_infoCreateContextGL,

	SGFE_infoFreeContextBuffer,
	SGFE_infoFreeContextGL,

	SGFE_infoCount,
};

/* TODO */
SGFE_DEF const char* SGFE_debugCodeAPIGetName(SGFE_debugType type, isize code);
/* TODO */
SGFE_DEF const char* SGFE_debugCodeAPIGetDesc(SGFE_debugType type, isize code);


typedef SGFE_ENUM(isize, SGFE_errorPlatform) {
	#ifdef SGFE_3DS
	SGFE_errorPlatformInitKYGX,
	SGFE_errorPlatformCreateGlassCtx,
	SGFE_errorPlatformTextInput,
	#endif
	SGFE_errorPlatformCount
};

typedef SGFE_ENUM(isize, SGFE_warningPlatform) {
	SGFE_warningPlatformCount
};

typedef SGFE_ENUM(isize, SGFE_infoPlatform) {
	SGFE_infoPlatformCount
};

/* TODO */
SGFE_DEF const char* SGFE_debugSourcePlatformAPIGetName(SGFE_debugType type, isize code);
/* TODO */
SGFE_DEF const char* SGFE_debugSourcePlatformAPIGetDesc(SGFE_debugType type, isize code);


/* TODO */
SGFE_DEF const char* SGFE_debugCodeSystemGetName(SGFE_debugType type, isize code);
/* TODO */
SGFE_DEF const char* SGFE_debugSourceSystemGetDesc(SGFE_debugType type, isize code);


/* TODO */
SGFE_DEF const char* SGFE_debugCodeGLGetName(isize code);
/* TODO */
SGFE_DEF const char* SGFE_debugCodeGLGetDesc(isize code);



#if 1
void (SGFE_debugSendMsg)(void* ctx_ptr, SGFE_debugType type, isize code,
	const char* msg, const char* filename, isize line, const char* function);
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
const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_bufferFormatCount];

const char* SGFE_VIDEO_MODE_NAME_LUT[SGFE_videoModeCount];
const char* SGFE_PIXEL_FORMAT_NAME_LUT[SGFE_bufferFormatCount];



/* NOTE(EimaMei): Do not use this! Only used for assertions. */
SGFE_window* SGFE__ROOT_WIN;



SGFE_DEF SGFE_bool SGFE_windowMake_platform(SGFE_window* win);

SGFE_DEF void SGFE_windowClose_platform(SGFE_window* win);

#if SGFE_HAS_MULTIPLE_SCREENS != 0
SGFE_DEF SGFE_screen SGFE_windowGetCurrentScreen_platform(SGFE_window* win);
SGFE_DEF SGFE_bool SGFE_windowIsScreenEnabled_platform(SGFE_window* win, SGFE_screen screen);
#endif



SGFE_DEF const char* SGFE_controllerGetNameButton_platform(SGFE_controllerType type,
	SGFE_buttonType button);

SGFE_DEF SGFE_bool SGFE_controllerEnablePointer_platform(SGFE_controller* controller,
	SGFE_pointerType pointer, SGFE_bool enable);

SGFE_DEF SGFE_bool SGFE_controllerEnableMotion_platform(SGFE_controller* controller,
	SGFE_pointerType pointer, SGFE_bool enable);


SGFE_DEF u8* SGFE__fetchSwapBuffer(SGFE_contextBuffer* b);

void SGFE__controllerSetConnection(SGFE_window* win, SGFE_controller* controller, SGFE_bool should_connect);
void SGFE__controllerAddToList(SGFE_controllerList* list, SGFE_controller* controller);
void SGFE__controllerRemoveFromList(SGFE_controllerList* list, SGFE_controller* controller);

SGFE_DEF void SGFE__processCallbackAndEventQueue_ButtonDown(SGFE_window* win, SGFE_controller* controller);
SGFE_DEF void SGFE__processCallbackAndEventQueue_ButtonUp(SGFE_window* win, SGFE_controller* controller);
SGFE_DEF void SGFE__processCallbackAndEventQueue_Axis(SGFE_window* win, SGFE_controller* controller, const SGFE_axis* axis);
SGFE_DEF void SGFE__processCallbackAndEventQueue_Pointer(SGFE_window* win, SGFE_controller* controller, const SGFE_pointer* p);
void SGFE__processCallbackAndEventQueue_Motion(SGFE_window* win, SGFE_controller* controller, const SGFE_motion* m);

void (SGFE_debugSendAPI)(void* ctx_ptr, SGFE_debugType type, isize code, const char* filename, isize line, const char* function);
void (SGFE_debugSendPlatformAPI)(void* ctx_ptr, SGFE_debugType type, isize code, const char* filename, isize line, const char* function);
void (SGFE_debugSendSystem)(void* ctx_ptr, isize code, const char* filename, isize line, const char* function);
SGFE_bool (SGFE_debugSendGL)(void* ctx_ptr, const char* filename, isize line, const char* function);
SGFE_debugType SGFE_debugSystemGenerateType_platform(void* ctx_ptr, isize code);

/* var - VARIABLE | mask - UINT | set - SGFE_bool
 * Sets/unsets the mask for the variable. */
#define SGFE_MASK_SET(var, mask, set) do { \
	if (set) (var) |=  (mask); \
	else     (var) &= ~(mask); \
} while (0)

#define SGFE_CALLBACK_TEMPLATE(proc, member_name, args) \
	if (win->callbacks.member_name) { \
		((proc)win->callbacks.member_name)args; \
	} do {} while(0)

SGFE_debugProc SGFE_debugProcSrc;
void* SGFE__debugProcSrcUserParam;

#define SGFE_windowDeviceSleepCallback(win, is_sleeping) \
	SGFE_CALLBACK_TEMPLATE(SGFE_deviceSleepProc, sleep, (win, is_sleeping))

#define SGFE_windowQuitCallback(win) \
	SGFE_CALLBACK_TEMPLATE(SGFE_quitProc, quit, (win))

#define SGFE_windowFocusCallback(win, is_focused) \
	SGFE_CALLBACK_TEMPLATE(SGFE_focusProc, focus, (win, is_focused))

#define SGFE_windowControllerCallback(win, controller_s, is_connected) \
	SGFE_CALLBACK_TEMPLATE(SGFE_controllerProc, controller, (win, controller_s, is_connected))

#define SGFE_windowControllerBatteryCallback(win, controller, state, battery_procent) \
	SGFE_CALLBACK_TEMPLATE(SGFE_controllerBatteryProc, controller_battery, (win, controller, state, battery_procent))

#define SGFE_windowButtonCallback(win, controller, button_s, pressed) \
	SGFE_CALLBACK_TEMPLATE(SGFE_buttonProc, button, (win, controller, button_s, pressed))

#define SGFE_windowAxisCallback(win, controller, type) \
	SGFE_CALLBACK_TEMPLATE(SGFE_axisProc, axis, (win, controller, type))

#define SGFE_windowPointerCallback(win, controller, type) \
	SGFE_CALLBACK_TEMPLATE(SGFE_pointerProc, pointer, (win, controller, type))

#define SGFE_windowMotionCallback(win, controller, type) \
	SGFE_CALLBACK_TEMPLATE(SGFE_motionProc, motion, (win, controller, type))


#define SGFE_windowTextInputCallback(win, text, len) \
	SGFE_CALLBACK_TEMPLATE(SGFE_textInputProc, text_input, (win, text, len))


#define SGFE_debugCallback(ctx) \
	if (SGFE_debugProcSrc) { \
		ctx.user_data = SGFE__debugProcSrcUserParam; \
		SGFE_debugProcSrc(ctx); \
	} do {} while(0)



/* TODO */
#define SGFE_debugSendAPI(ctx, type, code) (SGFE_debugSendAPI)(ctx, type, code, __FILE__, __LINE__, __func__)
/* TODO */
#define SGFE_debugSendPlatformAPI(ctx, type, code) (SGFE_debugSendPlatformAPI)(ctx, type, code, __FILE__, __LINE__, __func__)
/* TODO */
#define SGFE_debugSendSystem(ctx, code) (SGFE_debugSendSystem)(ctx, code, __FILE__, __LINE__, __func__)
/* TODO | can return true or false */
#define SGFE_debugSendGL(ctx) (SGFE_debugSendGL)(ctx, __FILE__, __LINE__, __func__)


void SGFE__controllerSetConnection(SGFE_window* win, SGFE_controller* controller, SGFE_bool should_connect) {
	SGFE_ASSERT(SGFE_windowIsControllerConnected(win, controller) == !should_connect);

	if (should_connect) {
		SGFE__controllerRemoveFromList(&win->disconnected_controllers, controller);
		SGFE__controllerAddToList(&win->state.controllers, controller);
	}
	else if (!should_connect) {
		SGFE__controllerRemoveFromList(&win->state.controllers, controller);
		SGFE__controllerAddToList(&win->disconnected_controllers, controller);
	}
}

void SGFE__controllerAddToList(SGFE_controllerList* list, SGFE_controller* controller) {
	SGFE_ASSERT(list->count >= 0 && list->count < SGFE_MAX_CONTROLLERS);

	if (list->first == NULL) {
		list->count = 1;
		list->first = controller;
		list->last = controller;

		controller->prev = NULL;
		controller->next = NULL;
		return;
	}
	SGFE_controller* previous_last = list->last;

	controller->prev = previous_last;
	controller->next = NULL;

	previous_last->next = controller;
	list->last = controller;
	list->count += 1;
}

void SGFE__controllerRemoveFromList(SGFE_controllerList* list, SGFE_controller* controller) {
	SGFE_ASSERT(list->count > 0 && list->count <= SGFE_MAX_CONTROLLERS);

	if (list->count == 1) {
		list->first = NULL;
		list->last = NULL;
		list->count = 0;
		return;
	} else if (list->first == controller) {
		list->first = controller->next;
		list->count -= 1;
		return;
	} else if (list->last == controller) {
		controller->prev->next = NULL;
		list->last = controller->prev;
		list->count -= 1;
		return;
	}

	controller->prev->next = controller->next;
	controller->next->prev = controller->prev;

	list->count += 1;
}


void SGFE__processCallbackAndEventQueue_ButtonDown(SGFE_window* win, SGFE_controller* controller) {
	if (controller->buttons_held == 0 || (!win->is_queueing_events && !win->callbacks.button)) {
		return;
	}

	SGFE_button mask = controller->buttons_held;
	SGFE_buttonType button;

	while (SGFE_iterateButtonMask(&mask, &button)) {
		SGFE_windowButtonCallback(win, controller, button, SGFE_TRUE);

		if (win->is_queueing_events) {
			SGFE_event event;
			event.type = SGFE_eventButtonDown;
			event.button.timestamp = SGFE_platformGetTicks();
			event.button.controller = controller;
			event.button.button = button;
			event.button.repeat = (controller->buttons_down & SGFE_BIT(button)) == 0;
			event.button.raw_button = SGFE_buttonToAPI(controller->type, SGFE_BIT(button));
			SGFE_windowEventPush(win, &event);
		}
	}
}

void SGFE__processCallbackAndEventQueue_ButtonUp(SGFE_window* win, SGFE_controller* controller) {
	if (controller->buttons_up == 0 || (!win->is_queueing_events && !win->callbacks.button)) {
		return;
	}

	SGFE_button mask = controller->buttons_up;
	SGFE_buttonType button;

	while (SGFE_iterateButtonMask(&mask, &button)) {
		SGFE_windowButtonCallback(win, controller, button, SGFE_TRUE);

		if (win->is_queueing_events) {
			SGFE_event event;
			event.type = SGFE_eventButtonUp;
			event.button.controller = controller;
			event.button.button = button;
			event.button.repeat = SGFE_FALSE;
			event.button.raw_button = SGFE_buttonToAPI(controller->type, SGFE_BIT(button));
			SGFE_windowEventPush(win, &event);
		}
	}
}

void SGFE__processCallbackAndEventQueue_Axis(SGFE_window* win, SGFE_controller* controller, const SGFE_axis* axis) {
	if (!win->is_queueing_events && !win->callbacks.axis) {
		return;
	}

	SGFE_windowAxisCallback(win, controller, axis);
	if (win->is_queueing_events) {
		SGFE_event event;
		event.type = SGFE_eventAxis;
		event.axis.controller = controller;
		event.axis.which = axis->type;
		event.axis.value = axis->value;
		event.axis.deadzone = axis->deadzone;
		SGFE_windowEventPush(win, &event);
	}
}

void SGFE__processCallbackAndEventQueue_Pointer(SGFE_window* win, SGFE_controller* controller, const SGFE_pointer* pointer) {
	if (!win->is_queueing_events && !win->callbacks.pointer) {
		return;
	}

	SGFE_windowPointerCallback(win, controller, pointer);
	if (win->is_queueing_events) {
		SGFE_event event;
		event.type = SGFE_eventPointer;
		event.pointer.controller = controller;
		event.pointer.which = pointer->type;
		event.pointer.x = pointer->x;
		event.pointer.y = pointer->y;
		SGFE_windowEventPush(win, &event);
	}
}

void SGFE__processCallbackAndEventQueue_Motion(SGFE_window* win, SGFE_controller* controller, const SGFE_motion* motion) {
	if (!win->is_queueing_events && !win->callbacks.motion) {
		return;
	}

	SGFE_windowMotionCallback(win, controller, motion);
	if (win->is_queueing_events) {
		SGFE_event event;
		event.type = SGFE_eventMotion;
		event.motion.controller = controller;
		event.motion.which = motion->type;
		event.motion.x = motion->x;
		event.motion.y = motion->y;
		event.motion.z = motion->z;
		SGFE_windowEventPush(win, &event);
	}
}

isize SGFE_sizeofWindow(void) { return sizeof(SGFE_window); }
isize SGFE_sizeofWindowSrc(void) { return sizeof(SGFE_windowSource); }

void* SGFE_windowGetUserPtr(SGFE_window* win) { SGFE_ASSERT_NOT_NULL(win); return win->user_ptr; }
void SGFE_windowSetUserPtr(SGFE_window* win, void* ptr) { SGFE_ASSERT_NOT_NULL(win); win->user_ptr = ptr; }

SGFE_windowSource* SGFE_windowGetSource(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	return &win->src;
}

SGFE_screen SGFE_windowGetCurrentScreen(SGFE_window* win) {
	#if SGFE_HAS_MULTIPLE_SCREENS == 0
	return SGFE_screenPrimary;
	#else
	return SGFE_windowGetCurrentScreen_platform(win);
	#endif
}

SGFE_bool SGFE_windowIsScreenEnabled(SGFE_window* win, SGFE_screen screen) {
	#if SGFE_HAS_MULTIPLE_SCREENS == 0
	return SGFE_TRUE;
	#else
	return SGFE_windowIsScreenEnabled_platform(win, screen);
	#endif
}



SGFE_window* SGFE_windowMake(SGFE_videoMode mode, SGFE_windowFlag flags) {
	SGFE_window* win = (SGFE_window*)SGFE_ALLOC(sizeof(SGFE_window));
	if (win == NULL) {
		SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorOutOfMemory);
		return NULL;
	}

	SGFE_window* res = SGFE_windowMakePtr(mode, flags, win);
	if (res != NULL) { win->is_allocated = SGFE_TRUE; }
	else { SGFE_FREE(win); }

	return res;
}

SGFE_window* SGFE_windowMakePtr(SGFE_videoMode mode, SGFE_windowFlag flags,
		SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

	win->is_allocated = SGFE_FALSE;
	win->is_queueing_events = SGFE_FALSE;
	win->has_polled_events = SGFE_FALSE;
	win->event_len = 0;
	win->flags = flags;
	win->user_ptr = NULL;
	win->state.should_quit = SGFE_FALSE;

	SGFE_MEMSET(win->current_type, 0, sizeof(win->current_type));
	SGFE_MEMSET(win->current, 0, sizeof(win->current));
	SGFE_MEMSET(win->ctx, 0, sizeof(win->ctx));
	SGFE_MEMSET(win->controllers, 0, sizeof(win->controllers));
	SGFE_MEMSET(&win->disconnected_controllers, 0, sizeof(win->disconnected_controllers));
	SGFE_MEMSET(&win->state, 0, sizeof(win->state));
	SGFE_MEMSET(&win->callbacks, 0, sizeof(win->callbacks));

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		SGFE__controllerAddToList(&win->disconnected_controllers, &win->controllers[i]);
	}

	SGFE_windowSetEventEnabledDefault(win);

	SGFE_bool res = SGFE_windowMake_platform(win);
	if (res == SGFE_FALSE) { return NULL; }
	SGFE__ROOT_WIN = win;
	SGFE_windowSetFlags(win, win->flags);

	switch (win->flags & (SGFE_windowFlagBuffer | SGFE_windowFlagOpenGL)) {
		case 0: break;
		case SGFE_windowFlagBuffer: {
			#ifndef SGFE_BUFFER_NO_CONVERSION
			SGFE_bufferFormat format = SGFE_bufferFormatRGBA8;
			SGFE_bool is_native = SGFE_FALSE;
			#else
			SGFE_bufferFormat format = SGFE_bufferFormatOptimal();
			SGFE_bool is_native = SGFE_TRUE;
			#endif

			res = SGFE_windowCreateContextBuffer(win, mode, format, is_native);
		} break;

		#ifdef SGFE_OPENGL
		case SGFE_windowFlagOpenGL: {
			res = SGFE_windowCreateContextGL(win, mode);
		} break;
		#endif

		default:
			SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorMultipleAPIs);
	}

	if (!res) {
		SGFE_windowClose(win);
		return NULL;
	}

	return win;
}

SGFE_window* SGFE_windowMakeContextless(SGFE_windowFlag flags) {
	SGFE_ASSERT((flags & (SGFE_windowFlagOpenGL | SGFE_windowFlagBuffer)) == 0);
	return SGFE_windowMake(0, flags);
}

SGFE_window* SGFE_windowMakePtrContextless(SGFE_windowFlag flags, SGFE_window* win) {
	return SGFE_windowMakePtr(0, flags, win);
}


void SGFE_windowClose(SGFE_window* win) {
	SGFE_windowFreeContext(win);
	SGFE_windowSetVisible(win, SGFE_FALSE);
	SGFE_windowClose_platform(win);

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		SGFE_controller* controller = SGFE_windowGetController(win, i);
		if (win == NULL) { continue; }
		SGFE__controllerSetConnection(win, controller, SGFE_FALSE);
	}

	#ifndef SGFE__BACKEND_FREE_WINDOW_IN_CLOSE
	if (win->is_allocated) {
		SGFE_FREE(win);
	}
	#endif
}

void SGFE_windowFreeContext(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

	for (SGFE_screen screen = SGFE_screenPrimary; screen < SGFE_screenCount; screen += 1) {
		switch (win->flags & (SGFE_windowFlagBuffer | SGFE_windowFlagOpenGL)) {
			case 0: break;
			case SGFE_windowFlagBuffer: {
				SGFE_bufferFreeContext(&win->ctx[screen].buffer);
			} break;

			#ifdef SGFE_OPENGL
			case SGFE_windowFlagOpenGL: {
				SGFE_glFreeContext(&win->ctx[screen].gl);
			} break;
			#endif

			default: SGFE_ASSERT(0);
		}
	}

	win->flags &= ~(SGFE_windowFlag)(SGFE_windowFlagBuffer | SGFE_windowFlagOpenGL);
}


SGFE_windowFlag SGFE_windowGetFlags(const SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	return win->flags;
}


SGFE_bool SGFE_windowGetIsQueuingEvents(const SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	return win->is_queueing_events;

}

void SGFE_windowSetQueueEvents(SGFE_window* win, SGFE_bool is_queuing_events) {
	SGFE_ASSERT_NOT_NULL(win);
	win->is_queueing_events = SGFE_BOOL(is_queuing_events);
}

SGFE_bool SGFE_windowGetEventEnabled(const SGFE_window* win, SGFE_eventType type) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(type > 0 && type < SGFE_eventCount);

	return win->enabled_events[type];
}

void SGFE_windowSetEventEnabled(SGFE_window* win, SGFE_eventType type, SGFE_bool is_enabled) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(type > 0 && type < SGFE_eventCount);

	win->enabled_events[type] = SGFE_BOOL(is_enabled);
}

void SGFE_windowSetEventEnabledDefault(SGFE_window* win) {
	SGFE_windowSetEventEnabled(win, SGFE_eventQuit, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventDeviceSleep, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventDeviceWakeup, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventFocusOut, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventFocusIn, SGFE_TRUE);

	SGFE_windowSetEventEnabled(win, SGFE_eventControllerConnected, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventControllerDisconnected, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventControllerBattery, SGFE_TRUE);

	SGFE_windowSetEventEnabled(win, SGFE_eventButtonDown, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventButtonUp, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventAxis, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventPointer, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMotion, SGFE_TRUE);

	SGFE_windowSetEventEnabled(win, SGFE_eventKeyboardConnected, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventKeyboardDisconnected, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventKeyboardDown, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventKeyboardUp, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventTextInput, SGFE_FALSE);

	SGFE_windowSetEventEnabled(win, SGFE_eventMouseConnected, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMouseDisconnected, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMouseMove, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMouseButtonDown, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMouseButtonUp, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventMouseWheel, SGFE_FALSE);
}

SGFE_bool SGFE_windowCheckEvent(SGFE_window* win, const SGFE_event** out_event) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(out_event != NULL);

	if (win->has_polled_events == SGFE_FALSE) {
		SGFE_windowPollEvents(win);
	}

	const SGFE_event* event = SGFE_windowEventPop(win);
	*out_event = event;
	win->has_polled_events = (event != NULL);

	return (event != NULL);
}

SGFE_bool SGFE_windowCheckQueuedEvent(SGFE_window* win, const SGFE_event** out_event) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(out_event != NULL);

	const SGFE_event* event = SGFE_windowEventPop(win);
	*out_event = event;

	return (event != NULL);
}

SGFE_bool SGFE_windowEventPush(SGFE_window* win, const SGFE_event* event) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(win->event_len >= 0);

	if (win->event_len >= SGFE_COUNTOF(win->events)) {
		SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorEventQueue);
		SGFE_windowEventQueueFlush(win);
		return SGFE_FALSE;
	}

	win->event_len += 1;
	SGFE_event* out = &win->events[SGFE_COUNTOF(win->events) - win->event_len];
	*out = *event;
	out->text.timestamp = SGFE_platformGetTicks();

	return SGFE_TRUE;
}

const SGFE_event* SGFE_windowEventPop(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(win->event_len >= 0 && win->event_len <= SGFE_COUNTOF(win->events));

	if (win->event_len <= 0) {
		return NULL;
	}
	SGFE_ASSERT(win->is_queueing_events == SGFE_TRUE);

	SGFE_event* ev = &win->events[SGFE_COUNTOF(win->events) - win->event_len];
	win->event_len -= 1;

	return ev;
}

void SGFE_windowEventQueueFlush(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	win->event_len = 0;
}


void SGFE_windowSwapBuffers(SGFE_window* win) {
	SGFE_ASSERT_MSG(SGFE_windowIsVisible(win), "Swapping buffers when the screen is not visible. Possibly user-bug?");

	switch (SGFE_windowGetContextType(win)) {
		case SGFE_contextTypeBuffer: {
			SGFE_windowSwapBuffersBuffer(win);
		} break;

		#ifdef SGFE_OPENGL
		case SGFE_contextTypeGL: {
			SGFE_windowSwapBuffersGL(win);
		} break;
		#endif
	}
}

SGFE_contextType SGFE_windowGetContextType(SGFE_window* win) {
	return SGFE_windowGetContextTypeEx(win, SGFE_windowGetCurrentScreen(win));
}

SGFE_contextType SGFE_windowGetContextTypeEx(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(screen >= 0 && screen < SGFE_screenCount);
	return win->current_type[screen];
}

void* SGFE_windowGetContext(SGFE_window* win) {
	return SGFE_windowGetContextEx(win, SGFE_windowGetCurrentScreen(win));
}

void* SGFE_windowGetContextEx(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(screen >= 0 && screen < SGFE_screenCount);

	return win->current[screen];
}


void SGFE_windowSetContext(SGFE_window* win, SGFE_contextType type, void* ctx) {
	SGFE_windowSetContextEx(win, type, ctx, SGFE_windowGetCurrentScreen(win));
}

void SGFE_windowSetContextEx(SGFE_window* win, SGFE_contextType type, void* ctx,
		SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(type > 0 && type < SGFE_contextTypeCount);

	switch (type) {
		case SGFE_contextTypeBuffer: {
			SGFE_windowSetContextExBuffer(win, (SGFE_contextBuffer*)ctx, screen);
		} break;

		#ifdef SGFE_OPENGL
		case SGFE_contextTypeGL: {
			SGFE_windowSetContextExGL(win, (SGFE_contextGL*)ctx, screen);
		} break;
		#endif
	}
}


SGFE_bool SGFE_windowShouldClose(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	return win->state.should_quit;
}

void SGFE_windowSetShouldClose(SGFE_window* win, SGFE_bool should_close) {
	SGFE_ASSERT_NOT_NULL(win);

	win->state.should_quit = SGFE_BOOL(should_close);
	if (should_close) {
		SGFE_windowQuitCallback(win);
	}
}


SGFE_bool SGFE_windowIsVisible(const SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	return win->state.is_visible;
}


SGFE_bool SGFE_windowInitTerminalOutput(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

	if (SGFE_windowGetContextType(win) != SGFE_contextTypeBuffer) {
		SGFE_bool res = SGFE_windowCreateContextBuffer(
			win, SGFE_videoGetOptimalMode(), SGFE_TRUE, SGFE_TRUE
		);
		if (res == SGFE_FALSE) { return SGFE_FALSE; }
	}

	SGFE_bool res = SGFE_platformInitTerminalOutput(SGFE_windowGetContextBuffer(win));
	if (res == SGFE_FALSE) { return res; }
	SGFE_windowSwapBuffers(win);

	SGFE__ROOT_WIN = win;
	return SGFE_TRUE;
}

void SGFE_windowAssert(SGFE_window* win, SGFE_bool is_asserted, const char* condition_str,
		const char* file, isize line, const char* message) {
	if (is_asserted) { return; }
	if (win == NULL) { win = SGFE__ROOT_WIN; }

	if (win == NULL) {
		win = SGFE_windowMakeContextless(SGFE_windowFlagNone);
		if (win == NULL) {
			/* NOTE(EimaMei): In case that 'stderr' actually leads to somewhere. */
			SGFE_PRINTF("%s:%zi: %s: %s\n", file, line, condition_str, message ? message : "");
			exit(1);
		}
	}

	SGFE_windowSetQueueEvents(win, SGFE_FALSE);
	SGFE_windowSetEventEnabled(win, SGFE_eventQuit, SGFE_TRUE);
	SGFE_windowSetEventEnabled(win, SGFE_eventButtonDown, SGFE_TRUE);

	SGFE_bool res = SGFE_windowInitTerminalOutput(win);
	SGFE_PRINTF("\033[91m%s:%zi\033[0m: %s: %s\n\nPress any button to exit.\n", file, line, condition_str, message ? message : "");
	if (!res) { exit(1); }

	while (!SGFE_windowShouldClose(win)) {
		SGFE_windowPollEvents(win);

		for (ssize_t i = 0; i < win->state.controllers.count; i += 1) {
			SGFE_controller* controller = SGFE_windowGetController(win, i);
			if (controller == NULL) { continue; }

			if (controller->buttons_down != 0) {
				SGFE_windowSetShouldClose(win, SGFE_TRUE);
				break;
			}
		}

		SGFE_windowSwapBuffers(win);
	}

	SGFE_windowClose(win);
	exit(1);
}

#ifdef SGFE_NO_CUSTOM_ASSERT_FMT
#include <stdarg.h>

void SGFE_windowAssertFmt(SGFE_window* win, SGFE_bool is_asserted, const char* condition_str,
		const char* file, isize line, const char* fmt, ...) {
	if (is_asserted) { return; }

	char buffer[1024];
	va_list va;
	va_start(va, fmt);
	vsnprintf(buffer, SGFE_COUNTOF(buffer), fmt, va);
	va_end(va);

	SGFE_windowAssert(win, is_asserted, condition_str, file, line, buffer);
}
#endif

SGFE_bool SGFE_windowCreateContextBuffer(SGFE_window* win, SGFE_videoMode mode,
		SGFE_bufferFormat format, SGFE_bool is_native) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_windowFreeContext(win);

	for (SGFE_screen screen = SGFE_screenPrimary; screen < SGFE_screenCount; screen += 1) {
		SGFE_contextBuffer* b = &win->ctx[screen].buffer;
		SGFE_bufferMakeWithDefaultSettings(b, mode, format, SGFE_FALSE);
		b->screen = screen;
		b->is_native = is_native;

		SGFE_bool res = SGFE_bufferAllocFramebuffers(b);
		if (res == SGFE_FALSE) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorCreateContextBuffer);
			return SGFE_FALSE;
		}

		res = SGFE_bufferCreateContext(b);
		if (res == SGFE_FALSE) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorCreateContextBuffer);
			return SGFE_FALSE;
		}

		SGFE_windowSetContextExBuffer(win, b, screen);
	}

	SGFE_windowSetVisible(win, !SGFE_BOOL(win->flags & SGFE_windowFlagDontShow));
	win->flags |= SGFE_windowFlagBuffer;
	return SGFE_TRUE;
}

SGFE_contextBuffer* SGFE_windowGetContextBuffer(SGFE_window* win) {
	return SGFE_windowGetContextEx(win, SGFE_windowGetCurrentScreen(win));
}

SGFE_contextBuffer* SGFE_windowGetContextExBuffer(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT(SGFE_windowGetContextTypeEx(win, screen) == SGFE_contextTypeBuffer);
	return (SGFE_contextBuffer*)SGFE_windowGetContextEx(win, screen);
}

void SGFE_windowSetContextBuffer(SGFE_window* win, SGFE_contextBuffer* ctx) {
	SGFE_windowSetContextExBuffer(win, ctx, SGFE_windowGetCurrentScreen(win));
}

void SGFE_windowSetContextExBuffer(SGFE_window* win, SGFE_contextBuffer* b,
		SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(b == NULL || b->screen == screen);

	win->current[screen] = b;
	win->current_type[screen] = b ? SGFE_contextTypeBuffer : SGFE_contextTypeNone;
}

SGFE_bool SGFE_bufferMakeWithDefaultSettings(SGFE_contextBuffer* out_buffer,
		SGFE_videoMode mode, SGFE_bufferFormat format, SGFE_bool allocate_buffers) {
	SGFE_ASSERT(out_buffer != NULL);
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	SGFE_ASSERT(format >= 0 && format < SGFE_bufferFormatCount);

	SGFE_contextBuffer* b = out_buffer;
	b->screen = SGFE_screenPrimary;
	b->mode = mode;
	b->format = format;

	b->current = 0;

	b->is_buffer_allocated = SGFE_FALSE;
	b->is_double_buffered = SGFE_TRUE;
	b->is_native = SGFE_FALSE;

	if (allocate_buffers) {
		SGFE_bool res = SGFE_bufferAllocFramebuffers(out_buffer);
		if (res) { return SGFE_FALSE; }
	}
	else {
		SGFE_MEMSET(b->buffers, 0, sizeof(b->buffers));
		SGFE_MEMSET(b->buffers_native, 0, sizeof(b->buffers_native));
	}

	SGFE_bool res = SGFE_bufferSetPlatformSettings(b);
	if (!res) { return SGFE_FALSE; }

	SGFE_bufferSetSwapInterval(b, 1);
	return SGFE_TRUE;
}


isize SGFE_bufferGetSwapInterval(const SGFE_contextBuffer* b) {
	isize fps = SGFE_bufferGetFramesPerSecond(b);
	return (fps >= 0) ? SGFE_VBLANK_RATE / fps : fps;
}

isize SGFE_bufferGetFramesPerSecond(const SGFE_contextBuffer* b) {
	SGFE_ASSERT_NOT_NULL(b);
	return b->frames;
}

void SGFE_bufferSetSwapInterval(SGFE_contextBuffer* b, isize swap_interval) {
	SGFE_bufferSetFramesPerSecond(
		b, (swap_interval >= 0) ? (SGFE_VBLANK_RATE / swap_interval) : swap_interval
	);
}
void SGFE_bufferSetFramesPerSecond(SGFE_contextBuffer* b, isize fps) {
	SGFE_ASSERT_NOT_NULL(b);
	b->frames = fps;
}


SGFE_bool SGFE_bufferSetFormat(SGFE_contextBuffer* b, SGFE_bufferFormat format) {
	if (b == NULL) { return SGFE_FALSE; }

	/* TODO(EimaMei): Have a way to save the original bpp for performance reasons. */
	if (SGFE_bufferFormatGetBytesPerPixel(b->format) < SGFE_bufferFormatGetBytesPerPixel(format)) {
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


SGFE_bufferFormat SGFE_bufferGetFormat(const SGFE_contextBuffer* b) {
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
	return b->screen;
}

isize SGFE_bufferGetFramebufferCount(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->is_double_buffered ? 2 : 1;
}

SGFE_videoMode SGFE_bufferGetVideoMode(const SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	return b->mode;
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
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT_FMT(port >= 0 && port < SGFE_MAX_CONTROLLERS, "port = %i;", port);

	const SGFE_controllerList* list = &win->state.controllers;
	if (port >= list->count) { return NULL; }

	SGFE_controller* res = list->first;
	for (isize i = port + 1; i < list->count; i += 1) {
		res = res->next;
	}

	return res;
}

SGFE_bool SGFE_windowIsControllerConnected(SGFE_window* win, const SGFE_controller* controller) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT_NOT_NULL(controller);

	const SGFE_controllerList* list = &win->state.controllers;

	SGFE_controller* res = list->first;
	for (isize i = 0; i < list->count; i += 1) {
		if (res == controller) { return SGFE_TRUE; }
		res = res->next;
	}

	/* NOTE(EimaMei): There's a slim chance that the given controller is invalid
	 * because A) it doesn't come from SGFE directly OR B) the backend forgot to
	 * set it as disconnected. Regardless, this code should catch both cases during
	 * debug. */
#ifndef NDEBUG
	list = &win->disconnected_controllers;

	res = list->first;
	for (isize i = 0; i < list->count; i += 1) {
		/* NOTE(EimaMei): Controller *is* in the disconnected list, meaning everything
		 * is OK. */
		if (res == controller) { return SGFE_FALSE; }
		res = res->next;
	}

	/* NOTE(EimaMei): Something 100% went wrong. */
	/* TODO(EimaMei): Replace this with a panic message about the comment above. */
	SGFE_ASSERT(0);
#endif

	return SGFE_FALSE;
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
	*buttons_mask &= (SGFE_button)~SGFE_BIT(type);
	*out_button = type;

	return SGFE_TRUE;
}


SGFE_button SGFE_buttonGetMask(SGFE_controllerType type) {
	SGFE_ASSERT(type >= 0 && type < SGFE_controllerTypeCount);
	return SGFE_BUTTON_MASK_BITS_LUT[type];
}


const SGFE_axis* SGFE_controllerGetAxis(const SGFE_controller* controller,
		SGFE_axisType which) {
	SGFE_ASSERT(controller != NULL);
	#ifndef NDEBUG
	SGFE_axisType start, end;
	SGFE_controllerGetRangeAxis(controller->type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(which >= start && which <= end);
	#endif

	return &controller->axes[which];
}

const SGFE_pointer* SGFE_controllerGetPointer(const SGFE_controller* controller,
		SGFE_pointerType which) {
	SGFE_ASSERT(controller != NULL);
	#ifndef NDEBUG
	SGFE_pointerType start, end;
	SGFE_controllerGetRangePointer(controller->type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(which >= start && which <= end);
	#endif

	return &controller->pointers[which];
}

const SGFE_motion* SGFE_controllerGetMotion(const SGFE_controller* controller,
		SGFE_motionType which) {
	SGFE_ASSERT(controller != NULL);
	#ifndef NDEBUG
	SGFE_motionType start, end;
	SGFE_controllerGetRangeMotion(controller->type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(which >= start && which <= end);
	#endif

	return &controller->motions[which];
}

void SGFE_controllerGetRangeButton(SGFE_controllerType type,
		SGFE_buttonType* out_first,SGFE_buttonType* out_last) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	if (out_first) { *out_first = SGFE_BUTTON_RANGE_LUT[type][0]; }
	if (out_last)  { *out_last  = SGFE_BUTTON_RANGE_LUT[type][1]; }
}

void SGFE_controllerGetRangeAxis(SGFE_controllerType type,
		SGFE_axisType* out_first, SGFE_axisType* out_last) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	if (out_first) { *out_first = SGFE_AXIS_RANGE_LUT[type][0]; }
	if (out_last)  { *out_last  = SGFE_AXIS_RANGE_LUT[type][1]; }
}

void SGFE_controllerGetRangePointer(SGFE_controllerType type,
		SGFE_pointerType* out_first, SGFE_pointerType* out_last) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	if (out_first) { *out_first = SGFE_POINTER_RANGE_LUT[type][0]; }
	if (out_last)  { *out_last  = SGFE_POINTER_RANGE_LUT[type][1]; }
}

void SGFE_controllerGetRangeMotion(SGFE_controllerType type,
		SGFE_motionType* out_first, SGFE_motionType* out_last) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	if (out_first) { *out_first = SGFE_MOTION_RANGE_LUT[type][0]; }
	if (out_last)  { *out_last  = SGFE_MOTION_RANGE_LUT[type][1]; }
}


const char* SGFE_controllerGetName(SGFE_controllerType type) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	return SGFE_CONTROLLER_NAME_LUT[type];
}

const char* SGFE_controllerGetNameButton(SGFE_controllerType type,
		SGFE_buttonType button) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	#ifndef NDEBUG
	SGFE_buttonType start, end;
	SGFE_controllerGetRangeButton(type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(button >= start && button <= end);
	#endif

	return SGFE_controllerGetNameButton_platform(type, button);
}

const char* SGFE_controllerGetNameAxis(SGFE_controllerType type,
		SGFE_axisType axis) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	#ifndef NDEBUG
	SGFE_axisType start, end;
	SGFE_controllerGetRangeAxis(type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(axis >= start && axis <= end);
	#endif

	return SGFE_AXIS_NAME_LUT[axis];
}

const char* SGFE_controllerGetNamePointer(SGFE_controllerType type,
		SGFE_pointerType pointer) {
	SGFE_ASSERT_FMT(type >= 0 && type < SGFE_controllerTypeCount, "type = %i;", type);
	#ifndef NDEBUG
	SGFE_pointerType start, end;
	SGFE_controllerGetRangePointer(type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(pointer >= start && pointer <= end);
	#endif

	return SGFE_POINTER_NAME_LUT[pointer];
}

const char* SGFE_controllerGetNameMotion(SGFE_controllerType type,
		SGFE_motionType motion) {
	SGFE_ASSERT(type >= 0 && type < SGFE_controllerTypeCount);
	#ifndef NDEBUG
	SGFE_motionType start, end;
	SGFE_controllerGetRangeMotion(type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(motion >= start && motion <= end);
	#endif

	return SGFE_MOTION_NAME_LUT[motion];
}


SGFE_bool SGFE_controllerEnablePointer(SGFE_controller* controller, SGFE_motionType pointer,
		SGFE_bool enable) {
	SGFE_ASSERT(controller != NULL);
	SGFE_ASSERT(pointer >= 0 && pointer < SGFE_pointerTypeCount);

	#ifndef NDEBUG
	isize start, end;
	SGFE_controllerGetRangePointer(controller->type, &start, &end);
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

	#ifndef NDEBUG
	isize start, end;
	SGFE_controllerGetRangeMotion(controller->type, &start, &end);
	SGFE_ASSERT(start != -1 && end != -1);
	SGFE_ASSERT(motion >= start && motion <= end);
	#endif

	SGFE_bool res = SGFE_controllerEnableMotion_platform(controller, motion, SGFE_BOOL(enable));
	if (res) {
		controller->enabled_motions[motion] = SGFE_BOOL(enable);
	}

	return res;
}



#if defined(SGFE_OPENGL)

void SGFE_glHintsMakeWithDefaultSettings(SGFE_contextHintsGL* gl) {
	SGFE_ASSERT(gl != NULL);

	gl->profile = SGFE_glProfileCore;
	gl->major = 1;
	gl->minor = 0;

	gl->red = 8;
	gl->green = 8;
	gl->blue = 8;
	gl->alpha = 8;

	gl->depth = 24;
	gl->stencil = 0;

	gl->accum_red = 0;
	gl->accum_green = 0;
	gl->accum_blue = 0;
	gl->accum_alpha = 0;

	gl->samples = 0;
	gl->aux_buffers = 0;

	gl->is_double_buffered = SGFE_TRUE;
	gl->is_vsync = SGFE_TRUE;
	gl->is_software_renderer = SGFE_FALSE;
	gl->is_stereo = SGFE_FALSE;
	gl->is_SRGB = SGFE_FALSE;
	gl->is_robust = SGFE_FALSE;
	gl->is_debug = SGFE_FALSE;
	gl->has_no_errors = SGFE_FALSE;
	gl->flush_on_ctx_change = SGFE_FALSE;

	gl->shared_context = NULL;

	gl->screen = SGFE_screenPrimary;
}

void SGFE_glHintsAssert(const SGFE_contextHintsGL* gl) {
	SGFE_ASSERT_NOT_NULL(gl);
	SGFE_ASSERT_FMT(gl->profile >= 0 && gl->profile < SGFE_glProfileCount, "Invalid OpenGL profile. gl->profile = %i;", gl->profile);
	SGFE_ASSERT_NOT_NEG(gl->major);
	SGFE_ASSERT_NOT_NEG(gl->minor);

	SGFE_ASSERT_NOT_NEG(gl->red);
	SGFE_ASSERT_NOT_NEG(gl->green);
	SGFE_ASSERT_NOT_NEG(gl->blue);
	SGFE_ASSERT_NOT_NEG(gl->alpha);

	SGFE_ASSERT_NOT_NEG(gl->depth);
	SGFE_ASSERT_NOT_NEG(gl->stencil);

	SGFE_ASSERT_NOT_NEG(gl->accum_red);
	SGFE_ASSERT_NOT_NEG(gl->accum_green);
	SGFE_ASSERT_NOT_NEG(gl->accum_blue);
	SGFE_ASSERT_NOT_NEG(gl->accum_alpha);

	SGFE_ASSERT_NOT_NEG(gl->samples);
	SGFE_ASSERT_NOT_NEG(gl->downscale_width);
	SGFE_ASSERT_NOT_NEG(gl->downscale_height);
	SGFE_ASSERT_MSG(gl->samples == 0 || (gl->downscale_width == 0 && gl->downscale_height == 0), "You can only select to use _one_ of the anti-aliasing methods.");
	SGFE_ASSERT_MSG(gl->downscale_width == 0 || gl->samples == 0, "You can only select to use _one_ of the anti-aliasing methods.");
	SGFE_ASSERT_MSG(gl->downscale_height == 0 || gl->samples == 0, "You can only select to use _one_ of the anti-aliasing methods.");

	SGFE_ASSERT_NOT_NEG(gl->aux_buffers);

	SGFE_ASSERT_BOOL(gl->is_double_buffered);
	SGFE_ASSERT_BOOL(gl->is_vsync);
	SGFE_ASSERT_BOOL(gl->is_software_renderer);
	SGFE_ASSERT_BOOL(gl->is_stereo);
	SGFE_ASSERT_BOOL(gl->is_SRGB);
	SGFE_ASSERT_BOOL(gl->is_robust);
	SGFE_ASSERT_BOOL(gl->is_debug);
	SGFE_ASSERT_BOOL(gl->has_no_errors);
	SGFE_ASSERT_BOOL(gl->flush_on_ctx_change);

	SGFE_ASSERT_FMT(gl->screen >= 0 && gl->screen < SGFE_screenCount, "Invalid screen. gl->screen = %i;", gl->screen);
}

SGFE_contextHintsGL SGFE__GLOBAL_HINTS;
SGFE_bool SGFE__GLOBAL_HINTS_SET = SGFE_FALSE;

SGFE_contextHintsGL* SGFE_glHintsGetGlobal(void) {
	if (!SGFE__GLOBAL_HINTS_SET) {
		SGFE_glHintsMakeWithDefaultSettings(&SGFE__GLOBAL_HINTS);
		SGFE__GLOBAL_HINTS_SET = SGFE_TRUE;
	}

	return &SGFE__GLOBAL_HINTS;
}
void SGFE_glHintsSetGlobal(const SGFE_contextHintsGL* hints) {
	if (!SGFE__GLOBAL_HINTS_SET) {
		SGFE__GLOBAL_HINTS_SET = SGFE_TRUE;
	}
	SGFE__GLOBAL_HINTS = *hints;
}


SGFE_bool SGFE_windowCreateContextGL(SGFE_window* win, SGFE_videoMode mode) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_windowFreeContext(win);

	SGFE_contextHintsGL* hints = SGFE_glHintsGetGlobal();
	SGFE_glHintsAssert(hints);

	for (SGFE_screen screen = SGFE_screenPrimary; screen < SGFE_screenCount; screen += 1) {
		if (!SGFE_windowIsScreenEnabled(win, screen)) { continue; }

		SGFE_contextGL* gl = &win->ctx[screen].gl;
		hints->screen = screen;

		SGFE_bool res = SGFE_glCreateContext(gl, mode, hints);
		if (res == SGFE_FALSE) {
			SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorCreateContextBuffer);
		}

		SGFE_windowSetContextExGL(win, gl, screen);
	}

	SGFE_windowSetVisible(win, !SGFE_BOOL(win->flags & SGFE_windowFlagDontShow));
	win->flags |= SGFE_windowFlagOpenGL;
	return SGFE_TRUE;
}

SGFE_contextGL* SGFE_windowGetContextGL(SGFE_window* win) {
	SGFE_ASSERT(SGFE_windowGetContextType(win) == SGFE_contextTypeGL);
	return (SGFE_contextGL*)SGFE_windowGetContext(win);
}

SGFE_contextGL* SGFE_windowGetContextExGL(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT(SGFE_windowGetContextTypeEx(win, screen) == SGFE_contextTypeGL);
	return (SGFE_contextGL*)SGFE_windowGetContextEx(win, screen);
}

void SGFE_windowSetContextGL(SGFE_window* win, SGFE_contextGL* ctx) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_windowSetContextExGL(win, ctx, SGFE_windowGetCurrentScreen(win));
}

#endif



SGFE_bool SGFE_textInputSettingsMakeDefault(SGFE_textInputSettings* s, SGFE_bool null_terminated_strings) {
	SGFE_ASSERT_NOT_NULL(s);

	s->type = SGFE_textInputTypeStandard;
	s->password_mode = SGFE_textInputPasswordNone;
	s->flags = 0;
	s->dialog_button_count = -1;
	s->null_terminated_strings = null_terminated_strings;
	s->initial_text = NULL;
	s->hint_text = NULL;
	s->initial_text_len = (isize)(SIZE_MAX >> 1);
	s->hint_text_len = (isize)(SIZE_MAX >> 1);

	return SGFE_textInputSetPlatformFlags(s);
}


SGFE_bool SGFE_windowTextInputIsActive(const SGFE_window* win) {
	return SGFE_windowGetEventEnabled(win, SGFE_eventTextInput);
}



void SGFE_videoModeResolution(SGFE_videoMode mode, isize* out_width, isize* out_height) {
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	if (out_width)  {  *out_width = SGFE_VIDEO_RESOLUTION_LUT[mode][0]; }
	if (out_height) { *out_height = SGFE_VIDEO_RESOLUTION_LUT[mode][1]; }
}

isize SGFE_bufferFormatGetBytesPerPixel(SGFE_bufferFormat format) {
	SGFE_ASSERT(format >= 0 && format < SGFE_bufferFormatCount);
	return SGFE_FORMAT_BYTES_PER_PIXEL_LUT[format];
}


const char* SGFE_bufferFormatGetStr(SGFE_bufferFormat format) {
	SGFE_ASSERT(format >= 0 && format < SGFE_bufferFormatCount);
	return SGFE_PIXEL_FORMAT_NAME_LUT[format];
}

const char* SGFE_videoModeStr(SGFE_videoMode mode) {
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	return SGFE_VIDEO_MODE_NAME_LUT[mode];
}



u8* SGFE__fetchSwapBuffer(SGFE_contextBuffer* b) {
	#ifndef SGFE_BUFFER_NO_CONVERSION
	return SGFE_bufferConvertFramebufferToNative(b);
	#else
	return SGFE_bufferGetFramebuffer(b);
	#endif
}


#define SGFE_CALLBACK_DEFINE(return_type, name, member_name) \
	return_type name(SGFE_window* win, return_type func) { \
		SGFE_ASSERT_NOT_NULL(win); \
		\
		return_type previous_func = (return_type)win->callbacks.member_name; \
		win->callbacks.member_name = (void (*)(void))func; \
		return  (return_type)previous_func; \
	}

SGFE_CALLBACK_DEFINE(SGFE_deviceSleepProc, SGFE_windowSetDeviceSleepCallback, sleep)

SGFE_CALLBACK_DEFINE(SGFE_quitProc, SGFE_windowSetQuitCallback, quit)
SGFE_CALLBACK_DEFINE(SGFE_focusProc, SGFE_windowSetFocusCallback, focus)

SGFE_CALLBACK_DEFINE(SGFE_controllerProc, SGFE_windowSetControllerCallback, controller)
SGFE_CALLBACK_DEFINE(SGFE_controllerBatteryProc, SGFE_windowSetControllerBatteryCallback, controller_battery)

SGFE_CALLBACK_DEFINE(SGFE_buttonProc, SGFE_windowSetButtonCallback, button)
SGFE_CALLBACK_DEFINE(SGFE_axisProc, SGFE_windowSetAxisCallback, axis)
SGFE_CALLBACK_DEFINE(SGFE_pointerProc, SGFE_windowSetPointerCallback, pointer)
SGFE_CALLBACK_DEFINE(SGFE_motionProc, SGFE_windowSetMotionCallback, motion)

SGFE_CALLBACK_DEFINE(SGFE_textInputProc, SGFE_windowSetTextInputCallback, text_input)


SGFE_debugProc SGFE_setDebugCallback(SGFE_debugProc func, void* user_param) {
	SGFE_debugProc previous = SGFE_debugProcSrc;
	SGFE_debugProcSrc = func;
	SGFE__debugProcSrcUserParam = user_param;
	return previous;
}



void (SGFE_debugSendMsg)(void* ctx_ptr, SGFE_debugType type, isize code, const char* msg,
		const char* filename, isize line, const char* function) {
	SGFE_ASSERT(type >= 0 && type < SGFE_debugTypeCount);

	struct SGFE_debugContext ctx;
	ctx.source = SGFE_debugSourceApp;
	ctx.type = type;
	ctx.code = code;

	ctx.msg_len = msg ? SGFE_STRLEN(msg) : 0;
	ctx.msg = (const u8*)msg;

	ctx.filename = filename;
	ctx.line = line;
	ctx.function = function;

	ctx.ctx = ctx_ptr;

	SGFE_debugCallback(ctx);
}


const char* SGFE_debugSourceName(SGFE_debugSource source) {
	SGFE_ASSERT(source >= 0 && source < SGFE_debugSourceCount);

	static const char* NAME_LUT[SGFE_debugSourceCount] = {
		"SGFE_debugSourceAPI",
		"SGFE_debugSourcePlatformAPI",
		"SGFE_debugSourceSystem",
		"SGFE_debugSourceGL",
		"SGFE_debugSourceApp",
	};
	return NAME_LUT[source];
}

const char* SGFE_debugTypeName(SGFE_debugType type) {
	SGFE_ASSERT(type >= 0 && type < SGFE_debugTypeCount);

	static const char* NAME_LUT[SGFE_debugSourceCount] = {
		"SGFE_debugTypeError",
		"SGFE_debugTypeWarning",
		"SGFE_debugTypeInfo"
	};
	return NAME_LUT[type];
}


const char* SGFE_debugCodeGetName(SGFE_debugSource source, SGFE_debugType type, isize code) {
	SGFE_ASSERT(source >= 0 && source < SGFE_debugSourceCount);
	SGFE_ASSERT(type >= 0 && type < SGFE_debugTypeCount);

	switch (source) {
		case SGFE_debugSourceAPI: return SGFE_debugCodeAPIGetName(type, code);
		case SGFE_debugSourcePlatformAPI: return SGFE_debugSourcePlatformAPIGetName(type, code);
		case SGFE_debugSourceSystem: return SGFE_debugCodeSystemGetName(type, code);
		case SGFE_debugSourceGL: return SGFE_debugCodeGLGetName(code);
	}

	return "";
}

const char* SGFE_debugCodeGetDesc(SGFE_debugSource source, SGFE_debugType type, isize code) {
	SGFE_ASSERT(source >= 0 && source < SGFE_debugSourceCount);
	SGFE_ASSERT(type >= 0 && type < SGFE_debugTypeCount);

	switch (source) {
		case SGFE_debugSourceAPI: return SGFE_debugCodeAPIGetDesc(type, code);
		case SGFE_debugSourcePlatformAPI: return SGFE_debugSourcePlatformAPIGetDesc(type, code);
		case SGFE_debugSourceSystem: return SGFE_debugSourceSystemGetDesc(type, code);
		case SGFE_debugSourceGL: return SGFE_debugCodeGLGetDesc(code);
	}

	return "";
}


const char* SGFE_debugCodeAPIGetName(SGFE_debugType type, isize code) {
	SGFE_ASSERT(
		(type == SGFE_debugTypeError   && (code >= 0 && code < SGFE_errorCount))   ||
		(type == SGFE_debugTypeWarning && (code >= 0 && code < SGFE_warningCount)) ||
		(type == SGFE_debugTypeInfo    && (code >= 0 && code < SGFE_infoCount))
	);

	static const char* ERROR_LUT[SGFE_errorCount] = {
		"SGFE_errorOutOfMemory",

		"SGFE_errorEventQueue",
		"SGFE_errorMultipleAPIs",

		"SGFE_errorCreateContextBuffer",
		"SGFE_errorCreateContextGL",

		"SGFE_errorGLContextProfile",
		"SGFE_errorGLContextVersion",
		"SGFE_errorGLContextStencil",
		"SGFE_errorGLContextDepth",
		"SGFE_errorGLContextColor",
		"SGFE_errorGLContextSamples",
		"SGFE_errorGLContextDownscale"
	};

	static const char* WARNING_LUT[SGFE_warningCount] = {
		"SGFE_warningGLContextStencil",
		"SGFE_warningGLContextDepth",
		"SGFE_warningGLContextColor",
	};

	static const char* INFO_LUT[SGFE_infoCount] = {
		"SGFE_infoCreateContextBuffer",
		"SGFE_infoCreateContextGL",

		"SGFE_infoFreeContextBuffer",
		"SGFE_infoFreeContextGL",
	};

	static const char** ARR_LUT[] = {ERROR_LUT, WARNING_LUT, INFO_LUT};
	return ARR_LUT[type][code];
}

const char* SGFE_debugCodeAPIGetDesc(SGFE_debugType type, isize code) {
	SGFE_ASSERT(
		(type == SGFE_debugTypeError   && (code >= 0 && code < SGFE_errorCount))   ||
		(type == SGFE_debugTypeWarning && (code >= 0 && code < SGFE_warningCount)) ||
		(type == SGFE_debugTypeInfo    && (code >= 0 && code < SGFE_infoCount))
	);

	static const char* ERROR_LUT[SGFE_errorCount] = {
		"Ran out of memory",

		"Event queue limit 'SGFE_MAX_EVENTS' has been reached",
		"Multiple graphical APIs have been specified when you can only pick one",

		"Failed to create a buffer context",
		"Failed to create an OpenGL context",

		"OpenGL profile is not supported",
		"OpenGL version is not supported",
		"OpenGL stencil buffer size is not supported",
		"OpenGL depth buffer size is not supported",
		"OpenGL color buffer size is not supported",
		"OpenGL sample count is not supported",
		"OpenGL downscale count is not supported",
	};

	static const char* WARNING_LUT[SGFE_warningCount] = {
		"Specified OpenGL stencil buffer size is not supported, but a suitable one has been found instead",
		"Specified OpenGL depth buffer size is not supported but a suitable one has been found instead",
		"Specified OpenGL color buffer size is not supported but a suitable one has been found instead",
	};

	static const char* INFO_LUT[SGFE_infoCount] = {
		"Succesfully created a window",
		"Succesfully created a buffer context",
		"Succesfully created an OpenGL context",
	};

	static const char** ARR_LUT[] = {ERROR_LUT, WARNING_LUT, INFO_LUT};
	return ARR_LUT[type][code];
}


const char* SGFE_debugCodeGLGetName(isize code) {
	switch (code) {
		case 0x0500 /* GL_INVALID_ENUM                  */: return "GL_INVALID_ENUM";
		case 0x0501 /* GL_INVALID_VALUE                 */: return "GL_INVALID_VALUE";
		case 0x0502 /* GL_INVALID_OPERATION             */: return "GL_INVALID_OPERATION";
		case 0x0503 /* GL_STACK_OVERFLOW                */: return "GL_STACK_OVERFLOW";
		case 0x0504 /* GL_STACK_UNDERFLOW               */: return "GL_STACK_UNDERFLOW";
		case 0x0505 /* GL_OUT_OF_MEMORY                 */: return "GL_OUT_OF_MEMORY";
		case 0x0506 /* GL_INVALID_FRAMEBUFFER_OPERATION */: return "GL_INVALID_FRAMEBUFFER_OPERATION";
	}

	return "Unknown error";
}

const char* SGFE_debugCodeGLGetDesc(isize code) {
	switch (code) {
		case 0x0500 /* GL_INVALID_ENUM                  */: return "An invalid enumeration has been specified";
		case 0x0501 /* GL_INVALID_VALUE                 */: return "An invalid value has been specified";
		case 0x0502 /* GL_INVALID_OPERATION             */: return "The set of state is not legal for the parameters given to that command";
		case 0x0503 /* GL_STACK_OVERFLOW                */: return "A stack pushing operation cannot be done";
		case 0x0504 /* GL_STACK_UNDERFLOW               */: return "A stack popping operation cannot be done";
		case 0x0505 /* GL_OUT_OF_MEMORY                 */: return "Ran out of memory";
		case 0x0506 /* GL_INVALID_FRAMEBUFFER_OPERATION */: return "Tried accessing a framebuffer that is not complete";
	}

	return "Unknown error";
}



void (SGFE_debugSendAPI)(void* ctx_ptr, SGFE_debugType type, isize code,
		const char* filename, isize line, const char* function) {
	struct SGFE_debugContext ctx;
	ctx.source = SGFE_debugSourceAPI;
	ctx.type = type;
	ctx.code = code;

	ctx.msg_len = 0;
	ctx.msg = (const u8*)"";

	ctx.filename = filename;
	ctx.line = line;
	ctx.function = function;

	ctx.ctx = ctx_ptr;

	SGFE_debugCallback(ctx);
}

void (SGFE_debugSendPlatformAPI)(void* ctx_ptr, SGFE_debugType type, isize code,
		const char* filename, isize line, const char* function) {
	struct SGFE_debugContext ctx;
	ctx.source = SGFE_debugSourcePlatformAPI;
	ctx.type = type;
	ctx.code = code;

	ctx.msg_len = 0;
	ctx.msg = (const u8*)"";

	ctx.filename = filename;
	ctx.line = line;
	ctx.function = function;

	ctx.ctx = ctx_ptr;

	SGFE_debugCallback(ctx);
}

void (SGFE_debugSendSystem)(void* ctx_ptr, isize code, const char* filename, isize line,
		const char* function) {
	struct SGFE_debugContext ctx;
	ctx.source = SGFE_debugSourceSystem;
	ctx.type = SGFE_debugSystemGenerateType_platform(ctx_ptr, code);
	ctx.code = code;

	ctx.msg_len = 0;
	ctx.msg = (const u8*)"";

	ctx.filename = filename;
	ctx.line = line;
	ctx.function = function;

	ctx.ctx = ctx_ptr;

	SGFE_debugCallback(ctx);
}

#ifdef SGFE_OPENGL
SGFE_bool (SGFE_debugSendGL)(void* ctx_ptr, const char* filename, isize line, const char* function) {
	const char* msg = NULL;
	GLenum error;

	while ((error = glGetError()) != GL_NO_ERROR) {
		struct SGFE_debugContext ctx;
		ctx.source = SGFE_debugSourceGL;
		ctx.type = SGFE_debugTypeError;
		ctx.code = (isize)error;

		ctx.msg_len = SGFE_STRLEN(msg);
		ctx.msg = (const u8*)msg;

		ctx.filename = filename;
		ctx.line = line;
		ctx.function = function;

		ctx.ctx = ctx_ptr;

		SGFE_debugCallback(ctx);
	}

	return (msg == NULL);
}
#endif



#ifndef SGFE_CUSTOM_BACKEND
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

const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_bufferFormatCount] = {
	4, 3, 2, 2, 2
};




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

#ifdef SGFE_OPENGL
bool kygxInit(void);
void kygxExit(void);
#endif

void SGFE__aptHookCallback(APT_HookType hook, void* param);
void SGFE__bufferOnVblankCallback(void* buffer_ptr);

void SGFE__gspPresentFramebuffer(SGFE_contextBuffer* b, u8* buffer);
SGFE_bool SGFE__setBatteryState(SGFE_controller* controller);


void SGFE__aptHookCallback(APT_HookType hook, void* param) {
	SGFE_window* win = (SGFE_window*)param;

	SGFE_eventType type;
	switch ((int)hook) {
		case APTHOOK_ONSUSPEND:
		case APTHOOK_ONRESTORE: {
			type = (hook == APTHOOK_ONRESTORE) ? SGFE_eventFocusIn : SGFE_eventFocusOut;

			if (SGFE_windowGetEventEnabled(win, type)) {
				win->state.is_focused = (type == SGFE_eventFocusIn);

				SGFE_windowFocusCallback(win, win->state.is_focused);
				if (win->is_queueing_events) {
					SGFE_event event;
					event.type = type;
					SGFE_windowEventPush(win, &event);
				}
			}
		} break;

		case APTHOOK_ONSLEEP:
		case APTHOOK_ONWAKEUP: {
			type = (hook == APTHOOK_ONSLEEP) ? SGFE_eventDeviceSleep : SGFE_eventDeviceWakeup;

			if (SGFE_windowGetEventEnabled(win, type)) {
				win->state.is_sleeping = (type == SGFE_eventDeviceSleep);

				SGFE_windowDeviceSleepCallback(win, win->state.is_sleeping);
				if (win->is_queueing_events) {
					SGFE_event event;
					event.type = type;
					SGFE_windowEventPush(win, &event);
				}
			}
		} break;

		case APTHOOK_ONEXIT: {
			if (SGFE_windowGetEventEnabled(win, SGFE_eventQuit)) {
				win->state.should_quit = SGFE_TRUE;

				SGFE_windowQuitCallback(win);
				if (win->is_queueing_events) {
					SGFE_event event;
					event.type = SGFE_eventQuit;
					SGFE_windowEventPush(win, &event);
				}
			}
		} break;
	}
}

void SGFE__bufferOnVblankCallback(void* buffer_ptr) {
	SGFE_contextBuffer* b = (SGFE_contextBuffer*)buffer_ptr;

	b->src.frames_counter += b->frames;
	if (b->src.frames_counter >= 60) {
		b->src.frames_counter = 0;
		*b->src.run_gsp_loop = SGFE_FALSE;
	}
}

/* NOTE(EimaMei): Taken from libctru gfx.c */
void SGFE__gspPresentFramebuffer(SGFE_contextBuffer* b, u8* buffer) {
	u32 stride = GSP_SCREEN_WIDTH * (u32)SGFE_bufferFormatGetBytesPerPixel(b->format);
	u32 pixel_format = (u32)b->format | (1 << 8);

	u8* fb_b = buffer;
	if (b->screen == 0) {
		switch (b->mode) {
		case SGFE_videoMode2D: {
			pixel_format |= BIT(6);
		} break;

		case SGFE_videoMode3D: {
			pixel_format |= BIT(5);
			if (SGFE_platformGet3DSlider() > 0.0f) fb_b += b->src.size / 2;
		} break;
		}
	}

	gspPresentBuffer((u32)b->screen, (u32)b->current, buffer, fb_b, stride, pixel_format);
}

SGFE_bool SGFE__setBatteryState(SGFE_controller* controller) {
	static SGFE_bool first_time = SGFE_FALSE;

	u8 level;
	Result res = MCUHWC_GetBatteryLevel(&level);
	isize og_battery_proc = controller->battery_procent,
		  og_power_state = controller->power_state;

	if (res == 0) {
		if (level == 100) {
			/* NOTE(EimaMei): For whatever reason (hardware bug?) 'GetBatteryLevel'
			 * function returns '100' as the battery level for a few frames when
			 * physically plugging in a charing cable. Blocking the function the
			 * first time makes it so that the battery event doesn't fire while also
			 * not taking forever to actually report a battery event.  */
			if (!first_time) { return SGFE_FALSE; }
			first_time = SGFE_TRUE;
		}
		controller->battery_procent = level;
	}

	if (controller->battery_procent == 100) {
		controller->power_state = SGFE_powerStateFullyCharged;
	}
	else {
		res = PTMU_GetBatteryChargeState(&level);
		if (res == 0) {
			controller->power_state = level
				? SGFE_powerStateCharging
				: SGFE_powerStateOnBattery;
		}
	}

	return (og_battery_proc != controller->battery_procent) || (og_power_state != controller->power_state);
}




SGFE_screen SGFE_windowGetCurrentScreen_platform(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	return (win->flags & SGFE_windowFlagTopScreen) ? SGFE_screenTop : SGFE_screenBottom;
}

SGFE_bool SGFE_windowIsScreenEnabled_platform(SGFE_window* win, SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	return SGFE_BOOL(win->flags & SGFE_BIT(29 + screen));
}


SGFE_bool SGFE_windowMake_platform(SGFE_window* win) {
	if ((win->flags & SGFE_windowFlagDualScreen) == 0) {
		win->flags |= SGFE_windowFlagTopScreen;
	}

	Result res = gspInit();
	if (res != 0) { SGFE_debugSendSystem(win, res); return SGFE_FALSE; }

	#ifdef SGFE_OPENGL
	gfxInitDefault();
	SGFE_bool kygx_init = kygxInit();
	if (!kygx_init) {
		SGFE_debugSendPlatformAPI(win, SGFE_debugTypeError, SGFE_errorPlatformInitKYGX);
		return SGFE_FALSE;
	}
	#endif

	res = mcuHwcInit();
	if (res != 0) { SGFE_debugSendSystem(win, res); }

	res = ptmuInit();
	if (res != 0) { SGFE_debugSendSystem(win, res); }

	aptHook(&win->src.apt_hook, SGFE__aptHookCallback, win);

	SGFE_controller* controller = &win->controllers[0];
	controller->array_index = 0;
	controller->type = SGFE_controllerTypeStandard;
	controller->enabled_pointers[SGFE_pointerTouchscreen] = SGFE_TRUE;

	for (SGFE_axisType which = 0; which < SGFE_axisTypeCount; which += 1) {
		controller->axes[which].type = which;
		/* NOTE(EimaMei): I picked '40' as the deadzone based on how the CPAD bits
		 * are set if the value is larger than 40. (http://3dbrew.org/wiki/HID_Shared_Memory). */
		controller->axes[which].deadzone = (40.0f / 175.0f);
	}

	for (SGFE_pointerType which = 0; which < SGFE_pointerTypeCount; which += 1) {
		controller->pointers[which].type = which;
	}

	for (SGFE_motionType which = 0; which < SGFE_motionTypeCount; which += 1) {
		controller->motions[which].type = which;
	}

	SGFE__setBatteryState(controller);
	SGFE__controllerSetConnection(win, controller, SGFE_TRUE);

	return SGFE_TRUE;
}


void SGFE_windowClose_platform(SGFE_window* win) {
	gspExit();
	mcuHwcExit();
	ptmuExit();

	#ifdef SGFE_OPENGL
	kygxExit();
	gfxExit();
	#endif
	SGFE_UNUSED(win);
}


void SGFE_windowSetFlags(SGFE_window* win, SGFE_windowFlag flags) {
	SGFE_ASSERT_NOT_NULL(win);
	if (flags & SGFE_windowFlagTerminal) {
		SGFE_bool res = SGFE_windowInitTerminalOutput(win);
		if (res) { flags &= ~(SGFE_windowFlag)SGFE_windowFlagTerminal; }
	}

	win->flags = flags;
}


const SGFE_windowState* SGFE_windowPollEvents(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

	SGFE_bool continue_running = aptMainLoop();
	if (SGFE_windowGetEventEnabled(win, SGFE_eventQuit)) {
		if (!continue_running) {
			SGFE_windowSetShouldClose(win, SGFE_TRUE);
			SGFE_windowQuitCallback(win);

			if (win->is_queueing_events) {
				SGFE_event event;
				event.type = SGFE_eventQuit;
				SGFE_windowEventPush(win, &event);
			}
			return &win->state;
		}
	}

	hidScanInput();
	u32 held = hidKeysHeld();

	SGFE_controller* controller = &win->controllers[0];

	if (SGFE_windowGetEventEnabled(win, SGFE_eventButtonDown)) {
		controller->buttons_held = SGFE_platformButtonFromAPI(held);
		controller->buttons_down = SGFE_platformButtonFromAPI(hidKeysDown());

		SGFE__processCallbackAndEventQueue_ButtonDown(win, controller);
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventButtonUp)) {
		controller->buttons_up = SGFE_platformButtonFromAPI(hidKeysUp());

		SGFE__processCallbackAndEventQueue_ButtonUp(win, controller);
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventAxis)) {
		circlePosition cpad;
		hidCircleRead(&cpad);

		if (held & (KEY_CPAD_LEFT | KEY_CPAD_RIGHT)) {
			SGFE_axis* a = &controller->axes[SGFE_axisLeftX];
			a->value = (float)cpad.dx / 175.0f;
			SGFE__processCallbackAndEventQueue_Axis(win, controller, a);
		}

		if (held & (KEY_CPAD_UP | KEY_CPAD_DOWN)) {
			SGFE_axis* a = &controller->axes[SGFE_axisLeftY];
			a->value = (float)cpad.dy / 175.0f;
			SGFE__processCallbackAndEventQueue_Axis(win, controller, a);
		}
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventPointer)) {
		if (controller->enabled_pointers[SGFE_pointerTouchscreen] && (held & KEY_TOUCH)) {
			touchPosition touch;
			hidTouchRead(&touch);

			SGFE_pointer* p = &controller->pointers[SGFE_pointerTouchscreen];
			p->x = touch.px;
			p->y = touch.py;

			SGFE__processCallbackAndEventQueue_Pointer(win, controller, p);
		}
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventMotion)) {
		if (controller->enabled_motions[SGFE_motionAccelerometer]) {
			accelVector accel;
			hidAccelRead(&accel);

			SGFE_motion* m = &controller->motions[SGFE_motionAccelerometer];
			m->x = (float)accel.x / 512.0f * SGFE_STANDARD_GRAVITY;
			m->y = (float)accel.y / 512.0f * SGFE_STANDARD_GRAVITY;
			m->z = (float)accel.z / 512.0f * SGFE_STANDARD_GRAVITY;

			SGFE__processCallbackAndEventQueue_Motion(win, controller, m);
		}

		if (controller->enabled_motions[SGFE_motionGyroscope]) {
			angularRate gyro;
			hidGyroRead(&gyro);

			float coeff = 1.0f;
			HIDUSER_GetGyroscopeRawToDpsCoefficient(&coeff);

			SGFE_motion* m = &controller->motions[SGFE_motionGyroscope];
			m->x = (float)gyro.x * coeff * SGFE_PI / 180.0f;
			m->y = (float)gyro.y * coeff * SGFE_PI / 180.0f;
			m->z = (float)gyro.z * coeff * SGFE_PI / 180.0f;

			SGFE__processCallbackAndEventQueue_Motion(win, controller, m);
		}
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventControllerBattery)) {
		SGFE_bool update = SGFE__setBatteryState(controller);

		if (update) {
			SGFE_windowControllerBatteryCallback(win, controller, controller->power_state, controller->battery_procent);
			if (win->is_queueing_events) {
				SGFE_event event;
				event.type = SGFE_eventControllerBattery;
				event.battery.controller = controller;
				event.battery.state = controller->power_state;
				event.battery.battery_procent = controller->battery_procent;
				SGFE_windowEventPush(win, &event);
			}
		}
	}


	if (SGFE_windowGetEventEnabled(win, SGFE_eventTextInput) && win->src.swkbd_shared_mem != NULL) {
		SwkbdState* kb = win->src.keyboard;
		isize state = -1;

		while (state == -1) {
			aptLaunchLibraryApplet(
				APPID_SOFTWARE_KEYBOARD, kb, sizeof(*kb), win->src.swkbd_shared_mem_handle
			);

			switch (kb->result) {
				case SWKBD_PARENTAL_OK:
				case SWKBD_D0_CLICK:
				case SWKBD_D1_CLICK1:
				case SWKBD_D2_CLICK2: {
					state = 1;
				} break;

				/* NOTE(EimaMei): Window focus events are automatically handled thankfully. */
				case SWKBD_HOMEPRESSED:
				case SWKBD_D2_CLICK1: {
					state = 2;
				} break;

				case SWKBD_RESETPRESSED: {
					SGFE_windowSetShouldClose(win, SGFE_TRUE);
					aptSetChainloaderToSelf();
					state = 0;
				} break;

				case SWKBD_INVALID_INPUT: {
					SGFE_debugSendPlatformAPI(win, SGFE_debugTypeError, SGFE_errorPlatformTextInput);
					state = 0;
				} break;

				case SWKBD_OUTOFMEM: {
					SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorOutOfMemory);
					state = 0;
				} break;

				case SWKBD_POWERPRESSED:
				case SWKBD_D1_CLICK0:
				case SWKBD_D2_CLICK0: {
					state = 0;
				} break;

				case SWKBD_NONE:
				case SWKBD_BANNED_INPUT:
				case SWKBD_PARENTAL_FAIL:
					break;
			}
		}

		win->state.has_text_input = (state == 1);
		switch (state) {
			/* NOTE(EimaMei): No input, end text input.*/
			case 0: {
				SGFE_windowTextInputEnd(win);
			} break;

			/* NOTE(EimaMei): Succesful input, end text input.*/
			case 1: {
				win->state.text_len = utf16_to_utf8(
					win->state.text,
					(u16*)(void*)(win->src.swkbd_shared_mem) + kb->text_offset,
					kb->max_text_len
				);
				if (win->src.kb_null_terminated) {
					win->state.text[win->state.text_len] = '\0';
				}

				SGFE_windowTextInputCallback(win, win->state.text, win->state.text_len);
				if (win->is_queueing_events) {
					SGFE_event event;
					event.type = SGFE_eventTextInput;
					event.text.text = win->state.text;
					event.text.text_len = win->state.text_len;
					SGFE_windowEventPush(win, &event);
				}

				SGFE_windowTextInputEnd(win);
			} break;

			/* NOTE(EimaMei): No input possibly, but text input events should continue.*/
			default:
				break;
		}
	}
	else if (win->state.has_text_input) {
		win->state.has_text_input = SGFE_FALSE;
		win->state.text = NULL;
		win->state.text_len = 0;
	}

	return &win->state;
}


void SGFE_windowSetVisible(SGFE_window* win, SGFE_bool is_visible) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT_BOOL(is_visible);

	win->state.is_visible = is_visible;
	gspWaitForVBlank();
	GSPGPU_SetLcdForceBlack(!is_visible);
}



SGFE_button SGFE_buttonFromAPI(SGFE_controllerType type, u32 mask) {
	SGFE_ASSERT_FMT(type > 0 && type <= SGFE_controllerTypeCount, "type = %i;", type);
	return SGFE_platformButtonFromAPI(mask);
}

u32 SGFE_buttonToAPI(SGFE_controllerType type, SGFE_button button) {
	SGFE_ASSERT_FMT((button & ~SGFE_buttonGetMask(type)) == 0, "button = %i; ", button);
	return SGFE_platformButtonToAPI(button);
}


const char* SGFE_controllerGetNameButton_platform(SGFE_controllerType type,
		SGFE_buttonType button) {
	return SGFE_BUTTON_NAMES_3DS_LUT[button];
	SGFE_UNUSED(type);
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



SGFE_bufferFormat SGFE_bufferGetOptimalFormat(void) {
	return SGFE_bufferFormatBGR8;
}


SGFE_bool SGFE_bufferSetPlatformSettings(SGFE_contextBuffer* b) {
	SGFE_ASSERT_NOT_NULL(b);

	b->src.size = 0;
	b->src.frames_counter = 0;
	b->src.run_gsp_loop = NULL;
	return SGFE_TRUE;
}


SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	SGFE_ASSERT_MSG(
		SGFE_systemGetModel() != SGFE_systemModel2DS || b->mode != SGFE_videoModeWide,
		"Regular Nintendo 2DS consoles (excluding XLs) _do not_ support wide mode. "
		"Update your code to take this into account."
	);

	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);
	b->src.size = (u32)(width * height * SGFE_bufferFormatGetBytesPerPixel(b->format));
	b->current = 0;

	SGFE__gspPresentFramebuffer(b, (b->is_native) ? b->buffers[0] : b->buffers_native[0]);

	SGFE_debugSendAPI(b, SGFE_debugTypeInfo, SGFE_infoCreateContextBuffer);
	return SGFE_TRUE;
}

void SGFE_bufferFreeContext(SGFE_contextBuffer* b) {
	SGFE_bufferFreeFramebuffers(b);
	SGFE_debugSendAPI(b, SGFE_debugTypeInfo, SGFE_infoFreeContextBuffer);
}


SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	SGFE_bufferFreeFramebuffers(b);

	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);

	isize size = width * height * SGFE_bufferFormatGetBytesPerPixel(b->format);

	if (b->is_native) {
		u8* buffers = linearAlloc((size_t)(2 * size));
		if (buffers == NULL) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		b->buffers[0] = &buffers[0 * size];
		b->buffers[1] = &buffers[1 * size];
	}
	else {
		u8* buffers = SGFE_ALLOC((size_t)(2 * size));
		if (buffers == NULL) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		u8* native_buffers = linearAlloc((size_t)(2 * size));
		if (native_buffers == NULL) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		b->buffers[0] = &buffers[0 * size];
		b->buffers[1] = &buffers[1 * size];

		b->buffers_native[0] = &native_buffers[0 * size];
		b->buffers_native[1] = &native_buffers[1 * size];
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

		linearFree(b->buffers_native[0]);
		b->buffers_native[0] = NULL;
		b->buffers_native[1] = NULL;
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

	isize bpp = SGFE_bufferFormatGetBytesPerPixel(b->mode);
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


void SGFE_windowSwapBuffersBuffer(SGFE_window* win) {
	SGFE_bool wait[2];

	for (SGFE_screen screen = 0; screen < SGFE_screenCount; screen += 1) {
		if (win->current_type[screen] != SGFE_contextTypeBuffer) {
			wait[screen] = SGFE_FALSE;
			continue;
		}

		SGFE_contextBuffer* b = SGFE_windowGetContextExBuffer(win, screen);

		u8* buffer = SGFE__fetchSwapBuffer(b);
		GSPGPU_FlushDataCache(buffer, b->src.size);
		SGFE__gspPresentFramebuffer(b, buffer);

		b->current ^= b->is_double_buffered;

		if ((win->flags & (u32)(SGFE_windowFlagTopScreen << screen)) != 0) {
			gspSetEventCallback(GSPGPU_EVENT_VBlank0 + (GSPGPU_Event)screen, SGFE__bufferOnVblankCallback, b, SGFE_FALSE);
			b->src.run_gsp_loop = &wait[screen];
			wait[screen] = SGFE_TRUE;
		}
		else {
			wait[screen] = SGFE_FALSE;
		}
	}


	while (wait[0] || wait[1]) {
		gspWaitForAnyEvent();
	}

	gspSetEventCallback(GSPGPU_EVENT_VBlank0, NULL, NULL, SGFE_FALSE);
	gspSetEventCallback(GSPGPU_EVENT_VBlank1, NULL, NULL, SGFE_FALSE);
}



#ifdef SGFE_OPENGL

void* SGFE_glGetBoundContext(void) {
	return glassGetBoundContext();
}


isize SGFE_glGetVertexShaderType(void)   { return GL_SHADER_BINARY_PICA; }
isize SGFE_glGetFragmentShaderType(void) { return -1; }
isize SGFE_glGetGeometryShaderType(void) { return GL_GEOMETRY_SHADER_PICA; }


SGFE_bool SGFE_glCreateContext(SGFE_contextGL* gl, SGFE_videoMode mode, SGFE_contextHintsGL* hints) {
	SGFE_ASSERT(gl != NULL);
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	SGFE_ASSERT_MSG(
		SGFE_systemGetModel() != SGFE_systemModel2DS || mode != SGFE_videoModeWide,
		"Regular Nintendo 2DS consoles (excluding XLs) _do not_ support wide mode. "
		"Update your code to take this into account."
	);
	SGFE_glHintsAssert(hints);

	if (hints->profile != SGFE_glProfileES) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextProfile);
		return SGFE_FALSE;
	}

	if (hints->major != 1 || (hints->minor != 0 && hints->minor != 1)) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextVersion);
		return SGFE_FALSE;
	}

	if (hints->stencil > 8) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextStencil);
		return SGFE_FALSE;
	}
	else if (hints->stencil != 0 && hints->stencil != 8) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextStencil);
		hints->stencil = 8;
	}

	if (hints->depth > 24) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextDepth);
		return SGFE_FALSE;
	}
	else if (hints->depth != 0 && hints->depth != 24) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextDepth);
		hints->depth = 24;
	}

	if (hints->samples != 0) {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextSamples);
		return SGFE_FALSE;
	}

	isize r = hints->red,
		  g = hints->green,
		  b = hints->blue,
		  a = hints->alpha;

	GLenum internal_format;
	if (a == 0) {
		if (r <= 5 && g <= 6 && b <= 5) {
			internal_format = GL_RGB565;

			if (r != 5 || g != 6 || b != 5) {
				SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
				hints->red = 5;
				hints->green = 6;
				hints->blue = 5;
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGB8_OES;

			if (r != 8 || g != 8 || b != 8) {
				SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
				hints->red = hints->green = hints->blue = 8;
			}
		}
		else {
			SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextColor);
			return SGFE_FALSE;
		}
	}
	else if (a == 1) {
		if (r <= 5 && g <= 5 && b <= 5) {
			internal_format = GL_RGB5_A1;

			if (r != 5 || g != 5 || b != 5) {
				SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
				hints->red = hints->green = hints->blue = 5;
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
			internal_format = GL_RGBA8_OES;
			hints->red = hints->green = hints->blue = hints->alpha = 8;
		}
		else {
			SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextColor);
			return SGFE_FALSE;
		}
	}
	else if (a <= 4) {
		if (r <= 4 && g <= 4 && b <= 4) {
			internal_format = GL_RGBA4;

			if (r != 4 || g != 4 || b != 4 || a != 4) {
				SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
				hints->red = hints->green = hints->blue = hints->alpha = 4;
			}
		}
		else if (r <= 8 && g <= 8 && b <= 8) {
			SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
			internal_format = GL_RGBA8_OES;
			hints->red = hints->green = hints->blue = hints->alpha = 8;
		}
		else {
			SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextColor);
			return SGFE_FALSE;
		}
	}
	else if (a <= 8) {
		if (r <= 8 && g <= 8 && b <= 8) {
			internal_format = GL_RGBA8_OES;

			if (r != 8 || g != 8 || b != 8 || a != 8) {
				hints->red = hints->green = hints->blue = hints->alpha = 8;
				SGFE_debugSendAPI(gl, SGFE_debugTypeWarning, SGFE_warningGLContextColor);
			}
		}
		else {
			SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextColor);
			return SGFE_FALSE;
		}
	}
	else {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextColor);
		return SGFE_FALSE;
	}

	GLASSCtxParams param;
	param.version = GLASS_VERSION_ES_2;
	param.targetScreen = (GLASSScreen)hints->screen;
	param.targetSide = GLASS_SIDE_LEFT;
	param.GPUCmdList.mainBuffer = NULL;
	param.GPUCmdList.secondBuffer = NULL;
	param.GPUCmdList.capacity = 0;
	param.GPUCmdList.offset = 0;
	param.vsync = hints->is_vsync;
	param.horizontalFlip = false;
	param.flushAllLinearMem = true;

	if (hints->downscale_width == 0 && hints->downscale_height == 0) {
		param.downscale = GLASS_DOWNSCALE_NONE;
	}
	else if (hints->downscale_width == 0 && hints->downscale_height == 1) {
		param.downscale = GLASS_DOWNSCALE_1X2;
	}
	else if (hints->downscale_width == 1 && hints->downscale_height == 1) {
		param.downscale = GLASS_DOWNSCALE_2X2;
	}
	else {
		SGFE_debugSendAPI(gl, SGFE_debugTypeError, SGFE_errorGLContextDownscale);
		return SGFE_FALSE;
	}

	gl->ctx = glassCreateContext(&param);
	if (gl->ctx == NULL) {
		SGFE_debugSendPlatformAPI(NULL, SGFE_debugTypeError, SGFE_errorPlatformCreateGlassCtx);
		return SGFE_FALSE;
	}
	glassBindContext(gl->ctx);

	isize width, height;
	if (hints->screen == SGFE_screenTop) {
		SGFE_videoModeResolution(hints->is_stereo ? SGFE_videoMode3D : mode, &width, &height);
	}
	else {
		width  = 320;
		height = 240;
	}
	width  *= hints->downscale_width  + 1;
	height *= hints->downscale_height + 1;

	glGenRenderbuffers(1, &gl->renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, gl->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
	SGFE_debugSendGL(NULL);

	glGenFramebuffers(1, &gl->framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gl->framebuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, gl->renderbuffer);
	SGFE_debugSendGL(NULL);

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	SGFE_debugSendAPI(gl, SGFE_debugTypeInfo, SGFE_infoCreateContextGL);
	return SGFE_TRUE;
}

void SGFE_glFreeContext(SGFE_contextGL* gl) {
	SGFE_ASSERT(gl != NULL);
	if (gl->ctx == NULL) { return; }

	glassBindContext(gl->ctx);
	glDeleteRenderbuffers(1, &gl->renderbuffer);
	glDeleteFramebuffers(1, &gl->framebuffer);

	glassDestroyContext(gl->ctx);
	glassBindContext(NULL);
	gl->ctx = NULL;

	SGFE_debugSendAPI(gl, SGFE_debugTypeInfo, SGFE_infoFreeContextGL);
}


isize SGFE_glGetSwapInterval(const SGFE_contextGL* gl) {
	SGFE_ASSERT_NOT_NULL(gl);
	SGFE_ASSERT_NOT_NULL(gl->ctx);
	return (isize)glassHasVSync(gl->ctx);
}

void SGFE_glSetSwapInterval(SGFE_contextGL* gl, isize swap_interval) {
	SGFE_ASSERT_NOT_NULL(gl);
	SGFE_ASSERT_NOT_NULL(gl->ctx);
	SGFE_ASSERT_NOT_NEG(swap_interval);

	glassSetVSync(gl->ctx, SGFE_BOOL(swap_interval));
}


void SGFE_windowSwapBuffersGL(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

	GLASSCtx top = (SGFE_windowGetContextTypeEx(win, SGFE_screenTop) == SGFE_contextTypeGL)
		? SGFE_windowGetContextExGL(win, SGFE_screenTop)->ctx
		: NULL;
	GLASSCtx bottom = (SGFE_windowGetContextTypeEx(win, SGFE_screenBottom) == SGFE_contextTypeGL)
		? SGFE_windowGetContextExGL(win, SGFE_screenBottom)->ctx
		: NULL;

	glassSwapContextBuffers(top, bottom);
}


void SGFE_windowSetContextExGL(SGFE_window* win, SGFE_contextGL* gl, SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(screen >= 0 && screen < SGFE_screenCount);

	win->current[screen] = gl;
	win->current_type[screen] = gl ? SGFE_contextTypeGL : SGFE_contextTypeNone;
	glassBindContext(gl ? gl->ctx : NULL);
}

#endif



SGFE_bool SGFE_textInputSetPlatformFlags(SGFE_textInputSettings* s) {
	SGFE_ASSERT_NOT_NULL(s);
	s->platform_flags = SGFE_textInputPlatformFlagDarkenTopScreen |
						SGFE_textInputPlatformFlagPredictiveInput |
						SGFE_textInputPlatformFlagAllowHome       |
						SGFE_textInputPlatformFlagAllowReset      |
						SGFE_textInputPlatformFlagAllowPower;
	return SGFE_TRUE;
}


/* NOTE(EimaMei): This is a heavily modified version of libctru's swkbdInputText
 * to make it more fine-tuned for SGFE. The main rationale behind the rewrite is
 * adding functionality to omit the NULL byte as (for whatever reason) the original
 * writes one to the output buffer, which may not be what users want. */
SGFE_bool SGFE_windowTextInputBegin(SGFE_window* win, u8* buffer, isize buffer_len,
		SGFE_textInputSettings* s) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT_NOT_NULL(buffer);
	SGFE_ASSERT_NOT_NULL(s);
	SGFE_ASSERT_NOT_NEG(buffer_len);
	SGFE_windowTextInputEnd(win);

	SwkbdState* kb = SGFE_ALLOC(sizeof(SwkbdState));
	if (!kb) {
		SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorOutOfMemory);
		return SGFE_FALSE;
	}
	win->src.keyboard = kb;

	swkbdInit(
		kb,
		(SwkbdType)s->type,
		s->dialog_button_count != -1 ? s->dialog_button_count : 2,
		buffer_len - s->null_terminated_strings
	);

	SGFE_windowSource* src = &win->src;
	{
		kb->shared_memory_size =
			(((sizeof(u16) * (kb->max_text_len + 1) + 3) & (u32)~3)
			+ 0xFFF) & (u32)~0xFFF;

		src->swkbd_shared_mem = memalign(0x1000, kb->shared_memory_size);
		if (!src->swkbd_shared_mem) {
			SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		Result res = svcCreateMemoryBlock(
			&src->swkbd_shared_mem_handle,
			(u32)src->swkbd_shared_mem,
			kb->shared_memory_size,
			MEMPERM_READ | MEMPERM_WRITE, MEMPERM_READ | MEMPERM_WRITE
		);

		if (R_FAILED(res)) {
			free(src->swkbd_shared_mem);
			SGFE_debugSendAPI(win, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}
	}

	{
		SwkbdValidInput valid;
		if (s->flags  & SGFE_textInputFlagFixedLength) {
			valid = SWKBD_FIXEDLEN;
		}
		else {
			switch (s->flags & (SGFE_textInputFlagNotEmpty | SGFE_textInputFlagNotBlank)) {
				case 0: valid = SWKBD_ANYTHING; break;
				case SGFE_textInputFlagNotEmpty:
					valid = SWKBD_NOTEMPTY; break;
				case SGFE_textInputFlagNotBlank:
					valid = SWKBD_NOTBLANK; break;
				case SGFE_textInputFlagNotEmpty | SGFE_textInputFlagNotBlank:
					valid = SWKBD_NOTEMPTY_NOTBLANK; break;
			}
		}
		kb->valid_input = (int)valid;
	}

	{
		if (s->flags & SGFE_textInputFlagMultiline) {
			s->platform_flags |= SGFE_textInputPlatformFlagMultiline;
		}
		swkbdSetFeatures(kb, s->platform_flags);
	}

	if (s->initial_text) {
		kb->initial_text_offset = 0;

		isize len = utf8_to_utf16(
			(u16*)(void*)src->swkbd_shared_mem,
			(const u8*)s->initial_text,
			(buffer_len > s->initial_text_len) ? (usize)s->initial_text_len : (usize)buffer_len
		);
		*(((u16*)(void*)src->swkbd_shared_mem) + len) = '\0';
	}

	if (s->hint_text) {
		isize len = utf8_to_utf16(
			kb->hint_text,
			(const u8*)s->hint_text,
			(buffer_len > s->hint_text_len) ? (usize)s->hint_text_len : (usize)buffer_len
		);
		kb->hint_text[len] = '\0';
	}

	kb->password_mode = s->password_mode;
	kb->filter_flags = 0;
	kb->max_digits = 0;
	SGFE_MEMSET(kb->reserved, 0, sizeof(kb->reserved));

	SGFE_windowSetEventEnabled(win, SGFE_eventTextInput, SGFE_TRUE);
	win->state.text = buffer;
	win->state.text_len = 0;
	win->src.kb_null_terminated = s->null_terminated_strings;

	return SGFE_TRUE;
}

void SGFE_windowTextInputEnd(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	if (!SGFE_windowTextInputIsActive(win)) { return; }

	SGFE_windowSource* src = &win->src;
	svcCloseHandle(src->swkbd_shared_mem_handle);
	free(src->swkbd_shared_mem);
	SGFE_FREE(src->keyboard);

	src->swkbd_shared_mem_handle = 0;
	src->swkbd_shared_mem = NULL;
	src->keyboard = NULL;

	SGFE_windowSetEventEnabled(win, SGFE_eventTextInput, SGFE_FALSE);
}


SGFE_videoSignal SGFE_videoGetSignal(void) {
	return SGFE_videoSignalDigital;
}

SGFE_videoMode SGFE_videoGetOptimalMode(void) {
	return SGFE_videoMode2D;
}



SGFE_systemModel SGFE_systemGetModel(void) {
	u8 model;
	Result res = CFGU_GetSystemModel(&model);
	if (res != 0) { return SGFE_systemModelUnknown; }

	return (model <= SGFE_systemModelN2DSXL) ? (model - 1) : SGFE_systemModelUnknown;
}

SGFE_systemRegion SGFE_systemGetRegion(void) {
	u8 region;
	Result res = CFGU_SecureInfoGetRegion(&region);
	if (res != 0) { return SGFE_systemRegionUnknown; }

	switch ((CFG_Region)region) {
		case CFG_REGION_JPN: return SGFE_systemRegionJapan;
		case CFG_REGION_USA: return SGFE_systemRegionUS;
		case CFG_REGION_EUR: return SGFE_systemRegionEurope;
		case CFG_REGION_AUS: return SGFE_systemRegionAustralia;
		case CFG_REGION_CHN: return SGFE_systemRegionChina;
		case CFG_REGION_KOR: return SGFE_systemRegionKorea;
		case CFG_REGION_TWN: return SGFE_systemRegionTaiwan;
		default: return SGFE_systemRegionNotInTheList;
	}
}

SGFE_systemLanguage SGFE_systemGetLanguage(void) {
	u8 region;
	Result res = CFGU_GetSystemLanguage(&region);
	if (res != 0) { return SGFE_systemLanguageUnknown; }

	switch ((CFG_Language)region) {
		case CFG_LANGUAGE_JP: return SGFE_systemLanguageJapanese;
		case CFG_LANGUAGE_EN: return SGFE_systemLanguageEnglish;
		case CFG_LANGUAGE_FR: return SGFE_systemLanguageFrench;
		case CFG_LANGUAGE_DE: return SGFE_systemLanguageGerman;
		case CFG_LANGUAGE_IT: return SGFE_systemLanguageItalian;
		case CFG_LANGUAGE_ES: return SGFE_systemLanguageSpanish;
		case CFG_LANGUAGE_ZH: return SGFE_systemLanguageChinese;
		case CFG_LANGUAGE_KO: return SGFE_systemLanguageKorean;
		case CFG_LANGUAGE_NL: return SGFE_systemLanguageDutch;
		case CFG_LANGUAGE_PT: return SGFE_systemLanguagePortuguese;
		case CFG_LANGUAGE_RU: return SGFE_systemLanguageRussian;
		case CFG_LANGUAGE_TW: return SGFE_systemLanguageTaiwanese;
		case CFG_LANGUAGE_DEFAULT:
		default: return SGFE_systemLanguageNotInTheList;
	}
}



i64 SGFE_platformGetTimeFromTicks(u64 ticks) {
	osTimeRef_s ref = osGetTimeRef();

	ticks -= ref.value_tick;
	u64 clock_hz  = (u64)ref.sysclock_hz;
	u64 seconds = (ticks / clock_hz + (u64)ref.drift_ms * 1000000) - ((u64)1000000000 * 60 * 60 * 24 * 25567);
	u64 rem_cycles = ticks % clock_hz;

	return (i64)(seconds * 1000000000 + (rem_cycles * 1000000000) / clock_hz);
}

u64 SGFE_platformGetTicks(void) {
	return svcGetSystemTick();
}

u64 SGFE_platformGetClockSpeed(void) {
	osTimeRef_s ref = osGetTimeRef();
	return (u64)ref.sysclock_hz;
}


void SGFE_platformWaitForVBlank(void) {
	gspWaitForVBlank();
}

SGFE_bool SGFE_platformInitTerminalOutput(SGFE_contextBuffer* b) {
	/* TODO(EimaMei): Remove this entire function and replace it with a helper library
	 * and function. */
	/* NOTE(EimaMei): Taken from libctru console.c */
	static SGFE_bool console_initialized = SGFE_FALSE;
	SGFE_ASSERT(b != NULL);

	SGFE_bufferSetNative(b, SGFE_TRUE);
	SGFE_bufferSetFormat(b, SGFE_bufferFormatRGB565);
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
	/*if ((win->flags & SGFE_windowFlagDualScreen) == SGFE_windowFlagDualScreen) {
		win->current ^= 1;
	}*/

	consoleClear();
	return SGFE_TRUE;
}


SGFE_bool SGFE_platformHasSoftwareKeybord(void) {
	return SGFE_TRUE;
}


SGFE_button SGFE_platformButtonFromAPI(u32 mask) {
	#define SGFE__BUTTONS_BITS (u32)(0xFFF)
	#define SGFE__CSTICK_BITS (u32)(KEY_CSTICK_RIGHT | KEY_CSTICK_LEFT | KEY_CSTICK_UP | KEY_CSTICK_DOWN)
	#define SGFE__ZL_ZR_BITS (u32)(KEY_ZL | KEY_ZR)

	return
		((mask & SGFE__CSTICK_BITS) >> 11) |
		((mask & SGFE__ZL_ZR_BITS) >> 2) |
		((mask & SGFE__BUTTONS_BITS));

	#undef SGFE__BUTTONS_BITS
	#undef SGFE__CSTICK_BITS
	#undef SGFE__ZL_ZR_BITS
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
SGFE_bool SGFE_platformRotateScreenGL(GLuint shader_program, const char* mat4_uniform_name) {
	static const float deg90_rotation_matrix[4][4] = {
		{ 0.0f,  1.0f, 0.0f, 0.0f },
		{-1.0f,  0.0f, 0.0f, 0.0f },
		{ 0.0f,  0.0f, 1.0f, 0.0f },
		{ 0.0f,  0.0f, 0.0f, 1.0f }
	};

	GLint uniform = glGetUniformLocation(shader_program, mat4_uniform_name);
	if (uniform == -1) {
		SGFE_debugSendGL(NULL);
		return SGFE_FALSE;
	}

	glUniformMatrix4fv(uniform, 1, GL_FALSE, (const float*)deg90_rotation_matrix);
	return SGFE_debugSendGL(NULL);
}
#endif



const char* SGFE_debugSourcePlatformAPIGetName(SGFE_debugType type, isize code) {
	SGFE_ASSERT(
		(type == SGFE_debugTypeError   && (code >= 0 && code < SGFE_errorPlatformCount))   ||
		(type == SGFE_debugTypeWarning && (code >= 0 && code < SGFE_warningPlatformCount)) ||
		(type == SGFE_debugTypeInfo    && (code >= 0 && code < SGFE_infoPlatformCount))
	);

	static const char* ERROR_LUT[SGFE_errorPlatformCount] = {
		"SGFE_errorPlatformInitKYGX",
		"SGFE_errorPlatformCreateGlassCtx",
		"SGFE_errorPlatformTextInput"
	};

	return ERROR_LUT[code];
}

const char* SGFE_debugSourcePlatformAPIGetDesc(SGFE_debugType type, isize code) {
	SGFE_ASSERT(
		(type == SGFE_debugTypeError   && (code >= 0 && code < SGFE_errorPlatformCount))   ||
		(type == SGFE_debugTypeWarning && (code >= 0 && code < SGFE_warningPlatformCount)) ||
		(type == SGFE_debugTypeInfo    && (code >= 0 && code < SGFE_infoPlatformCount))
	);

	static const char* ERROR_LUT[SGFE_errorPlatformCount] = {
		"Failed to initialize KYGX",
		"Failed to create GLASS context",
		"Something went wrong with text input. Possibly a platform API bug."
	};

	return ERROR_LUT[code];
}


const char* SGFE_debugCodeSystemGetName(SGFE_debugType type, isize code) {
	switch (R_DESCRIPTION(code)) {
		case RD_SUCCESS:              return "RD_SUCCESS";
		case RD_INVALID_RESULT_VALUE: return "RD_INVALID_RESULT_VALUE";
		case RD_TIMEOUT:              return "RD_TIMEOUT";
		case RD_OUT_OF_RANGE:         return "RD_OUT_OF_RANGE";
		case RD_ALREADY_EXISTS:       return "RD_ALREADY_EXISTS";
		case RD_CANCEL_REQUESTED:     return "RD_CANCEL_REQUESTED";
		case RD_NOT_FOUND:            return "RD_NOT_FOUND";
		case RD_ALREADY_INITIALIZED:  return "RD_ALREADY_INITIALIZED";
		case RD_NOT_INITIALIZED:      return "RD_NOT_INITIALIZED";
		case RD_INVALID_HANDLE:       return "RD_INVALID_HANDLE";
		case RD_INVALID_POINTER:      return "RD_INVALID_POINTER";
		case RD_INVALID_ADDRESS:      return "RD_INVALID_ADDRESS";
		case RD_NOT_IMPLEMENTED:      return "RD_NOT_IMPLEMENTED";
		case RD_OUT_OF_MEMORY:        return "RD_OUT_OF_MEMORY";
		case RD_MISALIGNED_SIZE:      return "RD_MISALIGNED_SIZE";
		case RD_MISALIGNED_ADDRESS:   return "RD_MISALIGNED_ADDRESS";
		case RD_BUSY:                 return "RD_BUSY";
		case RD_NO_DATA:              return "RD_NO_DATA";
		case RD_INVALID_COMBINATION:  return "RD_INVALID_COMBINATION";
		case RD_INVALID_ENUM_VALUE:   return "RD_INVALID_ENUM_VALUE";
		case RD_INVALID_SIZE:         return "RD_INVALID_SIZE";
		case RD_ALREADY_DONE:         return "RD_ALREADY_DONE";
		case RD_NOT_AUTHORIZED:       return "RD_NOT_AUTHORIZED";
		case RD_TOO_LARGE:            return "RD_TOO_LARGE";
		case RD_INVALID_SELECTION:    return "RD_INVALID_SELECTION";
	}
	return "Unknown";
	SGFE_UNUSED(type);
}

const char* SGFE_debugSourceSystemGetDesc(SGFE_debugType type, isize code) {
	return osStrError(code);
	SGFE_UNUSED(type);
}

SGFE_debugType SGFE_debugSystemGenerateType_platform(void* ctx_ptr, isize code) {
	switch (R_LEVEL(code)) {
		case RL_FATAL: return SGFE_debugTypeError;
		case RL_INFO:  return SGFE_debugTypeInfo;
	}
	return SGFE_debugTypeWarning;
	SGFE_UNUSED(ctx_ptr);
}


#endif /* SGFE_3DS */




#ifdef SGFE_WII



void SGFE_windowSwapBuffers(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

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


SGFE_bool SGFE_platformInitTerminalOutput(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);

	if (win->current == NULL) {
		SGFE_bool res = SGFE_windowCreateContextBuffer(win, SGFE_videoGetOptimalMode(), SGFE_bufferFormatYUV, SGFE_TRUE);

		if (res == SGFE_FALSE) {
			return SGFE_FALSE;
		}
	}

	SGFE_contextBuffer* b = SGFE_windowGetCurrentBuffer(win);
	SGFE_bufferSetNative(b, SGFE_TRUE);
	SGFE_bufferSetFormat(b, SGFE_bufferFormatYUV);
	SGFE_bufferSetDoubleBuffered(b, SGFE_FALSE);

	console_init(
		SGFE_bufferGetFramebuffer(b),
		0, 0, b->gx_video_mode->fbWidth, b->gx_video_mode->xfbHeight,
		b->gx_video_mode->fbWidth * VI_DISPLAY_PIX_SZ
	);

	return SGFE_TRUE;
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


#endif /* ifdef SGFE_WII */
#endif /* ifndef SGFE_CUSTOM_BACKEND */

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
