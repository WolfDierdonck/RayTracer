#include <glm/ext.hpp>

#include "RayTracer.hpp"
#include "../Modeling/GeometryNode.hpp"
#include "../Modeling/BooleanNode.hpp"

const float MIN_REFLECTION_WEIGHT = 0.05;

// The main ray tracing function. This is called for each pixel in the image, as well as recursive rays.
glm::vec3 trace(
    SceneNode *root,
    const Ray &ray,
    const glm::vec3 &ambient,
    const std::list<Light *> &lights,
    const std::list<GeometryNode *> &areaLights,
    const std::function<glm::vec3(const Ray &)> backgroundFunction,
    float weight)

{
    // Check if we have intersected with the scene
    Intersection intersection = intersectWithScene(root, ray);

    if (!intersection.isValid || ray.getT(intersection.entry.position) < 0)
    {
        return backgroundFunction(ray);
    }

    // If we have, we want to calculate the lighting for this point
    SurfacePoint &surfacePoint = intersection.entry;
    SurfacePoint &exitPoint = intersection.exit;

    glm::vec3 surfacePosition = surfacePoint.position;

    // Cast a shadow ray to each point light source
    std::list<std::tuple<Light *, float>> visibleLights;
    for (Light *light : lights)
    {
        Ray shadowRay(surfacePosition, glm::normalize(light->position - surfacePosition));
        float lightContribution = getLightContribution(root, shadowRay, light->position, nullptr);
        if (lightContribution > 0)
        {
            visibleLights.push_back(std::make_tuple(light, lightContribution));
        }
    }

    // Calculate how visible this point is to the area lights
    for (GeometryNode *node : areaLights)
    {
        if (node == surfacePoint.node)
        {
            continue;
        }

        float averageLightContribution = 0;
        for (int i = 0; i < node->m_emission_samples; ++i)
        {
            glm::vec3 randomPoint = glm::vec3(node->totalHierarchyTransform * glm::vec4(node->m_primitive->samplePoint(), 1.0f));
            Ray shadowRay(surfacePosition, glm::normalize(randomPoint - surfacePosition));
            averageLightContribution += getLightContribution(root, shadowRay, randomPoint, node);
        }

        averageLightContribution /= node->m_emission_samples;
        if (averageLightContribution > 0)
        {
            Light *light = node->m_emission;
            visibleLights.push_back(std::make_tuple(light, averageLightContribution));
        }
    }

    // Get the surface color based on all the visible lights
    glm::vec3 surfaceColor = calculateLighting(ray, surfacePoint, ambient, visibleLights);

    // Potentially add transparency and reflection
    double transparency = surfacePoint.node->m_material->getTransparency();
    if (transparency > 0)
    {
        Ray transmissionRay(exitPoint.position, ray.direction);
        glm::vec3 transmissionColor = trace(root, transmissionRay, ambient, lights, areaLights, backgroundFunction, transparency * weight);
        surfaceColor = (1 - transparency) * surfaceColor + transparency * transmissionColor;
    }

    float reflectivity = surfacePoint.node->m_material->getReflectivity();
    if (reflectivity * weight > MIN_REFLECTION_WEIGHT)
    {
        glm::vec3 reflectionDirection = glm::normalize(ray.direction - 2 * glm::dot(ray.direction, surfacePoint.normal) * surfacePoint.normal);
        Ray reflectionRay(surfacePosition, reflectionDirection);
        std::function<glm::vec3(const Ray &)> reflectionBackgroundFunction = [&ambient](const Ray &backgroundRay)
        {
            return ambient;
        };
        glm::vec3 reflectionColor = trace(root, reflectionRay, ambient, lights, areaLights, reflectionBackgroundFunction, reflectivity * weight);
        surfaceColor = (1 - reflectivity) * surfaceColor + reflectivity * reflectionColor;
    }

    return surfaceColor;
}

// Helper method to intersect with the scene
Intersection intersectWithScene(const SceneNode *root, const Ray &ray)
{
    std::vector<Intersection> intersection = traverseNode(root, ray);
    Intersection result;

    for (Intersection &i : intersection)
    {
        if (!result.isValid || ray.getT(i.entry.position) < ray.getT(result.entry.position))
        {
            result = i;
        }
    }

    return result;
}

