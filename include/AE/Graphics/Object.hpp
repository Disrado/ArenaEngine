#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Updatable.hpp>
#include <AE/Graphics/SceneNode.hpp>
#include <AE/Graphics/Transformable.hpp>
#include <memory>


namespace ae
{

class SceneNode;
    
class SFML_GRAPHICS_API Object : public Drawable,
	       public Transformable
{
private:
    std::string name;
    int drawOrder;
    bool visible;
    bool attached;
    std::weak_ptr<ae::SceneNode> parentNode;
    
public:
    Object();
    Object(std::string _name, bool _visible = true);
	   
    virtual ~Object();

    void notifyAttached(std::shared_ptr<ae::SceneNode> _parentSNode);
    void notifyDetached();

    bool isAttached();
    void setVisible(bool _visible);
    bool isVisible();

    void setDrawOrder(int newDrawOrder);
    int getDrawOrder();
    
    void detachFromParent();
    
    const std::string& getName();
    void setName(const std::string&);

    std::shared_ptr<ae::SceneNode> getParentSceneNode();

    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};

} // namespace ae

#endif //OBJECT_HPP
