#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Object.hpp>
#include <AE/Graphics/Transformable.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <map>
#include <memory>

namespace ae
{
    
class SceneNode : public Transformable,
                  public std::enable_shared_from_this<SceneNode> 
{    
private:
    typedef std::shared_ptr<SceneNode> SceneNode_sptr;

    int drawOrder;
    std::string tag;
    SceneNode_sptr parent;
    std::map<std::string, SceneNode_sptr> children;
    std::shared_ptr<Object> attachedObject;

    void sortChildrenByDrawOrder();
        
protected:
    SceneNode();
    SceneNode(const std::string& _tag, int _drawOrder);
    virtual ~SceneNode() {}
    void removeParent();
    virtual void draw(ae::RenderWindow& renderWindow);
    
public:
    void setParent(SceneNode_sptr _parent);
    SceneNode_sptr getParent() const;
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;
    
    void setTag(const std::string& _tag);
    const std::string& getTag() const;

    void setChildren(const std::map<std::string, SceneNode_sptr>& children);
    const std::map<std::string, SceneNode_sptr>& getChildren() const;
    
    void removeChild(const std::string& _tag);
    void removeChild(SceneNode_sptr _child);
    
    void removeChildren();
    void addChild(SceneNode_sptr);
    void rebaseToNewParent(SceneNode_sptr newParent);
    
    void attachObject(std::shared_ptr<Object> object);
    void attachObjectWithTransform(std::shared_ptr<Object> object);

    std::shared_ptr<Object> detachObject();
};
	
} //namespace ae


#endif //SCENENODE_HPP
