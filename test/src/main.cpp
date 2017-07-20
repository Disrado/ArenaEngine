#include <SceneNode_Test.hpp>
#include <iostream>

int testCount = 0;

int main()
{
    SceneNode_Test SNodeTest = SceneNode_Test();

    std::cout << "Running test #" << ++testCount << " (SceneNodeConstructor_Test)" << std::endl;
    if(SNodeTest.SceneNodeConstructor_Test())
	std::cout << "Test #" << testCount << " complete successfully";
    else
	std::cout << "Test #" << testCount << " failed" << std::endl;	
}

