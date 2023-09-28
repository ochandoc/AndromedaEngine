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

	~Window();

	Window& operator =(const Window&) = delete;
	Window& operator =(Window&&) = delete;

	bool is_open() const;

	void set_vsync(bool vsync);
	bool is_vsync() const;

	void* get_native_window() const;

	Renderer& create_renderer();

	std::shared_ptr<GraphicsContext> get_context() const { return m_Context; }

	std::function<void()> m_OnWindowClose;

	class ImGuiImpl
	{
	private:
		ImGuiImpl(Window& window);
	public:
		~ImGuiImpl();
		void new_frame();
		void end_frame();

		friend class Window;
	private:
		Window& m_Window;
	};
	friend class Renderer;
private:
	std::unique_ptr<class ImGuiImpl> make_imgui_impl();

	void* m_Handle;
	bool m_IsVSync;
	WindowCreationInfo m_CreationInfo;
	std::shared_ptr<GraphicsContext> m_Context;
	std::unique_ptr<Renderer> m_Renderer;

};

}
