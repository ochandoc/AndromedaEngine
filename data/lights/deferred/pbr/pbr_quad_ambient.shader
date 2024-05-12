#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float ambient_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
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


in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;



struct AmbientLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float ambient_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 2) uniform UniformAmbient{
  AmbientLight ambient_light;
};

const float PI = 3.14159265359;



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
  //vec3 F0 = vec3(0.04); 
  //F0 = mix(F0, albedo, metallic);

  //vec3 Lo = vec3(0.0);

  // calculate per-light radiance
  //vec3 L = normalize(point.position - frag_position);
  //vec3 H = normalize(V + L);
  //float distance = length(point.position - frag_position);
  //float attenuation = 1.0 / (distance * distance);
  //vec3 radiance = point.diffuse_color * attenuation;

  // Cook-Torrance BRDF
  //float NDF = DistributionGGX(N, H, roughness);   
  //float G   = GeometrySmith(N, V, L, roughness);      
  //vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
      
  //vec3 numerator    = NDF * G * F; 
  //float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
  //vec3 specular = numerator / denominator;
  
  // kS is equal to Fresnel
  //vec3 kS = F;
  // for energy conservation, the diffuse and specular light can't
  // be above 1.0 (unless the surface emits light); to preserve this
  // relationship the diffuse component (kD) should equal 1.0 - kS.
  //vec3 kD = vec3(1.0) - kS;
  // multiply kD by the inverse metalness such that only non-metals 
  // have diffuse lighting, or a linear blend if partly metal (pure metals
  // have no diffuse light).
  //kD *= 1.0 - metallic;	  

  // scale light by NdotL
  //float NdotL = max(dot(N, L), 0.0);        

  // add to outgoing radiance Lo
  //Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
  
  vec3 color = (ambient_light.ambient_strength * ambient_light.diffuse_color) * albedo * ao;

  //vec3 ambient = vec3(0.03) * albedo * ao;
  //vec3 ambient = vec3(0.03) * albedo * ao;
  //vec3 color = Lo + ambient;
  //vec3 color = Lo + (albedo * ao);

  
  // HDR tonemapping
  color = color / (color + vec3(1.0));
  // gamma correct
  color = pow(color, vec3(1.0/2.2)); 

  FragColor = vec4(color, 1.0);
  //FragColor = vec4(color.r + metallic, color.g + roughness, color.b + ambient_oclusion, 1.0);
}