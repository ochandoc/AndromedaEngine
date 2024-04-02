#version 430 core

out vec4 FragColor;
in vec3 blend_color;

void main(){
  //FragColor = vec4(0.0, 0.6, 0.0, 1.0);
  FragColor = vec4(blend_color.x, blend_color.y, blend_color.z, 1.0);
  
}