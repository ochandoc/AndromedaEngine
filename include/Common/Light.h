#pragma once



struct AmbientLight{
  float active;
  float direction[3];
  float diffuse_color[3];
  float specular_color[3];
  float specular_strength;
  float specular_shininess;

};

struct DirectionalLight{
  float active;
  float direction[3];
  float diffuse_color[3];
  float specular_color[3];
  float specular_strength;
  float specular_shininess;
};

struct PointLight{
  float active;
  float position[3];
  float diffuse_color[3];
  float specular_color[3];
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att;
};

struct SpotLight{
  float active;
  float position[3];
  float direction[3];
  float cutt_off;
  float outer_cut_off;
  float diffuse_color[3];
  float specular_color[3];
  float specular_strength;
  float specular_shininess;
  float constant_att;
  float linear_att;
  float quadratic_att;
};
