#ifndef LAYER_HPP
#define LAYER_HPP

#include <AE/Graphics/SceneNode.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <memory>

namespace ae
{

class SFML_GRAPHICS_API Layer : public Drawable
{ 
private:
    std::shared_ptr<SceneNode> rootSceneNode;
    int drawOrder;
    std::string tag;
    
public:
    Layer();
    Layer(int _drawOrder, const std::string& _tag);
    
    std::shared_ptr<SceneNode> getRootSceneNode();

    void setTag(const std::string& _tag);
    const std::string& getTag();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;

    void clear();
    
    void draw(RenderTarget& target, RenderStates states) const override;

    bool operator< (const Layer& right)
    {
        return this->getDrawOrder() < right.getDrawOrder();
    }
};
    
} //namespace ae
    
#endif //LAYER_HPP
