# AndromedaEngine

Graphics library made in C++ with OpenGL

__Authors__
- Jose Maria Maestre
- Hector Ochando 

## Dependences
- Python 3.11
- Conan < 2 
- Cmake

## Set up instructions

``` 
cd tools
./Setup.bat
```
Then follow the script instructions

Open Visual Studio solution


## Hello World tutorial

In our main.cpp 

```
#include "Andromeda.h"
```

Basic engine creation
```
And::Engine engine;
```

Creating a Window
```
// Window
std::shared_ptr<And::Window> window = And::Window::make(engine, 1920, 1080, "Andromeda Engine");

// Create Renderer and Graphics Context
std::shared_ptr<And::GraphicsContext> g_context = window->get_context();
And::Renderer g_renderer(*window); 
```

In our principal loop we have to do 2 principal calls at begining and 2 at the end
```
while (window->is_open()){
    window->update();
    g_renderer.new_frame();
    

    g_renderer.end_frame();
    window->swap_buffers();
}
```

And we should see something like this

![window](./docs/main.png)

## How to draw a OBJ

First, we have to create the __Entity Component System__ to manage all scene entities and create or modify

```
// Create
And::EntityComponentSystem entity_comp;

// Add basic components
And::AddBasicComponents(entity_comp);
```

- Create Mesh Component
```
And::MeshComponent MC;
MC.MeshOBJ = And::Geometry::load("teapot.obj");
```

- Create __Transform__ component to manage *position*, *scale* and *rotation* of the entity
```
And::TransformComponent tran;
tran.position[0] = 0.0f;
tran.position[1] = -4.0f;
tran.position[2] = -10.0f;
tran.rotation[0] = 1.0f;
tran.rotation[1] = 1.0f;
tran.rotation[2] = 1.0f;
tran.scale[0] = 1.0f;
tran.scale[1] = 1.0f;
tran.scale[2] = 1.0f;
```
- Add obj to the entity list
```
And::Entity* obj_id = entity_comp.new_entity(MC, tran);
```

__Note:__ You should save the entity returned by the entity component system to modify his values in the future

Now we can render the object in the scene but we have to add a light before in order to being able to see that object

- Add __Directional light__

```
And::DirectionalLight directional{};
directional.SetDirection(1.0f, 0.0f, 0.0f);
directional.SetDiffuseColor(1.0f, 1.0f, 1.0f);
directional.SetSpecularColor(1.0f, 1.0f, 1.0f);
directional.SetEnabled(true);
```

- Add directional light to entity list
```
And::Entity* light_directional_entity = entity_comp.new_entity(directional);
```

- __Render__ the scene

```
// Between new_frame() and end_frame()
And::DrawForward(entity_comp, g_renderer);
```

And now we should see something like this
![window](./docs/teapot.png)



