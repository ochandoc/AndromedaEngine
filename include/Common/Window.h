#pragma once

#include "base.h"

#include "Common/KeyCodes.h"

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

	friend class Renderer;
	friend class Input;
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	
  KeyboardState m_KeyBoard;
	std::unique_ptr<struct WindowData> m_Data;
};

}
