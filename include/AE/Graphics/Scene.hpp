#ifndef SCENE_HPP
#define SCENE_HPP

#include <AE/Graphics/Layer.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Updatable.hpp>
#include <memory>
#include <vector>

namespace ae
{

class SFML_GRAPHICS_API Scene : public Drawable,
				public Updatable
{
public:
    typedef std::vector<Layer::LayerPtr>           LayersQueue;
    typedef std::vector<Layer::LayerPtr>::iterator LayerItr;
    typedef std::shared_ptr<Scene>                 ScenePtr;
    
private:
    std::string name;
    LayersQueue layers;

    
    LayerItr getLayerItr(const std::string& name);
    LayerItr getLayerItr(const Layer::LayerPtr layer);
    void sortLayersByDrawOrder();

public:
    Scene(const std::string& _name);
    virtual ~Scene() {}

    static ScenePtr create(const std::string& name);

    void setName(const std::string& newName) { name = newName; }
    const std::string& getName() { return name; }
    
    Layer::LayerPtr createLayer(const std::string& name, int drawOrder = 0);
    void addLayer(Layer::LayerPtr layer);
    void deleteLayer(const std::string& name);
    void deleteLayer(Layer::LayerPtr layer);
    void changeLayerDrawOrder(const std::string& name, int newDrawOrder);
    void changeLayerDrawOrder(Layer::LayerPtr layer, int newDrawOrder);
    int getNumberOfLayers() { return layers.size(); }

    void update() override;
    virtual void draw(RenderTarget& target, RenderStates states) const override;
};

}

#endif //SCENE_HPP
