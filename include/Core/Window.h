#pragma once

#include <string>
#include <memory>
#include <functional>

struct WindowCreationInfo
{
	unsigned int width, height;
	std::string title;
};

class Window
{
public:
	Window();
	Window(const Window&) = delete;
	Window(Window&&) = delete;

	virtual ~Window();

	Window& operator =(const Window&) = delete;
	Window& operator =(Window&&) = delete;

	static std::shared_ptr<Window> Create(const WindowCreationInfo& info);

	virtual void update() = 0;

	virtual bool is_open() const = 0;

	virtual void set_vsync(bool vsync) = 0;
	virtual bool is_vsync() const = 0;

	virtual void* get_native_window() = 0;

	std::function<void()> m_OnWindowClose;
};