// Termm--Fall 2024

#pragma once

#include <string>
#include <glm/glm.hpp>
#include <list>

class Light;

bool run_lua(const std::string &filename);

struct RenderMetadata
{
  std::string image_name;
  int image_width;
  int image_height;
  glm::vec3 camera_eye;
  glm::vec3 camera_view;
  glm::vec3 camera_up;
  double camera_fovy;
  glm::vec3 scene_ambient;
  std::list<Light *> scene_lights;
  bool enable_supersampling;
  uint thread_count;
  std::string background_image;
};
