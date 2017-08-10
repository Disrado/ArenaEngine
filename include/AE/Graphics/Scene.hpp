#ifndef SCENE_HPP
#define SCENE_HPP

#include <AE/Graphics/Layer.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <memory>
#include <set>

namespace ae
{

class SFML_GRAPHICS_API Scene : public Drawable
{
protected:
    std::set<std::shared_ptr<Layer>> layers;
    
public:
    Scene();
    virtual ~Scene() {}

    std::shared_ptr<Layer> createLayer(int drawOrder, const std::string& name = "");
    void addLayer(std::shared_ptr<Layer> layer);
    void deleteLayer(const std::string& name);
    void deleteLayer(std::shared_ptr<Layer> layer);
    void changeLayerDrawOrder(const std::string& name, int newDrawOrder);
    void changeLayerDrawOrder(std::shared_ptr<Layer> layer, int newDrawOrder);

    void update();
    virtual void draw(RenderTarget& target, RenderStates states) const override;
};

}

#endif //SCENE_HPP
