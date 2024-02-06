#pragma once


namespace And{

struct AmbientLight{
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float enabled = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f; // 48 bytes
};

struct DirectionalLight{
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float enabled = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f; // 48 bytes
};

struct PointLight{
  float position[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float constant_att = 0.5f;
  float linear_att = 0.5f;
  float quadratic_att = 0.5f;
  float enabled = 0.5f;
  float padding = 0.5f; // 64 bytes
};

struct SpotLight{
  float enabled = 0.5f;
  float position[3] = {0.5f, 0.5f, 0.5f};
  float cutt_off = 0.5f;
  float direction[3] = {0.5f, 0.5f, 0.5f};
  float outer_cut_off = 0.5f;
  float diffuse_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_strength = 0.5f;
  float specular_color[3] = {0.5f, 0.5f, 0.5f};
  float specular_shininess = 0.5f;
  float constant_att = 0.5f;
  float linear_att = 0.5f;
  float quadratic_att = 0.5f; // 80 bytes
};

}