#version 430 core

layout(location = 0) in vec3 position;

out vec3 blend_color;
uniform mat4 view;
uniform mat4 projection;


void main(){
  gl_Position = projection * view * vec4(position, 1.0f);
  //gl_Position = vec4(position.x, position.y, position.z, 1.0);
  blend_color = position;
}