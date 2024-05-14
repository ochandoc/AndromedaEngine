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


out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;
out mat4 lightSpace;

void main(){
  gl_Position = vec4(position, 1.0);
  uv = TexCoord;
  lightSpace = ProjViewLight;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D Frag_Position;
uniform sampler2D Frag_Normal;
uniform sampler2D Frag_Color;
uniform sampler2D Met_Roug_Ao;
uniform sampler2D texShadow;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
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
  float quadratic_att; 
  // 80 bytes
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


const float PI = 3.14159265359;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal_value, sampler2D tex, vec3 frag_pos){

  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.x >= 1.0 || projCoords.x <= 0.0 ||
    projCoords.y >= 1.0 || projCoords.y <= 0.0){
    return 0.0;
  }

  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(tex, projCoords.xy).r; 
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // calculate bias (based on depth map resolution and slope)
  vec3 lightDir = normalize(spot.position - frag_pos);
  float bias = max(0.05 * (1.0 - dot(normal_value, lightDir)), 0.005);
  // check whether current frag pos is in shadow
  // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(tex, 0);
  for(int x = -1; x <= 1; ++x)
  {
      for(int y = -1; y <= 1; ++y)
      {
          float pcfDepth = texture(tex, projCoords.xy + vec2(x, y) * texelSize).r; 
          shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
      }    
  }
  shadow /= 9.0;
  
  // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if(projCoords.z > 1.0)
      shadow = 0.0;
      
  return shadow;
}

Light CalcLight(vec3 light_direction, vec3 light_color, vec3 normals, vec3 position, vec3 view_dir){
  
  Light light;

  float diff = max(dot(normals, light_direction),0.0);
  light.diffuse_color = diff * light_color;// * texture(u_texture, uv).rgb;

  vec3 reflectDir = reflect(-light_direction, normals);
  float spec = pow(max(dot(view_dir, reflectDir), 0.0), spot.specular_shininess);

  light.specular_color = spot.specular_strength * spec * spot.specular_color; // * texture(u_texture, uv).rgb;

  return light;
}

vec3 CalculeSpotLightJou(SpotLight spot, vec3 normals, vec3 view_dir, vec3 position, float attenuation){

  vec3 lightDir  = normalize(spot.position - position);
  float cut_off = cos(spot.cutt_off * 3.1415/180.0);
  float outer_cut_off = cos(spot.outer_cut_off * 3.1415/180.0);
  Light light = CalcLight(lightDir, spot.diffuse_color, normals, position, view_dir);

  float distance = length(spot.position - position);

  //float k0 = spot.constant_att;
  //float k1 = spot.linear_att;
  //float k2 = spot.quadratic_att;

  //float attenuationAmount = 1.0 / (k0 + (k1*distance) + k2* (distance*distance));
  
  light.diffuse_color *= attenuation ;
  light.specular_color *= attenuation;


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

vec3 fresnelSchlick(float cosTheta, vec3 F0){

  // Clampeo para evitar puntos negros
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main(){

  // Get textures
  //vec3 albedo = pow(texture(Frag_Color, uv).rgb, vec3(2.2));
  vec3 albedo = texture(Frag_Color, uv).rgb;
  vec3 N = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;

  vec3 stacked = texture(Met_Roug_Ao, uv).rgb;
  float metallic = stacked.r;
  float roughness = stacked.g;
  float ao = stacked.b;
  vec3 V = normalize(camera_position - frag_position);
  

  // PBR calculations
  // 0.04 porque asumimos que la mayoria de superficies dielectricas se ven visualmente correctas con ese valor
  vec3 F0 = vec3(0.04); 
  F0 = mix(F0, albedo, metallic);

  vec3 Lo = vec3(0.0);

  // calculate per-light radiance
  vec3 L = normalize((spot.position - frag_position) * 8.0);
  vec3 H = normalize(V + L);
  float distance = length(spot.position - frag_position);
  float attenuation = 1.0 / (distance * distance);
  // La atenuacion del pbr la tengo aqui


  //vec3 radiance = point.diffuse_color * attenuation;

  vec3 radiance = CalculeSpotLightJou(spot, N, V, frag_position, attenuation);

  // Cook-Torrance BRDF
  float NDF = DistributionGGX(N, H, roughness);   
  float G   = GeometrySmith(N, V, L, roughness);      
  vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
      
  vec3 numerator    = NDF * G * F; 
  float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
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
  Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
  

  //vec3 ambient = vec3(0.03) * albedo * ao;
  vec3 ambient = vec3(0.03) * albedo * ao;
  vec3 color = Lo;
  //vec3 color = Lo + (albedo * ao);



  vec4 light_space_tmp = lightSpace * texture(Frag_Position, uv); 
  float shadow = ShadowCalculation(light_space_tmp, N, texShadow, frag_position);
  color = (1.0 - shadow) * color;
  //color.rgb += albedo * 0.1;
  
  // HDR tonemapping
  color = color / (color + vec3(1.0));
  // gamma correct
  color = pow(color, vec3(1.0/2.2)); 

  FragColor = vec4(color, 1.0);
  //FragColor = vec4(color.r + metallic, color.g + roughness, color.b + ambient_oclusion, 1.0);
}