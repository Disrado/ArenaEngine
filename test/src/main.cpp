#include <SceneNode_Test.hpp>
#include <iostream>

int main()
{
    SceneNode_Test SNodeTest = SceneNode_Test();

    std::cout << "Running SceneNodeConstructor_Test:" << std::endl;
    if(SNodeTest.SceneNodeConstructor_Test())
	std::cout << "Test complete successfully" << std::endl;
    else
	std::cout << "Fail" << std::endl;	
}

