#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Updatable.hpp>
#include <AE/Graphics/SceneNode.hpp>
#include <memory>


namespace ae
{

class SceneNode;
    
class Object : public Drawable,
	       public Transformable
{
private:
    std::string name;
    bool visible;
    bool attached;
    std::weak_ptr<ae::SceneNode> parentNode;
    // This field need only wile object attached to SceneNode
    int drawOrder;
    
public:
    Object();
    Object(std::string _name, bool _visible = true);
	   
    virtual ~Object();

    void notifyAttached(std::shared_ptr<ae::SceneNode> _parentSNode);
    void notifyDetached();

    bool isAttached();
    void setVisible(bool _visible);
    bool isVisible();

    void setDrawOrder(int _drawOrder);
    
    void detachFromParent();
    
    const std::string& getName();
    void setName(const std::string&);

    std::shared_ptr<ae::SceneNode> getParentSceneNode();

    virtual void draw(RenderTarget& target, RenderStates states) const = 0;

    bool operator< (const Object& right)
    {
        return this->drawOrder < right.drawOrder;
    }
};

} // namespace ae

#endif //OBJECT_HPP
