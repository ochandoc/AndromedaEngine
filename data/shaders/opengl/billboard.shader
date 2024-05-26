#type Vertex
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 TexCoord;

// model del billboard
// Cam view matrix
// cam projection matrix
// camera position
layout (std140, binding = 0) uniform UniformBlock{
  mat4 model; 
  mat4 view_cam; 
  mat4 proj_cam; 
  vec3 camera_position; 
};


out vec3 blend_color;
out vec3 s_normal;
out vec3 s_fragPos;
out vec3 camera_pos;
out vec2 uv;
out mat4 lightSpace;

void main(){
  uv = TexCoord;
  vec4 obj_position = model * vec4(position, 1.0);

  vec3 CameraRight = vec3(view_cam[0][0], view_cam[1][0], view_cam[2][0]);
  vec3 CameraUp = vec3(view_cam[0][1], view_cam[1][1], view_cam[2][1]);
  vec3 center = (vec4(0.0f, 0.0f, 0.0f, 1.0f) *  model).xyz;
  vec3 scale = vec3(model[0][0], model[1][1], model[2][2]);
  
  gl_Position = vec4(position, 1.0);
  //gl_Position = center + CameraRight * input.Pos.x * scale.x + CameraUp * input.Pos.y * scale.y;

  lightSpace = proj_cam * view_cam;
}

#type Fragment
#version 430 core

layout(location = 0) out vec4 FragColor;

uniform sampler2D texShadow;
uniform sampler2D Frag_Position;
uniform sampler2D Frag_Normal;
uniform sampler2D Frag_Color;
uniform sampler2D Met_Roug_Ao;

in vec3 blend_color;
in vec3 s_normal;
in vec3 s_fragPos;
in vec3 camera_pos;
in vec2 uv;
in mat4 lightSpace;


layout (std140, binding = 0) uniform UniformBlock{
  mat4 model;
  mat4 view_cam;
  mat4 proj_cam;
  vec3 camera_position;
};




void main(){

  // Get textures
  vec3 frag_color = texture(Frag_Color, uv).rgb;
  vec3 frag_normal = texture(Frag_Normal, uv).rgb;
  vec3 frag_position = texture(Frag_Position, uv).rgb;
  vec3 color = frag_color;

  FragColor = vec4(color, 1.0);
}