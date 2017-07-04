#include <AE/Graphics/SceneNode.hpp>
#include <algorithm>
#include <functional>

namespace ae
{

namespace
{
    struct DrawOrderComparator
    {
        bool operator() (const std::shared_ptr<SceneNode> l,
                         const std::shared_ptr<SceneNode> r)
        {
            return l->getDrawOrder() < r->getDrawOrder();
        }
    };
}
    
SceneNode::SceneNode() :
    drawOrder(0),
    tag(""),
    parent(nullptr),
    children(std::set<std::shared_ptr<SceneNode>>(DrawOrderComparator))
{ }

SceneNode::SceneNode(int _drawOrder, const std::string& _tag) :
    drawOrder(_drawOrder),
    tag(_tag),
    parent(nullptr),
    children(std::set<std::shared_ptr<SceneNode>>(DrawOrderComparator))
{ }
    
void SceneNode::removeParent()
{
    parent = nullptr;
}
    
void SceneNode::setParent(std::shared_ptr<SceneNode> _parent)
{
    parent->removeChild(shared_from_this());
    _parent->addChild(shared_from_this());
}

void SceneNode::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
    parent->removeChild(shared_from_this());
    parent->addChild(shared_from_this());
}

int SceneNode::getDrawOrder() const
{
    return drawOrder;
}

void SceneNode::setTag(const std::string& _tag)
{
    tag = _tag;
}

const std::string& SceneNode::getTag() const
{
    return tag;
}
    
void SceneNode::setChildren(const std::set<std::shared_ptr<SceneNode>>& _children)
{
    children.clear();
    children.insert(_children.begin(), _children.end());
}
    
const std::set<std::shared_ptr<SceneNode>>& SceneNode::getChildren() const
{
    return children;
}

void SceneNode::rebaseToNewParent(std::shared_ptr<SceneNode> newParent)
{
    parent->removeChild(shared_from_this());
    newParent->addChild(shared_from_this());
}
    
void SceneNode::removeChild(const std::string& _tag)
{
    auto itr = std::find_if(children.begin(), children.end(),
                       [&_tag](const std::shared_ptr<SceneNode>& item) -> bool
                       {
                           return item->getTag() == _tag;
                       });
    
    if(itr != children.end()) {
        (*itr)->removeParent();
        children.erase(itr);        
    }
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> _child)
{
    auto itr = children.find(_child);

    if(itr != children.end()) {
        (*itr)->removeParent();
        children.erase(itr);
    }
}

void SceneNode::removeChildren()
{
    for(auto& itr : children)
        itr->removeParent();
    
    children.clear();
}

void SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
    child->setParent(shared_from_this());
    children.insert(child);
}

void SceneNode::attachObject(std::shared_ptr<Object> object)
{
    attachedObject = object;

    attachedObject->setOrigin(this->getOrigin());
    attachedObject->setScale(this->getScale());
    attachedObject->setPosition(this->getPosition());
    attachedObject->setRotation(this->getRotation());
}
    
std::shared_ptr<Object> SceneNode::detachObject()
{
    auto returnValue = attachedObject;
    attachedObject = nullptr;
    return returnValue; 
}
    
void SceneNode::setOriginRecursive(const ae::Vector2f& origin)
{
    for(auto& child : children)
        child->setOriginRecursive(origin);

    this->setOrigin(origin);
}
    
void SceneNode::setOriginRecursive(float x, float y)
{
    for(auto& child : children)
        child->setOriginRecursive(x, y);
    
    this->setOrigin(x, y);
}
    
void SceneNode::setScaleRecursive(const ae::Vector2f& factors)
{
    for(auto& child : children)
        child->setScaleRecursive(factors);
    
    this->setScale(factors);
}
    
void SceneNode::setScaleRecursive(float factorX, float factorY)
{
    for(auto& child : children)
        child->setScaleRecursive(factorX, factorY);
    
    this->setScale(factorX, factorY);
}

void SceneNode::setPositionRecursive(const ae::Vector2f& position)
{
    for(auto& child : children)
        child->setPositionRecursive(position);
    
    this->setPosition(position);
}
    
void SceneNode::setPositionRecursive(float x, float y)
{
    for(auto& child : children)
        child->setPositionRecursive(x, y);
    
    this->setPosition(x, y);
}
    
void SceneNode::setRotationRecursive(const float angle)
{
    for(auto& child : children)
        child->setRotationRecursive(angle);
    
    this->setRotation(angle);
}

void SceneNode::moveRecursive(const Vector2f& offset)
{
    for(auto& child : children)
        child->moveRecursive(offset);
    
    this->move(offset);
}
    
void SceneNode::moveRecursive(float offsetX, float offsetY)
{
    for(auto& child : children)
        child->moveRecursive(offsetX, offsetY);
    
    this->move(offsetX, offsetY);
}
    
void SceneNode::rotateRecursive(const float angle)
{
    for(auto& child : children)
        child->rotateRecursive(angle);
    
    this->rotate(angle);
}
    
void SceneNode::scaleRecursive(const Vector2f& factor)
{
    for(auto& child : children)
        child->scaleRecursive(factor);
    
    this->scale(factor);
}
    
void SceneNode::scaleRecursive(float factorX, float factorY)
{
    for(auto& child : children)
        child->scaleRecursive(factorX, factorY);
    
    this->scale(factorX, factorY);
}        
    
void SceneNode::draw(ae::RenderWindow& renderWindow)
{
    for(auto& child : children)
        child->draw(renderWindow);
    
    renderWindow.draw(*attachedObject);
}
    
} //namespace ae
