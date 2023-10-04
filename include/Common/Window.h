#pragma once

namespace And
{
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
public:
	Window(const WindowCreationInfo& info);
	Window(const Window&) = delete;
	Window(Window&&) = delete;

	~Window();

	Window& operator =(const Window&) = delete;
	Window& operator =(Window&&) = delete;

	bool is_open() const;

	void set_vsync(bool vsync);
	bool is_vsync() const;

	void* get_native_window();

	Renderer& create_renderer();

	std::shared_ptr<GraphicsContext> get_context() const;

	std::function<void()> m_OnWindowClose;

	friend class Renderer;
	friend class Input;
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	std::unique_ptr<struct WindowData> m_Data;
	KeyboardState m_KeyBoard;
};

}
