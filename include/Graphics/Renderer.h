#pragma once
#include "Shader.h"
#include <vector>

namespace And
{

class Renderer
{
public:
  Renderer();
  Renderer(const Renderer&) = delete;
  Renderer(const Renderer&&) = delete;

  virtual ~Renderer();

  Renderer& operator =(const Renderer&) = delete;
  Renderer& operator =(Renderer&&) = delete;

  
  /**
   * @brief Set the viewport position and size
   * 
   * @param x window position X
   * @param y window position Y
   * @param width window width 
   * @param height window height
   */
  virtual void set_viewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

    
  /**
   * @brief Set the clear color of the window
   * 
   * @param color 
   */
  virtual void set_clear_color(float* color) = 0;

  /**
  * @brief Cleans color buffer and deep buffer
  * 
  */
  virtual void clear() = 0;

   /**
   * @brief Create a Shader object
   * 
   * @param s_info 
   * @return std::shared_ptr<Shader> 
   */
  virtual std::shared_ptr<Shader> createShader(std::vector<ShaderInfo> s_info) = 0;

  /**
   * @brief Shows simple triangle
   * 
   */
  virtual void showDemo() = 0;
};

}
