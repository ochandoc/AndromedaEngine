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

const float PI = 3.14159265359;


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
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), spot.specular_shininess);

  light.specular_color = spot.specular_strength * spec * spot.specular_color; // * texture(u_texture, uv).rgb;

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

float DistributionGGX(vec3 N, vec3 H, float roughness){
  float a = roughness*roughness;
  float a2 = a*a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH*NdotH;

  float nom   = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness){
  float r = (roughness + 1.0);
  float k = (r*r) / 8.0;

  float nom   = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0){
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CalCuleReflection(vec3 worldPos, vec3 view_dir, vec3 N, float roughness, float metallic, vec3 F0, vec3 albedo){
  // calculate per-light radiance
  vec3 L = normalize(spot.position - worldPos);
  vec3 H = normalize(view_dir + L);
  float distance = length(spot.position - worldPos);
  float attenuation = 1.0 / (distance * distance);
  vec3 radiance = (spot.diffuse_color * 300.0) * attenuation;

  // Cook-Torrance BRDF
  float NDF = DistributionGGX(N, H, roughness);   
  float G   = GeometrySmith(N, view_dir, L, roughness);      
  vec3 F    = fresnelSchlick(max(dot(H, view_dir), 0.0), F0);
      
  vec3 numerator    = NDF * G * F; 
  float denominator = 4.0 * max(dot(N, view_dir), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
  vec3 specular = numerator / denominator;
  
  // kS is equal to Fresnel
  vec3 kS = F;
  // for energy conservation, the diffuse and specular light can't
  // be above 1.0 (unless the surface emits light); to preserve this
  // relationship the diffuse component (kD) should equal 1.0 - kS.
  vec3 kD = vec3(1.0) - kS;
  // multiply kD by the inverse metalness such that only non-metals 
  // have diffuse lighting, or a linear blend if partly metal (pure metals
  // have no diffuse light).
  kD *= 1.0 - metallic;	  

  // scale light by NdotL
  float NdotL = max(dot(N, L), 0.0);        

  // add to outgoing radiance Lo
  return (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

void main(){

  // Get textures
  vec3 frag_color = texture(Frag_Color, uv).rgb;

  //vec3 frag_color = pow(texture(Frag_Color, uv).rgb, vec3(2.2));
  vec3 frag_normal = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;

  vec3 stacked = texture(Met_Roug_Ao, uv).rgb;
  float metallic = stacked.r;
  float roughness = stacked.g;
  float ambient_oclusion = stacked.b;
  vec3 view_direction = normalize(camera_pos - frag_position);
  vec3 direct_color = CalculeSpotLightJou(spot, frag_normal, frag_position) * frag_color;

  //vec3 F0 = vec3(0.04); 
  //F0 = mix(F0, frag_color, metallic);


  //vec3 indirect_color = CalCuleReflection(frag_position, view_direction, frag_normal, roughness, metallic, F0, frag_color);
  //vec3 ambient_color = vec3(0.03) * frag_color * ambient_oclusion;
  
  //vec3 color =  direct_color * frag_color * indirect_color;
  vec3 color = direct_color;
  //color += ambient_color;
  
  // HDR tonemapping
  color = color / (color + vec3(1.0));
  // gamma correct
  //color = pow(color, vec3(1.0/2.2)); 


  FragColor = vec4(color, 1.0);
}