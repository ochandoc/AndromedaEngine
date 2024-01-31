#pragma once


namespace And{

struct AmbientLight{
  float enabled = 0.0f;
  float direction[3] = {0.0f, 0.0f, 0.0f};
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f; // 48 bytes
};

struct DirectionalLight{
  float enabled = 0.0f;
  float direction[3] = {0.0f, 0.0f, 0.0f};
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f; // 48 bytes
};

struct PointLight{
  float enabled = 0.0f;
  float position[3] = {0.0f, 0.0f, 0.0f};
  float diffuse_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_color[3] = {0.0f, 0.0f, 0.0f};
  float specular_strength = 0.0f;
  float specular_shininess = 0.0f;
  float constant_att = 0.0f;
  float linear_att = 0.0f;
  float quadratic_att = 0.0f;
  float padding = 0.0f; // 64 bytes
};

struct SpotLight{
  float enabled = 0.0f;
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
  float quadratic_att = 0.0f; // 80 bytes
};

}