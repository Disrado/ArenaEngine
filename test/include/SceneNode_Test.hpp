#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <AE/Graphics/SceneNode.hpp>
#include <Test.hpp>

class SceneNode_Test : public Test
{
private:
    void SceneNodeConstructor_Test();
    void createChildSceneNode_Test();
    void addChild_Test();
    void removeChild_Test();
    void removeChildren_Test();
    void destroyChildrenRecursive_Test();
    void rebaseToNewParent_Test();
    void rebaseChildrenToNewParent_Test();
    void attachObject_Test();
    void detachObject_Test();
    void set_get_DrawOrder_Test();
    void set_get_Tag_Test();
    void setOriginRecursive_Test();
    void setScaleRecursive_Test();
    void setPositionRecursive_Test();
    void setRotationRecursive_Test();
    void moveRecursive_Test();
    void rotateRecursive_Test();
    void scaleRecursive_Test();

public:
    void beginTests() override;
};
    
#endif //SCENENODE_HPP_TEST
