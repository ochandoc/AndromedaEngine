#pragma once

#include <string>
#include <functional>

#include "Graphics/GraphicsContext.h"

namespace And
{

struct WindowCreationInfo
{
	unsigned int width, height;
	std::string title;
	GraphicsAPI api;
};

class Renderer;

class Window
{
public:
	Window(const WindowCreationInfo& info);
	Window(const Window&) = delete;
	Window(Window&&) = delete;

	virtual ~Window();

	Window& operator =(const Window&) = delete;
	Window& operator =(Window&&) = delete;

	virtual void update();

	virtual bool is_open() const;

	virtual void set_vsync(bool vsync);
	virtual bool is_vsync() const;

	virtual void* get_native_window() const;

	virtual Renderer& create_renderer();

	std::shared_ptr<GraphicsContext> get_context() const { return m_Context; }

	std::function<void()> m_OnWindowClose;

private:
	void* m_Handle;
	bool m_IsVSync;
	WindowCreationInfo m_CreationInfo;
	std::shared_ptr<GraphicsContext> m_Context;
	std::unique_ptr<Renderer> m_Renderer;
};

}
