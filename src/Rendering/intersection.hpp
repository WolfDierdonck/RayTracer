#pragma once

#include <memory>
#include <iostream>
#include <glm/glm.hpp>

class GeometryNode;

struct Ray
{
    Ray(const glm::vec3 &start, const glm::vec3 &direction)
        : start(start), direction(direction)
    {
    }

    Ray(const Ray &ray)
        : start(ray.start), direction(ray.direction)
    {
    }

    float getT(const glm::vec3 &point) const
    {
        glm::vec3 diff = point - start;

        float t = glm::dot(diff, direction);

        return t;
    }

    glm::vec3 start;
    glm::vec3 direction;
};

struct SurfacePoint
{
    bool isValid;
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec2 uv;
    const GeometryNode *node;

    SurfacePoint()
        : isValid(false)
    {
    }

    // Copy constructor
    SurfacePoint(const SurfacePoint &surfacePoint)
        : isValid(surfacePoint.isValid), position(surfacePoint.position), normal(surfacePoint.normal), tangent(surfacePoint.tangent), uv(surfacePoint.uv), node(surfacePoint.node)
    {
    }

    // Move constructor
    SurfacePoint(SurfacePoint &&surfacePoint)
        : isValid(surfacePoint.isValid), position(std::move(surfacePoint.position)), normal(std::move(surfacePoint.normal)), tangent(std::move(surfacePoint.tangent)), uv(std::move(surfacePoint.uv)), node(surfacePoint.node)
    {
    }

    SurfacePoint &operator=(const SurfacePoint &surfacePoint)
    {
        isValid = surfacePoint.isValid;
        position = surfacePoint.position;
        normal = surfacePoint.normal;
        tangent = surfacePoint.tangent;
        uv = surfacePoint.uv;
        node = surfacePoint.node;
        return *this;
    }

    SurfacePoint &operator=(SurfacePoint &&surfacePoint)
    {
        isValid = surfacePoint.isValid;
        position = std::move(surfacePoint.position);
        normal = std::move(surfacePoint.normal);
        tangent = std::move(surfacePoint.tangent);
        uv = std::move(surfacePoint.uv);
        node = surfacePoint.node;
        return *this;
    }
};

struct Intersection
{
    bool isValid;
    SurfacePoint entry;
    SurfacePoint exit;

    Intersection()
        : isValid(false)
    {
    }

    // Copy constructor
    Intersection(const Intersection &intersection)
        : isValid(intersection.isValid), entry(intersection.entry), exit(intersection.exit)
    {
    }

    // Move constructor
    Intersection(Intersection &&intersection)
        : isValid(intersection.isValid), entry(std::move(intersection.entry)), exit(std::move(intersection.exit))
    {
    }

    Intersection &operator=(const Intersection &intersection)
    {
        isValid = intersection.isValid;
        entry = intersection.entry;
        exit = intersection.exit;
        return *this;
    }

    Intersection &operator=(Intersection &&intersection)
    {
        isValid = intersection.isValid;
        entry = std::move(intersection.entry);
        exit = std::move(intersection.exit);
        return *this;
    }
};

Intersection intersectWithSphere(const Ray &ray, const glm::vec3 &spherePos, double radius);
Intersection intersectWithBox(const Ray &ray, const glm::vec3 &boxMin, const glm::vec3 &boxMax);
Intersection intersectWithCylinder(const Ray &ray, const glm::vec3 &center, double radius, double height);
Intersection intersectWithCone(const Ray &ray, const glm::vec3 &center);
SurfacePoint intersectWithTriangle(const Ray &ray, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec2 &t0, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2);