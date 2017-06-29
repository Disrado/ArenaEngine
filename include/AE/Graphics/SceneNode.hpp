#ifndef SCENENODE_HPP
#define SCENENODE_HPP

#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Transformable.hpp>
#include <vector>

typedef std::shared_ptr<SceneNode> sptrNode;

namespace ae
{

class SceneNode : public Drawable, public Transformable
{    
private:
    int drawOrder;
    std::string tag;
    sptrNode parent;
    std::vector<sptrNode> children;
    
protected:
    Node();
    virtual ~Node() {}
    
    void setParent(sptrNode _parent);
    sptrNode getParent() const;
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;
        
    void setTag(const std::string& _tag);
    std::string getTag() const;

    void setChildren(const std::vector<sptrNode>& _children);
    std::vector<sptrNode> getChildren() const;
    
    void removeAllChildren();

    void addChild(sptrNode child, int _drawOrder = 0, const std::string& _tag = "");
};

} //namespace ae


#endif //SCENENODE_HPP
