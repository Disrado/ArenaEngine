#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Object.hpp>
#include <AE/Graphics/Transformable.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <set>
#include <memory>

namespace ae
{
    
class SceneNode : public Transformable,
                  public std::enable_shared_from_this<SceneNode> 
{    
private:
    int drawOrder;
    std::string tag;
    std::shared_ptr<SceneNode> parent;
    std::set<std::shared_ptr<SceneNode>> children;
    std::shared_ptr<Object> attachedObject;
    
protected:
    SceneNode();
    SceneNode(int _drawOrder, const std::string& _tag);
    virtual ~SceneNode() {}

    void setParent(std::shared_ptr<SceneNode>  _parent);
    void removeParent();
    
public:
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;
    
    void setTag(const std::string& _tag);
    const std::string& getTag() const;

    void setChildren(const std::set<std::shared_ptr<SceneNode>>& children);
    const std::set<std::shared_ptr<SceneNode>>& getChildren() const;

    void removeChildren();

    void addChild(std::shared_ptr<SceneNode>);

    void removeChild(std::shared_ptr<SceneNode> _child);
    void removeChild(const std::string& _tag);
    
    void rebaseToNewParent(std::shared_ptr<SceneNode> newParent);
    
    void attachObject(std::shared_ptr<Object> object);
    std::shared_ptr<Object> detachObject();

    void setOriginRecursive(const Vector2f& origin);
    void setOriginRecursive(float x, float y);
    
    void setScaleRecursive(const Vector2f& factors);
    void setScaleRecursive(float factorX, float factorY);

    void setPositionRecursive(const Vector2f& position);
    void setPositionRecursive(float x, float y);
    
    void setRotationRecursive(const float angle);
    
    void moveRecursive(const Vector2f& offset);
    void moveRecursive(float offsetX, float offsetY);
    
    void rotateRecursive(const float angle);
    
    void scaleRecursive(const Vector2f& factor);
    void scaleRecursive(float factorX, float factorY);
    
    virtual void draw(ae::RenderWindow& renderWindow);
};
	
} //namespace ae


#endif //SCENENODE_HPP
