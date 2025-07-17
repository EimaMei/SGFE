#include <RGFW_embedded.h>


#if RGFW_3DS
	#define BUTTON_LEFT   RGFW_DpadLeft
	#define BUTTON_RIGHT  RGFW_DpadRight
	#define BUTTON_UP     RGFW_DpadUp
	#define BUTTON_DOWN   RGFW_DpadDown

    #define BUTTON_WEST  RGFW_Y
    #define BUTTON_EAST  RGFW_A
    #define BUTTON_NORTH RGFW_X
    #define BUTTON_SOUTH RGFW_B

    #define BUTTON_BACK  RGFW_Select
    #define BUTTON_START RGFW_Start
#endif