// Recursive function that performs a hierarchical traversal of the scene.
std::vector<Intersection> traverseNode(const SceneNode *node, const Ray &ray)
{
    // Transform the ray into the local space of the node
    glm::vec3 rayStart = ray.start;
    glm::vec3 rayPoint = ray.start + ray.direction;
    rayStart = glm::vec3(node->invtrans * glm::vec4(ray.start, 1.0f));
    rayPoint = glm::vec3(node->invtrans * glm::vec4(rayPoint, 1.0f));
    Ray transformedRay(rayStart, glm::normalize(rayPoint - rayStart));

    // Compute the intersection with the node (based on BooleanNode, GeometryNode, or regular SceneNode)
    std::vector<Intersection> result = computeNodeIntersection(node, transformedRay);

    for (Intersection &i : result)
    {
        // Now that we have the result, we need to transform the position, normal, and tangent back up
        i.entry.position = glm::vec3(node->trans * glm::vec4(i.entry.position, 1.0f));
        i.exit.position = glm::vec3(node->trans * glm::vec4(i.exit.position, 1.0f));
        i.entry.normal = glm::normalize(node->transpose_inv_trans * i.entry.normal);
        i.entry.tangent = glm::normalize(node->transpose_inv_trans * i.entry.tangent);
        i.exit.normal = glm::normalize(node->transpose_inv_trans * i.exit.normal);
        i.exit.tangent = glm::normalize(node->transpose_inv_trans * i.exit.tangent);
    }

    return result;
}

std::vector<Intersection> computeNodeIntersection(const SceneNode *node, const Ray &ray)
{
    // If we have a boolean node, then we need to perform CSG
    if (node->m_nodeType == NodeType::BooleanNode)
    {
        const BooleanNode *booleanNode = static_cast<const BooleanNode *>(node);
        return performCSGIntersection(booleanNode, ray);
    }

    std::vector<Intersection> result;

    // Now, we're not a CSG node
    // Check if we can intersect with the current node
    if (node->m_nodeType == NodeType::GeometryNode)
    {
        const GeometryNode *geometryNode = static_cast<const GeometryNode *>(node);
        Primitive *primitive = geometryNode->m_primitive;
        Intersection intersection = primitive->intersect(ray);
        if (intersection.isValid)
        {
            if (ray.getT(intersection.entry.position) > 0.001)
            {
                intersection.entry.node = geometryNode;
                intersection.exit.node = geometryNode;
                result.push_back(intersection);
            }
        }
    }

    // Try intersecting with all child nodes
    for (SceneNode *child : node->children)
    {
        std::vector<Intersection> intersections = traverseNode(child, ray);
        for (Intersection &intersection : intersections)
        {
            result.push_back(intersection);
        }
    }

    return result;
}

