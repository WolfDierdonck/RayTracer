// Termm--Fall 2024

#include "Material.hpp"

Material::Material(const glm::vec3 &kd, const glm::vec3 &ks, double shininess, double reflectivity, double transparency)
    : m_kd(kd), m_ks(ks), m_shininess(shininess), m_reflectivity(reflectivity), m_transparency(transparency) {};

Material::~Material()
{
}
