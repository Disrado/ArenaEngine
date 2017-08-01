#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Node.hpp>
#include <memory>


namespace ae
{

class Object : public Drawable,
	       public Updatable,
	       protected Transformable
{
private:
    std::string name;
    bool visible;
    bool attached;
    bool needUpdateFromParent;
    std::weak_ptr<ae::Node> parentNode;
    
public:
    Object(std::string _name, bool _visible = true);
	   
    virtual ~Object();

    void notifyAttached(std::shared_ptr<ae::Node> _parentSNode);
    void notifyDetached();

    bool isAttached();
    void setVisible(bool _visible);
    bool isVisible();

    void setNeedUpdateFromParent();
    
    void detachFromParent();
    
    const std::string& getName();
    void setName(const std::string&);

    std::shared_ptr<ae::Node> getParentSceneNode();

    virtual void update();
    virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};

} // namespace ae

#endif //OBJECT_HPP
