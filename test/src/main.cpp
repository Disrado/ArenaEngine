#include <SceneNode_Test.hpp>
#include <vector>

int main()
{
    std::vector<Test*> testClasses =
    {
	new SceneNode_Test()
	//...
    };

    for(auto itr : testClasses)
	itr->beginTests();

    for(auto &itr : testClasses)
	delete itr;
}

