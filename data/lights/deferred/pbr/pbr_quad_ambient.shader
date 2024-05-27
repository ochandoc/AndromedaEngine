#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float ambient_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 2) uniform UniformAmbient{
  AmbientLight ambient_light;
};

out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;

void main(){
  gl_Position = vec4(position, 1.0);
  uv = TexCoord;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D Frag_Position;
uniform sampler2D Frag_Normal;
uniform sampler2D Frag_Color;
uniform sampler2D Met_Roug_Ao;


in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;



struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float ambient_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 2) uniform UniformAmbient{
  AmbientLight ambient_light;
};

const float PI = 3.14159265359;



void main(){

  // Get textures
  vec3 albedo = texture(Frag_Color, uv).rgb;
  vec3 N = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;

  vec3 stacked = texture(Met_Roug_Ao, uv).rgb;
  float metallic = stacked.r;
  float roughness = stacked.g;
  float ao = stacked.b;
  vec3 V = normalize(camera_position - frag_position);
  
  vec3 color = (ambient_light.ambient_strength * ambient_light.diffuse_color) * albedo * ao;

  // HDR tonemapping
  color = color / (color + vec3(1.0));
  // gamma correct
  color = pow(color, vec3(1.0/2.2)); 

  FragColor = vec4(color, 1.0);
}