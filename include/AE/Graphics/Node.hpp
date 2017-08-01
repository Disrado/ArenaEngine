#ifndef NODE_HPP
#define NODE_HPP

#include <AE/Graphics/Updatable.hpp>
#include <AE/Graphics/Transformable.hpp>
#include <map>
#include <memory>

namespace ae
{

struct NodeFullTransform
{
    ae::vector2f        posiion;
    const ae::Vector2f& scale;
    const ae::Vector2f& origin;
    float               ratation;

public:
    FullTranfsorm(const ae::Vector2f& position,
		  const ae::Vector2f& scale,
		  const ae::Vector2f& origin,
		  float               angle);
};

class SFML_GRAPHICS_API Node : public Updatable,
			       public Transformbale,
			       public std::enable_shared_from_this<SceneNode>
{
public:
    typedef std::shared_ptr<Node>          NodePtr;
    typedef std::weak_ptr<Node>            ParentPtr;
    typedef std::map<std::string, NodePtr> NodeMap;
   
protected:

    void setParent(NodePtr _parent);
    void removeParent();

    /*
     * Set position, ratotion, origin, scale and visible this node
     * to child(paremeter)
     */
    void setParentNodeTransform();

    /*
     * Return position, ratotion, origin, scale and visible this node
     * to child(paremeter)
     */
    void getFullTransform();    

private:
    std::string name;
    ParentPtr parent;
    NodeMap children;
    bool needUpdateChildren;
    bool needUpdateFromParent;
    
public:
    Node(const std::string&  name,
	 const ae::Vector2f& position = ae::Vector2f(),
	 const ae::Vector2f& scale = ae::Vector2f(1, 1),
	 const ae::Vector2f& origin = ae::Vector2f(),
	 float               rotation = 0.0);

    virtual ~Node();


    static NodePtr create(const std::string&  name,
			  const ae::Vector2f& position = ae::Vector2f(),
			  const ae::Vector2f& scale = ae::Vector2f(1, 1),
			  const ae::Vector2f& origin = ae::Vector2f(),
			  float               rotation = 0.0);    
    
    /** Create new Node, add it as child to this node then return it */
    virtual NodePtr createChild(const std::string& name, int drawOrder);

    virtual void addChild(NodePtr);
    virtual NodePtr getChild(const std::string& _name);

    /* 
     * Remove child from whis node
     * But if pointer to child was saved in other place
     * children of child will not be deleted from children
     */    
    virtual void removeChild(NodePtr childToRemove);
    virtual void removeChild(const std::string& _name);
    virtual void removeChildren();
    
    virtual void rebaseToNewParent(NodePtr newParent);
    virtual void rebaseChildrenToNewParent(NodePtr newParent);
     
    virtual void setName(const std::string& _name) { name = _name; }
    virtual const std::string& getName() const { return name; }

    virtual const NodePtr getParent() const;

    /** Return count of children of this node without grandchildren asf */
    virtual int getChildrenCount() const { return children.size(); }

    /** Return count of children of this node with grandchildren asf */
    virtual int getDescendantCount() const;

    virtual void updateFromParent();
    
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
};
    
} //namespace ae

#endif //SCENENODE_HPP
