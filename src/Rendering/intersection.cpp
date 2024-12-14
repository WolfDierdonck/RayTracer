#include "intersection.hpp"
#include "polyroots.hpp"

#include <glm/gtc/epsilon.hpp>
#include <glm/ext.hpp>
#include <iostream>

Intersection intersectWithSphere(const Ray &ray, const glm::vec3 &spherePos, double radius)
{
    Intersection intersection;
    // Need to figure out if the ray given by S + tD intersects with the sphere
    // The equation of a sphere is (x - a)^2 + (y - b)^2 + (z - c)^2 = r^2
    // This can be rewritten as |P - C|^2 - r^2 = 0, substituting P = S + tD gives
    // |S + tD - C|^2 - r^2 = 0 => D^2t^2 + 2D(S - C)t + |S - C|^2 - r^2 = 0
    // This is a quadratic equation of the form at^2 + bt + c = 0
    double a = 1.0;
    double b = 2.0 * glm::dot(ray.direction, ray.start - spherePos);
    double c = glm::dot(ray.start - spherePos, ray.start - spherePos) - radius * radius;

    double roots[2];
    int numRoots = quadraticRoots(a, b, c, roots);
    if (numRoots == 1)
    {
        std::cout << "WARNING: Only one root found for sphere intersection" << std::endl;
    }

    if (numRoots == 0)
    {
        return intersection;
    }

    double entry_t = glm::min(roots[0], roots[1]);
    double exit_t = glm::max(roots[0], roots[1]);

    glm::vec3 entry_point = ray.start + (float)entry_t * ray.direction;
    glm::vec3 entry_normal = glm::normalize(entry_point - spherePos);
    glm::vec3 exit_point = ray.start + (float)exit_t * ray.direction;
    glm::vec3 exit_normal = glm::normalize(exit_point - spherePos);

    intersection.isValid = true;

    intersection.entry.isValid = true;
    intersection.entry.position = entry_point;
    intersection.entry.normal = entry_normal;
    intersection.entry.tangent = glm::cross(glm::vec3(0, 1, 0), entry_point - spherePos);
    intersection.entry.node = nullptr;
    intersection.entry.uv = glm::vec2(1 - (atan2(entry_normal.x, entry_normal.z) / (2 * M_PI) + 0.5), entry_normal.y * 0.5 + 0.5);

    intersection.exit.isValid = true;
    intersection.exit.position = exit_point;
    intersection.exit.normal = exit_normal;
    intersection.exit.tangent = glm::cross(glm::vec3(0, 1, 0), exit_point - spherePos);
    intersection.exit.node = nullptr;
    intersection.exit.uv = glm::vec2(1 - (atan2(exit_normal.x, exit_normal.z) / (2 * M_PI) + 0.5), exit_normal.y * 0.5 + 0.5);

    return intersection;
}

