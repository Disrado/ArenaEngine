#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Object.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <map>
#include <memory>

namespace ae
{

class Object;

class SFML_GRAPHICS_API SceneNode : public Drawable,
				    public Transformable,
				    public std::enable_shared_from_this<SceneNode>
{
public:
    typedef std::shared_ptr<Object>         ObjPtr;
    typedef std::shared_ptr<SceneNode>      SNodePtr;
    typedef std::weak_ptr<SceneNode>        ParentPtr;
    typedef std::map<std::string, SNodePtr> SceneNodeMap;
    typedef std::map<std::string, ObjPtr>   ObjectMap;

    struct Parameters
    {
	ae::Vector2f        position;
	const ae::Vector2f& scale;
	const ae::Vector2f& origin;
	float               rotation;
	bool                visible;

    public:
	/*
	 * Takes as a parameter the node
	 * whose fields will be used for initialization
	 */
	Parameters(SNodePtr snodeForInit) :
	    position(snodeForInit->getPosition()), 
	    scale(snodeForInit->getScale()),
	    origin(snodeForInit->getOrigin()),
	    rotation(snodeForInit->getRotation()),
	    visible(snodeForInit->getVisible())
	    { }
    };
    
private:
    std::string name;
    int drawOrder;
    bool visible;
    
    ParentPtr parent;
    SceneNodeMap children;
    ObjectMap attachedObjects;

protected:
    void setParent(SNodePtr _parent);
    void removeParent();
    const Parameters getParameters();
    /*
     * Return position, ratotion, origin, scale and visible this node
     * to child(paremeter)
     */
    void setParentParameters(const Parameters& param);
    
public:
    SceneNode(const std::string&  name,
	      int                 drawOrder = 0,
	      bool                visible = true,
	      const ae::Vector2f& position = ae::Vector2f(),
	      const ae::Vector2f& scale = ae::Vector2f(1, 1),
	      const ae::Vector2f& origin = ae::Vector2f(),
	      float               rotaion = 0.0);

    ~SceneNode();

    static SNodePtr create(const std::string&  name,
			   int                 drawOrder = 0,
			   bool                visible = true,
			   const ae::Vector2f& position = ae::Vector2f(),
			   const ae::Vector2f& scale = ae::Vector2f(1, 1),
			   const ae::Vector2f& origin = ae::Vector2f(),
			   float               rotation = 0.0);    
    

    /** Create new Node, add it as child to this node then return it */
    SNodePtr createChildSceneNode(const std::string& name, int drawOrder);

    void addChild(SNodePtr childNode);
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
     
    void setName(const std::string& _name) { name = _name; }
    const std::string& getName() const { return name; }

    const SNodePtr getParent() const;

    /** Return count of children of this node without grandchildren asf */
    int getChildrenCount() const { return children.size(); }

    /** Return count of children of this node with grandchildren asf */
    int getDescendantCount() const;
    
    bool isVisible();
    void setVisible(bool _visible);
    bool getVisible();
    void setVisibleRecursive(bool _visible);
     
    void attachObject(ObjPtr object, int drawOrder = 0);
    void detachObject(ObjPtr object);
    ObjPtr detachObject(const std::string& objectName);
    void detachAllObjects();
    ObjPtr getAttachedObject(const std::string& objectName);
    int numAttachedObjects();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const { return drawOrder; }
    
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
        return this->drawOrder < right.drawOrder;
    }
};
    
} //namespace ae

#endif //SCENENODE_HPP
