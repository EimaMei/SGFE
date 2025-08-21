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

typedef SGFE_ENUM(isize, SGFE_bufferFormat) {
	SGFE_bufferFormatRGBA8,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_bufferFormatCount,
};


typedef SGFE_ENUM(isize, SGFE_controllerType) {
	SGFE_controllerTypeStandard = 0,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_controllerTypeCount,
	SGFE_controllerTypeLast = SGFE_controllerTypeCount - 1
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
	SGFE_textInputPlatformFlagNone = 0,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif
};

#endif /* SGFE_CUSTOM_BACKEND */

/* === STRUCTURE TYPES === */

#ifdef SGFE_CUSTOM_BACKEND

struct SGFE_windowSource {
	/* ... */
};

struct SGFE_contextBufferSource {
	/* ... */
};

#ifdef SGFE_OPENGL
struct SGFE_contextGL {
	/* ... */
};
#endif

#endif /* SGFE_CUSTOM_BACKEND */


#include <SGFE.h>

#ifdef SGFE_CUSTOM_BACKEND

/* === PLATFORM FUNCTIONS FUNCTIONS === */
/* SGFE_DEF isize SGFE_platformGetSmthExamle(void); */

#endif




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

const isize SGFE_FORMAT_BYTES_PER_PIXEL_LUT[SGFE_bufferFormatCount] = {
	4, /* ... */
};

const char* SGFE_VIDEO_MODE_NAME_LUT[SGFE_videoModeCount] = {
	/* "..." */
};

