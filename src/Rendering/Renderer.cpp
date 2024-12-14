#include <glm/ext.hpp>

#include "Renderer.hpp"
#include "RayTracer.hpp"
#include "RenderingThreadPool.hpp"
#include "../Modeling/GeometryNode.hpp"
#include "../Modeling/BooleanNode.hpp"

void Render(SceneNode *root, Image &image, const RenderMetadata &metadata)
{
	std::list<GeometryNode *> areaLights;
	std::stack<std::tuple<SceneNode *, glm::mat4>> stack;
	stack.push(std::make_tuple(root, glm::mat4(1.0f)));
	while (!stack.empty())
	{
		auto t = stack.top();
		stack.pop();
		SceneNode *node = std::get<0>(t);
		glm::mat4 transformationMatrix = std::get<1>(t);
		transformationMatrix *= node->trans;

		node->totalHierarchyTransform = transformationMatrix;

		// Apply the transformation matrix
		if (node->m_nodeType == NodeType::GeometryNode)
		{
			GeometryNode *geometryNode = static_cast<GeometryNode *>(node);
			if (!geometryNode->m_material)
			{
				std::cerr << "WARNING: GeometryNode " << geometryNode->m_name << " has no material" << std::endl;
				throw std::runtime_error("GeometryNode has no material");
			}

			if (geometryNode->m_emission != nullptr)
			{
				Light *light = geometryNode->m_emission;
				light->position = glm::vec3(transformationMatrix * glm::vec4(geometryNode->m_primitive->getCenter(), 1.0f));
				std::cout << "Light position " << glm::to_string(light->position) << std::endl;
				areaLights.push_back(geometryNode);
			}
		}
		else if (node->m_nodeType == NodeType::BooleanNode)
		{
			BooleanNode *booleanNode = static_cast<BooleanNode *>(node);
			if (booleanNode->children.size() != 2)
			{
				std::cerr << "WARNING: BooleanNode " << booleanNode->m_name << " has " << booleanNode->children.size() << " children" << std::endl;
				throw std::runtime_error("BooleanNode has incorrect number of children");
			}
		}

		for (SceneNode *child : node->children)
		{
			stack.push(std::make_tuple(child, transformationMatrix));
		}
	}

	std::cout << "F24: Calling Render for " << metadata.image_name << "(\n"
			  << "\t" << *root << "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
																											  "\t"
			  << "eye:  " << glm::to_string(metadata.camera_eye) << std::endl
			  << "\t" << "view: " << glm::to_string(metadata.camera_view) << std::endl
			  << "\t" << "up:   " << glm::to_string(metadata.camera_up) << std::endl
			  << "\t" << "fovy: " << metadata.camera_fovy << std::endl
			  << "\t" << "ambient: " << glm::to_string(metadata.scene_ambient) << std::endl
			  << "\t" << "lights{" << std::endl;

	for (const Light *light : metadata.scene_lights)
	{
		std::cout << "\t\t" << *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std::cout << "\t" << "enable_supersampling: " << metadata.enable_supersampling << std::endl;
	std::cout << "\t" << "thread_count: " << metadata.thread_count << std::endl;
	std::cout << ")" << std::endl;

	std::unique_ptr<Image> background_image;
	if (metadata.background_image != "")
	{
		// Load the background image
		background_image = std::make_unique<Image>(metadata.background_image);
	}

	size_t h = image.height();
	size_t w = image.width();

	RenderingThreadPool pool(metadata.thread_count, w, h);

	auto pixel_function = [&root, &metadata, &image, &background_image, &areaLights](uint32_t x, uint32_t y)
	{
		glm::vec3 colour = getPixelColor(root, x, y, metadata, background_image, areaLights);

		// Red:
		image(x, y, 0) = (double)colour.r;
		// Green:
		image(x, y, 1) = (double)colour.g;
		// Blue:
		image(x, y, 2) = (double)colour.b;
	};

	// Spawn threads to render the image
	pool.process(pixel_function);

	// Wait for all threads to finish (on destruction)
}

// Helper method to get the color of a pixel. Potentially do supersampling
glm::vec3 getPixelColor(SceneNode *root, uint x, uint y, const RenderMetadata &metadata, std::unique_ptr<Image> &background_image, std::list<GeometryNode *> &areaLights)
{
	glm::vec3 colour = glm::vec3(0.0f);

	if (!metadata.enable_supersampling)
	{
		glm::vec2 pixel = glm::vec2(x, y);
		colour = renderPixel(root, pixel, metadata, background_image, areaLights);
	}
	else
	{
		glm::vec3 colours[9] = {};

		int i = 0;
		for (double xOffset = -0.5; xOffset <= 0.5; xOffset += 0.5)
		{
			for (double yOffset = -0.5; yOffset <= 0.5; yOffset += 0.5)
			{
				glm::vec2 pixel = glm::vec2(x + xOffset, y + yOffset);
				colours[i++] = renderPixel(root, pixel, metadata, background_image, areaLights);
			}
		}

		for (int i = 0; i < 9; i++)
		{
			colour += colours[i];
		}

		colour /= 9.0f;
	}

	return colour;
}

glm::vec3 renderPixel(SceneNode *root, glm::vec2 pixel, const RenderMetadata &metadata, std::unique_ptr<Image> &background_image, std::list<GeometryNode *> &areaLights)
{
	// Get the position of the pixel in camera space
	glm::vec3 pixelPosition = pixelToCameraPos(metadata.image_width, metadata.image_height, metadata.camera_eye, metadata.camera_view, metadata.camera_up, metadata.camera_fovy, pixel);

	// Define a function to get the background color
	std::function<glm::vec3(const Ray &)> backgroundFunction = [&metadata, &background_image](const Ray &backgroundRay)
	{
		glm::vec2 pixel = rayToPixel(metadata.image_width, metadata.image_height, metadata.camera_eye, metadata.camera_view, metadata.camera_up, metadata.camera_fovy, backgroundRay);

		if (background_image)
		{
			glm::vec2 scaledPixel = glm::vec2(pixel.x * background_image->width() / metadata.image_width, pixel.y * background_image->height() / metadata.image_height);

			if (scaledPixel.x < 0 || scaledPixel.x >= background_image->width() || scaledPixel.y < 0 || scaledPixel.y >= background_image->height())
			{
				return metadata.scene_ambient;
			}

			return glm::vec3(
				(*background_image)(scaledPixel.x, scaledPixel.y, 0),
				(*background_image)(scaledPixel.x, scaledPixel.y, 1),
				(*background_image)(scaledPixel.x, scaledPixel.y, 2));
		}
		return getBackground(pixel, metadata.image_width, metadata.image_height);
	};

	// Now trace the ray
	Ray ray(metadata.camera_eye, glm::normalize(pixelPosition - metadata.camera_eye));
	return trace(root, ray, metadata.scene_ambient, metadata.scene_lights, areaLights, backgroundFunction, 1.0f);
}

// Provide a background color for the scene if no image is provided
glm::vec3 getBackground(const glm::vec2 &pixel, size_t width, size_t height)
{
	// sporadic stars in the image
	int x = (int)pixel.x;
	int y = (int)pixel.y;
	int hash = (x * 293847 ^ y * 9248132) + 1234567;

	if (hash % 500 == 0)
	{
		return glm::vec3(1.0f);
	}

	// Create a simple gradient background
	float t = glm::clamp(pixel.y / height, 0.0f, 1.0f);

	glm::vec3 bottomColor = glm::vec3(0.17f, 0.33f, 0.56f); // blue
	glm::vec3 topColor = glm::vec3(0.0f, 0.1f, 0.3f);		// dark blue

	// Combine the two blended results
	return glm::mix(bottomColor, topColor, 1 - t);
}

glm::vec3 pixelToCameraPos(
	size_t width,
	size_t height,
	const glm::vec3 &eye,
	const glm::vec3 &view,
	const glm::vec3 &up,
	double fovy,
	const glm::vec2 &pixel)
{
	// We will express the pixel in terms of the camera's coordinate system
	// The camera's coordinate system is defined by the eye (camera position),
	// view (camera viewing direction), up (camera up direction), and the fovy
	// (field of view in the y direction) parameters.
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	glm::mat4 projection = glm::perspective((float)glm::radians(fovy), aspect, 0.1f, 1000.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, eye + view, up);
	glm::mat4 invCameraMatrix = glm::inverse(projection * viewMatrix);

	float ndcX = (2.0f * pixel.x) / width - 1.0f;
	float ndcY = 1.0f - (2.0f * pixel.y) / height;
	glm::vec4 ndc = glm::vec4(ndcX, ndcY, -1.0f, 1.0f); // set z to -1.0f to get the position of the pixel on the near plane

	glm::vec4 worldCoords = invCameraMatrix * ndc;
	worldCoords /= worldCoords.w; // normalize by w

	return glm::vec3(worldCoords);
}

// This is used to convert a ray into a pixel on the screen, while getting the background color
glm::vec2 rayToPixel(
	size_t width,
	size_t height,
	const glm::vec3 &eye,
	const glm::vec3 &view,
	const glm::vec3 &up,
	double fovy,
	const Ray &ray)
{
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	glm::mat4 projection = glm::perspective((float)glm::radians(fovy), aspect, 0.1f, 1000.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, eye + view, up);
	glm::mat4 cameraMatrix = projection * viewMatrix;

	// Transform the ray start and direction into clip space
	glm::vec4 rayStartClip = cameraMatrix * glm::vec4(ray.start + ray.direction, 1.0f);
	glm::vec4 rayEndClip = cameraMatrix * glm::vec4(ray.start + 2 * ray.direction, 1.0f);

	// Normalize the clip space coordinates
	rayStartClip /= rayStartClip.w;
	rayEndClip /= rayEndClip.w;

	// Calculate the intersection with the far plane (z = 1 in clip space)
	float t = (1.0f - rayStartClip.z) / (rayEndClip.z - rayStartClip.z);
	glm::vec4 farPlanePointClip = rayStartClip + t * (rayEndClip - rayStartClip);

	// Convert the clip space coordinates to NDC (Normalized Device Coordinates)
	glm::vec3 farPlanePointNDC = glm::vec3(farPlanePointClip) / farPlanePointClip.w;

	// Convert NDC to screen space coordinates
	glm::vec2 pixel;
	pixel.x = (farPlanePointNDC.x * 0.5f + 0.5f) * width;
	pixel.y = (1.0f - (farPlanePointNDC.y * 0.5f + 0.5f)) * height;

	return pixel;
}