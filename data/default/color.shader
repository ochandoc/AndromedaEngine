#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

layout(std140, binding = 0) uniform UniformBlock
{
  mat4 PVM;
  sampler2D color;
};

out vec2 s_uv;

void main()
{
  gl_position = PVM * vec4(position, 1.0);
  s_uv = uv;
}

#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

layout(std140, binding = 0) uniform UniformBlock
{
  mat4 PVM;
  sampler2D color;
};

in vec2 s_uv;

void main()
{
  FragColor = texture(color, s_uv);
}

