#include <AE/Graphics/Scene.hpp>

namespace ae
{
    
Scene::Scene() :
    layers(std::set<std::shared_ptr<Layer>>())
{ }

std::shared_ptr<Layer> Scene::createLayer(int drawOrder, const std::string& tag)
{
    auto newLayer = std::make_shared<Layer>(drawOrder, tag);
    layers.insert(newLayer);

    return newLayer;
}
    
void Scene::addLayer(std::shared_ptr<Layer> layer)
{
    if(layer)
        layers.insert(layer);
}
    
void Scene::deleteLayer(const std::string& name)
{
    auto itr = std::find_if(layers.begin(), layers.end(),
                            [&name](const std::shared_ptr<Layer>& item) -> bool
                            {
                                return item->getName() == name;
                            });
    
    if(itr != layers.end()) {
        layers.erase(itr);        
    }
}

void Scene::deleteLayer(std::shared_ptr<Layer> layer)
{
    if(layer) {
        auto itr = layers.find(layer);
        
        if(itr != layers.end()) {
            layers.erase(itr);        
        }
    }    
}
         
void Scene::changeLayerDrawOrder(const std::string& name, int newDrawOrder)
{
    auto itr = std::find_if(layers.begin(), layers.end(),
                            [&name](const std::shared_ptr<Layer>& item) -> bool
                            {
                                return item->getName() == name;
                            });

    if(itr != layers.end()) {
        auto layer = *itr;
        layers.erase(itr);
        layer->setDrawOrder(newDrawOrder);
        layers.insert(layer);
    }
}
    
void Scene::changeLayerDrawOrder(std::shared_ptr<Layer> layer, int newDrawOrder)
{
    auto itr = layers.find(layer);
        
    if(itr != layers.end()) {
        auto layer = *itr;
        layers.erase(itr);
        layer->setDrawOrder(newDrawOrder);
        layers.insert(layer);
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
