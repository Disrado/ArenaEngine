#include <AE/Graphics/Layer.hpp> 

namespace ae
{

Layer::Layer() :
    rootSceneNode(std::make_shared<RootSceneNode>())
{ }

Layer::Layer(int _drawOrder, const std::string& _tag) :
    rootSceneNode(std::make_shared<RootSceneNode>()),
    drawOrder(_drawOrder),
    tag(_tag)
{ }

std::shared_ptr<RootSceneNode> Layer::getRootSceneNode()
{
    return rootSceneNode;
}
    
void Layer::setTag(const std::string& _tag)
{
    tag = _tag;
}
    
const std::string& Layer::getTag()
{
    return tag;
}
    
void Layer::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
}

    int Layer::getDrawOrder() const
{
    return drawOrder;
}

void Layer::clear()
{
    rootSceneNode->removeChildren();
    rootSceneNode->detachAllObjects();
}
    
void Layer::draw(RenderTarget& target, RenderStates states) const
{
    rootSceneNode->draw(target, states);
}

} // namespace ae
