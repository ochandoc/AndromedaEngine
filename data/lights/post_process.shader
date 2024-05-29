#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

out vec2 uv;

void main()
{
  gl_Position = vec4(position, 1.0);
  uv = TexCoord;
}

#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D screenTex;

in vec2 uv;

void main()
{
  vec3 color = texture(screenTex, uv).rgb;
  float gray = (color.r + color.g + color.b) / 3.0;
  FragColor = vec4(gray, gray, gray, 1.0);
}

