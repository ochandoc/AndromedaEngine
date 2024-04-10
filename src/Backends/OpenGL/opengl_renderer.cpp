
#include "Backends/OpenGL/OpenGL.h"

#include "Andromeda/Graphics/Renderer.h"
#include "Backends/OpenGL/RendererOpenGL.h"
#include "Andromeda/HAL/Window.h"


#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui_impl_opengl3.h"

#include "Andromeda/Graphics/Triangle.h"
#include "Andromeda/Graphics/Geometry.h"

#include "Andromeda/UI/Plot/implot.h"

#include "Andromeda/Graphics/Shader.h"
#include "Andromeda/ECS/Components/TransformComponent.h"
#include "Andromeda/ECS/Components/MeshComponent.h"
#include "Backends/OpenGL/OpenGLTexture2D.h"
#include "Backends/OpenGL/opengl_uniform_buffer.h"
#include "Backends/OpenGL/OpenGLShader.h"
#include "Andromeda/Graphics/Lights/SpotLight.h"
#include "Andromeda/Graphics/Lights/AmbientLight.h"
#include "Andromeda/Graphics/Lights/DirectionalLight.h"
#include "Andromeda/Graphics/Lights/PointLight.h"

namespace And
{

  struct UniformBlockMatrices{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection; // 192
    glm::vec3 camera_position;
    //float padding; // 448 bytes, aligned to 28 blocks of 16 bytes
  };
  
  struct UniformBlockMatricesPointLight{
    glm::mat4 model;
    glm::mat4 proj_view_light[6];
    glm::mat4 proj_view_cam; 
    glm::vec3 camera_position; // 524 bytes
  };

  struct Direction {
      glm::vec3 dir[6];
  };


  std::shared_ptr<Renderer> Renderer::CreateShared(Window& window){

    switch(window.get_api_type()){
      case EGraphicsApiType::OpenGL:

        return std::make_shared<RendererOpenGL>(window);
      break;
      case EGraphicsApiType::DirectX11:
        return nullptr;
        //return std::make_shared<RendererDirectX>(window);
      break;
      default:
          assert(false);
          return nullptr;
      break;
    }
  }

}
  