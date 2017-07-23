#include <AE/Graphics/SceneNode.hpp>
#include <algorithm>
#include <AE/System/Logger.hpp>

namespace ae
{

SceneNode::SceneNode(int _drawOrder,
                     const std::string& _tag,
                     const ae::Vector2f& position,
                     const ae::Vector2f& scale,
                     const ae::Vector2f& origin,
                     float angle)
    : drawOrder(_drawOrder),
      tag(_tag),
      attachedObject(nullptr)
{
    Transformable::setPosition(position);
    Transformable::setScale(scale);
    Transformable::setOrigin(origin);
    Transformable::setRotation(angle);
}

static std::shared_ptr<SceneNode>
create(int drawOrder = 0,
       const std::string& _tag = "",
       const ae::Vector2f& position = ae::Vector2f(),
       const ae::Vector2f& scale = ae::Vector2f(1, 1),
       const ae::Vector2f& origin = ae::Vector2f(),
       float angle = 0.0)
{
    return std::make_shared<SceneNode>(drawOrder,
				       tag,
				       position,
				       scale,
				       origin,
				       angle);
}
    
std::shared_ptr<SceneNode>
SceneNode::createChildSceneNode(int drawOrder,
                                const std::string& tag,
                                const ae::Vector2f& position,
                                const ae::Vector2f& scale,
                                const ae::Vector2f& origin,
                                float angle)
{
    auto newChild = std::make_shared<SceneNode>(drawOrder,
                                                tag,
                                                position,
                                                scale,
                                                origin,
                                                angle);
    this->addChild(newChild);
    return newChild;
}

void SceneNode::setParent(std::shared_ptr<SceneNode> newParent)
{
    parent = newParent;
}

void SceneNode::removeParent()
{
    parent.reset();
}
    
void SceneNode::addChild(std::shared_ptr<SceneNode> child)
{
    if(child) {
        child->setParent(shared_from_this());
        children.insert(child);
    }
}

std::shared_ptr<SceneNode> SceneNode::getChildByTag(const std::string& _tag)
{
    if(children.empty())
	return nullptr;

    auto tagComparator =
	[&_tag](const std::shared_ptr<SceneNode> item) {
	    return item->getTag() == _tag;
	};
    
    auto itr = std::find_if(children.begin(), children.end(), tagComparator);
    
    if(itr != children.end())
	return *itr;
}	

void SceneNode::removeChild(const std::string& _tag)
{
    if(children.empty())
	return;

    auto tagComparator =
	[&_tag](const std::shared_ptr<SceneNode> item) {
	    return item->getTag() == _tag;
	};
 
    
    auto itr = std::find_if(children.begin(), children.end(), tagComparator);

    if(itr != children.end()) {
        (*itr)->removeParent();
        children.erase(itr);        
    }
}

void SceneNode::removeChild(std::shared_ptr<SceneNode> _child)
{
    if(children.empty())
	return;

    auto itr = children.find(_child);

    if(itr != children.end()) {
        (*itr)->removeParent();
        children.erase(itr);
    }
}

void SceneNode::removeChildren()
{
    for(auto& child : children)
        child->removeParent();
    
    children.clear();
}

void SceneNode::destroyChildrenRecursive()
{
    for(auto& child : children) {
	child->removeParent(); //this using instead removeChildren,
	                       //because removeChildren will again call loop  
	child->detachObject();
        child->destroyChildrenRecursive();
    }

    children.clear();
}
    
void SceneNode::rebaseToNewParent(std::shared_ptr<SceneNode> newParent)
{
    if(newParent) {
	if(auto ptr = parent.lock())
	    ptr->removeChild(shared_from_this());
	
        newParent->addChild(shared_from_this());
    }
}
    
void SceneNode::rebaseChildrenToNewParent(std::shared_ptr<SceneNode> newParent)
{
    if(newParent) {
        for(auto& child : children)
            newParent->addChild(child);

        children.clear();
    }
}       

void SceneNode::attachObject(std::shared_ptr<Object> object)
{
    if(object) {
        attachedObject = object;
	
	attachedObject->setOrigin(this->getOrigin());
	attachedObject->setScale(getScale());
	attachedObject->setPosition(this->getPosition());
	attachedObject->setRotation(this->getRotation());
    }
}
    
std::shared_ptr<Object> SceneNode::detachObject()
{
    if(attachedObject) {
        auto returnValue = attachedObject;
        attachedObject = nullptr;
        return returnValue;
    } else
        return nullptr;
}
    
void SceneNode::setDrawOrder(int _drawOrder)
{// TODO: sort children instead add/remove
    drawOrder = _drawOrder;
    if(auto ptr = parent.lock()) {
	ptr->removeChild(shared_from_this()); 
	ptr->addChild(shared_from_this());
    }
}

int SceneNode::getChildrenCount() const
{
    return children.size();
}
    
void SceneNode::setOrigin(const Vector2f& origin)
{
    this->Transformable::setOrigin(origin);
    if(attachedObject)
        attachedObject->setOrigin(origin);
}
    
void SceneNode::setOrigin(float x, float y)
{
    this->Transformable::setOrigin(x, y);
    if(attachedObject)
        attachedObject->setOrigin(x, y);
}    

void SceneNode::setScale(const Vector2f& factors)
{
    this->Transformable::setScale(factors);
    if(attachedObject)
        attachedObject->setScale(factors);
}

void SceneNode::setScale(float factorX, float factorY)
{
    this->Transformable::setScale(factorX, factorY);
    if(attachedObject)
        attachedObject->setScale(factorX, factorY);
}

void SceneNode::setPosition(const Vector2f& position)
{
    this->Transformable::setPosition(position);
    if(attachedObject)
        attachedObject->setPosition(position);
}

void SceneNode::setPosition(float x, float y)
{
    this->Transformable::setPosition(x, y);
    if(attachedObject)
        attachedObject->setPosition(x, y);
}    

void SceneNode::setRotation(const float angle)
{
    this->Transformable::setRotation(angle);
    if(attachedObject)
        attachedObject->setRotation(angle);
}
        
void SceneNode::move(const Vector2f& offset)
{
    this->Transformable::move(offset);
    if(attachedObject)
        attachedObject->move(offset);
}

void SceneNode::move(float offsetX, float offsetY)
{
    this->Transformable::move(offsetX, offsetY);
    if(attachedObject)
        attachedObject->move(offsetX, offsetY);
}
    
void SceneNode::rotate(const float angle)
{
    this->Transformable::rotate(angle);
    if(attachedObject)
        attachedObject->rotate(angle);
}
    
void SceneNode::scale(const Vector2f& factor)
{
    this->Transformable::scale(factor);
    if(attachedObject)
        attachedObject->scale(factor);
}

void SceneNode::scale(float factorX, float factorY)
{
    this->Transformable::scale(factorX, factorY);
    if(attachedObject)
        attachedObject->setOrigin(factorX, factorY);
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
    
void SceneNode::draw(RenderTarget& target, RenderStates states) const
{
    for(auto& child : children)
        child->draw(target, states);

    if(attachedObject)
        attachedObject->draw(target, states);
}
    
} //namespace ae
