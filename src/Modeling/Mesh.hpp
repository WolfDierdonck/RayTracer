#pragma once

#include <vector>
#include <iosfwd>
#include <string>

#include <glm/glm.hpp>

#include "Primitive.hpp"

// Use this #define to selectively compile your code to render the
// bounding boxes around your mesh objects. Uncomment this option
// to turn it on.
// #define RENDER_BOUNDING_VOLUMES

struct Triangle
{
	size_t v1;
	size_t v2;
	size_t v3;

	size_t vt1;
	size_t vt2;
	size_t vt3;

	size_t vn1;
	size_t vn2;
	size_t vn3;

	Triangle(size_t pv1, size_t pv2, size_t pv3, size_t pvt1, size_t pvt2, size_t pvt3, size_t pvn1, size_t pvn2, size_t pvn3)
		: v1(pv1), v2(pv2), v3(pv3), vt1(pvt1), vt2(pvt2), vt3(pvt3), vn1(pvn1), vn2(pvn2), vn3(pvn3)
	{
	}
};

// A polygonal mesh.
class Mesh : public Primitive
{
public:
	Mesh(const std::string &fname);
	virtual Intersection intersect(const Ray &ray) override;
	virtual glm::vec3 samplePoint() override;
	virtual glm::vec3 getCenter() override;

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec3> m_normals;
	std::vector<glm::vec2> m_uvs;
	std::vector<Triangle> m_faces;

	friend std::ostream &operator<<(std::ostream &out, const Mesh &mesh);
};
