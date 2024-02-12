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
  blend_color = vec3(camera_position.x/20.0, camera_position.y/20.0, camera_position.z/20.0);
  s_fragPos = vec3(model * vec4(position, 1.0));
  s_normal = normals;
  camera_pos = camera_position;
  uv = TexCoord;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D tex;
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


vec3 CalculeDirLight(AmbientLight light, vec3 normal, vec3 viewDir, vec3 color_base) {

  //vec3 dir = vec3(0.5, 0.5, 0.0);  

  /*---Difuse---*/
  //float diff = max(dot(normal, dir), 0.0);
  float diff = max(dot(normal, light.direction), 0.0);
  vec3 diffuse = diff * light.diffuse_color * color_base.xyz;

  /*---Specular---*/

  //vec3 reflectDir = normalize(reflect(-(dir), normalize(normal))  );
  vec3 reflectDir = normalize(reflect(-(light.direction), normalize(normal))  );
  float spec = pow(max(dot(normalize(viewDir), normalize(reflectDir)), 0.0), light.specular_shininess);
  vec3 specular = light.specular_strength * spec * light.specular_color * color_base.xyz;

  //return (diffuse + specular * light.active);
  return (diffuse * light.enabled);
}


void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  ambient_color = ambient_strength * ambient_color;
  vec3 color = ambient_color;
  vec3 color_base = vec3(0.5, 0.5, 0.5);

  
  vec4 tex_color = texture(tex, uv);
  color += CalculeDirLight(ambient_light, s_normal, view_direction, color_base);

  FragColor = vec4(ambient_light.diffuse_color.x, ambient_light.diffuse_color.y, ambient_light.diffuse_color.z, 1.0);
  //FragColor = vec4(color, 1.0) + tex_color;

  //FragColor = vec4(blend_color, 1.0);

}