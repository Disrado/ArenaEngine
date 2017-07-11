#include <AE/Graphics/Layer.hpp> 

namespace ae
{

Layer::Layer() :
    rootSceneNode(std::make_shared<SceneNode>(0)),
    drawOrder(0)
{ }

Layer::Layer(int _drawOrder) :
    rootSceneNode(std::make_shared<SceneNode>(0)),
    drawOrder(_drawOrder)
{ }

int Layer::getDrawOrder() const
{
    return drawOrder;
}

void Layer::draw(RenderTarget& target, RenderStates states) const
{
    rootSceneNode->draw(target, states);
}

} // namespace ae
