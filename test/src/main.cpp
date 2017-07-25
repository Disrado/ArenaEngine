#include <SceneNode_Test.hpp>
#include <Test.hpp>
#include <memory>
#include <iostream>

int main()
{
    auto test = std::make_unique<Test>();
    test = std::move(std::make_unique<SceneNode_Test>());
    test->beginTests();
}

