#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

layout (std140) uniform Matrix
{
  mat4 model;
  mat4 view;
  mat4 projection;
};

layout (std140) uniform Ambient_light
{
  float a;
  vec3 direction;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
};

out vec3 blend_color;

void main(){
  gl_Position = projection * view * model * vec4(position, 1.0f);
  blend_color = vec3(model[0][0], model[0][1], model[0][2]);
}


#type Fragment
#version 430 core

out vec4 FragColor;
in vec3 blend_color;

layout (std140) uniform Ambient_light
{
  float a;
  vec3 direction;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
};


void main(){

  FragColor = vec4(1.0, 0.0, 0.5, 1.0);
  FragColor = vec4(blend_color, 1.0f);
}