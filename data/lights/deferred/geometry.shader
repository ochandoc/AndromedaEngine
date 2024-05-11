#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;


layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

uniform int m_use_normal_texture;
uniform vec4 m_albedoColor;
uniform int m_use_texture;
uniform int m_use_specular_texture;
uniform int is_pbr;

//out int use_normal_texture;
//out vec4 albedoColor_color;
//out int use_texture;

out vec3 s_normal;
out vec3 s_fragPos;
out vec2 s_texCoords;
out vec3 worldPos;

void main()
{
  vec4 worldPos_tmp = model * vec4(position, 1.0);
  worldPos = vec3(model * vec4(position, 1.0));

  s_fragPos = worldPos_tmp.xyz;
  s_texCoords = TexCoord;

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  s_normal = normalMatrix * normals;

  gl_Position = projection * view * worldPos_tmp;


  //use_texture = m_use_texture;
  //albedoColor_color = m_albedoColor;
  //use_normal_texture = m_use_normal_texture;
}

#type Fragment
#version 430 core

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 FragNormal;
layout(location = 2) out vec4 FragColor;
layout(location = 3) out vec3 Met_Roug_Ao; // Texture for Metallic, roughness and ambient oclusion

in vec3 s_normal;
in vec3 s_fragPos;
in vec2 s_texCoords;
in vec3 worldPos;

uniform int m_use_normal_texture;
uniform vec4 m_albedoColor;
uniform int m_use_texture;
uniform int m_use_specular_texture;


//in int use_normal_texture;
//in vec4 albedoColor_color;
//in int use_texture;

uniform sampler2D texMaterial;
uniform sampler2D texNormal;
uniform sampler2D texSpecular;

uniform sampler2D texMetallic;
uniform sampler2D texRoughness;
uniform sampler2D texAmbientOclusion;


vec3 getNormalFromMap(){
  vec3 tangentNormal = texture(texNormal, s_texCoords).xyz * 2.0 - 1.0;

  vec3 Q1  = dFdx(worldPos);
  vec3 Q2  = dFdy(worldPos);
  vec2 st1 = dFdx(s_texCoords);
  vec2 st2 = dFdy(s_texCoords);

  vec3 N   = normalize(s_normal);
  vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
  vec3 B  = -normalize(cross(N, T));
  mat3 TBN = mat3(T, B, N);

  return normalize(TBN * tangentNormal);
}


void main()
{
  Position = s_fragPos;

  //FragNormal = use_normal_texture == 1 ? texture(texNormal, s_texCoords).rgb : normalize(s_normal);
  //FragNormal = (normalize(s_normal) * -use_normal_texture) + (texture(texNormal, s_texCoords).rgb * use_normal_texture);

  //FragNormal = vec3(m_use_normal_texture);
  if(m_use_normal_texture == 1){
    //FragNormal = texture(texNormal, s_texCoords).rgb;
    FragNormal = getNormalFromMap();
  }else{
    FragNormal = normalize(s_normal);
  }


  if(m_use_texture == 1){

    if(m_use_normal_texture == 1){
      FragColor.rgb =  pow(texture(texMaterial, s_texCoords).rgb, vec3(2.2));
    }else{
      FragColor.rgb = texture(texMaterial, s_texCoords).rgb;
    }
  }else{
    FragColor = m_albedoColor;
  }
  
  FragColor.a = 1.0; // specular
}