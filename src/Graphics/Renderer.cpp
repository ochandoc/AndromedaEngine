#include <utility>

#include "Graphics/Renderer.h"
#include "Core/Window.h"

namespace And
{

Renderer::Renderer(Window& window) : m_Window(window)
{
	m_ImGuiImpl = std::move(window.make_imgui_impl());
}

Renderer::~Renderer() {}

}
