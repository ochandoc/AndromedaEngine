#include "Backends/DirectX11/DirectX11Renderer.h"

#include "GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include "Backends/DirectX11/DirectX11IndexBuffer.h"
#include "Backends/DirectX11/DirectX11VertexBuffer.h"
#include "Backends/DirectX11/DirectX11Shader.h"
#include "Backends/DirectX11/DirectX11Texture2D.h"

namespace And
{
  static ID3D11Device* s_Device = nullptr;
  static ID3D11DeviceContext* s_DeviceContext = nullptr;

  DirectX11Renderer::DirectX11Renderer() 
    : m_Camera(nullptr)
  {
    m_ClearColor[0] = 0.0f;
    m_ClearColor[1] = 0.0f;
    m_ClearColor[2] = 0.0f;
    m_ClearColor[3] = 1.0f;

    std::vector<Vertex> BillboardVertices = {
      {
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f
      },
      {
        -0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, -1.0f, 
        0.0f, 1.0f
      },
      {
        0.5f, 0.5f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 1.0f
      },
      {
        0.5f, -0.5f, 0.0f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f
      }
    };

    std::vector<uint32> BillboardIndices = {
      0, 2, 1, 0, 3, 2
    };

    m_Billboard.VertexBuffer = DirectX11VertexBuffer::CreateShare(BillboardVertices);
    m_Billboard.IndexBuffer = DirectX11IndexBuffer::CreateShared(BillboardIndices);

    m_VSObjectData = DirectX11ConstantBuffer::CreateShared(sizeof(DirectX11::VertexShader::ObjectData));
    m_PSObjectData = DirectX11ConstantBuffer::CreateShared(sizeof(DirectX11::VertexShader::ObjectData));
    m_PSLightData = DirectX11ConstantBuffer::CreateShared(sizeof(DirectX11::PixelShader::LightData));

    m_LightTexture = DirectX11Texture2D::CreateShared("bulp_billboard.png");
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

    D3D11_DEPTH_STENCIL_DESC SkyboxDSDesc = {
      .DepthEnable = true,
      .DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO,
      .DepthFunc = D3D11_COMPARISON_LESS_EQUAL,
      .StencilEnable = false,
      .StencilReadMask = 0,
      .StencilWriteMask = 0,
      .FrontFace = {},
      .BackFace = {}
    };

    ComPtr<ID3D11DepthStencilState> SkyboxDepthStencilState;
    result = device->CreateDepthStencilState(&SkyboxDSDesc, SkyboxDepthStencilState.GetAddressOf());

    assert(SUCCEEDED(result));

    D3D11_DEPTH_STENCIL_DESC DSDesc = {
      .DepthEnable = true,
      .DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
      .DepthFunc = D3D11_COMPARISON_LESS_EQUAL,
      .StencilEnable = false,
      .StencilReadMask = 0,
      .StencilWriteMask = 0,
      .FrontFace = {},
      .BackFace = {}
    };

    ComPtr<ID3D11DepthStencilState> DepthStencilState;
    result = device->CreateDepthStencilState(&DSDesc, DepthStencilState.GetAddressOf());

    assert(SUCCEEDED(result));

    D3D11_BLEND_DESC LightBlendDesc = {
      .AlphaToCoverageEnable = false,
      .IndependentBlendEnable = false,
      .RenderTarget = {
        { // 0
          .BlendEnable = true,
          .SrcBlend = D3D11_BLEND_ONE,
          .DestBlend = D3D11_BLEND_ONE,
          .BlendOp = D3D11_BLEND_OP_ADD,
          .SrcBlendAlpha = D3D11_BLEND_ONE,
          .DestBlendAlpha = D3D11_BLEND_ONE,
          .BlendOpAlpha = D3D11_BLEND_OP_ADD,
          .RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
        }
      },
    };

    ComPtr<ID3D11BlendState> LightBlendState;

    result = device->CreateBlendState(&LightBlendDesc, LightBlendState.GetAddressOf());

    assert(SUCCEEDED(result));

    D3D11_BLEND_DESC ZeroBlendDesc = {
      .AlphaToCoverageEnable = false,
      .IndependentBlendEnable = false,
      .RenderTarget = {
        { // 0
          .BlendEnable = false,
          .SrcBlend = D3D11_BLEND_ONE,
          .DestBlend = D3D11_BLEND_ZERO,
          .BlendOp = D3D11_BLEND_OP_ADD,
          .SrcBlendAlpha = D3D11_BLEND_ONE,
          .DestBlendAlpha = D3D11_BLEND_ZERO,
          .BlendOpAlpha = D3D11_BLEND_OP_ADD,
          .RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
        }
      },
    };

    ComPtr<ID3D11BlendState> ZeroBlendState;

    result = device->CreateBlendState(&ZeroBlendDesc, ZeroBlendState.GetAddressOf());

    assert(SUCCEEDED(result));

    s_Device = device.Get();
    s_DeviceContext = deviceContext.Get();

    std::shared_ptr<DirectX11Renderer> renderer(new DirectX11Renderer);
    renderer->m_SwapChain = swapChain;
    renderer->m_Device = device;
    renderer->m_DeviceContext = deviceContext;
    renderer->m_RasterizerState = RasterizerState;
    renderer->m_DepthStencilView = depthStencilView;
    renderer->m_RenderTargetView = RenderTargetView;
    renderer->m_DepthStencil.LessEqual = DepthStencilState;
    renderer->m_LightsBlendState = LightBlendState;
    renderer->m_ZeroBlendState = ZeroBlendState;

    renderer->set_viewport(0, 0, window.get_width(), window.get_height());

    renderer->m_Skybox.DepStencilState = SkyboxDepthStencilState;
    renderer->m_Skybox.Mesh = std::make_shared<Mesh>(RawMesh::CreateSkybox());
    renderer->m_Skybox.Enabled = false;
    return renderer;
  }

