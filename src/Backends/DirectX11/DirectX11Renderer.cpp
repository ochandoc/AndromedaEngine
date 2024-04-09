#include "Backends/DirectX11/DirectX11Renderer.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

namespace And
{
  DirectX11Renderer::DirectX11Renderer() 
  {
    m_ClearColor[0] = 0.0f;
    m_ClearColor[1] = 0.0f;
    m_ClearColor[2] = 0.0f;
    m_ClearColor[3] = 0.0f;
  }

  DirectX11Renderer::~DirectX11Renderer() {}

  std::shared_ptr<DirectX11Renderer> DirectX11Renderer::CreateShared(Window& window)
  {
    HRESULT result = S_OK;

    DXGI_SWAP_CHAIN_DESC swapChainDescritor = {
      .BufferDesc = {
          .Width = window.get_width(),
          .Height = window.get_height(),
          .RefreshRate = { .Numerator = 60, .Denominator = 1},
          .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
          .ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
          .Scaling = DXGI_MODE_SCALING_UNSPECIFIED
      },
      .SampleDesc = { .Count = 1, .Quality = 0 },
      .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
      .BufferCount = 1,
      .OutputWindow = glfwGetWin32Window((GLFWwindow*)window.get_native_window()),
      .Windowed = TRUE,
      .SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
      .Flags = 0
    };

    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;

    result = D3D11CreateDeviceAndSwapChain(
      NULL,
      D3D_DRIVER_TYPE_HARDWARE,
      NULL,
      0,
      NULL,
      0,
      D3D11_SDK_VERSION,
      &swapChainDescritor,
      swapChain.GetAddressOf(),
      device.GetAddressOf(),
      0,
      deviceContext.GetAddressOf()
    );

    return std::shared_ptr<DirectX11Renderer>();
  }

  void DirectX11Renderer::set_camera(CameraBase* camera)
  {
  }

  void DirectX11Renderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
  {
  }

  void DirectX11Renderer::set_clear_color(float* color)
  {
  }

  void DirectX11Renderer::new_frame()
  {
  }

  void DirectX11Renderer::end_frame()
  {
  }

  void DirectX11Renderer::draw_forward(EntityComponentSystem& ecs)
  {
  }


}