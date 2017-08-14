#include <AE/Graphics/Scene.hpp>

namespace ae
{
    
Scene::Scene(const std::string& _name) :
    name(_name)
{ }

Scene::ScenePtr Scene::create(const std::string& name)
{
    return std::make_shared<Scene>(name);
}
    
Scene::LayerItr Scene::getLayerItr(const std::string& name)
{
    return std::find_if(begin(layers), end(layers),
                        [&name](Layer::LayerPtr lhs)
                        {
                            return lhs->getName() == name;
                        });   
}

Scene::LayerItr Scene::getLayerItr(const Layer::LayerPtr layer)
{
    return std::find(begin(layers), end(layers), layer);
}

void Scene::sortLayersByDrawOrder()
{
    std::sort(begin(layers), end(layers),
              [](const Layer::LayerPtr lhs, const Layer::LayerPtr rhs)
              {
                  return lhs->getDrawOrder() < rhs->getDrawOrder();
              });	
}
    
Layer::LayerPtr Scene::createLayer(const std::string& name, int drawOrder)
{
    auto newLayer = std::make_shared<Layer>(name, drawOrder);
    layers.push_back(newLayer);
    newLayer->notifyAddedOnScene();
    sortLayersByDrawOrder();
    
    return newLayer;
}
    
void Scene::addLayer(Layer::LayerPtr layer)
{
    if(!layer)
	return;

    if(!layer->isOnScene()) {
	layers.push_back(layer);
	layer->notifyAddedOnScene();
	sortLayersByDrawOrder();
    }
}
    
void Scene::deleteLayer(const std::string& name)
{
    auto itr = getLayerItr(name);
    
    if(itr != layers.end()) {
	layers.erase(itr);
	(*itr)->notifyRemovedFromScene();
    }
}

void Scene::deleteLayer(Layer::LayerPtr layer)
{
    if(!layer)
	return;
    
    if(!layer->isOnScene()) {
	auto itr = getLayerItr(layer);
    
	if(itr != layers.end()) {
	    layers.erase(itr);
	    layer->notifyRemovedFromScene();
	}
    }
}
         
void Scene::changeLayerDrawOrder(const std::string& name, int newDrawOrder)
{
    auto itr = getLayerItr(name);

    if(itr != layers.end()) {
        (*itr)->setDrawOrder(newDrawOrder);
	sortLayersByDrawOrder();
    }
}
    
void Scene::changeLayerDrawOrder(Layer::LayerPtr layer, int newDrawOrder)
{
    if(!layer)
	return;

    auto itr = getLayerItr(name);

    if(itr != layers.end()) {
        layer->setDrawOrder(newDrawOrder);
	sortLayersByDrawOrder();
    }
}

void Scene::update()
{
    for(auto layer : layers)
        layer->update();
}
    
void Scene::draw(RenderTarget& target, RenderStates states) const
{
    for(auto layer : layers)
        layer->draw(target, states);
}

} //namespace ae
