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



layout (std140, binding = 1) uniform UniformBlockPointLight{
  mat4 model;
  mat4 ProjViewLight[6];
  mat4 ProjViewCam;
  vec3 camera_position;
};

layout (std140, binding = 4) uniform UniformPoint{
  PointLight point;
};


out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;
out vec4 lightSpace[6];

void main(){
  vec4 obj_position = model * vec4(position, 1.0);
  gl_Position = ProjViewCam * model * vec4(position, 1.0);
  blend_color = vec3(camera_position.x/20.0, camera_position.y/20.0, camera_position.z/20.0);
  s_fragPos = vec3(model * vec4(position, 1.0));

  s_normal = normals;
  camera_pos = camera_position;
  uv = TexCoord;
  for(int i = 0; i < 6; i++){
    lightSpace[i] = ProjViewLight[i] * obj_position;
  }
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texShadow[6];
in vec2 TexCoord;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;
in vec4 lightSpace[6];


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

layout (std140, binding = 1) uniform UniformBlockPointLight{
  mat4 model;
  mat4 ProjViewLight[6];
  mat4 ProjViewCam;
  vec3 camera_position;
};

layout (std140, binding = 4) uniform UniformPoint{
  PointLight point;
};

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

float ShadowCalculation(vec4 fragPosLightSpace, sampler2D tex){

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
  vec3 normal = normalize(s_normal);
  vec3 lightDir = normalize(point.position - s_fragPos);
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
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

void main(){
  vec3 view_direction = normalize(camera_pos - s_fragPos);
  float ambient_strength = 0.01;
  vec3 ambient_color = vec3(1.0);
  ambient_color = ambient_strength * ambient_color;
  vec3 color = ambient_color;
  vec3 color_base = vec3(0.5, 0.5, 0.5);
  
  color += CalculePointLight(point, s_normal, view_direction, s_fragPos);
  float shadow = 0.0;
  for(int i = 0; i < 6; i++){
    shadow += ShadowCalculation(lightSpace[i], texShadow[i]);
  }

  color = (1.0 - shadow) * color;


  FragColor = vec4(color, 1.0);


}