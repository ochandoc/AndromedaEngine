#pragma once

#include "Andromeda/Graphics/Renderer.h"

#include "Backends/DirectX11/DirectX11.h"
#include "Backends/DirectX11/DirectX11ConstantBuffer.h"
#include "Backends/DirectX11/DirectX11ShaderInputStructs.h"
#include "Backends/DirectX11/DirectX11Texture2D.h"

namespace And
{
  class DirectX11Renderer : public Renderer
  {
  private:
    DirectX11Renderer();

  public:
    virtual ~DirectX11Renderer();

    static std::shared_ptr<DirectX11Renderer> CreateShared(Window& window);

    virtual void set_camera(CameraBase* camera) override;

    virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
    virtual void set_clear_color(float* color) override;

    virtual void new_frame() override;
    virtual void end_frame() override;

    virtual void draw_forward(EntityComponentSystem& ecs) override;

    virtual void Draw(VertexBuffer* vb, IndexBuffer* ib, Shader* s) override;

    static ID3D11Device* GetDevice();
    static ID3D11DeviceContext* GetDeviceContext();

  private:
    float m_ClearColor[4];
    ComPtr<IDXGISwapChain> m_SwapChain;
    ComPtr<ID3D11Device> m_Device;
    ComPtr<ID3D11DeviceContext> m_DeviceContext;
    ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
    ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
    ComPtr<ID3D11RasterizerState> m_RasterizerState;
    std::shared_ptr<DirectX11ConstantBuffer> m_ObjectConstantBuffer;
    std::shared_ptr<DirectX11Texture2D> m_Tex;
    CameraBase* m_Camera;
  };
}