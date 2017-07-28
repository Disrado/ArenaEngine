#include <AE/Graphics/Object.hpp>

namespace ae
{

Object::Object(std::string _name, bool _visible) :
    name(_name),
    visible(_visible),
    attached(false)
{
    
}

Object::~Object()
{
    if(auto pnode = parentNode.lock()) {
	pnode->detachObject(name);
    }
}

void Object::notifyAttached(std::shared_ptr<ae::SceneNode> _parentSNode)
{
    parentNode = _parentSNode;
    attached = true;
}

void Object::notifyDetached()
{
    attached = false;
}
    
bool Object::isAttached()
{
    return attached;
}

void Object::setVisible(bool _visible)
{
    visible = _visible;
}

bool Object::isVisible()
{
    return visible;
}

void Object::detachFromParent()
{
    if(auto pnode = parentNode.lock()) {
	pnode->detachObject(name);
    }
}
    
const std::string& Object::getName()
{
    return name;
}

std::shared_ptr<ae::SceneNode> Object::getParentSceneNode()
{
    if(attached)
	return parentNode.lock();
}

} // namespace ae
