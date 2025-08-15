#define SGFE_CUSTOM_BACKEND

/* === MANDATORY FIELDS TO FILL OUT === */
#ifndef SGFE_MAX_CONTROLLERS
#define SGFE_MAX_CONTROLLERS 1
#endif

#define SGFE_HAS_OPENGL              1
#define SGFE_HAS_MULTIPLE_SCREENS    1
#define SGFE_VBLANK_RATE            60


#ifndef SGFE_ENUM
	#define SGFE_ENUM(type, name) type name; enum
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
	typedef ptrdiff_t isize;
	#define SGFE_DEFINE_TYPE_SIZE
#endif

/* === ENUMERATOR TYPES === */
/* NOTE(EimaMei): Inside the SGFE header file there is always going to be platform
 * specific enumerator values that have to be defined for each backend. Enumerator
 * values that are still present in this template are cross-platform and cannot
 * be removed (except for 'BlankEnumValueSoThatTheCompilerWouldStopComplaining').
 *
 * The '#ifdef SGFE_CUSTOM_BACKEND' scope denotes where the platform specific values
 * should be in relation to the cross-platform values. */

#ifdef SGFE_CUSTOM_BACKEND

typedef SGFE_ENUM(isize, SGFE_screen) {
	SGFE_screenPrimary,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_screenCount
};

typedef SGFE_ENUM(isize, SGFE_videoMode) {
	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_videoModeCount
};

typedef SGFE_ENUM(isize, SGFE_pixelFormat) {
	SGFE_pixelFormatRGBA8,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_pixelFormatCount,
};


typedef SGFE_ENUM(isize, SGFE_controllerType) {
	SGFE_controllerTypeStandard = 0,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_controllerTypeCount,
};

typedef SGFE_ENUM(isize, SGFE_buttonType) {
	SGFE_buttonInvalid = -1,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif
};

typedef SGFE_ENUM(u32, SGFE_button) {
	#ifdef SGFE_CUSTOM_BACKEND
	BlankEnumValueSoThatTheCompilerWouldStopComplaining,
	/* ... */
	#endif
};

typedef SGFE_ENUM(isize, SGFE_axisType) {
	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_axisTypeCount,
};

typedef SGFE_ENUM(isize, SGFE_pointerType) {
	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_pointerTypeCount,
};

typedef SGFE_ENUM(isize, SGFE_motionType) {
	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_motionTypeCount,
};

typedef SGFE_ENUM(isize, SGFE_systemModel) {
	SGFE_systemModelUnknown,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_systemModelCount,
};


typedef SGFE_ENUM(u32, SGFE_textInputPlatformFlag) {
	/* ... */
};

#endif /* SGFE_CUSTOM_BACKEND */

/* === STRUCTURE TYPES === */

#ifdef SGFE_CUSTOM_BACKEND

struct SGFE_contextBufferSource {
	/* ... */
	u8 blank;
};

#ifdef SGFE_OPENGL
struct SGFE_contextGL {
	/* ... */
};
#endif

struct SGFE_windowSource {
	/* ... */
};

#endif /* SGFE_CUSTOM_BACKEND */


#include <SGFE.h>


#ifdef SGFE_IMPLEMENTATION

const SGFE_button SGFE_BUTTON_MASK_BITS_LUT[SGFE_controllerTypeCount] = {
	/* ... */
};

const isize SGFE_BUTTON_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	/* ... */
};
const isize SGFE_AXIS_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	/* {..., ...} */
};
const isize SGFE_POINTER_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	/* {..., ...} */
};
const isize SGFE_MOTION_RANGE_LUT[SGFE_controllerTypeCount][2] = {
	/* {..., ...} */
};

const char* SGFE_CONTROLLER_NAME_LUT[SGFE_controllerTypeCount] = {
	/* "..." */
};
#if 0
/* NOTE(EimaMei): You need to define your own lookup-table for each controller type.
 * as well as the SGFE_controllerGetNameButton_platform() function. */
const char* SGFE_BUTTON_NAMES_<CONTROLLER_TYPE_NAME_HERE>_LUT[SGFE_buttonCount_Nunchuk] = {
	/* "..." */
};
#endif
const char* SGFE_AXIS_NAME_LUT[SGFE_axisTypeCount] = {
	/* "..." */
};
const char* SGFE_POINTER_NAME_LUT[SGFE_pointerTypeCount] = {
	/* "..." */
};
const char* SGFE_MOTION_NAME_LUT[SGFE_motionTypeCount] = {
	/* "..." */
};