Intersection intersectWithBox(const Ray &ray, const glm::vec3 &boxMin, const glm::vec3 &boxMax)
{
    Intersection intersection;
    // Need to figure out if the ray given by S + tD intersects with the box
    // Solve for t values for each axis
    glm::vec3 tMin = (boxMin - ray.start) / ray.direction;
    glm::vec3 tMax = (boxMax - ray.start) / ray.direction;

    // Get the min and max t values for each axis
    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    // Find entry/exit point that is inside the box for all axes
    float entry_t = glm::max(glm::max(t1.x, t1.y), t1.z);
    float exit_t = glm::min(glm::min(t2.x, t2.y), t2.z);

    // No intersection
    if (entry_t > exit_t)
    {
        return intersection;
    }

    glm::vec3 entry_point = ray.start + (float)entry_t * ray.direction;
    glm::vec3 exit_point = ray.start + (float)exit_t * ray.direction;

    glm::vec3 entry_normal;
    glm::vec3 entry_tangent;
    glm::vec2 entry_uv;
    if (entry_t == t1.x)
    {
        entry_normal = glm::vec3(-glm::sign(ray.direction.x), 0, 0);
        entry_uv = glm::vec2((entry_point.z - boxMin.z) / (boxMax.z - boxMin.z), 1.0 - (entry_point.y - boxMin.y) / (boxMax.y - boxMin.y));
        entry_tangent = glm::vec3(0, 0, 1);
    }
    else if (entry_t == t1.y)
    {
        entry_normal = glm::vec3(0, -glm::sign(ray.direction.y), 0);
        entry_uv = glm::vec2(1.0 - (entry_point.x - boxMin.x) / (boxMax.x - boxMin.x), 1.0 - (entry_point.z - boxMin.z) / (boxMax.z - boxMin.z));
        entry_tangent = glm::vec3(1, 0, 0);
    }
    else if (entry_t == t1.z)
    {
        entry_normal = glm::vec3(0, 0, -glm::sign(ray.direction.z));
        entry_uv = glm::vec2(1.0 - (entry_point.x - boxMin.x) / (boxMax.x - boxMin.x), 1.0 - (entry_point.y - boxMin.y) / (boxMax.y - boxMin.y));
        entry_tangent = glm::vec3(0, 1, 0);
    }

    glm::vec3 exit_normal;
    glm::vec3 exit_tangent;
    glm::vec2 exit_uv;
    if (exit_t == t2.x)
    {
        exit_normal = glm::vec3(-glm::sign(ray.direction.x), 0, 0);
        exit_uv = glm::vec2((exit_point.z - boxMin.z) / (boxMax.z - boxMin.z), 1.0 - (exit_point.y - boxMin.y) / (boxMax.y - boxMin.y));
        exit_tangent = glm::vec3(0, 0, 1);
    }
    else if (exit_t == t2.y)
    {
        exit_normal = glm::vec3(0, -glm::sign(ray.direction.y), 0);
        exit_uv = glm::vec2(1.0 - (exit_point.x - boxMin.x) / (boxMax.x - boxMin.x), 1.0 - (exit_point.z - boxMin.z) / (boxMax.z - boxMin.z));
        exit_tangent = glm::vec3(1, 0, 0);
    }
    else if (exit_t == t2.z)
    {
        exit_normal = glm::vec3(0, 0, -glm::sign(ray.direction.z));
        exit_uv = glm::vec2(1.0 - (exit_point.x - boxMin.x) / (boxMax.x - boxMin.x), 1.0 - (exit_point.y - boxMin.y) / (boxMax.y - boxMin.y));
        exit_tangent = glm::vec3(0, 1, 0);
    }

    intersection.isValid = true;

    intersection.entry.isValid = true;
    intersection.entry.position = entry_point;
    intersection.entry.normal = entry_normal;
    intersection.entry.tangent = entry_tangent;
    intersection.entry.node = nullptr;
    intersection.entry.uv = entry_uv;

    intersection.exit.isValid = true;
    intersection.exit.position = exit_point;
    intersection.exit.normal = exit_normal;
    intersection.exit.tangent = exit_tangent;
    intersection.exit.node = nullptr;
    intersection.exit.uv = exit_uv;

    return intersection;
}

Intersection intersectWithCylinder(const Ray &ray, const glm::vec3 &center, double radius, double height)
{
    Intersection intersection;
    // Since a parametric cylinder has infinite height, we define 2D points (without y):
    glm::vec2 S = glm::vec2(ray.start.x, ray.start.z);
    glm::vec2 D = glm::vec2(ray.direction.x, ray.direction.z);
    glm::vec2 C = glm::vec2(center.x, center.z);

    // Need to figure out if the ray given by P = S + tD intersects with the cylinder
    // This can be rewritten as |P - C|^2 - r^2 = 0, substituting P = S + tD gives
    // |S + tD - C|^2 - r^2 = 0 => D^2t^2 + 2D(S - C)t + |S - C|^2 - r^2 = 0
    // This is a quadratic equation of the form at^2 + bt + c = 0
    double a = glm::dot(D, D);
    double b = 2.0 * glm::dot(D, S - C);
    double c = glm::dot(S - C, S - C) - radius * radius;

    double roots[2];
    int numRoots = quadraticRoots(a, b, c, roots);
    if (numRoots == 1)
    {
        std::cout << "WARNING: Only one root found for sphere intersection" << std::endl;
    }

    if (numRoots == 0)
    {
        return intersection;
    }

    double entry_t = glm::min(roots[0], roots[1]);
    double exit_t = glm::max(roots[0], roots[1]);

    float top = center.y + height;
    float bottom = center.y;

    glm::vec3 entry_point = ray.start + (float)entry_t * ray.direction;
    glm::vec3 entry_normal = glm::normalize(glm::vec3(entry_point.x - center.x, 0, entry_point.z - center.z));
    glm::vec3 exit_point = ray.start + (float)exit_t * ray.direction;
    glm::vec3 exit_normal = glm::normalize(glm::vec3(exit_point.x - center.x, 0, exit_point.z - center.z));

    if (entry_point.y < bottom && exit_point.y < bottom)
    {
        return intersection;
    }
    else if (entry_point.y > top && exit_point.y > top)
    {
        return intersection;
    }

    if (entry_point.y < bottom)
    {
        float t = (bottom - ray.start.y) / ray.direction.y;
        entry_point = ray.start + t * ray.direction;
        entry_normal = glm::vec3(0, -1, 0);
    }
    else if (entry_point.y > top)
    {
        float t = (top - ray.start.y) / ray.direction.y;
        entry_point = ray.start + t * ray.direction;
        entry_normal = glm::vec3(0, 1, 0);
    }

    if (exit_point.y < bottom)
    {
        float t = (bottom - ray.start.y) / ray.direction.y;
        exit_point = ray.start + t * ray.direction;
        exit_normal = glm::vec3(0, -1, 0);
    }
    else if (exit_point.y > top)
    {
        float t = (top - ray.start.y) / ray.direction.y;
        exit_point = ray.start + t * ray.direction;
        exit_normal = glm::vec3(0, 1, 0);
    }

    intersection.isValid = true;

    intersection.entry.isValid = true;
    intersection.entry.position = entry_point;
    intersection.entry.normal = entry_normal;
    intersection.entry.tangent = entry_normal.x == 0 && entry_normal.z == 0 ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
    intersection.entry.node = nullptr;
    intersection.entry.uv = glm::vec2(atan2(entry_point.x, entry_point.z) / (2 * M_PI) + 0.5, 1 - glm::mod(entry_point.y, 1.0f));

    intersection.exit.isValid = true;
    intersection.exit.position = exit_point;
    intersection.exit.normal = exit_normal;
    intersection.exit.tangent = exit_normal.x == 0 && exit_normal.z == 0 ? glm::vec3(1, 0, 0) : glm::vec3(0, 1, 0);
    intersection.exit.node = nullptr;
    intersection.exit.uv = glm::vec2(atan2(exit_point.x, exit_point.z) / (2 * M_PI) + 0.5, 1 - glm::mod(exit_point.y, 1.0f));

    return intersection;
}

