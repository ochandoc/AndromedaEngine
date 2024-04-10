#pragma once

#include "Andromeda/Misc/CoreMiscDefines.h"
#include "Andromeda/HAL/Types.h"
#include "Geometry.h"
#include "Andromeda/Graphics/RenderTarget.h"
#include "Andromeda/Graphics/Camera.h"
#include "Andromeda/Graphics/LightOld.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Andromeda/ECS/Scene.h"
#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Lights/PointLight.h"

namespace And
{
  class Window;
  class OldShader;
  class Triangle;
  class Geometry;
  class TransformComponent;
  struct OldShaderInfo;
  class UniformBuffer;

  struct Direction;

  class Renderer
  {
    NON_COPYABLE_CLASS(Renderer)
    NON_MOVABLE_CLASS(Renderer)
  public:
    Renderer() = default;
    virtual ~Renderer() = default;

    static std::shared_ptr<Renderer> CreateShared(Window& window);

    /**
     * @brief Must called at begining after window->update
     *
     */
    virtual void new_frame() = 0;

    /**
     * @brief Must called at end of the frame before swap buffers
     *
     */
    virtual void end_frame() = 0;

    /**
     * @brief Set the viewport of the window
     *
     * @param x position
     * @param y position
     * @param width size
     * @param height size
     */
    virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

    /**
     * @brief Set clear color value
     *
     * @param color
     */
    virtual void set_clear_color(float* color) = 0;

    virtual void set_camera(CameraBase* cam) = 0;

    virtual void draw_forward(EntityComponentSystem& entity) = 0;

  };

}
