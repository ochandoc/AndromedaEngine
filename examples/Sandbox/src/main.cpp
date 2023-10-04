#include <assert.h>

#include <functional>
#include <algorithm>
#include <utility>

#include <optional>
#include <memory>

#include <string>
#include <vector>

#include "Common/Window.h"
#include "Common/GraphicsContext.h"
#include "Common/Renderer.h"
#include "Common/Shader.h"

#include "Common/KeyboardInput.h"

#include <windows.h>


// que te parece si hacemos que la ventana tenga una instancia del input, es muy poarecido a lo que esta diciendo el
// hacemos lo del input de mientras o que
// espera porqie si nos ponemos focus no atendemos y esta diciendo cosas importantes


int main(int argc, char** argv){
  And::WindowCreationInfo WindowInfo;
  WindowInfo.width = 1024;
  WindowInfo.height = 720;
  WindowInfo.title = "Andromeda Engine";


  std::shared_ptr<And::Window> window(new And::Window(WindowInfo));
  std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
  And::Renderer& g_renderer = window->create_renderer();

  // Create basic shader
  And::ShaderInfo vs_info = { And::Shader_Vertex, "../../data/vshader.vs" };
  And::ShaderInfo fs_info = { And::Shader_Fragment, "../../data/fshader.fs" };
  std::vector<And::ShaderInfo> shaders_vec;
  shaders_vec.push_back(vs_info);
  shaders_vec.push_back(fs_info);

  std::shared_ptr<And::Shader> g_shader = g_renderer.createShader(shaders_vec);

  float clear_color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  g_renderer.set_clear_color(clear_color);

  // Show pc info
  g_context->create_info();

  And::Input input{*window};


  double mouseX = -1.0f, mouseY = -1.0f;
  double mouseXx = -1.0f, mouseYy = -1.0f;

  input.init_input();

  
  while (window->is_open()){
    g_renderer.new_frame();
    
    // Testing inputs
    if(input.IsMouseButtonPressed(And::Key::MouseLeft)){
      printf("Left click");
    }
    
    if(input.IsMouseButtonPressed(And::Key::MouseRight)){
      printf("Right click");
    }

    input.GetMousePosition(&mouseX, &mouseY);
    mouseXx = input.GetMouseX();
    mouseYy = input.GetMouseY();
    //printf("X[%f] Y[%f] x[%f] y[%f]\n", mouseX, mouseY, mouseXx, mouseYy);


     // Bucle principal
    MSG msg;
    if(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if(input.IsKeyDown(And::Key::KeyCode::Space)){
      printf("Space down!\n");
    }
    
    if(input.IsKeyPressed(And::Key::KeyCode::Space)){
      printf("Space pressed!\n");
    }
    if(input.IsKeyRelease(And::Key::KeyCode::Space)){
      printf("Space release!\n");
    }

    
    g_shader->use();
    g_renderer.showDemo();
    
    //input.update_input();
    g_renderer.end_frame();
  }


  
  

  
  return 0;
}