const isize SGFE_VIDEO_RESOLUTION_LUT[SGFE_videoModeCount][2] = {
	/* {..., ...} */
};

const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_pixelFormatCount] = {
	4, /* ... */
};

const char* SGFE_VIDEO_MODE_NAME_LUT[SGFE_videoModeCount] = {
	/* "..." */
};

const char* SGFE_PIXEL_FORMAT_NAME_LUT[SGFE_pixelFormatCount] = {
	"SGFE_pixelFormatRGBA8", /* "..." */
};




/* NOTE(EimaMei): If 'SGFE_HAS_MULTIPLE_SCREENS' is zero for the backend, you do
 * not have to define these functions. */
#if SGFE_HAS_MULTIPLE_SCREENS != 0
SGFE_screen SGFE_windowGetCurrentScreen_platform(SGFE_window* win) {
	/* NOTE(EimaMei): 'SGFE_windowGetCurrentScreen' already asserts that the window
	 * cannot be NULL.*/
	#warning "Warning to notify that this function hasn't been implemented yet."
}

SGFE_bool SGFE_windowIsScreenEnabled_platform(SGFE_window* win, SGFE_screen screen) {
	/* NOTE(EimaMei): 'SGFE_windowIsScreenEnabled' already asserts that the window
	 * cannot be NULL.*/
	#warning "Warning to notify that this function hasn't been implemented yet."
}
#endif


SGFE_bool SGFE_windowMake_platform(SGFE_window* win) {
	/* NOTE(EimaMei): 'SGFE_windowMake' already asserts that the window cannot be
	 * NULL.*/
	#warning "Warning to notify that this function hasn't been implemented yet."
}


void SGFE_windowClose_platform(SGFE_window* win) {
	/* NOTE(EimaMei): 'SGFE_windowClose' already asserts that the window cannot be
	 * NULL.*/
	#warning "Warning to notify that this function hasn't been implemented yet."
}


void SGFE_windowSetFlags(SGFE_window* win, SGFE_windowFlag flags) {
	SGFE_ASSERT(win != NULL);
	if (flags & SGFE_windowFlagTerminal) {
		SGFE_bool res = SGFE_windowInitTerminalOutput(win);
		if (res) { flags &= ~(SGFE_windowFlag)SGFE_windowFlagTerminal; }
	}

	#warning "Warning to notify that this function hasn't been implemented yet."
	win->flags = flags;
}


const SGFE_windowState* SGFE_windowPollEvents(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."

	if (SGFE_windowGetEventEnabled(win, SGFE_eventQuit)) {
		if (0) {
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

	/* ... */

	if (SGFE_windowGetEventEnabled(win, SGFE_eventButtonDown)) {
		/* controller->buttons_held = ...; */
		/* controller->buttons_down = ...; */

		/* SGFE__processCallbackAndEventQueue_ButtonDown(win, controller); */
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventButtonUp)) {
		/* controller->buttons_up = ... ; */

		/* SGFE__processCallbackAndEventQueue_ButtonUp(win, controller); */
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventAxis)) {
		/* ... */
		if (1) {
			/* SGFE__processCallbackAndEventQueue_Axis(win, controller, axis_ptr); */
		}
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventPointer)) {
		if (0 /* controller->enabled_pointers[pointer_type] */ ) {
			if (1) {
				/* SGFE__processCallbackAndEventQueue_Pointer(win, controller, pointer_ptr); */
			}
		}
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventMotion)) {
		if (0 /* controller->enabled_pointers[motion_type] */ ) {
			if (1) {
				/* SGFE__processCallbackAndEventQueue_Pointer(win, controller, motion_ptr); */
			}
		}
	}

	if (SGFE_windowGetEventEnabled(win, SGFE_eventControllerBattery)) {
		/* win->state.is_battery_updated = ...; */

		#if 0
		if (win->state.is_battery_updated) {
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
		#endif
	} else if (win->state.is_battery_updated) {
		win->state.is_battery_updated = SGFE_FALSE;
	}


	if (SGFE_windowGetEventEnabled(win, SGFE_eventTextInput)) {
		/* win->state.has_text_input = ...; */
		/* win->state.text = ...; */
		/* win->state.text_len = ...; */

		#if 0
		if (win->state.has_text_input)
			SGFE_windowTextInputCallback(win, win->state.text, win->state.text_len);
			if (win->is_queueing_events) {
				SGFE_event event;
				event.type = SGFE_eventTextInput;
				event.text.text = win->state.text;
				event.text.text_len = win->state.text_len;
				SGFE_windowEventPush(win, &event);
			}

			SGFE_windowTextInputEnd(win);
		}
		#endif
	}
	else if (win->state.has_text_input) {
		win->state.has_text_input = SGFE_FALSE;
		win->state.text = NULL;
		win->state.text_len = 0;
	}

	return &win->state;
}


