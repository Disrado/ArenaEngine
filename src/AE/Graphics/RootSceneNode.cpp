#include <AE/Graphics/RootSceneNode.hpp>

namespace ae
{

namespace
{
    typedef ae::RootSceneNode::ObjectQueue ObjectQueue; 
}
    
RootSceneNode::RootSceneNode(bool                visible,
			     const ae::Vector2f& position,
			     const ae::Vector2f& scale,
			     const ae::Vector2f& origin,
			     float               rotation)
    : SceneNode("RootSceneNode",
		0,
		visible,
		position,
		scale,
		origin,
		rotation) 
{ }

void RootSceneNode::draw(RenderTarget& target, RenderStates states) const
{
    SceneNode::draw(target, states);
}
    
} //namespace ae
