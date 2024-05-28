#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

struct DirectionalLight{
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

layout (std140, binding = 3) uniform UniformDirectional{
  DirectionalLight directional_light;
};

uniform int m_use_normal_texture;
uniform vec4 m_albedoColor;
uniform int m_use_texture;
uniform int m_use_specular_texture;

out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0);

  s_fragPos = vec3(model * vec4(position, 1.0));

  s_normal = vec3(transpose(inverse(model))  * vec4(normals, 0.0));
  //s_normal = normals;
  
  camera_pos = camera_position;

  uv = TexCoord;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texMaterial;
uniform sampler2D texNormal;
uniform sampler2D texSpecular;

uniform sampler2D texMetallic;
uniform sampler2D texRoughness;
uniform sampler2D texAmbientOclusion;

in vec2 TexCoord;

uniform int m_use_normal_texture;
uniform vec4 m_albedoColor;
uniform int m_use_texture;
uniform int m_use_specular_texture;

in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;

struct DirectionalLight{
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

layout (std140, binding = 3) uniform UniformDirectional{
  DirectionalLight directional_light;
};


vec3 CalculeDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 color_base) {

  //vec3 dir = vec3(0.5, 0.5, 0.0);  

  /*---Difuse---*/
  //float diff = max(dot(normal, dir), 0.0);
  float diff = max(dot(normal, -light.direction), 0.0); // este -1 es un poco raro
  vec3 diffuse = diff * light.diffuse_color;// * color_base.xyz;

  /*---Specular---*/

  //vec3 reflectDir = normalize(reflect(-(dir), normalize(normal))  );
  vec3 reflectDir = normalize(reflect(-(light.direction), normalize(normal))  );
  float spec = pow(max(dot(normalize(viewDir), normalize(reflectDir)), 0.0), light.specular_shininess);
  vec3 specular = light.specular_strength * spec * light.specular_color;// * color_base.xyz;

  //return (diffuse * light.enabled);
  return (diffuse + specular * light.enabled);
}


void main(){
 vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  ambient_color = ambient_strength * ambient_color;
  vec3 color = ambient_color;
  vec3 color_base = vec3(0.5, 0.5, 0.5);

  vec3 normal_value;
  if(m_use_normal_texture == 1){
    normal_value = texture(texNormal,uv).rgb;
  }else{
    normal_value = s_normal;
  }

  color = CalculeDirLight(directional_light, normal_value, view_direction, color_base);

  vec4 tex_color;
  if(m_use_texture == 1){
    tex_color = texture(texMaterial, uv); 
  }else{
    tex_color = m_albedoColor;
  }

  FragColor = vec4(color, 1.0) * tex_color;


}