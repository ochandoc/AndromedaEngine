#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

out vec3 blend_color;

layout (std140) uniform Matrix{
  mat4 model;
  mat4 view;
  mat4 projection;
};


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0f);
  //gl_Position = vec4(position.x, position.y, position.z, 1.0);
  blend_color = normals;
}

#type Fragment
#version 430 core

out vec4 FragColor;
in vec3 blend_color;

void main(){
  //FragColor = vec4(0.0, 0.6, 0.0, 1.0);
  FragColor = vec4(blend_color, 1.0);
  
}