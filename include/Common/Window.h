#pragma once

#include "base.h"

namespace And
{
class Engine;
class Renderer;
class GraphicsContext;

struct WindowCreationInfo
{
	unsigned int width, height;
	std::string title;
};

struct KeyboardState
{
	// estado del key buffer al iniciar el frame (todo en false) que cambia cuando recibe input
	bool keysBufferBefore[128];

	// estado del key buffer cuando acaba el frame (en el siguiente frame es el estado que habia en el frame anterior)
	bool keysBufferAfter[128];

	const int buffer_size = 128;
};

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
	friend class Input;
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	std::shared_ptr<GraphicsContext> m_Context;
  KeyboardState m_KeyBoard;
	PLATFORM_WINDOW_DATA m_Data;
};

}
