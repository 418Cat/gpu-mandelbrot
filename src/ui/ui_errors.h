#ifndef UI_ERR_H
#define UI_ERR_H

#define UI_ERROR int
/*
 * 	- shared errors from -1 to -1023
 * 	- shared warnings from 1 to 1023
 *
 * 	- function-specific errors starting -1024
 * 	- function-specific warnings starting 1024
 */


// SHARED
#define ERR_NOT_INIT					-1 // ui_init() hasn't been called before other ui functions
#define OK								 0
#define WARN_NOT_SHOW_UI				 1 // ui display function called while `show_ui` is set to false

// UI_INIT()
#define ERR_INIT_GLFW					-1024 // Could not init glfw
#define ERR_INIT_WINDOW					-1025 // Could not create window

// UI_RENDER_START()
#define ERR_RENDER_START_TWICE			-1024 // `ui_render_start()` has been called twice without `ui_render_stop()` in between


// UI_RENDER_STOP()
#define ERR_RENDER_STOP_WITHOUT_START	-1024 // `ui_render_stop()` has been called without a previous `ui_render_start()`

#endif
