#include <AE/Graphics/Layer.hpp> 


namespace ae
{

Layer::Layer(const std::string& _name, int _drawOrder) :
    rootSceneNode(std::make_shared<RootSceneNode>()),
    drawOrder(_drawOrder),
    name(_name),
    onScene(false)
    
{ }

Layer::LayerPtr Layer::create(const std::string& name, int drawOrder)
{
    return std::make_shared<Layer>(name, drawOrder);
}
    
void Layer::updateNodesCommonPool()
{
    nodesCommonPool.clear();
    buildNodesCommonPool(rootSceneNode);
    
    nodesCommonPool.insert(std::make_pair(rootSceneNode->getName(), rootSceneNode));

    for(auto child : rootSceneNode->getChildren())
        nodesCommonPool.insert(std::make_pair(child->getName(), child));
            
    rootSceneNode->setNeedUpdateNodesCommonPool(false);
}

void Layer::buildNodesCommonPool(SceneNode::SNodePtr node)
{
    for(auto child : node->getChildren()) {
        auto children = child->getChildren();

        for(auto itr : children)
            nodesCommonPool.insert(std::make_pair(itr->getName(), itr));

        buildNodesCommonPool(child);
    }
}
    
void Layer::updateDrawableQueue()
{
    drawableQueue.clear();

    drawableQueue.insert(end(drawableQueue),
                         begin(rootSceneNode->getDrawableObjects()),
                         end(rootSceneNode->getDrawableObjects()));
    
    buildDrawableQueue(rootSceneNode);
    rootSceneNode->setNeedUpdateDrawableQueue(false);
}

void Layer::buildDrawableQueue(SceneNode::SNodePtr node)
{
    for(auto child : node->getDrawableChildren()) {
        if(child->getDrawableObjects().size()) {
            auto v = child->getDrawableObjects(); 
            drawableQueue.insert(end(drawableQueue), begin(v), end(v));
        }
        
        buildDrawableQueue(child);
    }
}
    
RootSceneNode::RootSNodePtr Layer::getRootSceneNode()
{
    return rootSceneNode;
}

SceneNode::SNodePtr Layer::getSceneNode(const std::string& name)
{
    if(nodesCommonPool.empty())
	return nullptr;
    
    auto itr = nodesCommonPool.find(name);
    
    if(itr != nodesCommonPool.end()) {
	return itr->second;
    } else {
	Logger::getInstance().write("WARNING",
                                    "Child node named "
                                    + name + "does not exist in nodesCommonPool");
	return nullptr;
    }
}

int Layer::getNumberAllNodes()
{
    return nodesCommonPool.size();
}
 
void Layer::clear()
{
    rootSceneNode->destroyChildren();
    rootSceneNode->detachAllObjects();
}

void Layer::update()
{
    rootSceneNode->update();
    
    if(rootSceneNode->isNeedUpdateNodesCommonPool()) {
        updateNodesCommonPool();
    }

    if(rootSceneNode->isNeedUpdateDrawableQueue()) {
        updateDrawableQueue();
    }
}
    
void Layer::draw(RenderTarget& target, RenderStates states) const
{
    //rootSceneNode->draw(target, states);

    for(auto itr : drawableQueue) {
        itr->draw(target, states);
    }
}

} // namespace ae
