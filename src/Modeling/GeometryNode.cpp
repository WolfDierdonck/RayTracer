#include "GeometryNode.hpp"

//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string &name, Primitive *prim, Material *mat)
	: SceneNode(name), m_material(mat), m_primitive(prim), m_emission(nullptr), m_texture(nullptr), m_normal(nullptr)
{
	m_nodeType = NodeType::GeometryNode;
}

GeometryNode::~GeometryNode()
{
	if (m_texture != nullptr)
	{
		delete m_texture;
	}

	if (m_normal != nullptr)
	{
		delete m_normal;
	}
}

void GeometryNode::setMaterial(Material *mat)
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

void GeometryNode::setTexture(const std::string &texture)
{
	if (m_texture != nullptr)
	{
		delete m_texture;
	}

	m_texture = new Image(texture);
}

void GeometryNode::setNormal(const std::string &normal)
{
	if (m_normal != nullptr)
	{
		delete m_normal;
	}

	m_normal = new Image(normal);
}