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
  mat4 view;
  mat4 projection;
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
  gl_Position = projection * view * model * vec4(position, 1.0);
  //blend_color = vec3(camera_position.x/20.0, camera_position.y/20.0, camera_position.z/20.0);
  s_fragPos = vec3(model * vec4(position, 1.0));
  s_normal = normals;
  camera_pos = camera_position;
  uv = TexCoord;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texMaterial;
in vec2 TexCoord;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;

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
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 2) uniform UniformAmbient{
  AmbientLight ambient_light;
};



void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 1.0;
  vec3 color = ambient_strength * ambient_light.diffuse_color;
  
  vec4 tex_color = texture(texMaterial, uv); 
  FragColor = vec4(color, 1.0) * tex_color;
  //FragColor = tex_color;
}