#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

struct PointLight {
  vec3 position;
  float specular_strength;
  vec3 diffuse_color;
  float specular_shininess;
  vec3 specular_color;
  float constant_att;
  float linear_att;
  float quadratic_att;
  float enabled;
  float padding; // 64 bytes
};

layout(std140, binding = 0) uniform UniformBlock
{
  mat4 PVM;
  mat4 model;
  vec3 cameraPos;
  PointLight light;
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

struct PointLight {
  vec3 position;
  float specular_strength;
  vec3 diffuse_color;
  float specular_shininess;
  vec3 specular_color;
  float constant_att;
  float linear_att;
  float quadratic_att;
  float enabled;
  float padding; // 64 bytes
};

layout(std140, binding = 0) uniform UniformBlock
{
  mat4 PVM;
  mat4 model;
  vec3 cameraPos;
  PointLight light;
};

in vec2 s_uv;
in vec3 s_normal;
in vec3 s_fragPos;

vec3 CalculePointLight(PointLight l, vec3 normalValue, vec3 view_dir, vec3 fragPos) {

  vec3 lightDir = normalize(l.position - fragPos);

  //Diffuse
  float diff = max(dot(normalValue, lightDir), 0.0);

  //Specular
  vec3 reflectDir = reflect(-lightDir, normalValue);
  float spec = pow(max(dot(view_dir, reflectDir), 0.0), l.specular_shininess);

  float lightDistance = length(l.position - fragPos);
  float attenuation = 1.0 / (l.constant_att + l.linear_att * lightDistance + l.quadratic_att * (lightDistance * lightDistance));


  vec3 difuse = l.diffuse_color * diff;
  vec3 specular = (l.specular_strength * l.specular_shininess * spec) * l.specular_color;


  difuse *= attenuation;
  specular *= attenuation;

  return (difuse + specular);


void main()
{
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  vec3 light_color = CalculePointLight(light, s_normal, view_direction, s_fragPos);
  FragColor = vec4(light_color, 1.0f);
}

