#pragma once

#include <glm/glm.hpp>

#include "Material.hpp"
#include "../Rendering/intersection.hpp"

class Primitive
{
public:
  virtual ~Primitive();
  virtual Intersection intersect(const Ray &ray) = 0;
  virtual glm::vec3 samplePoint() = 0;
  virtual glm::vec3 getCenter() = 0;
};

class Sphere : public Primitive
{
public:
  virtual ~Sphere();
  virtual Intersection intersect(const Ray &ray) override;
  virtual glm::vec3 samplePoint() override;
  virtual glm::vec3 getCenter() override;
};

class Cube : public Primitive
{
public:
  virtual ~Cube();
  virtual Intersection intersect(const Ray &ray) override;
  virtual glm::vec3 samplePoint() override;
  virtual glm::vec3 getCenter() override;
};

class Cylinder : public Primitive
{
public:
  virtual ~Cylinder();
  virtual Intersection intersect(const Ray &ray) override;
  virtual glm::vec3 samplePoint() override;
  virtual glm::vec3 getCenter() override;
};

class Cone : public Primitive
{
public:
  virtual ~Cone();
  virtual Intersection intersect(const Ray &ray) override;
  virtual glm::vec3 samplePoint() override;
  virtual glm::vec3 getCenter() override;
};

class NonhierSphere : public Primitive
{
public:
  NonhierSphere(const glm::vec3 &pos, double radius)
      : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual Intersection intersect(const Ray &ray) override;
  virtual glm::vec3 samplePoint() override;
  virtual glm::vec3 getCenter() override;

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive
{
public:
  NonhierBox(const glm::vec3 &pos, double size)
      : m_pos(pos), m_size(size)
  {
  }

  virtual ~NonhierBox();
  virtual Intersection intersect(const Ray &ray) override;
  virtual glm::vec3 samplePoint() override;
  virtual glm::vec3 getCenter() override;

private:
  glm::vec3 m_pos;
  double m_size;
};