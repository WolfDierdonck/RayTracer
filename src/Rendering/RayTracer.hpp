#pragma once

#include <glm/glm.hpp>
#include "../Modeling/SceneNode.hpp"
#include "../Modeling/BooleanNode.hpp"
#include "../Modeling/Light.hpp"
#include "../Modeling/Primitive.hpp"

glm::vec3 trace(
    SceneNode *root,
    const Ray &ray,
    const glm::vec3 &ambient,
    const std::list<Light *> &lights,
    const std::list<GeometryNode *> &areaLights,
    const std::function<glm::vec3(const Ray &)> backgroundFunction,
    float weight);

Intersection intersectWithScene(const SceneNode *root, const Ray &ray);

std::vector<Intersection> traverseNode(const SceneNode *node, const Ray &ray);

std::vector<Intersection> computeNodeIntersection(const SceneNode *node, const Ray &ray);

std::vector<Intersection> performCSGIntersection(const BooleanNode *node, const Ray &ray);

float getLightContribution(const SceneNode *root, const Ray &ray, const glm::vec3 &lightPosition, const SceneNode *target);

glm::vec3 calculateLighting(
    const Ray &ray,
    SurfacePoint &surfacePoint,
    const glm::vec3 &ambient,
    const std::list<std::tuple<Light *, float>> &lights);