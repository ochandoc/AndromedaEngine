#include <assert.h>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>

#include "Common/Engine.h"
#include "Common/Window.h"
#include "Common/GraphicsContext.h"
#include "Common/Renderer.h"
#include "Common/Shader.h"

#include "Common/Input.h"

#include <windows.h>


// que te parece si hacemos que la ventana tenga una instancia del input, es muy poarecido a lo que esta diciendo el
// hacemos lo del input de mientras o que
// espera porqie si nos ponemos focus no atendemos y esta diciendo cosas importantes


int main(int argc, char** argv){
  And::Engine e;

  std::shared_ptr<And::Window> window = And::Window::make(e, 1024, 720, "Andromeda Engine");
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer g_renderer(*window);


  // Creamos el shader
  std::shared_ptr<And::Shader> g_shader = g_renderer.createShader();

  char* error = nullptr;
  // Subimos los shader que queramos y comprobamos que no hay error
  g_shader->upload_shader(And::Shader_Vertex,"../../data/vshader.vs");
  error = g_shader->get_upload_shader_error();
  if(error){ 
    printf("Error: %s\n", error);
  }
  
  g_shader->upload_shader(And::Shader_Fragment,"../../data/fshader.fs");
  error = g_shader->get_upload_shader_error();
  if(error){ 
    printf("Error: %s\n", error);
  }

  // Una vez subimos, linkamos
  g_shader->link_shaders();


  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  // Show pc info
  g_context->create_info();

  And::Input input{*window};


  double mouseX = -1.0f, mouseY = -1.0f;
  double mouseXx = -1.0f, mouseYy = -1.0f;

  
  while (window->is_open()){
    window->update();
    g_renderer.new_frame();
    
    if (input.IsKeyDown(And::KeyCode::Space))
    {
      printf("Space down!\n");
    }

    if (input.IsKeyPressed(And::KeyCode::Space))
    {
      printf("Space pressed!\n");
    }

    if (input.IsKeyRelease(And::KeyCode::Space))
    {
      printf("Space released!\n");
    }

    g_shader->use();
    g_renderer.showDemo();
    
    //input.update_input();
    g_renderer.end_frame();
  }


  
  

  
  return 0;
}