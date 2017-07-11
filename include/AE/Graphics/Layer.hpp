#ifndef LAYER_HPP
#define LAYER_HPP

#include <AE/Graphics/SceneNode.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <memory>

namespace ae
{

class Layer : public Drawable
{
private:
    std::shared_ptr<SceneNode> rootSceneNode;
    int drawOrder;
    
public:
    Layer();
    Layer(int _drawOrder);

    bool operator< (const Layer& right)
    {
        return this->getDrawOrder() < right.getDrawOrder();
    }
    
    std::shared_ptr<SceneNode> getRootSceneNode();
    
    int getDrawOrder() const;

    virtual void draw(RenderTarget& target, RenderStates states) const override;
};
    
} //namespace ae
    
#endif //LAYER_HPP
