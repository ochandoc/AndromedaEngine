#pragma once

#include "Core/Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace And
{

class WindowGLFW : public Window
{
public:
	WindowGLFW(const WindowCreationInfo& info);

	virtual ~WindowGLFW();

	virtual void update() override;

	virtual bool is_open() const override;

	virtual void set_vsync(bool vsync) override;
	virtual bool is_vsync() const override;

	virtual void* get_native_window() override;

	virtual std::shared_ptr<GraphicsContext> create_context() override;

private:
	GLFWwindow* m_Window; // struct of glfw
	bool m_IsVSync;
	WindowCreationInfo m_CreationInfo;
};

}