void SGFE_windowSetVisible(SGFE_window* win, SGFE_bool is_visible) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT_BOOL(is_visible);

	win->state.is_visible = is_visible;
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): Here SGFE should turn on/off the screen using a platform-specific
	 * function. */
	/* ... */
}


SGFE_button SGFE_buttonFromAPI(SGFE_controllerType type, u32 mask) {
	SGFE_ASSERT(type > 0 && type <= SGFE_controllerTypeCount);
	#warning "Warning to notify that this function hasn't been implemented yet."
}

u32 SGFE_buttonToAPI(SGFE_controllerType type, SGFE_button button) {
	SGFE_ASSERT((button & ~SGFE_buttonGetMask(type)) == 0);
	#warning "Warning to notify that this function hasn't been implemented yet."
}


const char* SGFE_controllerGetNameButton_platform(SGFE_controllerType,
		SGFE_buttonType button) {
	/* NOTE(EimaMei): 'SGFE_controllerGetNameButton' already asserts that the
	 * controller type is valid and that the button is valid for the controller
	 * type. */
	#warning "Warning to notify that this function hasn't been implemented yet."
}


SGFE_bool SGFE_controllerEnablePointer_platform(SGFE_controller* controller,
		SGFE_pointerType pointer, SGFE_bool enable) {
	/* NOTE(EimaMei): 'SGFE_controllerEnablePointer' already asserts that the
	 * controller cannot be NULL, that motion is valid for the controller and that
	 * the boolean is either a one or zero. */
	#warning "Warning to notify that this function hasn't been implemented yet."
}

SGFE_bool SGFE_controllerEnableMotion_platform(SGFE_controller* controller,
		SGFE_motionType motion, SGFE_bool enable) {
	/* NOTE(EimaMei): 'SGFE_controllerEnableMotion' already asserts that the
	 * controller cannot be NULL, that motion is valid for the controller and that
	 * the boolean is either a one or zero. */
	#warning "Warning to notify that this function hasn't been implemented yet."
}



SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."
}

void SGFE_bufferFreeContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."
}


SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	SGFE_ASSERT(b != NULL);
	SGFE_bufferFreeFramebuffers(b);

	#warning "Warning to notify that this function hasn't been implemented yet."

	if (b->is_native) {
		/* ... */
	}
	else {
		/* ... */
	}

	b->is_buffer_allocated = SGFE_TRUE;
	return SGFE_TRUE;
}

SGFE_bool SGFE_bufferFreeFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	if (b->buffers[0] == NULL || !b->is_buffer_allocated) { return SGFE_FALSE; }

	#warning "Warning to notify that this function hasn't been implemented yet."

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

	/* ... */
	#warning "Warning to notify that this function hasn't been implemented yet."

	return dst;
}


void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."

	#if 0
	/* Depending on the platform: */
	SGFE_videoModeResolution(b->mode, out_width, out_height);
	/* or */

	if (out_width)  {  *out_width = /* ... */; }
	if (out_height) { *out_height = /* ... */; }
	#endif
}



#ifdef SGFE_OPENGL

void* SGFE_glGetBoundContext(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}


SGFE_bool SGFE_glCreateContext(SGFE_contextGL* gl, SGFE_videoMode mode, SGFE_contextHintsGL* hints) {
	SGFE_ASSERT(gl != NULL);
	SGFE_ASSERT(mode >= 0 && mode < SGFE_videoModeCount);
	SGFE_glHintsAssert(hints);

	#warning "Warning to notify that this function hasn't been implemented yet."
	return SGFE_TRUE;
}

