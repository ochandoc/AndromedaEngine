#version 430 core

out vec4 FragColor;
in vec2 blend_color;

void main(){
  //FragColor = vec4(0.0, 1.0, 0.0, 1.0);
  FragColor = vec4(blend_color.xy, 0.0, 1.0);
  
}