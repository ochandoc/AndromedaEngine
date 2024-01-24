#pragma once


namespace And{

struct AmbientLight{
  float active = 0.0f;
  float direction[3] = {0.0f, 0.0f, 0.0f};
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f;
};

struct DirectionalLight{
  float active = 0.0f;
  float direction[3] = {0.0f, 0.0f, 0.0f};
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f;
};

struct PointLight{
  float active = 0.0f;
  float position[3] = {0.0f, 0.0f, 0.0f};
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f;
  float constant_att = 0.0f;
  float linear_att = 0.0f;
  float quadratic_att = 0.0f;
  float padding = 0.0f;
};

struct SpotLight{
  float active = 0.0f;
  float position[3] = {0.0f, 0.0f, 0.0f};
  float direction[3] = {0.0f, 0.0f, 0.0f};
  float cutt_off = 0.0f;
  float outer_cut_off = 0.0f;
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f;
  float constant_att = 0.0f;
  float linear_att = 0.0f;
  float quadratic_att = 0.0f;
};

}