void SGFE_glFreeContext(SGFE_contextGL* gl) {
	SGFE_ASSERT(gl != NULL);
	/* if (gl->ctx == NULL) { return; } */

	#warning "Warning to notify that this function hasn't been implemented yet."
	/* gl->ctx = NULL; */
}



isize SGFE_glGetSwapInterval(const SGFE_contextGL* gl) {
	SGFE_ASSERT_NOT_NULL(gl);
	/* SGFE_ASSERT_NOT_NULL(gl->ctx); */
	#warning "Warning to notify that this function hasn't been implemented yet."
}

void SGFE_glSetSwapInterval(SGFE_contextGL* gl, isize swap_interval) {
	SGFE_ASSERT_NOT_NULL(gl);
	/* SGFE_ASSERT_NOT_NULL(gl->ctx); */
	#warning "Warning to notify that this function hasn't been implemented yet."
}


void SGFE_windowSwapBuffersGL(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."
}


void SGFE_windowSetContextExGL(SGFE_window* win, SGFE_contextGL* gl, SGFE_screen screen) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT(screen >= 0 && screen < SGFE_screenCount);

	win->current[screen] = gl;
	win->current_type[screen] = gl ? SGFE_contextTypeGL : SGFE_contextTypeNone;
	#warning "Warning to notify that this function hasn't been implemented yet."
}

#endif



SGFE_bool SGFE_textInputSetPlatformFlags(SGFE_textInputSettings* s) {
	SGFE_ASSERT_NOT_NULL(s);
	#warning "Warning to notify that this function hasn't been implemented yet."

	/* If backend does not support text input, return false. */
	/* s->platform_flags = ... */
	return SGFE_TRUE;
}


SGFE_bool SGFE_windowTextInputBegin(SGFE_window* win, u8* buffer, isize buffer_len,
		SGFE_textInputSettings* s) {
	SGFE_ASSERT_NOT_NULL(win);
	SGFE_ASSERT_NOT_NULL(buffer);
	SGFE_ASSERT_NOT_NULL(s);
	SGFE_ASSERT_NOT_NEG(buffer_len);
	SGFE_windowTextInputEnd(win);

	#warning "Warning to notify that this function hasn't been implemented yet."

	/* If backend does not support text input, return false. */
	/* s->platform_flags = ... */
	return SGFE_TRUE;
}

void SGFE_windowTextInputEnd(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	if (!SGFE_windowTextInputIsActive(win)) { return; }
	#warning "Warning to notify that this function hasn't been implemented yet."
}




SGFE_videoMode SGFE_videoModeOptimal(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}

SGFE_videoMode SGFE_pixelFormatOptimal(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}



/* === PLATFORM FUNCTIONS === */
/* NOTE(EimaMei): The 'platform' functions below are the only ones that must be
 * present on every backend.
 *
 * Platform functions are backend-specific functions that do not fit anywhere in
 * the base SGFE API, but might be useful to the end user such as hardware-specific
 * features (e.g. 3D slider on the 3DS). The API also includes general functions
 * that are platform-dependent (e.g. getting time and CPU clock speed, initializing
 * terminal output, getting system model, etc). */
#if 1


i64 SGFE_platformGetTimeFromTicks(u64 ticks) {
	/* NOTE(EimaMei): The return must be UNIX time with nanosecond precision. */
	#warning "Warning to notify that this function hasn't been implemented yet."
}

u64 SGFE_platformGetTicks(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}

u64 SGFE_platformGetClockSpeed(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}


SGFE_systemModel SGFE_platformGetModel(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If there are no available models, return ModelNone. */
	return SGFE_systemModelNone;
	#endif
}

SGFE_videoCompositeMode SGFE_platformGetCompositeFormat(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If there are no available composite formats, return SGFE_videoCompositeModeIsDigital. */
	return SGFE_videoCompositeModeIsDigital;
	#endif
}


SGFE_systemRegion SGFE_platformGetRegion(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If platform returned an error or if there's no information, return SGFE_systemRegionUnknown.
	 *
	 * If a region is detected but is not in the list, return 'SGFE_systemRegionNotInTheList'.
	 *
	 * Make an issue in the SGFE repository for the region to be added. */
	return SGFE_systemRegionUnknown;
	#endif
}

