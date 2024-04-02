#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
}

#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

float near = 0.1;
float far = 1000.0;

float LinearizeDepth(float depth)
{
  float z = depth * 2.0 - 1.0; // back to NDC 
  return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
  float depth = LinearizeDepth(gl_FragCoord.z) / far;
  FragColor = vec4(vec3(depth), 1.0);
}