#define SGFE_CUSTOM_BACKEND
#define SGFE_MAX_CONTROLLERS 1

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

typedef SGFE_ENUM(isize, SGFE_systemModel) {
	SGFE_systemModelNone,

	#ifdef SGFE_CUSTOM_BACKEND
	/* ... */
	#endif

	SGFE_systemModelUnknown,
	SGFE_systemModelCount,
};


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

#endif /* SGFE_CUSTOM_BACKEND */


#include <SGFE.h>

/* === STRUCTURE TYPES === */

#ifdef SGFE_CUSTOM_BACKEND

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

	/* ... */
};

#ifdef SGFE_OPENGL
struct SGFE_contextOpenGL {
	SGFE_screen screen;
	SGFE_videoMode mode;
	SGFE_pixelFormat format;
	/* ctx_type*/ ctx;

	/* ... */
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
	/* ... */
	SGFE_context ctx;
	/* or: SGFE_context ctx[SGFE_screenCount] */
};

struct SGFE_window {
	SGFE_windowSource src;
	SGFE_context* current[SGFE_screenCount];

	SGFE_event events[32];
	isize event_len;

	SGFE_controller controllers[SGFE_MAX_CONTROLLERS];

	SGFE_bool queue_events, polled_events;
	u32 _flags;

	SGFE_callbacks callbacks;
	void* userPtr;
};

#endif /* SGFE_CUSTOM_BACKEND */




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




SGFE_bool SGFE_windowMake_platform(SGFE_window* win) {
	/* NOTE(EimaMei): 'SGFE_windowMake' already asserts that the window cannot be
	 * NULL.*/
	#warning "Warning to notify that this function is not implemented."
	/* ... */
	SGFE_windowSource* src = &win->src;
	SGFE_MEMSET(&src->ctx, 0, sizeof(src->ctx));
	/* ... */
}


void SGFE_windowClose_platform(SGFE_window* win) {
	/* NOTE(EimaMei): 'SGFE_windowClose' already asserts that the window cannot be
	 * NULL.*/
	#warning "Warning to notify that this function is not implemented."
}


void SGFE_windowSetFlags(SGFE_window* win, SGFE_windowFlags flags) {
	SGFE_ASSERT(win != NULL);
	#warning "Warning to notify that this function is not implemented."
}


void SGFE_windowPollEvents(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	#warning "Warning to notify that this function is not implemented."
}


void SGFE_windowSwapBuffers(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	/* NOTE(EimaMei): On systems that have multiple screens, both current contexts
	 * must share the same type. */
	#warning "Warning to notify that this function is not implemented."
}

void SGFE_windowMakeCurrent(SGFE_window* win, SGFE_context* ctx) {
	SGFE_ASSERT(win != NULL);
	SGFE_ASSERT(ctx != NULL);
	win->current = ctx;

	switch (SGFE_contextGetType(ctx)) {
		/* ... */
	}
	#warning "Warning to notify that this function is not implemented."
}


SGFE_bool SGFE_windowInitTerminalOutput(SGFE_window* win) {
	SGFE_ASSERT(win != NULL);
	#warning "Warning to notify that this function is not implemented."
}



SGFE_button SGFE_buttonFromAPI(SGFE_controllerType type, u32 mask) {
	SGFE_ASSERT(type > 0 && type <= SGFE_controllerTypeCount);
	#warning "Warning to notify that this function is not implemented."
}

u32 SGFE_buttonToAPI(SGFE_controllerType type, SGFE_button button) {
	SGFE_ASSERT((button & ~SGFE_buttonGetMask(type)) == 0);
	#warning "Warning to notify that this function is not implemented."
}


const char* SGFE_controllerGetNameButton_platform(const SGFE_controller* controller,
		SGFE_buttonType button) {
	/* NOTE(EimaMei): 'SGFE_windowClose' already asserts that the controller cannot be
	 * NULL and that the button is valid for the controller type. */
	#warning "Warning to notify that this function is not implemented."
}


SGFE_bool SGFE_controllerEnablePointer_platform(SGFE_controller* controller,
		SGFE_pointerType pointer, SGFE_bool enable) {
	/* NOTE(EimaMei): 'SGFE_windowClose' already asserts that the controller cannot
	 * be NULL, that pointer is valid for the controller and that the boolean is
	 * either a one or zero. */
	#warning "Warning to notify that this function is not implemented."
}

SGFE_bool SGFE_controllerEnableMotion_platform(SGFE_controller* controller,
		SGFE_motionType motion, SGFE_bool enable) {
	/* NOTE(EimaMei): 'SGFE_windowClose' already asserts that the controller cannot
	 * be NULL, that motion is valid for the controller and that the boolean is
	 * either a one or zero. */
	#warning "Warning to notify that this function is not implemented."
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

	/* ... */
	return res;
}


SGFE_bool SGFE_bufferCreateContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function is not implemented."
}

void SGFE_bufferFreeContext(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function is not implemented."
}


SGFE_bool SGFE_bufferAllocFramebuffers(SGFE_contextBuffer* b) {
	SGFE_ASSERT(b != NULL);
	SGFE_ASSERT(b != NULL);
	SGFE_bufferFreeFramebuffers(b);

	#warning "Warning to notify that this function is not implemented."

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

	#warning "Warning to notify that this function is not implemented."

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
	#warning "Warning to notify that this function is not implemented."
	
	return dst;
}


void SGFE_bufferGetResolution(SGFE_contextBuffer* b, isize* out_width, isize* out_height) {
	SGFE_ASSERT(b != NULL);
	#warning "Warning to notify that this function is not implemented."

	#if 0
	/* Depending on the platform: */
	SGFE_videoModeResolution(b->mode, out_width, out_height);
	/* or */

	if (out_width)  {  *out_width = /* ... */; }
	if (out_height) { *out_height = /* ... */; }
	#endif
}



SGFE_videoMode SGFE_videoModeOptimal(void) {
	#warning "Warning to notify that this function is not implemented."
}

SGFE_videoMode SGFE_pixelFormatOptimal(void) {
	#warning "Warning to notify that this function is not implemented."
}



/* === PLATFORM FUNCTIONS === */
/* NOTE(EimaMei): All 'SGFE_platform' functions except 'SGFE_platformGetModel' are
 * implementation-specified and can be anything.
 *
 * If there are no available system models, SGFE_platformGetModel() returns None. */
#if 1

SGFE_systemModel SGFE_platformGetModel(void) {
	#warning "Warning to notify that this function is not implemented."
	#if 0
	/* Depending on the platform. Check the above comment: */
	return SGFE_systemModelNone;
	#endif
}

#endif

#endif /* SGFE_IMPLEMENTATION */
