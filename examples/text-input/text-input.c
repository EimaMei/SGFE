
#include <stdio.h>
#define SGFE_IMPLEMENTATION
#include <SGFE.h>
#include <resources/controls.h>

static
void debug_callback(SGFE_debugContext ctx) {
	if (ctx.source != SGFE_debugSourceApp) {
		printf(
			"%s:%zi:%s: %s %s: %s (%s)\n",
			ctx.filename, ctx.line, ctx.function,
			SGFE_debugSourceName(ctx.source), SGFE_debugTypeName(ctx.type),
			SGFE_debugCodeGetDesc(ctx.source, ctx.type, ctx.code),
			SGFE_debugCodeGetName(ctx.source, ctx.type, ctx.code)
		);
		return;
	}

	printf(
		"%s:%zi:%s: %s\n",
		ctx.filename, ctx.line, ctx.function, ctx.msg
	);
}

static
const char* get_event_name(isize event_mode) {
	switch (event_mode) {
		case 0:  return "state";
		case 1:  return "event queue";
		case 2:  return "callback";
		default: return "???";
	}
}

static
void text_input_callback(SGFE_window* win, u8* text, isize len) {
	isize event_mode = *(isize*)win->user_ptr;

	printf(
		"\nFrom %s: \"%.*s\" string with a length of '%zi' was received\n",
		get_event_name(event_mode),
		len, text,
		len
	);
}

int si = 0;
void annoying_af(void* buffer_ptr) {
	printf("refres!\n");
	si = 1;
}

int main(void) {
	SGFE_setDebugCallback(debug_callback, NULL);

	SGFE_window* win = SGFE_windowMakeContextless(SGFE_windowFlagTerminal);
	if (win == NULL) { return 1; }

	u8 buf[64];
	SGFE_textInputSettings settings;

	printf(
		"Text input example:\n"
		"%s button - standard text input\n"
		"%s button - numpad text input + hint\n"
		"%s button - qwerty text input + initial + multiline\n"
		"%s button - western text input + initital + hint + non-standard flags (length-specified)\n"
		"\n"
		"%s/%s buttons:\n"
		"cycle through methods of printing 'SGFE_eventTextInput' (0 - state, 1 - queue, 2 - callback)\n"
		"\n"
		"Has software keyboard: %i\n",

		SGFE_controllerGetNameButton(SGFE_controllerTypeStandard, SGFE_buttonGetType(BUTTON_PRIMARY)),
		SGFE_controllerGetNameButton(SGFE_controllerTypeStandard, SGFE_buttonGetType(BUTTON_SECONDARY)),
		SGFE_controllerGetNameButton(SGFE_controllerTypeStandard, SGFE_buttonGetType(BUTTON_TERTIARY)),
		SGFE_controllerGetNameButton(SGFE_controllerTypeStandard, SGFE_buttonGetType(BUTTON_QUATERNARY)),

		SGFE_controllerGetNameButton(SGFE_controllerTypeStandard, SGFE_buttonGetType(BUTTON_LEFT)),
		SGFE_controllerGetNameButton(SGFE_controllerTypeStandard, SGFE_buttonGetType(BUTTON_RIGHT)),

		SGFE_platformHasSoftwareKeyboard()
	);

	isize event_mode = 0;
	SGFE_windowSetUserPtr(win, &event_mode);

	while (!SGFE_windowShouldClose(win)) {
		switch (event_mode) {
			case 0:
			case 2: {
				const SGFE_windowState* state = SGFE_windowPollEvents(win);
				if (event_mode == 0 && state->has_text_input) {
					text_input_callback(win, state->text, state->text_len);
				}
			} break;

			case 1: {
				const SGFE_event* event;
				while (SGFE_windowCheckEvent(win, &event)) {
					if (event->type != SGFE_eventTextInput) {
						continue;
					}

					text_input_callback(win, event->text.text, event->text.text_len);
				}
			} break;
		}

		SGFE_controller* controller = SGFE_controllerGet(SGFE_windowGetState(win)->controllers, 0);
		if (controller == NULL) { SGFE_windowSwapBuffers(win); continue; }

		if (SGFE_isDown(controller, BUTTON_START)) {
			SGFE_windowSetShouldClose(win, SGFE_TRUE);
			break;
		}

		if (controller->buttons_down & (BUTTON_RIGHT | BUTTON_LEFT)) {
			if (SGFE_isDown(controller, BUTTON_LEFT)) {
				event_mode -= 1;
				if (event_mode < 0) { event_mode = 2; }
			}
			else {
				event_mode += 1;
				if (event_mode > 2) { event_mode = 0; }
			}
			printf("\nCurrent event mode: %s\n", get_event_name(event_mode));

			SGFE_windowSetQueueEvents(win, (event_mode == 1));
			SGFE_windowSetTextInputCallback(win, (event_mode == 2) ? text_input_callback : NULL);
		}

		if (SGFE_isDown(controller, BUTTON_PRIMARY)) {
			SGFE_textInputSettingsMakeDefault(&settings, SGFE_TRUE);

			SGFE_bool res = SGFE_windowTextInputBegin(win, buf, sizeof(buf), &settings);
			if (!res) {
				SGFE_debugSendMsg(win, 0, 0, "Primary 'SGFE_windowTextInputBegin()' failed.");
			}
		}

		if (SGFE_isDown(controller, BUTTON_SECONDARY)) {
			SGFE_textInputSettingsMakeDefault(&settings, SGFE_TRUE);
			settings.type = SGFE_textInputTypeNumpad;
			settings.password_mode = SGFE_textInputPasswordHide;
			settings.hint_text = "Numbers only!";

			SGFE_bool res = SGFE_windowTextInputBegin(win, buf, sizeof(buf), &settings);
			if (!res) {
				SGFE_debugSendMsg(win, 0, 0, "Secondary 'SGFE_windowTextInputBegin()' failed.");
			}
		}

		if (SGFE_isDown(controller, BUTTON_TERTIARY)) {
			SGFE_textInputSettingsMakeDefault(&settings, SGFE_TRUE);
			settings.type = SGFE_textInputTypeQWERTY;
			settings.initial_text = "QWERTY only!";
			settings.initial_text_len = SGFE_COUNTOF("QWERTY") - 1;
			settings.flags = SGFE_textInputFlagMultiline;

			SGFE_bool res = SGFE_windowTextInputBegin(win, buf, sizeof(buf), &settings);
			if (!res) {
				SGFE_debugSendMsg(win, 0, 0, "Tertiary 'SGFE_windowTextInputBegin()' failed.");
			}
		}

		if (SGFE_isDown(controller, BUTTON_QUATERNARY)) {
			SGFE_textInputSettingsMakeDefault(&settings, SGFE_FALSE);
			settings.type = SGFE_textInputTypeWestern;
			settings.initial_text = "Western keys only cuz most fonts only support 300 glyphs anyway.";
			settings.hint_text = "Ką?";
			settings.flags = SGFE_textInputFlagNotEmpty | SGFE_textInputFlagNotBlank;

			SGFE_bool res = SGFE_windowTextInputBegin(win, buf, sizeof(buf), &settings);
			if (!res) {
				SGFE_debugSendMsg(win, 0, 0, "Quaternary 'SGFE_windowTextInputBegin()' failed.");
			}
		}

		SGFE_windowSwapBuffers(win);
	}
	SGFE_windowClose(win);
}
