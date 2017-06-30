#include <AE/Graphics/SceneNode.hpp>
#include <functional>

namespace ae
{

SceneNode::SceneNode() :
    drawOrder(0),
    tag(""),
    parent(nullptr)
{
    children = std::map<int, sptrNode>();
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

void SceneNode::setChildren(const std::map<int, sptrNode>& _children)
{
    children = _children;
}
    
const std::map<int, sptrNode>& SceneNode::getChildren() const
{
    return children;
}

void SceneNode::removeAllChildren()
{
    children.clear();
}

void SceneNode::addChildNode(sptrNode child, int drawOrder, const std::string& tag)
{
    // child->setDrawOrder(drawOrder);
    // child->setTag(tag);
    // child->setParent(this);

    
}


} //namespace ae
