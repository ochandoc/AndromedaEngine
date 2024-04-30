#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;


struct PointLight{
  vec3 position;
  float specular_strength;
  vec3 diffuse_color;
  float specular_shininess;
  vec3 specular_color;
  float constant_att;
  float linear_att;
  float quadratic_att;
  float enabled;
  float attenuation; // 64 bytes
};



layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 4) uniform UniformPoint{
  PointLight point;
};

uniform int m_use_normal_texture;
uniform vec4 m_albedoColor;
uniform int m_use_texture;
uniform int m_use_specular_texture;

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

uniform sampler2D texMaterial;
uniform sampler2D texNormal;
uniform sampler2D texSpecular;

uniform sampler2D texMetallic;
uniform sampler2D texRoughness;
uniform sampler2D texAmbientOclusion;

in vec2 TexCoord;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;

uniform int m_use_normal_texture;
uniform vec4 m_albedoColor;
uniform int m_use_texture;
uniform int m_use_specular_texture;


struct PointLight{
  vec3 position;
  float specular_strength;
  vec3 diffuse_color;
  float specular_shininess;
  vec3 specular_color;
  float constant_att;
  float linear_att;
  float quadratic_att;
  float enabled;
  float attenuation; // 64 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 4) uniform UniformPoint{
  PointLight point;
};

vec3 CalculePointLight(PointLight light, vec3 normalValue, vec3 view_dir, vec3 fragPos){

  vec3 lightDir = normalize(light.position - fragPos);
 
  //Diffuse
  float diff = max(dot(normalValue, lightDir), 0.0);

  //Specular
  vec3 reflectDir = reflect(-lightDir, normalValue);
  float spec = pow(max(dot(view_dir, reflectDir), 0.0), light.specular_shininess);

  float lightDistance = length(light.position- fragPos);
  float attenuation = 1.0 / (light.constant_att + light.linear_att * lightDistance + light.quadratic_att * (lightDistance * lightDistance));
  

  vec3 difuse = light.diffuse_color * diff;  
  vec3 specular = (light.specular_strength * light.specular_shininess * spec) * light.specular_color;


  difuse *= (attenuation * light.attenuation);
  specular *= attenuation;
  
  return (difuse + specular);

}

void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  ambient_color = ambient_strength * ambient_color;
  vec3 color = ambient_color;

  vec3 normal_value;
  if(m_use_normal_texture == 1){
    normal_value = texture(texNormal,uv).rgb;
  }else{
    normal_value = s_normal;
  }
  
  color += CalculePointLight(point, normal_value, view_direction, s_fragPos);

  vec4 tex_color;
  if(m_use_texture == 1){
    tex_color = texture(texMaterial, uv); 
  }else{
    tex_color = m_albedoColor;
  }
  
  FragColor = vec4(color, 1.0) * tex_color;
}