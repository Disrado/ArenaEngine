#include <AE/Graphics/Layer.hpp> 


namespace ae
{

Layer::Layer() :
    rootSceneNode(std::make_shared<RootSceneNode>())
{
    drawableQueue.reserve(1000);
}

Layer::Layer(int _drawOrder, const std::string& _name) :
    rootSceneNode(std::make_shared<RootSceneNode>()),
    drawOrder(_drawOrder),
    name(_name)
{
    drawableQueue.reserve(1000);
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

void Layer::buildNodesCommonPool(std::shared_ptr<SceneNode> node)
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

void Layer::buildDrawableQueue(std::shared_ptr<SceneNode> node)
{
    for(auto child : node->getDrawableChildren()) {
        if(child->getDrawableObjects().size()) {
            auto v = child->getDrawableObjects(); 
            drawableQueue.insert(end(drawableQueue), begin(v), end(v));
        }
        
        buildDrawableQueue(child);
    }
}
    
std::shared_ptr<RootSceneNode> Layer::getRootSceneNode()
{
    return rootSceneNode;
}

std::shared_ptr<SceneNode> Layer::getSceneNode(const std::string& name)
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
    
void Layer::setName(const std::string& newName)
{
    name = newName;
}
    
const std::string& Layer::getName()
{
    return name;
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
