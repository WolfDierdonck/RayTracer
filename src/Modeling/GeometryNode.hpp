#pragma once

#include "SceneNode.hpp"
#include "Primitive.hpp"
#include "Material.hpp"
#include "../Rendering/Image.hpp"

class GeometryNode : public SceneNode
{
public:
	GeometryNode(const std::string &name, Primitive *prim,
				 Material *mat = nullptr);
	~GeometryNode();

	void setMaterial(Material *material);
	void setTexture(const std::string &texture);
	void setNormal(const std::string &normal);

	Material *m_material;
	Primitive *m_primitive;
	Image *m_texture;
	Image *m_normal;
	Light *m_emission;
	int m_emission_samples;
};
