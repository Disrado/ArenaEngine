#ifndef ROOTSCENENODE_HPP
#define ROOTSCENENODE_HPP

#include <AE/Graphics/SceneNode.hpp>

namespace ae
{

class SFML_GRAPHICS_API RootSceneNode : public SceneNode
{
public:
    typedef std::shared_ptr<RootSceneNode> RootSNodePtr;
    
    RootSceneNode(bool                visible = true,
		  const ae::Vector2f& position = ae::Vector2f(),
		  const ae::Vector2f& scale = ae::Vector2f(1, 1),
		  const ae::Vector2f& origin = ae::Vector2f(),
		  float               rotaion = 0.0);

    void draw(RenderTarget& target, RenderStates states) const;
    
private:
    //* Because RootSceneNode cannot have parent
    using SceneNode::getParent;
    using SceneNode::rebaseToNewParent;
    
    //* Because all children is children of RootSceneNode
    using SceneNode::rebaseChildrenToNewParent;

    //* Because RootSceneNode have constant name "RootSceneNode"
    using SceneNode::setName;

    //* Because RootSceneNode cannot have brothers
    using SceneNode::setDrawOrder;
    using SceneNode::getDrawOrder;

};

} //namesapce ae
    
#endif //ROOTESCENENODE_HPP
