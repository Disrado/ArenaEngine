#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Object.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <AE/Graphics/Updatable.hpp>
#include <map>
#include <memory>

namespace ae
{

class Object;

class SFML_GRAPHICS_API SceneNode : public Drawable,
				    public Transformable,
				    public Updatable,
				    public std::enable_shared_from_this<SceneNode>
{
public:
    typedef std::shared_ptr<Object>         ObjPtr;
    typedef std::shared_ptr<SceneNode>      SNodePtr;
    typedef std::weak_ptr<SceneNode>        ParentPtr;
    typedef std::map<std::string, SNodePtr> SceneNodeMap;
    typedef std::map<std::string, ObjPtr>   ObjectMap;
    typedef std::vector<SNodePtr>           Queue;

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
    Queue objectsQueue;
    Queue childrenQueue;
    
    bool needUpdateChildrenQueue;
    bool needUpdateObjectQueue;

protected:
    void setParent(SNodePtr _parent);
    void removeParent();
    const Parameters getParameters();
    /*
     * Return position, ratotion, origin, scale and visible this node
     * to child(paremeter)
     */
    void setParentParameters(const Parameters& param);
    void setNeedUpdateChildrenQueue();
    void setNeedUpdateObjectQueue();
    void updateChildrenQueue();
    void updateObjectQueue();

    friend void Object::setDrawOrder(int);
    
public:
    SceneNode(const std::string&  name,
	      int                 drawOrder = 0,
	      bool                visible = true,
	      const ae::Vector2f& position = ae::Vector2f(),
	      const ae::Vector2f& scale = ae::Vector2f(1, 1),
	      const ae::Vector2f& origin = ae::Vector2f(),
	      float               rotaion = 0.0);

    virtual ~SceneNode();

    static SNodePtr create(const std::string&  name,
			   int                 drawOrder = 0,
			   bool                visible = true,
			   const ae::Vector2f& position = ae::Vector2f(),
			   const ae::Vector2f& scale = ae::Vector2f(1, 1),
			   const ae::Vector2f& origin = ae::Vector2f(),
			   float               rotation = 0.0);    
    

    /** Create new Node, add it as child to this node then return it */
    virtual SNodePtr createChildSceneNode(const std::string& name, int drawOrder);

    virtual void addChild(SNodePtr childNode);
    virtual SNodePtr getChild(const std::string& _name);

    /* 
     * Remove child from whis node
     * But if pointer to child was saved in other place
     * children of child will not be deleted from children
     */    
    virtual void removeChild(SNodePtr childToRemove);
    virtual void removeChild(const std::string& _name);
    virtual void removeChildren();

    /*
     * Remove child from whis node
     * If pointer to child was saved in other place
     * children of child will be deleted from child children
     * Also destroy* methods detach all objects from children
     */
    virtual void destroyChild(SNodePtr childToRemove);
    virtual void destroyChild(const std::string& _name);
    virtual void destroyChildren();
    
    virtual void rebaseToNewParent(SNodePtr newParent);
    virtual void rebaseChildrenToNewParent(SNodePtr newParent);
     
    virtual void setName(const std::string& _name) { name = _name; }
    virtual const std::string& getName() const { return name; }

    virtual const SNodePtr getParent() const;

    /** Return count of children of this node without grandchildren asf */
    virtual int getChildrenCount() const { return children.size(); }

    /** Return count of children of this node with grandchildren asf */
    virtual int getDescendantCount() const;
    
    virtual bool isVisible();
    virtual void setVisible(bool _visible);
    virtual bool getVisible();
    virtual void setVisibleRecursive(bool _visible);
     
    virtual void attachObject(ObjPtr object, int objectDrawOrder = 0);
    virtual void detachObject(ObjPtr object);
    virtual ObjPtr detachObject(const std::string& objectName);
    virtual void detachAllObjects();
    virtual ObjPtr getAttachedObject(const std::string& objectName);
    virtual int numAttachedObjects();
    
    virtual void setDrawOrder(int _drawOrder);
    virtual int getDrawOrder() const { return drawOrder; }
    
    virtual void setOrigin(const Vector2f& origin);
    virtual void setOrigin(float x, float y);
    
    virtual void setScale(const Vector2f& factors);
    virtual void setScale(float factorX, float factorY);

    virtual void setPosition(const Vector2f& position);
    virtual void setPosition(float x, float y);
    
    virtual void setRotation(const float angle);
    
    virtual void move(const Vector2f& offset);
    virtual void move(float offsetX, float offsetY);
    
    virtual void rotate(const float angle);
    
    virtual void scale(const Vector2f& factor);
    virtual void scale(float factorX, float factorY);
    
    virtual void setOriginRecursive(const Vector2f& origin);
    virtual void setOriginRecursive(float x, float y);
    
    virtual void setScaleRecursive(const Vector2f& factors);
    virtual void setScaleRecursive(float factorX, float factorY);

    virtual void setPositionRecursive(const Vector2f& position);
    virtual void setPositionRecursive(float x, float y);
    
    virtual void setRotationRecursive(const float angle);
    
    virtual void moveRecursive(const Vector2f& offset);
    virtual void moveRecursive(float offsetX, float offsetY);
    
    virtual void rotateRecursive(const float angle);
    
    virtual void scaleRecursive(const Vector2f& factor);
    virtual void scaleRecursive(float factorX, float factorY);

    virtual void update();
    
    virtual void draw(RenderTarget& target, RenderStates states) const override;

    bool operator< (const SceneNode& right)
    {
        return this->drawOrder < right.drawOrder;
    }
};
    
} //namespace ae

#endif //SCENENODE_HPP
