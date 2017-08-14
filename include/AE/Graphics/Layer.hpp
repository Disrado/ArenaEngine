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
    typedef std::shared_ptr<Layer>                       LayerPtr;
    typedef std::vector<std::shared_ptr<Object>>         ObjectQueue;
    typedef std::unordered_multimap<std::string,
                                    SceneNode::SNodePtr> SceneNodeMap;

private:
    RootSceneNode::RootSNodePtr rootSceneNode;
    SceneNodeMap nodesCommonPool;
    ObjectQueue drawableQueue;
    std::string name;
    bool onScene;    
    int drawOrder;

    void updateNodesCommonPool();
    void buildNodesCommonPool(SceneNode::SNodePtr node);
    void updateDrawableQueue();
    void buildDrawableQueue(SceneNode::SNodePtr node);
    
public:
    Layer(const std::string& _name, int _drawOrder = 0);

    static LayerPtr create(const std::string& name, int drawOrder);
    
    RootSceneNode::RootSNodePtr getRootSceneNode();
    SceneNode::SNodePtr getSceneNode(const std::string& name);
    int getNumberAllNodes();
    
    void setName(const std::string& newName) { name = newName; }
    const std::string& getName() { return name; }

    void setDrawOrder(int newDrawOrder) { drawOrder = newDrawOrder; }
    int getDrawOrder() const { return drawOrder; }

    // Don't use this methods! It is for Scene class
    void notifyAddedOnScene() { onScene = true; }
    void notifyRemovedFromScene() { onScene = false; }

    bool isOnScene() { return onScene; }
    
    void clear();

    void update() override;
    void draw(RenderTarget& target, RenderStates states) const override;
};
    
} //namespace ae
    
#endif //LAYER_HPP
