#include <SceneNode_Test.hpp>
#include <AE/Graphics.hpp>
#include <memory>
#include <iostream>


bool SceneNode_Test::SceneNodeConstructor_Test()
{
    auto SNode = ae::SceneNode();

    if(SNode.getDrawOrder() == 0
       && SNode.getTag() == ""
       && SNode.getPosition() == ae::Vector2f(0, 0)
       && SNode.getScale() == ae::Vector2f(1, 1)
       && SNode.getOrigin() == SNode.getPosition()
       && SNode.getRotation() == 0.0)
     	return true;
    else {
	std::cerr <<" \n\tIn SceneNode()"
		  << "\n\t\tIncorrect values of default parameters" << std::endl;
	
	return false;
    }
}

bool SceneNode_Test::createChildSceneNode_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();

    auto childNode = SNode->createChildSceneNode();

    if(!childNode) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tIncorrect return value" << std::endl;
	return false;
    }

    if(childNode->getParent() != SNode) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tIncorrect value of parent in child node" << std::endl;
	return false;
    }

    if(SNode->getChildrenCount() == 0) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tError while child edding as child to parent" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::addChild_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();

    auto childNode = SNode->createChildSceneNode();

    SNode->addChild(childNode);

    if(childNode->getParent() != SNode) {
	std::cerr <<" \n\tIn createChildSceneNode()"
		  << "\n\t\tIncorrect value of parent in child node" << std::endl;
	return false;
    }
    
    if(SNode->getChildrenCount() == 0) {
	std::cerr <<" \n\tIn addChild()"
		  << "\n\t\tError while child edding as child to parent" << std::endl;
	return false;
    }

    return true;
}

bool SceneNode_Test::removeChild_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
    auto childNode = SNode->createChildSceneNode();
    childNode->setTag("child_1");
    
    SNode->addChild(childNode);
    SNode->removeChild(childNode);

    SNode->addChild(childNode);
    SNode->removeChild("child_1");
    
    if(childNode->getParent() == SNode) {
     	std::cerr << " \n\tIn removeChild()"
     		  << "\n\t\tIncorrect value of parent in child node" << std::endl;
     	return false;
    }
    
    if(SNode->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while child deleting child from parent" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::removeChildren_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();

    for(int i(0); i < 10; ++i)
	SNode->createChildSceneNode();

    SNode->removeChildren();

    if(SNode->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while deleting childredn from parent" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::destroyChildrenRecursive_Test()
{
    auto SNode = std::make_shared<ae::SceneNode>();
    auto child_1_1 = SNode->createChildSceneNode();
    auto child_1_2 = SNode->createChildSceneNode();

    auto child_2_1 = child_1_1->createChildSceneNode();
    auto child_2_2 = child_1_2->createChildSceneNode();

    auto child_3_1 = child_2_1->createChildSceneNode();
    auto child_3_2 = child_2_2->createChildSceneNode();
    
    auto sprite = std::make_shared<ae::Sprite>();
    child_3_2->attachObject(sprite);

    SNode->destroyChildrenRecursive();

    if(SNode->getChildrenCount() != 0 &&
       child_1_1->getChildrenCount() != 0 &&
       child_1_2->getChildrenCount() != 0 &&
       child_2_1->getChildrenCount() != 0 &&
       child_2_2->getChildrenCount() != 0) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while deleting childredn from parent" << std::endl;
	return false;
    }

    if(child_3_2->detachObject() != nullptr) {
	std::cerr <<" \n\tIn removeChild()"
		  << "\n\t\tError while removing object drom node" << std::endl;
	return false;
    }
    
    return true;
}

bool SceneNode_Test::rebaseToNewParent_Test()
{
    return true;
}

bool SceneNode_Test::rebaseChildrenToNewParent_Test()
{
    return true;
}

bool SceneNode_Test::attachObject_Test()
{
    return true;
}

bool SceneNode_Test::detachObject_Test()
{
    return true;
}

bool SceneNode_Test::set_get_DrawOrder_Test()
{    
    return true;
}

bool SceneNode_Test::set_get_Tag_Test()
{
    return true;
}

bool SceneNode_Test::setOriginRecursive_Test()
{
    return true;
}

bool SceneNode_Test::setScaleRecursive_Test()
{
    return true;
}

bool SceneNode_Test::setPositionRecursive_Test()
{
    return true;
}

bool SceneNode_Test::setRotationRecursive_Test()
{
    return true;
}

bool SceneNode_Test::moveRecursive_Test()
{
    return true;
}

bool SceneNode_Test::rotateRecursive_Test()
{
    return true;
}

bool SceneNode_Test::scaleRecursive_Test()
{
    return true;
}
