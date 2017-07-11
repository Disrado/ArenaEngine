#include <AE/Graphics/Layer.hpp> 

namespace ae
{

Layer::Layer() :
    rootSceneNode(std::make_shared<SceneNode>(0)),
    drawOrder(0)
{ }

Layer::Layer(int _drawOrder, const std::string& _tag) :
    rootSceneNode(std::make_shared<SceneNode>(0)),
    drawOrder(_drawOrder),
    tag(_tag)
{ }

void Layer::setTag(const std::string& _tag)
{
    tag = _tag;
}
    
const std::string& Layer::getTag() const
{
    return tag;
}
    
int Layer::getDrawOrder() const
{
    return drawOrder;
}

void Layer::setDrawOrder(int _drawOrder)
{
    drawOrder = _drawOrder;
}

void Layer::clear()
{
    rootSceneNode->removeChildren();
    rootSceneNode->detachObject();
}
    
void Layer::draw(RenderTarget& target, RenderStates states) const
{
    rootSceneNode->draw(target, states);
}

} // namespace ae
