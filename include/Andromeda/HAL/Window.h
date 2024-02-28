#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"

#include "Andromeda/Events/Events.h"

#include "Andromeda/HAL/KeyCodes.h"

#include "Andromeda/TaskSystem/TaskSystem.h"
#include "Andromeda/Graphics/RenderTarget.h"

DECLARE_MULTICAST_DELEGATE_TWO_PARAM(OnWindowResizeSignature, uint32, width, uint32, height)
DECLARE_MULTICAST_DELEGATE(OnWindowCloseSignature)

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

	void set_size(uint32 width, uint32 height);

	uint32 get_width() const;
	uint32 get_height() const;

	void* get_native_window();

	void update();
	void swap_buffers();

	float get_delta_time() const;

	std::shared_ptr<GraphicsContext> get_context() const;

	std::function<void(WorkerThreadData& Data)> get_worker_function();

	OnWindowResizeSignature OnWindowResize;
	OnWindowCloseSignature OnWindowClose;

	friend class Renderer;
	friend class Input;
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	
	std::unique_ptr<struct WindowData> m_Data;
};

}