  void DirectX11Renderer::set_camera(CameraBase* camera)
  {
    m_Camera = camera;
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
    if (!m_Camera) return;

    SkyboxPass();

    /**  Ambient light */
    {
      /**  Enable Blend state */
      m_DeviceContext->OMSetBlendState(m_ZeroBlendState.Get(), nullptr, 0xffffffff);
      /**  Enable depth stencil state */
      m_DeviceContext->OMSetDepthStencilState(m_DepthStencil.LessEqual.Get(), 0);
      for (auto& [mesh_component, transform, matComp] : ecs.get_components<MeshComponent, TransformComponent, MaterialComponent>())
      {
        ObjectPass(mesh_component->GetMesh().get(), transform, matComp->GetMaterial().get(), "Light.Ambient");
      }
      /**  Disable Blend state */
      m_DeviceContext->OMSetBlendState(m_LightsBlendState.Get(), nullptr, 0xffffffff);
    }
    
    /**  Directional light pass */
    for (auto& [light] : ecs.get_components<DirectionalLight>())
    {
      if (!light->GetEnabled()) continue;

      for (auto& [mesh_component, transform, matComp] : ecs.get_components<MeshComponent, TransformComponent, MaterialComponent>())
      {
        /**  Upload pixel shader light buffer */
        {
          DirectX11::PixelShader::LightData* PSLightData;
          D3D11_MAPPED_SUBRESOURCE PSLightMappedData;
          m_DeviceContext->Map(m_PSLightData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &PSLightMappedData);
          PSLightData = (DirectX11::PixelShader::LightData*)PSLightMappedData.pData;
          PSLightData->Direction = glm::make_vec3(light->GetDirection());
          PSLightData->DiffuseColor = glm::make_vec3(light->GetDiffuseColor());
          PSLightData->SpecularStrength = light->GetSpecularStrength();
          PSLightData->SpecularShininess = light->GetSpecularShininess();
          m_DeviceContext->Unmap(m_PSLightData->GetBuffer(), 0);
        }

        ObjectPass(mesh_component->GetMesh().get(), transform, matComp->GetMaterial().get(), "Light.Directional");
      }
    }

    /**  Spot light pass */
    for (auto& [light] : ecs.get_components<SpotLight>())
    {
      BillboardPass(glm::make_vec3(light->GetPosition()), glm::vec2(0.75f), m_LightTexture.get());

      if (!light->GetEnabled()) continue;

      for (auto& [mesh_component, transform, matComp] : ecs.get_components<MeshComponent, TransformComponent, MaterialComponent>())
      {
        /**  Upload pixel shader light buffer */
        {
          DirectX11::PixelShader::LightData* PSLightData;
          D3D11_MAPPED_SUBRESOURCE PSLightMappedData;
          m_DeviceContext->Map(m_PSLightData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &PSLightMappedData);
          PSLightData = (DirectX11::PixelShader::LightData*)PSLightMappedData.pData;
          PSLightData->Position = glm::make_vec3(light->GetPosition());
          PSLightData->ConstantAttenuation = light->GetConstantAtt();
          PSLightData->Direction = glm::make_vec3(light->GetDirection());
          PSLightData->LinearAttenuation = light->GetLinearAtt();
          PSLightData->DiffuseColor = glm::make_vec3(light->GetDiffuseColor());
          PSLightData->QuadraticAttenuation = light->GetQuadraticAtt();
          PSLightData->SpecularStrength = light->GetSpecularStrength();
          PSLightData->SpecularShininess = light->GetSpecularShininess();
          PSLightData->InnerConeAngle = light->GetCuttOff();
          PSLightData->OuterConeAngle = light->GetOuterCuttOff();
          m_DeviceContext->Unmap(m_PSLightData->GetBuffer(), 0);
        }

        ObjectPass(mesh_component->GetMesh().get(), transform, matComp->GetMaterial().get(), "Light.Spot");
      }
    }

    /**  Point light pass */
    for (auto& [light] : ecs.get_components<PointLight>())
    {
      BillboardPass(glm::make_vec3(light->GetPosition()), glm::vec2(0.75f), m_LightTexture.get());

      if (!light->GetEnabled()) continue;

      for (auto& [mesh_component, transform, matComp] : ecs.get_components<MeshComponent, TransformComponent, MaterialComponent>())
      {
        /**  Upload pixel shader light buffer */
        {
          DirectX11::PixelShader::LightData* PSLightData;
          D3D11_MAPPED_SUBRESOURCE PSLightMappedData;
          m_DeviceContext->Map(m_PSLightData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &PSLightMappedData);
          PSLightData = (DirectX11::PixelShader::LightData*)PSLightMappedData.pData;
          PSLightData->Position = glm::make_vec3(light->GetPosition());
          PSLightData->ConstantAttenuation = light->GetConstantAtt();
          PSLightData->LinearAttenuation = light->GetLinearAtt();
          PSLightData->DiffuseColor = glm::make_vec3(light->GetDiffuseColor());
          PSLightData->QuadraticAttenuation = light->GetQuadraticAtt();
          PSLightData->SpecularStrength = light->GetSpecularStrength();
          PSLightData->SpecularShininess = light->GetSpecularShininess();
          m_DeviceContext->Unmap(m_PSLightData->GetBuffer(), 0);
        }

        ObjectPass(mesh_component->GetMesh().get(), transform, matComp->GetMaterial().get(), "Light.Point");
      }
    }
  }

