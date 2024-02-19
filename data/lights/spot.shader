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
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 5) uniform UniformSpot{
  SpotLight spot;
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

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

layout (std140, binding = 5) uniform UniformSpot{
  SpotLight spot;
};

Light CalcLight(vec3 light_direction, vec3 light_color){
  vec3 viewDir = normalize(camera_position - s_fragPos);
  Light light;

  float diff = max(dot(s_normal, light_direction),0.0);
  light.diffuse_color = diff * light_color;// * texture(u_texture, uv).rgb;

  vec3 reflectDir = reflect(-light_direction, s_normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

  light.specular_color = 0.5 * spec * vec3(1.0, 1.0, 1.0); // * texture(u_texture, uv).rgb;

  return light;
}

vec3 CalculeSpotLightJou(SpotLight spot){

  vec3 lightDir  = normalize(spot.position - s_fragPos);
  float cut_off = cos(spot.cutt_off * 3.1415/180);
  float outer_cut_off = cos(spot.outer_cut_off * 3.1415/180);
  Light light = CalcLight(lightDir, spot.diffuse_color);

  float distance = length(spot.position - s_fragPos);

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

vec3 CalculeSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));

  vec3 diffuse = vec3(0.0);
  vec3 specular = vec3(0.0);

  if (theta > light.outer_cut_off)
  {
    /*---Diffuse---*/
    float diff = max(dot(normal, lightDir), 0.0);
    diffuse = light.diffuse_color * diff; //* u_color.xyz;
    /*---Specular---*/
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.specular_shininess);
    specular = (light.specular_strength * spec * light.specular_color);// * u_color.xyz);
    
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant_att + light.linear_att * distance + 
                              light.quadratic_att * (distance * distance));

    float epsilon = light.cutt_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    diffuse *= attenuation;
    specular *= intensity;

    diffuse *= intensity;
    specular *= intensity;

  }
  return (diffuse + specular);
}

void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  //ambient_color = ambient_strength * ambient_color;
  //vec3 color = ambient_color;
  vec3 color_base = vec3(0.5, 0.5, 0.5);
  
  vec3 color = CalculeSpotLightJou(spot);


  FragColor = vec4(color, 1.0);


}