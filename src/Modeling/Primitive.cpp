#include "Primitive.hpp"

#include <random>

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<float> uniform01(0.0f, 1.0f);

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Intersection Sphere::intersect(const Ray &ray)
{
    return intersectWithSphere(ray, glm::vec3(0), 1.0);
}

glm::vec3 Sphere::samplePoint()
{
    float u = uniform01(gen);
    float v = uniform01(gen);
    float theta = 2.0f * M_PI * u;
    float phi = acos(2.0f * v - 1.0f);
    float x = sin(phi) * cos(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(phi);
    return glm::vec3(x, y, z);
}

glm::vec3 Sphere::getCenter()
{
    return glm::vec3(0);
}

Cube::~Cube()
{
}

Intersection Cube::intersect(const Ray &ray)
{
    return intersectWithBox(ray, glm::vec3(0), glm::vec3(1));
}

glm::vec3 Cube::samplePoint()
{
    float face = uniform01(gen) * 6.0f;
    float u = uniform01(gen);
    float v = uniform01(gen);

    if (face < 1.0f)
    {
        return glm::vec3(0, u, v);
    }
    else if (face < 2.0f)
    {
        return glm::vec3(1, u, v);
    }
    else if (face < 3.0f)
    {
        return glm::vec3(u, 0, v);
    }
    else if (face < 4.0f)
    {
        return glm::vec3(u, 1, v);
    }
    else if (face < 5.0f)
    {
        return glm::vec3(u, v, 0);
    }
    else
    {
        return glm::vec3(u, v, 1);
    }
}

glm::vec3 Cube::getCenter()
{
    return glm::vec3(0.5);
}

Cylinder::~Cylinder()
{
}

Intersection Cylinder::intersect(const Ray &ray)
{
    return intersectWithCylinder(ray, glm::vec3(0), 1.0, 1.0);
}

glm::vec3 Cylinder::samplePoint()
{
    // Surface areas
    const float sideArea = 2.0f * M_PI; // 2πr×h, with r=1, h=1
    const float capArea = M_PI;         // πr², with r=1
    const float totalArea = sideArea + 2.0f * capArea;

    float r = uniform01(gen);
    if (r < sideArea / totalArea)
    {
        // Sample on side
        float theta = uniform01(gen) * 2.0f * M_PI;
        float height = uniform01(gen);
        return glm::vec3(glm::cos(theta), height, glm::sin(theta));
    }
    else
    {
        // Sample on caps
        float theta = uniform01(gen) * 2.0f * M_PI;
        float radius = glm::sqrt(uniform01(gen)); // sqrt for uniform distribution
        float y = (r < (sideArea + capArea) / totalArea) ? 0.0f : 1.0f;
        return glm::vec3(radius * glm::cos(theta), y, radius * glm::sin(theta));
    }
}

glm::vec3 Cylinder::getCenter()
{
    return glm::vec3(0, 0.5, 0);
}

Cone::~Cone()
{
}

Intersection Cone::intersect(const Ray &ray)
{
    return intersectWithCone(ray, glm::vec3(0));
}

glm::vec3 Cone::samplePoint()
{
    throw std::runtime_error("Not implemented");
}

glm::vec3 Cone::getCenter()
{
    throw std::runtime_error("Not implemented");
}

NonhierSphere::~NonhierSphere()
{
}

Intersection NonhierSphere::intersect(const Ray &ray)
{
    return intersectWithSphere(ray, m_pos, m_radius);
}

glm::vec3 NonhierSphere::samplePoint()
{
    throw std::runtime_error("Not implemented");
}

glm::vec3 NonhierSphere::getCenter()
{
    throw std::runtime_error("Not implemented");
}

NonhierBox::~NonhierBox()
{
}

Intersection NonhierBox::intersect(const Ray &ray)
{
    return intersectWithBox(ray, m_pos, m_pos + glm::vec3(m_size));
}

glm::vec3 NonhierBox::samplePoint()
{
    throw std::runtime_error("Not implemented");
}

glm::vec3 NonhierBox::getCenter()
{
    throw std::runtime_error("Not implemented");
}