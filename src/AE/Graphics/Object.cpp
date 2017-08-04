#include <AE/Graphics/Object.hpp>

namespace ae
{

Object::Object() :
    name    (),
    visible (true),
    attached(false),
    drawOrder(0)
{ }
    
Object::Object(std::string _name, bool _visible) :
    name    (_name),
    visible (_visible),
    attached(false),
    drawOrder(0)
{ }

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

void Object::setDrawOrder(int _drawOrder)
{
    drawOrder = drawOrder;

    if(auto pnode = parentNode.lock()) {
	pnode->setNeedUpdateObjectQueue();
    }
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

void Object::setName(const std::string& _name)
{
    name = _name;
}

std::shared_ptr<ae::SceneNode> Object::getParentSceneNode()
{
    if(attached)
	return parentNode.lock();
}

} // namespace ae
