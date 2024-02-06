#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout (std140) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
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

//uniform vec3 u_camera_position;

out vec4 FragColor;
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

uniform AmbientLight u_ambient_light;
uniform DirectionalLight u_directional_light;
uniform PointLight u_point_light;
uniform SpotLight u_spot_light;

layout (std140) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
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

  float test = u_ambient_light.enabled + u_ambient_light.direction.x + u_ambient_light.direction.y + u_ambient_light.direction.z + u_ambient_light.diffuse_color.x + u_ambient_light.diffuse_color.y + u_ambient_light.diffuse_color.z + u_ambient_light.specular_color.x + u_ambient_light.specular_color.y + u_ambient_light.specular_color.z + u_ambient_light.specular_strength + u_ambient_light.specular_shininess;
  float test_directional = u_directional_light.enabled + u_directional_light.direction.x + u_directional_light.direction.y + u_directional_light.direction.z + u_directional_light.diffuse_color.x + u_directional_light.diffuse_color.y + u_directional_light.diffuse_color.z + u_directional_light.specular_color.x + u_directional_light.specular_color.y + u_directional_light.specular_color.z + u_directional_light.specular_strength + u_directional_light.specular_shininess;
  float test_point = u_point_light.enabled + u_point_light.position.x + u_point_light.position.y + u_point_light.position.z + u_point_light.diffuse_color.x + u_point_light.diffuse_color.y + u_point_light.diffuse_color.z + u_point_light.specular_color.x + u_point_light.specular_color.y + u_point_light.specular_color.z + u_point_light.specular_strength + u_point_light.specular_shininess + u_point_light.constant_att + u_point_light.linear_att + u_point_light.quadratic_att + u_point_light.padding;
  float test_spot = u_spot_light.enabled + u_spot_light.position.x + u_spot_light.position.y + u_spot_light.position.z + u_spot_light.direction.x + u_spot_light.direction.y + u_spot_light.direction.z + u_spot_light.cutt_off + u_spot_light.outer_cut_off + u_spot_light.diffuse_color.x + u_spot_light.diffuse_color.y + u_spot_light.diffuse_color.z + u_spot_light.specular_color.x + u_spot_light.specular_color.y + u_spot_light.specular_color.z + u_spot_light.specular_strength + u_spot_light.specular_shininess + u_spot_light.constant_att + u_spot_light.linear_att + u_spot_light.quadratic_att;   

  color += CalculeDirLight(u_ambient_light, s_normal, view_direction, color_base);
  //FragColor = vec4(ambient.diffuse_color.x, ambient.diffuse_color.y, ambient.diffuse_color.z, 1.0);
  //FragColor = vec4(u_ambient_light.direction.x, u_ambient_light.direction.y, u_ambient_light.direction.z, 1.0);


  //FragColor = vec4(blend_color, 1.0);
  FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}