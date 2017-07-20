#include <SceneNode_Test.hpp>
#include <iostream>

bool SceneNode_Test::SceneNodeConstructor_Test()
{
    auto SNode = ae::SceneNode();

    if(SNode.getDrawOrder() == 0
       && SNode.getTag() == ""
       && SNode.getPosition() == ae::Vector2f(0, 0)
       && SNode.getScale() == ae::Vector2f(1, 1)
       && SNode.getOrigin() == SNode.getPosition()
       && SNode.getRotation() == 0.0 && false)
     	return true;
    else {
	std::cerr << "SceneNodeConstructor_Test failed"
		  <<" \n\tIn SceneNode()"
		  << "\n\t\tIncorrect values of default parameters" << std::endl;
	
	return false;
    }
}

bool createChildSceneNode_Test()
{
    return true;
}

bool addChild_Test()
{
    return true;
}

bool removeChild_Test()
{
    return true;
}

bool removeChildren_Test()
{
    return true;
}

bool destroyChildrenRecursive_Test()
{
    return true;
}

bool rebaseToNewParent_Test()
{
    return true;
}

bool rebaseChildrenToNewParent_Test()
{
    return true;
}

bool attachObject_Test()
{
    return true;
}

bool detachObject_Test()
{
    return true;
}

bool set_get_DrawOrder_Test()
{
    return true;
}

bool set_get_Tag_Test()
{
    return true;
}

bool setOriginRecursive_Test()
{
    return true;
}

bool setScaleRecursive_Test()
{
    return true;
}

bool setPositionRecursive_Test()
{
    return true;
}

bool setRotationRecursive_Test()
{
    return true;
}

bool moveRecursive_Test()
{
    return true;
}

bool rotateRecursive_Test()
{
    return true;
}

bool scaleRecursive_Test()
{
    return true;
}