Intersection intersectWithCone(const Ray &ray, const glm::vec3 &center)
{
    Intersection intersection;
    // Equation of a cone is (x - c_x)^2 + (z - c_z)^2 - (y - c_y)^2 = 0
    // Equation of a ray is P = S + tD
    // Expanding for one dimension gives:
    // t^2 D_x^2 + 2*t*D_x*(S_x - c_x) + (S_x - c_x)^2

    float height = 1.0;
    glm::vec3 S = ray.start;
    glm::vec3 D = ray.direction;
    glm::vec3 C = center;

    double a = D.x * D.x - D.y * D.y + D.z * D.z;
    double b = 2 * D.x * (S.x - C.x) - 2 * D.y * (S.y - C.y) + 2 * D.z * (S.z - C.z);
    double c = (S.x - C.x) * (S.x - C.x) - (S.y - C.y) * (S.y - C.y) + (S.z - C.z) * (S.z - C.z);

    double roots[2];
    int numRoots = quadraticRoots(a, b, c, roots);
    if (numRoots == 0)
    {
        return intersection;
    }

    double entry_t = glm::min(roots[0], roots[1]);
    double exit_t = glm::max(roots[0], roots[1]);

    float top = center.y;
    float bottom = center.y - height;

    glm::vec3 entry_point = ray.start + (float)entry_t * ray.direction;
    glm::vec3 entry_normal = glm::normalize(glm::vec3(2 * entry_point.x, -2 * entry_point.y, 2 * entry_point.z));
    glm::vec3 exit_point = ray.start + (float)exit_t * ray.direction;
    glm::vec3 exit_normal = glm::normalize(glm::vec3(2 * exit_point.x, -2 * exit_point.y, 2 * exit_point.z));

    // case 1. if both are above, then no intersection
    if (entry_point.y > top && exit_point.y > top)
    {
        return intersection;
    }

    // case 2. both are below, then no intersection
    if (entry_point.y < bottom && exit_point.y < bottom)
    {
        return intersection;
    }

    // case 3. if entry point is above and exit point is below, then also outside
    // note that in this case, we must have started inside the cone
    if (entry_point.y > top && exit_point.y < bottom)
    {
        return intersection;
    }

    // case 4. if entry point is below and exit point is above, then also outside
    // note that in this case, we must have started inside the cone
    if (entry_point.y < bottom && exit_point.y > top)
    {
        return intersection;
    }

    // At least one of the intersections is inside the cone now.
    glm::vec3 first_point;
    glm::vec3 first_normal;
    glm::vec3 second_point;
    glm::vec3 second_normal;

    if (exit_point.y < bottom || exit_point.y > top)
    {
        first_point = entry_point;
        first_normal = entry_normal;

        // the cap is defined by y=bottom
        float t = (bottom - ray.start.y) / ray.direction.y;
        second_point = ray.start + t * ray.direction;
        second_normal = glm::vec3(0, -1, 0);
    }
    else if (entry_point.y < bottom || entry_point.y > top)
    {
        first_point = exit_point;
        first_normal = exit_normal;

        // the cap is defined by y=bottom
        float t = (bottom - ray.start.y) / ray.direction.y;
        second_point = ray.start + t * ray.direction;
        second_normal = glm::vec3(0, -1, 0);
    }
    else
    {
        first_point = entry_point;
        first_normal = entry_normal;
        second_point = exit_point;
        second_normal = exit_normal;
    }

    if (glm::distance(ray.start, first_point) > glm::distance(ray.start, second_point))
    {
        std::swap(first_point, second_point);
        std::swap(first_normal, second_normal);
    }

    intersection.isValid = true;

    intersection.entry.isValid = true;
    intersection.entry.position = first_point;
    intersection.entry.normal = first_normal;
    float angle = atan2(first_point.x, first_point.z);
    intersection.entry.tangent = glm::vec3(glm::cos(angle), 1, glm::sin(angle));
    intersection.entry.node = nullptr;
    intersection.entry.uv = glm::vec2(angle / (2 * M_PI) + 0.5, 1 - glm::mod(first_point.y, 1.0f));

    intersection.exit.isValid = true;
    intersection.exit.position = second_point;
    intersection.exit.normal = second_normal;
    angle = atan2(second_point.x, second_point.z);
    intersection.exit.tangent = glm::vec3(glm::cos(angle), 1, glm::sin(angle));
    intersection.exit.node = nullptr;
    intersection.exit.uv = glm::vec2(angle / (2 * M_PI) + 0.5, 1 - glm::mod(second_point.y, 1.0f));

    return intersection;
}

