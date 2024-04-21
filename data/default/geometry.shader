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

out vec3 s_normal;
out vec3 s_fragPos;
out vec2 s_texCoords;

vec2 points[] = {
  -1,-1,
  -1, 1,
  1, 1,
  1, -1
}

void main()
{
  vec4 worldPos = model * vec4(position, 1.0);

  s_fragPos = worldPos.xyz;
  s_texCoords = TexCoord;

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  s_normal = normalMatrix * normals;

  gl_Position = projection * view * worldPos;
  gl_Position = vec4(points[i], 0.0, 1.0);
}

#type Fragment
#version 430 core

layout(location = 0) out vec3 Position;
layout(location = 1) out vec3 FragNormal;
layout(location = 2) out vec4 FragColor;

in vec3 s_normal;
in vec3 s_fragPos;
in vec2 s_texCoords;

uniform sampler2D texMaterial;

//uniform sampler2D colorTexture;
//uniform sampler2D specularTexture;

void main()
{
  Position = s_fragPos;
  FragNormal = normalize(s_normal);
  //FragColor.rgb = vec3(s_normal);
  FragColor.rgb = texture(texMaterial, s_texCoords).rgb;
  FragColor.a = 1.0; // specular
  //FragColor.a = texture(specularTexture, s_texCoords).r;
  
}