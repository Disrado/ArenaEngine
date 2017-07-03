#include <AE/Graphics/SceneNode.hpp>
#include <algorithm>
#include <functional>

namespace ae
{

SceneNode::SceneNode() :
    drawOrder(0),
    tag(""),
    parent(nullptr),
    children(std::map<std::string, SceneNode_sptr>())
{ }

SceneNode::SceneNode(const std::string& _tag, int _drawOrder) :
    drawOrder(_drawOrder),
    tag(_tag),
    parent(nullptr),
    children(std::map<std::string, SceneNode_sptr>())
{ }

void SceneNode::sortChildrenByDrawOrder()
{
    // TODO: sort children by drawOrder
    // auto comparer =
    //     [=](const std::pair<std::string, SceneNode_sptr>& a,
    //         const std::pair<std::string, SceneNode_sptr>& b) -> bool
    //     {
    //         return  a.second->getDrawOrder() < b.second->getDrawOrder();
    //     };
    
    // std::sort(children.begin(), children.end(), comparer);
}
    
void SceneNode::removeParent()
{
    parent = nullptr;
}
    
void SceneNode::setParent(SceneNode_sptr _parent)
{
    parent->removeChild(this->tag);
    _parent->addChild(shared_from_this());
}

std::shared_ptr<SceneNode> SceneNode::getParent() const
{
    return parent;
}

void SceneNode::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
    sortChildrenByDrawOrder();
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
    
void SceneNode::setChildren(const std::map<std::string, SceneNode_sptr>& _children)
{
    children.clear();
    children.insert(_children.begin(), _children.end());
}
    
const std::map<std::string, std::shared_ptr<SceneNode>>& SceneNode::getChildren() const
{
    return children;
}

void SceneNode::rebaseToNewParent(SceneNode_sptr newParent)
{
    parent->removeChild(this->getTag());
    newParent->addChild(shared_from_this());
}
    
void SceneNode::removeChild(const std::string& _tag)
{
    auto itr = children.find(tag);
    if(itr != children.end()) {
        children[tag]->removeParent();
        children.erase(tag);        
    }
}

void SceneNode::removeChild(SceneNode_sptr _child)
{
    auto childTag = _child->getTag();
    auto itr = children.find(childTag);
    if(itr != children.end()) {
        children[childTag]->removeParent();
        children.erase(childTag);        
    }
}

void SceneNode::removeChildren()
{
    for(auto& itr : children)
        itr.second->removeParent();
    
    children.clear();
}

void SceneNode::addChild(SceneNode_sptr child)
{
    child->setParent(shared_from_this());
    children.insert(std::pair<std::string, SceneNode_sptr>(child->getTag(), child));
    sortChildrenByDrawOrder();
}

void SceneNode::attachObject(std::shared_ptr<Object> object)
{
    attachedObject = object;
}
    
void SceneNode::attachObjectWithTransform(std::shared_ptr<Object> object)
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

void SceneNode::draw(ae::RenderWindow& renderWindow)
{
    for(auto& child : children)
        child.second->draw(renderWindow);
    
    renderWindow.draw(*(attachedObject.get()));
}
    
} //namespace ae
