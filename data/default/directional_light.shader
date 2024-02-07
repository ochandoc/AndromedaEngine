#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

struct DirectionalLight {
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout(std140, binding = 0) uniform UniformBlock
{
  mat4 PVM;
  mat4 model;
  vec3 cameraPos;
  DirectionalLight light;
};

out vec2 s_uv;
out vec3 s_normal;
out vec3 s_fragPos;

void main()
{
  gl_position = PVM * vec4(position, 1.0);
  s_uv = uv;
  s_norlam = normal;
  s_fragPos = vec3(model * vec4(position, 1.0));
}

#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

struct DirectionalLight {
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout(std140, binding = 0) uniform UniformBlock
{
  mat4 PVM;
  mat4 model;
  vec3 cameraPos;
  DirectionalLight light;
};

in vec2 s_uv;
in vec3 s_normal;
in vec3 s_fragPos;

vec3 CalculeDirLight(DirectionalLight l, vec3 normal, vec3 viewDir, vec3 color_base) {

  /*---Difuse---*/
  float diff = max(dot(normal, l.direction), 0.0);
  vec3 diffuse = diff * l.diffuse_color * color_base.xyz;

  /*---Specular---*/
  vec3 reflectDir = normalize(reflect(-(l.direction), normalize(normal)));
  float spec = pow(max(dot(normalize(viewDir), normalize(reflectDir)), 0.0), l.specular_shininess);
  vec3 specular = l.specular_strength * spec * l.specular_color * color_base.xyz;

  return (diffuse * l.enabled);
}

void main()
{
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  vec3 light_color = CalculeDirLight(light, s_normal, view_direction, vec3(0.0, 0.0, 0.0));
  FragColor = vec4(light_color, 1.0f);
}

