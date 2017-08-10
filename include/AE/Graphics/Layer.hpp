#ifndef LAYER_HPP
#define LAYER_HPP

#include <AE/Graphics/RootSceneNode.hpp>
#include <AE/Graphics/Updatable.hpp>
#include <AE/Graphics/Drawable.hpp>
#include <AE/Graphics/Object.hpp>
#include <AE/System/Logger.hpp>
#include <memory>
#include <vector>
#include <unordered_map>


namespace ae
{

class SFML_GRAPHICS_API Layer : public Drawable,
                                public Updatable
{
public:
    typedef std::vector<std::shared_ptr<Object>>         ObjectQueue;
    typedef std::shared_ptr<RootSceneNode>               RootPtr;
    typedef std::unordered_multimap<std::string,
                                    SceneNode::SNodePtr> SceneNodeMap;

public:
    RootPtr rootSceneNode;
    ObjectQueue drawableQueue;
    SceneNodeMap nodesCommonPool;
    
    int drawOrder;
    std::string name;

// public is tmp, only for testing    
public:
    void updateNodesCommonPool();
    void buildNodesCommonPool(std::shared_ptr<SceneNode> node);
    void updateDrawableQueue();
    void buildDrawableQueue(std::shared_ptr<SceneNode> node);
    
public:
    Layer();
    Layer(int _drawOrder, const std::string& _tag);
    
    std::shared_ptr<RootSceneNode> getRootSceneNode();
    std::shared_ptr<SceneNode> getSceneNode(const std::string& name);

    int getNumberAllNodes();
    
    void setName(const std::string& newName);
    const std::string& getName();
    
    void setDrawOrder(int _drawOrder);
    int getDrawOrder() const;
    
    void clear();

    void update();
    void draw(RenderTarget& target, RenderStates states) const override;
};
    
} //namespace ae
    
#endif //LAYER_HPP