SurfacePoint intersectWithTriangle(const Ray &ray, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec2 &t0, const glm::vec2 &t1, const glm::vec2 &t2, const glm::vec3 &n0, const glm::vec3 &n1, const glm::vec3 &n2)
{
    SurfacePoint surfacePoint;
    // Compute the normal of the triangle
    glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

    // Compute the plane equation: Ax + By + Cz + D = 0 since the normal is (A, B, C)
    float D = -glm::dot(normal, v0);

    // Check how parallel the ray is to the plane
    float denominator = glm::dot(normal, ray.direction);
    if (fabs(denominator) < 1e-9)
    {
        return surfacePoint;
    }

    // Compute the intersection point of the ray with the triangle's plane
    float t = -(glm::dot(normal, ray.start) + D) / denominator;

    glm::vec3 P = ray.start + t * ray.direction;

    // Check if the intersection point P is inside the triangle
    glm::vec3 C;

    // Edge 0
    glm::vec3 edge0 = v1 - v0;
    glm::vec3 vp0 = P - v0;
    C = glm::cross(edge0, vp0);
    if (glm::dot(normal, C) < 0)
    {
        return surfacePoint;
    }

    // Edge 1
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 vp1 = P - v1;
    C = glm::cross(edge1, vp1);
    if (glm::dot(normal, C) < 0)
    {
        return surfacePoint;
    }

    // Edge 2
    glm::vec3 edge2 = v0 - v2;
    glm::vec3 vp2 = P - v2;
    C = glm::cross(edge2, vp2);
    if (glm::dot(normal, C) < 0)
    {
        return surfacePoint;
    }

    // Now, interpolate the normal and uv coordinates of the intersection point
    float area = glm::length(glm::cross(v1 - v0, v2 - v0));
    float alpha = glm::length(glm::cross(v1 - P, v2 - P)) / area;
    float beta = glm::length(glm::cross(v2 - P, v0 - P)) / area;
    float gamma = 1 - alpha - beta;

    if (n0 != glm::vec3(0) && n1 != glm::vec3(0) && n2 != glm::vec3(0))
    {
        normal = alpha * n0 + beta * n1 + gamma * n2;
    }

    glm::vec2 uv;
    if (t0 != glm::vec2(0) && t1 != glm::vec2(0) && t2 != glm::vec2(0))
    {
        uv = alpha * t0 + beta * t1 + gamma * t2;
        uv = glm::vec2(uv.x, 1 - uv.y);
    }

    edge1 = v1 - v0;
    edge2 = v2 - v0;
    glm::vec2 deltaUV1 = t1 - t0;
    glm::vec2 deltaUV2 = t2 - t0;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    glm::vec3 tangent;
    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    surfacePoint.isValid = true;
    surfacePoint.position = P;
    surfacePoint.normal = glm::normalize(normal);
    surfacePoint.tangent = glm::normalize(tangent);
    surfacePoint.node = nullptr;
    surfacePoint.uv = uv;

    return surfacePoint;
}