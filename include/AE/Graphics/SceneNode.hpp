#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/TransformableObject.hpp>
#include <AE/Graphics/RenderWindow.hpp>
#include <AE/Graphics/Node.hpp>
#include <unordered_map>
#include <memory>

namespace ae
{

class SFML_GRAPHICS_API SceneNode : public Node,
				    public Drawable,
				    public Transformable,
				    public Updatable,
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
    
    /*
     * Set position, ratotion, origin, scale and visible this node
     * to child(paremeter)
     */
    
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
    

    bool isVisible();
    void setVisible(bool _visible);
    void setVisibleRecursive(bool _visible);
    
    /*
     * Remove child from whis node
     * If pointer to child was saved in other place
     * children of child will be deleted from child children
     * Also destroy* methods detach all objects from children
     */
    void destroyChild(SNodePtr childToRemove);
    void destroyChild(const std::string& _name);
    void destroyChildren();
     
    void attachObject(TObjPtr object);
    void detachObject(TObjPtr object);
    TObjPtr detachObject(const std::string& objectName);
    void detachAllObjects();
    TObjPtr getAttachedObject(const std::string& objectName);
    int numAttachedObjects();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const { return drawOrder; }
    
    void draw(RenderTarget& target, RenderStates states) const override;
};
    
} //namespace ae

#endif //SCENENODE_HPP
