#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

struct DirectionalLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 3) uniform UniformDirectional{
  DirectionalLight directional_light;
};

out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;
out mat4 lightSpace;

//out vec3 obj_position;

void main(){
  gl_Position = vec4(position, 1.0);
  uv = TexCoord;
  //obj_position = model * vec4(position, 1.0);
  lightSpace = ProjViewLight;
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texShadow;
uniform sampler2D Frag_Position;
uniform sampler2D Frag_Normal;
uniform sampler2D Frag_Color;
uniform sampler2D Met_Roug_Ao;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;
in mat4 lightSpace;

//in vec3 obj_position;

struct DirectionalLight{
  vec3 direction;
  float enabled;
  vec3 diffuse_color;
  float specular_strength;
  vec3 specular_color;
  float specular_shininess; // 48 bytes
};



layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 ProjViewCam;
  mat4 ProjViewLight;
  vec3 camera_position;
};

layout (std140, binding = 3) uniform UniformDirectional{
  DirectionalLight directional_light;
};


float ShadowCalculation(vec4 fragPosLightSpace, vec3 normals){
 
  // perform perspective divide
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  // transform to [0,1] range
  projCoords = projCoords * 0.5 + 0.5;

  if (projCoords.x >= 1.0 || projCoords.x <= 0.0 ||
    projCoords.y >= 1.0 || projCoords.y <= 0.0){
    return 0.0;
  }

  // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
  float closestDepth = texture(texShadow, projCoords.xy).r; 
  // get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;
  // calculate bias (based on depth map resolution and slope)
  vec3 normal = normalize(normals);

  float x = camera_position.x + ( (-1.0 * directional_light.direction.x) * 50.0);
  float z = camera_position.z + ( (-1.0 * directional_light.direction.z) * 50.0);
  vec3 light_pos = vec3(x, camera_position.y, z);

  vec3 lightDir = normalize(light_pos - s_fragPos);
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  // check whether current frag pos is in shadow
  // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(texShadow, 0);
  for(int x = -1; x <= 1; ++x){
    for(int y = -1; y <= 1; ++y){
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


vec3 CalculeDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  
  //vec3 dir = vec3(0.5, 0.5, 0.0);  

  /*---Difuse---*/
  //float diff = max(dot(normal, dir), 0.0);
  float diff = max(dot(normal, -light.direction), 0.0); // este -1 es un poco raro
  vec3 diffuse = diff * light.diffuse_color;// * color_base.xyz;

  /*---Specular---*/

  //vec3 reflectDir = normalize(reflect(-(dir), normalize(normal))  );
  vec3 reflectDir = normalize(reflect(-(light.direction), normalize(normal))  );
  float spec = pow(max(dot(normalize(viewDir), normalize(reflectDir)), 0.0), light.specular_shininess);
  vec3 specular = light.specular_strength * spec * light.specular_color;// * color_base.xyz;

  //return (diffuse * light.enabled);
  return (diffuse + specular * light.enabled);
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

  vec4 light_space_tmp = lightSpace * texture(Frag_Position, uv); 

  //vec3 N = getNormalFromMap(frag_normal,frag_position);
  //vec3 N = frag_normal;
  vec3 view_direction = normalize(camera_pos - frag_position);



  //vec3 F0 = vec3(0.04); 
  //F0 = mix(F0, frag_color, metallic);

  //vec3 color = vec3(frag_color * ambient_oclusion);
  
  // HDR tonemapping
  //color = color / (color + vec3(1.0));
  // gamma correct
  //color = pow(color, vec3(1.0/2.2)); 

  vec3 color = CalculeDirLight(directional_light, frag_normal, view_direction) * frag_color;
  float shadow = ShadowCalculation(light_space_tmp, frag_normal);
  color = (1.0 - shadow) * color;

  
  FragColor = vec4(color, 1.0);
}