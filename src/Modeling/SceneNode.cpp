#include "SceneNode.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;

//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string &name)
	: m_name(name),
	  m_nodeType(NodeType::SceneNode),
	  trans(mat4()),
	  invtrans(mat4()),
	  m_nodeId(nodeInstanceCount++)
{
}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode &other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for (SceneNode *child : other.children)
	{
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
	for (SceneNode *child : children)
	{
		delete child;
	}
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4 &m)
{
	trans = m;
	invtrans = glm::inverse(m);
	transpose_inv_trans = glm::transpose(glm::inverse(glm::mat3(trans)));
}

//---------------------------------------------------------------------------------------
const glm::mat4 &SceneNode::get_transform() const
{
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4 &SceneNode::get_inverse() const
{
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode *child)
{
	children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode *child)
{
	children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle)
{
	vec3 rot_axis;

	switch (axis)
	{
	case 'x':
		rot_axis = vec3(1, 0, 0);
		break;
	case 'y':
		rot_axis = vec3(0, 1, 0);
		break;
	case 'z':
		rot_axis = vec3(0, 0, 1);
		break;
	default:
		break;
	}
	mat4 rot_matrix = glm::rotate(glm::radians(angle), rot_axis);
	set_transform(rot_matrix * trans);
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 &amount)
{
	set_transform(glm::scale(amount) * trans);
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3 &amount)
{
	set_transform(glm::translate(amount) * trans);
}

//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const
{
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const SceneNode &node)
{

	// os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType)
	{
	case NodeType::SceneNode:
		os << "SceneNode";
		break;
	case NodeType::GeometryNode:
		os << "GeometryNode";
		break;
	case NodeType::BooleanNode:
		os << "BooleanNode";
		break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}