SGFE_systemLanguage SGFE_platformGetLanguage(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If platform returned an error or if there's no information, return SGFE_systemLanguageUnknown.
	 *
	 * If a language is detected but is not in the list, return 'SGFE_systemLanguageNotInTheList'.
	 *
	 * Make an issue in the SGFE repository for the language to be added. */
	return SGFE_systemLanguageUnknown;
	#endif
}


SGFE_bool SGFE_platformInitTerminalOutput(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."

	#if 0
	/* If there are no ways to initialize a terminal output, return SGFE_FALSE. */
	return SGFE_FALSE;
	#else
	/* ... */
	return SGFE_TRUE;
	#endif
}

SGFE_bool SGFE_bufferSetPlatformSettings(SGFE_contextBuffer* out_buffer) {
	SGFE_ASSERT_NOT_NULL(out_buffer);
	#warning "Warning to notify that this function hasn't been implemented yet."
}

#endif
/* === DEBUG FUNCTIONS === */
/* NOTE(EimaMei): The backend has to providee string representations of all
 * SGFE platform API errors', warnings' and infos' names as well as their descriptions.
 * Alongside this system error names and descriptions have to be given. */




const char* SGFE_debugSourcePlatformAPIGetName(SGFE_debugType type, isize code) {
	SGFE_ASSERT(
		(type == SGFE_debugTypeError   && (code >= 0 && code < SGFE_errorPlatformCount))   ||
		(type == SGFE_debugTypeWarning && (code >= 0 && code < SGFE_warningPlatformCount)) ||
		(type == SGFE_debugTypeInfo    && (code >= 0 && code < SGFE_infoPlatformCount))
	);
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): If the custom backend does not contain any errors, warnings
	 * or infos, these arrays can be removed. */

	#if 0
	static const char* ERROR_LUT[SGFE_errorPlatformCount] = {
		/* ... */
	};

	static const char* WARNING_LUT[SGFE_warningPlatformCount] = {
		/* ... */

	}

	static const char* INFO_LUT[SGFE_infoPlatformCount] = {
		/* ... */
	}

	static const char** ARR_LUT[] = {ERROR_LUT, WARNING_LUT, INFO_LUT};
	return ARR_LUT[type][code];
	#endif
}

const char* SGFE_debugSourcePlatformAPIGetDesc(SGFE_debugType type, isize code) {
	SGFE_ASSERT(
		(type == SGFE_debugTypeError   && (code >= 0 && code < SGFE_errorPlatformCount))   ||
		(type == SGFE_debugTypeWarning && (code >= 0 && code < SGFE_warningPlatformCount)) ||
		(type == SGFE_debugTypeInfo    && (code >= 0 && code < SGFE_infoPlatformCount))
	);
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): If the custom backend does not contain any errors, warnings
	 * or infos, these arrays can be removed. */

	#if 0
	static const char* ERROR_LUT[SGFE_errorPlatformCount] = {
		/* ... */
	};

	static const char* WARNING_LUT[SGFE_warningPlatformCount] = {
		/* ... */

	};

	static const char* INFO_LUT[SGFE_infoPlatformCount] = {
		/* ... */
	};

	static const char** ARR_LUT[] = {ERROR_LUT, WARNING_LUT, INFO_LUT};
	return ARR_LUT[type][code];
	#endif
}


const char* SGFE_debugCodeSystemGetName(SGFE_debugType type, isize code) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): Usually you can ignore the debug type for system errors. */
	return "Unknown";
	SGFE_UNUSED(type);
}

const char* SGFE_debugSourceSystemGetDesc(SGFE_debugType type, isize code) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): Usually you can ignore the debug type for system errors. */
	return "Unknown";
	SGFE_UNUSED(type);
}


SGFE_debugType SGFE_debugSystemGenerateType_platform(void* ctx_ptr, isize code) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): More often than not system APIs either provide no further
	 * debug types than just 'errors' or have debug types that are incompatible
	 * with SGFE's. In such cases this function has to return the closest
	 * representation of what the code type could be in SGFE. */
}

#endif /* SGFE_IMPLEMENTATION */
