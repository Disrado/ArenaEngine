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

    std::cout << std::endl << "destroyChildrenRecursive_Test_Test:" << std::endl;
    if(SNodeTest.destroyChildrenRecursive_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;
}

