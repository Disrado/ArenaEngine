#ifndef SCENE_NODE_HPP
#define SCENE_NODE_HPP

#include <AE/Graphics/SceneNode.hpp>

class SceneNode_Test
{
public:
    friend class SceneNode;
    
    bool SceneNodeConstructor_Test();
    
    bool createChildSceneNode_Test();
    
    bool addChild_Test();
    
    bool removeChild_Test();

    bool removeChildren_Test();
    bool destroyChildrenRecursive_Test();
    
    bool rebaseToNewParent_Test();
    bool rebaseChildrenToNewParent_Test();
    
    bool attachObject_Test();
    bool detachObject_Test();
    
    bool set_get_DrawOrder_Test();
    bool set_get_Tag_Test();

    bool setOriginRecursive_Test();
    bool setScaleRecursive_Test();
    bool setPositionRecursive_Test();
    bool setRotationRecursive_Test();
    bool moveRecursive_Test();
    bool rotateRecursive_Test();
    bool scaleRecursive_Test();
};
    
#endif //SCENENODE_HPP_TEST
