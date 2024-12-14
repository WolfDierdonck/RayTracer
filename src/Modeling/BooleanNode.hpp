#pragma once

#include "SceneNode.hpp"

enum class BooleanType
{
    Intersection,
    Union,
    Difference
};

class BooleanNode : public SceneNode
{
public:
    BooleanNode(const std::string &name, BooleanType type);
    ~BooleanNode();

    BooleanType m_type;
};