  void DirectX11Renderer::SkyboxPass()
  {
    if (!m_Skybox.Enabled) return;
    if (!m_Skybox.Texture) return;

    DirectX11SkyboxTexture* SkyboxTexture = static_cast<DirectX11SkyboxTexture*>(m_Skybox.Texture.get());
    DirectX11VertexBuffer* VB = static_cast<DirectX11VertexBuffer*>(m_Skybox.Mesh->GetVertexBuffer());
    DirectX11IndexBuffer* IB = static_cast<DirectX11IndexBuffer*>(m_Skybox.Mesh->GetIndexBuffer());
    std::shared_ptr<DirectX11Shader> shader = m_ShaderLibrary.GetForwardShader("Skybox");

    /**  Upload object data into the constant buffer */
    DirectX11::VertexShader::ObjectData ObjectData;
    ObjectData.projection = glm::transpose(glm::make_mat4(m_Camera->GetProjectionMatrix()));
    ObjectData.view = glm::transpose(glm::make_mat4(m_Camera->GetViewMatrix()));

    D3D11_MAPPED_SUBRESOURCE ObjectBuffData;
    m_DeviceContext->Map(m_VSObjectData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ObjectBuffData);
    memcpy(ObjectBuffData.pData, (void*)&ObjectData, sizeof(DirectX11::VertexShader::ObjectData));
    m_DeviceContext->Unmap(m_VSObjectData->GetBuffer(), 0);

    /**  Setup draw info */
    m_DeviceContext->IASetInputLayout(shader->GetVertexShader()->GetInputLayout());
    m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    /**  Vertex Shader */
    m_DeviceContext->VSSetShader(shader->GetVertexShader()->GetShader(), nullptr, 0);
    ID3D11Buffer* VSConstantBuffers[] = { m_VSObjectData->GetBuffer() };
    m_DeviceContext->VSSetConstantBuffers(0, 1, VSConstantBuffers);

    /**  Pixel Shader */
    m_DeviceContext->PSSetShader(shader->GetPixelShader()->GetShader(), nullptr, 0);
    ID3D11ShaderResourceView* PSViews[] = { SkyboxTexture->GetView() };
    m_DeviceContext->PSSetShaderResources(0, 1, PSViews);
    ID3D11SamplerState* PSSamplerStates[] = { SkyboxTexture->GetSampler() };

    /**  Depth Stencil State */
    m_DeviceContext->OMSetDepthStencilState(m_Skybox.DepStencilState.Get(), 0);
    /**  Blend state */
    m_DeviceContext->OMSetBlendState(m_ZeroBlendState.Get(), nullptr, 0xffffffff);

    ID3D11Buffer* VertexBuffers[] = { VB->GetBuffer(), };
    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;
    m_DeviceContext->IASetVertexBuffers(0, 1, VertexBuffers, &stride, &offset);
    m_DeviceContext->IASetIndexBuffer(IB->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

    m_DeviceContext->DrawIndexed((uint32)IB->GetNumIndices(), 0, 0);
  }

  void DirectX11Renderer::ObjectPass(Mesh* mesh, TransformComponent* tr, Material* material, const std::string& shader)
  {
    DirectX11VertexBuffer* VB = static_cast<DirectX11VertexBuffer*>(mesh->GetVertexBuffer());
    DirectX11IndexBuffer* IB = static_cast<DirectX11IndexBuffer*>(mesh->GetIndexBuffer());
    DirectX11Texture2D* ColorTexture = static_cast<DirectX11Texture2D*>(material->GetColorTexture().get());
    std::shared_ptr<DirectX11Shader> Shader = m_ShaderLibrary.GetForwardShader(shader);

    m_VSConstantBuffers.clear();
    m_PSConstantBuffers.clear();

    /**  Upload vertex shader object data */
    {
      glm::vec3 position = glm::make_vec3(tr->position);
      glm::vec3 rotation = glm::make_vec3(tr->rotation);
      glm::vec3 scale = glm::make_vec3(tr->scale);

      glm::mat4 model = glm::identity<glm::mat4>();
      model = glm::translate(model, position);
      model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, scale);

      DirectX11::VertexShader::ObjectData* VSObjectData;
      D3D11_MAPPED_SUBRESOURCE VSObjectMappedData;
      m_DeviceContext->Map(m_VSObjectData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &VSObjectMappedData);
      VSObjectData = (DirectX11::VertexShader::ObjectData*)VSObjectMappedData.pData;
      VSObjectData->model = glm::transpose(model);
      VSObjectData->view = glm::transpose(glm::make_mat4(m_Camera->GetViewMatrix()));
      VSObjectData->projection = glm::transpose(glm::make_mat4(m_Camera->GetProjectionMatrix()));
      m_DeviceContext->Unmap(m_VSObjectData->GetBuffer(), 0);

      m_VSConstantBuffers.push_back(m_VSObjectData->GetBuffer());
    }

    /**  Upload pixel shader object buffer */
    {
      DirectX11::PixelShader::ObjectData* PSObjectData;
      D3D11_MAPPED_SUBRESOURCE PSObjectMappedData;
      m_DeviceContext->Map(m_PSObjectData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &PSObjectMappedData);
      PSObjectData = (DirectX11::PixelShader::ObjectData*)PSObjectMappedData.pData;
      PSObjectData->Color = glm::make_vec4(material->GetColor());
      PSObjectData->CameraPos = glm::make_vec3(m_Camera->GetPosition());
      PSObjectData->HasColorTxture = (ColorTexture) ? 1 : 0;
      m_DeviceContext->Unmap(m_PSObjectData->GetBuffer(), 0);

      m_PSConstantBuffers.push_back(m_PSObjectData->GetBuffer());
    }

    /**  Upload pixel shader buffer */
    {
      if (shader.find("Light") != std::string::npos)
        m_PSConstantBuffers.push_back(m_PSLightData->GetBuffer());
    }

    /**  Configure input assembly */
    m_DeviceContext->IASetInputLayout(Shader->GetVertexShader()->GetInputLayout());
    m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11Buffer* VertexBuffers[] = { VB->GetBuffer(), };
    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;
    m_DeviceContext->IASetVertexBuffers(0, 1, VertexBuffers, &stride, &offset);
    m_DeviceContext->IASetIndexBuffer(IB->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

    /**  Configure vertex shader stage */
    m_DeviceContext->VSSetShader(Shader->GetVertexShader()->GetShader(), nullptr, 0);
    //ID3D11Buffer* VSConstantBuffers[] = { m_VSObjectData->GetBuffer() };
    m_DeviceContext->VSSetConstantBuffers(0, (uint32)m_VSConstantBuffers.size(), m_VSConstantBuffers.data());

    /**  Configure pixel shader stage */
    m_DeviceContext->PSSetShader(Shader->GetPixelShader()->GetShader(), nullptr, 0);
    //ID3D11Buffer* PSConstantBuffers[] = { m_PSObjectData->GetBuffer() };
    m_DeviceContext->PSSetConstantBuffers(0, (uint32)m_PSConstantBuffers.size(), m_PSConstantBuffers.data());
    ID3D11ShaderResourceView* PSViews[] = { (ColorTexture) ? ColorTexture->GetView() : nullptr };
    m_DeviceContext->PSSetShaderResources(0, 1, PSViews);
    ID3D11SamplerState* PSSamplers[] = { (ColorTexture) ? ColorTexture->GetSampler() : nullptr };
    m_DeviceContext->PSSetSamplers(0, 1, PSSamplers);

    /**  Depth stencil state */
    //m_DeviceContext->OMSetDepthStencilState(m_DepthStencilState.Get(), 0);

    m_DeviceContext->DrawIndexed((uint32)IB->GetNumIndices(), 0, 0);
  }

  void DirectX11Renderer::BillboardPass(const glm::vec3& pos, const glm::vec2 size, Texture* tex)
  {
    std::shared_ptr<DirectX11Shader> shader = m_ShaderLibrary.GetForwardShader("Billboard");
    DirectX11Texture2D* ColorTexture = static_cast<DirectX11Texture2D*>(tex);
    ID3D11BlendState* LastBlendState = nullptr;
    float LastBlendFactor[4];
    uint32 LastBlendMask = 0xffffffff;

    /**  Upload vertex shader object buffer */
    {
      glm::vec3 rotation(0.0f, 0.0f, 0.0f);
      glm::vec3 scale(size.x, size.y, 1.0f);

      glm::mat4 model = glm::identity<glm::mat4>();
      model = glm::translate(model, pos);
      model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, scale);

      DirectX11::VertexShader::ObjectData* VSObjectData;
      D3D11_MAPPED_SUBRESOURCE VSObjectMappedData;
      m_DeviceContext->Map(m_VSObjectData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &VSObjectMappedData);
      VSObjectData = (DirectX11::VertexShader::ObjectData*)VSObjectMappedData.pData;
      VSObjectData->model = glm::transpose(model);
      VSObjectData->view = glm::transpose(glm::make_mat4(m_Camera->GetViewMatrix()));
      VSObjectData->projection = glm::transpose(glm::make_mat4(m_Camera->GetProjectionMatrix()));
      m_DeviceContext->Unmap(m_VSObjectData->GetBuffer(), 0);
    }

    /**  Upload pixels shader object buffer */
    {
      DirectX11::PixelShader::ObjectData* PSObjectData;
      D3D11_MAPPED_SUBRESOURCE PSObjectMappedData;
      m_DeviceContext->Map(m_PSObjectData->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &PSObjectMappedData);
      PSObjectData = (DirectX11::PixelShader::ObjectData*)PSObjectMappedData.pData;
      PSObjectData->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
      PSObjectData->CameraPos = glm::make_vec3(m_Camera->GetPosition());
      PSObjectData->HasColorTxture = (ColorTexture) ? 1 : 0;
      m_DeviceContext->Unmap(m_PSObjectData->GetBuffer(), 0);
    }

    /**  Configure input assembly */
    m_DeviceContext->IASetInputLayout(shader->GetVertexShader()->GetInputLayout());
    m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ID3D11Buffer* VertexBuffers[] = { m_Billboard.VertexBuffer->GetBuffer() };
    uint32 stride = sizeof(Vertex);
    uint32 offset = 0;
    m_DeviceContext->IASetVertexBuffers(0, 1, VertexBuffers, &stride, &offset);
    m_DeviceContext->IASetIndexBuffer(m_Billboard.IndexBuffer->GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

    /**  Configure vertex shader stage */
    m_DeviceContext->VSSetShader(shader->GetVertexShader()->GetShader(), nullptr, 0);
    ID3D11Buffer* VSConstantBuffers[] = { m_VSObjectData->GetBuffer() };
    m_DeviceContext->VSSetConstantBuffers(0, 1, VSConstantBuffers);

    /**  Configure pixel shader stage */
    m_DeviceContext->PSSetShader(shader->GetPixelShader()->GetShader(), nullptr, 0);
    ID3D11Buffer* PSConstantBuffers[] = { m_PSObjectData->GetBuffer() };
    m_DeviceContext->PSSetConstantBuffers(0, 1, PSConstantBuffers);
    ID3D11ShaderResourceView* PSViews[] = { (ColorTexture) ? ColorTexture->GetView() : nullptr };
    m_DeviceContext->PSSetShaderResources(0, 1, PSViews);
    ID3D11SamplerState* PSSamplers[] = { (ColorTexture) ? ColorTexture->GetSampler() : nullptr };
    m_DeviceContext->PSSetSamplers(0, 1, PSSamplers);

    /**  Depth stencil state */
    m_DeviceContext->OMSetDepthStencilState(m_DepthStencil.LessEqual.Get(), 0);
    /**  Blend state */
    m_DeviceContext->OMGetBlendState(&LastBlendState, LastBlendFactor, &LastBlendMask);
    m_DeviceContext->OMSetBlendState(m_ZeroBlendState.Get(), nullptr, 0xffffffff);

    m_DeviceContext->DrawIndexed((uint32)m_Billboard.IndexBuffer->GetNumIndices(), 0, 0);

    /**  Restore blend state */
    m_DeviceContext->OMSetBlendState(LastBlendState, LastBlendFactor, LastBlendMask);
  }

  void DirectX11Renderer::enable_skybox(bool value)
  {
    m_Skybox.Enabled = value;
  }

  void DirectX11Renderer::set_skybox_texture(std::shared_ptr<SkyboxTexture> texture)
  {
    m_Skybox.Texture = texture;
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