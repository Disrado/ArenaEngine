#include <AE/Graphics/Layer.hpp> 
#include <iostream>


namespace ae
{

Layer::Layer() :
    rootSceneNode(std::make_shared<RootSceneNode>())
{
    drawableQueue.reserve(1000);
}

Layer::Layer(int _drawOrder, const std::string& _tag) :
    rootSceneNode(std::make_shared<RootSceneNode>()),
    drawOrder(_drawOrder),
    tag(_tag)
{
    drawableQueue.reserve(1000);
}

void Layer::updateDrawableQueue()
{
    drawableQueue.clear();

    drawableQueue.insert(drawableQueue.end(),
                         rootSceneNode->getDrawableObjects().begin(),
                         rootSceneNode->getDrawableObjects().end());
    
    buildDrawableQueue(rootSceneNode);
}

void Layer::buildDrawableQueue(std::shared_ptr<SceneNode> node)
{
    // for(auto child : node->getDrawableChildren()) {
    //     if(child->getDrawableObjects().size()) {
    //         drawableQueue.insert(drawableQueue.end(),
    //                              child->getDrawableObjects().begin(),
    //                              child->getDrawableObjects().end());
    //         buildDrawableQueue(child);
    //     }
    // }
}
    
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
    rootSceneNode->destroyChildren();
    rootSceneNode->detachAllObjects();
}

void Layer::update()
{
    if(rootSceneNode->isNeedUpdateDrawableQueue()) {
        updateDrawableQueue();
    }
}
    
void Layer::draw(RenderTarget& target, RenderStates states) const
{
    //    rootSceneNode->draw(target, states);

    for(auto itr : drawableQueue) {
        itr->draw(target, states);
    }
}

} // namespace ae
