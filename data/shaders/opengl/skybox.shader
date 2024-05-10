#type Vertex

#version 430 core
layout (location = 0) in vec3 position;


out vec3 TexCoords;


layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};

void main(){
  TexCoords = position;
  vec4 pos_tmp = projection * view * vec4(position, 1.0);
  gl_Position = pos_tmp.xyww;
}


#type Fragment

#version 430 core
out vec4 FragColor;

in vec3 TexCoords;

layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view;
  mat4 projection;
  vec3 camera_position;
};


uniform samplerCube skybox;

void main()
{    
  FragColor = texture(skybox, TexCoords);
}