#include <SceneNode_Test.hpp>
#include <iostream>

int main()
{
    SceneNode_Test SNodeTest = SceneNode_Test();

    std::cout << std::endl << "Running SceneNodeConstructor_Test:" << std::endl;
    if(SNodeTest.SceneNodeConstructor_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "createChildSceneNode_Test:" << std::endl;
    if(SNodeTest.createChildSceneNode_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "addChild_Test:" << std::endl;
    if(SNodeTest.addChild_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "removeChild_Test:" << std::endl;
    if(SNodeTest.removeChild_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "removeChildren_Test:" << std::endl;
    if(SNodeTest.removeChildren_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "destroyChildrenRecursive_Test:" << std::endl;
    if(SNodeTest.destroyChildrenRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "rebaseToNewParent_Test:" << std::endl;
    if(SNodeTest.rebaseToNewParent_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "rebaseChildrenToNewParent_Test:" << std::endl;
    if(SNodeTest.rebaseChildrenToNewParent_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "attachObject_Test:" << std::endl;
    if(SNodeTest.attachObject_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "detachObject_Test:" << std::endl;
    if(SNodeTest.detachObject_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "set_get_DrawOrder_Test:" << std::endl;
    if(SNodeTest.set_get_DrawOrder_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "set_get_Tag_Test:" << std::endl;
    if(SNodeTest.set_get_Tag_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "setOriginRecursive_Test:" << std::endl;
    if(SNodeTest.setOriginRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "setScaleRecursive_Test:" << std::endl;
    if(SNodeTest.setScaleRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "setPositionRecursive_Test:" << std::endl;
    if(SNodeTest.setPositionRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "setRotationRecursive_Test:" << std::endl;
    if(SNodeTest.setRotationRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "moveRecursive_Test:" << std::endl;
    if(SNodeTest.moveRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "rotateRecursive_Test:" << std::endl;
    if(SNodeTest.rotateRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;

    std::cout << std::endl << "scaleRecursive_Test:" << std::endl;
    if(SNodeTest.scaleRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;
}

