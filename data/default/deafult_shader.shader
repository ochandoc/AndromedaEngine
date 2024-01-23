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
  gl_Position = projection * view * model * vec4(position, 1.0);
}


#type Fragment
#version 430 core

out vec4 FragColor;
in vec3 blend_color;


void main(){

  FragColor = vec4(1.0, 0.0, 0.5, 1.0);
}