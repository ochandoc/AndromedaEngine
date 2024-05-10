#type Vertex

#version 430 core
layout (location = 0) in vec3 position;


out vec3 TexCoords;


layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

void main()
{

    TexCoords = position;
    gl_Position = (projection * view * vec4(position, 1.0)).xyww;
}


#type Fragment

#version 430 core
out vec4 FragColor;

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}