#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;


void main(){
  gl_Position = projection * view * model * vec4(position, 1.0f);
}


#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;
in vec3 blend_color;

void main(){
  FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}