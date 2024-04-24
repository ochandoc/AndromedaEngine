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

//out int use_normal_texture;
//out vec4 albedoColor_color;
//out int use_texture;

out vec3 s_normal;
out vec3 s_fragPos;
out vec2 s_texCoords;


void main()
{
  vec4 worldPos = model * vec4(position, 1.0);

  s_fragPos = worldPos.xyz;
  s_texCoords = TexCoord;

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  s_normal = normalMatrix * normals;

  gl_Position = projection * view * worldPos;

  //use_texture = m_use_texture;
  //albedoColor_color = m_albedoColor;
  //use_normal_texture = m_use_normal_texture;
}

#type Fragment
#version 430 core

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 FragNormal;
layout(location = 2) out vec4 FragColor;

in vec3 s_normal;
in vec3 s_fragPos;
in vec2 s_texCoords;

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
//uniform sampler2D colorTexture;
//uniform sampler2D specularTexture;

void main()
{
  Position = s_fragPos;

  //FragNormal = use_normal_texture == 1 ? texture(texNormal, s_texCoords).rgb : normalize(s_normal);
  //FragNormal = (normalize(s_normal) * -use_normal_texture) + (texture(texNormal, s_texCoords).rgb * use_normal_texture);

  //FragNormal = vec3(m_use_normal_texture);
  if(m_use_normal_texture == 1){
    FragNormal = texture(texNormal, s_texCoords).rgb;
  }else{
    FragNormal = normalize(s_normal);
  }



  //FragColor.rgb = vec3(s_normal);
  //FragColor.rgb = (texture(texMaterial, s_texCoords).rgb * -use_color) + (albedoColor_color * use_color);





  if(m_use_texture == 1){
    FragColor.rgb = texture(texMaterial, s_texCoords).rgb;
    FragColor.a = 1.0; // specular
  }else{
    FragColor = m_albedoColor;
  }

  if(m_use_specular_texture == 1){
    FragColor.a = texture(texSpecular, s_texCoords).r; // specular
  }
  
  
  
  
  
  
  //FragColor.a = texture(specularTexture, s_texCoords).r;
  
}