#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

struct SpotLight{
  vec3 position;
  float padding3;
  vec3 direction;
  float padding;
  vec3 diffuse_color;
  float padding2;
  vec3 specular_color;
  float enabled;
  float cutt_off;
  float outer_cut_off;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att; // 80 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 5) uniform UniformSpot{
  SpotLight spot;
};


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

in vec3 camera_pos;
in vec2 uv;
in mat4 lightSpace;

struct Light{
  vec3 position;
  float padding3;
  vec3 direction;
  float padding;
  vec3 diffuse_color;
  float padding2;
  vec3 specular_color;
  float enabled;
  float cutt_off;
  float outer_cut_off;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att; // 80 bytes
};

struct SpotLight{
  vec3 position;
  float padding3;
  vec3 direction;
  float padding;
  vec3 diffuse_color;
  float padding2;
  vec3 specular_color;
  float enabled;
  float cutt_off;
  float outer_cut_off;
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att; // 80 bytes
};




layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 5) uniform UniformSpot{
  SpotLight spot;
};


Light CalcLight(vec3 light_direction, vec3 light_color, vec3 normals, vec3 position){
  vec3 viewDir = normalize(camera_position - position);
  Light light;

  float diff = max(dot(normals, light_direction),0.0);
  light.diffuse_color = diff * light_color;// * texture(u_texture, uv).rgb;

  vec3 reflectDir = reflect(-light_direction, normals);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

  light.specular_color = 0.5 * spec * vec3(1.0, 1.0, 1.0); // * texture(u_texture, uv).rgb;

  return light;
}

vec3 CalculeSpotLightJou(SpotLight spot, vec3 normals, vec3 position){

  vec3 lightDir  = normalize(spot.position - position);
  float cut_off = cos(spot.cutt_off * 3.1415/180);
  float outer_cut_off = cos(spot.outer_cut_off * 3.1415/180);
  Light light = CalcLight(lightDir, spot.diffuse_color, normals, position);

  float distance = length(spot.position - position);

  float k0 = spot.linear_att;
  float k1 = spot.linear_att;
  float k2 = spot.quadratic_att;

  float attenuationAmount = 1.0 / (k0 + (k1*distance) + k2* (distance*distance));
  light.diffuse_color *= attenuationAmount ;
  light.specular_color *= attenuationAmount;
  float theta = dot(lightDir,normalize(-spot.direction));//0.5

  float epsilon = (cut_off - outer_cut_off); // 0.9978 - 0.953 /
  float intensity = clamp((theta - outer_cut_off) / epsilon, 0.0, 1.0); //  0.5 - 0.953 / 0.9978 - 0.953
  light.diffuse_color  *= intensity;
  light.specular_color *= intensity;

  return light.diffuse_color + light.specular_color;
}


void main(){

  // Get textures
  //vec3 frag_color = pow(texture(Frag_Color, uv).rgb, vec3(2.2));
  vec3 frag_color = texture(Frag_Color, uv).rgb;
  vec3 frag_normal = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;

  vec3 stacked = texture(Met_Roug_Ao, uv).rgb;
  float metallic = stacked.r;
  float roughness = stacked.g;
  float ambient_oclusion = stacked.b;

  vec3 view_direction = normalize(camera_pos - frag_position);
  vec3 color = CalculeSpotLightJou(spot, frag_normal, frag_position) * frag_color;
  
  FragColor = vec4(color, 1.0);
}