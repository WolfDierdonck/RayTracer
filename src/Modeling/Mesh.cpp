#include <iostream>
#include <fstream>

#include <glm/ext.hpp>
#include <sstream>

#include "Mesh.hpp"
#include "../Rendering/intersection.hpp"

Mesh::Mesh(const std::string &fname)
	: m_vertices(), m_faces(), m_normals(), m_uvs()
{
	std::ifstream ifs(fname.c_str());
	std::string line;
	while (std::getline(ifs, line))
	{
		std::istringstream iss(line);
		std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

		if (tokens.empty())
		{
			continue;
		}

		const std::string &code = tokens[0];
		if (code == "v" && tokens.size() >= 4)
		{
			double vx = std::stod(tokens[1]);
			double vy = std::stod(tokens[2]);
			double vz = std::stod(tokens[3]);
			m_vertices.push_back(glm::vec3(vx, vy, vz));
		}
		else if (code == "f" && tokens.size() >= 4)
		{
			const std::string &vertex1 = tokens[1];
			const std::string &vertex2 = tokens[2];
			const std::string &vertex3 = tokens[3];

			std::istringstream v1s(vertex1);
			std::vector<std::string> vertex1_split;
			std::string segment;
			while (std::getline(v1s, segment, '/'))
			{
				vertex1_split.push_back(segment);
			}

			std::istringstream v2s(vertex2);
			std::vector<std::string> vertex2_split;
			while (std::getline(v2s, segment, '/'))
			{
				vertex2_split.push_back(segment);
			}

			std::istringstream v3s(vertex3);
			std::vector<std::string> vertex3_split;
			while (std::getline(v3s, segment, '/'))
			{
				vertex3_split.push_back(segment);
			}

			size_t v1 = std::stoul(vertex1_split[0]);
			size_t v2 = std::stoul(vertex2_split[0]);
			size_t v3 = std::stoul(vertex3_split[0]);

			size_t vt1, vt2, vt3;
			if (vertex1_split.size() == 2)
			{
				vt1 = std::stoul(vertex1_split[1]);
				vt2 = std::stoul(vertex2_split[1]);
				vt3 = std::stoul(vertex3_split[1]);
			}

			size_t vn1, vn2, vn3;
			if (vertex1_split.size() == 3)
			{
				if (vertex1_split[1].empty())
				{
					vt1 = -1;
					vt2 = -1;
					vt3 = -1;
				}
				else
				{
					vt1 = std::stoul(vertex1_split[1]);
					vt2 = std::stoul(vertex2_split[1]);
					vt3 = std::stoul(vertex3_split[1]);
				}

				vn1 = std::stoul(vertex1_split[2]);
				vn2 = std::stoul(vertex2_split[2]);
				vn3 = std::stoul(vertex3_split[2]);
			}

			if (tokens.size() == 5)
			{
				const std::string &vertex4 = tokens[4];

				std::istringstream v4s(vertex4);
				std::vector<std::string> vertex4_split;
				while (std::getline(v4s, segment, '/'))
				{
					vertex4_split.push_back(segment);
				}

				size_t v4 = std::stoul(vertex4_split[0]);

				size_t vt4;
				if (vertex4_split.size() == 2)
				{
					vt4 = std::stoul(vertex4_split[1]);
				}

				size_t vn4;
				if (vertex4_split.size() == 3)
				{
					if (vertex4_split[1].empty())
					{
						vt4 = -1;
					}
					else
					{
						vt4 = std::stoul(vertex4_split[1]);
					}

					vn4 = std::stoul(vertex4_split[2]);
				}

				m_faces.push_back(Triangle(v1 - 1, v2 - 1, v3 - 1, vt1 - 1, vt2 - 1, vt3 - 1, vn1 - 1, vn2 - 1, vn3 - 1));
				m_faces.push_back(Triangle(v1 - 1, v3 - 1, v4 - 1, vt1 - 1, vt3 - 1, vt4 - 1, vn1 - 1, vn3 - 1, vn4 - 1));
			}
			else
			{
				m_faces.push_back(Triangle(v1 - 1, v2 - 1, v3 - 1, vt1 - 1, vt2 - 1, vt3 - 1, vn1 - 1, vn2 - 1, vn3 - 1));
			}
		}
		else if (code == "vt" && tokens.size() >= 3)
		{
			double u = std::stod(tokens[1]);
			double v = std::stod(tokens[2]);
			m_uvs.push_back(glm::vec2(u, v));
		}
		else if (code == "vn" && tokens.size() >= 4)
		{
			double vx = std::stod(tokens[1]);
			double vy = std::stod(tokens[2]);
			double vz = std::stod(tokens[3]);
			m_normals.push_back(glm::vec3(vx, vy, vz));
		}
		else
		{
			std::cerr << "Unknown or malformed line: " << line << std::endl;
		}
	}
}

std::ostream &operator<<(std::ostream &out, const Mesh &mesh)
{
	out << "mesh {}" << std::endl;
	return out;
}

Intersection Mesh::intersect(const Ray &ray)
{
	// Get a bounding box for the mesh
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());
	for (const auto &vertex : m_vertices)
	{
		min = glm::min(min, vertex);
		max = glm::max(max, vertex);
	}

	Intersection boxIntersection = intersectWithBox(ray, min, max);
#ifdef RENDER_BOUNDING_VOLUMES
	return boxIntersection;
#endif

	if (!boxIntersection.isValid)
	{
		return Intersection();
	}

	Intersection result;

	for (const auto &face : m_faces)
	{
		glm::vec3 v0 = m_vertices[face.v1];
		glm::vec3 v1 = m_vertices[face.v2];
		glm::vec3 v2 = m_vertices[face.v3];

		glm::vec2 t0;
		glm::vec2 t1;
		glm::vec2 t2;
		if (m_uvs.size() > 0)
		{
			t0 = m_uvs[face.vt1];
			t1 = m_uvs[face.vt2];
			t2 = m_uvs[face.vt3];
		}

		glm::vec3 n0;
		glm::vec3 n1;
		glm::vec3 n2;
		if (m_normals.size() > 0)
		{
			n0 = m_normals[face.vn1];
			n1 = m_normals[face.vn2];
			n2 = m_normals[face.vn3];
		}

		SurfacePoint surface_point = intersectWithTriangle(ray, v0, v1, v2, t0, t1, t2, n0, n1, n2);
		if (surface_point.isValid)
		{
			if (!result.entry.isValid || ray.getT(surface_point.position) < ray.getT(result.entry.position))
			{
				result.entry = surface_point;
			}
			if (!result.exit.isValid || ray.getT(surface_point.position) < ray.getT(result.exit.position))
			{
				result.exit = surface_point;
			}
		}
	}

	if (!result.entry.isValid || !result.exit.isValid)
	{
		return result;
	}

	result.isValid = true;

	return result;
}

glm::vec3 Mesh::getCenter()
{
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());
	for (const auto &vertex : m_vertices)
	{
		min = glm::min(min, vertex);
		max = glm::max(max, vertex);
	}

	return (min + max) / 2.0f;
}

glm::vec3 Mesh::samplePoint()
{
	return getCenter();
}