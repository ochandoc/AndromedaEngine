#pragma once

#include "Core/Window.h"
#include <GLFW/glfw3.h>

class WindowGLFW : public Window
{
public:
	WindowGLFW(const WindowCreationInfo& info);

	virtual ~WindowGLFW();

	virtual void update() override;

	virtual bool is_open() const override; // xema

	virtual void set_vsync(bool vsync) override; // xema
	virtual bool is_vsync() const override;

	virtual void* get_native_window() override;

private:
	GLFWwindow* m_Window; // struct of glfw
	bool m_IsVSync;
};