// Helper method to perform CSG intersection. This does all the logic for intersection/union/difference
std::vector<Intersection> performCSGIntersection(const BooleanNode *node, const Ray &ray)
{
    // Traverse the two children
    std::vector<Intersection> firstIntersection = traverseNode(node->children.front(), ray);
    std::vector<Intersection> secondIntersection = traverseNode(node->children.back(), ray);

    if (node->m_type == BooleanType::Intersection)
    {
        // check each line segment in the first with each in the second
        std::vector<Intersection> result;
        for (Intersection &firstI : firstIntersection)
        {
            for (Intersection &secondI : secondIntersection)
            {
                // we need to check if the two line segments intersect
                float first_start = ray.getT(firstI.entry.position);
                float first_end = ray.getT(firstI.exit.position);
                float second_start = ray.getT(secondI.entry.position);
                float second_end = ray.getT(secondI.exit.position);
                if (glm::max(first_start, second_start) < glm::min(first_end, second_end))
                {
                    // we have an intersection
                    Intersection intersection;
                    intersection.isValid = true;
                    intersection.entry = first_start > second_start ? firstI.entry : secondI.entry;
                    intersection.exit = first_end < second_end ? firstI.exit : secondI.exit;
                    result.push_back(intersection);
                }
            }
        }

        return result;
    }
    else if (node->m_type == BooleanType::Union)
    {
        // We can just return the union of the two sets
        for (Intersection &i : secondIntersection)
        {
            firstIntersection.push_back(i);
        }

        return firstIntersection;
    }
    else if (node->m_type == BooleanType::Difference)
    {
        if (secondIntersection.empty())
        {
            return firstIntersection;
        }

        std::vector<Intersection> result;
        for (Intersection &firstI : firstIntersection)
        {
            for (Intersection &secondI : secondIntersection)
            {
                SurfacePoint &A = firstI.entry;
                float tA = ray.getT(A.position);
                SurfacePoint &B = firstI.exit;
                float tB = ray.getT(B.position);
                SurfacePoint &C = secondI.entry;
                float tC = ray.getT(C.position);
                SurfacePoint &D = secondI.exit;
                float tD = ray.getT(D.position);

                // Now, we need to compute AB - CD
                // If AB is entirely before or after CD, then we return AB
                if (tC > tB || tD < tA)
                {
                    result.push_back(firstI);
                    continue;
                }

                // If AB is entirely inside CD, then we return nothing
                if (tA >= tC && tB <= tD)
                {
                    continue;
                }

                // In this case, they are partially overlapping. We need to compute the new entry and exit points
                // while making sure to flip the normal

                // If we have to cut off the entry point (tC < tA). In this case, D becomes the new A
                if (tC < tA)
                {
                    // The new line segment is [-D, B]
                    Intersection newIntersection;
                    newIntersection.isValid = true;
                    newIntersection.entry = D;
                    newIntersection.entry.normal = -newIntersection.entry.normal;
                    newIntersection.exit = B;

                    result.push_back(newIntersection);
                }
                // If we have to cutoff the exit point (tD > tB). In this case, C becomes the new B
                else if (tD > tB)
                {
                    // The new line segment is [A, -C]
                    Intersection newIntersection;
                    newIntersection.isValid = true;
                    newIntersection.entry = A;
                    newIntersection.exit = C;
                    newIntersection.exit.normal = -newIntersection.exit.normal;

                    result.push_back(newIntersection);
                }
                // In this case, CD is entirely inside AB.
                else
                {
                    // We get multiple disjoint line segments here
                    // The first line segment is [A, -C]
                    Intersection newFirstIntersection;
                    newFirstIntersection.isValid = true;
                    newFirstIntersection.entry = A;
                    newFirstIntersection.exit = C;
                    newFirstIntersection.exit.normal = -newFirstIntersection.exit.normal;

                    // The second line segment is [-D, B]
                    Intersection newSecondIntersection;
                    newSecondIntersection.isValid = true;
                    newSecondIntersection.entry = D;
                    newSecondIntersection.entry.normal = -newSecondIntersection.entry.normal;
                    newSecondIntersection.exit = B;

                    result.push_back(newFirstIntersection);
                    result.push_back(newSecondIntersection);
                }
            }
        }

        return result;
    }
}

// Get how "visible" the light is at a certain point. This is used to calculate shadows
float getLightContribution(const SceneNode *root, const Ray &ray, const glm::vec3 &lightPosition, const SceneNode *target)
{
    float contribution = 1.0f;
    Ray currentRay = ray;
    while (true)
    {
        Intersection shadowIntersection = intersectWithScene(root, currentRay);
        if (!shadowIntersection.isValid)
        {
            return contribution;
        }
        else
        {
            SurfacePoint &shadowSurfacePoint = shadowIntersection.entry;
            SurfacePoint &shadowExitPoint = shadowIntersection.exit;
            // if we have an intersection, we need to check if the intersection is before the light.
            // it can also be the case that we intersected with the target (to some approximation)
            if (currentRay.getT(shadowSurfacePoint.position) > currentRay.getT(lightPosition))
            {
                return contribution;
            }
            // If we have an intersection with the target, then we return the contribution
            else if (shadowSurfacePoint.node == target)
            {
                return contribution;
            }
            // if not, we may need to continue based on the transparency of the object
            else if (shadowSurfacePoint.node->m_material->getTransparency() > 0)
            {
                contribution *= shadowSurfacePoint.node->m_material->getTransparency();
                currentRay = Ray(shadowExitPoint.position, currentRay.direction);
            }
            else
            {
                return 0.0f;
            }
        }
    }
}

