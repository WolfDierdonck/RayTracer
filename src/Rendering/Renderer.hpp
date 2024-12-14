#pragma once

#include <glm/glm.hpp>

#include "../Modeling/SceneNode.hpp"
#include "../Modeling/Light.hpp"
#include "Image.hpp"
#include "../Modeling/Primitive.hpp"
#include "../Lua/scene_lua.hpp"

void Render(SceneNode *root, Image &image, const RenderMetadata &metadata);

glm::vec3 getPixelColor(SceneNode *root, uint x, uint y, const RenderMetadata &metadata, std::unique_ptr<Image> &background_image, std::list<GeometryNode *> &areaLights);

glm::vec3 renderPixel(SceneNode *root, glm::vec2 pixel, const RenderMetadata &metadata, std::unique_ptr<Image> &background_image, std::list<GeometryNode *> &areaLights);

glm::vec3 getBackground(const glm::vec2 &pixel, size_t width, size_t height);

glm::vec3 pixelToCameraPos(
	size_t width,
	size_t height,
	const glm::vec3 &eye,
	const glm::vec3 &view,
	const glm::vec3 &up,
	double fovy,
	const glm::vec2 &pixel);

glm::vec2 rayToPixel(
	size_t width,
	size_t height,
	const glm::vec3 &eye,
	const glm::vec3 &view,
	const glm::vec3 &up,
	double fovy,
	const Ray &ray);