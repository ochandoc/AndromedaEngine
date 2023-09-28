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
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	std::unique_ptr<struct WindowData> m_Data;
};

}
