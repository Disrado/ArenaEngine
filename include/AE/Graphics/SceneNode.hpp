#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Object.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <set>
#include <memory>

namespace ae
{
            
class SceneNode : public Object,
                  public std::enable_shared_from_this<SceneNode> 
{
private:
    int drawOrder;
    std::string tag;
    std::shared_ptr<SceneNode> parent;
    std::set<std::shared_ptr<SceneNode>> children;
    std::shared_ptr<Object> attachedObject;
          
    void setParent(std::shared_ptr<SceneNode> _parent);
    void removeParent();
    
public:
    SceneNode();
    SceneNode(int _drawOrder = 0,
              const std::string& _tag = "",
              const ae::Vector2f& _position = ae::Vector2f(),
              const ae::Vector2f& _scale = ae::Vector2f(),
              const ae::Vector2f& _origin = ae::Vector2f(),
              float _angle = 0.0);

    virtual ~SceneNode() {}
    
    std::shared_ptr<SceneNode>
    createChildSceneNode(int _drawOrder = 0,
                         const std::string& _tag = "",
                         const ae::Vector2f& _position = ae::Vector2f(),
                         const ae::Vector2f& _scale = ae::Vector2f(),
                         const ae::Vector2f& _origin = ae::Vector2f(),
                         float _angle = 0.0);
    
    void addChild(std::shared_ptr<SceneNode>);

    void removeChild(std::shared_ptr<SceneNode> _child);
    void removeChild(const std::string& _tag);
    void removeChildren();
    
    void rebaseToNewParent(std::shared_ptr<SceneNode> newParent);
    void rebaseChildrenToNewParent(std::shared_ptr<SceneNode> newParent);
    
    void attachObject(std::shared_ptr<Object> object);
    std::shared_ptr<Object> detachObject();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;
    
    void setTag(const std::string& _tag);
    const std::string& getTag() const;

    void setOrigin(const Vector2f& origin);
    void setOrigin(float x, float y);
    
    void setScale(const Vector2f& factors);
    void setScale(float factorX, float factorY);

    void setPosition(const Vector2f& position);
    void setPosition(float x, float y);
    
    void setRotation(const float angle);
    
    void move(const Vector2f& offset);
    void move(float offsetX, float offsetY);
    
    void rotate(const float angle);
    
    void scale(const Vector2f& factor);
    void scale(float factorX, float factorY);
    
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

    void draw(RenderTarget& target, RenderStates states) const override;

    bool operator< (const SceneNode& right)
    {
        return this->getDrawOrder() < right.getDrawOrder();
    }
};
    
} //namespace ae

#endif //SCENENODE_HPP
