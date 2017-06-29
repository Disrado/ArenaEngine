#include <AE/Graphics/SceneNode.hpp>

namespace ae
//{

SceneNode::SceneNode() :
    drawOrder(0),
    tag(""),
    parent(nullptr),
    children(std::vector<sptrNode>())
{
    
}

void SceneNode::setParent(sptrNode _parent)
{
    parent = _parent;
}

sptrNode SceneNode::getParent() const
{
    return parent;
}

void SceneNode::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
}

int SceneNode::getDrawOrder() const
{
    return drawOrder;
}

void SceneNode::setTag(const std::string& _tag)
{
    tag= _tag;
}

const std::string& SceneNode::getTag() const
{
    return tag;
}

void SceneNode::setChildren(const std::vector<sptrNode>& _children)
{
    children = _children;
}
    
std::vector<sptrNode> SceneNode::getChildren() const
{
    return children;
}

void SceneNode::removeAllChildren()
{
    children.clear();
}


void SceneNode::addChild(sptrNode child, int drawOrder = 0, const std::string& tag = "")
{
    children.push_back(child);
    child->setDrawOrder(drawOrder);
    child->setTag(tag);
    child->setParent(this);
}


} //namespace ae
