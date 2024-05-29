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
class RendererOpenGL;
class GraphicsContext;

/**
 * @enum EGraphicsApiType
 * @brief Enumerates the different types of graphics APIs supported.
 *
 * This enumeration lists the types of graphics APIs that can be used for rendering.
 */
enum class EGraphicsApiType : uint8
{
	OpenGL,
	DirectX11,
};

class Window
{
	NON_COPYABLE_CLASS(Window)
	NON_MOVABLE_CLASS(Window)
private:
	Window();

public:
	~Window();
	/**
	 * @brief Creates a Window
	 * 
	 * @param e Andromeda engine reference
	 * @param w Window width
	 * @param h Window height
	 * @param title Window title
	 * @return std::shared_ptr<Window> 
	 */
	static std::shared_ptr<Window> make(Engine& e, uint32 w, uint32 h, const char* title, EGraphicsApiType api);

	/**
	 * @brief Return if window is open
	 * 
	 * @return true 
	 * @return false 
	 */
	bool is_open() const;

	/**
	 * @brief Set the vsync active
	 * 
	 * @param vsync 
	 */
	void set_vsync(bool vsync);

	/**
	 * @brief Check if vsync is active
	 * 
	 * @return true 
	 * @return false 
	 */
	bool is_vsync() const;

	/**
	 * @brief Set the size of thw window
	 * 
	 * @param width 
	 * @param height 
	 */
	void set_size(uint32 width, uint32 height);

	/**
	 * @brief Get the width of the window
	 * 
	 * @return uint32 
	 */
	uint32 get_width() const;

	/**
	 * @brief Get the height of the window
	 * 
	 * @return uint32 
	 */
	uint32 get_height() const;

	/**
	 * @brief Get the native window object
	 * 
	 * @return void* 
	 */
	void* get_native_window();

	/**
	 * @brief Update window, must called every frame at begining
	 * 
	 */
	void update();

	/**
	 * @brief Swap draw buffers
	 * 
	 */
	void swap_buffers();

	/**
	 * @brief Get the delta time
	 * 
	 * @return float 
	 */
	float get_delta_time() const;

	inline EGraphicsApiType get_api_type() const { return m_ApiType; }

	/**
	 * @brief Get the graphics context object
	 * 
	 * @return std::shared_ptr<GraphicsContext> 
	 */
	std::shared_ptr<GraphicsContext> get_context() const;

	/**
	 * @brief Get the job system thread of current graphics api (OpenGL)
	 * 
	 * @return std::function<void(WorkerThreadData& Data)> 
	 */
	std::function<void(WorkerThreadData& Data)> get_worker_function();

	/**
	 * @brief On Window resize event
	 * 
	 */
	OnWindowResizeSignature OnWindowResize;

	/**
	 * @brief On Window close event
	 * 
	 */
	OnWindowCloseSignature OnWindowClose;

	friend class Renderer;
	friend class RendererOpenGL;
	friend class Input;
private:
	void imgui_start();
	void imgui_end();

	void new_frame();
	void end_frame();

	
	std::unique_ptr<struct WindowData> m_Data;
	EGraphicsApiType m_ApiType;
};

EGraphicsApiType GetGraphicApiType();
}
