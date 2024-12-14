#pragma once

#include <glm/glm.hpp>
#include "./Light.hpp"

class Material
{
public:
  Material(const glm::vec3 &kd, const glm::vec3 &ks, double shininess, double reflectivity, double transparency);
  virtual ~Material();
  glm::vec3 getKd() const { return m_kd; }
  glm::vec3 getKs() const { return m_ks; }
  double getShininess() const { return m_shininess; }
  double getReflectivity() const { return m_reflectivity; }
  double getTransparency() const { return m_transparency; }

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double m_reflectivity;
  double m_transparency;
};
