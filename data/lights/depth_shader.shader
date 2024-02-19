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


// Este vertex recibe la posicion de la luz
void main(){
  gl_Position = projection * view * model * vec4(position, 1.0);
}


#type Fragment
#version 430 core

void main(){

}