#pragma once

#include "configuration.h"
#include "types.h"

#if defined(AND_WINDOWS) || defined(AND_LINUX)

struct GLFWwindow;
#define WindowHandle GLFWwindow*

struct _window_data
{
	WindowHandle handle;
	uint32 width, height;
	bool is_open;
	bool is_vsync;
};

#define PLATFORM_WINDOW_DATA _window_data

#endif