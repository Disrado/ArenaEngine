#include <AE/Graphics/Node.hpp>
#include <AE/System/Logger.hpp>


namespace ae
{

namespace
{
    typedef ae::SceneNode::SNodePtr SNodePtr;
    typedef ae::SceneNode::TObjPtr TObjPtr;
}
    
//------------------------------------------------------------------------------    
Node::Node(const std::string&  _name,
	   const ae::Vector2f& position,
	   const ae::Vector2f& scale,
	   const ae::Vector2f& origin,
	   float               rotation)
    : name(_name)
{
    Transformable::setPosition(position);
    Transformable::setScale(scale);
    Transformable::setOrigin(origin);
    Transformable::setRotation(rotation);
}
//------------------------------------------------------------------------------
Node::~Node()
{
    if(auto prnt = parent.lock()) {
	prnt->removeChild(shared_from_this());
    }
}
//------------------------------------------------------------------------------    
NodePtr Node::createChild(const std::string& name,
			  int  drawOrder)
{
    auto newChild = std::make_shared<Node>(name, drawOrder);

    newChild->setParentNodeTransform(this->getFullTransform());
    
    this->addChild(newChild);
    
    return newChild;
}
//------------------------------------------------------------------------------
NodePtr Node::create(const std::string&  name,
		     const ae::Vector2f& position,
		     const ae::Vector2f& scale,
		     const ae::Vector2f& origin,
		     float               rotation)
{
    return std::make_shared<Node>(name, position, scale, origin, rotation);
}
//------------------------------------------------------------------------------
void Node::setParent(NodePtr newParent)
{
    parent = newParent;
}
//------------------------------------------------------------------------------
void Node::removeParent()
{
    parent.reset();
}
//------------------------------------------------------------------------------
void Node::setParentNodeTransform(NodeFullTransorm parentTransform)
{
    this->setPosition(parentTransform.position);
    this->setScale(parentTransform.scale);
    this->setOrigin(parentTransform.origin);
    this->setRotation(parentTransform.rotation);
}
//------------------------------------------------------------------------------ 
bool Node::getFulltransform()
{
    return ae::NodeFullTransform(this->getPosition(),
				 this->getScale(),
				 this->getOrigin(),
				 this-getRotation);
}
//------------------------------------------------------------------------------    
void Node::addChild(NodePtr child)
{
    if(child) {
	SNodePtr parent = child->parent.lock();
	if(!parent) {

	    auto res = children.insert(
		std::make_pair(child->getName(), child));

	    if(res.second) {
		child->setParent(shared_from_this());
		child->needUpdateFromParent = true;
		child->needUpdateChildren = true;
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
NodePtr Node::getChild(const std::string& _name)
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
void Node::removeChild(NodePtr childToRemove)
{
    if(children.empty())
	return;

    auto itr = children.find(childToRemove->name);

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
void Node::removeChild(const std::string& _name)
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
void Node::removeChildren()
{
    for(auto& child : children)
        child.second->removeParent();
    
    children.clear();
}
//------------------------------------------------------------------------------
void Node::rebaseToNewParent(NodePtr newParent)
{
    if(newParent) {
	if(auto ptr = parent.lock())
	    ptr->removeChild(shared_from_this());
	
        newParent->addChild(shared_from_this());
    }
}
//------------------------------------------------------------------------------    
void Node::rebaseChildrenToNewParent(NodePtr newParent)
{
    if(newParent) {
        for(auto& child : children)
            newParent->addChild(child.second);

        children.clear();
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
    needUpdateChildren = true;
}
//------------------------------------------------------------------------------    
void SceneNode::setOrigin(float x, float y)
{
    this->Transformable::setOrigin(x, y);
    needUpdateChildren = true;
}    
//------------------------------------------------------------------------------
void SceneNode::setScale(const Vector2f& factors)
{
    this->Transformable::setScale(factors);
    needUpdateChildren = true;
}
//------------------------------------------------------------------------------
void SceneNode::setScale(float factorX, float factorY)
{
    this->Transformable::setScale(factorX, factorY);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setOrigin(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::setPosition(const Vector2f& position)
{
    this->Transformable::setPosition(position);
    needUpdateChildren = true;
	for(auto& obj : attachedObjects)
        obj.second->setPosition(position);
}
//------------------------------------------------------------------------------
void SceneNode::setPosition(float x, float y)
{
    this->Transformable::setPosition(x, y);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setPosition(x, y);
}    
 //------------------------------------------------------------------------------
void SceneNode::setRotation(const float angle)
{
    this->Transformable::setRotation(angle);
    needUpdateChildren = true;
	for(auto& obj : attachedObjects)
        obj.second->setRotation(angle);
}
//------------------------------------------------------------------------------        
void SceneNode::move(const Vector2f& offset)
{
    this->Transformable::move(offset);
    needUpdateChildren = true;
	for(auto& obj : attachedObjects)
        obj.second->move (offset);
}
//------------------------------------------------------------------------------
void SceneNode::move(float offsetX, float offsetY)
{
    this->Transformable::move(offsetX, offsetY);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->move(offsetX, offsetY);
}
//------------------------------------------------------------------------------    
void SceneNode::rotate(const float angle)
{
    this->Transformable::rotate(angle);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
        obj.second->rotate(angle);
}
//------------------------------------------------------------------------------    
void SceneNode::scale(const Vector2f& factor)
{
    this->Transformable::scale(factor);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
        obj.second->scale(factor);
}
//------------------------------------------------------------------------------
void SceneNode::scale(float factorX, float factorY)
{
    this->Transformable::scale(factorX, factorY);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->scale(factorX, factorY);
}
//------------------------------------------------------------------------------    
void SceneNode::setOriginRecursive(const ae::Vector2f& origin)
{
    this->setOrigin(origin);
    needUpdateChildren = true;
    
    for(auto& obj : attachedObjects)
	obj.second->setOrigin(origin);

    for(auto& child : children)
        child.second->setOriginRecursive(origin);
}
//------------------------------------------------------------------------------    
void SceneNode::setOriginRecursive(float x, float y)
{
    this->setOrigin(x, y);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setOrigin(x, y);

    for(auto& child : children)
        child.second->setOriginRecursive(x, y);
}
//------------------------------------------------------------------------------    
void SceneNode::setScaleRecursive(const ae::Vector2f& factors)
{
    this->setScale(factors);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setScale(factors);

    for(auto& child : children)
        child.second->setScaleRecursive(factors);
}
//------------------------------------------------------------------------------    
void SceneNode::setScaleRecursive(float factorX, float factorY)
{
    this->setScale(factorX, factorY);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setScale(factorX, factorY);
    
    for(auto& child : children)
        child.second->setScaleRecursive(factorX, factorY);
}
//------------------------------------------------------------------------------
void SceneNode::setPositionRecursive(const ae::Vector2f& position)
{
    this->setPosition(position);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setPosition(position);

    for(auto& child : children)
        child.second->setPositionRecursive(position);
}
//------------------------------------------------------------------------------
void SceneNode::setPositionRecursive(float x, float y)
{
    this->setPosition(x, y);
    needUpdateChildren = true;
    for(auto& obj : attachedObjects)
	obj.second->setPosition(x, y);

    for(auto& child : children)
        child.second->setPositionRecursive(x, y);
}
//------------------------------------------------------------------------------
void SceneNode::setRotationRecursive(const float angle)
{
    this->setRotation(angle);
    needUpdateChildren = true;
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
