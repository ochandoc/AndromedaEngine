#pragma once

#include "base.h"

namespace And
{
class Engine;
class Renderer;
class GraphicsContext;

class Window
{
	NON_COPYABLE_CLASS(Window)
	NON_MOVABLE_CLASS(Window)
private:
	Window();

public:
	~Window();

	static std::shared_ptr<Window> make(Engine& e, uint32 w, uint32 h, const char* title);

	bool is_open() const;

	void set_vsync(bool vsync);
	bool is_vsync() const;

	void* get_native_window();

	void update();

	std::shared_ptr<GraphicsContext> get_context() const;

	std::function<void()> m_OnWindowClose;

	friend class Renderer;
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	std::shared_ptr<GraphicsContext> m_Context;
	PLATFORM_WINDOW_DATA m_Data;
};

}
