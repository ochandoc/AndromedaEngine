#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

struct DirectionalLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

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
  float padding; // 64 bytes
};

struct SpotLight{
  float enabled;
  vec3 position;
  float cutt_off;
  vec3 direction;
  float outer_cut_off;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att; // 80 bytes
};


layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 1) uniform UniformLights{
  AmbientLight ambient_light;
  DirectionalLight directional;
  PointLight point;
  SpotLight spot;
};


out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0);
  blend_color = vec3(camera_position.x/20.0, camera_position.y/20.0, camera_position.z/20.0);
  s_fragPos = vec3(model * vec4(position, 1.0));
  s_normal = normals;
  camera_pos = camera_position;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;
in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;

struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

struct DirectionalLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

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
  float padding; // 64 bytes
};

struct SpotLight{
  float enabled;
  vec3 position;
  float cutt_off;
  vec3 direction;
  float outer_cut_off;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att; // 80 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 1) uniform UniformLights{
  AmbientLight ambient_light;
  DirectionalLight directional;
  PointLight point;
  SpotLight spot;
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


  difuse *= attenuation;
  specular *= attenuation;
  
  return (difuse + specular);

}

void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  ambient_color = ambient_strength * ambient_color;
  vec3 color = ambient_color;
  vec3 color_base = vec3(0.5, 0.5, 0.0);

  float test = ambient_light.enabled + ambient_light.direction.x + ambient_light.direction.y + ambient_light.direction.z + ambient_light.diffuse_color.x + ambient_light.diffuse_color.y + ambient_light.diffuse_color.z + ambient_light.specular_color.x + ambient_light.specular_color.y + ambient_light.specular_color.z + ambient_light.specular_strength + ambient_light.specular_shininess;
  float test_directional = directional.enabled + directional.direction.x + directional.direction.y + directional.direction.z + directional.diffuse_color.x + directional.diffuse_color.y + directional.diffuse_color.z + directional.specular_color.x + directional.specular_color.y + directional.specular_color.z + directional.specular_strength + directional.specular_shininess;
  float test_point = point.enabled + point.position.x + point.position.y + point.position.z + point.diffuse_color.x + point.diffuse_color.y + point.diffuse_color.z + point.specular_color.x + point.specular_color.y + point.specular_color.z + point.specular_strength + point.specular_shininess + point.constant_att + point.linear_att + point.quadratic_att + point.padding;
  float test_spot = spot.enabled + spot.position.x + spot.position.y + spot.position.z + spot.direction.x + spot.direction.y + spot.direction.z + spot.cutt_off + spot.outer_cut_off + spot.diffuse_color.x + spot.diffuse_color.y + spot.diffuse_color.z + spot.specular_color.x + spot.specular_color.y + spot.specular_color.z + spot.specular_strength + spot.specular_shininess + spot.constant_att + spot.linear_att + spot.quadratic_att;   

  color += CalculeDirLight(ambient_light, s_normal, view_direction, color_base);
  //FragColor = vec4(ambient.diffuse_color.x, ambient.diffuse_color.y, ambient.diffuse_color.z, 1.0);
  FragColor = vec4(color, 1.0);


  //FragColor = vec4(blend_color, 1.0);

}