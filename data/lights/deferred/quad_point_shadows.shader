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

layout (std140, binding = 1) uniform UniformBlockPoivvntLight{
  mat4 model;
  mat4 ProjViewLight[6];
  mat4 ProjViewCam;
  vec3 camera_position;
};

layout (std140, binding = 4) uniform UniformPoivvnt{
  PointLight point;
};

out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;
out mat4 lightSpace[6];

void main(){
  gl_Position = vec4(position, 1.0);
  uv = TexCoord;
  for(int i = 0; i < 6; i++){
    lightSpace[i] = ProjViewLight[i];// * obj_position;
  }
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texShadow[6];
uniform sampler2D Frag_Position;
uniform sampler2D Frag_Normal;
uniform sampler2D Frag_Color;
uniform sampler2D Met_Roug_Ao;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;
in mat4 lightSpace[6];


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

layout (std140, binding = 1) uniform UniformBlockPoivvntLight{
  mat4 model;
  mat4 ProjViewLight[6];
  mat4 ProjViewCam;
  vec3 camera_position;
};

layout (std140, binding = 4) uniform UniformPoivvnt{
  PointLight point;
};


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal_value, sampler2D tex){

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
  vec3 lightDir = normalize(point.position - s_fragPos);
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

  // Get textures
  //vec3 frag_color = pow(texture(Frag_Color, uv).rgb, vec3(2.2));
  vec3 frag_color = texture(Frag_Color, uv).rgb;
  vec3 frag_normal = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;

  vec3 stacked = texture(Met_Roug_Ao, uv).rgb;
  float metallic = stacked.r;
  float roughness = stacked.g;
  float ambient_oclusion = stacked.b;

  vec3 view_direction = normalize(camera_position - frag_position);
  

  //vec3 color = vec3(0.0, 0.0, 0.0);
  vec3 color = CalculePointLight(point, frag_normal, view_direction, frag_position) * frag_color;

  float shadow = 0.0;
  for(int i = 0; i < 6; i++){
    vec4 light_space_tmp = lightSpace[i] * texture(Frag_Position, uv); 
    shadow += ShadowCalculation(light_space_tmp, frag_normal, texShadow[i]);
  }
  color = (1.0 - shadow) * color;
  
  FragColor = vec4(color, 1.0);
}