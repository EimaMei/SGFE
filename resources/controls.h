#include <SGFE.h>


#if SGFE_3DS
	#define BUTTON_LEFT   SGFE_DpadLeft
	#define BUTTON_RIGHT  SGFE_DpadRight
	#define BUTTON_UP     SGFE_DpadUp
	#define BUTTON_DOWN   SGFE_DpadDown

	#define BUTTON_PRIMARY   SGFE_B
	#define BUTTON_SECONDARY SGFE_A

	#define BUTTON_BACK  SGFE_Select
	#define BUTTON_START SGFE_Start

#elif SGFE_WII
	#define BUTTON_LEFT   SGFE_Left
	#define BUTTON_RIGHT  SGFE_Right
	#define BUTTON_UP     SGFE_Up
	#define BUTTON_DOWN   SGFE_Down

	#define BUTTON_PRIMARY   SGFE_A
	#define BUTTON_SECONDARY SGFE_B

	#define BUTTON_BACK  SGFE_Minus
	#define BUTTON_START SGFE_Plus

#endif
