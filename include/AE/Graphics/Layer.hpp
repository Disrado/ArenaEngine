#ifndef LAYER_HPP
#define LAYER_HPP

#include <AE/Graphics/RootSceneNode.hpp>
#include <AE/Graphics/Updatable.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Object.hpp>
#include <memory>
#include <vector>

namespace ae
{

class SFML_GRAPHICS_API Layer : public Drawable,
                                public Updatable
{
public:
    typedef std::vector<std::shared_ptr<Object>> ObjectQueue;
    typedef std::shared_ptr<RootSceneNode>       RootPtr;

private:
    RootPtr rootSceneNode;
    ObjectQueue drawableQueue;
    
    int drawOrder;
    std::string tag;

    void updateDrawableQueue();
    void buildDrawableQueue(std::shared_ptr<SceneNode> node);
    
public:
    Layer();
    Layer(int _drawOrder, const std::string& _tag);
    
    std::shared_ptr<RootSceneNode> getRootSceneNode();

    void setTag(const std::string& _tag);
    const std::string& getTag();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;

    void clear();

    void update();
    void draw(RenderTarget& target, RenderStates states) const override;

    bool operator< (const Layer& right)
    {
        return this->getDrawOrder() < right.getDrawOrder();
    }
};
    
} //namespace ae
    
#endif //LAYER_HPP
