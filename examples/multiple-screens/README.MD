# Multiple screens on SGFE Embedded
## Problem
There are a few platforms that are capable of rendering multiple screens at once.
The initial thought would be to handle them as extra windows like on desktop
computers, however you *cannot* treat them as such since they don't actually generate 
events on their own, so instead they're surfaces meant for binding graphical contexts.
This raises a concern with SGFE
specifically since the graphical context is tied to the window itself (many context 
functions like `SGFE_window_swapBuffers` request a window, not a context),
which is an issue as these extra screens have to be treated as "one giant" window 
with multiple contexts for performance reasons.

As such, to fully take advantage of multiple screens in SGFE Embedded it is recommended 
to follow the general guide laid out here while also consulting the examples in 
this directory for any platform-specific nuances.

NOTE: Examples will utilize the Nintendo 3DS backend as the basis to demonstrate
the API differences better, however said changes are applicable on every platform.

## Primary differences between SGFE Embedded and Desktop
### Binding contexts,  current "window"
On Desktop `SGFE_window_makeCurrent` requires a window handle to set the current
graphical context, however on Embedded the argument is replaced with an integer
enum type `SGFE_screen` whose each value represent a sepcific screen. Each one
is also accompanied by an enumerator following the naming scheme of `SGFE_screen<Name>`.
Platforms that only have one screen instead have `SGFE_screenPrimary` as the only option.

NOTE: due to this change `SGFE_getCurrent` returns an integer also to current screen
instead of a window. `SGFE_window_swapInterval` as well is effected by the currently
selected context.

```c
// Desktop
SGFE_DEF void SGFE_window_makeCurrent(SGFE_window* win);
SGFE_DEF SGFE_window* SGFE_getCurrent(void);

// Embedded
SGFE_DEF void SGFE_window_makeCurrent(SGFE_window* win, SGFE_screen screen);
SGFE_DEF SGFE_screen SGFE_getCurrent(SGFE_window* win);


// Usage:
//
// Current graphical context is now set to the top screen of the 3DS. Every
// backend will have their human-readable version of the screen IDs following
// the format of 'SGFE_SCREEN_<name>'.
SGFE_window_makeCurrent(SGFE_screenTop);
// The top screen is going to always run at 60 fps.
SGFE_window_swapInterval(win, SGFE_TRUE);

// Now the graphical context is set to the bottom screen. 
SGFE_window_makeCurrent(SGFE_screenBottom);
// The bottom screen will run at an uncapped FPS, independent of the upper screen.
SGFE_window_swapInterval(win, SGFE_TRUE);
```

### Window flags 
1. When creating a new video you're able to select a screen(s) to use via flags.
If no screen is selected, the backend will implicitly enable the default/top screen
for the system.

2. You're able to specify multiple screens. Usually there is going to be an available 
enumerator value that is the `AND` of all screen flags for convenience.

3. On initialization the current graphical context is set to the specified screen.
If all screens are specified, the default screen (from point 1) is choosen as the 
current graphical context.

```c
// Applies the 'SGFE_windowTopScreen' flag automatically. SGFE_getCurrent() == SGFE_screenTop.
SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), 0);
// Applies the 'SGFE_windowTopScreen' flag automatically. SGFE_getCurrent() == SGFE_screenTop.
SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowInitNoAPI);
// You can manually apply the 'SGFE_windowTopScreen' flag if you wish. SGFE_getCurrent() == SGFE_screenTop.
SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowTopScreen);

// Now only the `SGFE_windowBottomScreen` flag is applied. SGFE_getCurrent() == SGFE_screenBottom.
SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowBottomScreen);a

// All screens have been specified so that the window could render both at the same time. SGFE_getCurrent() == SGFE_screenTop.
SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowTopScreen | SGFE_windowBottomScreen);
// Same thing as the previous declaration. SGFE_getCurrent() == SGFE_screenTop.
SGFE_window* win = SGFE_createWindow(SGFE_videoModeOptimal(), SGFE_windowDualScreen);
```

### Swapping buffers
Biggest difference between the two libraries is that unlike on Desktop where you'd 
run `SGFE_window_swapBuffers` for each context, on Embedded you'd run that same
function for *all* screens/contexts. This change is done for performance reasons
as flushing the graphics per screen is very not efficient. 

```c
// Draw on the top screen.
SGFE_window_makeCurrent(SGFE_screenTop); {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, SGFE_COUNTOF(vertices));
}

// Draw on the bottom screen.
SGFE_window_makeCurrent(SGFE_screenBottom); {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, SGFE_COUNTOF(vertices));
}

// Normally you'd call `SGFE_window_swapBuffers` at the end of each screen's scope,
// however it is heavily recommended to only call it once as the function internally 
// swaps the buffers of all screens for you.
//
// Trying to swap the buffers for each screen individually will result in worse
// performance.
SGFE_window_swapBuffers(win);
```