const char* SGFE_PIXEL_FORMAT_NAME_LUT[SGFE_bufferFormatCount] = {
	"SGFE_bufferFormatRGBA8", /* "..." */
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

	/* NOTE(EimaMei): By default 'SGFE_windowMake' disconnects all controllers
	 * and adds them to the "disconnected list.
	 *
	 * When handling the connection of a controller you should call the internal
	 * 'SGFE__controllerSetConnection()' function, which handles the internal
	 * double-linking logic for you. Specifying a true boolean adds the controller
	 * to the "connected controllers" linked-list and removes the controller from
	 * the "disconnected controllers" linked-list. Likewise specifying false does
	 * the opposite thing (add to "disconnected", remove from "connected" lists).
	 *
	 * If you need to have low-level control for the logic mentioned in the above
	 * you can utilize 'SGFE__controllerRemoveFromList' and 'SGFE__controllerAddToList'.
	 *
	 * It's important to note that you shouldn't invoke these functions when it's
	 * unapplicable. This include using these functions when:
	 *	1. 'SGFE_TRUE' is specified but every controller is already connected.
	 *	2. 'SGFE_FALSE' is specified but every controller is already disconnected.
	 *  3. 'SGFE_TRUE' is specified but the given controller is already connected.
	 *  4. 'SGFE_FALSE' is specified but the given controller is already disconnected.
	 *
	 * 'SGFE__controllerSetConnection()' has assertions for all four invalid states,
	 * while the two other internal functions only check for the first two states.
	 "*/
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

	for (isize i = 0; i < SGFE_MAX_CONTROLLERS; i += 1) {
		SGFE_controller* controller = &win->controllers[i];

		if (SGFE_windowGetEventEnabled(win, SGFE_eventControllerDisconnected)) {
			/* ... */
			SGFE_windowControllerCallback(win, controller, SGFE_FALSE);
			if (win->is_queueing_events) {
				SGFE_event event;
				event.type = SGFE_eventControllerDisconnected;
				event.controller.controller = controller;
				SGFE_windowEventPush(win, &event);
			}
		}
		else if (SGFE_windowGetEventEnabled(win, SGFE_eventControllerConnected)) {
			/* ... */
			SGFE_windowControllerCallback(win, controller, SGFE_TRUE);
			if (win->is_queueing_events) {
				SGFE_event event;
				event.type = SGFE_eventControllerConnected;
				event.controller.controller = controller;
				SGFE_windowEventPush(win, &event);
			}
		}

		switch (controller->type) {
			/*case SGFE_controllerTypeStandard: */
			case 0: {
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
							/* SGFE__processCallbackAndEventQueue_Motion(win, controller, motion_ptr); */
						}
					}
				}

				if (SGFE_windowGetEventEnabled(win, SGFE_eventControllerBattery)) {
					#if 0
					if (is_battery_updated) {
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
				}
			} break;

			#if 0
			case SGFE_controllerTypeDifferent: {

			} break;
			#endif
		}
	}

	#if 0
	while (SGFE_TRUE) {
		keyboard_event_struct kEvent;
		i32 res = get_keyboard_event(&kEvent);
		if (res != SGFE_TRUE) { break; }

		switch (kEvent.type) {
			case KEYBOARD_EVENT_DISCONNECTED:
			case KEYBOARD_EVENT_CONNECTED: {
				SGFE_bool connect = (kEvent.type == KEYBOARD_CONNECTED);
				if (!SGFE_windowGetEventEnabled(win, SGFE_eventKeyboardConnected + !connect)) {
					break;
				}

				u32 key;
				SGFE_MEMSET(keyboard, 0, sizeof(*keyboard));
				if (connect) {
					keyboard->repeat_interval = 250 * 100000;

					#if 0
					for (key = 0x04; key <= 0x1D; key += 1) {
						SGFE__keyboardSetLUT(keyboard, key, SGFE_keyA + (SGFE_key)(key - 0x04));
					}
					for (key = 0x1E; key <= 0x26; key += 1) {
						SGFE__keyboardSetLUT(keyboard, key, SGFE_key1 + (SGFE_key)(key - 0x1E));
					}
					SGFE__keyboardSetLUT(keyboard, 0x27, SGFE_key0);
					#endif
					/* ... */
				}

				SGFE__keyboardSetConnection(win, keyboard, connect);
				SGFE_windowKeyboardCallback(win, keyboard, connect);
				if (win->is_queueing_events) {
					SGFE_event event;
					event.type = connect ? SGFE_eventKeyboardConnected : SGFE_eventKeyboardDisconnected;
					event.keyboard.keyboard = keyboard;
					event.keyboard.keyboard = win->state.text;
					event.text.text_len = win->state.text_len;
					SGFE_windowEventPush(win, &event);
				}
			} break;

			case KEYBOARD_EVENT_PRESSED: {
				if (SGFE_windowGetEventEnabled(win, SGFE_eventKeyboardDown)) {
					SGFE_key key = SGFE_keyFromAPI(keyboard, kEvent.keycode);
					keyboard->keystate[key].was_down = SGFE_FALSE;
					keyboard->keystate[key].is_down  = SGFE_TRUE;
					keyboard->first_press_key = key;
					keyboard->first_press_timestamp = SGFE_platformGetTicks();
					/* keyboard->modifiers = ... */
					/* keyboard->SGFE_to_SYMBOL_LUT[key] = kEvent.symbol; */

					SGFE__processCallbackAndEventQueue_KeyDown(win, keyboard, key);

					/* if (SGFE_windowGetEventEnabled(win, SGFE_eventTextInput)) {
						SGFE__handleTextInput(win, keyboard, key, win->src.text_input_type, win->src.text_is_multiline, win->src.text_max_len);
					} */
				}
			} break;

			case KEYBOARD_EVENT_RELEASED: {
				if (!SGFE_windowGetEventEnabled(win, SGFE_eventKeyboardUp)) {
					break;
				}

				SGFE_key key = SGFE_keyFromAPI(keyboard, kEvent.keycode);
				keyboard->keystate[key].was_down = SGFE_TRUE;
				keyboard->keystate[key].is_down  = SGFE_FALSE;
				/* keyboard->modifiers = ... */

				SGFE__processCallbackAndEventQueue_KeyUp(win, keyboard, key);
			} break;
		}
	}
