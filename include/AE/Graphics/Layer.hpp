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
    std::string tag;
    int drawOrder;
    
public:
    Layer();
    Layer(int _drawOrder, const std::string& _tag);
    virtual ~Layer();
    
    std::shared_ptr<SceneNode> getRootSceneNode();

    void setTag(const std::string& _tag);
    const std::string& getTag() const;
    
    int getDrawOrder() const;
    void setDrawOrder(int _drawOrder);

    void clear();
    
    virtual void draw(RenderTarget& target, RenderStates states) const override;

    bool operator< (const Layer& right)
    {
        return this->getDrawOrder() < right.getDrawOrder();
    }
};
    
} //namespace ae
    
#endif //LAYER_HPP
