#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
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
out mat4 lightSpace;

void main(){
  gl_Position = vec4(position, 1.0);
  uv = TexCoord;
  vec4 obj_position = model * vec4(position, 1.0);
  lightSpace = ProjViewLight;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texShadow;
uniform sampler2D Frag_Position;
uniform sampler2D Frag_Normal;
uniform sampler2D Frag_Color;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;
in mat4 lightSpace;


struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
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


void main(){

  // Get textures
  vec3 frag_color = texture(Frag_Color, uv).rgb;
  vec3 frag_normal = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;

  float ambient_strength = 1.0;
  vec3 color = ambient_strength * ambient_light.diffuse_color;
  color *= frag_color; 

  FragColor = vec4(color, 1.0);
}