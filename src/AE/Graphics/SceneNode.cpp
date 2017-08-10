#include <AE/Graphics/SceneNode.hpp>
#include <algorithm>
#include <AE/System/Logger.hpp>


namespace ae
{

namespace
{
    typedef ae::SceneNode::SNodePtr   SNodePtr;
    typedef ae::SceneNode::ObjPtr     ObjPtr;
    typedef ae::SceneNode::Parameters Parameters;
    typedef ae::SceneNode::ChildItr   ChildItr;
    typedef ae::SceneNode::ObjItr     ObjItr;
}
    
//------------------------------------------------------------------------------    
SceneNode::SceneNode(const std::string&  _name,
		     int                 _drawOrder,
		     bool                visible,
		     const ae::Vector2f& position,
		     const ae::Vector2f& scale,
		     const ae::Vector2f& origin,
		     float               rotation)
    : name(_name),
      drawOrder(_drawOrder),
      visible(visible)
{
    Transformable::setPosition(position);
    Transformable::setScale(scale);
    Transformable::setOrigin(origin);
    Transformable::setRotation(rotation);
}
//------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
    if(auto prnt = parent.lock()) {
	prnt->removeChild(shared_from_this());
    }

    for(auto& obj : attachedObjects) {
	obj->notifyDetached();
    }
}
//------------------------------------------------------------------------------    
SNodePtr SceneNode::createChildSceneNode(const std::string& name,
					 int  drawOrder)
{
    auto newChild = std::make_shared<SceneNode>(name, drawOrder);
    newChild->setParentParameters(this->getParameters());
    this->addChild(newChild);

    setNeedUpdateNodesCommonPool();
    setNeedUpdateDrawableQueue();
    
    return newChild;
}
//------------------------------------------------------------------------------
SNodePtr SceneNode::create(const std::string&  name,
			   int                 drawOrder,
			   bool                visible,
			   const ae::Vector2f& position,
			   const ae::Vector2f& scale,
			   const ae::Vector2f& origin,
			   float               rotation)
{
    return std::make_shared<SceneNode>(name, drawOrder, visible,
				       position, scale, origin, rotation);
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
ChildItr SceneNode::getChildItr(SNodePtr child)
{
    return std::find(begin(children), end(children), child);
}
//------------------------------------------------------------------------------    
ChildItr SceneNode::getChildItr(const std::string &name)
{
    return std::find_if(begin(children), end(children),
                        [&name](SNodePtr lhs)
                        {
                            return lhs->getName() == name;
                        });
}
//------------------------------------------------------------------------------
ObjItr SceneNode::getObjectItr(ObjPtr obj)
{
    return std::find(begin(attachedObjects), end(attachedObjects), obj);
}
//------------------------------------------------------------------------------
ObjItr SceneNode::getObjectItr(const std::string &name)
{
    return std::find_if(begin(attachedObjects), end(attachedObjects),
                        [&name](ObjPtr lhs)
                        {
                            return lhs->getName() == name;
                        });
}
//--------------------------------------------------------------x----------------
const Parameters SceneNode::getParameters()
{
    return Parameters(shared_from_this());
}
//------------------------------------------------------------------------------
void SceneNode::setParentParameters(const Parameters& param)
{
    this->setPosition(param.position);
    this->setScale(param.scale);
    this->setOrigin(param.origin);
    this->setRotation(param.rotation);
    this->setVisible(param.visible);

    for(auto& child : children)
	child->setParentParameters(param);
}
//------------------------------------------------------------------------------
void SceneNode::setNeedUpdateDrawableQueue(bool value)
{
    needUpdateDrawableQueue = value;
}
//------------------------------------------------------------------------------
void SceneNode::setNeedUpdateNodesCommonPool(bool value)
{
    needUpdateNodesCommonPool = value;
}
//------------------------------------------------------------------------------
void SceneNode::sortChildrenByDrawOrder()
{
    std::sort(begin(children), end(children),
              [](const SNodePtr lhs, const SNodePtr rhs)
              {
                  return lhs->drawOrder < rhs->drawOrder;
              });
}
//------------------------------------------------------------------------------    
void SceneNode::sortObjectsByDrawOrder()
{
    std::sort(begin(attachedObjects), end(attachedObjects),
              [](const ObjPtr lhs, const ObjPtr rhs)
              {
                  return lhs->getDrawOrder() < rhs->getDrawOrder();
              });
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
	obj->setVisible(_visible);
}
//------------------------------------------------------------------------------
bool SceneNode::getVisible()
{
    return visible;
}
//------------------------------------------------------------------------------
void SceneNode::setVisibleRecursive(bool _visible)
{
    for(auto& obj : attachedObjects)
	obj->setVisible(_visible);

    for(auto& child : children)
	child->setVisibleRecursive(_visible);

    visible = _visible;

    setNeedUpdateDrawableQueue();
}
//------------------------------------------------------------------------------    
void SceneNode::addChild(SNodePtr child)
{
    if(child) {
	SNodePtr parent = parent = child->parent.lock();
	if(!parent) {
            // Check if child does't already exist as children of this node
            if(getChildItr(child) == children.end()) {
                children.push_back(child);
                
                child->setParent(shared_from_this());
		child->setParentParameters(this->getParameters());

                setNeedUpdateNodesCommonPool();
                setNeedUpdateDrawableQueue();
	    } else {
		Logger::getInstance().write(
		    "WARNING",
		    "Node " + child->name +
		    "was not attached because an node "
		    "was already attached to this node");
	    }
	} else {
	    Logger::getInstance().write(
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
    
    auto itr = getChildItr(_name);
    
    if(itr != children.end()) {
	return *itr;
    } else {
	Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + _name + "does not exist in " + name);
	
	return nullptr;
    }
}	
//------------------------------------------------------------------------------
//* This method is implemented separately instead simple call
//* removeChild(childToRemove->name);
//* because the search for an object by the pointer is faster than by name
void SceneNode::removeChild(SNodePtr childToRemove)
{
    if(children.empty())
	return;
    
    auto itr = getChildItr(childToRemove);

    if(itr != children.end()) {
        (*itr)->removeParent();
        children.erase(itr);

        setNeedUpdateNodesCommonPool();
        setNeedUpdateDrawableQueue();
    } else {
	Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + childToRemove->name +
	    " does not exist in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::removeChild(const std::string& _name)
{
    if(children.empty())
	return;

    auto itr = getChildItr(_name);

    if(itr != children.end()) {
        (*itr)->removeParent();
        children.erase(itr);

        setNeedUpdateNodesCommonPool();
        setNeedUpdateDrawableQueue();
    } else {
	Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + _name + "does not exist in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::removeChildren()
{
    for(auto& child : children)
        child->removeParent();

    children.clear();

    setNeedUpdateNodesCommonPool();
    setNeedUpdateDrawableQueue();
}
//------------------------------------------------------------------------------
void SceneNode::destroyChild(SNodePtr childToRemove)
{
    if(children.empty())
	return;

    auto itr = getChildItr(childToRemove);

    if(itr != children.end()) {
        (*itr)->removeParent();
	(*itr)->destroyChildren();
        (*itr)->detachAllObjects();
        children.erase(itr);

        setNeedUpdateNodesCommonPool();
        setNeedUpdateDrawableQueue();
    } else {
	Logger::getInstance().write(
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

    auto itr = getChildItr(_name);

    if(itr != children.end()) {
        (*itr)->removeParent();
	(*itr)->destroyChildren();
        (*itr)->detachAllObjects();
        children.erase(itr);

        setNeedUpdateNodesCommonPool();
        setNeedUpdateDrawableQueue();
    } else {
	Logger::getInstance().write(
	    "WARNING",
	    "Child node named " + _name + "does not exist in " + name);
    }   
}
//------------------------------------------------------------------------------
void SceneNode::destroyChildren()
{
    for(auto& child : children) {
        child->removeParent();
	child->detachAllObjects();
	child->destroyChildren();
    }
    
    children.clear();
    setNeedUpdateNodesCommonPool();
    setNeedUpdateDrawableQueue();
}
//------------------------------------------------------------------------------
void SceneNode::rebaseToNewParent(SNodePtr newParent)
{
    if(newParent) {
	if(auto ptr = parent.lock())
	    ptr->removeChild(shared_from_this());
	
        newParent->addChild(shared_from_this());

        setNeedUpdateDrawableQueue();
    }
}
//------------------------------------------------------------------------------    
void SceneNode::rebaseChildrenToNewParent(SNodePtr newParent)
{
    if(newParent) {
        for(auto& child : children)
            newParent->addChild(child);

        children.clear();

        setNeedUpdateDrawableQueue();
    }
}       
//------------------------------------------------------------------------------
void SceneNode::attachObject(ObjPtr object, int objectDrawOrder)
{
    if(object){
	if(!object->isAttached()) {
            
            auto itr = getObjectItr(object);
            
	    if(itr == attachedObjects.end()) {
                attachedObjects.push_back(object);
                
                object->notifyAttached(shared_from_this());
                object->setDrawOrder(objectDrawOrder);

		object->setOrigin(getOrigin());
		object->setScale(getScale());
		object->setPosition(getPosition());
		object->setRotation(getRotation());
		object->setVisible(visible);

                setNeedUpdateDrawableQueue();
	    } else {
		Logger::getInstance().write(
		    "WARNING",
		    "Object " + object->getName() +
		    " was not attached because an object of the " 
		    "same name was already attached to this node");
	    }
	    
	} else {
	    Logger::getInstance().write(
		"WARNING",
		"Object " + object->getName() +
		" was not attached because an object already "
		"attached to " + object->getParentSceneNode()->name); 
	}
    }
}
//------------------------------------------------------------------------------
void SceneNode::detachObject(ObjPtr object)
{
    auto itr = getObjectItr(object);

    if(itr != attachedObjects.end()) {
	attachedObjects.erase(itr);
	object->notifyDetached();
        
        setNeedUpdateDrawableQueue();
    } else {
	Logger::getInstance().write(
	    "WARNING",
	    "Object named " + object->getName() +
	    " is not attached to " + name);
    }
}
//------------------------------------------------------------------------------    
ObjPtr SceneNode::detachObject(const std::string& objectName)
{
    auto itr = getObjectItr(objectName);

    if(itr != attachedObjects.end()) {
	auto ret = *itr;
	attachedObjects.erase(itr);
	ret->notifyDetached();

        setNeedUpdateDrawableQueue();

        return ret;
    } else {
	Logger::getInstance().write(
	    "WARNING",
	    "Object named " + objectName + " is not attached to this node");
        return nullptr;
    }
}	
//------------------------------------------------------------------------------
void SceneNode::detachAllObjects()
{
    for(auto& obj : attachedObjects)
	obj->notifyDetached();

    attachedObjects.clear();
    setNeedUpdateDrawableQueue();
}
//------------------------------------------------------------------------------    
ObjPtr SceneNode::getAttachedObject(const std::string& objectName)
{
    auto itr = getObjectItr(objectName);
    
    if(itr != attachedObjects.end()) {
	return *itr;
    } else {
	Logger::getInstance().write(
	    "WARNING",
            "Object " + objectName + " not found in " + name);
        return nullptr;
    }
}
//------------------------------------------------------------------------------
int SceneNode::numAttachedObjects()
{
    return attachedObjects.size();
}
//------------------------------------------------------------------------------
std::vector<ObjPtr> SceneNode::getDrawableObjects()
{
    if(!visible)
        return std::vector<ObjPtr>();
        
    std::vector<ObjPtr> vect;

    for(auto obj : attachedObjects)
        if(obj->isVisible())
            vect.push_back(obj);

    return vect;
}
//------------------------------------------------------------------------------
std::vector<SNodePtr> SceneNode::getDrawableChildren()
{
    if(!visible)
        return std::vector<SNodePtr>();
        
    std::vector<SNodePtr> vect;

    for(auto child : children)
        if(child->isVisible())
            vect.push_back(child);

    return vect; 
}
//------------------------------------------------------------------------------
const std::vector<SNodePtr>& SceneNode::getChildren()
{
    return children;
}
//------------------------------------------------------------------------------    
void SceneNode::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
    
    if(auto ptr = parent.lock()) {
        ptr->sortChildrenByDrawOrder();
        setNeedUpdateDrawableQueue();
    }	
}
//------------------------------------------------------------------------------
const SNodePtr SceneNode::getParent()
{
    if(auto ptr = parent.lock())
	return ptr;
    else
	return nullptr;
}
//------------------------------------------------------------------------------
void SceneNode::changeObjectDrawOrder(const std::string &objName,
                                      int newDrawOrder)
{
    auto itr = getObjectItr(objName);
    
    if(itr != attachedObjects.end()) {
	(*itr)->setDrawOrder(newDrawOrder);
        sortObjectsByDrawOrder();
        setNeedUpdateDrawableQueue();
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Object " + objName + " not found in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::changeObjectDrawOrder(ObjPtr obj, int newDrawOrder)
{
    auto itr = getObjectItr(obj);
    
    if(itr != attachedObjects.end()) {
	obj->setDrawOrder(newDrawOrder);
        sortObjectsByDrawOrder(); // TODO: this call twice: there and in update
                                  // after object set flag. Need fix
        setNeedUpdateDrawableQueue();
    } else {
	ae::Logger::getInstance().write(
	    "WARNING",
	    "Object " + obj->getName() + " not found in " + name);
    }
}
//------------------------------------------------------------------------------
void SceneNode::setNeedSortAttachedObjects()
{
    needSortAttachedObjects = true;
}
//------------------------------------------------------------------------------
bool SceneNode::isNeedUpdateDrawableQueue()
{
    return needUpdateDrawableQueue;
}
//------------------------------------------------------------------------------
bool SceneNode::isNeedUpdateNodesCommonPool()
{
    return needUpdateNodesCommonPool;
}
//------------------------------------------------------------------------------    
void SceneNode::setOrigin(const Vector2f& origin)
{
    setOrigin(origin.x, origin.y);
}
//------------------------------------------------------------------------------    
void SceneNode::setOrigin(float x, float y)
{
    this->Transformable::setOrigin(x, y);
    for(auto& obj : attachedObjects)
	obj->setOrigin(x, y);
}    
//------------------------------------------------------------------------------
void SceneNode::setScale(const Vector2f& factors)
{
    setScale(factors.x, factors.y);
}
//------------------------------------------------------------------------------
void SceneNode::setScale(float factorX, float factorY)
{
    this->Transformable::setScale(factorX, factorY);
    for(auto& obj : attachedObjects)
	obj->setOrigin(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::setPosition(const Vector2f& position)
{
    setPosition(position.x, position.y);
}
//------------------------------------------------------------------------------
void SceneNode::setPosition(float x, float y)
{
    this->Transformable::setPosition(x, y);
    for(auto& obj : attachedObjects)
	obj->setPosition(x, y);
}    
//------------------------------------------------------------------------------
void SceneNode::setRotation(const float angle)
{
    this->Transformable::setRotation(angle);
    for(auto& obj : attachedObjects)
        obj->setRotation(angle);
}
//------------------------------------------------------------------------------        
void SceneNode::move(const Vector2f& offset)
{
    move(offset.x, offset.y);
}
//------------------------------------------------------------------------------
void SceneNode::move(float offsetX, float offsetY)
{
    this->Transformable::move(offsetX, offsetY);
    for(auto& obj : attachedObjects)
	obj->move(offsetX, offsetY);
}
//------------------------------------------------------------------------------    
void SceneNode::rotate(const float angle)
{
    this->Transformable::rotate(angle);
    for(auto& obj : attachedObjects)
        obj->rotate(angle);
}
//------------------------------------------------------------------------------    
void SceneNode::scale(const Vector2f& factors)
{
    scale(factors.x, factors.y);
}
//------------------------------------------------------------------------------
void SceneNode::scale(float factorX, float factorY)
{
    this->Transformable::scale(factorX, factorY);
    for(auto& obj : attachedObjects)
	obj->scale(factorX, factorY);
}
//------------------------------------------------------------------------------    
void SceneNode::setOriginRecursive(const ae::Vector2f& origin)
{
    setOriginRecursive(origin.x, origin.y);
}
//------------------------------------------------------------------------------    
void SceneNode::setOriginRecursive(float x, float y)
{
    this->setOrigin(x, y);

    for(auto& obj : attachedObjects)
	obj->setOrigin(x, y);

    for(auto& child : children)
        child->setOriginRecursive(x, y);
}
//------------------------------------------------------------------------------    
void SceneNode::setScaleRecursive(const ae::Vector2f& factors)
{
    setScaleRecursive(factors.x, factors.y);
}
//------------------------------------------------------------------------------    
void SceneNode::setScaleRecursive(float factorX, float factorY)
{
    this->setScale(factorX, factorY);

    for(auto& obj : attachedObjects)
	obj->setScale(factorX, factorY);
    
    for(auto& child : children)
        child->setScaleRecursive(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::setPositionRecursive(const ae::Vector2f& position)
{
    setPositionRecursive(position.x, position.y);
}
//------------------------------------------------------------------------------
void SceneNode::setPositionRecursive(float x, float y)
{
    this->setPosition(x, y);

    for(auto& obj : attachedObjects)
	obj->setPosition(x, y);

    for(auto& child : children)
        child->setPositionRecursive(x, y);
}
//------------------------------------------------------------------------------
void SceneNode::setRotationRecursive(const float angle)
{
    this->setRotation(angle);

    for(auto& obj : attachedObjects)
	obj->setRotation(angle);

    for(auto& child : children)
        child->setRotationRecursive(angle);
}
//------------------------------------------------------------------------------
void SceneNode::moveRecursive(const Vector2f& offset)
{
    moveRecursive(offset.x, offset.y);
}
//------------------------------------------------------------------------------
void SceneNode::moveRecursive(float offsetX, float offsetY)
{
    this->move(offsetX, offsetY);

    for(auto& obj : attachedObjects)
	obj->move(offsetX, offsetY);

    for(auto& child : children)
        child->moveRecursive(offsetX, offsetY);
}
//------------------------------------------------------------------------------
void SceneNode::rotateRecursive(const float angle)
{
    this->rotate(angle);
    
    for(auto& obj : attachedObjects)
	obj->rotate(angle);

    for(auto& child : children)
        child->rotateRecursive(angle);
}
//------------------------------------------------------------------------------    
void SceneNode::scaleRecursive(const Vector2f& factors)
{
    scaleRecursive(factors.x, factors.y);
}
//------------------------------------------------------------------------------
void SceneNode::scaleRecursive(float factorX, float factorY)
{
    this->scale(factorX, factorY);

    for(auto& obj : attachedObjects)
	obj->scale(factorX, factorY);

    for(auto& child : children)
        child->scaleRecursive(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::update()
{
    for(auto& child : children) {

        child->update();

        if(needSortAttachedObjects)
            sortObjectsByDrawOrder();

        if(needUpdateDrawableQueue && needUpdateNodesCommonPool) {
            if(auto pptr = parent.lock()) {
                pptr->setNeedUpdateDrawableQueue(true);
                setNeedUpdateDrawableQueue(false);

                pptr->setNeedUpdateNodesCommonPool(true);
                setNeedUpdateNodesCommonPool(false);
            }            
        } else {
            if(needUpdateDrawableQueue) {
                if(auto pptr = parent.lock()) {
                    pptr->setNeedUpdateDrawableQueue(true);
                    setNeedUpdateDrawableQueue(false);
                }
            } else if(needUpdateNodesCommonPool) {
                if(auto pptr = parent.lock()) {
                    pptr->setNeedUpdateNodesCommonPool(true);
                    setNeedUpdateNodesCommonPool(false);
                }
            }
        }
    }        
}
//------------------------------------------------------------------------------
void SceneNode::draw(RenderTarget& target, RenderStates states) const
{
    for(auto& child : children)
	child->draw(target, states);

    for(auto& obj : attachedObjects)
	obj->draw(target, states);
}
    
} // namespace ae
