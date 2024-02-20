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
  vec3 camera_position;
};


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0);
  blend_color = normals;
  vec3 test = vec3(ambient.enabled + directional.enabled, point.enabled, spot.enabled);
}

#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;
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
  vec3 camera_position;
};


void main(){
  //FragColor = vec4(0.0, 0.6, 0.0, 1.0);
  FragColor = vec4(blend_color, 1.0);
}