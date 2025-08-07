#include <RGFW_embedded.h>


#if RGFW_3DS
	#define BUTTON_LEFT   RGFW_DpadLeft
	#define BUTTON_RIGHT  RGFW_DpadRight
	#define BUTTON_UP     RGFW_DpadUp
	#define BUTTON_DOWN   RGFW_DpadDown

	#define BUTTON_PRIMARY   RGFW_B
	#define BUTTON_SECONDARY RGFW_A

	#define BUTTON_BACK  RGFW_Select
	#define BUTTON_START RGFW_Start

#elif RGFW_WII
	#define BUTTON_LEFT   RGFW_Left
	#define BUTTON_RIGHT  RGFW_Right
	#define BUTTON_UP     RGFW_Up
	#define BUTTON_DOWN   RGFW_Down

	#define BUTTON_PRIMARY   RGFW_A
	#define BUTTON_SECONDARY RGFW_B

	#define BUTTON_BACK  RGFW_Minus
	#define BUTTON_START RGFW_Plus

#endif
