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
  //mat4 view;
  mat4 ProjViewLight;
  //mat4 projection;
  mat4 ProjViewCam;
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
out vec4 lightSpace;


void main(){
  vec4 obj_position = model * vec4(position, 1.0);
  gl_Position = ProjViewCam * obj_position;
  blend_color = vec3(camera_position.x/20.0, camera_position.y/20.0, camera_position.z/20.0);
  s_fragPos = vec3(model * vec4(position, 1.0));
  s_normal = normals;
  camera_pos = camera_position;
  uv = TexCoord;
  lightSpace = ProjViewLight * obj_position;
  //lightSpace = vec4(1.0);
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texShadow;
in vec2 TexCoord;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;
in vec4 lightSpace;


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
  //mat4 view;
  mat4 ProjViewLight;
  //mat4 projection;
  mat4 ProjViewCam;
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

float ShadowCalculation(vec4 fragPosLightSpace){

  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;
  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(texShadow, projCoords.xy).r; 
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // calculate bias (based on depth map resolution and slope)
  vec3 normal = normalize(s_normal);
  vec3 lightDir = normalize(spot.position - s_fragPos);
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  // check whether current frag pos is in shadow
  // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(texShadow, 0);
  for(int x = -1; x <= 1; ++x)
  {
      for(int y = -1; y <= 1; ++y)
      {
          float pcfDepth = texture(texShadow, projCoords.xy + vec2(x, y) * texelSize).r; 
          shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
      }    
  }
  shadow /= 9.0;
  
  // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if(projCoords.z > 1.0)
      shadow = 0.0;
      
  return shadow;
}


void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  //ambient_color = ambient_strength * ambient_color;
  //vec3 color = ambient_color;
  vec3 color_base = vec3(0.5, 0.5, 0.5);
  
  vec3 color = CalculeSpotLightJou(spot);
  float shadow = ShadowCalculation(lightSpace);
  //color = (1.0 - shadow) * color;


  FragColor = vec4(color,1.0);


}