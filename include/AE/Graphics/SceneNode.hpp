#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/TransformableObject.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <map>
#include <unordered_map>
#include <memory>

namespace ae
{

class SFML_GRAPHICS_API SceneNode : public Drawable, public Transformable,
                                    public std::enable_shared_from_this<SceneNode>
{
public:
    typedef std::shared_ptr<TransformableObject> TObjPtr;
    typedef std::shared_ptr<SceneNode> SNodePtr;
    typedef std::weak_ptr<SceneNode> ParentPtr;
    typedef std::map<std::string, SNodePtr> SceneNodeMap;
    typedef std::unordered_map<std::string, TObjPtr> TObjectMap;
    
private:
    int drawOrder;
    std::string name;
    bool visible;
    
    ParentPtr parent;
    SceneNodeMap children;
    TObjectMap attachedObjects;

    
    void setParent(SNodePtr _parent);
    void removeParent();
    
    /*
     * Set position, ratotion, origin, scale and visible this node
     * to child(paremeter)
     */
    void setParentNodeParameters(SNodePtr SNode);
    
public:
    SceneNode(const std::string&  name,
	      int                 drawOrder = 0,
	      bool                visible = true,
	      const ae::Vector2f& position = ae::Vector2f(),
	      const ae::Vector2f& scale = ae::Vector2f(1, 1),
	      const ae::Vector2f& origin = ae::Vector2f(),
	      float               angle = 0.0);

    ~SceneNode();


    static SNodePtr create(const std::string& name,
			   int drawOrder = 0,
			   bool visible = true,
			   const ae::Vector2f& position = ae::Vector2f(),
			   const ae::Vector2f& scale = ae::Vector2f(1, 1),
			   const ae::Vector2f& origin = ae::Vector2f(),
			   float angle = 0.0);    
    
    /** Create new Node, add it as child to this node then return it */
    SNodePtr createChildSceneNode(const std::string& name, int drawOrder);

    bool isVisible();
    void setVisible(bool _visible);
    void setVisibleRecursive(bool _visible);
    
    void addChild(SNodePtr);
    SNodePtr getChild(const std::string& _name);

    /* 
     * Remove child from whis node
     * But if pointer to child was saved in other place
     * children of child will not be deleted from children
     */    
    void removeChild(SNodePtr childToRemove);
    void removeChild(const std::string& _name);
    void removeChildren();
    
    /*
     * Remove child from whis node
     * If pointer to child was saved in other place
     * children of child will be deleted from child children
     * Also destroy* methods detach all objects from children
     */
    void destroyChild(SNodePtr childToRemove);
    void destroyChild(const std::string& _name);
    void destroyChildren();

    void rebaseToNewParent(SNodePtr newParent);
    void rebaseChildrenToNewParent(SNodePtr newParent);
     
    void attachObject(TObjPtr object);
    void detachObject(TObjPtr object);
    TObjPtr detachObject(const std::string& objectName);
    void detachAllObjects();
    TObjPtr getAttachedObject(const std::string& objectName);
    int numAttachedObjects();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const { return drawOrder; }
    
    void setName(const std::string& _name) { name = _name; }
    const std::string& getName() const { return name; }

    const SNodePtr getParent() const;

    /** Return count of children of this node without grandchildren asf */
    int getChildrenCount() const { return children.size(); }

    /** Return count of children of this node with grandchildren asf */
    int getDescendantCount() const;
    
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
};
    
} //namespace ae

#endif //SCENENODE_HPP