#endif

	if (SGFE_windowGetEventEnabled(win, SGFE_eventTextInput)) {
		/* NOTE(EimaMei): We should always reset text input _state and length_ if
		 * there are no key inputs. Do not reset the text to NULL as that'll result
		 * in a crash for obvious reasons. Only set it to NULL when the user isn't
		 * resquesting text input anymore. */
		win->state.has_text_input = SGFE_FALSE;
		win->state.text_len = 0;

		/* win->state.has_text_input = ...; */
		/* win->state.text_len = ...; */

		#if 0
		if (win->state.has_text_input) {
			SGFE_windowTextInputCallback(win, win->state.text, win->state.text_len);
			if (win->is_queueing_events) {
				SGFE_event event;
				event.type = SGFE_eventTextInput;
				event.text.text = win->state.text;
				event.text.text_len = win->state.text_len;
				SGFE_windowEventPush(win, &event);
			}
		}
		#endif
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
	SGFE_ASSERT_FMT(type > 0 && type <= SGFE_controllerTypeCount, "type = %i;", type);
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	switch (type) {
		case 0: return SGFE_platformButtonFromType0(mask);
		case 1: return SGFE_platformButtonFromType1(mask);
		default: return 0;
	}
	#endif
}

u32 SGFE_buttonToAPI(SGFE_controllerType type, SGFE_button button) {
	SGFE_ASSERT_FMT((button & ~SGFE_buttonGetMask(type)) == 0, "button = %i; ", button);
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	switch (type) {
		case 0: return SGFE_platformButtonToType0(mask);
		case 1: return SGFE_platformButtonToType1(mask);
		default: return 0;
	}
	#endif
}


const char* SGFE_controllerGetNameButton_platform(SGFE_controllerType type,
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
	/* NOTE(EimaMei): The function should return 'SGFE_TRUE' if the backend was
	 * succesful in enabling/disabling the pointer. For example, failing to enable
	 * something should return 'SGFE_FALSE', but succesfully disabling something
	 * should return 'SGFE_TRUE'. */
}

SGFE_bool SGFE_controllerEnableMotion_platform(SGFE_controller* controller,
		SGFE_motionType motion, SGFE_bool enable) {
	/* NOTE(EimaMei): 'SGFE_controllerEnableMotion' already asserts that the
	 * controller cannot be NULL, that motion is valid for the controller and that
	 * the boolean is either a one or zero. */
	#warning "Warning to notify that this function hasn't been implemented yet."
	/* NOTE(EimaMei): The function should return 'SGFE_TRUE' if the backend was
	 * succesful in enabling/disabling the motion. For example, failing to enable
	 * something should return 'SGFE_FALSE', but succesfully disabling something
	 * should return 'SGFE_TRUE'. */
}



SGFE_bufferFormat SGFE_bufferGetOptimalFormat(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}

SGFE_bool SGFE_bufferSetPlatformSettings(SGFE_contextBuffer* out_buffer) {
	SGFE_ASSERT_NOT_NULL(out_buffer);
	#warning "Warning to notify that this function hasn't been implemented yet."
}


SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT_NOT_NULL(b);
	#warning "Warning to notify that this function hasn't been implemented yet."
	SGFE_debugSendAPI(b, SGFE_debugTypeInfo, SGFE_infoCreateContextBuffer);
}

void SGFE_bufferFreeContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT_NOT_NULL(b);
	#warning "Warning to notify that this function hasn't been implemented yet."
	b->buffers[0] = NULL;
	b->buffers[1] = NULL;
	SGFE_debugSendAPI(b, SGFE_debugTypeInfo, SGFE_infoFreeContextBuffer);
}


SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT_NOT_NULL(b);
	SGFE_bufferFreeFramebuffers(b);

	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);

	isize size = width * height * SGFE_bufferFormatGetBytesPerPixel(b->format);

	if (b->is_native) {
		#if 0
		u8* buffers = /* systemAllocFunction */((size_t)(2 * size));
		if (buffers == NULL) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		b->buffers[0] = &buffers[0 * size];
		b->buffers[1] = &buffers[1 * size];
		#endif
	}
	else {
		#if 0
		u8* buffers = SGFE_ALLOC((size_t)(2 * size));
		if (buffers == NULL) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		u8* native_buffers = /* systemAllocFunction */((size_t)(2 * size));
		if (native_buffers == NULL) {
			SGFE_debugSendAPI(b, SGFE_debugTypeError, SGFE_errorOutOfMemory);
			return SGFE_FALSE;
		}

		b->buffers[0] = &buffers[0 * size];
		b->buffers[1] = &buffers[1 * size];

		b->buffers_native[0] = &native_buffers[0 * size];
		b->buffers_native[1] = &native_buffers[1 * size];
		#endif
	}

	b->is_buffer_allocated = SGFE_TRUE;
	return SGFE_TRUE;
}

SGFE_bool SGFE_bufferFreeFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT_NOT_NULL(b);
	if (b->buffers[0] == NULL || !b->is_buffer_allocated) { return SGFE_FALSE; }

	#if 0
	if (b->is_native) {
		/* systemFreeFunction */(b->buffers[0]);
		b->buffers[0] = NULL;
		b->buffers[1] = NULL;
	}
	else {
		SGFE_FREE(b->buffers[0]);
		b->buffers[0] = NULL;
		b->buffers[1] = NULL;

		/* systemFreeFunction */(b->buffers_native[0]);
		b->buffers_native[0] = NULL;
		b->buffers_native[1] = NULL;
	}
	#endif

	b->is_buffer_allocated = SGFE_FALSE;
	return SGFE_TRUE;
}

u8* SGFE_bufferConvertFramebufferToNative(SGFE_contextBuffer* b) {
	u8* src = SGFE_bufferGetFramebuffer(b);

	if (b->is_native) {
		return src;
	}
	u8* dst = b->buffers_native[b->current];

	isize width, height;
	SGFE_bufferGetResolution(b, &width, &height);

	/* ... */
	#warning "Warning to notify that this function hasn't been implemented yet."

	return dst;
}


void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height) {
	SGFE_ASSERT_NOT_NULL(b);
	#warning "Warning to notify that this function hasn't been implemented yet."

	#if 0
	/* Depending on the platform: */
	SGFE_videoGetResolution(b->mode, out_width, out_height);
	/* or */

	if (out_width)  {  *out_width = /* ... */; }
	if (out_height) { *out_height = /* ... */; }
	#endif
}


void SGFE_windowSwapBuffersBuffer(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	#warning "Warning to notify that this function hasn't been implemented yet."

	/* NOTE(EimaMei): 'SGFE__fetchSwapBuffer()' is an internal helper function
	 * that has changes behavior depending on whenever or not 'SGFE_BUFFER_NO_CONVERSION'
	 * is defined. If it is, the current frame buffer gets converted into a native
	 * one (if it isn't already) and returns it. If the macro is not returned,
	 * the current frame buffer is returned without any additional checking or
	 * conversions. */
	#if 0
	SGFE_contextBuffer* b = SGFE_windowGetContextBuffer(win);

	u8* buffer = SGFE__fetchSwapBuffer(b);
	/* ... */
	b->current ^= b->is_double_buffered;
	#endif
}



#ifdef SGFE_OPENGL

void* SGFE_glGetBoundContext(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}


/* NOTE(EimaMei): If a system does not support an OpenGL shader format, -1 should be
 * returned. */
#warning "Warning to notify that THESE function haven't been implemented yet."
isize SGFE_glGetVertexShaderType(void)   { return -1; }
isize SGFE_glGetFragmentShaderType(void) { return -1; }
isize SGFE_glGetGeometryShaderType(void) { return -1; }


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
	SGFE_windowSetEventEnabled(win, SGFE_eventTextInput, SGFE_TRUE);
	win->state.has_text_input = SGFE_FALSE;
	win->state.text = buffer;
	win->state.text_len = 0;

	return SGFE_TRUE;
}

