#include "Backends/DirectX11/DirectX11Renderer.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Backends/DirectX11/DirectX11IndexBuffer.h"
#include "Backends/DirectX11/DriectX11VertexBuffer.h"
#include "Backends/DirectX11/DirectX11Shader.h"

namespace And
{
  static ID3D11Device* s_Device = nullptr;
  static ID3D11DeviceContext* s_DeviceContext = nullptr;

  DirectX11Renderer::DirectX11Renderer() 
  {
    m_ClearColor[0] = 1.0f;
    m_ClearColor[1] = 0.0f;
    m_ClearColor[2] = 0.0f;
    m_ClearColor[3] = 1.0f;
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

    assert(SUCCEEDED(result));

    ID3D11Texture2D* BackTexture = nullptr;

    result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackTexture));

    assert(SUCCEEDED(result));

    ComPtr<ID3D11RenderTargetView> RenderTargetView;
    result = device->CreateRenderTargetView(BackTexture, NULL, RenderTargetView.GetAddressOf());

    assert(SUCCEEDED(result));

    D3D11_TEXTURE2D_DESC DepthStencilDesc = {
      .Width = window.get_width(),
      .Height = window.get_height(),
      .MipLevels = 1,
      .ArraySize = 1,
      .Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
      .SampleDesc = { .Count = 1, .Quality = 0 },
      .Usage = D3D11_USAGE_DEFAULT,
      .BindFlags = D3D11_BIND_DEPTH_STENCIL,
      .CPUAccessFlags = 0,
      .MiscFlags = 0
    };

    ComPtr<ID3D11Texture2D> depthStencilTexture;
    result = device->CreateTexture2D(&DepthStencilDesc, NULL, depthStencilTexture.GetAddressOf());

    assert(SUCCEEDED(result));

    ComPtr<ID3D11DepthStencilView> depthStencilView;
    
    result = device->CreateDepthStencilView(depthStencilTexture.Get(),  NULL, depthStencilView.GetAddressOf());

    assert(SUCCEEDED(result));

    deviceContext->OMSetRenderTargets(1, RenderTargetView.GetAddressOf(), depthStencilView.Get());

    D3D11_RASTERIZER_DESC RasterizerStateDesc = {
      .FillMode = D3D11_FILL_SOLID,
      .CullMode = D3D11_CULL_BACK,
      .FrontCounterClockwise = true,
      .DepthBias = D3D11_DEFAULT_DEPTH_BIAS,
      .DepthBiasClamp = D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
      .SlopeScaledDepthBias = D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
      .DepthClipEnable = TRUE,
      .ScissorEnable = FALSE,
      .MultisampleEnable = FALSE,
      .AntialiasedLineEnable = FALSE
    };

    ComPtr<ID3D11RasterizerState> RasterizerState;
    result = device->CreateRasterizerState(&RasterizerStateDesc, RasterizerState.GetAddressOf());

    assert(SUCCEEDED(result));

    std::shared_ptr<DirectX11Renderer> renderer(new DirectX11Renderer);
    renderer->m_SwapChain = swapChain;
    renderer->m_Device = device;
    renderer->m_DeviceContext = deviceContext;
    renderer->m_RasterizerState = RasterizerState;
    renderer->m_DepthStencilView = depthStencilView;
    renderer->m_RenderTargetView = RenderTargetView;

    s_Device = device.Get();
    s_DeviceContext = deviceContext.Get();

    renderer->set_viewport(0, 0, window.get_width(), window.get_height());

    return renderer;
  }

  void DirectX11Renderer::set_camera(CameraBase* camera)
  {

  }

  void DirectX11Renderer::set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
  {
    D3D11_VIEWPORT viewport = {
      .TopLeftX = (float)x,
      .TopLeftY = (float)y,
      .Width = (float)width,
      .Height = (float)height,
      .MinDepth = 0.0f,
      .MaxDepth = 1.0f
    };

    m_DeviceContext->RSSetViewports(1, &viewport);
  }

  void DirectX11Renderer::set_clear_color(float* color)
  {
    m_ClearColor[0] = color[0];
    m_ClearColor[1] = color[1];
    m_ClearColor[2] = color[2];
    m_ClearColor[3] = color[3];
  }

  void DirectX11Renderer::new_frame()
  {
    m_DeviceContext->RSSetState(m_RasterizerState.Get());
    m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), m_ClearColor);
    m_DeviceContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
  }

  void DirectX11Renderer::end_frame()
  {
    m_SwapChain->Present(1, NULL);
  }

  void DirectX11Renderer::draw_forward(EntityComponentSystem& ecs)
  {
  }

  void DirectX11Renderer::Draw(VertexBuffer* vb, IndexBuffer* ib, Shader* s)
  {
    DirectX11VertexBuffer* dx11_vb = static_cast<DirectX11VertexBuffer*>(vb);
    DirectX11IndexBuffer* dx11_ib = static_cast<DirectX11IndexBuffer*>(ib);
    DirectX11Shader* dx11_s = static_cast<DirectX11Shader*>(s);

    m_DeviceContext->IASetInputLayout(dx11_s->GetVertexShader()->GetInputLayout());
    m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    m_DeviceContext->VSSetShader(dx11_s->GetVertexShader()->GetShader(), NULL, 0);
    m_DeviceContext->PSSetShader(dx11_s->GetPixelShader()->GetShader(), NULL, 0);

    ID3D11Buffer* VertexBuffers[] = { dx11_vb->GetBuffer(), };
    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;
    m_DeviceContext->IASetVertexBuffers(0, 1, VertexBuffers, &stride, &offset);
    m_DeviceContext->IASetIndexBuffer(dx11_ib->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

    m_DeviceContext->DrawIndexed((uint32)dx11_ib->GetNumIndices(), 0, 0);
  }

  ID3D11Device* DirectX11Renderer::GetDevice()
  {
    return s_Device;
  }

  ID3D11DeviceContext* DirectX11Renderer::GetDeviceContext()
  {
    return s_DeviceContext;
  }


}