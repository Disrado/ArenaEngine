#include <AE/Graphics/SceneNode.hpp>
#include <algorithm>
#include <AE/System/Logger.hpp>


namespace ae
{

namespace
{
    typedef ae::SceneNode::SNodePtr SNodePtr;
    typedef ae::SceneNode::TObjPtr TObjPtr;
}
    
//------------------------------------------------------------------------------    
SceneNode::SceneNode(const std::string&  _name,
		     int                 _drawOrder,
		     bool                visible,
		     const ae::Vector2f& position,
		     const ae::Vector2f& scale,
		     const ae::Vector2f& origin,
		     float               angle)
    : name(_name),
      drawOrder(_drawOrder),
      visible(visible)
{
    Transformable::setPosition(position);
    Transformable::setScale(scale);
    Transformable::setOrigin(origin);
    Transformable::setRotation(angle);
}
//------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
    if(auto prnt = parent.lock()) {
	prnt->removeChild(shared_from_this());
    }

    for(auto& obj : attachedObjects) {
	obj.second->notifyDetached();
    }
}
//------------------------------------------------------------------------------    
SNodePtr SceneNode::createChildSceneNode(const std::string& name,
					 int  drawOrder)
{
    auto newChild = std::make_shared<SceneNode>(name, drawOrder);

    setParentNodeParameters(newChild);
    
    this->addChild(newChild);
    
    return newChild;
}
//------------------------------------------------------------------------------
SNodePtr SceneNode::create(const std::string&  name,
			   int                 drawOrder,
			   bool                visible,
			   const ae::Vector2f& position,
			   const ae::Vector2f& scale,
			   const ae::Vector2f& origin,
			   float               angle)
{
    return std::make_shared<SceneNode>(name, drawOrder, visible,
				       position, scale, origin, angle);
}
//------------------------------------------------------------------------------
void SceneNode::setParent(SNodePtr newParent)
{
    parent = newParent;
}
//------------------------------------------------------------------------------
void SceneNode::removeParent()
{
    parent.reset();
}
//------------------------------------------------------------------------------
void SceneNode::setParentNodeParameters(SNodePtr SNode)
{
    SNode->setOrigin(getOrigin());
    SNode->setScale(getScale());
    SNode->setPosition(getPosition());
    SNode->setRotation(getRotation());
    SNode->setVisible(visible);
}
//------------------------------------------------------------------------------ 
bool SceneNode::isVisible()
{
    return visible;
}
//------------------------------------------------------------------------------    
void SceneNode::setVisible(bool _visible)
{
    visible = _visible;

    for(auto& obj : attachedObjects)
	obj.second->setVisible(_visible);
}
//------------------------------------------------------------------------------
void SceneNode::setVisibleRecursive(bool _visible)
{
    visible = _visible;

    for(auto& obj : attachedObjects)
	obj.second->setVisible(_visible);

    for(auto& child : children)
	child.second->setVisible(_visible);
}
//------------------------------------------------------------------------------    
void SceneNode::addChild(SNodePtr child)
{
    if(child) {
	SNodePtr parent(nullptr);
	if(parent = child->parent.lock()) {

	    auto res = children.insert(
		std::make_pair(child->getName(), child));

	    if(res.second) {
		child->setParent(shared_from_this());
		setParentNodeParameters(child);
	    } else {
		ae::Logger::getInstance().write(
		    "WARNING",
		    "Node " + child->name +
		    "was not attached because an node of the "
		    "same name was already attached to this node");
	    }

	} else {
	    ae::Logger::getInstance().write(
		"WARNING",
		"SceneNode " + child->name +
		"already was a child of " + parent->name);
        }
    }
}
//------------------------------------------------------------------------------
SNodePtr SceneNode::getChild(const std::string& _name)
{
    if(children.empty())
	return nullptr;

    auto itr = children.find(_name);
	
    if(itr != children.end()) {
	return itr->second;
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + _name + "does not exist in " + name);
	
	return nullptr;
    }
}	
//------------------------------------------------------------------------------
void SceneNode::removeChild(SNodePtr childToRemove)
{
    if(children.empty())
	return;

    auto itr = children.find(childToRemove->getName());

    if(itr != children.end()) {
        itr->second->removeParent();
        children.erase(itr);
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + childToRemove->name +
	    "does not exist in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::removeChild(const std::string& _name)
{
    if(children.empty())
	return;

    auto itr = children.find(_name);

    if(itr != children.end()) {
        itr->second->removeParent();
        children.erase(itr);
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + _name + "does not exist in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::removeChildren()
{
    for(auto& child : children)
        child.second->removeParent();
    
    children.clear();
}
//------------------------------------------------------------------------------
void SceneNode::destroyChild(SNodePtr childToRemove)
{
    if(children.empty())
	return;

    auto itr = children.find(childToRemove->getName());

    if(itr != children.end()) {
        itr->second->removeParent();
	itr->second->destroyChildren();
        children.erase(itr);
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + childToRemove->name +
	    "does not exist in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::destroyChild(const std::string& _name)
{
    if(children.empty())
	return;

    auto itr = children.find(_name);

    if(itr != children.end()) {
        itr->second->removeParent();
	itr->second->destroyChildren();
        children.erase(itr);
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + _name + "does not exist in " + name);
    }   
}
//------------------------------------------------------------------------------
void SceneNode::destroyChildren()
{
    for(auto& child : children) {
        child.second->removeParent();
	child.second->detachAllObjects();
	child.second->destroyChildren();
    }
    
    children.clear();
}
//------------------------------------------------------------------------------
void SceneNode::rebaseToNewParent(SNodePtr newParent)
{
    if(newParent) {
	if(auto ptr = parent.lock())
	    ptr->removeChild(shared_from_this());
	
        newParent->addChild(shared_from_this());
    }
}
//------------------------------------------------------------------------------    
void SceneNode::rebaseChildrenToNewParent(SNodePtr newParent)
{
    if(newParent) {
        for(auto& child : children)
            newParent->addChild(child.second);

        children.clear();
    }
}       
//------------------------------------------------------------------------------
void SceneNode::attachObject(TObjPtr object)
{
    if(object){
	if(!object->isAttached()) {

	    auto res = attachedObjects.insert(
		std::make_pair(object->getName(), object));

	    if(res.second) {
		object->notifyAttached(shared_from_this());
	    
		object->setOrigin(getOrigin());
		object->setScale(getScale());
		object->setPosition(getPosition());
		object->setRotation(getRotation());
	    } else {
		ae::Logger::getInstance().write(
		    "WARNING",
		    "Object " + object->getName() +
		    "was not attached because an object of the " 
		    "same name was already attached to this node");
	    }
	    
	} else {
	    ae::Logger::getInstance().write(
		"WARNING",
		"Object " + object->getName() +
		"was not attached because an object already "
		"attached to " + object->getParentSceneNode()->name); 
	}
    }
}
//------------------------------------------------------------------------------	    
void SceneNode::detachObject(TObjPtr object)
{
    auto itr = attachedObjects.find(object->getName());

    if(itr != attachedObjects.end()) {
	attachedObjects.erase(itr);
	object->notifyDetached();
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Object named " + object->getName() +
	    "is not attached to " + name);
    }
}
//------------------------------------------------------------------------------    
TObjPtr SceneNode::detachObject(const std::string& objectName)
{
    auto itr = attachedObjects.find(objectName);

    if(itr != attachedObjects.end()) {
	auto ret = itr->second;
	attachedObjects.erase(itr);
	ret->notifyDetached();
	return ret;
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Object " + objectName + " is not attached to this node");
    }
}	
//------------------------------------------------------------------------------
void SceneNode::detachAllObjects()
{
    for(auto& obj : attachedObjects)
	obj.second->notifyDetached();

    attachedObjects.clear();	
}
//------------------------------------------------------------------------------    
TObjPtr SceneNode::getAttachedObject(const std::string& objectName)
{
    auto itr = attachedObjects.find(objectName);
    
    if(itr != attachedObjects.end()) {
	return itr->second;
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Object " + objectName + " not found in " + name);
    }
}
//------------------------------------------------------------------------------
int SceneNode::numAttachedObjects()
{
    return attachedObjects.size();
}
//------------------------------------------------------------------------------    
void SceneNode::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
    
    if(auto ptr = parent.lock()) {
	ptr->removeChild(shared_from_this());
	ptr->addChild(shared_from_this());
    }	
}
//------------------------------------------------------------------------------
const SNodePtr SceneNode::getParent() const
{
    if(auto ptr = parent.lock())
	return ptr;
    else
	return nullptr;
}
//------------------------------------------------------------------------------
int SceneNode::getDescendantCount() const
{
    for(auto child : children)
	return children.size() + child.second->getDescendantCount();
}
//------------------------------------------------------------------------------    
void SceneNode::setOrigin(const Vector2f& origin)
{
    this->Transformable::setOrigin(origin);
    for(auto& obj : attachedObjects)
        obj.second->setOrigin(origin);
}
//------------------------------------------------------------------------------    
void SceneNode::setOrigin(float x, float y)
{
    this->Transformable::setOrigin(x, y);
    for(auto& obj : attachedObjects)
	obj.second->setOrigin(x, y);
}    
//------------------------------------------------------------------------------
void SceneNode::setScale(const Vector2f& factors)
{
    this->Transformable::setScale(factors);
    for(auto& obj : attachedObjects)
        obj.second->setScale(factors);
}
//------------------------------------------------------------------------------
void SceneNode::setScale(float factorX, float factorY)
{
    this->Transformable::setScale(factorX, factorY);
    for(auto& obj : attachedObjects)
	obj.second->setOrigin(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::setPosition(const Vector2f& position)
{
    this->Transformable::setPosition(position);
    for(auto& obj : attachedObjects)
        obj.second->setPosition(position);
}
//------------------------------------------------------------------------------
void SceneNode::setPosition(float x, float y)
{
    this->Transformable::setPosition(x, y);
    for(auto& obj : attachedObjects)
	obj.second->setPosition(x, y);
}    
//------------------------------------------------------------------------------
void SceneNode::setRotation(const float angle)
{
    this->Transformable::setRotation(angle);
    for(auto& obj : attachedObjects)
        obj.second->setRotation(angle);
}
//------------------------------------------------------------------------------        
void SceneNode::move(const Vector2f& offset)
{
    this->Transformable::move(offset);
    for(auto& obj : attachedObjects)
        obj.second->move (offset);
}
//------------------------------------------------------------------------------
void SceneNode::move(float offsetX, float offsetY)
{
    this->Transformable::move(offsetX, offsetY);
    for(auto& obj : attachedObjects)
	obj.second->move(offsetX, offsetY);
}
//------------------------------------------------------------------------------    
void SceneNode::rotate(const float angle)
{
    this->Transformable::rotate(angle);
    for(auto& obj : attachedObjects)
        obj.second->rotate(angle);
}
//------------------------------------------------------------------------------    
void SceneNode::scale(const Vector2f& factor)
{
    this->Transformable::scale(factor);
    for(auto& obj : attachedObjects)
        obj.second->scale(factor);
}
//------------------------------------------------------------------------------
void SceneNode::scale(float factorX, float factorY)
{
    this->Transformable::scale(factorX, factorY);
    for(auto& obj : attachedObjects)
	obj.second->scale(factorX, factorY);
}
//------------------------------------------------------------------------------    
void SceneNode::setOriginRecursive(const ae::Vector2f& origin)
{
    this->setOrigin(origin);

    for(auto& obj : attachedObjects)
	obj.second->setOrigin(origin);

    for(auto& child : children)
        child.second->setOriginRecursive(origin);
}
//------------------------------------------------------------------------------    
void SceneNode::setOriginRecursive(float x, float y)
{
    this->setOrigin(x, y);

    for(auto& obj : attachedObjects)
	obj.second->setOrigin(x, y);

    for(auto& child : children)
        child.second->setOriginRecursive(x, y);
}
//------------------------------------------------------------------------------    
void SceneNode::setScaleRecursive(const ae::Vector2f& factors)
{
    this->setScale(factors);

    for(auto& obj : attachedObjects)
	obj.second->setScale(factors);

    for(auto& child : children)
        child.second->setScaleRecursive(factors);
}
//------------------------------------------------------------------------------    
void SceneNode::setScaleRecursive(float factorX, float factorY)
{
    this->setScale(factorX, factorY);

    for(auto& obj : attachedObjects)
	obj.second->setScale(factorX, factorY);
    
    for(auto& child : children)
        child.second->setScaleRecursive(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::setPositionRecursive(const ae::Vector2f& position)
{
    this->setPosition(position);

    for(auto& obj : attachedObjects)
	obj.second->setPosition(position);

    for(auto& child : children)
        child.second->setPositionRecursive(position);
}
//------------------------------------------------------------------------------
void SceneNode::setPositionRecursive(float x, float y)
{
    this->setPosition(x, y);

    for(auto& obj : attachedObjects)
	obj.second->setPosition(x, y);

    for(auto& child : children)
        child.second->setPositionRecursive(x, y);
}
//------------------------------------------------------------------------------
void SceneNode::setRotationRecursive(const float angle)
{
    this->setRotation(angle);

    for(auto& obj : attachedObjects)
	obj.second->setRotation(angle);

    for(auto& child : children)
        child.second->setRotationRecursive(angle);
}
//------------------------------------------------------------------------------
void SceneNode::moveRecursive(const Vector2f& offset)
{
    this->move(offset);

    for(auto& obj : attachedObjects)
	obj.second->move(offset);

    for(auto& child : children)
        child.second->moveRecursive(offset);
}
//------------------------------------------------------------------------------
void SceneNode::moveRecursive(float offsetX, float offsetY)
{
    this->move(offsetX, offsetY);

    for(auto& obj : attachedObjects)
	obj.second->move(offsetX, offsetY);

    for(auto& child : children)
        child.second->moveRecursive(offsetX, offsetY);
}
//------------------------------------------------------------------------------
void SceneNode::rotateRecursive(const float angle)
{
    this->rotate(angle);
    
    for(auto& obj : attachedObjects)
	obj.second->rotate(angle);

    for(auto& child : children)
        child.second->rotateRecursive(angle);
}
//------------------------------------------------------------------------------    
void SceneNode::scaleRecursive(const Vector2f& factor)
{
    this->scale(factor);

    for(auto& obj : attachedObjects)
	obj.second->scale(factor);

    for(auto& child : children)
        child.second->scaleRecursive(factor);
}
//------------------------------------------------------------------------------
void SceneNode::scaleRecursive(float factorX, float factorY)
{
    this->scale(factorX, factorY);

    for(auto& obj : attachedObjects)
	obj.second->scale(factorX, factorY);

    for(auto& child : children)
        child.second->scaleRecursive(factorX, factorY);
}        
//------------------------------------------------------------------------------
void SceneNode::draw(RenderTarget& target, RenderStates states) const
{
    for(auto& child : children) {
	if(child.second->isVisible())
	    child.second->draw(target, states);
    }

    for(auto& obj : attachedObjects) {
	if(obj.second->isVisible())
	    obj.second->draw(target, states);
    }
}
    
} // namespace ae
