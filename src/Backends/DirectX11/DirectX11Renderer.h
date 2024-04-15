#pragma once

#include "Andromeda/Graphics/Renderer.h"

#include "Backends/DirectX11/DirectX11.h"
#include "Backends/DirectX11/DriectX11VertexBuffer.h"
#include "Backends/DirectX11/DirectX11IndexBuffer.h"
#include "Backends/DirectX11/DirectX11ConstantBuffer.h"
#include "Backends/DirectX11/DirectX11ShaderInputStructs.h"
#include "Backends/DirectX11/DirectX11Texture2D.h"
#include "Backends/DirectX11/DirectX11SkyboxTexture.h"
#include "Backends/DirectX11/DirectX11Shader.h"

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

    virtual void enable_skybox(bool value) override;
    virtual void set_skybox_texture(std::shared_ptr<SkyboxTexture> texture) override;

    void SkyboxPass();
    void ObjectPass();

    virtual void Draw(Mesh* mesh, Shader* s) override;

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

    struct {
      bool Enabled;
      std::shared_ptr<Mesh> Mesh;
      std::shared_ptr<DirectX11Shader> Shader;
      std::shared_ptr<SkyboxTexture> Texture;
      ComPtr<ID3D11DepthStencilState> DepStencilState;
    } m_Skybox;
    std::shared_ptr<DirectX11ConstantBuffer> m_ObjectConstantBuffer;
    CameraBase* m_Camera;
  };
}