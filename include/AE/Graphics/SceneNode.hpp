#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Transformable.hpp>
#include <map>
#include <memory>
#include <vector>

class SceneNode;

typedef std::shared_ptr<SceneNode> sptrNode;

namespace ae
{
	
class SceneNode : public Drawable, public Transformable
{    
private:
    int drawOrder;
    std::string tag;
    sptrNode parent;
    std::map<int, sptrNode> children;
    std::map<int, Drawable*> attachedObjects;
    
protected:
    SceneNode();
    virtual ~SceneNode() {}
    
    void setParent(sptrNode _parent);
    sptrNode getParent() const;
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;
    
    void setTag(const std::string& _tag);
    const std::string& getTag() const;
    
    void setChildren(const std::map<int, sptrNode>& children);
    const std::map<int, sptrNode>& getChildren() const;
    
    void removeAllChildren();
    
    void addChildNode(sptrNode child, int _drawOrder = 0, const std::string& _tag = "");

    void attachObject(std::shared_ptr<Drawable> object, int objectID);
    void detachObject(int objectID);
};
	
} //namespace ae


#endif //SCENENODE_HPP