// Use a Phong illumination model to calculate the lighting at a certain point. This also handles texture/normal maps.
glm::vec3 calculateLighting(
    const Ray &ray,
    SurfacePoint &surfacePoint,
    const glm::vec3 &ambient,
    const std::list<std::tuple<Light *, float>> &lights)
{
    const GeometryNode *surface = surfacePoint.node;
    glm::vec3 surfacePosition = surfacePoint.position;
    glm::vec3 surfaceNormal = surfacePoint.normal;

    glm::vec3 surfaceColor;
    if (surface->m_texture)
    {
        float xCoord = glm::min(surfacePoint.uv.x * surface->m_texture->width(), (float)surface->m_texture->width() - 1);
        float yCoord = glm::min(surfacePoint.uv.y * surface->m_texture->height(), (float)surface->m_texture->height() - 1);
        double textureR = (*surface->m_texture)(xCoord, yCoord, 0);
        double textureG = (*surface->m_texture)(xCoord, yCoord, 1);
        double textureB = (*surface->m_texture)(xCoord, yCoord, 2);
        surfaceColor = glm::vec3(textureR, textureG, textureB);
    }
    else
    {
        surfaceColor = surface->m_material->getKd();
    }

    if (surface->m_normal)
    {
        float xCoord = surfacePoint.uv.x * (surface->m_normal->width() - 1);
        float yCoord = surfacePoint.uv.y * (surface->m_normal->height() - 1);
        double normalR = ((*surface->m_normal)(xCoord, yCoord, 0) * 2) - 1;
        double normalG = ((*surface->m_normal)(xCoord, yCoord, 1) * 2) - 1;
        double normalB = ((*surface->m_normal)(xCoord, yCoord, 2) * 2) - 1;
        glm::vec3 perturbed_normal = glm::normalize(glm::vec3(normalR, normalG, normalB));

        glm::vec3 bitangent = glm::normalize(glm::cross(surfaceNormal, surfacePoint.tangent));

        // Create TBN matrix
        glm::mat3 TBN = glm::mat3(surfacePoint.tangent, bitangent, surfaceNormal);
        surfaceNormal = glm::normalize(TBN * perturbed_normal);
    }

    glm::vec3 finalColor = ambient * surfaceColor;

    // If this is an area light, then we need to add the emission (when looking at the light)
    if (surface->m_emission)
    {
        finalColor += surface->m_emission->colour;
    }

    // Phong illumination model
    for (std::tuple<Light *, float> t : lights)
    {
        Light *light = std::get<0>(t);
        float intensity = std::get<1>(t);
        // The contribution of this light is: p(v, l) * I * (l . n) / (c_1 + c_2 * d + c_3 * d^2)
        // p(v, l) = k_d + k_s * (r . v)^p / (n . l)
        glm::vec3 l = glm::normalize(light->position - surfacePosition);
        glm::vec3 n = glm::normalize(surfaceNormal);
        glm::vec3 v = glm::normalize(ray.start - surfacePosition);

        glm::vec3 r = -l + 2 * glm::dot(l, n) * n;
        float dotRV = glm::dot(r, v);
        float dotNL = glm::dot(n, l);
        glm::vec3 shininess = surface->m_material->getKs() * glm::pow(dotRV, surface->m_material->getShininess()) / dotNL;
        if (shininess.r < 0 || shininess.g < 0 || shininess.b < 0)
        {
            shininess = glm::vec3(0.0f);
        }
        glm::vec3 p = surfaceColor + shininess;

        double lightDistance = glm::length(light->position - surfacePosition);
        double falloff = light->falloff[0] + light->falloff[1] * lightDistance + light->falloff[2] * lightDistance * lightDistance;

        glm::vec3 contribution = p * light->colour * intensity * glm::max(glm::dot(l, n), 0.0f) / falloff;
        finalColor += contribution;
    }

    finalColor.r = glm::max(0.0f, glm::min(finalColor.r, 1.0f));
    finalColor.g = glm::max(0.0f, glm::min(finalColor.g, 1.0f));
    finalColor.b = glm::max(0.0f, glm::min(finalColor.b, 1.0f));

    return finalColor;
}
