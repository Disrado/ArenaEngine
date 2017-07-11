#ifndef SCENE_HPP
#define SCENE_HPP

#include <AE/Graphics/Layer.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <memory>
#include <set>

namespace ae
{

class Scene : public Drawable
{
private:
    std::set<std::shared_ptr<Layer>> layers;
    
public:
    Scene();
    virtual ~Scene() {}

    std::shared_ptr<Layer> createLayer(int drawOrder, const std::string& tag = "");
    void addLayer(std::shared_ptr<Layer> layer);
    void deleteLayer(const std::string& tag);
    void deleteLayer(std::shared_ptr<Layer> layer);
    void changeDrawOrder(const std::string& tag, int newDrawOrder);
    void changeDrawOrder(std::shared_ptr<Layer> layer, int newDrawOrder);
    
    virtual void draw(RenderTarget& target, RenderStates states) const override;
};

}

#endif //SCENE_HPP