void SGFE_windowTextInputEnd(SGFE_window* win) {
	SGFE_ASSERT_NOT_NULL(win);
	if (!SGFE_windowTextInputIsActive(win)) { return; }
	#warning "Warning to notify that this function hasn't been implemented yet."

	/* ... */
	SGFE_windowSetEventEnabled(win, SGFE_eventTextInput, SGFE_FALSE);
	win->state.has_text_input = SGFE_FALSE;
	win->state.text = NULL;
	win->state.text_len = 0;
}



SGFE_videoSignal SGFE_videoGetSignal(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If the system does not report a signal, return SGFE_videoSignalNone.
	 * If the system does report one but an error occured or it isn't in the list,
	 * return SGFE_videoSignalUnknown. */
	return SGFE_videoSignalNone;
	#endif
}

SGFE_videoCable SGFE_videoGetCable(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If the system does not report a cable, return SGFE_videoCableNone.
	 * If the system does report one but an error occured or it isn't in the list,
	 * return SGFE_videoCableUnknown. */
	return SGFE_videoCableNone;
	#endif
}

SGFE_bool SGFE_videoIsProgressive(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* Must return either true or false. */
	return SGFE_TRUE;
	#endif
}

SGFE_videoMode SGFE_videoGetOptimalMode(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}



SGFE_systemModel SGFE_systemGetModel(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If the system does not report a model, return SGFE_systemModelNone.
	 * If the system does report one but an error occured or it isn't in the list,
	 * return SGFE_systemModelUnknown. */
	return SGFE_systemModelNone;
	#endif
}

SGFE_systemRegion SGFE_systemGetRegion(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If the system does not report a region, return SGFE_systemRegionNone.
	 * If the system does report one but an error occured or it isn't in the list,
	 * return SGFE_systemRegionUnknown. */
	return SGFE_systemRegionUnknown;
	#endif
}

SGFE_systemLanguage SGFE_systemGetLanguage(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	#if 0
	/* If the system does not report a language, return SGFE_systemLanguageNone.
	 * If the system does report one but an error occured or it isn't in the list,
	 * return SGFE_systemLanguageUnknown. */
	return SGFE_systemLanguageUnknown;
	#endif
}



#if 1 /* === PLATFORM FUNCTIONS === */
/* NOTE(EimaMei): The 'platform' functions below are the only ones that must be
 * present on every backend.
 *
 * Platform functions are backend-specific functions that do not fit anywhere in
 * the base SGFE API, but might be useful to the end user such as hardware-specific
 * features (e.g. 3D slider on the 3DS). The API also includes general functions
 * that are platform-dependent (e.g. getting time and CPU clock speed, initializing
 * terminal output, getting system model, etc). */

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


void SGFE_platformWaitForVBlank(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
}

SGFE_bool SGFE_platformInitTerminalOutputEx(SGFE_contextBuffer* b, isize x, isize y,
		isize width, isize height) {
	SGFE_ASSERT_NOT_NULL(b);
	SGFE_ASSERT_NOT_NEG(x);
	SGFE_ASSERT_NOT_NEG(y);
	SGFE_ASSERT_NOT_NEG(width);
	SGFE_ASSERT_NOT_NEG(height);
	if (b->buffers[0] == NULL) { return SGFE_FALSE; }
	#warning "Warning to notify that this function hasn't been implemented yet."

	#if 0
	/* If there are no ways to initialize a terminal output, return SGFE_FALSE. */
	return SGFE_FALSE;
	#else
	/* ... */
	return SGFE_TRUE;
	#endif
}

SGFE_bool SGFE_platformHasSoftwareKeyboard(void) {
	#warning "Warning to notify that this function hasn't been implemented yet."
	return SGFE_FALSE;

}

#endif /* === PLATFORM FUNCTIONS === */



#if 1 /* === DEBUG FUNCTIONS === */
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

#endif /* === DEBUG FUNCTIONS === */

#endif /* SGFE_IMPLEMENTATION */
