// Termm--Fall 2024

#include "BooleanNode.hpp"

//---------------------------------------------------------------------------------------
BooleanNode::BooleanNode(
    const std::string &name, BooleanType type)
    : SceneNode(name), m_type(type)
{
    m_nodeType = NodeType::BooleanNode;
}

BooleanNode::~BooleanNode() {}
