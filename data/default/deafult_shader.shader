#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

out vec3 blend_color;

struct AmbientLight{
  float enabled;
  vec3 direction;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
};

struct DirectionalLight{
  float enabled;
  vec3 direction;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
};

struct PointLight{
  float enabled;
  vec3 position;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att;
  float padding; // 64 bytes
};

struct SpotLight{
  float enabled;
  vec3 position;
  vec3 direction;
  float cutt_off;
  float outer_cut_off;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att; // 80 bytes

};


layout (std140) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  AmbientLight ambient;
  DirectionalLight directional;
  PointLight point;
  SpotLight spot;
};


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0);
  blend_color = vec3(directional.enabled + ambient.enabled, point.enabled, spot.enabled);
}


#type Fragment
#version 430 core

out vec4 FragColor;
in vec3 blend_color;

struct AmbientLight{
  float enabled;
  vec3 direction;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
};

struct DirectionalLight{
  float enabled;
  vec3 direction;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
};

struct PointLight{
  float enabled;
  vec3 position;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att;
  float padding;
};

struct SpotLight{
  float enabled;
  vec3 position;
  vec3 direction;
  float cutt_off;
  float outer_cut_off;
  vec3 diffuse_color;
  vec3 specular_color;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att;
};

layout (std140) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  AmbientLight ambient;
  DirectionalLight directional;
  PointLight point;
  SpotLight spot;
};

void main(){

  FragColor = vec4(1.0, 0.0, 0.5, 1.0);
}